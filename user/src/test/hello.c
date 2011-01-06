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

#include <bkconfig.h>

#ifdef CONFIG_BK_TEST_HELLO

/* @remark standard includes */
#include <stdio.h>

/* @remark betakit includes */
#include <ops.h>
#include <btypes.h>

#include <berror.h>

#include <memory.h>

/* @remark experimental jemalloc support */

#include <dhrystone.h>
#include <whetstone.h>

#include <list.h>
#include <stack.h>
#include <queue.h>
#include <number.h>
#include <bstring.h>

#include <cli.h>

/* @remark CLI prompt strings for 'hello.c' */
#define	SZ_BETAKIT_PROMPT	"BetaKit " CONFIG_BK_VERSION "> "
#define SZ_BETAKIT_HELLO	"Hello Betakit User!"

#define MAX_MENU_ITEMS		8

t_string menu_items[MAX_MENU_ITEMS] = {
  "Calculate Dhrystones",
  "Calculate Whetstones",
  "Say Hello to Betakit",
  "Test Stack",
  "Test Queue",
  "Test Numbers",
  "Test Strings",
  "Exit Menu"
};

#ifdef CONFIG_BK_SYS_JEMALLOC
t_memory_calls jemalloc;
#endif


/**
 * @fn void say_hello(void)
 * @brief just send a hello message
 *
 * @remark also tests memory allocation 
 *         functions of betakit
 */
void say_hello( void )
{
#ifdef CONFIG_BK_SYS_MEMORY
  t_u8  stringalloc[BERR_SZ_LEN];
  t_str str_hello, str_ptr;

  str_ptr     = (t_str) stringalloc;
  str_hello   = (t_str) mem_alloc(BERR_SZ_LEN);

  bk_strcpy( str_hello, SZ_BETAKIT_HELLO );
  bk_strcpy( str_ptr,   (const t_str)str_hello );

  printf( "Betakit says %s\n", str_hello );
  printf( "Betakit: Bye!\n" );
  printf( "Betakit:(\"%s\") \n", str_ptr );

  mem_free( str_hello );
#else
  printf("%s() requires memory management (SYS) support, currently disabled.\n", __FUNCTION__ );
#endif
  return;
}


/**
 * @fn void test_strings(void)
 * @brief test string functions in bstring.h
 */
void test_strings( void )
{
#ifdef CONFIG_BK_DS_STRING
  t_str str_one, str_two, str_num;
  t_s32 i_strlen;
  t_u32 u_num = BK_NUM_TEST;

  str_one = (t_str) mem_alloc( BK_SZ_LEN );
  str_two = (t_str) mem_alloc( BK_SZ_LEN );
  str_num = (t_str) mem_alloc( BK_SZ_LEN );
  
  bk_strcpy( str_one, SZ_BETAKIT_HELLO );
  bk_strrev( str_two, str_one );
  bk_ultostr( u_num, str_num );

  i_strlen = bk_strlen( str_one );

  printf("%s: string[1] = (\"%s\")\n",   __FUNCTION__, str_one );
  printf("%s: string[2] = (\"%s\")\n",   __FUNCTION__, str_two );
  printf("%s: num = %u  string = \"%s\"\n", __FUNCTION__, u_num, str_num );
  printf("%s: strlen(string[1]) = %d\n", __FUNCTION__, i_strlen );

  bk_tolower( str_one );
  bk_toupper( str_two );

  printf("%s: string[1] = tolower():\"%s\" string[2] = toupper():\"%s\"\n", 
	 __FUNCTION__, str_one, str_two );


  bk_atbash( str_one );
  printf("%s: atbash string: \"%s\"\n", __FUNCTION__, str_one );

  bk_trstr( str_two, 2 );
  printf("%s: rot2 string: \"%s\"\n", __FUNCTION__, str_two );

  mem_free( str_one );
  mem_free( str_num );
  mem_free( str_two );
#else
  printf("%s: library support for string/bstring (DS) disabled.\n", __FUNCTION__ );
#endif	/* CONFIG_BK_DS_STRING */
  return;
}



/**
 * @fn void test_stack(void)
 * @brief tests stack operations
 */
void test_stack( void )
{
#ifdef CONFIG_BK_DS_STACK
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
#else
  printf("%s: library support for stack (DS) disabled.\n", __FUNCTION__ );
#endif	/* CONFIG_BK_DS_STACK */
  return;		       
}

/**
 * @fn test_queue(void)
 * @brief tests the queue data structure
 */
void test_queue( void )
{
#ifdef CONFIG_BK_DS_QUEUE
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
#else
  printf("%s: library support for queue (DS) disabled.\n", __FUNCTION__ );
#endif	/* CONFIG_BK_DS_QUEUE */
  return;

}


/**
 * @fn test_number( void )
 * @brief tests numerical functions in library
 * @see number.c
 */
void test_number( void )
{
#ifdef CONFIG_BK_DS_NUMBER
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
#else
  printf("%s: library support for number (DS) disabled.\n",__FUNCTION__ );
#endif	/* CONFIG_BK_DS_NUMBER */ 
  return;
}

/**
 * @fn void measure_dhrystones( void )
 * @brief measure dhrystone MIPS
 * @remark tests dhrystone MIPS C/2.1 algorithm implementation
 * @see dhrystone.c
 */
void measure_dhrystones( void )
{
#ifdef CONFIG_BK_SYS_DHRYSTONES
  t_dhrystone_result dresult;
  calculate_dhrystone ( DHRYSTONE_TEST_TIME, &dresult );
  printf( "\t your machine can do %f MIPS which is %f VAX-MIPS taking %f uSecs per loop\n",
	  dresult.dhrystones, dresult.vaxmips, dresult.loop_usecs );
  printf( "\t that is %u loops in %f Seconds\n", (t_u32) dresult.total_loops, (t_f32) dresult.real_time_taken );
#else
  printf("%s: library support for dhrystones (SYS) disabled.\n", __FUNCTION__ );
#endif	/* CONFIG_BK_SYS_DHRYSTONES */
  return;
}

/**
 * @fn void measure_whetstones( void )
 * @brief measures whetstones per second 
 * @remark demonstrates whetstone measurment algorithm
 * @see whetstone.c
 */
void measure_whetstones( void )
{
#ifdef CONFIG_BK_SYS_WHETSTONES
  t_whetstone_result wresult;
  calculate_whetstones( WHETSTONE_TEST_TIME, &wresult );
  printf( "\t your machine can do %f Whetstone MIPS with %f mSecs per loop\n",
	  wresult.whetstones, wresult.loop_msecs );
  printf( "\t that is %u loops in %f Seconds\n", (t_u32) wresult.total_loops, (t_f32) wresult.real_time_taken );
#else
  printf("%s: library support for whetstones (SYS) disabled.\n", __FUNCTION__ );
#endif	/* CONFIG_BK_SYS_WHETSTONES */
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
#ifdef CONFIG_BK_UI_CLI

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
      }
      break;
    case 4:
      {
      	test_stack();
	retval = choice;
      }
      break;
    case 5:
      {
	test_queue();
	retval = choice;
      }
      break;
    case 6:
      {
	test_number();
	retval = choice;
      }
      break;
    case 7:
      {
	test_strings();
	retval = choice;
      }
      break;
    case MAX_MENU_ITEMS:
      {
	retval = CLI_EXIT_ACTION;
	break;
      }
    default:
      printf("%s: invalid choice\n", __FUNCTION__ );

    } /* switch( choice ) */

#else
  printf("%s: CONFIG_BK_UI_CLI not defined. No CLI support.\n", __FUNCTION__ );
#endif /* CONFIG_BK_UI_CLI */
  return( retval );
}


/**
 * @fn	init_menu(void)
 * @brief initialise CLI based menu and provide callback
 * @detail starts user interaction
 */
void init_menu( void )
{
#ifdef CONFIG_BK_UI_CLI
  t_menu_ptr main_menu;
  main_menu = cli_menu_init();

  printf("%s: init_menu called\n", __FUNCTION__ );

  cli_menu_addlist( main_menu, menu_items, MAX_MENU_ITEMS );
  cli_menu_callback( main_menu, &hello_menu );
  cli_set_prompt((t_str)SZ_BETAKIT_PROMPT);

  cli_menu_user( main_menu );
  return;
#else
  printf("%s: test requires CONFIG_BK_UI_CLI to be set.\n", __FUNCTION__ );
#endif
}

/**
 * @fn		__ctor_main(void)
 * @brief	manually called constructor for main()
 */
void __ctor_main( void )
{
#ifdef CONFIG_BK_SYS_JEMALLOC
  bk_jemalloc_calls( &jemalloc );
#endif

#ifdef CONFIG_BK_SYS_ERRORHANDLER
  _bk_errlog_init( BK_STDERR );
#else
  printf("%s: WARNING! SERIOUS! library does not have error handler.\n", __FUNCTION__ );
#endif
}

/**
 * @fn		__dtor_main(void)
 * @brief	manually called destructor for main()
 */
void __dtor_main( void )
{
#ifdef CONFIG_BK_SYS_ERRORHANDLER
  t_s32 tmp;
  if( 0 > (tmp =_bk_errlog_flush()) )
    {
      printf("%s: oops _bk_errlog_flush() failed with -0x%02x\n", __FUNCTION__, -tmp );
    }

  _bk_errlog_free();
#endif

#ifdef CONFIG_BK_SYS_MEMORY
  mem_gc();			/* @remark garbage collect */
#endif	/* CONFIG_BK_SYS_MEMORY */

}

/**
 * @fn int main(void)
 * @brief demonstrates CLI and error logging capabilities
 * @return 0 always
 * @remark check /tmp/berror.log for test error posts
 */
int main( void )
{
  __ctor_main();

  init_menu();

  __dtor_main();
  return(0);
}
#else /* CONFIG_BK_TEST_HELLO (undefined) */

/**
 * alternate main, to avoid
 * compiler botch-up.
 */
int main( void )
{
  printf("%s: CONFIG_BK_TEST_HELLO disabled\n", __FILE__ );
  return(0);
}


#endif	/* CONFIG_BK_TEST_HELLO */

/* @remark end of file "hello.c" */
