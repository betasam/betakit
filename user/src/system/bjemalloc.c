/**
 * @file	bjemalloc
 * @author	Sunil Beta <betasam@gmail.com>
 * @date	2008-2012
 * @brief	imports jemalloc as alternate allocator
 * @remark	this is strictly to assist importing, none else.
 */

#include <bkconfig.h>
#include <memory.h>
#include <btypes.h>

#if defined(CONFIG_BK_SYS_JEMALLOC)

#ifdef JEMALLOC_P
#undef JEMALLOC_P
#endif

#ifndef JEMALLOC_P
#define JEMALLOC_P(s) bk_##s
#endif

/* FIXME! size_t behaving odd here; duct taped it. */
typedef unsigned long int size_t;

extern void     *JEMALLOC_P(malloc)(size_t size);
extern void     *JEMALLOC_P(calloc)(size_t num, size_t size);
extern int	JEMALLOC_P(posix_memalign)(void **memptr, size_t alignment, size_t size);
extern void	*JEMALLOC_P(realloc)(void *ptr, size_t size);
extern void	JEMALLOC_P(free)(void *ptr);
extern size_t	JEMALLOC_P(malloc_usable_size)(const void *ptr);
extern void	JEMALLOC_P(malloc_stats_print)(void (*write_cb)(void *, const char *),
    void *cbopaque, const char *opts);
extern int	JEMALLOC_P(mallctl)(const char *name, void *oldp, size_t *oldlenp,
    void *newp, size_t newlen);
extern int	JEMALLOC_P(mallctlnametomib)(const char *name, size_t *mibp,
    size_t *miblenp);
extern int	JEMALLOC_P(mallctlbymib)(const size_t *mib, size_t miblen, void *oldp,
    size_t *oldlenp, void *newp, size_t newlen);

extern int	JEMALLOC_P(allocm)(void **ptr, size_t *rsize, size_t size, int flags);
extern int	JEMALLOC_P(rallocm)(void **ptr, size_t *rsize, size_t size,
    size_t extra, int flags);
extern int	JEMALLOC_P(sallocm)(const void *ptr, size_t *rsize, int flags);
extern int      JEMALLOC_P(dallocm)(void *ptr, int flags);


/**
 * @fn bk_jemalloc_calls( t_memory_calls *p )
 * @param p pointer of type t_memory_calls
 * @brief force bkit memory allocator to use jemalloc
 */
t_void bk_jemalloc_calls( t_memory_calls *p )
{
  p->malloc     = JEMALLOC_P(malloc);
  p->calloc     = JEMALLOC_P(calloc);
  p->free       = JEMALLOC_P(free);
  p->realloc    = JEMALLOC_P(realloc);
  p->alloc_used = 0;

  mem_changecalls( p );
}

#endif	/* defined(CONFIG_BK_SYS_JEMALLOC) */

/* end of "bjemalloc.c" */
