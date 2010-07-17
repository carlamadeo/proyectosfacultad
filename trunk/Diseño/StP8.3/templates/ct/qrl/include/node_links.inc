//////////////////////////////////////////////////////////////////////////////
//
// 	sccsid[] = @(#)node_links.inc	4.2  94/08/03
// 	StP/IM
// 	Confidential property of Interactive Development Environments, Inc.
// 	Copyright (c) 1992-1993
// 	All rights reserved
//
//	Count or get links in/out of a given node.
//
//////////////////////////////////////////////////////////////////////////////


// Count the number of links of a given type attached to a node.
// Direction is "to" || "from".

int
link_count(node nodeobj, string linktype, string direction)
{
    string query;

    query = "link[${linktype} && link_refs &&  " +
	    "${direction}_node_id == ${nodeobj.id}]";

    return(selection_count(query));
}

// Count all links of a given type (may be an expression) connected
// to nodeobj.  Direction is "to" || "from".

list
node_links(node nodeobj, string linktype, string direction)
{
    string query;

    query = "link[${linktype} && link_refs &&  " +
	    "${direction}_node_id == ${nodeobj.id}]";

    return(list_select(query));
}

// Count all links of a given type connected to nodeobj in the given direction
// and connect to a node of othernodetype in the other direction.
// Direction is "to" || "from".

int
link_count_node(node nodeobj, string linktype, string direction, string othernodetype)
{
    string query;
    string otherdir;

    if (direction == "to")
      otherdir = "from";
    else
      otherdir = "to";

    query = "link[${linktype} && link_refs &&  " +
	    "${direction}_node_id == ${nodeobj.id} && " +
	    "${otherdir}_node[${othernodetype}]]";

    return(selection_count(query));
}

