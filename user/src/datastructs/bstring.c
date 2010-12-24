/**
 * @file	bstring.c
 * @brief	string manipulation primitives
 * @author	Sunil Beta Baskar <betasam@gmail.com>
 * @date	2010-2012
 * @detail	bstring.c provides a minimalist string library.
 *		This is designed for usage for minimal
 *		string manipulation purposes. Those who
 *		need extensive features can use string.h
 *		which will use libc functions entirely.
 *		These functions are not designed to be
 *		POSIX compliant, however they are similar
 *		in implementation to their POSIX 
 *		counterparts without architecture specific
 *		optimisations.
 *
 * @todo	Architecture specific optimisation.
 */

/* Debug Includes */
#include <stdio.h>
#include <stdlib.h>

#include <bconst.h>
#include <btypes.h>
#include <berror.h>

#include <bstring.h>

/**
 * @fn bk_strcpy( t_str sz_dst, const t_str sz_src )
 * @brief string copy function, not too efficient
 * @remark Yes, I know you can write this in one line.
 *	   while( *dst++ = *src++ );
 *	   better still is an optimized memcpy(...) which
 *	   can do either word size or dma style copy.
 * @return length copied on success, -ve on error
 */
t_s32 bk_strcpy( t_str sz_dst, const t_str sz_src )
{
  t_s32 rt_error = 0;
  t_s32 s32_idx = 0;
  t_str dst, src;

  dst = sz_dst;
  src = sz_src;			

  if( (NULL == sz_dst) || (NULL == sz_src) )
    {
      /* error: NULL pointer */
      return( rt_error = -BERR_NOMEM );
    }

  while ( (*dst++ = *src++) && (s32_idx++ <= (t_s32)BK_SZ_MAXLEN) );
  *dst = BK_CHR_NULL;

  return( rt_error = s32_idx );
}


/**
 * @fn	bk_strlen( t_str sz )
 * @brief simple strlen implementation
 * @return length of string if succeeded, -ve on failure
 * @warning if string is not null terminated,
 *	    function will overflow (despite the check.)
 */
t_s32 bk_strlen( t_str sz )
{
  t_s32 s_len = 0;
    
  if( NULL == sz ) return( s_len = -BERR_NOMEM );
  
  while( *(sz+s_len++) && (BK_SZ_MAXLEN >= s_len) );

  return( s_len );
}

/**
 * @fn bk_strrev( t_str revstr, t_str str )
 * @brief reverse str and store in revstr
 * @return length of string if success, or other if failure.
 */
t_s32 bk_strrev( t_str revstrptr, t_str strptr )
{
  t_s32 s_rt = 0;
  t_s32 len;
  t_u8 *tstr, *trev;
  
  trev = (t_u8*)revstrptr;
  tstr = (t_u8*)strptr;
  if( (NULL == tstr) || (NULL == trev) ) return( s_rt = -BERR_NOMEM );

  len = bk_strlen( strptr ) - 1;

  while( 0 < len )
    {
      *(trev++) = *(tstr + len-- - 1);
    }

  *trev = BK_CHR_NULL;

  return( s_rt = ((t_str)trev - revstrptr) );
}

/**
 * @fn bk_ultostr( t_u32 num, t_str strptr )
 * @brief converts (unsigned long) num to string strptr 
 * @remark ASCII only, not i8ln
 * @return 0 if success, non-zero on failure
 */
t_s32 bk_ultostr( t_u32 u_num, t_str strptr )
{
  t_s32 s_rt = 0;
  t_u8 *str, rev[BK_SZ_LEN];

  str = (t_u8*)strptr;
  if( NULL == str ) return( s_rt = -BERR_NOMEM );

  do
    {
      *str++ = (u_num % BK_NUM_BASE) + BK_CHR_0;
      u_num = u_num / BK_NUM_BASE;
    } while( 0 != u_num );

  *str = BK_CHR_NULL;

  str = (t_u8*)strptr;

  bk_strrev( (t_str)rev, (t_str)str ); /* rev = reverse(str) */
  bk_strcpy( (t_str)str, (t_str)rev ); /* str = reverse      */

  return( s_rt = (strptr - (t_str)str) );
}


/**
 * @fn bk_tolower( t_str strptr )
 * @brief convert string strptr to lower case
 * @remark works only for ASCII strings
 * @return length of string on success
 */
t_s32 bk_tolower( t_str strptr )
{
  t_s32 s_rt = 0;
  t_u8 *str;
  t_u8  chr;
  
  if( NULL == strptr ) return ( s_rt = -BERR_NOMEM );

  str = (t_u8*)strptr;
  
  do
    {
      chr = *str;

      if( (chr >= BK_CHR_A) && (chr <= BK_CHR_Z) )
	{
	  chr -= BK_CHR_A;
	  chr += BK_CHR_a;
	  *str = chr;
	}
      
    } while( *str++ );

  s_rt = (t_str)str - strptr;

  return( s_rt );
}

/**
 * @fn t_s32 bk_toupper( t_str strptr )
 * @brief convert string to upper case
 * @remark works only for ASCII strings
 * @return length of string on success
 */
t_s32 bk_toupper( t_str strptr )
{
  t_s32 s_rt = 0;

  t_u8 *str;
  t_u8  chr;

  if( NULL == strptr ) return ( s_rt = -BERR_NOMEM );

  str = (t_u8*)strptr;
  
  do
    {
      chr = *str;

      if( (chr >= BK_CHR_a) && (chr <= BK_CHR_z) )
	{
	  chr -= BK_CHR_a;
	  chr += BK_CHR_A;
	  *str = chr;
	}
      
    } while( *str++ );

  s_rt = (t_str)str - strptr;

  return( s_rt );

}

/**
 * @fn t_s32 bk_trstr( t_str strptr, t_s32 rot )
 * @brief translate string or rotate string
 * @remark for ASCII strings only
 *	   trivial implementation of tr//
 * @return length of string rotated
 */
t_s32 bk_trstr( t_str strptr, t_s32 rot )
{
  t_s32 s_rt = 0;
  volatile t_u8 *str;
 
  if( NULL == strptr ) return( s_rt = -BERR_NOMEM );

  if( (BK_STR_MAXROTATE < rot) || (BK_STR_MINROTATE > rot) || (0 == rot) )
    {
      return( s_rt = -BERR_INVALID );
    }

  str = (t_u8*)strptr;
  do 
    {
      if( (*str) != BK_CHR_NULL )
	{
	  *str += rot; 
	}
    } while( *str++ );

  s_rt = (t_str)str - strptr;

  return( s_rt );
}

/**
 * @fn t_s32 bk_atbash( t_str strptr )
 * @brief atbash transformation for ASCII string
 * @return length of string on success, -ve on errors.
 * @remark handles upper and lower case characters
 */
t_s32 bk_atbash( t_str strptr )
{
  t_s32 s_rt = 0;
  t_u8 b_dist, b_half;
  t_u8  *str;
  t_u8   chr;

  if ( NULL == strptr ) return( s_rt = -BERR_NOMEM );

  str = (t_u8*)strptr;


  b_dist = BK_CHR_Z - BK_CHR_A;
  b_half = b_dist / 2;

  do 
    {
      chr = *str;

      if( BK_CHR_NULL == chr )
	{
	  break;
	}

      /* handle upper case */
      if( (BK_CHR_A <= chr) && (BK_CHR_Z >= chr) )
	{
	  chr -= BK_CHR_A;

	  if( chr < b_half )
	    {
	      b_dist = b_half - chr;
	      chr    = b_half + b_dist;
	    }
	  else if( chr > b_half )
	    {
	      b_dist = chr - b_half;
	      chr    = b_half - b_dist;
       	    }
	  
	  chr += BK_CHR_A + 1;
	  *str = chr;
	}

      /* handle lower case */
      if( (BK_CHR_a <= chr) && (BK_CHR_z >= chr) )
	{
	  chr -= BK_CHR_a;

	  if( chr < b_half )
	    {
	      b_dist = b_half - chr;
	      chr    = b_half + b_dist;
	    }
	  else if( chr > b_half )
	    {
	      b_dist = chr - b_half;
	      chr    = b_half - b_dist;
	    }

	  chr += BK_CHR_a + 1;

	  *str = chr;
	}

      str++;

    } while( *str );

  s_rt = (t_str)str - strptr;

  return( s_rt );
}


/**
 * @fn t_s32 bk_AtBash( t_str strptr )
 * @brief atbash transformation for ASCII string
 * @return length of string on success, -ve on errors.
 * @remark lower to upper, upper to lower with atbash 
 *	   fancy function, no practical use thought of.
 */
t_s32 bk_AtBash( t_str strptr )
{
  t_s32 s_rt = 0;
  t_u8 b_dist, b_half;
  t_u8  *str;
  t_u8   chr;

  if ( NULL == strptr ) return( s_rt = -BERR_NOMEM );

  str = (t_u8*)strptr;


  b_dist = BK_CHR_Z - BK_CHR_A;
  b_half = b_dist / 2;

  do 
    {
      chr = *str;

      if( BK_CHR_NULL == chr )
	{
	  break;
	}

      /* handle upper case */
      if( (BK_CHR_A <= chr) && (BK_CHR_Z >= chr) )
	{
	  chr -= BK_CHR_A;

	  if( chr < b_half )
	    {
	      b_dist = b_half - chr;
	      chr    = b_half + b_dist;
	    }
	  else if( chr > b_half )
	    {
	      b_dist = chr - b_half;
	      chr    = b_half - b_dist;
       	    }
	  /* send to lower case */
	  chr += BK_CHR_a + 1;
	  *str = chr;
	}

      /* handle lower case */
      if( (BK_CHR_a <= chr) && (BK_CHR_z >= chr) )
	{
	  chr -= BK_CHR_a;

	  if( chr < b_half )
	    {
	      b_dist = b_half - chr;
	      chr    = b_half + b_dist;
	    }
	  else if( chr > b_half )
	    {
	      b_dist = chr - b_half;
	      chr    = b_half - b_dist;
	    }
	  /* send to upper case */
	  chr += BK_CHR_A + 1;

	  *str = chr;
	}

      str++;

    } while( *str );

  s_rt = (t_str)str - strptr;

  return( s_rt );
}


/**
 * @Note 
 *
 * Reference for Atbash Cipher
 *
   ABCDE
   zyxwv
 
   FGHIJ
   utsrq
 
   KLMNO
   ponml
 
   PQRST
   kjihg
 
   UVWXYZ
   fedcba
 *
 *
 */

/* end of file "bstring.c" */
