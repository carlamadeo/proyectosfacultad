
//////////////////////////////////////////////////////////////////////////////
//
//      StP/SE
//      Confidential property of Aonix
//      Copyright (c) 1992-1997
//      All rights reserved
//
//////////////////////////////////////////////////////////////////////////////


#include "qrl/include/se_utils.inc"
#include "qrl/include/general.inc"

boolean rename_diagram = False;
boolean rename_sce_diagram = False;
boolean rename_flows = False;
boolean rename_stores = False;
boolean rename_type_annots = False;
boolean rename_data_objects = False;
boolean rename_dse_diagram = False;
boolean rename_control_in_out = False;
boolean rename_offpage_externals = False;
boolean rename_offpage_processes = False;
boolean rename_data_objects_store = False;
boolean rename_data_flows_store = False;
boolean rename_dse_diagram_store = False;
boolean rename_control_flows = False;
boolean rename_dse_for_control_in_out = False;

// rename func for EventAction Symbol

void
ste_event_action_rename_init(gde_mapsymbol ms)
{
    gor_current_mapsym = ms;

    ui_set_value(to_ui_prop("EventActionRename", "NewEventLabel"), 
		 gde_cntx_label(to_gde_cntx(ms), "aux"));

    ui_set_value(to_ui_prop("EventActionRename", "NewActionLabel"), 
		 gde_cntx_label(to_gde_cntx(ms)));
}

void
ste_event_action_rename(gde_mapsymbol ms)
{
    gde_builtin("PropertySheetShow EventActionRename");
    ste_event_action_rename_init(ms);
}

void
ste_event_action_rename_action_done()
{
    gde_mapsymbol ms = gde_selected_mapsymbol();
    ui_prop p = to_ui_prop("EventActionRename", "All");
    
    if (gde_rename_action_done_standard_checks(ms, p, "ste_event_action_rename") == False)
	return;

    ste_event_action_rename_init(ms);
}

int
ste_event_action_rename_callback(ui_event_tp ev, ui_prop p, int id, string val)
{
    return ste_rename(ev, p, id, val, False);
}

int
ste_event_action_check_callback(ui_event_tp ev, ui_prop p, int id, string val)
{
    return ste_rename(ev, p, id, val, True);
}

int
ste_event_action_cancel_callback(ui_event_tp ev, ui_prop p, int id, string val)
{
    return 1;
}

int
ste_rename(ui_event_tp ev, ui_prop p, int id, string val, boolean check)
{
    string owner;
    string new_event_label, new_action_label;
 
    owner = ui_owner_name(p);
    ui_set_value(to_ui_prop(owner,"Status"),"");
    new_event_label = ui_value(to_ui_prop(owner,"NewEventLabel"));
    new_action_label = ui_value(to_ui_prop(owner,"NewActionLabel"));
 
    if (new_event_label == "" && new_action_label == "")
    {
	ui_set_value(to_ui_prop(owner,"Status"),"Enter a New Label.");
	return 0;
    }

    if (new_event_label == gde_cntx_label(to_gde_cntx(gor_current_mapsym), "aux") &&
        new_action_label == gde_cntx_label(to_gde_cntx(gor_current_mapsym)))
    {
	ui_set_value(to_ui_prop(owner,"Status"),"Enter a Different Label."); 
        return 0;
    }

    if (check)
    {
	editor_set_is_busy(True);
	gde_mapsym_global_rename(gor_current_mapsym, new_action_label, new_event_label, True);
	editor_set_is_busy (False);
	return 0;

    } else
    {
	editor_set_is_busy(True);
	gde_mapsym_global_rename(gor_current_mapsym, new_action_label, new_event_label, False);
	editor_set_is_busy (False);
	return 1;
    }
}


void
ste_event_inverse_map_func(gde_mapsymbol ms, node n, list rename_data,
			   gocha_tp gocha_type)
{
    string oldlabel = NULL;

    if ( gocha_type != gochaRename )
    {
	print_error( "ste_event_inverse_map_func() can only handle renames" );
	return; 
    }

    oldlabel = gde_cntx_label(to_gde_cntx(ms), "aux");
    gde_cntx_set_label(to_gde_cntx(ms), "aux", n.name);
    gde_print_rename_notification_message(ms,
		gde_mapsymbol_symbol_type_printname(ms) +
		" renamed from '" + oldlabel + "' to '" + n.name + "'");
}
	 
void
ste_action_inverse_map_func(gde_mapsymbol ms, node n, list rename_data,
			    gocha_tp gocha_type)
{
    string oldlabel = NULL;

    if ( gocha_type != gochaRename )
    {
	print_error( "ste_action_inverse_map_func() can only handle renames" );
	return; 
    }

    oldlabel = gde_cntx_label(to_gde_cntx(ms));
    gde_cntx_set_label(to_gde_cntx(ms), n.name);
    gde_print_rename_notification_message(ms,
	    gde_mapsymbol_symbol_type_printname(ms) +
	    " renamed from '" + oldlabel + "' to '" + n.name + "'");
}

////////////////////////////////
// rename func for data objects
////////////////////////////////
 
void
se_data_rename_init(gde_mapsymbol ms)
{
    gor_current_mapsym = ms;
 
    gor_current_label = gde_node_label(to_gde_node(gor_current_mapsym));

    ui_set_value(to_ui_prop("DataObjectRename", "NewLabel"), gor_current_label);

    // Initialize property sheet checkboxes

    if (rename_diagram)
	ui_set_value(to_ui_prop("DataObjectRename", "RenameDiagram"), 1);
    else 
	ui_set_value(to_ui_prop("DataObjectRename", "RenameDiagram"), 0);
	
    if (rename_flows)
	ui_set_value(to_ui_prop("DataObjectRename", "RenameFlows"), 1);
    else 
	ui_set_value(to_ui_prop("DataObjectRename", "RenameFlows"), 0);
	
    if (rename_stores)
	ui_set_value(to_ui_prop("DataObjectRename", "RenameStores"), 1);
    else 
	ui_set_value(to_ui_prop("DataObjectRename", "RenameStores"), 0);

    if (rename_type_annots)
	ui_set_value(to_ui_prop("DataObjectRename", "RenameTypeAnnots"), 1);
    else 
	ui_set_value(to_ui_prop("DataObjectRename", "RenameTypeAnnots"), 0);
}
 
void
se_data_rename(gde_mapsymbol ms)
{
    gde_builtin("PropertySheetShow DataObjectRename");
    se_data_rename_init(ms);
}


void
se_data_rename_action_done()
{
    gde_mapsymbol ms = gde_selected_mapsymbol();
    ui_prop p = to_ui_prop("DataObjectRename", "All");
    
    if (gde_rename_action_done_standard_checks(ms, p, "se_data_rename") == False)
	return;

    se_data_rename_init(ms);
}

int
data_rename_diagram_callback(ui_event_tp ev, ui_prop p, int id, string value)
{
    rename_diagram = ui_choice_isset(p, 0);
    return 0;
}

int
data_rename_flows_callback(ui_event_tp ev, ui_prop p, int id, string value)
{
    rename_flows = ui_choice_isset(p, 0);
	
    return 0;
}

int
data_rename_stores_callback(ui_event_tp ev, ui_prop p, int id, string value)
{
    rename_stores = ui_choice_isset(p, 0);
    return 0;
}

int
data_rename_type_annots_callback(ui_event_tp ev, ui_prop p, int id, string val)
{
    rename_type_annots = ui_choice_isset(p, 0);
    return 0;
}


//////////////////////////////
// rename func for dataflows
//////////////////////////////
 
void
dfe_data_flow_rename_init(gde_mapsymbol ms)
{
    gor_current_mapsym = ms;
 
    ui_set_value(to_ui_prop("DataFlowRename", "NewLabel"),
         gde_link_label(to_gde_link(ms)));

    // Initialize property sheet checkboxes
 
    if (rename_data_objects)
        ui_set_value(to_ui_prop("DataFlowRename", "RenameDataObjects"), 1);
    else 
        ui_set_value(to_ui_prop("DataFlowRename", "RenameDataObjects"), 0);

    if (rename_dse_diagram)
        ui_set_value(to_ui_prop("DataFlowRename", "RenameDseDiagram"), 1);
    else 
        ui_set_value(to_ui_prop("DataFlowRename", "RenameDseDiagram"), 0);
}
 
void
dfe_data_flow_rename(gde_mapsymbol ms)
{
    gde_builtin("PropertySheetShow DataFlowRename");
    dfe_data_flow_rename_init(ms);
}

void
dfe_data_flow_rename_action_done()
{
    gde_mapsymbol ms = gde_selected_mapsymbol();
    ui_prop p = to_ui_prop("DataFlowRename", "All");
    
    if (gde_rename_action_done_standard_checks(ms, p, "dfe_data_flow_rename") == False)
	return;

    dfe_data_flow_rename_init(ms);
}

////////////////////////////////
// rename func for controlflows
////////////////////////////////
 
void
dfe_control_flow_rename_init(gde_mapsymbol ms)
{
    gor_current_mapsym = ms;
 
    ui_set_value(to_ui_prop("ControlFlowRename", "NewLabel"),
         gde_link_label(to_gde_link(ms)));

    // Initialize property sheet checkboxes
 
    if (rename_data_objects)
        ui_set_value(to_ui_prop("ControlFlowRename", "RenameDataObjects"), 1);
    else 
        ui_set_value(to_ui_prop("ControlFlowRename", "RenameDataObjects"), 0);

    if (rename_dse_diagram)
        ui_set_value(to_ui_prop("ControlFlowRename", "RenameDseDiagram"), 1);
    else 
        ui_set_value(to_ui_prop("ControlFlowRename", "RenameDseDiagram"), 0);

    if (rename_control_in_out)
        ui_set_value(to_ui_prop("ControlFlowRename", "RenameControlInOut"), 1);
    else 
        ui_set_value(to_ui_prop("ControlFlowRename", "RenameControlInOut"), 0);
}
 
void
dfe_control_flow_rename(gde_mapsymbol ms)
{
    gde_builtin("PropertySheetShow ControlFlowRename");
    dfe_control_flow_rename_init(ms);
}


void
dfe_control_flow_rename_action_done()
{
    gde_mapsymbol ms = gde_selected_mapsymbol();
    ui_prop p = to_ui_prop("ControlFlowRename", "All");
    
    if (gde_rename_action_done_standard_checks(ms, p, "dfe_control_flow_rename") == False)
	return;

    dfe_control_flow_rename_init(ms);
}


// callback for renaming dse objects

int
flow_rename_data_objects_callback(ui_event_tp ev, ui_prop p, int id, string val)
{
    rename_data_objects = ui_choice_isset(p, 0);
    return 0;
}

// callback for renaming dse diagrams

int
flow_rename_dse_diagram_callback(ui_event_tp ev, ui_prop p, int id, string val)
{
    rename_dse_diagram = ui_choice_isset(p, 0);
    return 0;
}

// callback for renaming control in/out in det, elt, alt, and pat table

int
flow_rename_control_in_out_callback(ui_event_tp e, ui_prop p, int id, string v)
{
    rename_control_in_out = ui_choice_isset(p, 0);
    return 0;
}


//////////////////////////////////////////
// rename func for DFE data stores
//////////////////////////////////////////

void
dfe_store_rename_init(gde_mapsymbol ms)
{
    gor_current_mapsym = ms;

    ui_set_value(to_ui_prop("StoreRename", "NewLabel"),
         gde_node_label(to_gde_node(ms)));

    // Initialize property sheet checkboxes

    if (rename_data_flows_store)
        ui_set_value(to_ui_prop("StoreRename", "RenameDataFlows"), 1);
    else
        ui_set_value(to_ui_prop("StoreRename", "RenameDataFlows"), 0);

    if (rename_data_objects_store)
        ui_set_value(to_ui_prop("StoreRename", "RenameDataObjects"), 1);
    else
        ui_set_value(to_ui_prop("StoreRename", "RenameDataObjects"), 0);

    if (rename_dse_diagram_store)
        ui_set_value(to_ui_prop("StoreRename", "RenameDseDiagram"), 1);
    else
        ui_set_value(to_ui_prop("StoreRename", "RenameDseDiagram"), 0);
}

void
dfe_store_rename(gde_mapsymbol ms)
{
    gde_builtin("PropertySheetShow StoreRename");
    dfe_store_rename_init(ms);
}

void
dfe_store_rename_action_done()
{
    gde_mapsymbol ms = gde_selected_mapsymbol();
    ui_prop p = to_ui_prop("StoreRename", "All");
    
    if (gde_rename_action_done_standard_checks(ms, p, "dfe_store_rename") == False)
	return;

    dfe_store_rename_init(ms);
}

// callback for renaming dfe flows
 
int
store_rename_data_flows_callback(ui_event_tp ev, ui_prop p, int id, string val)
{
    rename_data_flows_store = ui_choice_isset(p, 0);
    return 0;
}

// callback for renaming dse objects

int
store_rename_data_objects_callback(ui_event_tp ev, ui_prop p, int id, string v)
{
    rename_data_objects_store = ui_choice_isset(p, 0);
    return 0;
}
 
// callback for renaming dse diagrams
 
int
store_rename_dse_diagram_callback(ui_event_tp ev, ui_prop p, int id, string val)
{
    rename_dse_diagram_store = ui_choice_isset(p, 0);
    return 0;
}


//////////////////////////////
// rename func for dfe externals
//////////////////////////////////
 
void
se_external_rename_init(gde_mapsymbol ms)
{
    gor_current_mapsym = ms;

    gor_current_label = gde_node_label(to_gde_node(gor_current_mapsym));

    ui_set_value(to_ui_prop("ExternalRename", "NewLabel"), gor_current_label);

    // Initialize property sheet checkboxes

    if (rename_offpage_externals)
        ui_set_value(to_ui_prop("ExternalRename", "RenameOffpageExternals"), 1);
    else
        ui_set_value(to_ui_prop("ExternalRename", "RenameOffpageExternals"), 0);
}
 
void
se_external_rename(gde_mapsymbol ms)
{
    gde_builtin("PropertySheetShow ExternalRename");
    se_external_rename_init(ms);
}

void
se_external_rename_action_done()
{
    gde_mapsymbol ms = gde_selected_mapsymbol();
    ui_prop p = to_ui_prop("ExternalRename", "All");
    
    if (gde_rename_action_done_standard_checks(ms, p, "se_external_rename") == False)
	return;

    se_external_rename_init(ms);
}

// callback for renaming offpage externals

int
external_rename_offpage_callback(ui_event_tp ev, ui_prop p, int id, string val)
{
    rename_offpage_externals = ui_choice_isset(p, 0);
    return 0;
}


//////////////////////////////////
// rename func for dfe processes
//////////////////////////////////
 
void
se_process_rename_init(gde_mapsymbol ms)
{
    gor_current_mapsym = ms;

    gor_current_label = gde_node_label(to_gde_node(gor_current_mapsym));

    ui_set_value(to_ui_prop("ProcessRename", "NewLabel"), gor_current_label);

    // Initialize property sheet checkboxes

    if (rename_offpage_processes)
        ui_set_value(to_ui_prop("ProcessRename", "RenameOffpageProcesses"), 1);
    else
        ui_set_value(to_ui_prop("ProcessRename", "RenameOffpageProcesses"), 0);
}
 
void
se_process_rename(gde_mapsymbol ms)
{
    gde_builtin("PropertySheetShow ProcessRename");
    se_process_rename_init(ms);
}

void
se_process_rename_action_done()
{
    gde_mapsymbol ms = gde_selected_mapsymbol();
    ui_prop p = to_ui_prop("ProcessRename", "All");
    
    if (gde_rename_action_done_standard_checks(ms, p, "se_process_rename") == False)
	return;

    se_process_rename_init(ms);
}

// callback for renaming offpage processes

int
process_rename_offpage_callback(ui_event_tp ev, ui_prop p, int id, string value)
{
    rename_offpage_processes = ui_choice_isset(p, 0);
    return 0;
}


//////////////////////////////////
// rename func for module objects
//////////////////////////////////
 
void
se_module_rename_init(gde_mapsymbol ms)
{
    gor_current_mapsym = ms;
 
    gor_current_label = gde_node_label(to_gde_node(gor_current_mapsym));
 
    ui_set_value(to_ui_prop("ModuleObjectRename", "NewLabel"), gor_current_label);
 
    // Initialize property sheet checkboxes
 
    if (rename_sce_diagram)
        ui_set_value(to_ui_prop("ModuleObjectRename", "RenameSCEDiagram"), 1);
    else
        ui_set_value(to_ui_prop("ModuleObjectRename", "RenameSCEDiagram"), 0);
 
}
 
void
se_module_rename(gde_mapsymbol ms)
{
    gde_builtin("PropertySheetShow ModuleObjectRename");
    se_module_rename_init(ms);
}

void
se_module_rename_action_done()
{
    gde_mapsymbol ms = gde_selected_mapsymbol();
    ui_prop p = to_ui_prop("ModuleObjectRename", "All");
    
    if (gde_rename_action_done_standard_checks(ms, p, "se_module_rename") == False)
	return;

    se_module_rename_init(ms);
}
 
// callback for renaming sce diagrams
 
int
module_rename_diagram_callback(ui_event_tp ev, ui_prop p, int id, string value)
{
    rename_sce_diagram = ui_choice_isset(p, 0);
    return 0;
}

// rename func for control in/out cells in decision table

void
cspec_control_in_out_rename_init(int row, int col)
{
    string gor_current_label =
	gte_get_cell_label(row, col);

    ui_set_value(to_ui_prop("ControlInOutRename", "NewLabel"), gor_current_label);

    if(rename_control_flows)
    	ui_set_value(to_ui_prop("ControlInOutRename", "RenameControlFlows"),1);
    else
    	ui_set_value(to_ui_prop("ControlInOutRename", "RenameControlFlows"),0);

    if (rename_dse_for_control_in_out)
    	ui_set_value(to_ui_prop("ControlInOutRename", "RenameDseforControlInOut"),1);
    else
    	ui_set_value(to_ui_prop("ControlInOutRename", "RenameDseforControlInOut"),0);
}

void
cspec_control_in_out_rename(int hsect, int vsect, int row, int col)
{
    gte_stpem("PropertySheetShow ControlInOutRename");

    cspec_control_in_out_rename_init(gte_get_row(hsect, row),
				     gte_get_col(vsect, col));
}

void
cspec_control_in_out_rename_action_done()
{
    int row = gte_get_current_cell_row();
    int col = gte_get_current_cell_col();
    ui_prop p = to_ui_prop("ControlInOutRename", "All");
    
    if (gte_rename_action_done_standard_checks(row, col, p, "cspec_control_in_out_rename") == False)
	return;
    
    cspec_control_in_out_rename_init(row, col);
}

int 
cspec_control_flow_rename_callback(ui_event_tp event,
                                   ui_prop theprop,
                                   int id,
                                   string value)
{
    rename_control_flows = ui_choice_isset(theprop, 0);
    return 0;
}

int 
cspec_dse_rename_callback(ui_event_tp event,
                          ui_prop theprop,
                          int id,
                          string value)
{
    rename_dse_for_control_in_out = ui_choice_isset(theprop, 0);
    return 0;
}


// Inverse map func for data and control flows
// This is needed for cases where a flow is not labelled.

void
dfe_flow_inverse_map_func(gde_mapsymbol ms, link l, list rename_data,
			  gocha_tp gocha_type)
{
    string label = NULL;
 
    if ( gocha_type != gochaRename )
    {
	print_error( "dfe_flow_inverse_map_func() can only handle renames" );
	return; 
    }

    label = gde_link_label(to_gde_link(ms));

    if (label != NULL && label != "")
	gde_default_inverse_link_map_func(ms, l, rename_data, gocha_type);
}



// inverse map function for files and directories

void
se_file_dir_inverse_map_rename_func(gde_mapsymbol ms, node n,
				    list rename_data_list, gocha_tp gocha_type)
{
    rename_data the_rename_data;
    gde_node gn;
    string oldlabel, ident;
    int i;

    if ( gocha_type != gochaRename )
    {
	print_error( "se_file_dir_inverse_map_rename_func() can only handle renames" );
	return; 
    }

    if (n != NULL && (n.type == "SEFile" || n.type == "SEDirectory"))
    {
	if (!gde_mapsymbol_is_node(ms))
	    return;

	gn = to_gde_node(ms);

	// do the rename
	if (n.type == "SEFile")
	{
	    oldlabel = gde_node_prop(gn, "SEFile");
            if ((str_has_val(oldlabel) && oldlabel != n.name) || (string_length(n.name) < 3))
		gde_node_set_prop(gn, "SEFile", n.name);
	    else if (str_no_val(oldlabel))
	    {
		// go ahead and do it if the rename makes this thing a different name
                // then the gde node
		if (oldlabel != string_extract(n.name, 0, string_length(n.name) - 2))
		    gde_node_set_prop(gn, "SEFile", n.name);
	    }
            ident="file";
	}
	else
	{
	    oldlabel = gde_node_prop(gn, "SEDirectory");
            if (oldlabel != n.name)
		gde_node_set_prop(gn, "SEDirectory", n.name);
            ident="directory";
	}

        if (str_has_val(oldlabel) && oldlabel != n.name)
            gde_print_rename_notification_message(ms,
                gde_mapsymbol_symbol_type_printname(ms) + "'s " + ident +
                " renamed from '" + oldlabel + "' to '" + n.name + "'");
        else
            gde_print_rename_notification_message(ms,
                gde_mapsymbol_symbol_type_printname(ms) + "'s " + ident +
                " renamed to '" + n.name + "'");
    }

    return;
}

// NAME FUNCTIONS for SE.
//
// These are only in this file as it is included everwhere.
//

string
se_basic_node_name_func(gde_mapsymbol ms)

{
    gde_node gn;
    string label;

    gn = to_gde_node(ms);
    if (gn == NULL) 
	return "";

    label = gde_node_label(gn);
    if (str_has_val(label))
	return se_repname(label);
    else
	return NULL;
}


list
se_basic_cntx_name_func(gde_mapsymbol ms)

{
    gde_cntx gc;
    string label;
    list l = list_create("string", 0);

    gc = to_gde_cntx(ms);
    if (gc == NULL) 
	return l;

    label = gde_cntx_label(gc);
    if (str_has_val(label))
	list_append(l, se_repname(label));

    return l;
}



// se_repname: This is the central name function for the SE
// product.  Here is where changes are to be made for special
// customer needs.

int Special42Name = -1;

string
se_repname(string label)
 
{
    string ti;
    int i;

    // leading and trailing space is history no matter what
    label = string_strip(label, "B", " \t\n");

    // Honeywell in Florida
    if (Special42Name == -1)
    {
        ti = toolinfo_variable("IDE_REPLACE_CR_WITH_SP");
	if (str_has_val(ti))
	    Special42Name = 3;
    }

    // ESystems
    // replace <cr> with _ if leading or trailing non underbar
    // replace <cr> with nothing if leading or trailing underbar
    if (Special42Name == -1)
    {    
        ti = toolinfo_variable("SE_USE_UNDERBARS_FOR_SPACE");
	if (str_has_val(ti))
	    Special42Name = 4;
    }

    if (Special42Name == -1)
    {
        ti = toolinfo_variable("IDE_SUPPRESS_CR");
        if (ti == NULL || ti == "")
        {
            ti = toolinfo_variable("IDE_SUPPRESS_SP");
            if (ti == NULL || ti == "")
                Special42Name = 0;
            else
                Special42Name = 2;
        }
        else
            Special42Name = 1;
    }

    // normal policy
    if (Special42Name == 0)
	label = list_to_string(string_to_list(label, " \n\t"), " ");
    else if (Special42Name == 1)
    {
	label = string_search_and_replace(label, "\t", " ");

	// If anybody has the string SPECIAL42NAME in a label, this code will not work.
	label = string_search_and_replace(label, "_\n", "SPECIAL42NAME");
	label = string_search_and_replace(label, "\n_", "SPECIAL42NAME");
	label = list_to_string(string_to_list(label, " \n\t"), " ");
	label = string_search_and_replace(label, "SPECIAL42NAME", "_");
    }
    else if (Special42Name == 3)
    {
	label = list_to_string(string_to_list(label, " \n\t"), " ");
	label = list_to_string(string_to_list(label, "_"), " ");
    }
    else if (Special42Name == 4)
    {
	label = string_search_and_replace(label, "\t", " ");

	// If anybody has the string SPECIAL42NAME in a label, this code will not work.
	label = string_search_and_replace(label, "_\n", "SPECIAL42NAME");
	label = string_search_and_replace(label, "\n_", "SPECIAL42NAME");
	label = list_to_string(string_to_list(label, " \n\t"), "_");
	label = string_search_and_replace(label, "SPECIAL42NAME", "_");
    }
    else // Special42Name is 2
    {
        // just change all spaces and cr's to nothing
        label = string_search_and_replace(label, " ", "");
        label = string_search_and_replace(label, "\n", "");
        label = string_search_and_replace(label, "\t", "");
    }
    return label;
}



// Example functions for annotation cloning.  Important note: these
// functions are not called anywhere.  They are intended to be
// executed via stpem commands as follows.
// stpem -ed dydfe -p <p> -s <s> -C 'EditorQrlEval SE_special_clone();' 
// or 
// stpem -ed dydfe -p <p> -s <s> -C 'EditorQrlEval SE_special_clone_comma_ants();' 
// 
// The second one is required if a user has annotations on comma separated 
// data flows within dfe. Note that which editor you call these with is not
// important, these functions have nothing to do with the editor, just the 
// database.
//
// Finally, if you delete unreferenced objects before calling either of
// these, they will not work!!.  If you require a slight variation of one of
// the functions, just create a new function in a user template visible by 
// one of the SE editors, and call it as above.

void
SE_special_clone()

{
    list no_ant_nodes;
    int i;
    node n, newn;

    print_error("SE_special_clone");

    no_ant_nodes = list_select("node[annot_file_id != 0 && ! node_refs && name $ '*_*' ]");

    for (i = 0; i < list_count(no_ant_nodes); i++)
    {
	n = list_get(no_ant_nodes, i);
	newn = find_by_query("node[" + n.type + "&& annot_file_id == 0 && node_refs && name = '" + se_repname(n.name) + "']");
	if (newn != NULL)
	{
	    print_message("Cloning annotation for " + n.type + " " + n.name);
            gde_clone_node_annots_internal(n, newn);
	}
    }
	
    // These are also available
    //gde_clone_link_annots_internal(NULL, NULL);
    //gde_clone_cntx_annots_internal(NULL, NULL);

    print_error("SE_special_clone complete");
    //gde_builtin("EditorQuit");

    return;
}

void
SE_special_clone_comma_ants()

{
    list comma_links, comma_names;
    int i, j;
    link l, flow;
    string name;

    print_error("Executing SE_special_clone_comma_ants()");

    comma_links = list_select("link[annot_file_id != 0 && DataFlow && ! link_refs && name $ '*,*']");
    for (i = 0; i < list_count(comma_links); i++)
    {
	l = list_get(comma_links, i);
	comma_names = string_to_list(l.name, ",");
	for (j = 0; j < list_count(comma_names); j++)
	{
	    name = se_repname(list_get(comma_names, j));
	    flow = find_by_query("link[DataFlow && link_refs && annot_file_id == 0 && name = '${name}']");
	    if (flow != NULL)
	    {
		print_error("Cloning annotation for " + flow.type + " `" + l.name + "' to `" + flow.name + "'.");
		gde_clone_link_annots_internal(l, flow);
	    }
	}
    }

    print_error("SE_special_clone_comma_ants() complete");

    return;
}



// file translation.  This routine is here simply 
// because this file is included in a lot of places.

 
const string BAD_FILE_CH = " !@#$%^&*()|\\{}[]/?~`:'\"\n\t";
const string GOOD_FILE_CH = string_repeat("_", string_length(BAD_FILE_CH));

string 
make_se_fname(string n)

{
    return string_translate(se_repname(n), BAD_FILE_CH, GOOD_FILE_CH);
}
