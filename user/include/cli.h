/**
 * @file cli.h
 * @brief command line interface header
 * @author Sunil Beta Baskar <betasam@gmail.com>
 */

/*
 * Copyright (c) 2005-2008 Sunil Beta Baskar <betasam@gmail.com>
 */

#ifndef _CLI_H_INC
#define _CLI_H_INC

#include <btypes.h>
#include <list.h>

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
typedef struct menu_s menu_t;
typedef menu_t *menu_t_ptr;
typedef struct cli_s cli_t;
typedef cli_t *cli_t_ptr;
typedef struct token_s token_t;
typedef token_t *token_t_ptr;

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
menu_t *cli_menu_init( void );
t_s32 cli_menu_title( menu_t *menu_ptr, t_string new_title );
t_s32 cli_menu_additem( menu_t *menu_ptr, t_string new_item );
t_s32 cli_menu_addlist( menu_t* menu_ptr, t_string *menuitem_list, t_s32 count );
t_s32 cli_menu_callback(menu_t *menu_ptr, t_s32 (*menu_callback)(t_s32 choice) );
t_s32 cli_show_menu( menu_t *menu_ptr );
t_s32 cli_menu_user( menu_t* menu_ptr );

#endif /* _CLI_H_INC */
