/**
 * @file	bstring.h
 * @brief	string manipulation assistance
 * @see		bstring.c
 * @author	Sunil Beta Baskar <betasam@gmail.com>
 * @date	2010-2012
 */

#include <bconst.h>
#include <btypes.h>

#ifndef _BSTRING_H_INC
#define _BSTRING_H_INC

#define BK_STR_MAXROTATE	127
#define BK_STR_MINROTATE	-127

#define BK_SZ_LEN		256
#define BK_SZ_MAXLEN		64 _KB

t_s32 bk_strcpy( t_str sz_dst, const t_str sz_src );
t_s32 bk_strlen( t_str sz );
t_s32 bk_strrev( t_str revstrptr, t_str strptr );
t_s32 bk_ultostr( t_u32 u_num, t_str strptr );
t_s32 bk_tolower( t_str strptr );
t_s32 bk_toupper( t_str strptr );
t_s32 bk_trstr( t_str strptr, t_s32 rot );
t_s32 bk_atbash( t_str strptr );
t_s32 bk_AtBash( t_str strptr );

#endif	/* _BSTRING_H_INC */
