/**
 * @file	btypes.h
 * @author	Sunil Beta <betasam@gmail.com>
 * @date	2009-2012
 * @brief	provide type definitions for machine portability.
 * @remark	you may avoid including inttypes.h and use this 
 *		with bkit for types. You may also use both as 
 *		bkit does not redefine types defined in inttypes.h
 *		although they are not exclusively used.
 *
 * CHANGELOG
 *
 *	6-JAN-2011	t_size defined like size_t
 *			except, it is made as large as possible
 *			t_unsigned is just typedef unsigned
 *			use with caution. As before, users can
 *			use inttypes.h without namespace
 *			pollution.
 */

#ifndef _BTYPES_H_INC
#define _BTYPES_H_INC

#include <bconst.h>

#ifndef __GNUC__
#warning This is not the GNU C Compiler.
#warning bkit assumes usage of gcc, type declarations may fail!
#warning please check and rewrite btypes.h to suit your compiler.
#error   unsupported compiler.
#endif

/* types: basic integer, short integer and float */
typedef int	       t_s32;
typedef unsigned short t_u16;
typedef unsigned int   t_u32;
typedef unsigned short t_shortword;
typedef unsigned int   t_word;
typedef unsigned       t_unsigned;

typedef float	       t_f32;
typedef double	       t_d64;

/* WARNING! ignore __WORDSIZE == 128 */
#if __WORDSIZE == 64
/* types: defined for 64-bit word size */
typedef int	       t_l32;
typedef unsigned int   t_ul;
typedef unsigned int   t_ul32;

typedef long	       t_s64;
typedef unsigned long  t_u64;
typedef unsigned long  t_longword;
typedef unsigned long long t_size;

#else
/* types: defined for 32-bit word size */
typedef long	       t_l32;
typedef unsigned long  t_ul;
typedef unsigned long  t_ul32;	

typedef unsigned long long	t_u64;
typedef long long		t_s64;
typedef unsigned long long	t_longword;
typedef unsigned long long	t_size;
/* types: end definition for 32-bit word size */
#endif

/* types: dependent on WORDSIZE and ARCH */
typedef unsigned       t_u;
typedef long	       t_l;
typedef void	       t_void;

typedef void*	       t_ptr;
typedef void*	       t_type;
typedef void*	       t_addr;

/* types: byte or 8-bit sized data and pointers to it */
typedef char	       t_s8;
typedef unsigned char  t_u8;

typedef char*	       t_string;
typedef char*	       t_str;
typedef unsigned char  t_byte;
typedef unsigned char  t_bool;

#endif /* _BTYPES_H_INC */
