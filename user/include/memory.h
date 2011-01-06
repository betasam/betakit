/**
 * @file	memory.h
 * @author	Sunil Beta <betasam@gmail.com>
 * @date	2009-2012
 * @brief	provide memory allocation functions
 *
 * @details
 * Provide support for memory allocation that can be
 * made POSIX independent with an alternate malloc 
 * algorithm that is being planned. All memory 
 * allocations are done using callbacks under this
 * assumption.
 */


#ifndef _MEMORY_H_INC
#define _MEMORY_H_INC

#include <bkconfig.h>

#ifdef CONFIG_BK_SYS_MEMORY

#include <btypes.h>

/** @remark define macros */
#define MEMORY_TRACKER_SIZE	1024
#define BKIT_SZLEN_DEFAULT	256
#define BKIT_MEM_SLEEP_USECS	100000

#define __new(x)		mem_alloc(sizeof(x))
#define __new__(x,y)		mem_alloc(sizeof(x) * y)
#define BKIT_ARRAY_SIZE(x)	(sizeof(x)/sizeof(x[0]))

/** @remark define structures */
struct s_memory_calls {
  t_ptr  (*malloc)( t_u32 ubytes );
  t_void  (*free)( t_ptr ptr );
  t_ptr (*realloc)( t_ptr ptr, t_u32 ubytes);
  t_ptr (*calloc)( t_u32 nmemb, t_u32 ubytes);
  t_u32 alloc_used;
};

/** @remark simply with type definitions */
typedef struct s_memory_calls t_memory_calls;

/** @remark declare functions */
t_ptr  mem_alloc( t_u32 ui_bytes );
t_ptr  mem_clearalloc( t_u32 ui_bytes );
t_void mem_free( t_ptr ptr_mem );
t_ptr  mem_realloc( t_ptr ptr_mem, t_u32 ui_bytes );
t_void mem_gc( t_void );
t_void mem_changecalls( t_memory_calls *new_calls );
t_u32  mem_copy( t_ptr dest_ptr, t_ptr src_ptr, t_u32 u_bytes_to_copy );

#if defined(BKIT_DEBUG_MODE)
t_void mem_stat(t_void);
#endif	/* BKIT_DEBUG_MODE */

/** @remark call to import jemalloc */
#if defined(CONFIG_BK_SYS_JEMALLOC)
t_void bk_jemalloc_calls( t_memory_calls *p );
#endif	/* CONFIG_BK_SYS_JEMALLOC */

#endif /* CONFIG_BK_SYS_MEMORY */

#endif /* _MEMORY_H_INC */
