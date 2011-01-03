/**
 * @file cli.h
 * @brief command line interface header
 * @author Sunil Beta Baskar <betasam@gmail.com>
 * @date 2008-2012
 */

/*
 * Copyright (c) 2008-2012 Sunil Beta Baskar <betasam@gmail.com>
 */

#ifndef _CLI_H_INC
#define _CLI_H_INC

#include <bkconfig.h>

#ifdef CONFIG_BK_UI_CLI

#include <btypes.h>
#include <list.h>

#define BK_MENU_HEADER_PRE	"-=[ "
#define BK_MENU_HEADER_AFT	" ]=-"

/* data structure definitions */
struct menu_s {

  t_s32 current_item;
  t_s32 last_item;
  t_list_ptr menu_choices;
  t_s32 (*menu_action)( t_s32 );
  t_s32 menu_count;
  t_string menu_title;

};

struct cli_s {

  t_list_ptr cli_choices;
  t_s32 (*cli_action) ( t_ptr command, t_list_ptr token_arguments );
  t_byte *cli_prompt;
  t_byte *cli_delimiters;
  t_u32 flags;

};

struct token_s {
  t_byte *token;
  t_ptr value;
  t_s32 type;
};

/* type definitions based on structures defined */
typedef struct menu_s t_menu;
typedef t_menu* t_menu_ptr;
typedef struct cli_s t_cli;
typedef t_cli* t_cli_ptr;
typedef struct token_s t_token;
typedef t_token* t_token_ptr;

/* macro definitions */
#define CLI_MENUITEM_SIZE	100
#define CLI_QUIT_STRING		"Q"
#define CLI_EXIT_STRING		"X"
#define CLI_EXIT_ACTION		-4

#define CLI_FLAG_CASESENSITIVE	1
#define CLI_FLAG_IGNORECASE	2
#define CLI_FLAG_BLOCK		4
#define CLI_FLAG_MULTILINE	8

#define CLI_TOKEN_REMARK	254
#define CLI_TOKEN_COMMAND	255
#define CLI_TOKEN_STR_PARM	256
#define CLI_TOKEN_INT_PARM	257
#define CLI_TOKEN_FP_PARM	258
#define CLI_TOKEN_DELIMIT	259
#define CLI_TOKEN_EXIT		260

/* function declarations */
t_s32 cli_set_prompt( t_string new_prompt );
t_menu *cli_menu_init( void );
t_s32 cli_t_menuitle( t_menu_ptr menu_ptr, t_string new_title );
t_s32 cli_menu_additem( t_menu_ptr menu_ptr, t_string new_item );
t_s32 cli_menu_addlist( t_menu_ptr menu_ptr, t_string *menuitem_list, t_s32 count );
t_s32 cli_menu_callback(t_menu_ptr menu_ptr, t_s32 (*menu_callback)(t_s32 choice) );
t_s32 cli_show_menu( t_menu_ptr menu_ptr );
t_s32 cli_menu_user( t_menu_ptr menu_ptr );

#endif	/* CONFIG_BK_UI_CLI */

#endif /* _CLI_H_INC */
