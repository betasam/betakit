/**
 * @file graph.h
 * @brief support for graph data structure
 */

#ifndef _GRAPH_H_INC
#define _GRAPH_H_INC

#include <btypes.h>
#include <list.h>

struct graph_struct;
struct edge_struct;

typedef struct graph_struct t_graph;
typedef t_graph *t_graph_ptr;
typedef struct edge_struct t_edge;


struct graph_struct {

  t_ptr data;
  t_list_ptr connects;
  
};

struct edge_struct {

  t_graph_ptr graph;
  t_u32 weight;
  t_u8 direction;
};


#endif /* _GRAPH_H_INC */
