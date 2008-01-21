/**
 * @Filex cli.c
 * @author Sunil Beta Baskar <betasam@gmail.com>
 * @brief Command Line Interface Abstraction
 */

/* standard includes */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* betakit includes */
#include <btypes.h>
#include <memory.h>
#include <list.h>

#include <cli.h>

static char cli_prompt[32];

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

menu_t *cli_menu_init( void )
{
  t_s32 retval = -1;
  menu_t *menu_ptr;

  menu_ptr = (menu_t*) mem_alloc( sizeof(struct menu_s) );

  if( 0 == menu_ptr ) 
    {
      return(NULL);
    }

  menu_ptr->menu_count = 0;
  menu_ptr->menu_choices = 0;
  menu_ptr->last_item = 0;
  menu_ptr->current_item = 0;
  menu_ptr->menu_title = (t_string)(mem_alloc( 100 ));

  strcpy( menu_ptr->menu_title, (t_string)" Main Menu ");

  retval++;

  return( menu_ptr );  
}

t_s32 cli_menu_title( menu_t *menu_ptr, t_string new_title )
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

t_s32 cli_menu_additem( menu_t *menu_ptr, t_string new_item )
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


t_s32 cli_menu_addlist( menu_t* menu_ptr, t_string *menuitem_list, t_s32 count )
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


t_s32 cli_menu_callback(menu_t *menu_ptr, t_s32 (*menu_callback)(t_s32 choice) )
{
  t_s32 retval = -1;
  
  if( 0 == menu_ptr )
    {
      return( retval );
    }
  retval++;
  
  menu_ptr->menu_action = menu_callback;
  
  return( retval );
}

t_s32 cli_show_menu( menu_t *menu_ptr )
{
  t_s32 retval = -3;
  t_s32 curitem;
  t_s32 count;
  t_list_ptr menuitem_ptr;

  menuitem_ptr = 0;
  
  if( 0 == menu_ptr )
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

  if( 0 == menuitem_ptr )
    {
      return(retval);
    }
  retval++;

  printf("-=< %s >=-\n\n", menu_ptr->menu_title );

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


t_s32 cli_menu_user( menu_t* menu_ptr )
{
  t_s32 retval;
  t_byte response[255] = "";
  t_s32 choice;
  t_s32 action = 0;

  retval = -3;

  if( 0 == menu_ptr )
    {
      return( retval );
    }

  retval++;
  
  if( 0 == menu_ptr->menu_count )
    {
      return( retval );
    }

  retval++;

  if( 0 == menu_ptr->menu_action )
    {
      return( retval );
    }

  retval++;

  while( (action != CLI_EXIT_ACTION) && (0 != (strcmp( (char*) response, (char*)CLI_EXIT_STRING ))) )
    {
      cli_show_menu( menu_ptr );
      printf("\n");
      printf("%s", cli_prompt );
      scanf("%s", (char*)response );
      choice = atoi( (char*)response );
      if( 0 != choice )
	{
	  action = menu_ptr->menu_action( choice );
	}
    }

  return( retval );
}

/*
 * TODO
 * 1. Destructor for Menu Structure
 * 2. Abstract support for sub-menus
 */
