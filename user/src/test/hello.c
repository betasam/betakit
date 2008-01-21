/**
 * @file hello.c
 * @author Sunil Beta <betasam@gmail.com>
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

/* standard includes */
#include <stdio.h>

/* betakit includes */
#include <ops.h>
#include <btypes.h>

#include <memory.h>
#include <dhrystone.h>
#include <whetstone.h>

#include <list.h>

#include <cli.h>


t_string menu_items[4] = {
  "Calculate Dhrystones",
  "Calculate Whetstones",
  "Say Hello to Betakit",
  "Exit Menu"
};


/**
 * @fn void say_hello(void)
 * @brief just send a hello message
 *
 * tests memory allocation functions of betakit
 */
void say_hello( void )
{
  char *str_hello;
  str_hello = (t_string) (mem_alloc(256));
  sprintf( str_hello, "%s", "Hello Betakit User!");
  printf( "Betakit says %s\n", str_hello );
  printf( "Betakit: exit\n");
  mem_free( (t_ptr)str_hello );
  return;
}


/**
 * @fn void measure_dhrystones( void )
 * @brief measure dhrystone MIPS
 *
 * tests dhrystone mips c/2.1 algorithm implementation
 */
void measure_dhrystones( void )
{
  dhrystone_result_t result;
  calculate_dhrystone ( 5.0, &result );
  printf( "\t your machine can do %f MIPS which is %f VAX-MIPS taking %f uSecs per loop\n",
	  result.dhrystones, result.vaxmips, result.loop_usecs );
  printf( "\t that is %u loops in %f Seconds\n", (unsigned) result.total_loops, (float) result.real_time_taken );
  return;
}

/**
 * @fn void measure_whetstones( void )
 * @brief measures whetstones per second 
 *
 * demonstrates whetstone measurment algorithm
 */
void measure_whetstones( void )
{
  whetstone_result_t wresult;
  calculate_whetstones( 5.0, &wresult );
  printf( "\t your machine can do %f Whetstone MIPS with %f mSecs per loop\n",
	  wresult.whetstones, wresult.loop_msecs );
  printf( "\t that is %u loops in %f Seconds\n", (unsigned) wresult.total_loops, wresult.real_time_taken );
  return;
}


/**
 * @fn t_s32 hello_menu( t_s32 choice )
 * @brief callback function for CLI menu interface
 */
t_s32 hello_menu( t_s32 choice )
{
  t_s32 retval = 0;
 
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
  menu_t *main_menu;

  main_menu = cli_menu_init();

  cli_menu_addlist( main_menu, menu_items, 4 );
  cli_menu_callback( main_menu, &hello_menu );
  cli_set_prompt((t_string)"Betakit> ");

  cli_menu_user( main_menu );
 
  mem_gc();			/* garbage collect */
  return(0);
}
