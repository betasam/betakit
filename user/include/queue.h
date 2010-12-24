/**
 * @file	queue.h
 * @author	Sunil Beta Baskar <betasam@gmail.com>
 * @brief	queue implementation header
 * @see		btypes.h
 */

#ifndef _QUEUE_H_INC
#define _QUEUE_H_INC

#include <btypes.h>

/* structure definitions */
struct queue_struct {
  t_ptr *queue_base;
  t_ptr *queue_end;
  t_ptr *queue_head;
  t_ptr *queue_tail;
  t_u8 queue_flags;
};

/* type definitions */
typedef struct queue_struct t_queue;
typedef t_queue* t_queue_ptr;

/* macro definitions */
#define BKIT_QUEUE_EMPTY	0x10
#define BKIT_QUEUE_FULL		0x20
#define BKIT_QUEUE_UNDEFINED	0x01
#define BKIT_QUEUE_CIRCULAR	0x02
#define BKIT_QUEUE_READY	0x04
#define BKIT_MIN_QUEUE_DEPTH	1

/* function declarations */
t_queue_ptr	queue_init( t_u32 queue_depth, t_bool circular );
t_void		queue_clean( t_queue_ptr queue_ptr );
t_s32		queue_add( t_queue_ptr queue_ptr, t_ptr data_ptr );
t_ptr		queue_get( t_queue_ptr queue_ptr );
t_u32		queue_items( t_queue_ptr queue_ptr );
t_u8		queue_status( t_queue_ptr queue_ptr );

/* debug symbols */
#ifdef DEBUG
#define _BKIT_QUEUE_DEBUG
#else
#undef _BKIT_QUEUE_DEBUG
#endif

#endif	/* _QUEUE_H_INC */
