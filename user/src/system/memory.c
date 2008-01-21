/**
 * @file	memory.c
 * @author	Sunil Beta <betasam@gmail.com>
 * @brief	provide memory allocation functions.
 *
 * Provides support for memory allocation that can be
 * made POSIX independent with an alternate malloc 
 * algorithm that is being planned.
 *
 * @warning	needs stdlib because of default dependency
 *		on malloc and related functions.
 */

/*
 * Copyright (c) 2005-2008 Sunil Beta Baskar <betasam@gmail.com>
 */

/* Standard Includes */
#include <stdlib.h>

/* bkit Includes */
#include <ops.h>
#include <btypes.h>

#include <memory.h>

static t_ptr ptr_track[ MEMORY_TRACKER_SIZE ];
static t_u32 memory_tracker_loc = 0;

static t_memory_calls mc;
static t_bool mem_init_state = 0;
static t_u32 total_memory_allocated = 0;

/**
 * @fn void mem_init( t_memory_calls *imc )
 * @brief initialises all needed memory callbacks
 *
 * Initialises all the needed memory callbacks to
 * the ones provided by stdlib malloc.
 *
 * @return Nothing.
 */
void mem_init( t_memory_calls *imc )
{
  imc->malloc = &malloc;
  imc->calloc = &calloc;
  imc->realloc = &realloc;
  imc->free = &free;
  mem_init_state = 1;
}

/**
 * @fn t_ptr mem_alloc( t_u32 ui_bytes )
 * @param ui_bytes number of bytes to allocate.
 * @brief allocates memory like malloc.
 *
 * This function is to mimic malloc but has the 
 * capability to use a different callback for
 * allocating memory.
 *
 * @return pointer to allocated memory heap
 *	   or NULL on failure.
 */
t_ptr mem_alloc( t_u32 ui_bytes )
{
  if( 0 == mem_init_state )
    {
      mem_init( &mc );
    }

  ptr_track[ memory_tracker_loc ] = mc.malloc( (size_t) ui_bytes );
  if( (t_ptr)0 == ptr_track[ memory_tracker_loc ] )
    {
      return((t_ptr)0);
    }
  memory_tracker_loc++;
  if( memory_tracker_loc >= MEMORY_TRACKER_SIZE ) /* wrap-around */
    {
      memory_tracker_loc = 0;
      total_memory_allocated += ui_bytes;
      return( ptr_track[ MEMORY_TRACKER_SIZE - 1 ] );
    }

  total_memory_allocated += ui_bytes;
  return( ptr_track[ memory_tracker_loc-1 ] );  
}

/**
 * @fn t_ptr mem_clearalloc( t_u32 ui_bytes )
 * @param ui_bytes number of bytes to allocate.
 * @brief same functionality as calloc.
 *
 * This function does exactly the same as calloc
 * except that the callback can be changed and
 * all allocations upto 256 are tracked for 
 * automatic memory free-up.
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

  ptr_track[ memory_tracker_loc ] = mc.calloc( 1, (size_t) ui_bytes );
  if( (t_ptr)0 == ptr_track[ memory_tracker_loc ] )
    {
      return((t_ptr)0);
    }
  memory_tracker_loc++;
  if( memory_tracker_loc >= MEMORY_TRACKER_SIZE ) /* wrap-around */
    {
      memory_tracker_loc = 0;
      total_memory_allocated += ui_bytes;
      return( ptr_track[ MEMORY_TRACKER_SIZE - 1 ] );
    }
  return( ptr_track[ memory_tracker_loc-1 ] );  
}

/**
 * @fn void mem_free( t_ptr ptr_mem )
 * @param ptr_mem pointer of the location to be freed.
 * @brief same as free(ptr) with callback support 
 *
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
      mem_init( &mc );
    }

  if( 0 == memory_tracker_loc )
    {
      memory_tracker_loc++;
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
	  if( 0 != ul_ptr_one )
	    {
	      mc.free( (t_ptr)ul_ptr_one );
	    }
	  ptr_track[ memory_tracker_loc - 1 ] = (t_ptr)0;
	  memory_tracker_loc--;	  
	}
    }
}

/**
 * @fn t_ptr mem_realloc( t_ptr ptr_mem, t_u32 ui_bytes )
 * @param ptr_mem pointer to memory whose allocated heapsize is
 *        to be changed.
 * @param ui_bytes new number of bytes to be allocated.
 *
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
  if( 0 == mem_init_state )
    {
      mem_init( &mc );
    }

  return( realloc( ptr_mem, ui_bytes ) );
}

/**
 * @fn void mem_gc( void )
 * @brief frees upto 256 memory allocations that were done
 *        using betakit's memory allocating calls.
 *
 * This is just to free up to a maximum of 256 memory locations
 * if they were allocated with the same kit. The purpose is
 * trivial and is to take care of memory that was not properly
 * freed.
 *
 * @return Nothing
 */
void mem_gc( void )
{
  t_u32 traverse_loc = 0;

  if( 0 == mem_init_state )
    {
      mem_init( &mc );
    }

  for( traverse_loc = 0; traverse_loc < memory_tracker_loc; traverse_loc++ )
    {
      if( (t_ptr)0 != (t_ptr)ptr_track[traverse_loc] )
	{
	  mc.free( ptr_track[ traverse_loc ] );
	}
    }
}

/**
 * @fn t_s32 mem_alloc_count( void )
 * @brief returns total memory allocated through mem_* so far.
 * @return memory allocated so far
 *
 * This is just to expose an internal variable in the library
 * through a hook. I am not very sure you can access these 
 * symbols although lint has been quite strict in teaching me
 * that they can indeed be accessed unless I throw them "static."
 */
t_s32 mem_alloc_count( void )
{
  return( memory_tracker_loc );
}


/**
 * @fn t_u32 mem_copy( t_ptr dest_ptr, t_ptr src_ptr, t_u32 bytes_to_copy )
 * @brief copies source to destination treating memory in bytes
 * 
 * This should be optimised to handle variables of register size.
 *
 * @return bytes copied
 */
t_u32 mem_copy( t_ptr dest_ptr, t_ptr src_ptr, t_u32 u_bytes_to_copy )
{
  t_u32 u_bytes_copied = 0;
  
  if( (0 == dest_ptr) || (0 == src_ptr) )
    {
      return( u_bytes_copied );
    }

  while( u_bytes_copied < u_bytes_to_copy )
    {
      *((t_byte*)(dest_ptr++)) = *((t_byte*)(src_ptr++));
      u_bytes_copied++;
    }
  return( u_bytes_copied );
}

/**
 * @fn void mem_changecalls( t_memory_calls *new_calls )
 * @param new_calls Pointer to structure containing calls to a 
 *        memory allocator different from stdlib's malloc.
 * @brief This is to allow the use of a new memory allocator.
 * 
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

  err_check = (0 != new_calls->malloc) ? (t_s32)(mc.malloc = new_calls->malloc) : 0 ;
  err_check = (0 != new_calls->calloc) ? (t_s32)(mc.calloc = new_calls->calloc) : 0 ;
  err_check = (0 != new_calls->free)   ? (t_s32)(mc.free   = new_calls->free  ) : 0 ;
  err_check = (0 != new_calls->realloc)? (t_s32)(mc.realloc= new_calls->realloc): 0 ;

}

/* end of file */
