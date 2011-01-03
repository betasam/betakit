/**
 * @file list.h
 * @author Sunil Beta <betasam@gmail.com>
 * @brief linked list abstraction header.
 * @see btypes.h
 *
 * This needs to be included by any application
 * using the linked list handler from betakit.
 */

/***
 * Copyright (c) 2008-2012 Sunil Beta Baskar <betasam@gmail.com>
 *
 **/

#ifndef _LIST_H_INC
#define _LIST_H_INC

#include <bkconfig.h>
#ifdef CONFIG_BK_DS_LIST

#include <btypes.h>

/* structure definition */
struct list_struct {
  t_ptr data;
  t_ptr next;
  t_ptr metadata;
};

/* type definitions */
typedef struct list_struct t_list;
typedef t_list* t_list_ptr;

/* macro definitions */
#define ERR_LIST_EMPTY		200
#define ERR_LIST_NODE_EMPTY	201
#define ERR_LIST_UNKNOWN	202
#define ERR_LIST_DATA_EMPTY	203
#define ERR_LIST_NODE_MISSING	204

/* function declarations */
t_list *list_create_node( t_ptr data );
t_s32 list_add( t_list *head, t_list *node );
t_s32 list_del( t_list *head, t_list *node );
t_list *list_find( t_list *head, t_ptr data_to_find, t_s32 (*data_cmp)( t_ptr data_in_list, t_ptr data_sought ) );
void list_clean( t_list *node );
t_s32 list_empty( t_list *head );

#endif	/* CONFIG_BK_DS_LIST */

#endif /* _LIST_H_INC */
