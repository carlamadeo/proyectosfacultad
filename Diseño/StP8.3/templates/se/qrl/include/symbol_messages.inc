//
//      sccsid[] = %W% %Y% %D%
//      StP/SE
//      Confidential property of Interactive Development Environments, Inc.
//      Copyright (c) 1992-1993
//      All rights reserved
//


//////////////////////////////////////////////////////////////////////////////
//
//
//    File: symbol_messages.inc
//
//    Description:
//
//      This is an extension to the functions found in appids.inc. 
//	node_error, link_error, and cntx_error have been extended
//	to allow an optional string to precede the node type and name
//	so that messages may contain an error or warning number.
//
//////////////////////////////////////////////////////////////////////////////

#include "qrl/include/appids.inc"

// Issue a symbol error of the form ${msg1} ${node.type} '${node.name}' ${msg2}

void
node_error_num(string msg1, node node, string msg2, file file)
{
    ide_error(msg1 + app_type_print_string(node) + qt_node_name(node) + msg2,
	  	node_appids(node, file)); 
}

void
node_warning_num(string msg1, node node, string msg2, file file)
{
    ide_warning(msg1 + app_type_print_string(node) + qt_node_name(node) + msg2,
		node_appids(node, file));
}

// Issue a symbol error of the form ${msg1} ${link.type} '${link.name}' ${msg2}

void
link_error_num(string msg1, link link, string msg2, file file)
{
    ide_error(msg1 + app_type_print_string(link) + qt_link_name(link) + msg2,
	      link_appids(link, file));
}
				       
void
link_warning_num(string msg1, link link, string msg2, file file)
{
    ide_warning(msg1 + app_type_print_string(link) + qt_link_name(link) + msg2,
	   	link_appids(link, file));
}
							    
// Issue a symbol error of the form ${msg1} ${cntx.type} '${cntx.name}' ${msg2}
					     
void
cntx_error_num(string msg1, cntx cntx, string msg2, file file)
{
    ide_error(msg1 + app_type_print_string(cntx) + qt_cntx_name(cntx) + msg2,
	       cntx_appids(cntx, file));
}   


void
cntx_warning_num(string msg1, cntx cntx, string msg2, file file)
{
    ide_warning(msg1 + app_type_print_string(cntx) + qt_cntx_name(cntx) + msg2,
	    	cntx_appids(cntx, file));
}


//
// Reports the error_msg for any node found by the query.
// Allows an optional string to prepend to the message so
// that an error number may be reported.
//
int node_exceptions_find_num(string error_msg1, string query, string error_msg2, file fileobj)
{
    node current;
    int errors = 0;
    node parent;
	 
    for_each_in_select(query, current)
    {
	if (current.scope_node_id != 0)
	{
	     query = "node[node_refs && id == ${current.scope_node_id}]";
	     parent = find_by_query(query);
	     if (parent != NULL)
	     	error_msg2 = "scoped to " + parent.type + " '" + parent.name + "'" + error_msg2;
     	}
     	node_error_num(error_msg1, current, error_msg2, fileobj);
    	errors = errors + 1;
     }
     return errors;
}
