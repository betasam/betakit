/**
 * @file	memory.h
 * @author	Sunil Beta <betasam@gmail.com>
 * @brief	provide type definitions for machine portability.
 * @warning	This header assumes that your registers are always 32-bit.
 */

/*
 * Copyright (c) 2005-2008 Sunil Beta Baskar <betasam@gmail.com>
 */

#ifndef _BTYPES_H_INC
#define _BTYPES_H_INC

/* types, defined for 32-bit processors */
typedef int	       t_s32;
typedef long	       t_l;
typedef char	       t_s8;

typedef unsigned short t_u16;
typedef unsigned int   t_u32;
typedef unsigned       t_u;
typedef unsigned long  t_ul;
typedef unsigned char  t_u8;

typedef unsigned long long	t_u64;
typedef long long		t_s64;


typedef void*	       t_ptr;
typedef void*	       t_type;
typedef char*	       t_string;

typedef unsigned char  t_byte;
typedef unsigned short t_word;
typedef unsigned long  t_longword;

typedef unsigned char  t_bool;

/* macro constants */
#ifndef true
#define true		1
#endif

#ifndef false
#define false		!(true)
#endif

#endif /* _BTYPES_H_INC */
