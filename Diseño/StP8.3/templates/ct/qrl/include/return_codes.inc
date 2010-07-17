//
// 	sccsid[] = %W% %Y% %D%
// 	StP/IM
// 	Confidential property of Interactive Development Environments, Inc.
// 	Copyright (c) 1992-1993
// 	All rights reserved
//

//
//	FILE:	return_codes.inc
//
//	This file contains globals for return codes used to
//	communicate between scripts and provides application-level
//	error handling.
//

int RC_OK = 0;
int RC_WARNING = 1;
int RC_ERROR = 2;

#include "qrl/include/appids.inc"

//
// Output a final error count message.
//
void error_count(string check_type, int errors)
{   
    string message = check_type + " completed with " + errors + " error";
    if (errors != 1)
    	message = message + "s";
    print_message(message);
}

//
// Reports the error_msg for any node found by the query.  
//
int node_exceptions_find(string query, string error_msg, file fileobj)
{
    node current;
    node parent;
    int errors = 0;

    for_each_in_select(query, current)
    {
	if (current.scope_node_id != 0)
	{
    	    query = "node[node_refs && id == ${current.scope_node_id}]";
    	    parent = find_by_query(query);
	    if (parent != NULL)
    	    	error_msg 
    	    	    = "scoped to " + parent.type + " '" + parent.name + "'" + 
    	    	    error_msg;
    	}
	node_error(current, error_msg, fileobj);
    	errors = errors + 1;
    }
    return errors;
}


//
// Reports the warning_msg for any node found by the query.  
//
void node_warnings_find(string query, string warning_msg, file fileobj)
{
    node current;
    node parent;

    for_each_in_select(query, current)
    {
	if (current.scope_node_id != 0)
	{
    	    query = "node[node_refs && id == ${current.scope_node_id}]";
    	    parent = find_by_query(query);
	    if (parent != NULL)
    	    	error_msg 
    	    	    = "scoped to " + parent.type + " '" + parent.name + "'" + 
    	    	    error_msg;
    	}
	node_warning(current, warning_msg, fileobj);
    }
}


//
// Reports the error_msg for any link found by the query.  
//
int link_exceptions_find(string query, string error_msg, file fileobj)
{
    link current;
    int errors = 0;

    for_each_in_select(query, current)
    {
	link_error(current, error_msg, fileobj);
    	errors = errors + 1;
    }
    return errors;
}
