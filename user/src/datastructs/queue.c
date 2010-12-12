/**
 * @file queue.c
 * @author Sunil Beta Baskar <betasam@gmail.com>
 * @date 2010-2012
 * @brief  Implementation of a Queue
 *
 * @todo Report memory allocation errors
 * @todo for functions returning pointers
 */

#ifdef _BKIT_QUEUE_DEBUG
#include <stdio.h>
#endif

/* @remark betakit includes */
#include <btypes.h>
#include <berror.h>
#include <memory.h>

#include <queue.h>

t_bool queue_wrap=false;

/**
 * @fn t_queue_ptr queue_init( t_u32 queue_depth, t_bool circular )
 * @brief initialise a queue and allocate memory
 * @return pointer to new queue on success, NULL on failure
 */
t_queue_ptr queue_init( t_u32 queue_depth, t_bool circular )
{
  t_queue_ptr new_queue=NULL;

  new_queue = (t_queue*)mem_alloc(sizeof(t_queue));
  if( NULL == new_queue )
    {
      return(NULL);		
      /**
       * @TODO Report memory allocation error 
       */
    }

  new_queue->queue_flags = (BKIT_QUEUE_EMPTY);
  new_queue->queue_flags |= BKIT_QUEUE_UNDEFINED;
  if( true == circular )
    {
      new_queue->queue_flags |= (BKIT_QUEUE_CIRCULAR);
    }

  queue_depth = (queue_depth > 0) ? queue_depth : BKIT_MIN_QUEUE_DEPTH;

  new_queue->queue_base = (t_ptr)mem_alloc( sizeof(t_ptr) * queue_depth );

  if( NULL == new_queue->queue_base )
    {
      return(new_queue);	
      /**
       * @TODO Report memory allocation error 
       */
    }

  queue_depth--;
  new_queue->queue_end  = new_queue->queue_base;
  new_queue->queue_end += queue_depth;
  /**
   * @WARNING Pointer Arithmetic 
   */
  new_queue->queue_head = new_queue->queue_base;
  new_queue->queue_tail = new_queue->queue_base;

  new_queue->queue_flags &= ~(BKIT_QUEUE_UNDEFINED);
  new_queue->queue_flags |= BKIT_QUEUE_READY;

  return(new_queue);
}


/**
 * @fn t_s32 queue_reset( t_queue_ptr queue_ptr )
 * @brief reset queue to be empty
 * @param queue_ptr is a pointer, type t_queue_ptr
 * @return 0 on success, -ve on failure
 */
inline t_s32 queue_reset( t_queue_ptr queue_ptr )
{
  t_s32 retval = ZERO;

  if( NULL == queue_ptr )
    {
      return( retval = -(BERR_NOMEM) );
    }
  queue_ptr->queue_flags = 0;
  if( NULL != queue_ptr->queue_base )
    {
      queue_ptr->queue_tail = queue_ptr->queue_base;
      queue_ptr->queue_head = queue_ptr->queue_base;
      queue_ptr->queue_flags |=  (BKIT_QUEUE_READY);
      queue_ptr->queue_flags |=  (BKIT_QUEUE_EMPTY);
    }
  else
    {
      queue_ptr->queue_flags |= (BKIT_QUEUE_UNDEFINED);
      retval = -(BERR_INVALID);
    }

  return( retval );
}


/**
 * @fn t_u8 queue_status( t_queue_ptr queue_ptr )
 * @param queue_ptr is a pointer, type t_queue_ptr
 * @brief gives the status flags of the queue
 * @return status flags always
 */
t_u8 queue_status( t_queue_ptr queue_ptr )
{
  t_u8 retval = ZERO;

  return( retval = queue_ptr->queue_flags );
}


/**
 * @fn t_void queue_clean( t_queue_ptr queue_ptr )
 * @param queue_ptr pointer to a queue, of type t_queue_ptr
 * @brief clean a queue passed by queue_ptr
 * @return none, no errors are reported
 */
t_void queue_clean( t_queue_ptr queue_ptr )
{

  if( NULL == queue_ptr )
    {
      return;
    }

  if( (BKIT_QUEUE_UNDEFINED & queue_ptr->queue_flags) == BKIT_QUEUE_UNDEFINED )
    {
      return;
    }

  queue_ptr->queue_flags |= BKIT_QUEUE_UNDEFINED;

  mem_free( queue_ptr->queue_base );
  queue_ptr->queue_base = (t_ptr)0; 
  /**
   * @WARNING NULL pointer assignment 
   */

  {
  queue_ptr->queue_end = queue_ptr->queue_head = 
    queue_ptr->queue_tail = queue_ptr->queue_base;
  }
  
}

/**
 * @fn t_s32 queue_add( t_queue_ptr queue_ptr, t_ptr data_ptr )
 * @brief add item to queue passed as data_ptr
 * @param queue_ptr queue to which data will be added
 * @param data_ptr  pointer pointing to data added to queue
 */
t_s32 queue_add( t_queue_ptr queue_ptr, t_ptr data_ptr )
{
  t_s32 retval = ZERO;

  /* @remark deal with null pointers and invalid or cleaned queues */
  if( NULL == queue_ptr )
    {
      return(retval = -(BERR_NOMEM));
    }
  if( (BKIT_QUEUE_UNDEFINED & queue_ptr->queue_flags) == BKIT_QUEUE_UNDEFINED )
    {
      return(retval = -(BERR_INVALID));
    }

  /* @remark deal with filled queue */
  if( (BKIT_QUEUE_FULL & queue_ptr->queue_flags) == BKIT_QUEUE_FULL )
    {
      return(retval = -(BERR_NOSPACE));
    }

  /* @remark lock queue or refuse to add */
  if( (BKIT_QUEUE_READY == (BKIT_QUEUE_READY & queue_ptr->queue_flags)) )
    {
      queue_ptr->queue_flags &= ~(BKIT_QUEUE_READY);
    }
  else
    {
      return(retval = -(BERR_CRASH));
    }

  /* @remark deal with empty queue */
  if( (BKIT_QUEUE_EMPTY & queue_ptr->queue_flags) == BKIT_QUEUE_EMPTY )
    {
      queue_ptr->queue_tail = queue_ptr->queue_base;
      queue_ptr->queue_flags |=  (BKIT_QUEUE_READY);
      queue_ptr->queue_flags &= ~(BKIT_QUEUE_EMPTY);
    }

  *(queue_ptr->queue_tail) = data_ptr;

#ifdef _BKIT_QUEUE_DEBUG
  printf("%s: tail [0x%08x] = %s : base [0x%08x] : end [0x%08x] : head [0x%08x]\n", __FUNCTION__, 
	 queue_ptr->queue_tail, (t_str)*(queue_ptr->queue_tail),  queue_ptr->queue_base, queue_ptr->queue_end, 
	 queue_ptr->queue_head ); 
#endif

  queue_ptr->queue_tail++;	
  /**
   * @WARNING pointer arithmetic 
   */

  /* @remark manage tail of queue */
  if( queue_ptr->queue_tail > queue_ptr->queue_end )
    {
      if( BKIT_QUEUE_CIRCULAR == (queue_ptr->queue_flags & BKIT_QUEUE_CIRCULAR) )
	{
	  queue_ptr->queue_tail = queue_ptr->queue_base;
	}
      else
	{
	  queue_ptr->queue_tail  = queue_ptr->queue_end;
	  queue_ptr->queue_flags |= (BKIT_QUEUE_FULL);
	}
    }


  /* @remark unlock queue */
  queue_ptr->queue_flags |=  (BKIT_QUEUE_READY);
  return( retval );
}


/**
 * @fn t_ptr queue_get( t_queue_ptr queue_ptr )
 * @brief gets an item from the queue
 * @param queue_ptr pointer to the queue, type t_queue_ptr
 */
t_ptr queue_get( t_queue_ptr queue_ptr )
{
  t_ptr ret_item = NULL;

  /* @remark deal with errors */
  if( NULL == queue_ptr )
    {
      return( ret_item = NULL ); 
      /**
       * @TODO Report error 
       */
    }

  if( ((BKIT_QUEUE_EMPTY & queue_ptr->queue_flags) == BKIT_QUEUE_EMPTY) ||
      (BKIT_QUEUE_UNDEFINED & queue_ptr->queue_flags) == BKIT_QUEUE_UNDEFINED )
    {
      return( ret_item = NULL ); 
      /**
       * @TODO Report error 
       */
    }

  /* @remark lock queue or refuse to get */
  if( (BKIT_QUEUE_READY == (BKIT_QUEUE_READY & queue_ptr->queue_flags)) )
    {
      queue_ptr->queue_flags &= ~(BKIT_QUEUE_READY);
    }
  else
    {
      return(ret_item = NULL);
    }


  /* @remark read from head of queue */
  ret_item = (t_ptr)*(queue_ptr->queue_head);

#ifdef _BKIT_QUEUE_DEBUG
  printf("%s: head [0x%08x] = %s : base [0x%08x] : end [0x%08x] : tail [0x%08x]\n", __FUNCTION__, 
	 queue_ptr->queue_head, (t_str)ret_item, queue_ptr->queue_base, queue_ptr->queue_end, 
	 queue_ptr->queue_tail ); 
#endif

  queue_ptr->queue_head++; 	
  /**
   * @WARNING Pointer Arithmetic 
   */

  /* @remark check if queue head and tail are valid, else empty the queue */
  if( (queue_ptr->queue_head > queue_ptr->queue_end) )
    {
    
      if ((BKIT_QUEUE_CIRCULAR & queue_ptr->queue_flags) == BKIT_QUEUE_CIRCULAR)
	{
	  queue_ptr->queue_head = queue_ptr->queue_base;
	  if( queue_ptr->queue_head == queue_ptr->queue_tail )
	    {
	      queue_ptr->queue_flags |= BKIT_QUEUE_EMPTY;
	    }
	}
      else
	{
	  queue_reset( queue_ptr );
	}
    }

  if( (queue_ptr->queue_head >= queue_ptr->queue_tail) )
    {
      if((queue_ptr->queue_tail != queue_ptr->queue_base) &&
         (queue_ptr->queue_flags & BKIT_QUEUE_CIRCULAR))
	{
	  queue_reset( queue_ptr );
	}

      if( !(queue_ptr->queue_flags & BKIT_QUEUE_CIRCULAR) )
	{
	  queue_reset( queue_ptr );
	}
    }

  /* @remark unlock queue */
  queue_ptr->queue_flags |=  (BKIT_QUEUE_READY);

  return( ret_item );
}



/**
 * @fn t_u32 queue_items( t_queue_ptr queue_ptr )
 * @brief calculates the number of items in the queue
 * @param queue_ptr is a pointer to a queue
 */
t_u32 queue_items( t_queue_ptr queue_ptr )
{
  t_u32 retval = ZERO;

  /* @remark handle invalid queue reference */
  if( NULL == queue_ptr )
    {
      return( retval = ZERO );
    }

  /* @remark handle empty or undefined queues */
  if( (queue_ptr->queue_flags & BKIT_QUEUE_EMPTY) ||
      (queue_ptr->queue_flags & BKIT_QUEUE_UNDEFINED) )
    {
      return( retval = ZERO );
    }


#ifdef _BKIT_QUEUE_DEBUG
  printf("%s: head [0x%08x] : base [0x%08x] : end [0x%08x] : tail [0x%08x]\n", __FUNCTION__, 
	 queue_ptr->queue_head,  queue_ptr->queue_base, queue_ptr->queue_end, 
	 queue_ptr->queue_tail ); 
#endif

  if( (queue_ptr->queue_flags & BKIT_QUEUE_CIRCULAR) == BKIT_QUEUE_CIRCULAR )
    {
      
      if( queue_ptr->queue_head > queue_ptr->queue_tail )
	{
	  retval = (t_u32)((queue_ptr->queue_end  - queue_ptr->queue_head) + 
			   (queue_ptr->queue_tail - queue_ptr->queue_base));
	  retval++;
	}
      else
	{
	  retval = (t_u32)(queue_ptr->queue_tail - queue_ptr->queue_head);
	  if( 0 == retval )
	    {
	      retval += (t_u32)(queue_ptr->queue_end - queue_ptr->queue_base);
	    }
	  retval++;
	}
    }
  else
    {
      retval = (t_u32)(queue_ptr->queue_tail - queue_ptr->queue_head);
      retval++;
    }

  return( retval );
}

/* @remark end of "queue.c" */
