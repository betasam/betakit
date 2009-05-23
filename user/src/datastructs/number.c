/**
 * @file	number.c
 * @author	Sunil Beta Baskar <betasam@gmail.com>
 * @brief	integer arithmetic and floating point arithmetic structures
 */


/**
 * CHANGELOG
 *
 * 2009-05-21	number_msbit( u_number ) implemented;
 *		number_gen_primes( ptr_u_primes, s_count ) defined;
 *		number_lsbit( u_number ) implemented;
 */

/* bkit includes */
#include <btypes.h>
#include <memory.h>
#include <ops.h>

/* self definitions include <number.h> */
#include <number.h>

/* global data */

t_u32 u_prime_table[15] = 
  {  2,  3,   5,  7, 11,
     13, 17, 19, 23, 29,
     31, 37, 41, 43, 47 
  };


/**
 * @fn		number_msbit
 * @brief	finds the most significant bit in a number
 */
t_s8 number_msbit( t_u32 u_number )
{
  t_s8 s_retval = NUMBER_INT_SIZE;  

  t_u8 u_nibble_count = 0;
  t_u8 u_nibble = NUMBER_NIBBLE_FULL;
  t_u8 u_bit_in_nibble = 0;

  t_u32 u_complement = 0;

  if( 0 == u_number )
    {
      return( s_retval );
    }

  /* 1's complement */
  u_complement = ~(u_number); 

  for(u_nibble_count = (NUMBER_NIBBLES_IN_INT-1); u_nibble_count >= 0; u_nibble_count--)
    {
      /* nibble count-down loop */
      u_nibble = (NUMBER_NIBBLE_FULL << (u_nibble_count * NUMBER_NIBBLE_SIZE));

      if( (u_complement & u_nibble) == 0 ) 
	{
	  /* nibble contains most significant bit */

	  for( u_bit_in_nibble = (NUMBER_NIBBLE_SIZE-1); u_bit_in_nibble >= 0; u_bit_in_nibble-- )
	    {	      
	      /* bit count-down loop */
	      if(u_number & ((1<<u_bit_in_nibble)<<(u_nibble_count*NUMBER_NIBBLE_SIZE)))
		{
		  s_retval = u_bit_in_nibble + (u_nibble_count * NUMBER_NIBBLE_SIZE);
		  return( s_retval );
		}
	    }
	}

    }
   
  return( s_retval );
}


/**
 * @fn		number_lsbit
 * @brief	finds the least significant bit in a number
 */
t_s8 number_lsbit( t_u32 u_number )
{
  t_s8 s_retval = NUMBER_INT_SIZE;  

  t_u8 u_nibble_count = 0;
  t_u8 u_nibble = NUMBER_NIBBLE_FULL;
  t_u8 u_bit_in_nibble = 0;

  t_u32 u_complement = 0;

  if( 0 == u_number )
    {
      return( s_retval );
    }

  /* 1's complement */
  u_complement = ~(u_number); 

  for(u_nibble_count = 0; u_nibble_count < NUMBER_NIBBLES_IN_INT; u_nibble_count++)
    {
      /* nibble count-up loop */
      u_nibble = (NUMBER_NIBBLE_FULL << (u_nibble_count * NUMBER_NIBBLE_SIZE));

      if( (u_complement & u_nibble) == 0 ) 
	{
	  /* nibble contains least significant bit */

	  for( u_bit_in_nibble = 0; u_bit_in_nibble < NUMBER_NIBBLE_SIZE; u_bit_in_nibble++)
	    {	      
	      /* bit count-up loop */
	      if(u_number & ((1<<u_bit_in_nibble)<<(u_nibble_count*NUMBER_NIBBLE_SIZE)))
		{
		  s_retval = u_bit_in_nibble + (u_nibble_count * NUMBER_NIBBLE_SIZE);
		  return( s_retval );
		}
	    }
	}
    }
   
  return( s_retval );
}


/**
 * @fn		number_sqrt_z
 * @brief	returns the approximate integer square root of a number
 *		for positive integers only
 */
t_u32 number_sqrt_z( t_u32 u_number_zplus )
{
  t_s8 s_msb = 0;
  
  t_u32 u_retval = 0;
  t_u32 u_root_lower = 0;
  t_u32 u_root_upper = 0;
  t_u32 u_curr_root = 0;
  t_u32 u_prev_root = 0;

  if( 0 == u_number_zplus) 
    {
      return( u_retval );
    }

  if( 1 == u_number_zplus)
    {
      u_retval = u_number_zplus;
      return( u_retval );
    }

  s_msb = number_msbit( u_number_zplus );
  /* DEBUG! here */

  u_root_lower = (1<< ((s_msb + 1)/2 - 1));
  u_root_upper = (1<< ((s_msb + 1)/2));

  u_prev_root  = u_root_upper;
  u_retval     = u_root_lower;

  for( u_curr_root = u_root_upper; u_curr_root >= u_root_lower; u_curr_root-- )
    {
      /* count down to find approximate root */
      if( (u_curr_root * u_curr_root) > u_number_zplus )
	{
	  u_retval = u_prev_root;
	  return( u_retval );
	}
      else
	{
	  u_prev_root = u_curr_root;
	}
    }
  
  return( u_retval );
}


/**
 * @fn		number_divisibility
 * @brief	tests whether an integer is divisible by a factor
 *		using just a modulo operator. It just has extra
 *		checks for 0 and 1 to avoid misinterpretations.
 * @comment	try to inline the function as the primality check is
 *		terrible in computation optimization
 */
inline t_bool number_divisibility( t_u32 u_number_z, t_u32 u_factor_z )
{
  t_bool b_retval = false;

  if( (0 == u_factor_z) || (1 == u_factor_z) )
    {      
      return( b_retval );
    }

  if( (1 == u_number_z) || (0 == u_number_z) )
    {
      return( b_retval );
    }

  if( 0 == (u_number_z % u_factor_z) )
    {
      b_retval = true;
      return( b_retval );
    }

  return( b_retval );    
}


/**
 * @fn		number_test_prime
 * @brief	weak primality test, uses approximated root factors
 */
t_bool number_test_prime( t_u32 u_number_zplus )
{
  t_bool b_retval = false;
  
  t_u32 u_number_zroot = 0;
  t_u32 u_ctr = 0;

  u_number_zroot = number_sqrt_z( u_number_zplus );
  /* DEBUG! here */
  
  if( u_number_zroot > u_prime_table[14] )
    {
      for( u_ctr = 0; u_ctr < 14; u_ctr++ )
	{
	  if( true == number_divisibility(u_number_zplus, u_prime_table[ u_ctr ]) )
	    {
	      return(b_retval); /* b_retval is initialized to false */
	    }
	}
    }
  else
    {
      if( true == number_divisibility(u_number_zplus, NUMBER_PRIME_STEP))
	{
	  return(b_retval);
	}

      for( u_ctr = NUMBER_PRIME_ONE; u_ctr <= u_number_zroot; u_ctr += NUMBER_PRIME_STEP )
	{
	  if( true == number_divisibility(u_number_zplus, u_ctr) )
	    {
	      return(b_retval);
	    }
	}

      /* CHECK! special case */
      b_retval = true;
      return( b_retval );
    }
  
  
  for( u_ctr = u_prime_table[14]; u_ctr <= u_number_zroot; u_ctr += NUMBER_PRIME_STEP )
    {
      /* test for primes larger than u_prime_table[14] */
      if( true == number_divisibility(u_number_zplus, u_ctr) )
	{
	  return( b_retval );
	}
    }

  b_retval = true;
  return( b_retval );
}


/**
 * @fn		number_gen_primes
 * @brief	generates a series of prime numbers (32-bit only)
 */
t_s32 number_gen_primes( t_u32 *ptr_u_primes, t_u32 u_count )
{
  t_s32 s_retval = -1;
  t_u32 u_counter = 0;

  t_u32 *ptr_u_new_primes;
  t_u32 u_number_ref;

  if( (u_count > 32) || (u_count <= 0) ) 
    {
      s_retval = -2;
      return( s_retval );
    }

  if( 0 == ptr_u_primes )
    {
      s_retval = -3;
      return( s_retval );
    }

  ptr_u_new_primes = ptr_u_primes;

  *(ptr_u_new_primes) = 1;
  *(ptr_u_new_primes + u_count - 1) = 1;
  /* DEBUG! here. Fault on memory allocation : Dump Stova Core? */

  if( u_count <= 15 )
    {
      for( u_counter = 0; u_counter < u_count; u_counter++ )
	{
	  *(ptr_u_new_primes + u_counter) = u_prime_table[ u_counter ];
	}
    }
  else
    {
      for( u_counter = 0; u_counter < 15; u_counter++ )
	{
	  *(ptr_u_new_primes + u_counter) = u_prime_table[ u_counter ];	  
	}

      for( u_counter = 15; u_counter < u_count; u_counter++ )
	{
	  u_number_ref = *(ptr_u_new_primes + u_counter) = u_prime_table[ u_counter - 1 ] + 2;

	  while( false == number_test_prime( u_number_ref ) )
	    {
	      u_number_ref = *(ptr_u_new_primes + u_counter) = (*(ptr_u_new_primes + u_counter) + 2);	      
	      /* DEBUG! Danger we might just hang here. */
	    }
	  
	}
    }

  s_retval = 0;			

  return( s_retval );
}

