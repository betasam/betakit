/**
 * @file hello.c
 * @author Sunil Beta <betasam@gmail.com>
 * @date 2009-2012
 * @brief tests services provided by betakit's libraries.
 *
 * Tests the services provided by Betakit's libraries
 * which are "libbsys" and "libbdata". This is a not so
 * simple Hello world application. It requires "stdio.h"
 * for printf and sprintf.
 *
 * This now includes support for "libbui" which is the 
 * user interface library added to betakit.
 */

/* @remark standard includes */
#include <stdio.h>

/* @remark betakit includes */
#include <ops.h>
#include <btypes.h>

#include <memory.h>
#include <dhrystone.h>
#include <whetstone.h>

#include <list.h>
#include <stack.h>
#include <queue.h>
#include <number.h>

#include <cli.h>

#define MAX_MENU_ITEMS		7

#define	SZ_BETAKIT_PROMPT	"BetaKit> "
#define SZ_BETAKIT_HELLO	"Hello Betakit User!"

t_string menu_items[MAX_MENU_ITEMS] = {
  "Calculate Dhrystones",
  "Calculate Whetstones",
  "Say Hello to Betakit",
  "Test Stack",
  "Test Queue",
  "Test Numbers",
  "Exit Menu"
};

/**
 * @fn void say_hello(void)
 * @brief just send a hello message
 *
 * @remark also tests memory allocation 
 *         functions of betakit
 */
void say_hello( void )
{
  t_str str_hello;
  str_hello = (t_str) mem_alloc(BKIT_SZLEN_DEFAULT);
  sprintf( str_hello, "%s", SZ_BETAKIT_HELLO);
  printf( "Betakit says %s\n", str_hello );
  printf( "Betakit: Bye!\n");
  mem_free( (t_ptr)str_hello );
  return;
}

/**
 * @fn void test_stack(void)
 * @brief tests stack operations
 */
void test_stack( void )
{
  t_str str_ptr;
  t_s32 idx;
  t_stack_ptr stack_ptr;

  stack_ptr = stack_init( MAX_MENU_ITEMS );

  for( idx = 0; idx < MAX_MENU_ITEMS; idx++ )
    {
      stack_push( stack_ptr, (t_ptr)menu_items[ idx ] );
    }

  for( idx = 0; idx < MAX_MENU_ITEMS; idx++ )
    {
      str_ptr = stack_pop( stack_ptr );
      if( NULL != str_ptr ) 
	{
	  printf("%s: stack pop [%d] := %s\n", __FUNCTION__, 
		 idx, str_ptr );
	}
      else 
	{
	  printf("%s: stack error := [NULL PTR]\n", __FUNCTION__);
	}
    }

  stack_clean( stack_ptr );
  return;		       
}

/**
 * @fn test_queue(void)
 * @brief tests the queue data structure
 */
void test_queue( void )
{
  t_str str_ptr;
  t_s32 idx;
  t_queue_ptr queue_ptr;

  queue_ptr = queue_init( MAX_MENU_ITEMS, true  ); /* circular: true */

  if( NULL == queue_ptr )
    {
      printf("%s: Fatal! unable to initialise queue\n", __FUNCTION__ );
      return;
    }

  for( idx = 0; idx < MAX_MENU_ITEMS; idx++ )
    {
      if( 0 > queue_add( queue_ptr, (t_ptr)menu_items[ idx ] ) )
	{
	  printf("%s: error adding item to queue (%d)\n", __FUNCTION__,
		 idx );
	}
    }

  for( idx = 0; idx < MAX_MENU_ITEMS; idx++ )
    {
      if( 0 > queue_add( queue_ptr, (t_ptr)menu_items[ idx ] ) )
	{
	  printf("%s: error adding item to queue (%d)\n", __FUNCTION__,
		 idx );
	}
    }

  printf("%s: queue has %d items\n", __FUNCTION__, queue_items( queue_ptr ));

  for( idx = 0; idx < MAX_MENU_ITEMS; idx++ )
    {
      str_ptr = queue_get( queue_ptr );
      if( NULL != str_ptr )
	{
	  printf("%s: queue read [%d] := %s\n", __FUNCTION__,
		 idx, str_ptr );
	}
      else
	{
	  printf("%s: queue read error [%d]\n", __FUNCTION__,
		 idx );
	}
    }
  printf("%s: queue has %d items\n", __FUNCTION__, queue_items( queue_ptr ));


  queue_clean( queue_ptr );
  return;

}

/**
 * @fn test_number( void )
 * @brief tests numerical functions in library
 * @see number.c
 */
void test_number( void )
{
  t_u64 test_number     = BK_NUMBER_TEST_M;
  t_u64 test_number_n   = ~(BK_NUMBER_TEST_M);
  t_u64 test_random     = 0ull;
  t_u8  msb_test_number = ZERO;
  t_u8  lsb_test_number = ZERO;

  test_number = BK_NUMBER_TEST_M;
  msb_test_number = number_msbit( test_number );
  printf("%s: number[0x%016llX], msb[%d]\n", __FUNCTION__,
	 test_number, msb_test_number );

  test_number = ~(BK_NUMBER_TEST_M); /* Reassignment */
  msb_test_number = number_msbit( test_number );
  printf("%s: number[0x%016llX], msb[%d]\n", __FUNCTION__,
	 test_number_n, msb_test_number );

  test_number = BK_NUMBER_TEST_L; /* Reassignment */
  lsb_test_number = number_lsbit( test_number );
  printf("%s: number[0x%016llX], lsb[%d]\n", __FUNCTION__,
	 test_number, lsb_test_number );

  test_number = ~(BK_NUMBER_TEST_L); /* Reassignment */
  lsb_test_number = number_lsbit( test_number );
  printf("%s: number[0x%016llX], lsb[%d]\n", __FUNCTION__,
	 test_number, lsb_test_number );

  test_random = number_rand( (t_u32)(test_number & BK_NUMBER_LMASK) );
  printf("%s: simple random[0x%016llX]\n", __FUNCTION__,
	 test_random );
	 
  return;
}

/**
 * @fn void measure_dhrystones( void )
 * @brief measure dhrystone MIPS
 * tests dhrystone MIPS C/2.1 algorithm implementation
 * @see dhrystone.c
 */
void measure_dhrystones( void )
{
  t_dhrystone_result dresult;
  calculate_dhrystone ( DHRYSTONE_TEST_TIME, &dresult );
  printf( "\t your machine can do %f MIPS which is %f VAX-MIPS taking %f uSecs per loop\n",
	  dresult.dhrystones, dresult.vaxmips, dresult.loop_usecs );
  printf( "\t that is %u loops in %f Seconds\n", (t_u32) dresult.total_loops, (t_f32) dresult.real_time_taken );
  return;
}

/**
 * @fn void measure_whetstones( void )
 * @brief measures whetstones per second 
 * demonstrates whetstone measurment algorithm
 * @see whetstone.c
 */
void measure_whetstones( void )
{
  t_whetstone_result wresult;
  calculate_whetstones( WHETSTONE_TEST_TIME, &wresult );
  printf( "\t your machine can do %f Whetstone MIPS with %f mSecs per loop\n",
	  wresult.whetstones, wresult.loop_msecs );
  printf( "\t that is %u loops in %f Seconds\n", (t_u32) wresult.total_loops, (t_f32) wresult.real_time_taken );
  return;
}


/**
 * @fn t_s32 hello_menu( t_s32 choice )
 * @brief callback function for CLI menu interface
 * @see cli.c
 */
t_s32 hello_menu( t_s32 choice )
{
  t_s32 retval = ZERO;
 
  switch( choice )
    {
    case 1:
      {
	measure_dhrystones();
	retval = choice;
      }
      break;
    case 2:
      {
	measure_whetstones();
	retval = choice;
      }
      break;
    case 3:
      {
	say_hello();
	retval = choice;
	break;
      }
    case 4:
      	test_stack();
	retval = choice;
	break;
    case 5:
      test_queue();
      retval = choice;
      break;
    case 6:
      test_number();
      retval = choice;
      break;
    case MAX_MENU_ITEMS:
      {
	retval = CLI_EXIT_ACTION;
	break;
      }
    default:
      printf("%s: invalid choice\n", __FUNCTION__ );
    }
  return( retval );
}


/**
 * @fn int main(void)
 * @brief demonstrates CLI capabilities of Betakit
 */
int main( void )
{
  t_menu_ptr main_menu;

  main_menu = cli_menu_init();

  cli_menu_addlist( main_menu, menu_items, MAX_MENU_ITEMS );
  cli_menu_callback( main_menu, &hello_menu );
  cli_set_prompt((t_str)SZ_BETAKIT_PROMPT);

  cli_menu_user( main_menu );
 
  mem_gc();			/* @remark garbage collect */
  return(0);
}

/* @remark end of file "hello.c" */
