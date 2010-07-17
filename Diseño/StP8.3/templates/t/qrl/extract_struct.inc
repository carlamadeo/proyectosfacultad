//
//      sccsid[] = %W% %Y% %D%
//      StP/T
//      Confidential property of Interactive Development Environments, Inc.
//      Copyright (c) 1994
//      All rights reserved
//
// extract_struct.inc
// extract and print structured (sequence/selection/iteration) information
// for SE.
/*
 * Public Functions:
 *	add_to_type_expansion
 *	build_up_kid_list
 *	create_selection_mapping
 *	expand_obj_type
 *	has_kids
 *	incr_sel_mapping
 *	no_selections_present
 *	selection_mapping_total
 *	start_type_expansion
 */


struct type_expansion_tp
{
    string seq_sel_iter;  // Sequence, Selection or Iteration
    string u_or_p;        // Use or Produce
    list adt_info_l;
    cntx the_cntx;
    node the_node;
    link the_link;
};


struct sel_mapping_tp
{
    int cur; 
    int max; 
    int index;
};



struct adt_info_tp
{
    string adt_type;   
    string adt_name;   
    string scope;
    int annot_id;
    boolean is_array;
};



list CurTypeExpansion_l;

void
start_type_expansion()

{
    CurTypeExpansion_l = list_create("type_expansion_tp", 0);

    return;

} // end of start_type_expansion()



list create_selection_mapping()

{
    int i;
    list sel_mapping_l;
    sel_mapping_tp sel_mapping;
    type_expansion_tp te;

    sel_mapping_l = list_create("sel_mapping_tp", 0);
    for (i = 0; i < list_count(CurTypeExpansion_l); i = i + 1)
    {
        te = list_get(CurTypeExpansion_l, i);

	if (te.seq_sel_iter == "Selection")
	{
	    sel_mapping.cur = 0;
	    sel_mapping.max = list_count(te.adt_info_l) - 1;
	    sel_mapping.index = i;
	    list_append(sel_mapping_l, sel_mapping);
	}	
    }
    return sel_mapping_l;

} // end of create_selection_mapping()




int selection_mapping_total(list sm_l)
 
{
    int j, total;
    sel_mapping_tp sm;
 
    total = 1;
 
    for (j = 0; j < list_count(sm_l); j = j + 1) 
    {
        sm = list_get(sm_l, j);
        total = total * (sm.max + 1);
    }
 
    return total;
 
} // end of selection_mapping_total()



list 
incr_sel_mapping(list sm_l)

{
    int i, j;
    sel_mapping_tp sm;

    i = list_count(sm_l) - 1;
    sm = list_get(sm_l, i);

    if (sm.cur == sm.max)
    {
	sm.cur = 0; 
	list_set(sm_l, i, sm);		

	// bubble the change down
	for (j = i - 1; j >= 0; j = j - 1)
	{
	    sm = list_get(sm_l, j);
	    if (sm.cur != sm.max)
	    {
		sm.cur = sm.cur + 1;
		list_set(sm_l, j, sm);		
		break;
	    }
	    else
	    {
		sm.cur = 0;
		list_set(sm_l, j, sm);		
	    }
	}
    }
    else
    {
	sm.cur = sm.cur + 1;
	list_set(sm_l, i, sm);		
    }

    return sm_l;

} // end of incr_sel_mapping() 





void
add_to_type_expansion(list adt_info_l, string ssi, string up, 
                      cntx the_cntx, node the_node, link the_link)

{
    type_expansion_tp te;

    te.seq_sel_iter = ssi;
    te.u_or_p = up;
    te.adt_info_l = list_copy(adt_info_l);
    te.the_cntx = the_cntx;
    te.the_node = the_node;
    te.the_link = the_link;

    list_append(CurTypeExpansion_l, te);

    return;

} // end of add_to_type_expansion()




int
has_kids(node n)
{
    string query;
    list l;

    query = "link[Component && from_node_id = ${n.id}]";
    l = list_select(query);
    if (l == NULL || list_count(l) == 0)
	return 0;

    return 1;
}


list VisitedAlready;
list ADTInfoList;
boolean ParentIsArray;


// create a list (ADTInfoList) that describes all the leaf level children.

boolean
build_up_kid_list(node kid, node parent, string instancename, string rootboxname)

{
    string query, tname = "";
    adt_info_tp the_adt_info;
    list baby_list;
    node baby;
    item lb, ub, the_item;
    int i, lb_val, ub_val, id;
    string compound_instname;
    node orig_kid = NULL;

    baby_list = NULL;
    compound_instname = kid.name;
    list_append(VisitedAlready, kid.id);

    // first off, if the node we are at has a dd.dt annotation, and that has
    // a box we want to search for the children there. 

    the_item = get_an_item_from_obj_notetype_itemtype(kid.id,
					"DataDefinition", "DataType");
    if (the_item != NULL && (! primitive_type(the_item.value)))
    {
	orig_kid = kid;
	kid = t_find_dse_node(the_item.value, NULL);
	if (kid == NULL)
	    kid = orig_kid;
	else
	{
	    the_item = NULL;

	    // now check this new kid for recursive definitions
	    if (list_find(VisitedAlready, 0, kid.id) != 
		   list_count(VisitedAlready))
	    {
		print_warning("[T-52] Error: `" + orig_kid.name + 
			      "', type `" + kid.name +		    
			      "':\n\trecursive definition. Skipping.");
		return False;
	    }
	}
    }

    // only do the following is there is no type annotation
    if (the_item == NULL)
    {
	query = "node[Sequence && node_refs && in_links[Component && from_node_id = ${kid.id}]]";
	baby_list = list_select(query);

	if (list_count(baby_list) == 0)
	{
	    query = "node[Selection && node_refs && in_links[Component && from_node_id = ${kid.id}]]";
	    baby_list = list_select(query);
	}
    }

    if (baby_list == NULL || list_count(baby_list) == 0)
    {
	if (the_item == NULL)
	    the_item = get_an_item_from_obj_notetype_itemtype(kid.id,
					    "DataDefinition", "DataType");

	if (the_item == NULL)
	{
	    print_warning("[T-35] Warning: Flow, parameter or data module `" + 
		instancename + "':\n\tDataType annotation for `" + 
		kid.name + "' not present. Skipping object.");
	    return False;
	}

	// issue a warning if needed
        if (! primitive_type(the_item.value))
	{
	    if (! predefined_type(the_item.value))
	        print_warning("[T-36] Warning: Flow, parameter or data module `" + 
		    instancename + "':\n\tDataType `" + 
		    the_item.value + "' is not a T primitive.\n\tIt must be defined in an include file for T to create test cases.");
	    tname = the_item.value;
	}

	the_adt_info.adt_name = instancename;
	the_adt_info.annot_id = kid.id;
	the_adt_info.scope = get_dse_object_scope(kid);
	if (tname == "")
	    the_adt_info.adt_type = the_adt_info.scope;
	else 
	    the_adt_info.adt_type = tname;

	the_adt_info.is_array = check_is_array(kid.id);
	list_append(ADTInfoList, the_adt_info);
    }
    else
    {
	for (i = 0; i < list_count(baby_list); i = i + 1)
	{
	    baby = list_get(baby_list, i);
	    if (list_find(VisitedAlready, 0, baby.id) != 
                   list_count(VisitedAlready))
	    {
		print_warning("[T-52] Error: `" + instancename + 
		              "', type `" + baby.name +	
                              "':\n\trecursive definition, skipping.");
		continue;
	    }
	    build_up_kid_list(baby, kid, instancename, rootboxname);

	    // each time we are done, we need to get rid of the last elem
            // of the VisitedAlready list
	    list_delete(VisitedAlready, (list_count(VisitedAlready) - 1));
	}
    }

    return True; 

} // end of build_up_kid_list()



// expand_obj_type:
//
// Grab type definitions from DSE.

void
expand_obj_type(int id, string name, string u_or_p, cntx the_cntx, node the_node, link the_link, string oname)

{
    item the_item;
    node box, new_box;
    string query;
    adt_info_tp the_adt_info;

    box = NULL;
    ADTInfoList = list_create("adt_info_tp", 0);
    ParentIsArray = False;

    // before building up the kid list, we need to find the
    // node in the dse that this thing belongs to. 

    // logic for parameters and globals is the same: get the DataType, look for it in DSE.
    // for flows, we find a box with the same name as the flow in DSE, get it's datatype, and look for
    // that in DSE.

    if (the_cntx != NULL) // Parameter?
    {
	the_item = get_an_item_from_obj_notetype_itemtype(id, 
						"ParameterDefinition", "DataType");

	if (the_item != NULL && primitive_type(the_item.value))
	{
	    print_warning("[T-37] Warning: Module `" + oname + "', parameter `" + name + "' (" + u_or_p + 
		"):\n\tType must not be a primitive T type. Skipping parameter.");
	}
	else if (the_item != NULL)
	{ 
            box = t_find_dse_node(the_item.value, NULL);

	    if (box == NULL)
	    {
	        if (! predefined_type(the_item.value))
		{
		    print_warning("[T-39] Warning: Module `" + oname + "', parameter `" + name + 
			"' (" + u_or_p + "):\n\tUnique type " + the_item.value + " not found in DSE.");
		}
	    }
	}
	else
	{
	    print_warning("[T-40] Warning: Module `" + oname + "', parameter `" + name + "' (" + u_or_p + 
	       "):\n\tNo type specified. Skipping Parameter.");
	}
    }
    else if (the_node != NULL) // Global?
    {
	the_item = get_an_item_from_obj_notetype_itemtype(id, 
						"GlobalDefinition", "DataType");

	if (the_item != NULL && primitive_type(the_item.value))
	{
	    print_warning("[T-45] Warning: Module `" + oname + "', data module `" + 
		name + "' (" + u_or_p + 
		"):\n\tType must not be a primitive T type. Skipping data module.");
	}
	else if (the_item != NULL)
	{ 
            box = t_find_dse_node(the_item.value, NULL);

	    if (box == NULL)
	    {
	        if (! predefined_type(the_item.value))
		{
		    print_warning("[T-39] Warning: Module `" + oname + "', data module `" + name + 
			"' (" + u_or_p + "):\n\tUnique type " + the_item.value + " not found in DSE.");
		}
	    }
	}
	else
	{
	    print_warning("[T-43] Warning: Module `" + oname + "', data module `" + name + "' (" + u_or_p + 
		"):\n\tNo type specified. Skipping data module.");
	}
    }
    else if (the_link != NULL)  // flow?
    {
        box = t_find_dse_node(name, NULL);

	if (box != NULL)
	{
	    // Check: if children, then if a DataType, then if a primitive T type.
	    if (has_kids(box) == 1)
	    {
		print_warning("[T-42] Warning: Process `" + oname + "', flow `" + name + "' (" + u_or_p + 
		    "):\n\tDSE instance must not contain children. Skipping flow.");
		box = NULL;
	    }
	    else
	    {
		the_item = get_an_item_from_obj_notetype_itemtype(box.id,
					    "DataDefinition", "DataType");

		if (the_item == NULL)
		{
		    print_warning("[T-44] Warning: Process `" + oname + "', flow `" + name + "' (" + u_or_p + 
			"):\n\tDSE instance has no DataType annotation. Skipping flow.");
		    box = NULL;
		}
		else if (primitive_type(the_item.value)) 
		{
		    print_warning("[T-46] Warning: Process `" + oname + "', flow `" + name + "' (" + u_or_p + 
			"):\n\tType must not be a primitive T type. Skipping flow.");
		    box = NULL;
		}
	    }

	    // so, finally, we have found a legit type if box != NULL.
	    // note that that implies that the_item will not be NULL either.
	
	    if (box != NULL)
	    {
		new_box = t_find_dse_node(the_item.value, NULL);

		if (new_box != NULL)
		    box = new_box;
		else
		{
		    // this is the case where the type does not exist in the
                    // design, we let this go, but issue a warning that if 
                    // it is no in include file, there will be a problem.
	            if (! predefined_type(the_item.value))
		    {
		        if (the_node != NULL) // data module
		            print_warning("[T-47] Warning: Module `" + oname + 
                                "', data module `" + name + "' (" + u_or_p + 
		                "):\n\tType `" + the_item.value + 
                                "' not defined in DSE.");
		        else
		            print_warning("[T-48] Warning: Process `" + oname + 
                                "', flow `" + name + "' (" + u_or_p + 
                                "):\n\tType `" + the_item.value + 
                                "' not defined in DSE.");
		    }
		}
	    }
	}
	else // no box in DSE
        {
	    if (the_node != NULL) // data module
		print_warning("[T-49] Warning: Module `" + oname + "', data module `" 
                    + name + "' (" + u_or_p + 
		    "):\n\tNo type specified. Skipping data module.");
	    else
		print_warning("[T-50] Warning: Process `" + oname + 
                               "', Flow `" + name + "' (" + u_or_p + 
                               "):\n\tNo type specified. Skipping flow.");
        }
    }

    if (box != NULL)
    {
	ParentIsArray = check_is_array(box.id);

	VisitedAlready = list_create("int", 0);
	if (build_up_kid_list(box, NULL, name, box.name))
            add_to_type_expansion(ADTInfoList, box.type, u_or_p, 
                          the_cntx, the_node, the_link);
    }

    return;
    
} // end of expand_obj_type()




int
no_selections_present(list te_l)

{
    int i;
    type_expansion_tp te;

    for (i = 0; te_l != NULL && i < list_count(te_l); i = i + 1)
    {
	te = list_get(te_l, i);
	if (te.seq_sel_iter == "Selection")
	    return 0;
    }
    return 1;

} // end of no_selections_present()


// find the DSE type corresponding to the name given. This routine needs to take scope into
// account. 

node 
t_find_dse_node(string name, string scope)

{
    list l;

    if (scope == NULL)
    {
	l = list_select("node[(Sequence || Selection) && node_refs[file[DseDiagram]] && name = '${name}']");
	if (list_count(l) == 0)
	    return NULL;
	else if (list_count(l) == 1)
	    return list_get(l, 0);
	else
	    print_message("[T-00] Warning: Type `" + name + "' has more then one definition in DSE.");
    }
    else
	print_error("Type `" + name + "': scope part not implemented yet"); 

    return NULL;
}



boolean
check_is_array(int id)

{
    item itm;

    if (ParentIsArray)
	return True;

    itm = find_by_query("item[ArraySize && obj_id = ${id}]");

    if (itm != NULL && itm.value != NULL && itm.value != "")
	return True;

    return False;
}

