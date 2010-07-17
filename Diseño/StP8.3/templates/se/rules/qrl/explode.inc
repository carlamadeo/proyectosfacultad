
//      sccsid[] = %W% %Y% %D%
//      StP/SE
//      Confidential property of Interactive Development Environments, Inc.
//      Copyright (c) 1995
//      All rights reserved
//
// explode.inc: routines to implement explode functionality.
 

#include "qrl/include/dfe_defs.inc"
#include "rules/qrl/qrl_std.inc"
#include "rules/qrl/move_decomp.inc"
#include "qrl/include/files.inc"
#include "qrl/include/pindex.inc"
#include "rules/qrl/collapse_explode.inc"
#include "qrl/decomp/mv_decomp.inc"

// A list of these are created for the decompositon being 
// exploded.

struct explode_tp
{
    node n;
    int oldpi;
    int newpi;
    file f; 
};



// Location master information

struct diag_location_tp
{
    int minx;
    int maxx;
    int miny;
    int maxy;
};

int
idehook_explode_active_func()
{
    list symbols = gde_selected_symbols();
    gde_symbol sym;

    // Do not explode if this is diagram top or 0 or no name
    if (str_no_val(gde_diagram_name()) || gde_diagram_name() == "top" || gde_diagram_name() == "0")
        return ACTIVE_FUNC_IS_INACTIVE;

    if (list_count(symbols) != 1)
	return ACTIVE_FUNC_IS_INACTIVE;

    sym = list_get(symbols, 0);
    if (gde_symbol_is_node(sym) &&
	gde_nt_name(gde_node_nt(to_gde_node(sym))) == PROCESS)
	return ACTIVE_FUNC_IS_ACTIVE;

    return ACTIVE_FUNC_IS_INACTIVE;
}

node
get_cur_process_node(gde_node gn)

{
    list l;
    int i;
    node n;

    l = gde_node_oms_nodes(gn);
    for (i = 0; i < list_count(l); i++)
    {
	n = list_get(l, i);
	if (n.type == "Process")
	    return n;
    }
    return NULL;
}



//
//  Replace the process selected with its decomposition
//  diagram.  Preserve the links to/from other nodes, and move subsequent
//  decompositions up a level.
//
void
ExplodeProcess(int file_id)
{
    file parent_file, decomp_file;
    int txn, i, pindex, psymid;
    gde_node process;
    diag_location_tp parent_diag_loc;
    list decomps_to_mv, l;
    explode_tp di;
    item it;
    string psymids = "", exploded_proc_name, command;
    gde_symbol sym;
    node n;

    // Some of the checks are caught by the active func, but having them again makes this 
    // routine more bomb proof in case anybody else decides to call it (not recommended).

    // Do not explode if this is diagram top or 0
    if (gde_diagram_name() == "top" || gde_diagram_name() == "0")
    {
        print_error("Cannot explode '" + gde_diagram_name() + "'. Explode cancelled.");
        return  rc;
    }

    parent_file = find_by_query("file[id = ${file_id}]");
    if (parent_file == NULL)
    {
	print_error("Name the current diagram first. Explode cancelled.");
	return;
    }

    // find pindex...
    n = get_cur_process_node(to_gde_node(list_get(gde_selected_symbols(), 0)));
    if (((it = find_by_query("item[RelativeIndex && obj_id = ${n.id}]")) == NULL) || 
        (str_no_val(it.value)))
    {
	print_error("Could not find current process' index. Explode cancelled."); 
	return;
    }

    pindex = to_int(it.value);
    decomp_file = FindDecomposition(parent_file.name, pindex);
    if (decomp_file == NULL)
    {
    	print_error("Selected Process has no decomposition. Explode cancelled.");
    	return;
    }

    // the active func assures that we have a gde_node
    process = to_gde_node(list_get(gde_selected_symbols(), 0));
    exploded_proc_name = gde_node_label(process);

    gde_builtin("FileSave");

    // find the x and y ranges of the current file.  We will just put
    // the new diagram below this one, rather then attempt to make it
    // nicer.

    parent_diag_loc = find_current_diagram_location();
    decomps_to_mv = find_decomp_info(decomp_file, find_max_pindex() + 1);

    txn = gde_start_edit_transaction();

    // go to decomposition
    //gde_builtin("FileSave");
    gde_builtin("FileLoad " + decomp_file.name);

    // select all symbols
    gde_builtin("SymbolSelect AppId 0");
    l = gde_all_symbols();
    for (i = 0; i < list_count(l); i++)
    {
	sym = list_get(l, i);
	if (gde_symbol_is_node(sym))
	    psymid = gde_node_psymid(to_gde_node(sym));
	else if (gde_symbol_is_arc(sym))
	    psymid = gde_arc_psymid(to_gde_arc(sym));
	else if (gde_symbol_is_cntx(sym))
	    psymid = gde_cntx_psymid(to_gde_cntx(sym));
	else 
	    // Should probably not get here
	    psymid = -1;

	if (psymid != -1)
	    psymids += to_string(psymid) + " ";
    }
    gde_builtin("SymbolSelect AppId " + psymids);
    gde_builtin("SymbolCopy");

    // back to the parent
    gde_builtin("FileLoad " + parent_file.name);

    // Change selection to an anchor and trim the diagram
    gde_node_replace_nt(process, "Anchor");
    remove_orphans();

    gde_builtin("BufferPaste");
    gde_builtin("SymbolMove " + parent_diag_loc.maxx + " " + (parent_diag_loc.maxy * 2));

    // now go through and renumber all the processes that are selected

    l = gde_nodes_find_by_type(gde_selected_symbols(), "Process");
    for (i = 0; i < list_count(l); i++)
    {
	process = list_get(l, i);
        di = get_di_from_list(decomps_to_mv, gde_node_label(process));
        gde_node_set_prop(process, "pindex", to_string(di.newpi));
    }
    
    // for safety, clear the paste buffer and unselect everything
    gde_builtin("SymbolSelect AppId 0");
    gde_builtin("SymbolCopy");

    // Move any decomps for those renumbered processes
    for (i = 0; i < list_count(decomps_to_mv); i++)
    {
        di = list_get(decomps_to_mv, i); 
	if (di.f != NULL)
	    do_mv_decomp(di.f, parent_file.name + "." + di.newpi, True, True);
    }

    //gde_builtin("FileSave");
    gde_builtin("BufferCenter");
    gde_builtin("BufferUpdateDmarks");
    gde_end_edit_transaction(GDE_CMD_REPLACE, txn, False);
    gde_map_all_symbols();

    // remove the child diagram
    command = "stputil -s " + current_system() + " -p " + current_projdir() + " -verbose -Quiet -Force  -C 'delete -e dydfe -n " + decomp_file.name + "'"; 
    system(command);

    
    print_message("Explode of `" + exploded_proc_name + "' complete.");

    return;
}


explode_tp
get_di_from_list(list l, string name)

{
    explode_tp di;
    int i;

    for (i = 0; i < list_count(l); i++)
    {
	di = list_get(l, i);
	if (di.n.name == name)
	    return di;
    }
    return NULL;
}



int find_max_pindex()

{
    list procl;
    gde_node gn;
    int pindex, max_pindex = -1, i;
    string pi;

    procl = gde_nodes_find_by_type(gde_all_symbols(), PROCESS);

    for (i = 0; i < list_count(procl); i++)
    {
    	gn = list_get(procl, i);
    	pi = gde_node_prop(gn, "pindex");
	if (pi == NULL || pi == "")
	    continue;
    	pindex = to_int(pi);
	if (pindex > max_pindex)
	    max_pindex = pindex;
    }
    return max_pindex;
}




diag_location_tp find_current_diagram_location()

{
    diag_location_tp dl;
    list l;
    int i, x, y;
    gde_node gn;

    l = gde_symbols_find_with_types(gde_all_symbols(), string_to_list("Process External OffPageProcess OffPageExternal Anchor Cspec Vertex", " "));

    for (i = 0; i < list_count(l); i++)
    {
	gn = to_gde_node(list_get(l, i));

	if (i == 0)
	{
	    dl.minx = dl.maxx = gde_node_x(gn);
	    dl.miny = dl.maxy = gde_node_y(gn);
	}
	else
	{
	    x = gde_node_x(gn);
	    y = gde_node_y(gn);
	    if (x < dl.minx)
		dl.minx = x;
	    if (x > dl.maxx)
		dl.maxx = x;
	    if (y < dl.miny)
		dl.miny = y;
	    if (y > dl.maxy)
		dl.maxy = y;
	}
    }

    return dl;
}


list find_decomp_info(file dfile, int startpindex)

{
    list l, decomps_to_move = list_create("explode_tp", 0);
    explode_tp di;
    item it;
    int i;

    l = list_select("node[Process && node_refs[file[id = ${dfile.id}]]]");
    for (i = 0; i < list_count(l); i++)
    {
	di.n = list_get(l, i);
        if (((it = find_by_query("item[RelativeIndex && obj_id = ${di.n.id}]")) != NULL) && 
            (str_has_val(it.value)))
	{
	    di.oldpi = to_int(it.value);
	    di.f = FindDecomposition(dfile.name, to_int(it.value));
	}

	di.newpi = startpindex + i;
	list_append(decomps_to_move, di);
    }
    return decomps_to_move;
}

