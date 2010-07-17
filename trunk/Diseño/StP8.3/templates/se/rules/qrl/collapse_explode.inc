//
//      sccsid[] = %W% %Y% %D%
//      StP/SE
//      Confidential property of Interactive Development Environments, Inc.
//      Copyright (c) 1995
//      All rights reserved
//

// collapse_explode.inc -- bits common to both collapse and explode.


#include "qrl/decomp/update_decomps.inc"
#include "qrl/include/dfe_defs.inc" 
#include "rules/qrl/gde_qrl_std.inc"
#include "rules/qrl/dfe_stputil.inc"

// structures for nodes, arcs in a decomposition
struct decomp_node
{
    gde_node	node;
    int	    	x;
    int	    	y;
    boolean 	replace;   	// replace the node with a decomp type?
    string	ref_appid;
    string 	type;
    string  	label;
    string 	old_pindex;
    string  	new_pindex;
};

struct decomp_arc
{
    decomp_node from_node;
    decomp_node to_node;
    dir_enum	dir;
    string 	type;
    string  	label;
};

//
//  Get any up-to-date DFE file with the name filename.  Return NULL
//  if there is none.
//
file
GetCurrentDfeFile(string filename)
{
    string query;
    file fileobj;

    query = "file[DfeDiagram && name == '${filename}']";
    fileobj = find_by_query(query);
    if (fileobj == NULL)
    	return NULL;
    return fileobj;
}

//
//  Store the characteristics of the link in a decomp_arc structure.
//
decomp_arc
GrabLink(link the_link)
{
    decomp_arc dec_link;
    dec_link.type = the_link.type;
    dec_link.label = the_link.name;
    dec_link.dir = NULL;

    return dec_link;
}

//
//  Store the characteristics of the node in a decomp_node structure.
//
void
GrabNode(decomp_node dec_node, node_ref the_node_ref, boolean replace,
    	 boolean get_sym)
{
    string  query;
    item    pindex;
    node    the_node;

    dec_node.replace = replace;
    dec_node.x = the_node_ref.xcoord;
    dec_node.y = the_node_ref.ycoord;
    dec_node.ref_appid = the_node_ref.appid;

    query = "node[node_refs[id == ${the_node_ref.id}]]";
    the_node = find_by_query(query);
    dec_node.type = NodeToSymbolType(the_node.type);

    if (dec_node.type == PROCESS)
    {
    	query = "item[RelativeIndex && obj_id == ${the_node.id}]";
    	pindex = find_by_query(query);
    	dec_node.old_pindex = pindex.value;    	
    }
    else
    	dec_node.old_pindex = NULL;

    /* ControlSpec and SplitFlow nodes can't be labelled */
    if (the_node.type == CSPEC || the_node.type == SPLIT_FLOW)
    	dec_node.label = "";
    else
        dec_node.label = the_node.name;
    dec_node.new_pindex = NULL;

    if (get_sym)
    	dec_node.node 
    	= gde_node_find_by_psymid(to_int(the_node_ref.appid));
    else
        dec_node.node = NULL;
}

//
//  Draw all the arcs in the list and their end nodes.
//  Draw all nodes (even those with non-null decomp_node.node) settings
//  if requested.
//
void 
DrawArcs (list arcs, boolean draw_all_nodes)
{
    int i, pos, displacement;
    decomp_node cur_node;
    gde_arc 	new_arc;
    gde_node 	from_node, to_node;
    decomp_arc	cur_arc;
    list    	nodes, existing_refs, endpoints, displacements;
    string  	endpoint;

    existing_refs = list_create("string", 0);
    nodes = list_create("gde_node", 0);
    endpoints = list_create("string", 0);
    displacements = list_create("int", 0);
    
    for (i = 0; i < list_count(arcs); i++)
    {    
    	cur_arc = list_get(arcs, i);

    	// If the node has already been drawn, just reuse it.
    	if ((pos = list_find(existing_refs, 0, cur_arc.from_node.ref_appid))
    	    == list_count(existing_refs))
    	{
    	    from_node = DrawNode(cur_arc.from_node, draw_all_nodes);
    	    list_append(existing_refs, cur_arc.from_node.ref_appid);
    	    list_append(nodes, from_node);
    	}
    	else
    	    from_node = list_get(nodes, pos);

    	// If the node has already been drawn, just reuse it.
    	if ((pos = list_find(existing_refs, 0, cur_arc.to_node.ref_appid))
    	    == list_count(existing_refs))
    	{
    	    to_node = DrawNode(cur_arc.to_node, draw_all_nodes);
    	    list_append(existing_refs, cur_arc.to_node.ref_appid);
    	    list_append(nodes, to_node);
    	}
    	else
    	    to_node = list_get(nodes, pos);

    	// There's already a link to this arc, so use a displacement
    	// so we get some vertices.
    	endpoint = cur_arc.from_node.ref_appid + "," 
    	    	   + cur_arc.to_node.ref_appid;
	if ((pos = list_find(endpoints, 0, endpoint)) != list_count(endpoints))
    	{
    	    // There are two endpoint entries for each displacement
    	    pos = pos/2;    
	    displacement = list_get(displacements, pos) + 20;
    	    list_set(displacements, pos, displacement);
    	}
	else
    	{
    	    list_append(endpoints, endpoint);
    	    list_append(endpoints, cur_arc.to_node.ref_appid + "," +
    	    	    	    	   cur_arc.from_node.ref_appid);
	    displacement = 0;
    	    list_append(displacements, displacement);
    	}

    	// Draw the arc and set its label.
    	new_arc = link_with_vertex(cur_arc.type, from_node, to_node, 
    	    	    	    	   displacement);
   	gde_arc_set_label(new_arc, cur_arc.label);
    	list_set(arcs, i, cur_arc);
    }
}

//
//  Draw node.  Convert each external node to its offpage equivalent.
//
gde_node
DrawNode (decomp_node cur_node, boolean draw_all_nodes)
{
    gde_node	new_node;

    if (cur_node.node != NULL && !draw_all_nodes)
    	return cur_node.node;
    new_node = gde_node_create(cur_node.type, cur_node.x, cur_node.y);

    if (cur_node.label != "")
	gde_node_set_label(new_node, cur_node.label);

    // Replace appropriate nodes
    if (cur_node.replace)
    	replace_node(new_node);

    // Get new pindex of any Process node
    if (gde_nt_name(gde_node_nt(new_node)) == PROCESS)
    	cur_node.new_pindex = gde_node_prop(new_node, "pindex");

    return new_node;
}

//
//  Connect from_node to from_node with link_sym_type link; use a vertex if
//  if necessary so the link is always visible.
//
gde_arc
link_with_vertex(string link_sym_type, gde_node from_node, 
    	    	 gde_node to_node, int displacement)
{ 
    gde_arc new_arc;
    new_arc = gde_arc_create(link_sym_type, from_node, to_node);

    // vertex needed
    if (displacement != 0)
    {
    	int x, y, xfrom, xto, yfrom, yto, xdiff, ydiff;

        xfrom = gde_node_x(from_node);
    	xto = gde_node_x(to_node);
        yfrom = gde_node_y(from_node);
        yto = gde_node_y(to_node);

    	x = (xfrom + xto)/2;
        y = (yfrom + yto)/2;

    	xdiff = xto - xfrom;
        if (xdiff < 0) 
    	    xdiff = 0 - xdiff;

        ydiff = yto - yfrom;
    	if (ydiff < 0) 
    	    ydiff = 0 - ydiff;

        if (xdiff < ydiff)
    	    x = x + displacement;
    	else
    	    y = y + displacement;
    	gde_arc_insert_vertex(new_arc, x, y);
    }
    return new_arc;
}

//
// Links object and symbol types
//
string
NodeToSymbolType(string node_type)
{
    if (node_type == FLOW_ANCHOR)
    	return "Anchor";
    else if (node_type == CSPEC)
    	return "ControlSpec";
    else 
    	return node_type;
}



// remove hanging symbols 
void
remove_orphans()
 
{
    int i, j;
    gde_node gn;
    list gns;
 
    // anchors
    gns = gde_nodes_find_by_type(gde_all_symbols(), "Anchor");
    list_concatenate(gns, gde_nodes_find_by_type(gde_all_symbols(), "OffPageProcess"));
    list_concatenate(gns, gde_nodes_find_by_type(gde_all_symbols(), "OffPageExternal"));
    for (i = 0; i < list_count(gns); i++)
    {
        gn = list_get(gns, i);
        if ((list_count(gde_node_inarcs_no_comments(gn)) == 0) && (list_count(gde_node_outarcs_no_comments(gn)) == 0))
            gde_node_delete(gn);
    }
    return;
}

