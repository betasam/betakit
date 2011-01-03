/**
 * @file	stack.h
 * @author	Sunil Beta Baskar <betasam@gmail.com>
 * @brief	support for stack implementation in stack.c
 * @see		btypes.h
 *
 * This needs to be included by any file using the stack
 * implementation provided here.
 */

#ifndef _STACK_H_INC
#define _STACK_H_IN

#include <bkconfig.h>
#ifdef CONFIG_BK_DS_STACK

#include <btypes.h>

/* structure definitions */
struct stack_struct {
  t_ptr *stack_base;
  t_ptr *stack_top;
  t_u32	stack_status;
  t_u32 stack_depth;
};

/* type definitions */
typedef struct stack_struct t_stack;
typedef t_stack* t_stack_ptr;

/* Macro definitions: error codes */
#define STACK_EMPTY	300
#define STACK_READY	301
#define STACK_FULL	302
#define STACK_ERROR	303
#define STACK_UNKNOWN	0

/* function declarations */
	t_stack *stack_init( t_u32 depth );
        t_void	 stack_clean( t_stack *stack_ptr );
	t_s32	 stack_push( t_stack *stack_ptr, t_ptr data );
	t_ptr	 stack_pop( t_stack *stack_ptr );
	t_s32	 stack_current_depth( t_stack *stack_ptr );

#endif	/* CONFIG_BK_DS_STACK */

#endif /* _STACK_H_INC */
