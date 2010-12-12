/**
 * @file graph.c
 * @author Sunil Beta Baskar <betasam@gmail.com>
 * @date 2009-2012
 * @brief abstract data structure for graphs
 *
 */


/* @remark betakit includes */
#include <ops.h>
#include <btypes.h>
#include <berror.h>

#include <memory.h>
#include <list.h>
#include <graph.h>

/**
 * @fn t_graph_ptr graph_nodeinit( t_ptr data )
 * @brief creates a node or a subgraph for a graph
 * @param data - pointer for initialising data in node
 * @return pointer to graph on success, NULL on failure
 */
t_node_ptr node_init( t_ptr data )
{
  t_node_ptr  new_node = NULL;

  if( NULL == (new_node = (t_node_ptr) mem_alloc( sizeof(t_node) )) )
    {
      return( new_node ); /** @TODO - Report error */
    }
    new_node->node_flags = BKIT_NODE_UNDEF;
    new_node->node_data = data; /** @WARNING - Pointer Assignment */
    new_node->edge_links = NULL;
    new_node->node_data_compare = NULL;
    new_node->node_flags = BKIT_NODE_INIT;

  return( new_node );
}

/**
 * @fn node_free( t_node_ptr node_ptr )
 * @brief free a node in the graph
 * @param node_ptr - pointer to the node, type t_node_ptr
 * @warning does not check graph for safety
 *	    or for connectivity.
 */
t_void node_free( t_node_ptr node_ptr )
{
  mem_free((t_ptr)node_ptr->node_data);
  mem_free((t_ptr)node_ptr);
}


/**
 * @fn t_s32 node_register( t_node_ptr node_ptr, t_s32 (*compare)(t_ptr data1, t_ptr data2) )
 * @param node_ptr - pointer to a node
 * @param compare  - function pointer that compares two data pointers
 * @return 0 on success, or -ve on failure; code explanatory
 */
t_s32 node_register( t_node_ptr node_ptr, t_s32 (*compare)(t_ptr data1, t_ptr data2) )
{
  t_s32 retval = 0;

  /* ERROR! one of the inputs is NULL */
  if( (NULL == node_ptr) || (NULL == compare) )
    {
      return( retval = -(BERR_SEVERE|BERR_NOMEM) );
    }

  /* ERROR! function already registered */
  if( node_ptr->node_data_compare != NULL )
    {
      return( retval = -(BERR_INVALID) );
    }

  /* SUCCESS! assignment follows */
  node_ptr->node_data_compare = compare;

  return( retval );
}

/**
 * @fn t_s32 node_unregister( t_node_ptr node_ptr )
 * @brief unregister data compare function from node
 * @param node_ptr - pointer to a node, type t_node_ptr
 * @return 0 on success, -ve on failure
 */
t_s32 node_unregister( t_node_ptr node_ptr )
{
  t_s32 retval = 0;
  if ( NULL == node_ptr )
    {
      /* NULL ptr error */
      return( retval = -(BERR_SEVERE|BERR_NOMEM) );
    }

  if( NULL == node_ptr->node_data_compare )
    {
      /* data function not registered */
      return( retval = -(BERR_NORMAL|BERR_NOMEM) );
    }

  node_ptr->node_data_compare = NULL;
  
  return( retval );
}

/**
 * @fn t_s32 node_compare(t_node_ptr node_1, t_node_ptr node_2)
 * @brief compare data across two nodes if nodes 
 * @param node_1 - pointer to a node of type t_node_ptr
 * @param node_2 - pointer to a node of type t_node_ptr
 * @return 0 on success, -ve on failure
 */
t_s32 node_compare( t_node_ptr node_1, t_node_ptr node_2 )
{
  t_s32 retval = ZERO;

  /* check if nodes are valid */
  if( (NULL == node_1) || (NULL == node_2) )
    {
      retval = -(BERR_NOMEM | BERR_SEVERE);
    }

  /* check if nodes contain valid data */
  if( (NULL == node_1->node_data) || (NULL == node_2->node_data) )
    {
      retval = -(BERR_NOMEM | BERR_NORMAL);
      return( retval );
    }

  /* is node_1 == node_2, in which case call node_self_connect() */
  if( node_1 == node_2 )
    {
      retval = -(BERR_INVALID | BERR_NORMAL);
      return( retval );
    }
  
  /* check if nodes have registered data compare functions */
  if( (NULL == node_1->node_data_compare) || (NULL == node_2->node_data_compare) )
    {
      retval = -(BERR_CRASH | BERR_NORMAL);
      return( retval );
    }

  /* check if data compare functions are equivalent */
  if( node_1->node_data_compare != node_2->node_data_compare )
    {
      retval = -(BERR_INVALID | BERR_NORMAL);
      return( retval );
    }

  /* run data compare */
  retval = node_1->node_data_compare( node_1->node_data, node_2->node_data );

  return( retval );
}


/**
 * @fn t_s32 is_node_connected( t_node_ptr alpha_node, t_node_ptr beta_node )
 * @brief check if nodes alpha_node and beta_node are connected
 * @param alpha_node pointer to a node, type t_node_ptr
 * @param beta_node  pointer to a node, type t_ndoe_ptr
 * 
 */
t_s32 is_node_connected( t_node_ptr alpha_node, t_node_ptr beta_node )
{
  t_s32 retval = 0;

  t_list_ptr edge_head = NULL;
  t_list_ptr edge_curr, edge_prev;

  if( (NULL == alpha_node) || (NULL == beta_node) )
    {
      return(retval = -(BERR_INVALID | BERR_SEVERE));
    }

  if( (NULL == alpha_node->edge_links) || (NULL ==  beta_node->edge_links) )
    {
      return(retval = -(BERR_NOMEM | BERR_SEVERE) );
    }

  if( alpha_node == beta_node )
    {
      return(retval = -(BERR_INVALID | BERR_NORMAL) );
    }

  edge_curr = edge_head = alpha_node->edge_links;
  edge_prev = edge_curr;

  while( NULL != edge_curr )
    {
      if( edge_curr->data == (t_ptr)beta_node )
	{
	  return( retval = 1 );	/* edge already connected */
	}

      edge_prev = edge_curr;
      edge_curr = edge_prev->next;

      if( edge_curr == alpha_node->edge_links )
	{
	  /* Last edge connects back to same node */
	  return( retval = -3 );
	}
    }

  return( retval = 2);
}


/**
 * @fn node_connect( t_node_ptr alpha_node, t_node_ptr beta_node )
 * @brief connect alpha_node and beta_node if they are not the same
 * @param alpha_node - pointer to a node, type t_node_ptr
 * @param beta_node  - pointer to a node, type t_node_ptr
 * @return -ve on failure, >= 0 on success
 */
t_s32 node_connect( t_node_ptr alpha_node, t_node_ptr beta_node )
{
  t_s32 retval = 0;
  t_list_ptr edge_curr, edge_prev, edge_new;

  /* either one of the nodes is NULL, error! */
  if( (NULL == alpha_node) || (NULL == beta_node) )
    {
      return(retval = -(BERR_INVALID | BERR_SEVERE));
    }

  /* both nodes are the same, error! */
  if( alpha_node == beta_node )
    {
      return(retval = -(BERR_INVALID | BERR_NORMAL));
    }

  /* both nodes have no initialised connections, connect! */
  if( (NULL == alpha_node->edge_links) && (NULL ==  beta_node->edge_links) )
    {
      alpha_node->edge_links = list_create_node( (t_ptr) beta_node );
       beta_node->edge_links = list_create_node( (t_ptr)alpha_node ); 
      return( retval = 0 );
    }

  /* alpha_node does not have a connection list, connect! */
  if( NULL == alpha_node->edge_links ) /* beta_node->edge_links != NULL */
    {
      alpha_node->edge_links = list_create_node( (t_ptr) beta_node );
      goto connectbeta;
    }

  /* beta_node does not have a connection list, connect! */
  if( NULL == beta_node->edge_links ) /* alpha_node->edge_links != NULL */
    {
      beta_node->edge_links = list_create_node( (t_ptr) alpha_node );
    }

  edge_curr = alpha_node->edge_links;
  if( edge_curr->data == (t_ptr) beta_node )
    {
      goto connectbeta;
    }

  /* nodes are not connected */
  if( _IS_ERROR(is_node_connected( alpha_node, beta_node )) )
    {
      while( NULL != edge_curr )
	{

	  if( NULL == edge_curr->next )
	    {
	      edge_new = list_create_node( (t_ptr)beta_node );
	      edge_new->next = NULL;
	      edge_curr->next = edge_new;
	      retval = 2;
	      goto connectbeta;
	    }

	  edge_prev = edge_curr;
	  edge_curr = edge_prev->next;

	  if( edge_curr == alpha_node->edge_links )
	    {
	      edge_new = list_create_node( (t_ptr)beta_node );
	      edge_new->next = edge_curr;
	      edge_prev->next= edge_new;
	      goto connectbeta;
	    }
	}

    connectbeta:

      edge_curr = beta_node->edge_links;
      if( edge_curr->data == (t_ptr) alpha_node )
	{
	  goto leave;
	}
      edge_prev = edge_curr;
      edge_new  = NULL;

      while( NULL != edge_curr )
	{
	  if( NULL == edge_curr->next )
	    {
	      edge_new = list_create_node( (t_ptr)alpha_node );
	      edge_curr->next = edge_new;
	      retval |= 4;
	      goto leave;
	    }

	  edge_prev = edge_curr;
	  edge_curr = edge_prev->next;

	  if( edge_curr == beta_node->edge_links )
	    {
	      edge_new = list_create_node( (t_ptr)alpha_node );
	      edge_new->next = edge_curr;
	      edge_prev->next = edge_new;
	      retval |= 8;
	      goto leave;
	    }
	}            
    }

 leave:
  return( retval );

}


/**
 * @fn t_s32 node_self_connect( t_node_ptr node_ptr )
 * @brief connect node to itself in at least one edge
 * @param node_ptr - a pointer to a node, type t_node_ptr
 * @return -ve on error, >= 0 on success
 */
t_s32 node_self_connect( t_node_ptr node_ptr )
{
  t_s32 retval = 0;
  t_list_ptr edge_head, edge_curr, edge_new, edge_prev;

  if( NULL == node_ptr )
    {
      return( retval = -(BERR_NOMEM|BERR_SEVERE) );
    }

  if( NULL == node_ptr->edge_links )
    {
      edge_new = list_create_node( (t_ptr) node_ptr );
      node_ptr->edge_links = edge_new;
      if( NULL == node_ptr->edge_links ) 
	{
	  retval = -(BERR_UNKNOWN|BERR_SEVERE);
	}
      return( retval );
    }

  edge_head = node_ptr->edge_links;
  edge_curr = edge_prev = edge_head;
  retval = -(BERR_UNKNOWN|BERR_NORMAL);

  while( NULL != edge_curr )
    {

      if( edge_curr->data == (t_ptr) node_ptr )
	{
	  retval = -(BERR_INVALID|BERR_NORMAL);
	  return( retval );	/* invalid! already exists */
	}

      if( NULL == edge_curr->next )
	{
	  edge_new = list_create_node( (t_ptr) node_ptr );
	  edge_curr->next = edge_new;
	  edge_new->next  = NULL;
	  retval = 2;
	  return( retval );
	}

      if( edge_curr->next == edge_head )
	{
	  edge_new = list_create_node( (t_ptr) node_ptr );
	  edge_new->next = edge_head;
	  edge_curr->next = edge_new;
	  retval = 4;
	  return( retval );	
	}

      edge_prev = edge_curr;
      edge_curr = edge_curr->next;
    }

  return( retval );
}

/**
 * @TODO
 *	provide basic options to connect graphs
 *	implement search algorithms - BFS, DFS
 *	report all memory allocation errors for 
 *	node_init() and edge_init() functions.
 */

/* @remark End of file "graph.c" */
