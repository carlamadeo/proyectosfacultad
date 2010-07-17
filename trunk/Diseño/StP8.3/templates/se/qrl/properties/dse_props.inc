//////////////////////////////////////////////////////////////////////////////
//
//      StP/SE
//      Confidential property of Interactive Development Environments, Inc.
//      Copyright (c) 1995
//      All rights reserved
//
// dse_props.inc: DSE property sheet qrl hooks
//
//////////////////////////////////////////////////////////////////////////////

#include "qrl/properties/se_common_props.inc" 

// bottom buttons callbacks
boolean ApplyBusy = False;
int
DseObjectDialog_Apply(ui_event_tp event, ui_prop theprop, int id, string value)

{
    ui_prop p;
    string s, nt, notename;
    list symbols;
    gde_node gn;
    int handle, i, val, rc, txn;
    node node;
    gde_symbol sym;
    ui_owner owner;

    if (ui_button_is_ok(theprop))
	rc = 1;
    else
        rc = 0;

    symbols = gde_selected_symbols();
    if (symbols == NULL || (list_count(symbols) == 0))
    {
	print_error("Select one or more symbols to apply properties to.");
	return 0;
    }

    if (list_count(symbols) > 1 && (! DseMultiObjectMode))
    {
        print_error("Select `Apply to Multiple Selection' to apply properties to many objects at once.");
	return 0;
    }

    ApplyBusy = True;
    gde_save_state();
    txn = gde_start_edit_transaction();

    owner = to_ui_owner("ObjectDialog");
    ui_busy(owner, True);
    for (i = 0; i < list_count(symbols); i++)
    {
	sym = list_get(symbols, i);

	if (! gde_symbol_is_node(sym))
	{
	    print_error("Apply: sequences, selections, typedefs and enumerations.");
	    continue;
	}

        gn = to_gde_node(list_get(symbols, i));

	nt = gde_nt_name(gde_node_nt(gn));

	if (nt == NULL || (nt != "Enumeration" && nt != "Sequence" && nt != "Selection" && nt != "Typedef"))
	{
	    print_error("Apply: sequences, selections, typedefs and enumerations.");
	    continue;
	}

        ui_footer_set_value(to_ui_prop("ObjectDialog", "status"), "Setting properties of " + nt + " `" + gde_node_label(gn) + "'.");

	common_dir_file_set(gn);

	node = get_proper_oms_node(gn);
	if (node == NULL)
	{
	    print_error("Could not find oms object for " + gde_nt_name(gde_node_nt(gn)) +
			     " `" + gde_node_label(gn) + "'.");
	    continue;
	}
	handle = send_start_edit_annot(node.id);

	if (nt != "Enumeration")
	{
	    // type annot

	    notename = common_type_annot_set(handle, gn, NULL);

	    // array index
	    p = to_ui_prop("ObjectDialog", "ArraySize");
	    s = ui_text_value(p);
	     
	    editor_send_msg(NGOAE_CLASS, OAE_CMD_NOTE_SEL + notename, handle);
	    editor_send_msg(NGOAE_CLASS, OAE_CMD_ITEM_SEL + "ArraySize", handle);

	    if (str_has_val(s))
		editor_send_msg(NGOAE_CLASS, OAE_CMD_ITEM_SETVAL + stpem_string_val(s), handle);
	    else
		editor_send_msg(NGOAE_CLASS, OAE_CMD_ITEM_DEL, handle);

	}
	if (nt != "Typedef" && nt != "Enumeration")
	{
	    s = ui_text_value(to_ui_prop("ObjectDialog", "StructureTag"));
	    if (str_has_val(s))
	    {
		editor_send_msg(NGOAE_CLASS, OAE_CMD_NOTE_SEL + "DataDefinition", handle);
		editor_send_msg(NGOAE_CLASS, OAE_CMD_ITEM_SEL + "Tag", handle);
		editor_send_msg(NGOAE_CLASS, OAE_CMD_ITEM_SETVAL + stpem_string_val(s), handle);
	    }
	    else
	    {
		editor_send_msg(NGOAE_CLASS, OAE_CMD_NOTE_SEL + "DataDefinition", handle);
		editor_send_msg(NGOAE_CLASS, OAE_CMD_ITEM_SEL + "Tag", handle);
		editor_send_msg(NGOAE_CLASS, OAE_CMD_ITEM_DEL, handle);
	    }

	    // SystemType
	    p = to_ui_prop("ObjectDialog", "SystemType");
	    val = ui_choice_value(p);
	    
	    if (val == 0 ||  val == 1)
	    {
		editor_send_msg(NGOAE_CLASS, OAE_CMD_NOTE_SEL + "DataDefinition", handle);
		editor_send_msg(NGOAE_CLASS, OAE_CMD_ITEM_SEL + "SystemType", handle);

		if (val == 0)
		    editor_send_msg(NGOAE_CLASS, OAE_CMD_ITEM_SETVAL + False, handle);
		else
		    editor_send_msg(NGOAE_CLASS, OAE_CMD_ITEM_SETVAL + True, handle);
	    }
	}
	send_oae_save_update_unload(handle);
    }

    gde_end_edit_transaction(GDE_CMD_INSERT, txn, False);
    ui_busy(owner, False);
    ApplyBusy = False;
 
    return rc;
}


void 
dse_activate_typedef()
{
    if (DseMultiObjectMode)
    {
        dse_activate_all();
        return;
    }
 
    ui_activate(to_ui_prop("ObjectDialog", "DseObjName"), True);
    ui_activate(to_ui_prop("ObjectDialog", "File"), True);
    ui_activate(to_ui_prop("ObjectDialog", "Directory"), True);
    ui_activate(to_ui_prop("ObjectDialog", "Type"), True);
    ui_activate(to_ui_prop("ObjectDialog", "ArraySize"), True);
    ui_activate(to_ui_prop("ObjectDialog", "StructureTag"), False);
    ui_activate(to_ui_prop("ObjectDialog", "SystemType"), False);

    return;
}

void 
dse_activate_enum()
{
    if (DseMultiObjectMode)
    {
        dse_activate_all();
        return;
    }
 
    ui_activate(to_ui_prop("ObjectDialog", "DseObjName"), True);
    ui_activate(to_ui_prop("ObjectDialog", "File"), True);
    ui_activate(to_ui_prop("ObjectDialog", "Directory"), True);
    ui_activate(to_ui_prop("ObjectDialog", "Type"), False);
    ui_activate(to_ui_prop("ObjectDialog", "ArraySize"), False);
    ui_activate(to_ui_prop("ObjectDialog", "StructureTag"), False);
    ui_activate(to_ui_prop("ObjectDialog", "SystemType"), False);

    return;
}


void 
dse_activate_all()
{
    ui_activate(to_ui_prop("ObjectDialog", "DseObjName"), True);
    ui_activate(to_ui_prop("ObjectDialog", "File"), True);
    ui_activate(to_ui_prop("ObjectDialog", "Directory"), True);
    ui_activate(to_ui_prop("ObjectDialog", "Type"), True);
    ui_activate(to_ui_prop("ObjectDialog", "ArraySize"), True);
    ui_activate(to_ui_prop("ObjectDialog", "StructureTag"), True);
    ui_activate(to_ui_prop("ObjectDialog", "SystemType"), True);

    return;
}


void 
dse_activate_none()
{
    LastResetSymbol = NULL;

    if (DseMultiObjectMode)
    {
        dse_activate_all();
        return;
    }
 
    ui_activate(to_ui_prop("ObjectDialog", "DseObjName"), False);
    ui_activate(to_ui_prop("ObjectDialog", "File"), False);
    ui_activate(to_ui_prop("ObjectDialog", "Directory"), False);
    ui_activate(to_ui_prop("ObjectDialog", "Type"), False);
    ui_activate(to_ui_prop("ObjectDialog", "ArraySize"), False);
    ui_activate(to_ui_prop("ObjectDialog", "StructureTag"), False);
    ui_activate(to_ui_prop("ObjectDialog", "SystemType"), False);

    return;
}


gde_symbol LastResetSymbol = NULL;

int
DseObjectDialog_Reset(ui_event_tp event, ui_prop theprop, int id, string value)

{
    // get the current selection and fill up the property sheet.

    ui_prop p;
    string s, nt;
    list symbols;
    gde_node gn;
    gde_node finalize_gd;
    gde_symbol sym;
    int handle;
    node node;
    item item;

    if (ApplyBusy)
	return 0;

    ui_footer_set_value(to_ui_prop("ObjectDialog", "status"), "");
    symbols = gde_selected_symbols();

    // be sure not to print too many messages
    if (DseMultiObjectMode && (list_count(symbols) != 1))
    {
	if (id != -99)
	    print_error("Reset: De-select `Apply to Multiple Selection' and choose one object.");

	dse_activate_none();
	return 0;
    }

    if (symbols == NULL || list_count(symbols) != 1)
    {
	if (id != -99)
            print_error("Reset: Select a single sequence, selection, enumeration or typedef.");
	dse_activate_none();
        return 0;
    }

    sym = list_get(symbols, 0); 
    if (sym == LastResetSymbol && value != "force" && id == -99)
    {
        // EH added for rts 10609, it could have been GOR'ed
        if ((finalize_gd = to_gde_node(sym)) != NULL)
        common_dir_file_reset(finalize_gd);
        return 0;
    }

    if ((! gde_symbol_is_node(sym)) || ((gn = to_gde_node(sym)) == NULL))
    {
	if (id != -99)
            print_error("Reset: Select a single sequence, selection, enumeration or typedef.");
	dse_activate_none();
        return 0;
    }

    nt = gde_nt_name(gde_node_nt(gn));

    if (nt != "Enumeration" && nt != "Sequence" && nt != "Selection" && nt != "Typedef")
    {
	if  (id != -99)
	    print_error("Reset: Select a single sequence, selection, enumeration or typedef.");
	dse_activate_none();
	return 0;
    }

    node = get_proper_oms_node(gn);
    // unmapped gde_node?
    if (node == NULL) 
	return 0;

    LastResetSymbol = sym;
    if (id != -99)
        ui_footer_set_value(to_ui_prop("ObjectDialog", "status"), "Resetting properties from " + nt + " `" + node.name + "'.");

    ui_text_set_value(to_ui_prop("ObjectDialog", "DseObjName"), node.name);

    // Directory

    common_dir_file_reset(gn);

    if (nt != "Enumeration")
    {
	// Type
	common_type_reset(node.id, "DataType", NULL);

	// ArraySize
	item = find_by_query("item[ArraySize && obj_id = ${node.id}]");
	if (item != NULL)
	    ui_text_set_value(to_ui_prop("ObjectDialog", "ArraySize"), item.value);
	else 
	    ui_text_set_value(to_ui_prop("ObjectDialog", "ArraySize"), "");
    }

    if (nt == "Selection" || nt == "Sequence")
    {
	dse_activate_all();
        // SystemType
	item = find_by_query("item[SystemType && obj_id = ${node.id}]");
	if (item != NULL)
	{
	    if (item.value == "True")
	        ui_choice_set_value(to_ui_prop("ObjectDialog", "SystemType"), 1);
            else
		ui_choice_set_value(to_ui_prop("ObjectDialog", "SystemType"), 0); 
	}
        else
	    ui_choice_set_value(to_ui_prop("ObjectDialog", "SystemType"), 0); 

	// StructureTag
	item = find_by_query("item[Tag && obj_id = ${node.id}]");
	if (item != NULL)
	{
	    ui_text_set_value(to_ui_prop("ObjectDialog", "StructureTag"), item.value);
	}
	else 
	    ui_text_set_value(to_ui_prop("ObjectDialog", "StructureTag"), "");

    }
    else
    {
	if (nt == "Enumeration")
	    dse_activate_enum();
	else
	    dse_activate_typedef();
    }

    return 0;
}


boolean DseMultiObjectMode = False;

int
DseSetMultiMode_cb(ui_event_tp event, ui_prop prop, int id, string value)
 
{
    DseMultiObjectMode = True;
    dse_activate_all();
 
    return 0;
}
 
int
DseUnSetMultiMode_cb(ui_event_tp event, ui_prop prop, int id, string value)
 
{
    DseMultiObjectMode = False;
    DseObjectDialog_Reset(NULL, NULL, -99, "force");
 
    return 0;
}


// callbacks for dse semantic checks

int
OkDseCheckSemantics(ui_event_tp event, ui_prop prop, int id, string value)
{
    ApplyDseCheckSemantics(event, prop, id, value);
    return(1);
}

int
ApplyDseCheckSemantics(ui_event_tp event, ui_prop prop, int id, string value)
{
    string	owner;
    int		check_one, check_two, verbose;

    check_one = ui_value(to_ui_prop("DSESemanticChecks", "check_one"));
    check_two = ui_value(to_ui_prop("DSESemanticChecks", "check_two"));
    verbose = ui_value(to_ui_prop("DSESemanticChecks", "verbose"));
    checkDseSemanticsFromDse(verbose, check_one, check_two);
    return(0);
}


