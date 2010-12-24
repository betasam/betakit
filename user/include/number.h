/**
 * @file	number.h
 * @author	Sunil Beta Baskar <betasam@gmail.com>
 * @brief	descriptors for numerical recipe data structures
 * @date	2005-2012
 */


#ifndef _NUMBER_H_INC
#define _NUMBER_H_INC

#include <btypes.h>

/* @remark macro assignments */
#define BK_NUMBER_MASK	0xffffffffffffffffull
#define BK_NUMBER_UMASK	0xffffffff00000000ull
#define BK_NUMBER_LMASK	0x00000000ffffffffull

#define BK_NUMBER_TEST_M  0x00000ea7deadbeefull
#define BK_NUMBER_TEST_L  0xc001cafef00dea70ull

#define BK_INTEGER_UMASK 0xffff0000ul
#define BK_INTEGER_LMASK 0x0000fffful

#define BK_SHORT_UMASK	 0xff00
#define BK_SHORT_LMASK	 0x00ff

#define BK_BYTE_UMASK	 0xf0
#define BK_BYTE_LMASK	 0x0f

#define BK_NIBBLE_UMASK	 0x0C
#define BK_NIBBLE_LMASK	 0x03

#define SIZEOF_T_U64	64
#define SIZEOF_T_U32	32
#define SIZEOF_T_U16	16
#define SIZEOF_T_U8	 8
#define SIZEOF_T_U4	 4
#define SIZEOF_T_U2	 2
#define SIZEOF_T_BIT	 1

#define FMT_NUMBER_HEXT	"0x%016llX"
#define FMT_number_hext	"0x%01611x"

#define BK_SLEEP_USECS	10000ull


/* @remark function declarations */
t_u8 number_lsbit( t_u64 u64_number );
t_u8 number_msbit( t_u64 u64_number );
t_u64 number_rand( t_u32 seed );

inline t_u64 number_mul( t_u32 foo, t_u32 bar );

#endif	/* _NUMBER_H_INC */
