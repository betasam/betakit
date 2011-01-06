/**
 * @file	bjemalloc.h
 * @author	Sunil Beta <betasam@gmail.com>
 * @date	2009-2012
 * @brief	provide helpers for jemalloc()
 *
 * @details
 *		imports jemalloc routines
 *		this header is for internal use
 *		only. do not call, use memory.h.
 */

#ifndef _BJEMALLOC_H_INC
#define _BJEMALLOC_H_INC

#ifndef _BKIT_JEMALLOC
#   error please use $ make JEMALLOC=1
#endif

#if defined(CONFIG_BK_SYS_JEMALLOC)

#ifndef JEMALLOC_P
#define JEMALLOC_P(s) bk_##s
#endif

#ifndef JEMALLOC_ATTR
#define JEMALLOC_ATTR(s) __attribute__((s))
#endif

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#endif	/* CONFIG_BK_SYS_JEMALLOC */

#endif	/* _BJEMALLOC_H_INC */
