
//      StP/SE
//      Confidential property of Aonix
//      Copyright (c) 1992-1999
//      All rights reserved

// pdl.inc :: functions to handle module PDL's and Process PSpecs
 

const string MODULE = "Module";



int
idehook_view_pdl_active_func()

{
    gde_node gn;
    node n;
    list symbols = gde_selected_symbols();
    
    if (list_count(symbols) != 1)
    	return ACTIVE_FUNC_IS_INACTIVE;

    // Only Modules and Processes selected
    if (list_count(symbols) == 
    	list_count(gde_symbols_find_with_types(symbols, string_to_list("Module", " "))))
    {
	gn = to_gde_node(list_get(symbols, 0));
	n = get_proper_oms_node(gn);
	if (n != NULL)
	{
	    if (find_by_query("note[ModulePDL && obj_id = ${n.id}]") != NULL)
		return ACTIVE_FUNC_IS_ACTIVE;
	}
    }

    return ACTIVE_FUNC_IS_INACTIVE;
}


int
idehook_view_pspec_active_func()

{
    gde_node gn;
    list symbols = gde_selected_symbols();

    if (str_no_val(gde_diagram_name()))
        return ACTIVE_FUNC_IS_INACTIVE;
    
    if (list_count(symbols) != 1)
    	return ACTIVE_FUNC_IS_INACTIVE;

    // Only Modules and Processes selected
    if (list_count(symbols) == 
    	list_count(gde_symbols_find_with_types(symbols, string_to_list("Process", " "))))
    {
	gn = to_gde_node(list_get(symbols, 0));
	if (FindPspecProcess(se_repname(gde_node_label(gn))))
	    return ACTIVE_FUNC_IS_ACTIVE;
    }

    return ACTIVE_FUNC_IS_INACTIVE;
}


// This function is used for Edit PSpec and Edit PDL

int
idehook_edit_pdl_active_func()
{
    list symbols = gde_selected_symbols();
    gde_symbol gs;

    if ( current_editor() != "sce" )
      if (str_no_val(gde_diagram_name()))
        return ACTIVE_FUNC_IS_INACTIVE;
    
    if (list_count(symbols) != 1)
    	return ACTIVE_FUNC_IS_INACTIVE;

    gs = list_get(symbols, 0);
    if (!gde_symbol_is_node(gs))
    	return ACTIVE_FUNC_IS_INACTIVE;

    if (string_length(gde_node_label(to_gde_node(gs))) == 0)
        return ACTIVE_FUNC_IS_INACTIVE;

    // Only Modules and Processes selected
    if (list_count(symbols) == 
    	list_count(gde_symbols_find_with_types(symbols, string_to_list("Module Process", " "))))
    	return ACTIVE_FUNC_IS_ACTIVE;

    return ACTIVE_FUNC_IS_INACTIVE;
}



int
idehook_gen_pdl_active_func()
{
    list symbols = gde_selected_symbols();

    if ( current_editor() != "sce" )
      if (str_no_val(gde_diagram_name()))
        return ACTIVE_FUNC_IS_INACTIVE;
    
    // No symbols selected; gen pdl for all modules which need it
    if (list_count(symbols) == 0)
    	return ACTIVE_FUNC_IS_ACTIVE;

    // Only Modules selected
    if (list_count(symbols) == 
    	list_count(gde_symbols_find_with_types(symbols, string_to_list(MODULE, ","))))
    	return ACTIVE_FUNC_IS_ACTIVE;

    return ACTIVE_FUNC_IS_INACTIVE;
}




// Generate a Pdl for all selected modules which have a Pdl note.
// This function is virtually identical to GenerateSelectedPspecs, any
// changes need to be made there as well.

void
GenerateSelectedPdls()

{
    list modsyms = gde_selected_symbols();
    int i;
    list nodel = list_create("node", 0);
    node n;
    gde_node gn;
    string fname;
    file f;

    fname = gde_diagram_name();

    if (str_no_val(fname))
    {
        print_error("Diagram must have a name before generating PDLs.");
        return;
    }

    f = find_by_query("file[SceDiagram && name == '${fname}']");
    if (f == NULL)
    {
        print_error("File '" + fname + "' not found in repository. Save without syntax errors first.");
        return;
    }

    editor_set_is_busy(True);
    // Get module node objects for symbols
    if (list_count(modsyms) == 0)
    {
        nodel = list_select("node[Module && node_refs[file_id = ${f.id}] && notes[ModulePDL]]");
        if (list_count(nodel) == 0)
        {
            print_error("No Modules in this diagram have PDL notes. No PDLs generated.");
            editor_set_is_busy(False);
            return;
        }
    }
    else
    {
        for (i=0; i<list_count(modsyms); i++)
        {
	    gn = to_gde_node(list_get(modsyms, i));
            n = get_proper_oms_node(gn);
            if (find_by_query("note[ModulePDL && obj_id = ${n.id}]") == NULL)
                print_error("Module '" + n.name + "' does not have a PDL note. Use Edit PDL Note first.");
            else
    	        list_append(nodel, n);
        }
    }

    for (i=0; i<list_count(nodel); i++)
    {
        n = list_get(nodel, i);
        generate_pdl(n, "", "generic", True);
    }
    print_message("Generate PDL(s) complete.");
    editor_set_is_busy(False);
    return;
}




string current_editor_module = "";

//
// Print out a pdl for the given module node.
//
void 
generate_pdl(node module, string output_dir, string pdl_type,
	     boolean called_from_editor)
{
    string outfile, command;
    string new_mod_name;

    if (output_dir == "")
        output_dir = GetSceFileStoreDir();

    new_mod_name = string_search_and_replace(module.name," ", "_");
    outfile = path_compose(output_dir, new_mod_name + ".pdl");

    if (file_exists(outfile) && (! write_file_access(outfile)))
    {
	print_error("Cannot write PDL file '" + outfile + "'.");
	return;
    }
    print_message("Generating PDL for '" + module.name + "' in '" + outfile + "'.");

    command = "qrp qrl/pdl/one_pdl.qrl -o " + outfile + 
	    " -p " + current_projdir() + " -s " + current_system() +
    	    " -x mod_name '" + module.name + "'" + " -x pdl_type " + pdl_type +
    	    " -f pspec.asc";

    system(command);
    return;
}


 

//    Find the PDL annotation associated with the give Module

void
OLDFindOrCreatePdlNote()
{
    gde_node gn;
    node n;
    int handle;
    string nname;
 
    // find the current object, and it's oms node

    gn = to_gde_node(list_get(gde_selected_symbols(), 0));
    if (gde_nt_name(gde_node_nt(gn)) == "Module")
	nname = "NoteSelect ModulePDL";
    else
	nname = "NoteSelect Pspec";
    n = get_proper_oms_node(gn);

    if (n == NULL)
    {
	print_error("Database object for current selection not found.  Nothing to edit.");
	return;
    }

    handle = editor_send_msg(OAE_CLASS, OAE_CMD_ANNOT_EDIT + n.id, CH_FIRST);
    editor_send_msg(OAE_CLASS, nname, handle);
    editor_send_msg(OAE_CLASS, "NoteEditDesc", handle);

    return;
}



int _CurId;
string _NoteName;

void 
PThingEdit(int is_pdl)   // 1 for pdl, 0 for pspec

{
    node pnode;
    note pnote;
    gde_node gn;

    gde_builtin("PropertySheetShow PThing_ps");

    // get the object
    gn = to_gde_node(list_get(gde_selected_symbols(), 0));
    pnode = get_proper_oms_node(gn);
    _CurId = pnode.id;

    if (is_pdl)
        _NoteName = "ModulePDL";
    else
        _NoteName = "Pspec";

    pnote = find_by_query("note[" + _NoteName + " && obj_id = ${pnode.id}]");

    if (pnote != NULL && pnote.desc != NULL && pnote.desc != "")
        ui_text_set_value(to_ui_prop("PThing_ps", "Desc"), pnote.desc);
    else
        ui_text_set_value(to_ui_prop("PThing_ps", "Desc"), "");

    return;   
}




int
PThing_ps_cb(ui_event_tp event, ui_prop prop, int id, string value)

{
    ui_prop p = to_ui_prop("PThing_ps", "Desc"); 
    string str, fname, notename;
    gde_node gn;
    node pnode;
    int handle;
    
    // Begin change ECR 7666
	str = string_search_and_replace(ui_text_value(p), "\"", "\\\"");
    handle = send_start_edit_annot(_CurId);
        
    editor_send_msg("ngoae", "NoteSelect " + _NoteName, handle);
	editor_send_msg("ngoae", "NoteSetDescription \"" + str + "\"", handle);
    send_oae_save_update_unload(handle);
    // End change ECR 7666

    if (ui_button_is_ok(prop))
        return 1;
    else
        return 0;
}


int
PThing_Cancel_cb(ui_event_tp event, ui_prop prop, int id, string value)

{
    return 1;
}


// 0 is for Pspecs, 1 is for Pdls
void
ViewPthings(int is_pdl)

{
    gde_node gn;
    node n;
    string fname, objname, fcontents, msgstr;
    ui_prop viewer;

    gn = to_gde_node(list_get(gde_selected_symbols(), 0));
    n = get_proper_oms_node(gn);

    if (is_pdl)
    {
	msgstr = "PDL";

        objname = string_search_and_replace(n.name," ", "_");
        fname = path_compose(GetSceFileStoreDir(), objname + ".pdl");
    }
    else
    {
        item pindex = get_process_index(n);
	msgstr = "PSpec";
   
        objname = string_search_and_replace(FindDecompositionName(gde_diagram_name(), to_int(pindex.value)), " ", "_");
        fname = path_compose(GetDfeFileStoreDir(), objname + ".pspec");
    }
 
    if (! file_exists(fname))
    {
	print_error("Generated " + msgstr + " for '" +  gde_node_label(gn) + "' not found. ('" + fname + "' does not exist).");
	return;
    }

    if (read_file_access(fname))
    {
        fcontents = read_file(fname);
        ui_prop_sheet_pop_up("SimpleTextViewer");
        ui_text_set_value(to_ui_prop("SimpleTextViewer", "TextFile"), fname);
	viewer = to_ui_prop("SimpleTextViewer", "TheText");
        ui_text_set_value(viewer, fcontents);
        ui_text_set_readonly(viewer, True);
	ui_text_set_readonly(to_ui_prop("SimpleTextViewer", "TextFile"), True);
        ui_text_scroll_to(viewer, 1);
    }
    else
        print_error("Generated " + msgstr + " '" + fname + "' is not readable.");

    return;
} 

