////////////////////////////////////////////////////////////////////////////////
//
// 	sccsid[] = %W% %Y% %D%
// 	StP/Core
// 	Confidential property of Interactive Development Environments, Inc.
// 	Copyright (c) 1992-1995
// 	All rights reserved
//
////////////////////////////////////////////////////////////////////////////////

int 
StpdtRenameCb(ui_event_tp event, ui_prop theprop, int id, string value)
{
    return stp_rename(event, theprop, id, value, False);
}

int
StpdtCheckImpactCb(ui_event_tp event, ui_prop theprop, int id, string value)
{
    return stp_rename(event, theprop, id, value, True);
}

list SEFileRenames;
string CurNewName;
ui_prop ParentProp;

int 
stp_rename(ui_event_tp e, ui_prop p, int id, string value, boolean check)
{
    ui_prop  prop;
    ui_owner owner;
    string str, newName, oldName;
    node n, n1;
    int rc;

    ParentProp = p;
    // dismiss any errant SEFile chooser that could screw this up
    prop = to_ui_prop("SEFileChooser", "Chooser");
    ui_list_clear(prop);

    if (ui_button_is_ok(p))
        rc = 1;
    else
        rc = 0;

    if (check)
    {
	print_error("Not implemented yet");
	return rc;
    } 
 
    n = find_by_query("node[" + string_substitute("${cmd.ids}") + "]");
    if (n == NULL)
    {
	print_error("Could not find selected Repository Object.");
	return rc;
    }

    owner = to_ui_owner(p);

    // Calling to_ui_prop with an owner name is what does not work, 
    // likely because of StPFindOwner.

    prop = to_ui_prop(owner, "NewName");
    newName = ui_text_value(prop);

    if (newName == NULL || newName == "")
    {
	print_error("New Name must not be an empty string.");
	return rc;
    }

    if (n.type == "SEFile")
    {
	// pop up a chooser for this
	int i;
	node nextn, dir;
	list l;
	
	SEFileRenames = list_select("node[SEFile && name == '${n.name}']");
	if (list_count(SEFileRenames) == 1)
	    complete_stp_rename(n, newName);
	else
	{
	    CurNewName = newName;
	    l = list_create("string", 0);
	    ui_prop_sheet_pop_up("SEFileChooser");
	    prop = to_ui_prop("SEFileChooser", "Chooser");
	    ui_list_clear(prop);

	    for (i = 0; i < list_count(SEFileRenames); i++)
	    {
		nextn = list_get(SEFileRenames, i);
                dir = find_by_query("node[id = ${nextn.scope_node_id}]");
		list_append(l, "SEFile `" + nextn.name + "', scoped to SEDirectory `" + dir.name + "'");
	    }
	    ui_list_append(prop, l);
	    return rc;
	}
    }
    else
	complete_stp_rename(n, newName);

    return rc;
}

void 
complete_stp_rename(node n, string newName)
{
    node      n1;

    ui_footer_set_value(to_ui_prop(to_ui_owner(ParentProp), "Status"), "Renaming `" + n.name + "' to `" + newName + "'.");

    n1 = n;
    node_name_set(n1, newName);
    node_rename(n, n1);
    return;
}


int
special_file_callback(ui_event_tp e, ui_prop p, int id, string value)

{
    ui_prop prop = to_ui_prop("SEFileChooser", "Chooser");
    int row = ui_list_selected_row(prop);
    node n = list_get(SEFileRenames, row);

    complete_stp_rename(n, CurNewName);

    // to be sure it goes away in all cases
    if (ui_owner_visible(prop))
        ui_owner_dismiss(to_ui_owner("SEFileChooser"));

    return 1;
}

