/**
 * @file	memory.c
 * @author	Sunil Beta <betasam@gmail.com>
 * @date	2005-2012
 * @brief	provide memory allocation functions.
 * @details
 * Provides support for memory allocation that can be
 * made POSIX independent with an alternate malloc 
 * algorithm that is being planned.
 *
 * @warning	needs stdlib because of default dependency
 *		on malloc and related functions.
 */

#include <bkconfig.h>

#ifdef CONFIG_BK_SYS_MEMORY

/* @remark Standard Includes : required for malloc() */
#include <stdlib.h>
#include <unistd.h>

/* @remark bkit Includes */
#include <ops.h>
#include <btypes.h>

#include <memory.h>

static t_ptr ptr_track[ MEMORY_TRACKER_SIZE ];
static t_u32 ptr_sizes[ MEMORY_TRACKER_SIZE ];

static t_u32 memory_tracker_loc = ZERO;

static t_memory_calls mc;
static t_bool mem_init_state = ZERO;
static t_u32 total_memory_allocated = ZERO;
static t_bool mem_ctl_lock = false;

/**
 * @fn     _bk_mem_lock()
 * @brief  statement to lock internal memory manager
 * @remark binary semaphore
 */
void _bk_mem_lock(void)
{
  while( true == mem_ctl_lock )
    {
      usleep( BKIT_MEM_SLEEP_USECS );
    }
  mem_ctl_lock = true;
  return;
}

/**
 * @fn	  _bk_mem_unlock()
 * @brief statement to unlock internal memory manager
 * @remark binary semaphore
 */
void _bk_mem_unlock(void)
{
  mem_ctl_lock = false;
  return;
}

/**
 * @fn void mem_init( t_memory_calls *imc )
 * @brief initialises all needed memory callbacks
 * @details
 * Initialises all the needed memory callbacks to
 * the ones provided by stdlib malloc.
 *
 * @return Nothing.
 */
void mem_init( t_memory_calls *imc )
{
  /* assign function pointers */
  imc->malloc  = &malloc;
  imc->calloc  = &calloc;
  imc->realloc = &realloc;
  imc->free    = &free;

  /* initiate state, usage counters */
  imc->alloc_used = 0;
  mem_init_state  = 1;
}


/**
 * @fn	_bk_mem_enable()
 * @brief  statement to enable memory state
 * @remark debug or internal use only
 */
inline void _bk_mem_enable( )
{
  _bk_mem_lock();
  mem_init_state = 1;
  _bk_mem_unlock();
  return;
}

/**
 * @fn	_bk_mem_disable()
 * @brief  statement to disable memory state
 * @remark debug or internal use only
 */
inline void _bk_mem_disable( )
{
  _bk_mem_lock();
  mem_init_state = 0;
  _bk_mem_unlock();
  return;
}

/**
 * @fn t_ptr mem_alloc( t_u32 ui_bytes )
 * @param ui_bytes number of bytes to allocate.
 * @brief allocates memory like malloc.
 * @details
 * This function is to mimic malloc but has the 
 * capability to use a different callback for
 * allocating memory.
 *
 * @return pointer to allocated memory heap
 *	   or NULL on failure.
 */
t_ptr mem_alloc( t_u32 ui_bytes )
{
  if( ZERO == mem_init_state )
    {
      mem_init( &mc );
    }

  _bk_mem_lock();
  ptr_track[ memory_tracker_loc ] = mc.malloc( (size_t) ui_bytes );
  if( (t_ptr)0 == ptr_track[ memory_tracker_loc ] )
    {
      _bk_mem_unlock();
      return((t_ptr)0);
    }
  ptr_sizes[ memory_tracker_loc ] = ui_bytes;
  mc.alloc_used++;
  memory_tracker_loc++;
  if( memory_tracker_loc >= MEMORY_TRACKER_SIZE ) /* @remark wrap-around */
    {
      memory_tracker_loc = 0;
      total_memory_allocated += ui_bytes;
      _bk_mem_unlock();
      return( ptr_track[ MEMORY_TRACKER_SIZE - 1 ] );
    }
  total_memory_allocated += ui_bytes;
  _bk_mem_unlock();

  return( ptr_track[ memory_tracker_loc-1 ] );  
}

/**
 * @fn t_ptr mem_clearalloc( t_u32 ui_bytes )
 * @param ui_bytes number of bytes to allocate.
 * @brief same functionality as calloc.
 * @details
 * This function does exactly the same as calloc
 * except that the callback can be changed and
 * all allocations upto [MEMORY_TRACKER_SIZE] 
 * are tracked for automatic memory free-up.
 *
 * @return pointer to cleared and allocated
 *	   memory heap or NULL on failure.
 */
t_ptr mem_clearalloc( t_u32 ui_bytes )
{

  if( 0 == mem_init_state )
    {
      mem_init( &mc );
    }
  
  _bk_mem_lock();
  ptr_track[ memory_tracker_loc ] = mc.calloc( sizeof(t_u8), (size_t) ui_bytes );
  if( NULL == ptr_track[ memory_tracker_loc ] )
    {
      _bk_mem_unlock();
      return(NULL);
    }
  ptr_sizes[ memory_tracker_loc ] = ui_bytes;
  mc.alloc_used++;
  memory_tracker_loc++;
  if( memory_tracker_loc >= MEMORY_TRACKER_SIZE ) /* @remark wrap-around */
    {

      memory_tracker_loc = 0;
      mc.alloc_used = memory_tracker_loc;

      total_memory_allocated += ui_bytes;
      _bk_mem_unlock();
      return( ptr_track[ MEMORY_TRACKER_SIZE - 1 ] );
    }
  _bk_mem_unlock();

  return( ptr_track[ memory_tracker_loc-1 ] );  
}

/**
 * @fn void mem_free( t_ptr ptr_mem )
 * @param ptr_mem pointer of the location to be freed.
 * @brief same as free(ptr) with callback support 
 * @details
 * This function frees up memory similar to free() except
 * that it cannot free memory if the memory callbacks were
 * changed after allocating memory from a different 
 * algorithm.
 *
 * @return Nothing even on failure.
 */
void mem_free( t_ptr ptr_mem )
{
  int traverse_loc = 0;
  unsigned long ul_ptr_one, ul_ptr_two;

  if( 0 == mem_init_state )
    {
      /* memory not initialised */
      return;
    }

  _bk_mem_lock();
  if( 0 >= mc.alloc_used )
    {      
      _bk_mem_unlock();
      traverse_loc = (int)0xf17eb17d;
      return;
    }

  if( 0 == memory_tracker_loc )
    {
      mc.alloc_used = 0;
      ul_ptr_one = (t_ul)ptr_track[ 0 ];
      if( (t_ul)ptr_mem == ul_ptr_one )
	{
	  mc.free( (t_ptr)ul_ptr_one );
	  ptr_track[ 0 ] = (t_ptr)NULL;
	}
      _bk_mem_unlock();
      return;
    }
  
  for( traverse_loc = 0; traverse_loc < memory_tracker_loc; traverse_loc++ )
    {
      if( ptr_track[ traverse_loc ] == ptr_mem )
	{
	  ul_ptr_one = (t_ul) ptr_track[ traverse_loc ];
	  ul_ptr_two = (t_ul) ptr_track[ memory_tracker_loc - 1 ];
	  _OPS_SWAP( ul_ptr_one, ul_ptr_two );

	  ptr_track[ traverse_loc ] = (t_ptr)ul_ptr_one;
	  ptr_track[ memory_tracker_loc - 1 ] = (t_ptr)ul_ptr_two;
	  if( (0 != ul_ptr_one) && (ul_ptr_one != ul_ptr_two) )
	    {
	      ptr_track[ traverse_loc ] = (t_ptr)NULL;
	      mc.free( ptr_mem );
	      mc.alloc_used--;
	      memory_tracker_loc--;	  
	      ptr_track[ memory_tracker_loc - 1 ] = (t_ptr)0;
	    }
	}
    }
  _bk_mem_unlock();

  traverse_loc = (int)0xdeadbeef;	/* debug watch */
  return;
}


#if defined(BKIT_DEBUG_MODE)
/**
 * @fn mem_stat
 * @brief status of memory allocation tracking
 * @remark debug function only, no use otherwise
 */
void mem_stat( void )
{
  int tidx = 0;

  if( 0 == mem_init_state )
    {
      printf("%s: caution memory state [ %d ]\n", __FUNCTION__, mem_init_state );
    }

  _bk_mem_lock();
  for( tidx = 0; tidx < memory_tracker_loc; tidx++ )
  {
    printf("%s: [%d] memory pointer [ 0x%08X ]\n", __FUNCTION__, tidx, ptr_track[ tidx ]);
  }
  printf("%s: total memory: %u bytes [%d]\n", __FUNCTION__, total_memory_allocated, memory_tracker_loc );
  _bk_mem_unlock();

  return;
}
#endif	/* BKIT_DEBUG_MODE */

/**
 * @fn t_ptr mem_realloc( t_ptr ptr_mem, t_u32 ui_bytes )
 * @param ptr_mem pointer to memory whose allocated heapsize is
 *        to be changed.
 * @param ui_bytes new number of bytes to be allocated.
 * @details
 * This function is similar to realloc. It can also use a different
 * callback for reallocation. This is to manage memory allocation
 * better inside an application.
 *
 * @warning does not work if callback was changed after first
 *          allocation.
 *
 * @return Pointer to the re-allocated heap which can be different
 *         if the callback was changed after the first allocation 
 *         on success and NULL on failure.
 *
 */
t_ptr mem_realloc( t_ptr ptr_mem, t_u32 ui_bytes )
{
  if( ZERO == mem_init_state )
    {
      mem_init( &mc );
    }

  return( realloc( ptr_mem, ui_bytes ) );
}

/**
 * @fn void mem_gc( void )
 * @brief frees upto 256 memory allocations that were done
 *        using betakit's memory allocating calls.
 * @details
 * This is just to free up to a maximum of 256 memory locations
 * if they were allocated with the same kit. The purpose is
 * trivial and is to take care of memory that was not properly
 * freed. 
 *
 * bug that caused double free has been fixed.
 *
 * @return Nothing
 */
void mem_gc( void )
{
  register t_u32 traverse_loc = 0;
  register t_u32 traverse_max = 0;
 

  if( 0 == mem_init_state )
    {
      return;
    }

  _bk_mem_lock();
  do {

  traverse_max = memory_tracker_loc;
  if( MEMORY_TRACKER_SIZE > traverse_max )
    {
      traverse_max = MEMORY_TRACKER_SIZE;
    }

  if( 0 >= mc.alloc_used )
    {      
      _bk_mem_unlock();
      traverse_loc = (int)0xf17eb17d;
      return;
    }

  for( traverse_loc = (traverse_max-1); 
       (traverse_loc >= 0) && (mc.alloc_used > 0); 
       traverse_loc-- )
    {
      mc.free( ptr_track[ traverse_loc ] );
      ptr_track[ traverse_loc ] = (t_ptr)NULL;
      mc.alloc_used--;
    }

  memory_tracker_loc = 0;
  } while(0);
  _bk_mem_unlock();

  return;
}

/**
 * @fn t_s32 mem_alloc_count( void )
 * @brief returns total memory allocated through mem_* so far.
 * @return memory allocated so far
 * @details
 * This is just to expose an internal variable in the library
 * through a hook. I am not very sure you can access these 
 * symbols although lint has been quite strict in teaching me
 * that they can indeed be accessed unless I throw them "static."
 */
t_s32 mem_alloc_count( void )
{
  register t_u32 u_count = 0;

  _bk_mem_lock();
  u_count = mc.alloc_used;
  _bk_mem_unlock();

  return( u_count );
}


/**
 * @fn t_u32 mem_copy( t_ptr dest_ptr, t_ptr src_ptr, t_u32 bytes_to_copy )
 * @brief copies source to destination treating memory in bytes
 * @detail 
 * This should be optimised to handle variables of register size.
 *
 * @return bytes copied
 */
t_u32 mem_copy( t_ptr dest_ptr, t_ptr src_ptr, t_u32 u_bytes_to_copy )
{
  t_u32 u_words_to_copy = 0;
  t_u32 u_bytes_copied  = 0;
  t_u32 u_words_copied  = 0;
  t_u32 u_scratch;
  
  if( (0 == dest_ptr) || (0 == src_ptr) )
    {
      return( u_bytes_copied );
    }

  u_scratch       = u_bytes_to_copy;
  u_words_to_copy = u_bytes_to_copy / sizeof(t_word);

  u_bytes_to_copy = u_scratch % sizeof( t_word );

  while( u_words_copied++ < u_words_to_copy )
    {
      *((t_word*)(dest_ptr++)) = *((t_word*)(src_ptr++));
    }

  while( u_bytes_copied++ < u_bytes_to_copy )
    {
      *((t_byte*)(dest_ptr++)) = *((t_byte*)(src_ptr++));
    }

  u_scratch = u_bytes_copied + (sizeof(t_word) * u_words_copied);

  return( u_scratch );
}

/**
 * @fn void mem_changecalls( t_memory_calls *new_calls )
 * @param new_calls Pointer to structure containing calls to a 
 *        memory allocator different from stdlib's malloc.
 * @brief This is to allow the use of a new memory allocator.
 * @details
 * This is to exercise the main purpose of this memory 
 * allocation wrapper. This function allows all primary
 * memory allocation services provided by the "bsys" library
 * to be changed to a new allocator.
 *
 * @return None
 * @warning No warranties on memory allocation after this
 *          hijack as the functions are never checked up.
 */
void mem_changecalls( t_memory_calls *new_calls )
{
  t_s32 err_check = 0;
  mem_gc();

  _bk_mem_lock();
  do {
  err_check += (0 != new_calls->malloc) ? (t_s32)(mc.malloc = new_calls->malloc) : 0 ;
  err_check += (0 != new_calls->calloc) ? (t_s32)(mc.calloc = new_calls->calloc) : 0 ;
  err_check += (0 != new_calls->free)   ? (t_s32)(mc.free   = new_calls->free  ) : 0 ;
  err_check += (0 != new_calls->realloc)? (t_s32)(mc.realloc= new_calls->realloc): 0 ;
  } while(0);
  _bk_mem_unlock();

  return;
}

#endif	/*CONFIG_BK_SYS_MEMORY */
/* @remark end of file "memory.c" */
