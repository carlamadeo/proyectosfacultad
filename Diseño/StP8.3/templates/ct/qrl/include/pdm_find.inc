//
// 	sccsid[] = %W% %Y% %D%
// 	StP/IM
// 	Confidential property of Interactive Development Environments, Inc.
// 	Copyright (c) 1992-1993
// 	All rights reserved
//

//  FILE:   pdm_find.inc
//
//  this file provides routines to find a pdm type by one of
//  its attributes in a list of pdm types. They all return the
//  index in the list where the match occurs or list_count if
//  not found. Arguments are assumed to be valid (e.g. no null
//  lists, start is in range, etc.).

    int
pdm_find_node_name( list the_nodes, int start, string the_name )
{
    int c, n;
    node the_node;

    for (c=list_count(the_nodes), n=start; n<c; n = n + 1)
    {
	the_node = list_get(the_nodes,n);
	if (the_node.name == the_name)
	    return n;
    }
    return c;
}
    // end of pdm_find_node_id()

    int
pdm_find_node_type( list the_nodes, int start, string the_type )
{
    int c, n;
    node the_node;

    for (c=list_count(the_nodes), n=start; n<c; n = n + 1)
    {
	the_node = list_get(the_nodes,n);
	if (the_node.type == the_type)
	    return n;
    }
    return c;
}
    // end of pdm_find_node_type()

    int
pdm_find_node_id( list the_nodes, int start, int the_id )
{
    int c, n;
    node the_node;

    for (c=list_count(the_nodes), n=start; n<c; n = n + 1)
    {
	the_node = list_get(the_nodes,n);
	if (the_node.id == the_id)
	    return n;
    }
    return c;
}
    // end of pdm_find_node_id()

    int
pdm_find_link_type( list the_links, int start, string the_type )
{
    int c, n;
    link the_link;

    for (c=list_count(the_links), n=start; n<c; n = n + 1)
    {
	the_link = list_get(the_links,n);
	if (the_link.type == the_type)
	    return n;
    }
    return c;
}
    // end of pdm_find_link_type()

    int
pdm_find_link_id( list the_links, int start, int the_id )
{
    int c, n;
    link the_link;

    for (c=list_count(the_links), n=start; n<c; n = n + 1)
    {
	the_link = list_get(the_links,n);
	if (the_link.id == the_id)
	    return n;
    }
    return c;
}
    // end of pdm_find_link_id()

    int
pdm_find_item_oid( list the_items, int start, int oid )
{
    int c, n;
    item the_item;

    for (c=list_count(the_items), n=start; n<c; n = n + 1)
    {
	the_item = list_get(the_items,n);
	if (the_item.obj_id == oid)
	    return n;
    }
    return c;
}
    // end of pdm_find_item_type()

    int
pdm_find_item_type( list the_items, int start, string the_type )
{
    int c, n;
    item the_item;

    for (c=list_count(the_items), n=start; n<c; n = n + 1)
    {
	the_item = list_get(the_items,n);
	if (the_item.type == the_type)
	    return n;
    }
    return c;
}
    // end of pdm_find_item_type()

    item
pdm_find_item( list the_items, string the_type )
{
    int c, n;
    item the_item;

    for (c=list_count(the_items), n=0; n<c; n = n + 1)
    {
	the_item = list_get(the_items,n);
	if (the_item.type == the_type)
	    return the_item;
    }
    return NULL;
}

