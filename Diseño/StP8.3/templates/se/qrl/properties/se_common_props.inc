//////////////////////////////////////////////////////////////////////////////
//
//      StP/SE
//      Confidential property of Interactive Development Environments, Inc.
//      Copyright (c) 1995
//      All rights reserved
//
// se_common_props.inc: common property sheet qrl hooks
//
//////////////////////////////////////////////////////////////////////////////
 
int SelectorUsage;
// 0 for directories, 1 for files and 2 for types


// ECR 6894 begin
int
SelectorOkButton_cb(ui_event_tp event, ui_prop theprop, int id, string value)
{
    int row;
    string label;
    ui_prop selector = to_ui_prop("ObjectDialogFiller", "Selector");
    ui_prop target;
     
    row = ui_list_selected_row(selector);
    label = ui_list_row_label(selector, row);

    if (SelectorUsage == 0)
        target = to_ui_prop("ObjectDialog", "Directory");
    else if (SelectorUsage == 1)
        target = to_ui_prop("ObjectDialog", "File");
    else
        target = to_ui_prop("ObjectDialog", "Type");
 
    ui_text_set_value(target, label);

    // Close selector in case of double click on item in list
    if (ui_owner_visible(selector))
	    ui_owner_dismiss(to_ui_owner("ObjectDialogFiller"));

    return 0;
}

int
ObjectSelectorRowDeSelected_cb(ui_event_tp event, ui_prop theprop, int id, string value)
{
    ui_activate(to_ui_prop("ObjectDialogFiller", "OK"), False);

    return 0;
}
// ECR 6894 end


int
ObjectSelectorRowSelected_cb(ui_event_tp event, ui_prop theprop, int id, string value)
{
    int row;
    string label;
    ui_prop selector = to_ui_prop("ObjectDialogFiller", "Selector");
    ui_prop target;
 
// ECR 6894 begin
    ui_activate(to_ui_prop("ObjectDialogFiller", "OK"), True);

/* 
    // Code bellow is moved to SelectorOkButton_cb()

    row = ui_list_selected_row(selector);
    label = ui_list_row_label(selector, row);
 
    if (SelectorUsage == 0)
        target = to_ui_prop("ObjectDialog", "Directory");
    else if (SelectorUsage == 1)
        target = to_ui_prop("ObjectDialog", "File");
    else
        target = to_ui_prop("ObjectDialog", "Type");
 
    ui_text_set_value(target, label);
*/
// ECR 6894 end

    return 0;
 
}
 


int
DirectorySelectorFill_cb(ui_event_tp event, ui_prop theprop, int id, string value)
{
    ui_prop selector;
    node dir;
    int i;
    list l, choosel = list_create("string", 0);

    ui_prop_sheet_pop_up("ObjectDialogFiller");
    selector = to_ui_prop("ObjectDialogFiller", "Selector");

    ui_list_clear(selector);
    
    l = list_select("node[SEDirectory]"); 
    for (i = 0; i < list_count(l); i ++)
    {
	dir = list_get(l, i);
	list_append(choosel, dir.name);
    }

    if (list_count(choosel) > 0)
	ui_list_append(selector, choosel);

    SelectorUsage = 0;

// ECR 6894 begin
    ui_activate(to_ui_prop("ObjectDialogFiller", "OK"), False);
// ECR 6894 end

    return 0;
}



int
FileSelectorFill_cb(ui_event_tp event, ui_prop theprop, int id, string value)
{
    ui_prop selector;
    node dir;
    int i;
    string prev_name, name;
    list l, choosel = list_create("string", 0);

    ui_prop_sheet_pop_up("ObjectDialogFiller");
    selector = to_ui_prop("ObjectDialogFiller", "Selector");

    ui_list_clear(selector);
    
    l = list_select("node[SEFile] sort by name"); 

    prev_name = "";
    for (i = 0; i < list_count(l); i ++)
    {
	dir = list_get(l, i);
	name = dir.name;
    
	if (name != prev_name)
	{
	    list_append(choosel, name);
	    prev_name = name;
	}
    }
    if (list_count(choosel) > 0)
	ui_list_append(selector, choosel);

    SelectorUsage = 1;

// ECR 6894 begin
    ui_activate(to_ui_prop("ObjectDialogFiller", "OK"), False);
// ECR 6894 end

    return 0;
}


int
TypeSelectorFill_cb(ui_event_tp event, ui_prop theprop, int id, string value)
{
    ui_prop selector;
    node n;
    item it;
    int i;
    set s;
    list l, choosel = list_create("string", 0);

    ui_prop_sheet_pop_up("ObjectDialogFiller");
    selector = to_ui_prop("ObjectDialogFiller", "Selector");

    ui_list_clear(selector);
    
    l = list_select("node[(Sequence | Selection | Enumeration | Typedef) && out_links]"); 
    s = set_create("string");

    for (i = 0; i < list_count(l); i++)
    {
	n = list_get(l, i);
	set_add(s, n.name);
    }

    l = list_select("item[DataType || Tag]");
    for (i = 0; i < list_count(l); i++)
    {
	it = list_get(l, i);
	set_add(s, it.value);
    }

    for (i = 0; i < set_count(s); i++)
	list_append(choosel, set_get_element(s, i));

    if (list_count(choosel) > 0)
        ui_list_append(selector, choosel);

    SelectorUsage = 2;

// ECR 6894 begin
    ui_activate(to_ui_prop("ObjectDialogFiller", "OK"), False);
// ECR 6894 end

    return 0;
}



// bottom buttons callbacks



void
common_dir_file_set(gde_node gn)

{
    ui_prop p;
    string s;
    int txn;
    string nt = gde_nt_name(gde_node_nt(gn));

 
    if (nt == "Module" || nt == "GlobalData" || list_count(gde_node_inarcs_no_comments(gn)) == 0)
    {
        gde_save_state();
        txn = gde_start_edit_transaction();
        p = to_ui_prop("ObjectDialog", "Directory");
        s = ui_text_value(p);
        if (s != NULL)
             gde_node_set_prop(gn, "SEDirectory", s);
 
        p = to_ui_prop("ObjectDialog", "File");
        s = ui_text_value(p);
        if (s != NULL && s != se_default_file_value(gn))
            gde_node_set_prop(gn, "SEFile", s);
        gde_end_edit_transaction(GDE_CMD_INSERT, txn, False);
    }
    else 
    {
	// just in case, clear the file and directory properties.
	gde_node_clear_prop(gn, "SEFile");
	gde_node_clear_prop(gn, "SEDirectory");
    }

    return;
}



// for nodes only. cntx routine is in sce_props.inc
string
common_type_annot_set(int handle, gde_node gn, string pname)

{
    ui_prop p;
    string s, nt, notename, itemname;

    if (pname == NULL)
	pname = "ObjectDialog";

    nt = gde_nt_name(gde_node_nt(gn));
    p = to_ui_prop(pname, "Type");
    s = ui_text_value(p);

    if (nt == "Module")
    {
	notename = "ModuleDefinition";
	itemname = "ModuleReturnType";
    }
    else if (nt == "GlobalData")
    {
	notename = "GlobalDefinition";
	itemname = "DataType";
    }
    else if (nt == "Typedef")
    {
	notename = "TypedefDefinition";
	itemname = "DataType";
    }
    else 
    {
	notename = "DataDefinition";
	itemname = "DataType";
    }

    if (str_has_val(s))
    {
	editor_send_msg(NGOAE_CLASS, OAE_CMD_NOTE_SEL + notename, handle);
	editor_send_msg(NGOAE_CLASS, OAE_CMD_ITEM_SEL + itemname, handle);
	editor_send_msg(NGOAE_CLASS, OAE_CMD_ITEM_SETVAL + stpem_string_val(s), handle);
    }
    else
    {
	editor_send_msg(NGOAE_CLASS, OAE_CMD_NOTE_SEL + notename, handle);
	editor_send_msg(NGOAE_CLASS, OAE_CMD_ITEM_SEL + itemname, handle);
	editor_send_msg(NGOAE_CLASS, OAE_CMD_ITEM_DEL, handle);
    }

    return notename;
}



void 
common_dir_file_reset(gde_node gn)

{
    string s;

    s = gde_node_prop(gn, "SEDirectory");
    if (s != NULL)
        ui_text_set_value(to_ui_prop("ObjectDialog", "Directory"), s);
    else
        ui_text_set_value(to_ui_prop("ObjectDialog", "Directory"), "");

    s = gde_node_prop(gn, "SEFile");
    if (s != NULL)
        ui_text_set_value(to_ui_prop("ObjectDialog", "File"), s);
    else
        ui_text_set_value(to_ui_prop("ObjectDialog", "File"), se_default_file_value(gn));

    return;
}



// works for contexts and nodes

void
common_type_reset(int id, string itemname, string pname)

{
    item it;

    if (pname == NULL)
        pname = "ObjectDialog";

    it = find_by_query("item[${itemname} && obj_id = ${id}]");
    if (it != NULL && str_has_val(it.value))
	ui_text_set_value(to_ui_prop(pname, "Type"), it.value);
    else  
	ui_text_set_value(to_ui_prop(pname, "Type"), "");

    return;
}



int
SEDialogFiller_Cancel(ui_event_tp event, ui_prop theprop, int id, string value)

{
    // remove the fill window
    ui_prop p = to_ui_prop("ObjectDialogFiller", "Selector");

    if (ui_owner_visible(p))
	ui_owner_dismiss(to_ui_owner("ObjectDialogFiller"));

    return 1;
}


