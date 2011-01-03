/**
 * @file	number.c
 * @author	Sunil Beta Baskar <betasam@gmail.com>
 * @brief	integer arithmetic and floating point arithmetic structures
 *
 * @TODO	Add _CONFIG_RANDOM for dependency 
 *		needed for random number generation 
 */


/**
 * CHANGELOG
 *
 * 2010-12-08   number_msbit( u64_number ) rewritten
 *		number_lsbit( u64_number ) rewritten
 *		number_mul( u32, u32 ) created
 *
 * 2009-05-21	number_msbit( u_number ) implemented;
 *		number_gen_primes( ptr_u_primes, s_count ) defined;
 *		number_lsbit( u_number ) implemented;
 */

#include <bkconfig.h>

#ifdef CONFIG_BK_DS_NUMBER

#include <time.h>
#include <stdlib.h>
#include <unistd.h>

/* @remark bkit includes */
#include <btypes.h>
#include <memory.h>
#include <ops.h>

/* @remark self definitions include <number.h> */
#include <number.h>

/* @remark global data */


/**
 * @fn number_msbit( t_u64 u64_number )
 * @brief finds the MSB of a 64-bit unsigned integer
 * @return returns the MSB (0...63)
 */
t_u8 number_msbit( t_u64 u64_number )
{
  t_u8 retval = ZERO;

  t_u32 u32_hold = ZERO;
  t_u16 u16_hold = ZERO;
  t_u8  u8_hold  = ZERO;
  t_u8  u4_hold  = ZERO;
  t_u8  u2_hold  = ZERO;
  t_u8  bitshift = ZERO;

  /* @remark get rid of zero integers */
  if( 0ull == u64_number )
    {
      return(retval = bitshift = (SIZEOF_T_U64));
    }

  /* @remark find 32-bit part having MSB */
  if( 0 != (u64_number & BK_NUMBER_UMASK))
    {
      u32_hold = (t_u32)((u64_number & BK_NUMBER_UMASK) >> SIZEOF_T_U32);
      bitshift    += SIZEOF_T_U32;
    }
  else if( 0 != (u64_number & BK_NUMBER_LMASK))
    {
      u32_hold = (t_u32)((u64_number & BK_NUMBER_LMASK));
    }
 
  /* @remark find 16-bit part having MSB */
  if( 0 != (u32_hold & BK_INTEGER_UMASK))
    {
      u16_hold = (t_u16)((u32_hold & BK_INTEGER_UMASK) >> SIZEOF_T_U16);
      bitshift    += SIZEOF_T_U16;
    }
  else if( 0 != (u32_hold & BK_INTEGER_LMASK))
    {
      u16_hold = (t_u16)((u32_hold & BK_INTEGER_LMASK));      
    }
  
  /* @remark find 8-bit part having MSB */
  if( 0 != (u16_hold & BK_SHORT_UMASK))
    {
      u8_hold = (t_u8)((u16_hold & BK_SHORT_UMASK) >> SIZEOF_T_U8);
      bitshift   += SIZEOF_T_U8;
    }
  else if( 0 != (u16_hold & BK_SHORT_LMASK))
    {
      u8_hold = (t_u8)((u16_hold & BK_SHORT_LMASK));
    }

  /* @remark find 4-bit nibble having MSB */
  if(0 != (u8_hold & BK_BYTE_UMASK))
    {
      u4_hold = (t_u8)((u8_hold & BK_BYTE_UMASK) >> SIZEOF_T_U4);
      bitshift   += SIZEOF_T_U4;
    }
  else if( 0 != (u8_hold & BK_BYTE_LMASK))
    {
      u4_hold = (t_u8)((u8_hold & BK_BYTE_LMASK));
    }

  /* @remark find 2-bit pair containing MSB */
  if( 0 != (u4_hold & BK_NIBBLE_UMASK ))
    {
      u2_hold = (t_u8)((u4_hold & BK_NIBBLE_UMASK) >> SIZEOF_T_U2);
      bitshift   += SIZEOF_T_U2;
    }
  else if( 0 != (u4_hold & BK_NIBBLE_LMASK))
    {
      u2_hold = (t_u8)((u4_hold & BK_NIBBLE_LMASK));
    }

  /* @remark find bit containing MSB */
  if( _OPS_BIT(1) == (u2_hold & _OPS_BIT(1)))
    {
      bitshift += SIZEOF_T_BIT;
    }
    else if( _OPS_BIT(0) == (u2_hold & _OPS_BIT(0)) )
    {
      bitshift = bitshift;
      /* @remark Do nothing */
    }

  retval = bitshift;
  return( retval  ); /* @remark MSB: bit[63...0] numbering */ 
}

/**
 * @fn number_lsbit( t_u64 u64_number )
 * @brief finds the LSB of a 64-bit unsigned integer
 * @return returns the MSB (0...63)
 */
t_u8 number_lsbit( t_u64 u64_number )
{
  t_u8 retval = ZERO;

  t_u32 u32_hold = ZERO;
  t_u16 u16_hold = ZERO;
  t_u8  u8_hold  = ZERO;
  t_u8  u4_hold  = ZERO;
  t_u8  u2_hold  = ZERO;
  t_u8  bitshift = ZERO;

  /* @remark get rid of zero integers */
  if( 0ull == u64_number )
    {
      return(retval = bitshift = (SIZEOF_T_U64 + 1));
    }

  /* @remark find 32-bit part having MSB */
  if( 0 != (u64_number & BK_NUMBER_LMASK))
    {
      u32_hold = (t_u32)((u64_number & BK_NUMBER_LMASK));
    }
  else if( 0 != (u64_number & BK_NUMBER_UMASK))
    {
      u32_hold = (t_u32)((u64_number & BK_NUMBER_UMASK) >> SIZEOF_T_U32);
      bitshift    += SIZEOF_T_U32;
    }

 
  /* @remark find 16-bit part having MSB */
  if( 0 != (u32_hold & BK_INTEGER_LMASK))
    {
      u16_hold = (t_u16)((u32_hold & BK_INTEGER_LMASK));      
    }
  else if( 0 != (u32_hold & BK_INTEGER_UMASK))
    {
      u16_hold = (t_u16)((u32_hold & BK_INTEGER_UMASK) >> SIZEOF_T_U16);
      bitshift    += SIZEOF_T_U16;
    }

  
  /* @remark find 8-bit part having MSB */
  if( 0 != (u16_hold & BK_SHORT_LMASK))
    {
      u8_hold = (t_u8)((u16_hold & BK_SHORT_LMASK));
    }
  else if( 0 != (u16_hold & BK_SHORT_UMASK))
    {
      u8_hold = (t_u8)((u16_hold & BK_SHORT_UMASK) >> SIZEOF_T_U8);
      bitshift   += SIZEOF_T_U8;
    }


  /* @remark find 4-bit nibble having MSB */
  if( 0 != (u8_hold & BK_BYTE_LMASK))
    {
      u4_hold = (t_u8)((u8_hold & BK_BYTE_LMASK));
    }
  else if(0 != (u8_hold & BK_BYTE_UMASK))
    {
      u4_hold = (t_u8)((u8_hold & BK_BYTE_UMASK) >> SIZEOF_T_U4);
      bitshift   += SIZEOF_T_U4;
    }


  /* @remark find 2-bit pair containing MSB */
  if( 0 != (u4_hold & BK_NIBBLE_LMASK))
    {
      u2_hold = (t_u8)((u4_hold & BK_NIBBLE_LMASK));
    }
  else if( 0 != (u4_hold & BK_NIBBLE_UMASK ))
    {
      u2_hold = (t_u8)((u4_hold & BK_NIBBLE_UMASK) >> SIZEOF_T_U2);
      bitshift   += SIZEOF_T_U2;
    }


  /* @remark find bit containing MSB */
  if( _OPS_BIT(0) == (u2_hold & _OPS_BIT(0)) )
    {
      bitshift = bitshift;
      /* @remark Do nothing */
    }
  else if( _OPS_BIT(1) == (u2_hold & _OPS_BIT(1)))
    {
      bitshift += SIZEOF_T_BIT;
    }

  retval = bitshift;
  return( retval  ); /* @remark LSB: bit[63...0] numbering */ 
}


/**
 * @fn t_u64 number_mul( t_u32 foo, t_u32 bar )
 * @brief multiplies two 32-bit numbers to a 64-bit 
 *	  operates on "J+" (or "Z+") only.
 * @note This should be a macro
 */
inline t_u64 number_mul( t_u32 foo, t_u32 bar )
{
  return( (t_u64)(foo * bar) );
}

/**
 * @fn t_u64 simple random number generator
 * @brief generates unreliable psuedo random numbers
 */
t_u64 number_rand( t_u32 seed )
{
  static t_u64 prev_number;  
  t_u64 ret_number;
  t_u64 temp_num = 0ull;
  time_t time_val;

 nrlabel:

  usleep(BK_SLEEP_USECS);
 do {
    time( &time_val );		/* @remark get time(...) @see <time.h> */
    srand( (t_u32)time_val );	/* @remark set random seed */
    ret_number = 0ull;
 } while(0);

  usleep(BK_SLEEP_USECS);
  time_val = (time_t)rand();	/* @remark get random number (1) */

  temp_num = (t_u32)time_val;
  temp_num <<= SIZEOF_T_U32;

  time( &time_val );
  time_val = (time_t)rand();	/* @remark get random number (2) */

#ifdef _NUMBER_DEBUG
  printf("%s: 0x%08x 0x%08x\n", __FUNCTION__, time_val_1, time_val_2);
#endif

  ret_number = 0ull;
  ret_number = (t_u64)( ((time_val & BK_NUMBER_LMASK) ) );
  ret_number |= temp_num;
  
  if( prev_number == ret_number )
    {
      usleep(BK_SLEEP_USECS);
      goto nrlabel;
    }

  return( prev_number = ret_number );
}

#endif	/* CONFIG_BK_DS_NUMBER */
/* @remark End of file "number.c" */
