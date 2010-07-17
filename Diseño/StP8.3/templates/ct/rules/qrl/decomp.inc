
//
//      sccsid[] = %W% %Y% %D%
//      StP/SE
//      Confidential property of Interactive Development Environments, Inc.
//      Copyright (c) 1995
//      All rights reserved
//

#include "rules/qrl/gde_qrl_std.inc"

//  Create a diagram which shows the selected symbol and its attached
//  links.  Connected nodes are replaced with editor-specific symbols.  
//  All other symbols are deleted.
//
void general_decomp(string decomp_symbol)
{
    gde_symbol symbol;
    gde_node selnode, end_node, anode;
    gde_arc arc;
    list symbols, arcs, all_symbols, arc_nodes, other_nodes, del_nodes, 
    	 del_arcs, replaced_nodes;
    set end_nodes, loop_arcs;
    string symtype;
    boolean symisok = False;
    int i, count, j, jcount;

    // Check for a selected symbol of the proper decomp type
    //
    symbols = gde_selected_symbols();

    if (list_count(symbols) == 1)
    {
	symbol = list_get(symbols, 0);

	if (gde_symbol_is_node(symbol))
	{
	    selnode = to_gde_node(symbol);
	    symtype = gde_nt_name(gde_node_nt(selnode));
    	    if (symtype == decomp_symbol)
    	    	symisok = True;
    	}
    }

    if (!symisok)    
    {
    	ide_error("Please select one " + decomp_symbol + " node");
    	return;
    }

    end_nodes = set_create("gde_node");
    arc_nodes = list_create("gde_node", 0);
    loop_arcs = set_create("gde_arc");

    // Get all attached nodes
    arcs = gde_node_inarcs(selnode);    
    for (i = 0, count = list_count(arcs); i < count; i++)
    {    
    	arc = list_get(arcs, i);
        gde_arc_get_real_fromnode(arc, False, arc_nodes, NULL);
    	label_arc(arc, arc_nodes);
    	for (j = 0; j < list_count(arc_nodes); j++)
	{
	    end_node = list_get(arc_nodes, j); 
	    if (end_node != selnode)
    	        set_add(end_nodes, end_node);
	    else
    	        set_add(loop_arcs, arc);
	}
    	list_clear(arc_nodes);
    }

    arcs = gde_node_outarcs(selnode);    
    for (i = 0, count = list_count(arcs); i < count; i++)
    {    
    	arc = list_get(arcs, i);
        gde_arc_get_real_tonode(arc, False, arc_nodes, NULL);
    	label_arc(arc, arc_nodes);
    	for (j = 0; j < list_count(arc_nodes); j++)
	{
	    end_node = list_get(arc_nodes, j); 
	    if (end_node != selnode)
    	        set_add(end_nodes, end_node);
	    else
    	        set_add(loop_arcs, arc);
	}
    	list_clear(arc_nodes);
    }

    // 	Find all nodes which are not end_nodes, the selected node, or
    //  vertices.
    all_symbols = gde_all_symbols();
    del_nodes = list_create("gde_node", 0);
    for (i = 0, count = list_count(all_symbols); i < count; i++)
    {
    	symbol = list_get(all_symbols, i);
    	if (gde_symbol_is_node(symbol))
    	{
    	    anode = to_gde_node(symbol);
    	    if (anode != selnode 
    	    	&& gde_nt_name(gde_node_nt(anode)) != VERTEX_SYMBOL_TYPE
    	    	&& !set_is_member(end_nodes, anode))
	    {
    	    	list_append(del_nodes, anode);    	    	
	    }
    	}
    }

    //	Replace the end_nodes with decomp symbols and make a new
    //  list of end nodes
    replaced_nodes = list_create("gde_node", 0);
    for (i = 0, count = set_count(end_nodes); i < count; i++)
    {
    	end_node = set_get_element(end_nodes, i);
    	off_node_replace(end_node);
    	list_append(replaced_nodes, end_node);
    }

    // Delete the nodes found above.  Do this deletion in two steps
    // since deleting nodes can also delete other symbols and change
    // the all_symbols list.
    //
    for (i = 0, count = list_count(del_nodes); i < count; i++)
    	gde_node_delete(list_get(del_nodes, i));

    //  Find all remaining links which are not from/to the selected node.
    //
    other_nodes = list_create("gde_node", 0);
    del_arcs = list_create("gde_arc", 0);
    for (i = 0, count = list_count(replaced_nodes); i < count; i++)
    {
    	anode = list_get(replaced_nodes, i);

    	arcs = gde_node_inarcs(anode);
        for (j = 0, jcount = list_count(arcs); j < jcount; j++)
    	{    
    	    arc = list_get(arcs, j);
    	    gde_arc_get_real_fromnode(arc, False, other_nodes, NULL);
    	    if (list_count(other_nodes) != 1 
    	    	|| list_get(other_nodes, 0) != selnode)
    	    {
    	    	list_append(del_arcs, arc);
    	    }
    	    list_clear(other_nodes);
    	}
    	arcs = gde_node_outarcs(anode);
        for (j = 0, jcount = list_count(arcs); j < jcount; j++)
    	{    
    	    arc = list_get(arcs, j);
    	    gde_arc_get_real_tonode(arc, False, other_nodes, NULL);
    	    if (list_count(other_nodes) != 1 
    	    	|| list_get(other_nodes, 0) != selnode)
    	    {
    	    	list_append(del_arcs, arc);
    	    }
    	    list_clear(other_nodes);
    	}
    }    

    // Delete the arcs found above.
    for (i = 0, count = list_count(del_arcs); i < count; i++)
    	gde_arc_delete(list_get(del_arcs, i));

    // delete all the loop arcs
    for (i = 0; i < set_count(loop_arcs); i++)
    	gde_arc_delete(set_get_element(loop_arcs, i));

    // clear the selected node
    clear_node(selnode);
}

