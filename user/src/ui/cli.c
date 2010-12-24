/**
 * @file cli.c command line interface
 * @author Sunil Beta Baskar <betasam@gmail.com>
 * @date 2005-2009
 * @brief Command Line Interface Abstraction
 *
 * @todo  Destructor for Menu Structure 
 * @todo  Abstract support for sub-menus 
 */



/** @remark standard includes */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/** @remark betakit includes */
#include <btypes.h>
#include <memory.h>
#include <list.h>

#include <cli.h>


static char cli_prompt[BKIT_SZLEN_DEFAULT];

/**
 * @fn t_s32 cli_set_prompt( t_string new_prompt )
 * @brief set the prompt for the command-line-environment
 * @param new_prompt - pointer to a SZ string 
 * @return 0 on success, -ve on error
 */
t_s32 cli_set_prompt( t_string new_prompt )
{
  t_s32 retval;

  retval = -1;

  if( 0 == new_prompt )
    {
      return( retval );
    }
  retval++;

  strcpy( cli_prompt, new_prompt );
  return( retval );
}

/**
 * @fn t_menu *cli_menu_init(void)
 * @brief initialise a menu
 * @return pointer on success, NULL on failure
 */
t_menu *cli_menu_init( void )
{
  t_s32 retval = -1;
  t_menu *menu_ptr;

  menu_ptr = (t_menu*) mem_alloc( sizeof(t_menu) );


  if( NULL == menu_ptr ) 
    {
      return(NULL);
    }

  menu_ptr->menu_count = 0;
  menu_ptr->menu_choices = 0;
  menu_ptr->last_item = 0;
  menu_ptr->current_item = 0;
  menu_ptr->menu_title = (t_string)(mem_alloc( BKIT_SZLEN_DEFAULT ));

  strcpy( menu_ptr->menu_title, (t_string)" Main Menu ");

  retval++;

  return( menu_ptr );  
}

/**
 * @fn cli_menu_title( t_menu *menu_ptr, t_string new_title )
 * @brief set the menu title
 * @param menu_ptr pointer to the menu for setting title
 * @param new_title SZ string containing title 
 * @return 0 on success, -ve on failure
 */
t_s32 cli_menu_title( t_menu *menu_ptr, t_string new_title )
{
  t_s32 retval = -2;

  if( 0 == menu_ptr )
    {
      return( retval );
    }
  retval++;

  if( 0 == new_title )
    {
      return( retval );
    }

  retval++;

  strcpy( menu_ptr->menu_title, (char*)new_title);

  return( retval );
}

/**
 * @fn cli_menu_additem( t_menu_ptr menu_ptr, t_string new_item )
 * @brief add an item to the menu for the CLI
 * @param menu_ptr pointer to cli menu
 * @param new_item SZ string with entry to be added to menu
 * @return 0 on success, -ve on failure
 */
t_s32 cli_menu_additem( t_menu_ptr menu_ptr, t_string new_item )
{
  t_list_ptr cur_menu_item = (t_list_ptr)0;
  t_list_ptr new_menu_item = (t_list_ptr)0;
  t_s32 retval = -2;
  t_s32 counter = 0;

  if( 0 == menu_ptr )
    {
      return( retval );
    }
  retval++;

  if( 0 == (t_ptr)new_item )
    {
      return( retval );
    }

  retval++;

  new_menu_item = (t_list*)list_create_node( (t_ptr) new_item );
  
  if( ((t_ptr)0 == (t_ptr)menu_ptr->menu_count) )
    {
      menu_ptr->menu_choices = (t_ptr) new_menu_item;
      retval = menu_ptr->menu_count++;
    }
  else
    {
      cur_menu_item = (t_ptr) menu_ptr->menu_choices;
      for( counter=0; counter < menu_ptr->menu_count; counter++ )
	{
	  if( 0 != cur_menu_item->next )
	    {
	      cur_menu_item = (t_ptr) cur_menu_item->next;
	    }
	  else
	    {
	      menu_ptr->menu_count = counter + 1;
	      goto choice_confirm;
	    }
	}
    choice_confirm:
      cur_menu_item->next = (t_ptr) new_menu_item;
      menu_ptr->menu_count++;
    }
  return( retval );
}


/**
 * @fn cli_menu_dadlist( t_menu_ptr menu_ptr, t_str *menuitem_list, t_s32 count )
 * @param menu_ptr - pointer to menu
 * @param menuitem_list - array of SZ strings
 * @param count - number of menu items
 * @return 0 on success, -ve on failure
 */
t_s32 cli_menu_addlist( t_menu_ptr menu_ptr, t_string *menuitem_list, t_s32 count )
{
  t_s32 curitem;
  t_s32 retval = 0;

  retval = -2;
  if( 0 == menu_ptr )
    {
      return(retval);
    }
  retval++;

  if( 0 == menuitem_list )
    {
      return(retval);
    }
  retval++;


  for(curitem = 0; curitem < count; curitem++)
    {
      cli_menu_additem( menu_ptr, menuitem_list[ curitem ] );
    }

  return( retval );
}


/**
 * @fn cli_menu_callback
 * @brief add callback function to menu 
 * @param menu_ptr - pointer to menu being referenced
 * @param menu_callback - function pointer that processes callbacks
 * @return 0 on success, -ve on failure
 */
t_s32 cli_menu_callback(t_menu *menu_ptr, t_s32 (*menu_callback)(t_s32 choice) )
{
  t_s32 retval = -1;
  
  if( NULL == menu_ptr )
    {
      return( retval );
    }
  retval++;
  
  menu_ptr->menu_action = menu_callback;
  
  return( retval );
}


/**
 * @fn cli_show_menu( t_menu *menu_ptr )
 * @brief displays the menu
 * @param menu_ptr - pointer to an initialised menu
 * @remark this does not wait for user input
 * @return 0 on success, -ve on failure
 */
t_s32 cli_show_menu( t_menu *menu_ptr )
{
  t_s32 retval = -3;
  t_s32 curitem;
  t_s32 count;
  t_list_ptr menuitem_ptr;

  menuitem_ptr = NULL;
  
  if( NULL == menu_ptr )
    {
      return( retval );
    }

  retval++;
  
  if( 0 == menu_ptr->menu_count )
    {
      return( retval );
    }

  count = menu_ptr->menu_count;
  retval++;

  menuitem_ptr = menu_ptr->menu_choices;

  if( NULL == menuitem_ptr )
    {
      return(retval);
    }
  retval++;

  printf("%s %s %s\n\n", BK_MENU_HEADER_PRE, menu_ptr->menu_title,
	 BK_MENU_HEADER_AFT);

  for( curitem = 0; curitem < count; curitem++ )
    {
      printf("(%2d) %s \n", (curitem+1), (t_string)(menuitem_ptr->data));
      menuitem_ptr = menuitem_ptr->next;
      if( 0 == menuitem_ptr )
	{
	  break;
	}
    }

  return( retval );
  
}

/**
 * @fn cli_menu_user( t_menu *menu_ptr )
 * @brief display menu and wait on user input
 * @param menu_ptr - pointer of initialised menu
 * @return 0 on success, -ve on failure
 * @remark this will wait for user input
 */
t_s32 cli_menu_user( t_menu* menu_ptr )
{
  t_s32 retval;
  t_byte response[255] = "";
  t_s32 choice;
  t_s32 action = ZERO;

  retval = -3;

  if( NULL == menu_ptr )
    {
      return( retval );
    }

  retval++;			/* retval = -2 */
  
  if( 0 == menu_ptr->menu_count )
    {
      return( retval );
    }

  retval++;			/* retval = =1 */

  if( NULL == menu_ptr->menu_action )
    {
      return( retval );
    }

  retval++;			/* retval = 0 */

  while( (action != CLI_EXIT_ACTION) && 
	 (0 != (strcmp( (t_str) response, (t_str)CLI_EXIT_STRING ))) )
    {
      cli_show_menu( menu_ptr );
      printf("\n");
      printf("%s", cli_prompt );
      /** @todo find a better alternative to scanf  */
      if( EOF == scanf("%s", (t_str) response ) ) 
	{
	  printf("cli: error reading user input.\n");
	  return(retval = -4);
	}
      choice = atoi( (t_str) response );
      if( ZERO != choice )
	{
	  action = menu_ptr->menu_action( choice );
	}
    }

  return( retval );
}

/** @remark end of file "cli.c" */
