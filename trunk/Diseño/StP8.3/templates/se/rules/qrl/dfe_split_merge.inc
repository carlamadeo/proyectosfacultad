
//
//      sccsid[] = %W% %Y% %D%
//      StP/SE
//      Confidential property of Interactive Development Environments, Inc.
//      Copyright (c) 1992-1996
//      All rights reserved
//
// dfe_split_merge -- implementation of split and merge for DFE.
//

// Constants
const string	SEL_SE_TARGET_PROMPT = "Please select a target for the Flow";
const string 	SYMBOL_TYPE = "SplitFlow";
const list	ALLOWED_LINK_SYMBOL_TYPES = string_to_list("DataFlow,ControlFlow", ",");
const list  	ALLOWED_END_SYMBOL_TYPES = string_to_list("SplitFlow,Process,Store,External,OffPageProcess,OffPageExternal,ControlSpec,Anchor", ",");



int
idehook_split_active_func()
{
    return split_active_func(ALLOWED_LINK_SYMBOL_TYPES, ALLOWED_END_SYMBOL_TYPES);
}

//
//  Split a link, attaching to selected endpoints.
//

void
CreateSplit()

{
    split_link(to_gde_arc(list_get(gde_selected_symbols(), 0)), SEL_SE_TARGET_PROMPT, SYMBOL_TYPE);

    // transaction will be ended in split_link
    return;
}

int
idehook_merge_active_func()
{
    return merge_active_func(ALLOWED_LINK_SYMBOL_TYPES, SYMBOL_TYPE,
    	    	    	     ALLOWED_END_SYMBOL_TYPES);
}

//
//  Merge a link, attaching from selected endpoints.
//
void
CreateMerge()
{
    list symbols, sel_link_syms, sel_node_syms;
    int count;

    symbols = gde_selected_symbols();
    sel_link_syms = gde_arcs_find_by_type(symbols, link_type);

    if ((count = list_count(sel_link_syms)) < 1)
    {
    	print_error("Please select unsplit " + link_type + " link(s) to merge");
	return;
    }

    if (count == 1)
    {
    	sel_node_syms = gde_symbols_find_with_types(symbols, 
    	    	    	    	    	    	    ALLOWED_END_SYMBOL_TYPES);
    	merge_one_link(list_get(sel_link_syms, 0), sel_node_syms, SYMBOL_TYPE);
    }
    else
    	merge_many_links(sel_link_syms, link_direction, SYMBOL_TYPE);

    // transaction will be ended in merge_*_link

    return;
}


//
// Link direction and type.  Set by active_func, used and reset by
// split/merge function.
//
dir_enum link_direction = NULL;
string link_type = "";

int
split_active_func(list allowed_link_types, list allowed_end_types)
{
    list symbols;
    string gnt;
    gde_arc ga;
    gde_node gn;

    if (list_count(gde_selected_symbols()) != 1)
	return ACTIVE_FUNC_IS_INACTIVE;

    symbols = gde_symbols_find_with_types(gde_selected_symbols(), allowed_link_types);
    if (list_count(symbols) != 1)
	return ACTIVE_FUNC_IS_INACTIVE;

    // check that the to and from nodes are legitimate types
    ga = to_gde_arc(list_get(symbols, 0));

    gn = gde_arc_get_real_fromnode(ga, True, NULL, NULL);
    gnt = gde_nt_name(gde_node_nt(gn));
    if (list_find(allowed_end_types, 0, gnt) == list_count(allowed_end_types))
	return ACTIVE_FUNC_IS_INACTIVE;

    gn = gde_arc_get_real_tonode(ga, True, NULL, NULL);
    gnt = gde_nt_name(gde_node_nt(gn));
    if (list_find(allowed_end_types, 0, gnt) == list_count(allowed_end_types))
	return ACTIVE_FUNC_IS_INACTIVE;
    
    return ACTIVE_FUNC_IS_ACTIVE;
}




// collect all the labels and qualifiers

struct lq_tp
{ 
    string l;
    string q;
};

// we can make a number of simplifying assumptions here because the string of arcs is 
// guaranteed to be a line with one begining and one end, sep. with vertices, NOT a network.

// 7/14/99 MHS Since there are no more qualifications, this code 
// can probably be a bit simpler

list
gde_arc_all_labels(gde_arc ga)

{
    gde_node gn;
    gde_arc next_ga;
    string label;
    lq_tp lq;
    list label_l, lq_l = list_create("lq_tp", 0);
    int i, icnt;
    
    // this arc
    label = gde_arc_label(ga);
    if (str_has_val(label))
    {
	label_l = string_to_list(label, ",");
	for (i = 0, icnt = list_count(label_l); i < icnt; i++)
	{
	    lq.l = string_strip(list_get(label_l, i), "B", " \n\t");
	    if (str_has_val(lq.l))
	    {
		//lq.q = gde_arc_prop(ga, "Qualifier-" + lq.l);
		list_append(lq_l, lq);
	    }
	}
    }

    // going backward
    gn = gde_arc_fromnode(ga);
    while (gde_nt_name(gde_node_nt(gn)) == "Vertex")
    {
	next_ga = list_get(gde_node_inarcs_no_comments(gn), 0);
	label = gde_arc_label(next_ga);
	if (str_has_val(label))
	{
	    label_l = string_to_list(label, ",");
	    for (i = 0, icnt = list_count(label_l); i < icnt; i++)
	    {
		lq.l = string_strip(list_get(label_l, i), "B", " \n\t");
		if (str_has_val(lq.l))
		{
		    //lq.q = gde_arc_prop(next_ga, "Qualifier-" + lq.l);
		    list_append(lq_l, lq);
		}
	    }
	}
	gn = gde_arc_fromnode(next_ga);
    }

    // going forward
    gn = gde_arc_tonode(ga);
    while (gde_nt_name(gde_node_nt(gn)) == "Vertex")
    {
	next_ga = list_get(gde_node_outarcs_no_comments(gn), 0);
	label = gde_arc_label(next_ga);
	if (str_has_val(label))
	{
	    label_l = string_to_list(label, ",");
	    for (i = 0, icnt = list_count(label_l); i < icnt; i++)
	    {
		lq.l = string_strip(list_get(label_l, i), "B", " \n\t");
		if (str_has_val(lq.l))
		{
		    //lq.q = gde_arc_prop(next_ga, "Qualifier-" + lq.l);
		    list_append(lq_l, lq);
		}
	    }
	}
	gn = gde_arc_tonode(next_ga);
    }

    return lq_l;
}

void
split_link(gde_arc ga, string select_prompt, string split_type)

{
    int		count, i, icnt, x, y, txn;
    string	label;
    gde_arc	split_arc;
    gde_node	from_node, split_node, cur_to_node, to_node;
    gde_symbol	cur_sym, split_sym;
    boolean 	from_node_selected = False;
    list lq_l, sel_node_syms = list_create("gde_symbol", 0);
    lq_tp lq;

    count = list_count(sel_node_syms);

    // Determine the from node of the selected link
    //
    // label = gde_arc_get_first_label(ga);
    // label = gde_arc_label(ga);

    lq_l = gde_arc_all_labels(ga);

    from_node = gde_arc_get_real_fromnode(ga, True, NULL, NULL);
    to_node = gde_arc_get_real_tonode(ga, True, NULL, NULL);

    x = gde_node_x(to_node);
    y = gde_node_y(to_node);

    x = (gde_arc_fromx(ga) + x) / 2;
    y = (gde_arc_fromy(ga) + y) / 2;

    gde_save_state();
    txn = gde_start_edit_transaction();

    // Create the split node and arc and set any label on the arc.
    //
    split_node = gde_node_create(split_type, x, y);
    split_arc = gde_arc_create(link_type, from_node, split_node);
    if (split_arc == NULL || split_node == NULL)
    {
    	print_error("Could not create split node and arc");
	gde_end_edit_transaction(GDE_CMD_INSERT, txn, False);

	return;
    }

    // use the lq_l to make a label
    if (list_count(lq_l) != 0)
    {
	label = "";
	for (i = 0, icnt = list_count(lq_l); i < icnt; i++)
	{
	    lq = list_get(lq_l, i);
	    if (i != 0)
		label += ",\n";
	    label += lq.l;

	    //if (str_has_val(lq.q))
	    //	gde_arc_set_prop(split_arc, "Qualifier-" + lq.l, lq.q);
	}
	    
	gde_arc_set_label(split_arc, label);
    }

    // If no node symbols have been selected, add link to any inferred to_node
    // and rubber band an arc from the from_node.
    //
    if (count == 0)
    {
	if (to_node != NULL)
	    split_arc = gde_arc_create(link_type, split_node,
    	    	    	    	       to_node);

	// this guy will eventually end the transaction that has been started.
	start_an_arc_from(to_gde_symbol(split_node), link_type, select_prompt,
    	    	    	  False);
    } 

    // Delete the original arc
    gde_arc_delete(ga);
}



int
merge_active_func(list allowed_link_types, string split_type,
    	    	  list allowed_end_types)
{
    list	symbols, arcs, new_arcs;
    int		arc_count, node_count, i;
    gde_arc 	link_arc;
    gde_symbol	to_sym, from_sym;
    set     	to_node_syms, from_node_syms;
    string  	cur_type;

    symbols = gde_selected_symbols();
    arcs = list_create("gde_arc", 0);

    for (i = 0, arc_count = list_count(allowed_link_types);
    	 i < arc_count; i++)
    {
    	cur_type = list_get(allowed_link_types, i);    	
    	new_arcs = gde_arcs_find_by_type(symbols, cur_type);

    	// All links selected must be the same type
    	if (list_count(new_arcs) != 0)
    	{
    	    if (list_count(arcs) != 0)
    	    	return ACTIVE_FUNC_IS_INACTIVE;
    	    else
    	    	link_type = cur_type;
    	}
    	list_concatenate(arcs, new_arcs);
    }

    // Select at least one link.
    // 
    if ((arc_count = list_count(arcs)) == 0)
	return ACTIVE_FUNC_IS_INACTIVE;

    to_node_syms = set_create("gde_symbol");
    from_node_syms = set_create("gde_symbol");

    // Make sure links aren't already in a split.
    //
    for (i = 0; i < arc_count; i++)
    {
    	link_arc = list_get(arcs, i);

        to_sym = to_gde_symbol(gde_arc_get_real_tonode(link_arc, False, NULL, NULL));
        from_sym = to_gde_symbol(gde_arc_get_real_fromnode(link_arc, False, NULL, NULL));
    	set_add(to_node_syms, to_sym);
    	set_add(from_node_syms, from_sym);
    }

    //  Make sure multiple links share either a to_ or from_node symbol but not
    // 	both.
       
    if (arc_count > 1)
    {
        if (set_count(to_node_syms) == 1 && set_count(from_node_syms) != 1)
    	    link_direction = TO;
        else if (set_count(from_node_syms) == 1 
    	    	 && set_count(to_node_syms) != 1)
    	    link_direction = FROM;
        else
    	{
    	    link_direction = NULL;
    	    return ACTIVE_FUNC_IS_INACTIVE;
        }
    }
	
    // If other symbols are selected, there should be only one link.  If
    // no other symbols are selected, there should be many links.
    //
    node_count = list_count(gde_symbols_find_with_types(symbols, 
    	    	    	    	    	    	    	allowed_end_types));

    if (node_count == 0 && arc_count == 1)
    	return ACTIVE_FUNC_IS_INACTIVE;
    if (node_count != 0)    
    {
        if (arc_count != 1)
	    return ACTIVE_FUNC_IS_INACTIVE;

    	// Make sure no extraneous symbols are selected
        if (node_count + 1 != list_count(symbols))
	    return ACTIVE_FUNC_IS_INACTIVE;
    }
    return ACTIVE_FUNC_IS_ACTIVE;
}


// EH Added for rts 10830
// Removes duplicate arcs selected on the same link so 
// merge won't draw two arcs over top of each other.

list
merge_flow_strip_dup_link_arc_segs(list selected_arcs_list)
{

    int x;
    gde_arc current_arc;
    gde_link current_link;
    list gde_links_list = list_create("gde_link", 0);
    list return_list = list_create("gde_arc", 0);

    for (x=0; x<list_count(selected_arcs_list); x++)
    {
        current_arc = list_get(selected_arcs_list, x);
        current_link = list_get(gde_arc_links(current_arc), 0);

        if (list_find(gde_links_list, 0, current_link) == 
            list_count(gde_links_list))
        {
            list_append(return_list, current_arc);
            list_append(gde_links_list, current_link);
        }
    }
    return return_list;
}


void
merge_many_links(list sel_link_syms, dir_enum direction, string merge_type)

{
    list    	targets, node_arcs, cur_arc_targets, end_nodes;
    int		count, i, j, k, x, y, txn;
    string	label;
    gde_arc	cur_arc, merge_arc;
    gde_node	end_node, merge_node, dummy, cur_target;

    if (direction == NULL)
    {
    	print_error("Internal error: no link direction");
    	return;
    }

    // EH Added in relation to rts 10830 above
    sel_link_syms = merge_flow_strip_dup_link_arc_segs(sel_link_syms);

    // For each link find all the target nodes
    //
    targets = list_create("gde_node", 0);
    for (i = 0; i < list_count(sel_link_syms); i++)
    {
	cur_arc = list_get(sel_link_syms, i);

    	if (direction == TO)
    	    cur_target = gde_arc_get_real_tonode(cur_arc, False, targets, NULL);
    	else
    	    cur_target = gde_arc_get_real_fromnode(cur_arc, False, targets, NULL);
    }

    gde_save_state();
    txn = gde_start_edit_transaction();

    // find the links that end on each target 
    //
    for (i = 0; i < list_count(targets); i++)
    {
	cur_target = list_get(targets, i);
	node_arcs = list_create("gde_arc", 0);
	x = 0;
	y = 0;
	for (j = 0; j < list_count(sel_link_syms); j++)
	{
	    // find arcs which go from this target and add them to the
    	    // node_arcs
    	    //
	    cur_arc = list_get(sel_link_syms, j);
	    cur_arc_targets = list_create("gde_node", 0);

    	    if (direction == TO)    
    	    	dummy 
    	    	  = gde_arc_get_real_tonode(cur_arc, False, cur_arc_targets, NULL);
    	    else
    	    	dummy
    	    	  = gde_arc_get_real_fromnode(cur_arc, False, cur_arc_targets, NULL);

	    if (list_find(cur_arc_targets, 0, cur_target) 
    	    	!= list_count(cur_arc_targets))
	    {
		list_append(node_arcs, cur_arc);
		x = x + (gde_arc_fromx(cur_arc) + gde_arc_tox(cur_arc)) / 2;
		y = y + (gde_arc_fromy(cur_arc) + gde_arc_toy(cur_arc)) / 2;
	    }
	}

	// Now node_arcs contains all arcs ending on the target
    	// If there are any arcs, try to create a merge symbol
    	//
	count = list_count(node_arcs);
	if (count == 0)
	    continue;

	x = x / count;
	y = y / count;
	merge_node = gde_node_create(merge_type, x, y);
	if (merge_node == NULL)
	{
	    gde_end_edit_transaction(GDE_CMD_INSERT, txn, False);
	    return;
	}

    	// Connect the merge node and the current target and any other
    	// arcs' end_nodes and the merge node
    	//
    	if (direction == TO)
    	    merge_arc = gde_arc_create(link_type, merge_node, 
    	    	    	    	    	cur_target);
    	else
    	    merge_arc = gde_arc_create(link_type, cur_target,
    	    	    	    	    	merge_node);

	for (j = 0; j < list_count(node_arcs); j++)
	{
	    cur_arc = list_get(node_arcs, j);
	    //label = gde_arc_get_first_label(cur_arc);
	    //label = gde_arc_label(cur_arc);
            // EH Added in relation to rts 10830 above
	    label = gde_link_label(list_get(gde_arc_links(cur_arc), 0));

	    end_nodes = list_create("gde_node", 0);

    	    if (direction == TO)
    	    	end_node = gde_arc_get_real_fromnode(cur_arc, False, end_nodes, NULL);
    	    else
    	    	end_node = gde_arc_get_real_tonode(cur_arc, False, end_nodes, NULL);

    	    // Connect the source nodes of the arc to the merge node
    	    //
	    for (k = 0; k < list_count(end_nodes); k++)
	    {
		end_node = list_get(end_nodes, k);

    	    	if (direction == TO)
    	    	    merge_arc = gde_arc_create(link_type, 
    	    	    	    	    	    	end_node, merge_node);
    	    	else
    	    	    merge_arc = gde_arc_create(link_type, 
    	    	    	    	    	    	merge_node, end_node);

		if (merge_arc == NULL)
		    continue;
		if (label != NULL)
		{
		    gde_arc_set_label(merge_arc, label);
		    //move_qual_props(cur_arc, merge_arc, label);
		}
	    }
	}
    }

    // Delete the old links
    for (i = 0; i < list_count(sel_link_syms); i++)
    {
	cur_arc = list_get(sel_link_syms, i);
	gde_arc_delete(cur_arc);
    }

    gde_end_edit_transaction(GDE_CMD_INSERT, txn, False);

    // Clear for next time    	
    link_direction = NULL;
}




void
merge_one_link(gde_arc link_arc, list sel_node_syms, string merge_type)
{

    int		i, x, y, txn;
    string	label;
    gde_arc	merge_arc;
    gde_node	to_node, merge_node, cur_from_node, from_node;
    gde_symbol	cur_sym;
    boolean 	to_node_selected = False;

    // Determine the target node of the selected link
    //
    //label = gde_arc_get_first_label(link_arc);
    label = gde_arc_label(link_arc);
    to_node = gde_arc_get_real_tonode(link_arc, True, NULL, NULL);
    if (to_node == NULL)
    {
    	print_error("Cannot identify link's target node; please select it");
	return;
    }

    // Find any source node of the selected link, add it to the select list
    // if necessary
    //
    from_node = gde_arc_get_real_fromnode(link_arc, False, NULL, NULL);
    if (from_node != NULL)
    {
	i = list_find(sel_node_syms, 0, to_gde_symbol(from_node));
	if (i == list_count(sel_node_syms))
	    list_append(sel_node_syms, to_gde_symbol(from_node));
    }

    // If the target node is in the select list, note that and remove it
    //
    i = list_find(sel_node_syms, 0, to_gde_symbol(to_node));
    if (i < list_count(sel_node_syms))
    {
    	to_node_selected = True;
	list_delete(sel_node_syms, i);
    }

    // Compute average of each node's x, y coords
    x = 0;
    y = 0;

    for (i = 0; i < list_count(sel_node_syms); i++)
    {
	cur_from_node = to_gde_node(list_get(sel_node_syms, i));
	x = gde_node_x(cur_from_node) + x;
	y = gde_node_y(cur_from_node) + y;
    }

    if (i == 0)
    {
	x = gde_arc_tox(link_arc);
	y = gde_arc_toy(link_arc);
    }
    else 
    {
        x = (gde_arc_tox(link_arc) + (x/i)) / 2;
    	y = (gde_arc_toy(link_arc) + (y/i)) / 2;
    }

    gde_save_state();
    txn = gde_start_edit_transaction();

    // Create the merge node and arc and set any label on the arc.
    //
    merge_node = gde_node_create(merge_type, x, y);
    merge_arc = gde_arc_create(link_type, merge_node, to_node);
    if (merge_arc == NULL || merge_node == NULL)
    {
    	print_error("Could not create merge node and arc");
	gde_end_edit_transaction(GDE_CMD_INSERT, txn, False);
	return;
    }
    if (label != NULL)
    {
	gde_arc_set_label(merge_arc, label);
	//move_qual_props(link_arc, merge_arc, label);
    }

    // Delete the original arc
    gde_arc_delete(link_arc);

    if (list_count(sel_node_syms) == 0)
    {
    	print_error("Internal error; no selected node symbols");
	gde_end_edit_transaction(GDE_CMD_INSERT, txn, False);
    	return;
    }	

    // Connect to_node to merge symbol; use a vertex so link is visible
    if (to_node_selected)
    	end_node_connect(merge_arc, link_type, to_node, merge_node);

    // Connect merge node to each selected node.
    for (i = 0; i < list_count(sel_node_syms); i++)
    {
    	cur_sym = list_get(sel_node_syms, i);
    	merge_arc = gde_arc_create(link_type, to_gde_node(cur_sym), merge_node);
    }

    gde_save_state();
    txn = gde_start_edit_transaction();

    return;
}

//
// Connect end_node to mid_node with link_sym_type link; use a vertex so
// link is always visible
//
void
end_node_connect(gde_arc end_arc, string link_sym_type, gde_node end_node, 
    	    	 gde_node mid_node)
{ 
    int x, y;
    gde_node vertex;

    x = gde_arc_tox(end_arc);
    if (x == gde_arc_fromx(end_arc))
    {
    	x = x - 30;
    	y = (gde_arc_toy(end_arc) + gde_arc_toy(end_arc)) / 2;
    } 
    else 
    {
    	x = (x + gde_arc_fromx(end_arc)) / 2;
    	y = gde_arc_fromx(end_arc);
    }
    vertex = gde_node_create(VERTEX_SYMBOL_TYPE, x, y);
    end_arc = gde_arc_create(link_sym_type, end_node, vertex);
    end_arc = gde_arc_create(link_sym_type, vertex, mid_node);
}


void
OLDmove_qual_props(gde_arc oldga, gde_arc newga, string label)

{

    list labell = string_to_list(label, ",");
    string prop;
    int i, icnt;

    for (i = 0, icnt = list_count(labell); i < icnt; i++)
    {
	label = string_strip(list_get(labell, i), "B", " \n\t");
	prop = gde_arc_prop(oldga, "Qualifier-" + label);
	if (str_has_val(prop))
	    gde_arc_set_prop(newga, "Qualifier-" + label, prop);
    }
    return;
}

