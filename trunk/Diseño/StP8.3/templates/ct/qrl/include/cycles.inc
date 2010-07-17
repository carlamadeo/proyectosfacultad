//////////////////////////////////////////////////////////////////////////////
//
// 	sccsid[] = @(#)cycles.inc	4.1  94/08/03
// 	StP/IM
// 	Confidential property of Interactive Development Environments, Inc.
// 	Copyright (c) 1992-1993
// 	All rights reserved
//
//	Detect link cycles.
//
//////////////////////////////////////////////////////////////////////////////


// Recursive function that takes a node, an extent (in_links || out_links)
// and a link type.  It follows the links until either the chain is 
// exhausted or a cycle is found.  "path" is used to return a list of nodes
// to any detected link cycle & visited is used to return to full list of
// visited nodes. Both lists should initially be passed empty.

boolean
cycles(node nodeobj, string extent, string linktype, list path, list visited)
{
    node   newnode;
    string query;

    if (list_find(path, 0, nodeobj) != list_count(path))
    {
	list_append(path, nodeobj);
	return(True);
    }
    list_append(path, nodeobj);
    list_append(visited, nodeobj);

    if (extent == "in_links")
    {
	query = "node[${extent}[${linktype} && link_refs && " +
		"from_node_id == ${nodeobj.id}]]";
    } else
    {
	query = "node[${extent}[${linktype} && link_refs && " +
		"to_node_id == ${nodeobj.id}]]";
    }
    for_each_in_select(query, newnode)
    {
	if (cycles(newnode, extent, linktype, path, visited))
	{
	    return(True);
	}
    }
    list_delete(path, list_count(path) - 1);
    return(False);
}

// Take a list of nodes and return a string of the concatenation of their 
// names separated by the separator string. 

string
nodelist_to_string(list thelist, string separator)
{
    int    i;
    node   nodeobj;
    string result;

    for(result = "", i = 0; i < list_count(thelist); i = i + 1)
    {
	nodeobj = list_get(thelist, i);

	if (i == 0)
	{
	    result = nodeobj.name;
	} else
	{
	    result = result + separator + nodeobj.name;
	}
    }
    return(result);
}

