/**
 * @file graph.h
 * @author Sunil Beta Baskar <betasam@gmail.com>
 * @brief support for graph data structure
 */

#ifndef _GRAPH_H_INC
#define _GRAPH_H_INC

/* basic type includes */
#include <btypes.h>
#include <list.h>

/* macro definitions */
#define BKIT_NODE_UNDEF		0xFE
#define	BKIT_NODE_INIT		0x01

/* structure definitions */
struct node_struct {
  t_ptr node_data;
  t_list_ptr edge_links;
  t_u8 node_flags;
  t_s32 (*node_data_compare)(t_ptr data1, t_ptr data2);
};

/* type definitions */
typedef struct node_struct t_node;
typedef t_node* t_node_ptr;


/* function declarations */
t_node_ptr node_init( t_ptr data );
t_void node_free( t_node_ptr node_ptr );
t_s32 node_register( t_node_ptr node_ptr, t_s32 (*compare)(t_ptr data1, t_ptr data2) );
t_s32 node_unregister( t_node_ptr node_ptr );
t_s32 node_compare( t_node_ptr node_1, t_node_ptr node_2 );
t_s32 is_node_connected( t_node_ptr alpha_node, t_node_ptr beta_node );
t_s32 node_connect( t_node_ptr alpha_node, t_node_ptr beta_node );
t_s32 node_self_connect( t_node_ptr node_ptr );


#endif /* @remark _GRAPH_H_INC */
