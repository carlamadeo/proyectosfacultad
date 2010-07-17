
//      sccsid[] = %W% %Y% %D%
//      StP/SE
//      Confidential property of Interactive Development Environments, Inc.
//      Copyright (c) 1995
//      All rights reserved
//
// collapse.inc: routines to implement collapse functionality.    

#include "qrl/include/se_utils.inc"
#include "qrl/include/files.inc"
#include "rules/qrl/qrl_std.inc"
#include "rules/qrl/collapse_explode.inc"
#include "qrl/decomp/mv_decomp.inc"


int
idehook_collapse_active_func()
{
    list symbols = gde_selected_symbols();
    int numprocs;

    // Do not collapse if this is diagram top or 0 or no name
    if (str_no_val(gde_diagram_name()) || gde_diagram_name() == "top" || gde_diagram_name() == "0")
	return ACTIVE_FUNC_IS_INACTIVE;
     
    if (list_count(symbols) == 0)
	return ACTIVE_FUNC_IS_INACTIVE;

    numprocs = list_count(gde_nodes_find_by_type(symbols, PROCESS));

    if (numprocs < 2 || numprocs != list_count(symbols))
	return ACTIVE_FUNC_IS_INACTIVE;

    return ACTIVE_FUNC_IS_ACTIVE;
}

int FileId;

void
CollapseProcessesPrompt(int id)

{
    if (id < 1)
    {
	print_error("Save the current diagram first. Collapse cancelled.");
	return;
    }

    FileId = id;

    // pop up the prop sheet
    ui_prop_sheet_pop_up("DfeNewPname");

    return;
}

//
//  Main collapse function
//

struct collapse_tp
{
    gde_node gn;
    string name; 
    int pi;
};    


void
debug_procinfo(list l)

{
    int i, j;
    collapse_tp p;

    for (j = 0; j < list_count(l); j++)
    {
	p = list_get(l, j);
	print_message("Node: " + gde_node_label(p.gn));
	print_message("Nodename(as above): " + p.name);
	print_message("Id: "+ p.pi);
    }
}



int
CancelCollapse_cb(ui_event_tp event, ui_prop theprop, int id, string value)

{
    print_error("Collapse cancelled.");
    
    return 1;
}



    
int
DoCollapse_cb(ui_event_tp event, ui_prop theprop, int id, string value)

{
    string newproc, pindex;
    file file, dfile;
    list procl; 
    gde_node master_gn, gn;
    int master_pindex, i, rc;
    list collapsel = list_create("collapse_tp", 0);
    collapse_tp procinfo;
    ui_prop prop;
    node n;
    note pspec;
    int handle;

    if (ui_button_is_ok(theprop))
        rc = 1;
    else
        rc = 0;

    prop = to_ui_prop("DfeNewPname", "NewProcessName");
    newproc = ui_text_value(prop);

    if (str_no_val(newproc))
    {
	print_error("A new process name is required. Collapse cancelled.");
	return 0;
    }
    if (idehook_collapse_active_func() != ACTIVE_FUNC_IS_ACTIVE)
    {
	print_error("Selection no longer valid. Collapse cancelled.");
	return  0;
    }

    // Do not collapse if this is diagram top or 0
    if (gde_diagram_name() == "top" || gde_diagram_name() == "0")
    {
	print_error("Cannot collapse `" + gde_diagram_name() + "'. Collapse cancelled.");
	return  0;
    }

    editor_set_is_busy(True);

    gde_builtin("FileSave");
    if ((file = find_by_query("file[id = ${FileId}]")) == NULL)
    {
	print_error("Diagram must first be saved. Collapse cancelled.");
        editor_set_is_busy(False);
    	return  0;
    }

    procl = gde_nodes_find_by_type(gde_selected_symbols(), PROCESS);

    master_pindex = -1;
    // find the master proc, and create a list of decompositions for each process
    for (i = 0; i < list_count(procl); i++)
    {
    	gn = list_get(procl, i);
    	pindex = gde_node_prop(gn, "pindex");
	if (pindex == NULL || pindex == "")
	{
	    print_error("No pindex for `" + gde_node_label(gn) + "'. Collapse cancelled.");
            editor_set_is_busy(False);
	    return  0;
	}
	procinfo.gn = gn;
	procinfo.name = gde_node_label(gn);
	procinfo.pi = to_int(pindex);

	if ((master_pindex == -1) || (to_int(pindex) < master_pindex))
	{ 
	    master_pindex = to_int(pindex); 
	    if (list_count(collapsel) == 0)
	        list_append(collapsel, procinfo);
	    else		
		list_insert(collapsel, 0, procinfo);
	}
	else
	    list_append(collapsel, procinfo);

    }

    //debug_procinfo(collapsel); 

    CollapseExplode = True;  // Turn off extra syntax checks

    // Move any process decompositions down
    for (i = 0; i < list_count(collapsel); i++)
    {    
	procinfo = list_get(collapsel, i);
	dfile = FindDecomposition(file.name, procinfo.pi);
	if (dfile == NULL)
	    continue;

	// call the same move decomp as everybody else does.
	do_mv_decomp(dfile, file.name + "." + master_pindex + "." + procinfo.pi, True, False);
    }

    // copy all the necessary bits into the copy buffer

    expand_copy_buffer(collapsel);

    // modify existing diagram
    master_gn = collapse_parent_diagram(collapsel, newproc);

    // clear diagram and create new diagram from the results in the copy buffer
    create_new_diagram(collapsel, file.name, master_pindex);

    CollapseExplode = False;

    gde_builtin("FileLoad " + file.name);

    // remove pspec from the newly collapsed process on the parent diagram
    n = gde_node_oms_process(master_gn);
    if (n != NULL)
    {
	pspec = find_by_query("note[Pspec && obj_id = ${n.id}]");
	if (pspec != NULL)
	{
	    handle = send_start_edit_annot(n.id);
	    print_message("Removing Pspec for `" + n.name + "'.");
	    editor_send_msg(NGOAE_CLASS, OAE_CMD_NOTE_SEL + "Pspec", handle);
	    editor_send_msg(NGOAE_CLASS, "NoteDelete", handle);
	    send_oae_save_update_unload(handle);
	}
    }

    print_message("Collapse to `" + newproc + "' complete.");

    editor_set_is_busy(False);
    return  rc;
}




void 
expand_copy_buffer(list collapsel)

{
    int i, j;
    list arcs, cntxs;
    gde_arc arc, newarc;
    gde_node gn;
    gde_cntx cntx;
    collapse_tp proc;
    string label, str;
    set appids;

    appids = set_create("int");
    // create new arc segments, moving labels and contexts.  The other arcs
    // will go away when the node is deleted
    for (i = 0; i < list_count(collapsel); i++)
    {
	proc = list_get(collapsel, i);
	set_add(appids, gde_node_psymid(proc.gn));
	arcs = gde_node_inarcs(proc.gn);
	for (j = 0; j < list_count(arcs); j++)
	{
	    arc = list_get(arcs, j);
	    get_arc_appids(arc, appids);

	    gn = gde_arc_get_real_fromnode(arc, True, NULL, NULL);
	    set_add(appids, gde_node_psymid(gn));

	    // what about the qualfifying contexts
	}

	arcs = gde_node_outarcs(proc.gn);
	for (j = 0; j < list_count(arcs); j++)
	{
	    arc = list_get(arcs, j);
	    get_arc_appids(arc, appids);

	    gn = gde_arc_get_real_tonode(arc, True, NULL, NULL);
	    set_add(appids, gde_node_psymid(gn));
	    // what about the qualfifying contexts
	}
    }

    str = set_to_string(appids, " ");
    gde_builtin("SymbolSelect AppId 0");
    gde_builtin("SymbolSelect AppId " + str);

    gde_builtin("SymbolCopy");

    return;
}




//
//  Alter the parent diagram to reflect the collapse.  Rename
//  the master, keeping her pindex. Connect all the flows to/from
//  the remaining processes to the master.  Do not do anything to try and
//  make the diagram look better at this point.
//

gde_node 
collapse_parent_diagram(list collapsel, string newname)
{
    int i, j, txn;
    list arcs, cntxs;
    gde_arc arc, newarc;
    gde_node fromgn, togn;
    gde_cntx cntx;
    collapse_tp proc, master;
    string label;

    gde_save_state();
    txn = gde_start_edit_transaction();

    master = list_get(collapsel, 0);
    gde_node_set_label(master.gn, newname);

    // create new arc segments, moving labels and contexts.  The other arcs
    // will go away when the node is deleted
    for (i = 1; i < list_count(collapsel); i++)
    {
	proc = list_get(collapsel, i);
	arcs = gde_node_inarcs(proc.gn);
	for (j = 0; j < list_count(arcs); j++)
	{
	    arc = list_get(arcs, j);
	    fromgn = gde_arc_fromnode(arc);
	    if (node_being_collapsed(gde_arc_get_real_fromnode(arc, True, NULL, NULL), collapsel))
		gde_arc_delete(arc);
	    else
	    {
		newarc = gde_arc_create(gde_lt_name(gde_arc_lt(arc)), gde_arc_fromnode(arc), master.gn);
		label = gde_arc_label(arc);
		if (str_has_val(label))
		    gde_arc_set_label(newarc, label);
	    }
	}

	arcs = gde_node_outarcs(proc.gn);
	for (j = 0; j < list_count(arcs); j++)
	{
	    arc = list_get(arcs, j);
	    togn = gde_arc_tonode(arc);
	    if (node_being_collapsed(gde_arc_get_real_tonode(arc, True, NULL, NULL), collapsel))
		gde_arc_delete(arc);
	    else
	    {
		newarc = gde_arc_create(gde_lt_name(gde_arc_lt(arc)), master.gn, gde_arc_tonode(arc));
		label = gde_arc_label(arc);
		if (str_has_val(label))
		    gde_arc_set_label(newarc, label);
	    }
	}

	gde_node_delete(proc.gn);
    }

    remove_orphans();

    gde_end_edit_transaction(GDE_CMD_INSERT, txn, False);
    gde_builtin("FileSave");

    return master.gn;
}



void
create_new_diagram(list collapsel, string fname, int pindex)

{
    // go through and change all the processes not involved in the collapse to externals.  
    // do not do anything else.

    int i, j, txn;
    gde_node gn;
    list gns;
    boolean extern;
    collapse_tp proc;

    gde_builtin("BufferClear");
    gde_builtin("BufferPaste");
    gde_builtin("BufferCenter");
    gde_map_all_symbols();

    gde_save_state();
    txn = gde_start_edit_transaction();

    gns = gde_nodes_find_by_type(gde_all_symbols(), "Process");
    for (i = 0; i < list_count(gns); i++)
    {
	gn = list_get(gns, i);
	extern = True;
	for (j = 0; j < list_count(collapsel); j++)
	{
	    proc = list_get(collapsel, j);
	    if (proc.name == gde_node_label(gn))
	    {
		extern = False;
		break;
	    }
	}
	if (extern)
	    gde_node_replace_nt(gn, "OffPageProcess");    
    }

    remove_orphans();

    gde_end_edit_transaction(GDE_CMD_INSERT, txn, False);
    gde_builtin("FileSaveas " + fname + "." + pindex);

    return;
}




boolean
node_being_collapsed(gde_node gn, list collapsel)
{
    int i;
    collapse_tp proc;


    for (i = 0; i < list_count(collapsel); i++)
    {
	proc = list_get(collapsel, i);
	if (proc.gn == gn)
	    return True;
    }
    return False;
}
	

void
get_arc_appids(gde_arc arc, set apps)

{
    int i, j;
    list lks, arcs;
    gde_link lk;
    gde_arc a;

    lks = gde_arc_links(arc);
    for (i = 0; i < list_count(lks); i++)
    {
	lk = list_get(lks, i);
	arcs = gde_link_arcs(lk);
	for (j = 0; j < list_count(arcs); j++)
	{
	    a = list_get(arcs, j);
	    set_add(apps, gde_arc_psymid(a));
	    set_add(apps, gde_node_psymid(gde_arc_tonode(a)));
	    set_add(apps, gde_node_psymid(gde_arc_fromnode(a)));
	}
    }
    return;
}



// This should probably be in a common place somewhere
string
set_to_string (set the_set, string separator)
{
    int         i;
    string      return_string;

    return_string = "";
    for (i=0; i<set_count(the_set); i++)
    {
        if (i>0)
            return_string = return_string + separator;
        return_string = return_string + to_string(set_get_element(the_set, i));
    }
    return return_string;
}

