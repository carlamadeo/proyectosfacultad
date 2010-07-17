//////////////////////////////////////////////////////////////////////////////
//
//      StP/SE
//      Confidential property of Interactive Development Environments, Inc.
//      Copyright (c) 1995
//      All rights reserved
//
// t_props.inc: DSE property sheet qrl hooks for StP/T 
//
//////////////////////////////////////////////////////////////////////////////

#include "qrl/properties/se_common_props.inc" 

// bottom buttons callbacks

boolean TApplyBusy = False;
int TCurId;


int
TObjectDialog_Apply(ui_event_tp event, ui_prop theprop, int id, string value)

{
    ui_prop p;
    string nt, notename;
    list symbols;
    gde_node gn;
    int handle, i, val, rc, tp, txn;
    node node;
    gde_symbol sym;
    ui_owner owner;
    boolean err = False;

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

    if (list_count(symbols) > 1 && (! TMultiObjectMode))
    {
        print_error("Select `Apply to Multiple Selection' to apply properties to many objects at once.");
	return 0;
    }

    ApplyBusy = True;
    gde_save_state();
    txn = gde_start_edit_transaction();
 
    owner = to_ui_owner("TObjectDialog");
    ui_busy(owner, True);
    for (i = 0; i < list_count(symbols); i++)
    {
	sym = list_get(symbols, i);

	if (! gde_symbol_is_node(sym))
	{
	    if (! err)
	        print_error("T Properties are for child Sequences and Selections only.");
            err = True;
	    continue;
	}

        gn = to_gde_node(list_get(symbols, i));
        node = get_proper_oms_node(gn);
        // unmapped gde_node?
        if (node == NULL) 
	    continue;

	nt = gde_nt_name(gde_node_nt(gn));

	if (nt == NULL || (nt != "Sequence" && nt != "Selection" ))
	{
	    if (! err)
	        print_error("T Properties are for child Sequences and Selections only.");
            err = True;
	    continue;
	}

	if (list_count(gde_node_outarcs_no_comments(gn)) != 0)
	{
	    if (! err)
	        print_error("T Properties are for child Sequences and Selections only.");
            err = True;
	    continue;
	}

	// start editing annotation
	handle = send_start_edit_annot(node.id);
	editor_send_msg(NGOAE_CLASS, OAE_CMD_NOTE_SEL + "DataDefinition", handle);

        ui_footer_set_value(to_ui_prop("TObjectDialog", "status"), "Setting properties of " + nt + " `" + gde_node_label(gn) + "'.");

	p = to_ui_prop("TObjectDialog", "Type");
	tp = ui_choice_value(p);
	if (tp == 1 || tp == 2)
	{
	    set_int_real_vals(tp, handle);
	}
	else if (tp == 5)
        {
	    set_boolean_vals(handle);
        }
	else if (tp == 4)
	{
	    editor_send_msg(NGOAE_CLASS, OAE_CMD_ITEM_SEL + "DataType", handle);
	    editor_send_msg(NGOAE_CLASS, OAE_CMD_ITEM_SETVAL + "string", handle);
	}

	set_common_vals(node.id, handle);

	// save annot
	send_oae_save_update_unload_note(handle);
    }
    
    gde_end_edit_transaction(GDE_CMD_INSERT, txn, False);
    ui_busy(owner, False);
    ApplyBusy = False;

    return rc;
}


void 
t_activate_int_real()
{
    if (TMultiObjectMode)
    {
	t_activate_all();
	return;
    }

    ui_activate(to_ui_prop("TObjectDialog", "TObjName"), True);
    ui_activate(to_ui_prop("TObjectDialog", "MinimumValue"), True);
    ui_activate(to_ui_prop("TObjectDialog", "MaximumValue"), True);
    ui_activate(to_ui_prop("TObjectDialog", "Resolution"), True);
    ui_activate(to_ui_prop("TObjectDialog", "StringValues"), True);
    ui_activate(to_ui_prop("TObjectDialog", "TrueValue"), False);
    ui_activate(to_ui_prop("TObjectDialog", "FalseValue"), False);
    ui_activate(to_ui_prop("TObjectDialog", "UnknownValue"), False);

    return;
}



void 
t_activate_string()
{
    if (TMultiObjectMode)
    {
	t_activate_all();
	return;
    }

    ui_activate(to_ui_prop("TObjectDialog", "TObjName"), True);
    ui_activate(to_ui_prop("TObjectDialog", "MinimumValue"), False);
    ui_activate(to_ui_prop("TObjectDialog", "MaximumValue"), False);
    ui_activate(to_ui_prop("TObjectDialog", "Resolution"), False);
    ui_activate(to_ui_prop("TObjectDialog", "StringValues"), True);
    ui_activate(to_ui_prop("TObjectDialog", "TrueValue"), False);
    ui_activate(to_ui_prop("TObjectDialog", "FalseValue"), False);
    ui_activate(to_ui_prop("TObjectDialog", "UnknownValue"), False);

    return;
}


void 
t_activate_boolean()
{
    if (TMultiObjectMode)
    {
	t_activate_all();
	return;
    }

    ui_activate(to_ui_prop("TObjectDialog", "TObjName"), True);
    ui_activate(to_ui_prop("TObjectDialog", "MinimumValue"), False);
    ui_activate(to_ui_prop("TObjectDialog", "MaximumValue"), False);
    ui_activate(to_ui_prop("TObjectDialog", "Resolution"), False);
    ui_activate(to_ui_prop("TObjectDialog", "StringValues"), True);
    ui_activate(to_ui_prop("TObjectDialog", "TrueValue"), True);
    ui_activate(to_ui_prop("TObjectDialog", "FalseValue"), True);
    ui_activate(to_ui_prop("TObjectDialog", "UnknownValue"), True);

    return;
}


void 
t_activate_none()
{
    if (TMultiObjectMode)
    {
	t_activate_all();
	return;
    }

    ui_activate(to_ui_prop("TObjectDialog", "TObjName"), True);
    ui_activate(to_ui_prop("TObjectDialog", "MinimumValue"), False);
    ui_activate(to_ui_prop("TObjectDialog", "MaximumValue"), False);
    ui_activate(to_ui_prop("TObjectDialog", "Resolution"), False);
    ui_activate(to_ui_prop("TObjectDialog", "StringValues"), True);
    ui_activate(to_ui_prop("TObjectDialog", "TrueValue"), False);
    ui_activate(to_ui_prop("TObjectDialog", "FalseValue"), False);
    ui_activate(to_ui_prop("TObjectDialog", "UnknownValue"), False);

    return;
}

void 
t_activate_all_none()
{
    TLastResetSymbol = NULL;
    if (TMultiObjectMode)
    {
	t_activate_all();
	return;
    }

    t_activate_none();

    ui_activate(to_ui_prop("TObjectDialog", "TObjName"), False);
    ui_activate(to_ui_prop("TObjectDialog", "InvalidSubDomain"), False);
    ui_activate(to_ui_prop("TObjectDialog", "Units"), False);
    ui_activate(to_ui_prop("TObjectDialog", "Format"), False);
    ui_activate(to_ui_prop("TObjectDialog", "Type"), False);

    return;
}

void 
t_activate_non_type()
{
    if (TMultiObjectMode)
    {
	t_activate_all();
	return;
    }

    t_activate_none();

    ui_activate(to_ui_prop("TObjectDialog", "InvalidSubDomain"), True);
    ui_activate(to_ui_prop("TObjectDialog", "Units"), True);
    ui_activate(to_ui_prop("TObjectDialog", "Format"), True);
    ui_activate(to_ui_prop("TObjectDialog", "Type"), True);

    return;
}

void 
t_activate_all()
{
    ui_activate(to_ui_prop("TObjectDialog", "MinimumValue"), True);
    ui_activate(to_ui_prop("TObjectDialog", "MaximumValue"), True);
    ui_activate(to_ui_prop("TObjectDialog", "Resolution"), True);
    ui_activate(to_ui_prop("TObjectDialog", "StringValues"), True);
    ui_activate(to_ui_prop("TObjectDialog", "TrueValue"), True);
    ui_activate(to_ui_prop("TObjectDialog", "FalseValue"), True);
    ui_activate(to_ui_prop("TObjectDialog", "UnknownValue"), True);
    ui_activate(to_ui_prop("TObjectDialog", "InvalidSubDomain"), True);
    ui_activate(to_ui_prop("TObjectDialog", "Units"), True);
    ui_activate(to_ui_prop("TObjectDialog", "Format"), True);
    ui_activate(to_ui_prop("TObjectDialog", "Type"), True);

    return;
}

int
TSetActivation_cb(ui_event_tp event, ui_prop theprop, int id, string value)

{
    ui_prop p = to_ui_prop("TObjectDialog", "Type");
    int i;

    i = ui_choice_value(p);

    if (i == 1 || i == 2)
    {
        t_activate_int_real();
	reset_int_real_vals(TCurId);
    }
    else if (i == 5)
    {
        t_activate_boolean();
	reset_boolean_vals(TCurId);
    }
    else if (i == 4)
    {
        t_activate_string();
    }
    else
        t_activate_none();

    reset_string_vals(TCurId);

    return 0;
}



void
set_int_real_vals(int tp, int h)

{
    ui_prop p;
    string s;

    editor_send_msg(NGOAE_CLASS, OAE_CMD_ITEM_SEL + "DataType", h);
    if (tp == 1)
	editor_send_msg(NGOAE_CLASS, OAE_CMD_ITEM_SETVAL + "integer", h);
    else
	editor_send_msg(NGOAE_CLASS, OAE_CMD_ITEM_SETVAL + "real", h);

    p = to_ui_prop("TObjectDialog", "MinimumValue");
    s = ui_text_value(p);
    if (str_has_val(s))
    {
	editor_send_msg(NGOAE_CLASS, OAE_CMD_ITEM_SEL + "TMinimumValue", h);
	editor_send_msg(NGOAE_CLASS, OAE_CMD_ITEM_SETVAL + stpem_string_val(s), h);
    }

    p = to_ui_prop("TObjectDialog", "MaximumValue");
    s = ui_text_value(p);
    if (str_has_val(s))
    {
	editor_send_msg(NGOAE_CLASS, OAE_CMD_ITEM_SEL + "TMaximumValue", h);
	editor_send_msg(NGOAE_CLASS, OAE_CMD_ITEM_SETVAL + stpem_string_val(s), h);
    }

    p = to_ui_prop("TObjectDialog", "Resolution");
    s = ui_text_value(p);
    if (str_has_val(s))
    {
	editor_send_msg(NGOAE_CLASS, OAE_CMD_ITEM_SEL + "TResolution", h);
	editor_send_msg(NGOAE_CLASS, OAE_CMD_ITEM_SETVAL + stpem_string_val(s), h);
    }

    return;
}




void
reset_int_real_vals(int id)

{
    ui_prop p;
    item it;

    p = to_ui_prop("TObjectDialog", "MinimumValue");
    it = find_by_query("item[TMinimumValue && obj_id = ${id}]");
    if (it != NULL && str_has_val(it.value))
	ui_text_set_value(p, it.value);
    else
	ui_text_set_value(p, "");
    
    p = to_ui_prop("TObjectDialog", "MaximumValue");
    it = find_by_query("item[TMaximumValue && obj_id = ${id}]");
    if (it != NULL && str_has_val(it.value))
	ui_text_set_value(p, it.value);
    else
	ui_text_set_value(p, "");

    p = to_ui_prop("TObjectDialog", "Resolution");
    it = find_by_query("item[TResolution && obj_id = ${id}]");
    if (it != NULL && str_has_val(it.value))
	ui_text_set_value(p, it.value);
    else
	ui_text_set_value(p, "");

    return;
}


void
set_boolean_vals(int h)

{
    ui_prop p;
    string s;
    
    editor_send_msg(NGOAE_CLASS, OAE_CMD_ITEM_SEL + "DataType", h);
    editor_send_msg(NGOAE_CLASS, OAE_CMD_ITEM_SETVAL + "boolean", h);

    p = to_ui_prop("TObjectDialog", "TrueValue");
    s = ui_text_value(p);
    if (str_has_val(s))
    {
	editor_send_msg(NGOAE_CLASS, OAE_CMD_ITEM_SEL + "TTrueValue", h);
	editor_send_msg(NGOAE_CLASS, OAE_CMD_ITEM_SETVAL + stpem_string_val(s), h);
    }

    p = to_ui_prop("TObjectDialog", "FalseValue");
    s = ui_text_value(p);
    if (str_has_val(s))
    {
	editor_send_msg(NGOAE_CLASS, OAE_CMD_ITEM_SEL + "TFalseValue", h);
	editor_send_msg(NGOAE_CLASS, OAE_CMD_ITEM_SETVAL + stpem_string_val(s), h);
    }

    p = to_ui_prop("TObjectDialog", "UnknownValue");
    s = ui_text_value(p);
    if (str_has_val(s))
    {
	editor_send_msg(NGOAE_CLASS, OAE_CMD_ITEM_SEL + "TUnknownValue", h);
	editor_send_msg(NGOAE_CLASS, OAE_CMD_ITEM_SETVAL + stpem_string_val(s), h);
    }

    return;
}



void
reset_boolean_vals(int id)

{
    ui_prop p;
    item it;
    
    p = to_ui_prop("TObjectDialog", "TrueValue");
    it = find_by_query("item[TTrueValue && obj_id = ${id}]");
    if (it != NULL && str_has_val(it.value))
	ui_text_set_value(p, it.value);
    else
	ui_text_set_value(p, "");
    
    p = to_ui_prop("TObjectDialog", "FalseValue");
    it = find_by_query("item[TFalseValue && obj_id = ${id}]");
    if (it != NULL && str_has_val(it.value))
	ui_text_set_value(p, it.value);
    else
	ui_text_set_value(p, "");

    p = to_ui_prop("TObjectDialog", "UnknownValue");
    it = find_by_query("item[TUnknownValue && obj_id = ${id}]");
    if (it != NULL && str_has_val(it.value))
	ui_text_set_value(p, it.value);
    else
	ui_text_set_value(p, "");

    return;
}



void
set_common_vals(int id, int h)

{
    ui_prop p;
    item it;
    list l;
    int i, val, handle;
    string s;

    set_string_vals(id, h);

    p = to_ui_prop("TObjectDialog", "InvalidSubDomain");
    val = ui_choice_value(p);
    if (val == 0)
	s = "Abnormal";
    else if (val == 1)
	s = "OutOfBounds";
    else if (val == 2)
	s = "BelowBounds";
    else if (val == 3)
	s = "AboveBounds";
    else if (val == 4)
	s = "OutOfType";
    else if (val == 5)
	s = "NotInList";
    else if (val == 6)
	s = "None";
    else 
	s = "Abnormal";

    editor_send_msg(NGOAE_CLASS, OAE_CMD_ITEM_SEL + "TInvalidSubdomain", h);
    editor_send_msg(NGOAE_CLASS, OAE_CMD_ITEM_SETVAL + s, h);

    // units
    p = to_ui_prop("TObjectDialog", "Units");
    s = ui_text_value(p);
    if (str_has_val(s))
    {
	editor_send_msg(NGOAE_CLASS, OAE_CMD_ITEM_SEL + "TTestUnits", h);
	editor_send_msg(NGOAE_CLASS, OAE_CMD_ITEM_SETVAL + s, h);
    }

    // format
    p = to_ui_prop("TObjectDialog", "Format");
    s = ui_text_value(p);
    if (str_has_val(s))
    {
	editor_send_msg(NGOAE_CLASS, OAE_CMD_ITEM_SEL + "TFormat", h);
	editor_send_msg(NGOAE_CLASS, OAE_CMD_ITEM_SETVAL + s, h);
    }
    return;
}




void
set_string_vals(int id, int h)

{
    ui_prop p;
    item it;
    list l, iteml;
    int i;
    string s, allowedv;
    
    p = to_ui_prop("TObjectDialog", "StringValues");
    s = ui_text_value(p);


    // first query for and delete all the current Allowed Values if they are not in the list, then 
    // reset values.

    if (str_has_val(s))
	l = string_to_list(s, "\n");
    else
	l = list_create("string", 0);

    iteml = list_select("item[AllowedValue && obj_id = ${id}]");

    for (i = 0; i < list_count(iteml); i++)
    {
	it = list_get(iteml, i);

	if (list_find(l, 0, it.value) == list_count(l))
	{
	    // delete item
	    editor_send_msg(NGOAE_CLASS, OAE_CMD_ITEM_SEL + "\"AllowedValue:" + it.value + "\"", h);
	    editor_send_msg(NGOAE_CLASS, OAE_CMD_ITEM_DEL, h);
	}
    }
    
    if (str_has_val(s))
    {
	for (i = 0; i < list_count(l); i++)
	{
	    allowedv = list_get(l, i);
            if (str_has_val(allowedv))
	    {
                editor_send_msg(NGOAE_CLASS, OAE_CMD_ITEM_SEL + "\"AllowedValue:" + allowedv + "\"", h);
                //editor_send_msg(NGOAE_CLASS, OAE_CMD_ITEM_SETVAL + "\"" + allowedv + "\"", h);
	    }
	}
    }

    //ui_text_set_value(p, s);
    return;
}



void
t_blank_string_vals()

{
    ui_prop p;
    
    p = to_ui_prop("TObjectDialog", "StringValues");
    ui_text_set_value(p, "");

    return;
}

void
reset_string_vals(int id)

{
    ui_prop p;
    item it;
    list l;
    int i;
    string s = "";
    
    p = to_ui_prop("TObjectDialog", "StringValues");
    l = list_select("item[AllowedValue && obj_id = ${id}]");

    for (i = 0; i < list_count(l); i++)
    {
	it = list_get(l, i);
        if (it != NULL && str_has_val(it.value))
	    s += it.value + "\n";
    }
    ui_text_set_value(p, s);

    return;
}




gde_symbol TLastResetSymbol = NULL;

int
TObjectDialog_Reset(ui_event_tp event, ui_prop theprop, int id, string value)

{
    // get the current selection and fill up the property sheet.

    ui_prop p;
    string nt;
    list symbols;
    gde_node gn;
    gde_symbol sym;
    int i, handle;
    node node;
    item it;

    if (ApplyBusy)
	return 0;

    ui_footer_set_value(to_ui_prop("TObjectDialog", "status"), "");
    symbols = gde_selected_symbols();

    // be sure not to print too many messages
    if (DseMultiObjectMode && (list_count(symbols) != 1))
    {
	if (id != -99)
	    print_error("Reset: De-select `Apply to Multiple Selection' and choose one object.");

	dse_activate_all();
	return 0;
    }

    if (symbols == NULL || list_count(symbols) != 1)
    {
	if (id != -99)
            print_error("Reset: Select a single child sequence or selection.");
	t_activate_all_none();
	t_blank_string_vals();
        return 0;
    }

    sym = list_get(symbols, 0); 
    if (sym == TLastResetSymbol && value != "force" && id == -99)
        return 0;

    if ((! gde_symbol_is_node(sym)) || ((gn = to_gde_node(sym)) == NULL))
    {
	if (id != -99)
            print_error("Reset: Select a single child sequence or selection.");
	t_activate_all_none();
	t_blank_string_vals();
        return 0;
    }

    nt = gde_nt_name(gde_node_nt(gn));

    if (nt != "Sequence" && nt != "Selection")
    {
	if  (id != -99)
	    print_error("Reset: Select a single child sequence or selection.");
	t_activate_all_none();
	t_blank_string_vals();
	return 0;
    }

    // kids only
    if (list_count(gde_node_outarcs_no_comments(gn)) != 0)
    {
	if  (id != -99)
	    print_error("Reset: T Properties are for child Sequences and Selections only.");
	t_activate_all_none();
	t_blank_string_vals();
	return 0;
    }

    node = get_proper_oms_node(gn);
    // unmapped gde_node?
    if (node == NULL) 
	return 0;

    TCurId = node.id;
    t_activate_non_type();

    TLastResetSymbol = sym;
    if (id != -99)
        ui_footer_set_value(to_ui_prop("TObjectDialog", "status"), "Resetting StP/T properties from " + nt + " `" + node.name + "'.");

    ui_text_set_value(to_ui_prop("TObjectDialog", "TObjName"), node.name);

    p = to_ui_prop("TObjectDialog", "Type");
    it = find_by_query("item[DataType && obj_id = ${node.id}]");
    if (it != NULL && str_has_val(it.value))
    {
	if (it.value == "integer" || it.value == "int" || it.value == "Integer")
	{
	    ui_choice_set_value(p, 1);
	    t_activate_int_real();
	    reset_int_real_vals(node.id);
	}
	else if (it.value == "real" || it.value == "Real" || it.value == "float" || it.value == "double")
	{
	    ui_choice_set_value(p, 2);
	    t_activate_int_real();
	    reset_int_real_vals(node.id);
	}
	else if (it.value == "character" || it.value == "Character")
	{
	    ui_choice_set_value(p, 3);
	    t_activate_none();
	}
	else if (it.value == "string" || it.value == "String")
	{
	    ui_choice_set_value(p, 4);
	    t_activate_string();
	}
	else if (it.value == "boolean" || it.value == "Boolean")
	{
	    ui_choice_set_value(p, 5);
	    t_activate_boolean();
	    reset_boolean_vals(node.id);
	}
	else
	{
	    ui_choice_set_value(p, 0);
	    t_activate_none();
	}

    }
    else
	ui_choice_set_value(p, 0);

    // reset common properties

    reset_string_vals(node.id);
    p = to_ui_prop("TObjectDialog", "InvalidSubDomain");
    it = find_by_query("item[TInvalidSubdomain && obj_id = ${node.id}]");
    if (it != NULL && str_has_val(it.value))
    {
	if (it.value == "Abnormal")
	    ui_choice_set_value(p, 0);
	else if (it.value == "OutOfBounds")
	    ui_choice_set_value(p, 1);
	else if (it.value == "BelowBounds")
	    ui_choice_set_value(p, 2);
	else if (it.value == "AboveBounds")
	    ui_choice_set_value(p, 3);
	else if (it.value == "OutOfType")
	    ui_choice_set_value(p, 4);
	else if (it.value == "NotInList")
	    ui_choice_set_value(p, 5);
	else if (it.value == "None")
	    ui_choice_set_value(p, 6);
	else 
	    ui_choice_set_value(p, 0);
    }
    else 
	ui_choice_set_value(p, 0);

    p = to_ui_prop("TObjectDialog", "Units");
    it = find_by_query("item[TTestUnits && obj_id = ${node.id}]");
    if (it != NULL && str_has_val(it.value))
	ui_text_set_value(p, it.value);
    else
	ui_text_set_value(p, "");

    p = to_ui_prop("TObjectDialog", "Format");
    it = find_by_query("item[TFormat && obj_id = ${node.id}]");
    if (it != NULL && str_has_val(it.value))
	ui_text_set_value(p, it.value);
    else
	ui_text_set_value(p, "");

    return 0;
}


boolean TMultiObjectMode = False;

int
TSetMultiMode_cb(ui_event_tp event, ui_prop prop, int id, string value)
 
{
    TMultiObjectMode = True;
    t_activate_all();
 
    return 0;
}
 
int
TUnSetMultiMode_cb(ui_event_tp event, ui_prop prop, int id, string value)
 
{
    TMultiObjectMode = False;
    TObjectDialog_Reset(NULL, NULL, -99, "force");
 
    return 0;
}
 

