/**
 * @file	memory.h
 * @author	Sunil Beta <betasam@gmail.com>
 * @brief	provide memory allocation functions
 *
 * Provide support for memory allocation that can be
 * made POSIX independent with an alternate malloc 
 * algorithm that is being planned. All memory 
 * allocations are done using callbacks under this
 * assumption.
 */

/***
 * Copyright (c) 2005-2008 Sunil Beta Baskar <betasam@gmail.com>
 */

#ifndef _MEMORY_H_INC
#define _MEMORY_H_INC

#include <btypes.h>

/* define macros */
#define MEMORY_TRACKER_SIZE	256

/* define structures */
struct s_memory_calls {
  t_ptr (*malloc)( t_u32 size );
  void  (*free)( t_ptr ptr );
  t_ptr (*realloc)( t_ptr ptr, t_u32 size);
  t_ptr (*calloc)( t_u32 nmemb, t_u32 size);
};

/* simply with type definitions */
typedef struct s_memory_calls t_memory_calls;


/* declare functions */
t_ptr mem_alloc( t_u32 ui_bytes );
t_ptr mem_clearalloc( t_u32 ui_bytes );
void mem_free( t_ptr ptr_mem );
t_ptr mem_realloc( t_ptr ptr_mem, t_u32 ui_bytes );
void mem_gc( void );
void mem_changecalls( t_memory_calls *new_calls );

#endif /* _MEMORY_H_INC */
