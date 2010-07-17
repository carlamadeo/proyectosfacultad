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
//	File: viewpoint.inc
//
//	Description:
//
//	Validate object viewpoints:
//		- for object_types, there is only one reference with vp.type
//		- the viewpoint reference is the to(from)_node_ref all 
//		  in(out)_link_types of in(out)_link_types connected to it
//
//	Arguments Notes
//		each is a parallel list of strings (except file obj)
//		nd_types should have one node type per string only others
//			can have oms query expressions.
//		either in_link_types or out_link_types can be lists of null lists
//
//////////////////////////////////////////////////////////////////////////////

int viewpoint_checks(file file_obj, // file to report error messages on
	list nd_types, // types of nodes (one per list atom) to check viewpoints
	list vp_types, // types of viewpoints your interested in
	list in_link_types, // types of in links required for viewpoint 
	list out_link_types) // types of out links required for viewpoint
{
    string one_vp;
    int index, errors = 0;
    node vp_node;
    string query;

    query = "node[(" + list_to_string(nd_types,"||") + ") && " +
	    "node_refs[file_id == ${file_obj.id}] && " + 
	    "viewpoints[" + list_to_string(vp_types,"||") + "]]";

    for_each_in_select(query,vp_node)
    {
	index = list_find(nd_types,0,vp_node.type);
	if (index == list_count(nd_types))
	{
	    // case of finding a node type with a viewpoint type
	    // that applies to other types of nodes
	    message("viewpoint_checks() internal error. Found "+vp_node.type+
		" with a viewpoint not being checked for.");
	    continue;
	}
	errors = errors + validate_viewpoint(file_obj, 
		list_get(vp_types,index), vp_node, 
		list_get(in_link_types, index), list_get(out_link_types, index));
    }
    return errors;
}

int validate_viewpoint(file file_obj, string vp_type, node obj,
	string in_link_type, // of app type strings
	string out_link_type ) // of app type strings
{
    int errors = 0;
    string vp_query = "viewpoint[ ${vp_type} && node_id == ${obj.id} ]";
    int vp_count = selection_count(vp_query);

    if (vp_count == 0) return errors;

    if (vp_count > 1)
    {
	node_error(obj," has more than one "+ vp_type + " defined.", file_obj);
	errors = errors + 1;
    }

    // don't check links when more than one viewpoint
    if (errors) return errors;
    
    if (in_link_type != NULL && in_link_type != "")
	errors = errors + 
	    validate_viewpoint_links(file_obj, obj, vp_type, "to", in_link_type);

    if (out_link_type != NULL && out_link_type != "")
	errors = errors + 
	    validate_viewpoint_links(file_obj, obj, vp_type, "from", out_link_type);

    return errors;
}

int validate_viewpoint_links(file file_obj, node obj, string vp_type, 
	string ndir, string ltype)
{
    int errors = 0;
    int link_count = 0, link_ref_count = 0;
    node_ref nd_vp_ref;
    string link_query = "link[${ltype} && link_refs && " +
	    "${ndir}_node_id == ${obj.id} ]";
    string links = "node_links";
    string id_link_query = "link[${links}]";
    string node_ref_query = "node_ref[node_id = ${obj.id} && " +
	"viewpoints[${vp_type} && node_id = ${obj.id} ] ]";
    string link_ref_query = "link_ref[ link[${links}] && " +
	"${ndir}_node_ref_id = ${nd_vp_ref.id} ]";

    id_list_create(link_query,links);
    link_count = selection_count(id_link_query);
    nd_vp_ref = find_by_query(node_ref_query);
    
    if (nd_vp_ref != NULL) 
    {
       link_ref_count = selection_count(link_ref_query);
    } else {
	link_ref_count = 0;
    }

    if (link_count != link_ref_count)
    {
	node_error(obj," has missing "+ltype+" on viewpoint "+vp_type+".",
		file_obj);
	errors = errors + 1;
    }

    id_list_free(links);
    return errors;
}
