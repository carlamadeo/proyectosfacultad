//
// 	sccsid[] = %W% %Y% %D%
// 	StP/IM
// 	Confidential property of Interactive Development Environments, Inc.
// 	Copyright (c) 1992-1993
// 	All rights reserved
//


//////////////////////////////////////////////////////////////////////////////
//
//
//    File: appids.inc
//
//    Description:
//
//	Error routines and supporting functions for getting
//	symbol appids (for use in Symbol Errors).
//
//////////////////////////////////////////////////////////////////////////////


// Return a list of application id's for the references to node 'nodeobj'
// in the file 'fileobj'.

list 
node_appids(node nodeobj, file fileobj)
{
    list     nodeids, ids = list_create("int", 0);
    string   query;
    node_ref noderef;

    if (fileobj != NULL && nodeobj != NULL)
    {
	query = "node_ref[node_id==${nodeobj.id} && file_id==${fileobj.id}]";

	for_each_in_select(query, noderef)
	{
	    // if the noderef is a gde link symbol then the appids 
	    // are in pairs - first & last arc segment

    	    nodeids = string_to_list(noderef.appid, ", ");

	    list_append(ids, to_int(list_get(nodeids, 0)));

	    if (list_count(nodeids) > 1)
	    {
		list_append(ids, to_int(list_get(nodeids, 1)));
	    }
	}
    }
    return(ids);
}

// Return a list of application id's for the references to link 'linkobj'
// in the file 'fileobj'.

list 
link_appids(link linkobj, file fileobj)
{
    list     linkids, ids = list_create("int", 0);
    string   query;
    link_ref linkref;

    if (fileobj != NULL && linkobj != NULL)
    {
	query = "link_ref[link_id==${linkobj.id} && file_id==${fileobj.id}]";

	for_each_in_select(query, linkref)
	{
    	    // gde link refs are in pairs. (first & last arc segment)
	    // gte just uses the cell id.

    	    linkids = string_to_list(linkref.appid, ", ");

	    list_append(ids, to_int(list_get(linkids, 0)));

	    if (list_count(linkids) > 1)
	    {
		list_append(ids, to_int(list_get(linkids, 1)));
	    }
	}
    }
    return(ids);
}

// Return a list of application id's for the references to cntx 'cntxobj'
// in the file 'fileobj'.

list 
cntx_appids(cntx cntxobj, file fileobj)
{
    list     cntxids, ids = list_create("int", 0);
    string   query;
    cntx_ref cntxref;

    if (fileobj != NULL && cntxobj != NULL)
    {
	query = "cntx_ref[cntx_id==${cntxobj.id} && file_id==${fileobj.id}]";

	for_each_in_select(query, cntxref)
	{
	    // if the cntxref is a gde link symbol then the appids 
	    // are in pairs - first & last arc segment

    	    cntxids = string_to_list(cntxref.appid, ", ");

	    list_append(ids, to_int(list_get(cntxids, 0)));

	    if (list_count(cntxids) > 1)
	    {
		list_append(ids, to_int(list_get(cntxids, 1)));
	    }
	}
    }
    return(ids);
}

// Issue a symbol error of the form ${node.type} '${node.name}' ${msg}

void
node_error(node node, string msg, file file)
{
    print_error(app_type_print_string(node) + qt_node_name(node) + msg, 
	      node_appids(node, file));
}

void
node_warning(node node, string msg, file file)
{
    print_warning(app_type_print_string(node) + qt_node_name(node) + msg
    /* , node_appids(node, file) */
    );
}

// Issue a symbol error of the form ${link.type} '${link.name}' ${msg}

void 
link_error(link link, string msg, file file)
{
    print_error(app_type_print_string(link) + qt_link_name(link) + msg, 
	      link_appids(link, file));
}

void
link_warning(link link, string msg, file file)
{
    print_warning(app_type_print_string(link) + qt_link_name(link) + msg
    /* , link_appids(link, file) */
    );
}

// Issue a symbol error of the form ${cntx.type} '${cntx.name}' ${msg}

void
cntx_error(cntx cntx, string msg, file file)
{
    print_error(app_type_print_string(cntx) + qt_cntx_name(cntx) + msg, 
	      cntx_appids(cntx, file));
}

void
cntx_warning(cntx cntx, string msg, file file)
{
    print_warning(app_type_print_string(cntx) + qt_cntx_name(cntx) + msg
    /* , cntx_appids(cntx, file) */
    );
}

// Same as node_error only pass in your own set of appids.

void
node_error_syms(node node, string msg, list syms)
{
    print_error(app_type_print_string(node) + qt_node_name(node) + msg, syms);
}

void
node_warning_syms(node node, string msg, list syms)
{
    print_warning(app_type_print_string(node) + qt_node_name(node) + msg
    /* , syms */
    );
}

// Same as link_error only pass in your own set of appids.

void
link_error_syms(link link, string msg, list syms)
{
    print_error(app_type_print_string(link) + qt_link_name(link) + msg, syms);
}

void
link_warning_syms(link link, string msg, list syms)
{
    print_warning(app_type_print_string(link) + qt_link_name(link) + msg
    /* , syms */
    );
}

// Same as cntx_error only pass in your own set of appids.

void
cntx_error_syms(cntx cntx, string msg, list syms)
{
    print_error(app_type_print_string(cntx) + qt_cntx_name(cntx) + msg, syms);
}

void
cntx_warning_syms(cntx cntx, string msg, list syms)
{
    print_warning(app_type_print_string(cntx) + qt_cntx_name(cntx) + msg
    /* , syms */
    );
}

string
qt_node_name(node node)
{
    if (node.name == "")
	return(" ");
    return(" '" + node.name + "' ");
}

string 
qt_link_name(link link)
{
    if (link.name == "")
	return(" ");
    return(" '" + link.name + "' ");
}

string
qt_cntx_name(cntx cntx)
{
    if (cntx.name == "")
	return(" ");
    return(" '" + cntx.name + "' ");
}

