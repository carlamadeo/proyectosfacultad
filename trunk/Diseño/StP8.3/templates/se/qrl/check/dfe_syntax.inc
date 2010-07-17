
//      sccsid[] = %W% %Y% %D%
//      StP/SE
//      Confidential property of Interactive Development Environments, Inc.
//      Copyright (c) 1995
//      All rights reserved
//
// decomp.inc: routines to help handle dfe decompositions

    
// Extra Syntax checking

boolean CollapseExplode = False;

struct n_pi_tp
{
    string n;    // oms name, not gde_node name
    int pi;
};

boolean
DfeCheckSyntax()
{
    boolean fine = True, ok;
    int i, j, k;
    set pi_s = set_create("int");
    set n_pi_s = set_create("n_pi_tp");
    n_pi_tp n_pi;
    gde_node cur_proc, gn, other_proc;
    string type, dname, name, pi;
    file obj;
    node n;
    list l, processes = gde_nodes_find_by_type(gde_all_symbols(), PROCESS);

    if (CollapseExplode == True)
	return True;

    /* by default the check succeeds */
    dname = gde_diagram_name();

    for (i = 0; i < list_count(processes); i++)
    {
    	cur_proc = list_get(processes, i);

	// check for any diagram: no duplicate process names. Need to 
	// look in repository for this one.
    
	n = gde_node_oms_process(cur_proc);
	if (n == NULL) // no process name.  Will be caught by built in check.
	    continue;

	obj = find_by_query("file[DfeDiagram && name != '${dname}' && node_refs[node[Process && name = '" + to_oms_string(n.name) + "']]]");

	if (obj != NULL)
        {
            print_error("Process `" + n.name + "' already exists in diagram `" + obj.name + "'.", 
                         gde_node_psymid(cur_proc));
            fine = False;
        }

	// check for reuse of pindex.  Does not check for single process with more then one index.

	pi = gde_node_prop(cur_proc, "pindex");
	if (str_has_val(pi))
	{
	    n_pi.n = n.name;
	    n_pi.pi = to_int(pi);
	    if (set_is_member(pi_s, n_pi.pi))
	    {
		if (! set_is_member(n_pi_s, n_pi))
		{
		    print_error("Process `" + n_pi.n + "' reuses pindex `" + n_pi.pi + "'.",
                         gde_node_psymid(cur_proc));
                    fine = False;
		}
	    }
	    else
	    {
		set_add(pi_s, n_pi.pi);
		set_add(n_pi_s, n_pi);
	    }

	    // single process with more then one index.  
	    for (j = 0; j < list_count(processes); j++)
	    {
		other_proc = list_get(processes, j);
		if (other_proc == cur_proc)
		    continue;
		if (n == gde_node_oms_process(other_proc) && 
		    gde_node_prop(other_proc, "pindex") != pi)
		{
		    print_error("Process `" + n.name + "' has more then one pindex.", gde_node_psymid(cur_proc));
		    fine = False;
		    break;
		}
	    }

	    // special check for top diagram

	    if (dname == "top" && pi != "0")
	    {
		print_error("Process on context diagram must have pindex of `0'", gde_node_psymid(cur_proc));
		fine = False;
	    }

	}

	// finally, check to be sure that there is an out link from all processes.  This can
        // not be done in rules, because there can be more then one instance of the process on
        // the diagram, and we want out links from the oms process, not the gde_node process

	if (list_count(gde_node_outarcs_no_comments(cur_proc)) == 0)
	{
	    ok = False;
	    for (j = 0; j < list_count(processes); j++)
	    {
	        other_proc = list_get(processes, j);
		if (other_proc == cur_proc)
		    continue;
		if (n == gde_node_oms_process(other_proc) && 
                    list_count(gde_node_outarcs_no_comments(other_proc)) != 0)
		{
		    ok = True;
		    break;
		}
	    }
	    if (! ok)
	    {
		print_error("Process `" + n.name + "' must have at least one outgoing link of type Control Flow or Data Flow.", gde_node_psymid(cur_proc));
		fine = False;
	    } 
	}
    }

    // check for context diagram: no offpages, at least one external, one
    // process, no cspecs;
    // or check for non context diagram: no externals

    if (dname == "top")
    {
	// The following could be done in one call, but the error messages are not a good
	l = gde_nodes_find_by_type(gde_all_symbols(), "ControlSpec");
	for (i = 0; i < list_count(l); i++)
	{
	    gn = list_get(l, i);
	    print_error("Cspec Bars not allowed on context (top) diagram.", gde_node_psymid(gn));
	    fine = False;
	}

	l = gde_nodes_find_by_type(gde_all_symbols(), "OffPageProcess");
	for (i = 0; i < list_count(l); i++)
	{
	    gn = list_get(l, i);
	    print_error("Offpage Processes are not allowed on context (top) diagram.", gde_node_psymid(gn));
	    fine = False;
	}

	l = gde_nodes_find_by_type(gde_all_symbols(), "OffPageExternal");
	for (i = 0; i < list_count(l); i++)
	{
	    gn = list_get(l, i);
	    print_error("Offpage Externals are not allowed on context (top) diagram.", gde_node_psymid(gn));
	    fine = False;
	}

	l = gde_nodes_find_by_type(gde_all_symbols(), "Anchor");
	for (i = 0; i < list_count(l); i++)
	{
	    gn = list_get(l, i);
	    print_error("Anchors are not allowed on context (top) diagram.", gde_node_psymid(gn));
	    fine = False;
	}

	l = gde_nodes_find_by_type(gde_all_symbols(), "Store");
	for (i = 0; i < list_count(l); i++)
	{
	    gn = list_get(l, i);
	    print_error("Stores not allowed on context (top) diagram.", gde_node_psymid(gn));
	    fine = False;
	}

	if (list_count(processes) != 1)
	{
	    print_error("Context (top) diagram must have exactly one Process.");
	    fine = False;
	}

	if (list_count(gde_nodes_find_by_type(gde_all_symbols(), EXTERNAL)) == 0)
	{
	    print_error("Context (top) diagram must have at least one External.");
	    fine = False;
	}

    }
    else
    {
	l = gde_nodes_find_by_type(gde_all_symbols(), "External");
	for (i = 0; i < list_count(l); i++)
	{
	    gn = list_get(l, i);
	    print_error("Externals are only allowed on context (top) diagrams.", gde_node_psymid(gn));
	    fine = False;
	}

	l = gde_nodes_find_by_type(gde_all_symbols(), "ExternalStore");
	for (i = 0; i < list_count(l); i++)
	{
	    gn = list_get(l, i);
	    print_error("External Stores are only allowed on context (top) diagrams.", gde_node_psymid(gn));
	    fine = False;
	}
    }

    if (check_split_flows() == False)
        fine = False;

    // and the default syntax

    if (gde_default_check_syntax() == False)
    {
        fine = False;
    }
    return fine;
}


boolean 
check_split_flows()

{
    int i;
    gde_node gn;
    list splits = gde_nodes_find_by_type(gde_all_symbols(), SPLIT_FLOW);
    boolean fine = True;

    for (i = 0; i < list_count(splits); i++)
    {
	gn = list_get(splits, i);

	if (list_count(gde_node_inarcs_no_comments(gn)) != 1 && list_count(gde_node_outarcs_no_comments(gn)) != 1)
	{
	    print_error("Split/Merge flows must have exactly one source flow OR exactly one destination flow.", gde_node_psymid(gn));
	    fine = False;
	}
    }

    return fine;
}



