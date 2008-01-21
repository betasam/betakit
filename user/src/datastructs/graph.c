/**
 * @file graph.c
 * @brief abstract data structure for graphs
 */

/***
 * Copyright (c) 2005-2008 Sunil Beta Baskar <betasam@gmail.com>
 */

/* betakit includes */
#include <btypes.h>
#include <memory.h>
#include <list.h>
#include <graph.h>

/**
 * @fn t_graph_ptr graph_nodeinit( t_ptr data )
 * @brief creates a node or a subgraph for a graph
 */
t_graph_ptr graph_nodeinit( t_ptr data )
{
  t_graph_ptr new_graph_node;

  new_graph_node = 0;

  {
    new_graph_node = (t_graph_ptr) mem_alloc( sizeof(t_graph) );
    new_graph_node->data = data; /* ignore NULL */
  }

  return( new_graph_node );
}

void graph_nodefree( t_graph_ptr graph_ptr )
{
  mem_free((t_ptr)graph_ptr->data);
  mem_free((t_ptr)graph_ptr);
}

t_list_ptr graph_edgeinit( t_u32 u_weight )
{
  t_list *new_connect;
  t_edge *new_edge;

  new_connect = 0;
  new_edge = 0;

  new_edge = (t_edge*) mem_alloc( sizeof(t_edge) );
  new_connect = list_create_node( (t_ptr)new_edge );

  if( (0 != new_edge ) && (0 != new_connect ) )
  {  
    new_edge->weight = u_weight;
  }
  
  return( new_connect );
}


t_s32 graph_cmp( t_ptr graph_ptr_1, t_ptr graph_ptr_2 )
{
  return( (t_s32) ( (t_u32)graph_ptr_1 ^ (t_u32)graph_ptr_2 ) );
}


t_s32 graph_connect( t_graph_ptr graph_ptr_1, t_graph_ptr graph_ptr_2 )
{
  int ret_connected = -2;

  t_list *node_edge, *found_node;

  node_edge = found_node = 0;

  /* initiate the connections if they do not exist prior */
  if( 0 == graph_ptr_1->connects )
    {
      graph_ptr_1->connects = graph_edgeinit( 0 );
    }

  if( 0 == graph_ptr_2->connects )
    {
      graph_ptr_2->connects = graph_edgeinit( 0 );
    }

  /* connect edge if it(connection) does not already exist */
  node_edge = graph_ptr_1->connects->data;
  if( 0 == (found_node = list_find( node_edge, (t_ptr) graph_ptr_1, &graph_cmp )) )
    {
  
      if( 0 == list_add( (t_ptr) graph_ptr_1, node_edge ) )
	{
	  ret_connected++;
	}
    }
  else
    {
      if( found_node->data == (t_ptr) graph_ptr_1 )
	{
	  ret_connected++;
	}
    }

  /* connect edge if it(connection) does not already exist */
  node_edge = graph_ptr_2->connects->data;
  if( 0 == (found_node = list_find( node_edge, (t_ptr) graph_ptr_2, &graph_cmp )) )
    {

      if(0 == list_add( (t_ptr) graph_ptr_2, node_edge ) )
	{
	  ret_connected++;
	}
    }
  else
    {
      if( found_node->data == (t_ptr) graph_ptr_2 )
	{
	  ret_connected++;
	}
    }

  return( ret_connected );
}

/**
 * TODO
 * 1. Implement Breadth first Search
 * 2. Implement Depth first Search
 * 3. Implement Graph to Tree Conversion
 * 4. Implement Traversal Algorithms to Find Nodes
 * 5. Algorithm to discern structure as graph or tree
 *
 * 23-Dec-2007 <betasam@gmail.com>
 */
