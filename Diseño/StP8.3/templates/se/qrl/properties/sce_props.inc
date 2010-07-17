//////////////////////////////////////////////////////////////////////////////
//
//      StP/SE
//      Confidential property of Interactive Development Environments, Inc.
//      Copyright (c) 1995
//      All rights reserved
//
// sce_props.inc: SCE property sheet qrl hooks
//
//////////////////////////////////////////////////////////////////////////////

#include "qrl/properties/se_common_props.inc" 

int 
set_parameter_type_from_prop(int handle)

{
    ui_prop p;
    string s;

    p = to_ui_prop("ObjectDialog", "Type");
    s = ui_text_value(p);

    editor_send_msg(NGOAE_CLASS, OAE_CMD_NOTE_SEL + "ParameterDefinition", handle);
    editor_send_msg(NGOAE_CLASS, OAE_CMD_ITEM_SEL + "DataType", handle);

    if (str_has_val(s))
	editor_send_msg(NGOAE_CLASS, OAE_CMD_ITEM_SETVAL + stpem_string_val(s), handle);
    else
	editor_send_msg(NGOAE_CLASS, OAE_CMD_ITEM_DEL, handle);

    return 0;
}

int
change_to_included(int handle, string y_or_n)
 
{
    if (y_or_n == "True")
    {
	editor_send_msg(NGOAE_CLASS, OAE_CMD_NOTE_SEL + "LexicalModule", handle);
	//editor_send_msg(NGOAE_CLASS, OAE_CMD_ITEM_SEL + "IsLexicalModule", handle);
	//editor_send_msg(NGOAE_CLASS, OAE_CMD_ITEM_SETVAL + "True", handle);
    }
    else
    {
	editor_send_msg(NGOAE_CLASS, OAE_CMD_NOTE_SEL + "LexicalModule", handle);
	//editor_send_msg(NGOAE_CLASS, "NoteDelete", handle);
    editor_send_msg(NGOAE_CLASS, "NoteRemove " + "LexicalModule", handle);
    } 

    return 0;
}
 
boolean ApplyBusy = False; 

// bottom buttons callbacks

int
SceObjectDialog_Apply(ui_event_tp event, ui_prop theprop, int id, string value)

{
    ui_prop p;
    string s, nt, ant;
    gde_node gn, arc;
    int handle;
    gde_symbol sym;
    gde_cntx gc;
    int i, j, rc, txn, h;
    list symbols, links, arcs;
    ui_owner owner;
    link l;
    node n;
    cntx c;

    // just find one selected thing for now

    if (ui_button_is_ok(theprop))
	rc = 1;
    else
        rc = 0;

    symbols = gde_selected_symbols();

    if (symbols == NULL || list_count(symbols) == 0)
    {
        print_error("Select one or more symbols to apply properties to.");
        return rc;
    }

    if (list_count(symbols) > 1 && (! SceMultiObjectMode))
    {
	print_error("Select `Apply to Multiple Selection' to apply properties to many objects at once.");
        return rc;
    }

    ApplyBusy = True;
    gde_save_state();
    txn = gde_start_edit_transaction();

    owner = to_ui_owner("ObjectDialog");
    ui_busy(owner, True);


    for (i = 0; i < list_count(symbols); i++)
    {
	sym = list_get(symbols, i);

	if (gde_symbol_is_cntx(sym))
	{
	    gc = to_gde_cntx(sym);
            ui_footer_set_value(to_ui_prop("ObjectDialog", "status"), "Setting properties of " + gde_ct_name(gde_cntx_ct(gc)) + " `" + gde_cntx_label(gc) + "'.");
	    c = get_proper_oms_cntx(gc);
	    if (c != NULL)
	    {
                h = send_start_edit_annot(c.id);
		set_parameter_type_from_prop(h);
		set_storage_class(h, "ParameterDefinition", "ParamStorageClass");
                send_oae_save_update_unload(h);
 
	    }
	    else
		print_error("Could not find oms object for parameter " + gde_ct_name(gde_cntx_ct(gc)) +
                             " `" + gde_cntx_label(gc) + "'.");
	    continue;
	}

	if (! gde_symbol_is_node(sym))
	    continue;

	gn = to_gde_node(list_get(symbols, i));
	nt = gde_nt_name(gde_node_nt(gn));

	if (nt != "Module" && nt != "GlobalData")
	{
	    print_error("Cannot set properties of `" + nt + "' symbols.");
	    continue;
	}

	ui_footer_set_value(to_ui_prop("ObjectDialog", "status"), "Setting properties of " + nt + " `" + gde_node_label(gn) + "'.");

	common_dir_file_set(gn);

	// type annot
	n = get_proper_oms_node(gn);

	if (n == NULL)
        {
	    print_error("Could not find oms object for " + gde_nt_name(gde_node_nt(gn)) +
                             " `" + gde_node_label(gn) + "'.");
	    continue;
	}

        h = send_start_edit_annot(n.id);
	common_type_annot_set(h, gn, NULL);

	// the module/lexical include switch    

	if (nt == "Module")
	{
	    p = to_ui_prop("ObjectDialog", "ModuleOrLexical");
	    if (ui_choice_value(p) == 1)
		change_to_included(h, "True");
	    else
		change_to_included(h, "False");

	    set_storage_class(h, "ModuleDefinition", "StorageClass");
	}
	else if (nt == "GlobalData")
	    set_storage_class(h, "GlobalDefinition", "StorageClass");
	

	// ArraySize
	if (nt == "GlobalData")
	{
	    s =  ui_text_value(to_ui_prop("ObjectDialog", "ArraySize"));
	    if (n != NULL) 
	    {
		editor_send_msg(NGOAE_CLASS, OAE_CMD_NOTE_SEL + "GlobalDefinition", h);
		editor_send_msg(NGOAE_CLASS, OAE_CMD_ITEM_SEL + "ArraySize", h);
		if (str_has_val(s))
		    editor_send_msg(NGOAE_CLASS, OAE_CMD_ITEM_SETVAL + stpem_string_val(s), h);
		else
		    editor_send_msg(NGOAE_CLASS, OAE_CMD_ITEM_DEL, h);
	    }
	}

	send_oae_save_update_unload(h);

	// access for globals
	if (nt != "GlobalData")
	    continue;

	p = to_ui_prop("ObjectDialog", "GlobalAccess");
	j = ui_choice_value(p);

	if (j == 0) // read
	    ant = "notspecified";
	else if (j == 1)
	    ant = "read";
	else if (j == 2)
	    ant = "write";
	else
	    ant = "read-write";

	// get the link going into this gde_node
	arcs = gde_node_inarcs_no_comments(gn);
	for (j = 0; j < list_count(arcs); j++)
	{
	    links = gde_arc_links(list_get(arcs, j));
	    l = list_get(gde_link_oms_links(list_get(links, 0)), 0);

	    if (l != NULL)
	    {
		handle = send_start_edit_annot(l.id);
		editor_send_msg(NGOAE_CLASS, OAE_CMD_NOTE_SEL + "GlobalUsage", handle);

		if (ant == "notspecified")
		{
		    editor_send_msg(NGOAE_CLASS, "NoteDelete", handle);
		    send_oae_save_update_unload(handle);
		}
		else
		{
		    editor_send_msg(NGOAE_CLASS, OAE_CMD_ITEM_SEL + "AccessMode", handle);
		    editor_send_msg(NGOAE_CLASS, OAE_CMD_ITEM_SETVAL + stpem_string_val(ant), handle);
		    send_oae_save_update_unload_note(handle);
		}
	    }
	}

    }
    gde_end_edit_transaction(GDE_CMD_INSERT, txn, False);
    ui_busy(owner, False);
    ApplyBusy = False;

    return rc;
}



void 
sce_activate_global()
{
    if (SceMultiObjectMode)
    {
	sce_activate_all();
	return;
    }

    ui_activate(to_ui_prop("ObjectDialog", "SceObjName"), True); 
    ui_activate(to_ui_prop("ObjectDialog", "File"), True); 
    ui_activate(to_ui_prop("ObjectDialog", "Directory"), True); 
    ui_activate(to_ui_prop("ObjectDialog", "Type"), True); 
    ui_activate(to_ui_prop("ObjectDialog", "ArraySize"), True); 
    ui_activate(to_ui_prop("ObjectDialog", "ModuleOrLexical"), False); 
    ui_activate(to_ui_prop("ObjectDialog", "GlobalAccess"), True); 
    ui_activate(to_ui_prop("ObjectDialog", "StorageClass"), True); 

    return;
}


void 
sce_activate_module()
{
    if (SceMultiObjectMode)
    {
	sce_activate_all();
	return;
    }

    ui_activate(to_ui_prop("ObjectDialog", "SceObjName"), True); 
    ui_activate(to_ui_prop("ObjectDialog", "File"), True); 
    ui_activate(to_ui_prop("ObjectDialog", "Directory"), True); 
    ui_activate(to_ui_prop("ObjectDialog", "Type"), True); 
    ui_activate(to_ui_prop("ObjectDialog", "ArraySize"), False); 
    ui_activate(to_ui_prop("ObjectDialog", "ModuleOrLexical"), True); 
    ui_activate(to_ui_prop("ObjectDialog", "GlobalAccess"), False); 
    ui_activate(to_ui_prop("ObjectDialog", "StorageClass"), True); 

    return;
}



void 
sce_activate_all()
{
    ui_activate(to_ui_prop("ObjectDialog", "SceObjName"), True); 
    ui_activate(to_ui_prop("ObjectDialog", "File"), True); 
    ui_activate(to_ui_prop("ObjectDialog", "Directory"), True); 
    ui_activate(to_ui_prop("ObjectDialog", "Type"), True); 
    ui_activate(to_ui_prop("ObjectDialog", "ArraySize"), True); 
    ui_activate(to_ui_prop("ObjectDialog", "ModuleOrLexical"), True); 
    ui_activate(to_ui_prop("ObjectDialog", "GlobalAccess"), True); 
    ui_activate(to_ui_prop("ObjectDialog", "StorageClass"), True); 

    return;
}



void 
sce_activate_params()
{
    if (SceMultiObjectMode)
    {
	sce_activate_all();
	return;
    }

    ui_activate(to_ui_prop("ObjectDialog", "SceObjName"), True); 
    ui_activate(to_ui_prop("ObjectDialog", "File"), False); 
    ui_activate(to_ui_prop("ObjectDialog", "Directory"), False); 
    ui_activate(to_ui_prop("ObjectDialog", "Type"), True); 
    ui_activate(to_ui_prop("ObjectDialog", "ArraySize"), False); 
    ui_activate(to_ui_prop("ObjectDialog", "ModuleOrLexical"), False); 
    ui_activate(to_ui_prop("ObjectDialog", "GlobalAccess"), False); 
    ui_activate(to_ui_prop("ObjectDialog", "StorageClass"), True); 

    return;
}


void 
sce_activate_none()
{
    LastResetSymbol = NULL;

    if (SceMultiObjectMode)
    {
	sce_activate_all();
	return;
    }

    ui_activate(to_ui_prop("ObjectDialog", "SceObjName"), False); 
    ui_activate(to_ui_prop("ObjectDialog", "File"), False); 
    ui_activate(to_ui_prop("ObjectDialog", "Directory"), False); 
    ui_activate(to_ui_prop("ObjectDialog", "Type"), False); 
    ui_activate(to_ui_prop("ObjectDialog", "ArraySize"), False); 
    ui_activate(to_ui_prop("ObjectDialog", "ModuleOrLexical"), False); 
    ui_activate(to_ui_prop("ObjectDialog", "GlobalAccess"), False); 
    ui_activate(to_ui_prop("ObjectDialog", "StorageClass"), False); 

    return;
}

gde_symbol LastResetSymbol = NULL;

int
SceObjectDialog_Reset(ui_event_tp event, ui_prop theprop, int id, string value)

{
    // get the current selection and fill up the property sheet.

    ui_prop p;
    string s, nt;
    list symbols, arcs, links;
    gde_node gn;
    gde_cntx gc;
    int txn, handle;
    node n;
    item it;
    cntx c;
    link l;
    gde_symbol sym;

    if (ApplyBusy)
	return 0;

    ui_footer_set_value(to_ui_prop("ObjectDialog", "status"), "");
    symbols = gde_selected_symbols();

    // be sure not to print too many messages
    if (SceMultiObjectMode && (list_count(symbols) != 1))
    {
	if (id != -99)
	    print_error("Reset: De-select `Apply to Multiple Selection' and choose one object.");

	sce_activate_none();
	return 0;
    }
    
    if (symbols == NULL || list_count(symbols) != 1)
    {
	if (id != -99)
            print_error("Reset: Select a single parameter, module or global.");
	sce_activate_none();
        return 0;
    }

    sym = list_get(symbols, 0);
    if (sym == LastResetSymbol && value != "force" && id == -99)
        return 0;

    if (gde_symbol_is_cntx(sym))
    {
	LastResetSymbol = sym;
	gc = to_gde_cntx(sym);
	c = get_proper_oms_cntx(gc);
        // unmapped gde_cntx?
        if (c == NULL) 
            return 0;

	if (id != -99)
	    ui_footer_set_value(to_ui_prop("ObjectDialog", "status"), "Resetting properties from " + c.type + " `" + c.name + "'.");
	ui_text_set_value(to_ui_prop("ObjectDialog", "SceObjName"), c.name);
	common_type_reset(c.id, "DataType", NULL);
        reset_storage_class(c.id, "ParamStorageClass");

	// grey everything else out
        sce_activate_params();

	return 0;
    }

    if ((! gde_symbol_is_node(sym)) || ((gn = to_gde_node(sym)) == NULL))
    {
	if (id != -99)
            print_error("Reset: Select a single parameter, module or global.");
        sce_activate_none();
        return 0;
    }

    nt = gde_nt_name(gde_node_nt(gn));

    if (nt != "Module" && nt != "GlobalData")
    {
	if (id != -99)
            print_error("Reset: Select a single parameter, module or global.");
        sce_activate_none();
        return 0;
    }

    n = get_proper_oms_node(gn);
    // unmapped gde_node?
    if (n == NULL) 
    {
	sce_activate_none();    
        return 0;
    }

    LastResetSymbol = sym;
    if (id != -99)
        print_message("Resetting properties from " + nt + " `" + n.name + "'.");
    ui_text_set_value(to_ui_prop("ObjectDialog", "SceObjName"), n.name);

    // Directory and File
    common_dir_file_reset(gn);

    // Type
    if (nt == "Module")
    {
	sce_activate_module();
        common_type_reset(n.id, "ModuleReturnType", NULL);
    }
    else
    {
	sce_activate_global();
        common_type_reset(n.id, "DataType", NULL);
    }

    // Lexical Module
    if (nt == "Module" )
    {
	// lexical modules

	it = find_by_query("item[IsLexicalModule && obj_id = ${n.id}]");
	if (it)
	    ui_choice_set_value(to_ui_prop("ObjectDialog", "ModuleOrLexical"), 1);
	else
	    ui_choice_set_value(to_ui_prop("ObjectDialog", "ModuleOrLexical"), 0);
    }
    else
        ui_choice_set_value(to_ui_prop("ObjectDialog", "ModuleOrLexical"), 0);

    // Global Access

    p = to_ui_prop("ObjectDialog", "GlobalAccess");
    if (nt == "GlobalData")
    {
	// get the link going into this gde_node
	arcs = gde_node_inarcs_no_comments(gn);
	if (list_count(arcs) != 0)
	{
	    links = gde_arc_links(list_get(arcs, 0));
	    if (list_count(links) != 0)
	    {
		l = list_get(gde_link_oms_links(list_get(links, 0)), 0); 
		
		if (l != NULL)
		{
		    it = find_by_query("item[AccessMode && obj_id = ${l.id}]");
		    if (it == NULL || str_no_val(it.value))
			ui_choice_set_value(p, 0);
		    else if (it.value == "read-write")
		        ui_choice_set_value(p, 3);
		    else if (it.value == "write")
		        ui_choice_set_value(p, 2);
		    else // read
		        ui_choice_set_value(p, 1);
		}
		else
		    ui_choice_set_value(p, 0);
	    }
	    else
		ui_choice_set_value(p, 0);
	}
	else
	    ui_choice_set_value(p, 0);

	// ArraySize for globals
	it = find_by_query("item[ArraySize && obj_id = ${n.id}]");
	if (it != NULL && str_has_val(it.value))
	    ui_text_set_value(to_ui_prop("ObjectDialog", "ArraySize"), it.value);
	else
	    ui_text_set_value(to_ui_prop("ObjectDialog", "ArraySize"), "");
    }
    else
        ui_choice_set_value(p, 3);

    // StorageClass
    if (nt == "Module" || nt == "GlobalData")
        reset_storage_class(n.id, "StorageClass");

    return 0;
}


void 
set_storage_class(int handle, string notename, string itemname)

{
    string value;
    int i;

    i = ui_choice_value(to_ui_prop("ObjectDialog", "StorageClass"));
    if (i == 0) 
    {
	editor_send_msg(NGOAE_CLASS, OAE_CMD_NOTE_SEL + notename, handle);
	editor_send_msg(NGOAE_CLASS, OAE_CMD_ITEM_SEL + itemname, handle);
	editor_send_msg(NGOAE_CLASS, OAE_CMD_ITEM_DEL, handle);
    }
    else
    {
	if (i == 1)
	    value = "const";
	else if (i == 2) 
	    value = "register";
	else if (i == 3) 
	    value = "register const";
	else if (i == 4) 
	    value = "static";
	else 
	    value = "static const";

	editor_send_msg(NGOAE_CLASS, OAE_CMD_NOTE_SEL + notename, handle);
	editor_send_msg(NGOAE_CLASS, OAE_CMD_ITEM_SEL + itemname, handle);
	editor_send_msg(NGOAE_CLASS, OAE_CMD_ITEM_SETVAL + stpem_string_val(value), handle);
    }

    return;
}


void 
reset_storage_class(int id, string omstype)

{
    item it;

    it = find_by_query("item[${omstype} && obj_id = ${id}]");
    if (it == NULL || str_no_val(it.value))
	ui_choice_set_value(to_ui_prop("ObjectDialog", "StorageClass"), 0);

    else if (it.value == "const")
	ui_choice_set_value(to_ui_prop("ObjectDialog", "StorageClass"), 1);
    else if (it.value == "register")
	ui_choice_set_value(to_ui_prop("ObjectDialog", "StorageClass"), 2);
    else if (it.value == "register const")
	ui_choice_set_value(to_ui_prop("ObjectDialog", "StorageClass"), 3);
    else if (it.value == "static")
	ui_choice_set_value(to_ui_prop("ObjectDialog", "StorageClass"), 4);
    else if (it.value == "static const")
	ui_choice_set_value(to_ui_prop("ObjectDialog", "StorageClass"), 5);
    else
	ui_choice_set_value(to_ui_prop("ObjectDialog", "StorageClass"), 0);

    return;
}


boolean SceMultiObjectMode = False;

int
SceSetMultiMode_cb(ui_event_tp event, ui_prop prop, int id, string value)

{
    SceMultiObjectMode = True;
    sce_activate_all();

    return 0;
}

int
SceUnSetMultiMode_cb(ui_event_tp event, ui_prop prop, int id, string value)

{
    SceMultiObjectMode = False;
    SceObjectDialog_Reset(NULL, NULL, -99, "force");

    return 0;
}



// callbacks for sce semantic checks

int
OkSceCheckSemantics(ui_event_tp event, ui_prop prop, int id, string value)
{
    ApplySceCheckSemantics(event, prop, id, value);
    return(1);
}

int
ApplySceCheckSemantics(ui_event_tp event, ui_prop prop, int id, string value)
{
    string fn;
    sceprop_tp	props;

    props.verbose = ui_value(to_ui_prop("SCESemanticChecks", "verbose"));

    props.check_one = ui_value(to_ui_prop("SCESemanticChecks", "check_one"));
    props.check_two = ui_value(to_ui_prop("SCESemanticChecks", "check_two"));
    props.check_three = ui_value(to_ui_prop("SCESemanticChecks", "check_three"));
    props.check_four = ui_value(to_ui_prop("SCESemanticChecks", "check_four"));
    props.check_five = ui_value(to_ui_prop("SCESemanticChecks", "check_five"));
    props.check_six = ui_value(to_ui_prop("SCESemanticChecks", "check_six"));
    props.check_seven = ui_value(to_ui_prop("SCESemanticChecks", "check_seven"));

    fn = gde_diagram_name();
    if (str_no_val(fn))
    {
        print_error("Save the diagram before running Check Semantics Selectively.");    
        return 0;
    }
 
    props.diaglist = file_list("SceDiagram", fn);

    checkSceSemanticsFromSce(props);

    return(0);

}
