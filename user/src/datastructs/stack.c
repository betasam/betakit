/**
 * @file	stack.c
 * @author	Sunil Beta Baskar <betasam@gmail.com>
 * @date	2010-2012
 * @brief	A simple stack implementation.
 * @details
 *		This is a over-documented stack implementation.
 *		A lot of checks are in place to avoid
 *		memory errors.
 *
 */

/* @remark betakit includes */
#include <memory.h>
#include <berror.h>
#include <btypes.h>

#include <stack.h>

/**
 * \verbatim
 * Incremental Stack / Design
 *
 * /------------\
 * | STACK_BASE | X
 * |------------|
 * | data ptr   | X  + PTR_SIZE
 * |------------|
 * | data ptr   | ...+ PTR_SIZE
 * |------------|
 * | data ptr   |
 * |------------|
 * | STACK_TOP  | 
 * \------------/
 *
 * \endverbatim
 */


/**
 * @fn	stack_init *stack_init( t_u32 depth )
 * @brief initializes a stack
 * @param depth		maximum depth of the stack
 * @return pointer to stack on success or NULL on failure
 */
t_stack *stack_init( t_u32 depth )
{
  t_stack *stack_ptr = NULL;

  if( ZERO == depth )
    {
      return( stack_ptr );
    }

  stack_ptr = mem_alloc( sizeof( t_stack ) );

  if( NULL == stack_ptr ) 
    {
      return( stack_ptr );
    }
  stack_ptr->stack_status = STACK_UNKNOWN; 
  stack_ptr->stack_depth  = depth + 1;
  /* @remark created stack but not ready */

  stack_ptr->stack_base = (t_ptr) mem_alloc( sizeof( t_ptr ) * (stack_ptr->stack_depth) );

  if( NULL == stack_ptr->stack_base ) 
    {
      stack_ptr->stack_status = STACK_ERROR; 
      /* @remark cannot allocate memory for stack */
      return( stack_ptr );
    }

  stack_ptr->stack_top  = stack_ptr->stack_base;

  stack_ptr->stack_status = STACK_EMPTY; /* @remark stack is empty and ready */

  return( stack_ptr );
}



/**
 * @fn stack_depth( t_stack *stack_ptr )
 * @brief gets the maximum depth of the stack
 * @return ZERO on error, depth on success.
 */
t_s32 stack_current_depth( t_stack *stack_ptr )
{
  t_s32 retval = ZERO;

  if( NULL == stack_ptr )
    {
      return(retval);
    }

  return( retval = (stack_ptr->stack_top - stack_ptr->stack_base) );
}

/**
 * @fn stack_clean( t_stack *stack_ptr )
 * @brief cleans up a stack 
 * @WARNING will clean up a stack even if it contains data
 * @return none (void)
 */
void stack_clean( t_stack *stack_ptr )
{

  if( NULL == stack_ptr ) return;

  mem_free( stack_ptr->stack_base );
  stack_ptr->stack_status = STACK_UNKNOWN;
  stack_ptr->stack_top   = (t_ptr)0; /* @WARNING DANGER! NULL Pointer */ 
  stack_ptr->stack_base  = stack_ptr->stack_top;

  return;
}

/**
 * @fn stack_push( t_stack *stack_ptr, t_ptr dataptr )
 * @brief adds an item to the stack
 * @param stack_ptr pointer to type t_stack
 * @param dataptr pointer to data pushed into stack
 * @return 0 on success and -ve on failure @see berror.h
 */
t_s32 stack_push( t_stack *stack_ptr, t_ptr dataptr)
{
  t_s32 retval = 0;

  if( NULL == stack_ptr ) return( retval = -(BERR_NOMEM|BERR_NORMAL) );

  if( STACK_FULL  == stack_ptr->stack_status ) return( retval = -(BERR_NOSPACE) );
  if( STACK_ERROR == stack_ptr->stack_status ) return( retval = -(BERR_CRASH) );

  if( (stack_ptr->stack_top - stack_ptr->stack_base) <= stack_ptr->stack_depth ) 
    {
      if( STACK_EMPTY != stack_ptr->stack_status ) 
	{
	  stack_ptr->stack_top++; /* @WARNING Pointer arithmetic */
	}

      *(stack_ptr->stack_top) = dataptr;

      if( (stack_ptr->stack_top - stack_ptr->stack_base) <= stack_ptr->stack_depth ) 
	{
	  stack_ptr->stack_status = STACK_READY; /* @remark STACK is ready */
	} 
      else 
	{
	  stack_ptr->stack_status = STACK_FULL; /* @remark STACK is full */
	}
    }

  return( retval );
}


/**
 * @fn stack_pop( t_stack *stack_ptr )
 * @brief pops an item out of the stack
 * @param stack_ptr pionter to type t_stack
 * @return pointer to data popped off the stack on success
 *	   NULL on failure.
 */
t_ptr stack_pop( t_stack *stack_ptr )
{
  t_ptr retval = NULL;

  if( (STACK_FULL  == stack_ptr->stack_status) || 
      (STACK_READY == stack_ptr->stack_status) )
    {

      retval = (t_ptr)(*(stack_ptr->stack_top));

      if( (stack_ptr->stack_top - stack_ptr->stack_base) <= stack_ptr->stack_depth )
	{
	  stack_ptr->stack_top--; /* @WARNING Pointer arithmetic */
	}
      else
	{
	  *(stack_ptr->stack_top) = NULL;
	  stack_ptr->stack_status = STACK_EMPTY;
	  return( retval );
	}
      
      if( (stack_ptr->stack_top - stack_ptr->stack_base) <= stack_ptr->stack_depth )
	{ 
	  stack_ptr->stack_status = STACK_READY; /* @remark STACK is not full */
	}
    }

  return( retval );
}

/* @remark end of file "stack.c" */
