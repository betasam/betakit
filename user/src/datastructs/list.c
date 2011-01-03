/**
 * @file list.c
 * @brief linked list data structure abstraction
 * @author Sunil Beta <betasam@gmail.com>
 * @details
 * Provides a linked list abstraction by allowing any type of
 * nodes as long as they are expressed as pointers in memory.
 *
 * @warning This code is not threadsafe.
 */

#include <bkconfig.h>
#ifdef CONFIG_BK_DS_LIST

/* @remark betakit includes */
#include <memory.h>
#include <btypes.h>

#include <list.h>

/**
 * @fn t_list *list_create_node( t_ptr data )
 * @brief create new list nodes
 * @details
 * Creates new linked list nodes, by allocating memory
 * and assigning data. 
 *
 * @warning This will fail if the data pointer is NULL.
 *
 * @param data Data to be contained in node.
 * @return A pointer to the new node.
 */
t_list *list_create_node( t_ptr data )
{
  t_s32 i_retval = 0; 
  t_list *list_node; 

  list_node = 0; /* @remark NULL node */

  if( 0 == data )
    {
      i_retval = -ERR_LIST_DATA_EMPTY;
    }

  if( 0 > i_retval )
    goto prep_exit;

  list_node = mem_alloc( sizeof(t_list) );
  list_node->data = data;

 prep_exit:
  return( list_node );
  
}

/**
 * @fn t_s32 list_add( t_list *head, t_list *node )
 * @brief adds a node to an existing list.
 * @details
 * Adds nodes to an existing list pointed by head
 * ensuring that the new node and head are existing 
 * however not considerate of the data contained in
 * either of the nodes whic could be NULL.
 *
 * @warning This code fails if the list is already 
 *          a circular list.
 *
 * @param head Head node of the linked list or any node
 *             in the list
 * @param node New node to be added to the list.
 * @return 0 on success and negative error 
 *         values on Failure.
 */
t_s32 list_add( t_list *head, t_list *node )
{

  t_s32 i_retval = 0;
  
  if( 0 == head ) 
    {
      i_retval = -ERR_LIST_EMPTY;
    }

  if( 0 == node )
    {
     i_retval = -ERR_LIST_NODE_EMPTY;
    }

  if( 0 > i_retval )
    goto add_exit;

  while( 0 != head->next )
    {
      head = head->next;
    }

  head->next = node;

 add_exit:
  return i_retval;

}

/**
 * @fn t_s32 list_del(t_list *head, t_list *node )
 * @brief deletes a node from a list.
 * @details
 * Deletes a given node from a list if the node is
 * present in the list pointed by head. This can
 * take care of circular lists also.
 *
 * @param head The head node of the linked list or 
 *             any node prior to the node to be deleted.
 * @param node The node that is to be deleted which is
 *             identified exclusively by its address.
 * @warning This will fail if the list has a loop on the
 *          traversal path to the node.
 * @return 0 on Success and negative error values on Failure.
 */
t_s32 list_del(t_list *head, t_list *node )
{

  t_s32 i_retval = 0;
  t_list *first;
  
  if( 0 == head ) 
    {
      i_retval = -ERR_LIST_EMPTY;
    }

  if( 0 == node )
    {
      i_retval = -ERR_LIST_NODE_EMPTY;
    }

  first = head;

  if( 0 > i_retval )
    goto del_exit;


  while( (0 != head->next) && (node != head->next ) && (head != head->next) && (first != head->next) )
    {
      head = head->next;
    }

  if( 0 == head->next )
    {
      i_retval = -ERR_LIST_NODE_MISSING;
      goto del_exit;
    }

  if( node == head->next )
    {
      head->next = node->next; /* @remark delink node */
    }

 del_exit:
  return i_retval;

}

/**
 * @fn t_list *list_find( t_list *head, t_ptr data_to_find, 
                          t_s32 (*data_cmp)( t_ptr data_in_list, t_ptr data_sought ) )
 * @brief Finds a node based on the data found in the list.
 * @details
 * Searches a linked list for a node based on the data. The data 
 * comparison is done by a callback function as this code is not
 * aware of the nature of the data present within the list. This
 * code can also handle circular lists.
 *
 * @param head The Head of the linked list or any such prior node.
 * @param data_to_find A pointer to the data to be located within the list.
 * @param data_cmp A function (pointer) to compare data in each node
 *                 with data_to_find.
 * 
 * @return A pointer to a node if found or NULL on failure.
 */
t_list *list_find( t_list *head, t_ptr data_to_find, t_s32 (*data_cmp)( t_ptr data_in_list, t_ptr data_sought ) )
{
  t_s32 i_retval = 0;
  t_list *first;

  if( 0 == head ) 
    {
      i_retval = -ERR_LIST_EMPTY;
      goto find_exit;
    }
 
  first = head;
  
  /* @remark linear search */
  while( head && (head->next != first) && (head->next != head) )
    {
      if( 0 != head->data )
	{
	  if( 0 == data_cmp( head->data, data_to_find ) )
	    {
	      return( head );
	    }
	}
      head = head->next;
    }

 find_exit:
  return(0);
}


/**
 * @fn void list_clean( t_list *node )
 * @param node the node to be freed.
 * @brief Frees memory from a single node in the list
 * @see mem_free
 * @details
 * Frees memory used up by a single node using a
 * dynamic memory management tool.
 *
 * @return void/nothing.
 */
void list_clean( t_list *node )
{

  if( 0 == node )
    {
      return;
    }

  mem_free(node->data);
  mem_free(node->next);
  mem_free(node->metadata);

  return;
}

/**
 * @fn t_s32 list_empty( t_list *head )
 * @param head The head node of the linked list.
 * @brief Destroys a linked list from head leaving
 *        the head of the list.
 * @see list_clean
 * @details
 * Cleans up an entire linked list except for its
 * Head node. The Head node can be cleaned up later
 * by calling list_clean.
 *
 * @return 0 on Success and negative UNKNOWN error 
 *         on failure.
 */
t_s32 list_empty( t_list *head )
{
  int i_retval = 0;
  t_list *node, *first;

  if( 0 == head ) 
    {
      i_retval = -ERR_LIST_EMPTY;
      goto empty_exit;
    }

  first = head;

  while( head->next && (head->next != first) && (head->next != head) )
    {
      node = head->next;
      if( 0 > list_del( head, (t_list*) head->next ) )
	{
	  i_retval = -ERR_LIST_UNKNOWN;
	  goto empty_exit;
	}
      list_clean(node);
    }

  mem_free(head->data);

 empty_exit:
  return i_retval;
}

#endif	/* CONFIG_BK_DS_LIST */
/* @remark end of file "list.c" */
