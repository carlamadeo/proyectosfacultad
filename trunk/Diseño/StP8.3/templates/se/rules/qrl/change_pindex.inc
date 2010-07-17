//
//      sccsid[] = %W% %Y% %D%
//      StP/SE
//      Confidential property of Interactive Development Environments, Inc.
//      Copyright (c) 1995
//      All rights reserved
//
// change_pindex.inc: code to change a process index.
 

#include "rules/qrl/qrl_std.inc"
#include "qrl/decomp/mv_decomp.inc"

gde_node CurGN;

int
idehook_change_pindex_active_func()
{
    list processes = gde_selected_symbols();
    gde_symbol process;

//    if (gde_diagram_name() == "top")
//	return ACTIVE_FUNC_IS_INACTIVE;

    if (list_count(processes) == 1)
    {
    	process = list_get(processes, 0);
    	if (gde_symbol_is_node(process) 
    	    && gde_nt_name(gde_node_nt(to_gde_node(process))) == PROCESS)
	{
	    CurGN = to_gde_node(process);
    	    return ACTIVE_FUNC_IS_ACTIVE;
	}
    }    	
    return ACTIVE_FUNC_IS_INACTIVE;
}


void
ChangeProcessIndexPrompt()
{
    // pop up the prop sheet
    ui_prop_sheet_pop_up("DfeNewPindex");

    return;
}

//
// Changes a Process' Index and, if necessary, moves its decomposition. 
// Must store and buffer each argument one at a time due to filter text 
// input interface restrictions.
//

int
DoChangePindex_cb(ui_event_tp event, ui_prop theprop, int id, string value)
{
    list processes, all_processes;
    gde_symbol procsym;
    gde_node other_process;
    string dname = gde_diagram_name(), str, old_pindex, query, process_label, decomp_name;
    file decomp_file;
    int i, txn, new_pindex, mv_decomp;
    node master_n;


    // first reverify the current selection
    if (idehook_change_pindex_active_func() == ACTIVE_FUNC_IS_INACTIVE)
    {
	print_error("Current selection is no longer valid. Change Pindex cancelled.");
	return 0;
    }

    str = ui_text_value(to_ui_prop("DfeNewPindex", "NewPindex"));
    mv_decomp = ui_choice_value(to_ui_prop("DfeNewPindex", "Decomps"));

    if (!string_converts(str, "int"))
    {
	print_error("Process index `" + str + "' is invalid. Change Process Index cancelled.");
	return 0;
    }

    new_pindex = to_int(str);
    if (new_pindex <= 0)
    {
        if (! ((dname == NULL || dname == "" || dname == "top") && new_pindex == 0))
	{
	    print_error("Process index must be >= 1. Change Process Index cancelled.");
	    return 0;
	}
    }

    process_label = gde_node_label(CurGN);

    // never allow a process index to be changed to the index of another
    // process on the diagram.

    master_n = gde_node_oms_process(CurGN);
    all_processes = gde_nodes_find_by_type(gde_all_symbols(), PROCESS);
    for (i = 0; i < list_count(all_processes); i++)
    {
    	other_process = list_get(all_processes, i);
    	if (CurGN != other_process
    	    && gde_node_prop(other_process, "pindex") == to_string(new_pindex))
    	{
	    // if the two processes map to the same object, it is still ok.
            // note that doing the obvious, and just checking the names, is not enough, 
            // because of space handling policies.  Same for below when changing pindex's
	    if (master_n == gde_node_oms_process(other_process))
		continue;

    	    print_error("Process index already used.  Change Process Index cancelled.",
    	    	    	    gde_node_psymid(other_process));
            return 0;
    	}
    }

    old_pindex = gde_node_prop(CurGN, "pindex");

    if (new_pindex == to_int(old_pindex))
    {
    	print_error("New index must be different then current index.  Change Process Index cancelled.");
    	return 0;
    }

    editor_set_is_busy(True);
    gde_save_state();
    txn = gde_start_edit_transaction();

    // set the new pindex to all the processes with this name
    all_processes = gde_nodes_find_by_type(gde_all_symbols(), PROCESS);
    for (i = 0; i < list_count(all_processes); i++)
    {
    	other_process = list_get(all_processes, i);
	if (master_n == gde_node_oms_process(other_process))
            gde_node_set_prop(other_process, "pindex", to_string(new_pindex));
    }

    if (dname != NULL && dname != "" && mv_decomp == 1)
    {
	decomp_file = FindDecomposition(dname, to_int(old_pindex));
	if (decomp_file != NULL)
	{
	    if (dname == "0")
	        do_mv_decomp(decomp_file, to_string(new_pindex), True, False);
	    else
	        do_mv_decomp(decomp_file, dname + "." + to_string(new_pindex), True, False);
	}
    }

    gde_end_edit_transaction(GDE_CMD_INSERT, txn, False);

    print_message("Change Process Index completed.");
    editor_set_is_busy(False);

    if (ui_button_is_ok(theprop))
        return 1;
    else
        return 0;

}


int
CancelChangePindex_cb(ui_event_tp event, ui_prop theprop, int id, string value)
 
{
    print_error("Change Pindex cancelled.");
 
    return 1;
}

