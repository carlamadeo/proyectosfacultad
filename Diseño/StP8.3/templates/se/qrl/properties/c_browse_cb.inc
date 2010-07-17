
//      StP/SE
//      Confidential property of Aonix
//      Copyright (c) 1995 - 1997
//      All rights reserved
//


#include "ct/rules/qrl/re_navigate.inc"
#include "qrl/include/se_utils.inc"

// callbacks for C Browser UI


int
RESelectFocusFiles_cb(ui_event_tp event, ui_prop theprop, int id, string value)

{
    ui_prop prop;
    string cmdstr, f, fname, s = NULL;
    list filelist;
    int i;
    list l = list_create("string", 0);

    cmdstr = "xrb_get_files -forward_messages " + editor_get_msgd_handle() +
	" -p " + current_projdir() + " -s " + current_system();

    // get values for types of files
    prop = to_ui_prop("REFocusFiles", "FocusFileSrc");
    if (ui_choice_value(prop) == 0)
        cmdstr = cmdstr + " -nsf";

    prop = to_ui_prop("REFocusFiles", "FocusFileUserHeader");
    if (ui_choice_value(prop) == 0)
        cmdstr = cmdstr + " -nuh";

    prop = to_ui_prop("REFocusFiles", "FocusFileSysHeader");
    if (ui_choice_value(prop) == 0)
        cmdstr = cmdstr + " -nsh";

    fname = temp_file("fflist", "se");

    cmdstr = cmdstr + " -of " + fname;

    system(cmdstr);

    prop = to_ui_prop("REFocusFiles", "FocusChoice");
    ui_list_clear(prop);

    if (read_file_access(fname))
    {
	s = read_file(fname);
    delete_file(fname);
    }

    if (s == NULL || s == "")
    {
	print_message("No matching files found.");
	return 0;
    }

    filelist = string_to_list(s, "\n");
    for (i = 0; i < list_count(filelist); i++)
    {
	f = list_get(filelist, i);
	list_append(l, f);
    }
    if (list_count(l) > 0)
	ui_list_append(prop, l);
	
    return 0;
}




int
REMoveToFocus_cb(ui_event_tp event, ui_prop theprop, int id, string value)
 
{
    list l, final_l = list_create("string", 0);
    ui_prop prop, fprop;
    string s, item;
    int i, j, num;
    boolean dup;

    prop = to_ui_prop("REFocusFiles", "FocusChoice");
    fprop = to_ui_prop("REFocusFiles", "Focus");

    l = ui_list_selected_items(prop);

    for (i = 0; i < list_count(l); i++)
    {
	item = list_get(l, i);
	dup = False;

	// check against what is there already
	num = ui_list_num_items(fprop, LastValue);
        for (j = 0; j < num; j++)
        {
            s = ui_list_row_label(fprop, j);
            if (s == item)
            {
                dup = True;
                break;
            }
        }

	if ( ! dup)
	{
	    num = list_count(final_l);
	    for (j = 0; j < num; j++)
	    {
		s = list_get(final_l, j);
		if (s == item)
		{
		    dup = True;
		    break;
		}
	    }
	}

	if (! dup)
	    list_append(final_l, item);
    }
    ui_list_append(fprop, final_l);
    
    return 0;
}




// try the is selected that returns a list.
int
REFocusDelete_cb(ui_event_tp event, ui_prop theprop, int id, string value)

{
    list sellist;
    ui_prop prop = to_ui_prop("REFocusFiles", "Focus");
    int i, val, offset;

    sellist = ui_list_selected(prop);
   
    for (i = 0, offset = 0; i < list_count(sellist); i++, offset++)
    {
	val = list_get(sellist, i);
	ui_list_delete(prop, val - offset);
    }
    
    return 0;
}



string
get_focus_file()

{
    string ff;

    // just check for read access on the focus file. If it is there, use it.

    ff = current_projdir() + current_system() + "/revc_files/FocusFile." + user();
    if (! read_file_access(ff))
	return "";
    else 
        return " -ff " + ff;
}


int
FocusFilesApply_cb(ui_event_tp event, ui_prop theprop, int id, string value) 

{ 
    int rc; 
    ui_prop prop = to_ui_prop("REFocusFiles", "Focus"),
            footerp = to_ui_prop("REFocusFiles", "Status"); 
    list l; 
    string s, ff;
    int i;

    ff = current_projdir() + current_system() + "/revc_files/FocusFile." + user();

    if (ui_button_is_ok(theprop))
        rc = 1;
    else
        rc = 0;

    ui_list_select_all(prop);
    l = ui_list_selected_items(prop);
    ui_list_deselect(prop);

    if ((l == NULL) || (list_count(l) == 0))
    {
    delete_file(ff);
	ui_footer_set_value(footerp, "Focus File removed.");
	return rc;
    }

    i = list_count(l);
    s = list_to_string(l, "\n");
   
    if (file_exists(ff) && (! write_file_access(ff)))
	print_error("Could not create focus file `" + ff + "' (check permissions).");
    else
    {
	if (i == 1)
	    ui_footer_set_value(footerp, "1 item saved to Focus File");
	else
	    ui_footer_set_value(footerp, i + " items saved to Focus File");
        write_file(ff, s);
    }

    return rc;
}



int
FocusFilesReset_cb(ui_event_tp event, ui_prop theprop, int id, string value) 

{ 
    int rc; 
    ui_prop prop = to_ui_prop("REFocusFiles", "Focus"); 
    list l; 
    string s, ff;
    int i;

    // restore the focus file if it exists
    ff = current_projdir() + current_system() + "/revc_files/FocusFile." + user();

    if (! read_file_access(ff))
	ui_footer_set_value(to_ui_prop("REFocusFiles", "Status"), "No Focus File for user " + user() + " exists in this system.");
    else
    {
	s = read_file(ff);
        l = string_to_list(s, "\n");
	if (list_count(l) < 1)
	    ui_footer_set_value(to_ui_prop("REFocusFiles", "Status"), "Empty Focus File found.");
	else
	{
	    ui_list_clear(prop);
	    for (i = 0; i < list_count(l); i++)
	    {
		s = list_get(l, i);
		ui_list_append(prop, s);
            }
	}
    }
    return 0;
}


int CurKey = -1;
string cb_last_name = NULL;


void
CCodeBrowseNotifySelect()
{
    ui_prop prop = to_ui_prop("BrowserCodeBrowseChooser", "Chooser"),prop2;
    int row = ui_list_selected_row(prop);
    string s;
    list l;

    if (row >= 0)
      {
	s = list_get(CurBrowseList, row);
	l = string_to_list(s, "\t");
	if (list_count(l) == 6)
	  { 
	    prop2 = to_ui_prop("RECCodeBrowse", "FuncTypeScope" );
	    ui_text_set_value(prop2, list_get(l, 5));
	  }

	if (list_count(l) > 4)
	  CurKey = to_int(list_get(l, 4));
	else
	  CurKey = -1;

	if (list_count(l) > 0)
	  {
	    prop2 = to_ui_prop("RECCodeBrowse", "CCodeBrowseName");
	    ui_text_set_value(prop2, list_get(l, 0));
	  }
      }
    return;
}

int
CCodeBrowseDoIt_cb(ui_event_tp event, ui_prop theprop, int id, string value)
 
{
    int i;
    ui_prop prop;
    string name, cmdstr, tf, s;
    list l;

    cmdstr = "xrb_search -forward_messages " + editor_get_msgd_handle() + 
	" -p " + current_projdir() + " -s " + current_system();

    // name
    prop = to_ui_prop("RECCodeBrowse", "CCodeBrowseName");
    name = ui_text_value(prop);

    if (name == NULL || name == "")
    {
	print_error("C Code Browser: name required.");
	CurKey = -1;
	return -1;
    }
    if ( cb_last_name != name )
	CurKey = -1;
    cb_last_name = name;

    cmdstr = cmdstr + " -name '" + name + "'";

    // type.  currently only an interface to get the number of the choiceitem
    // selected, rather then the onvalue/offvalue.
    prop = to_ui_prop("RECCodeBrowse", "ObjectType");
    i = ui_choice_value(prop);
    if (i == 0)
	cmdstr = cmdstr + " -T func";
    else if (i == 1)
	cmdstr = cmdstr + " -T glob";
    else if (i == 2)
	cmdstr = cmdstr + " -T lib";
    else if (i == 3)
	cmdstr = cmdstr + " -T const";
    else if (i == 4)
	cmdstr = cmdstr + " -T typedef";
    else if (i == 5)
	cmdstr = cmdstr + " -T literal";
    else if (i == 6)
	cmdstr = cmdstr + " -T datas";
    else if (i == 7)
	cmdstr = cmdstr + " -T datam";
    else if (i == 8)
	cmdstr = cmdstr + " -T define";
    else if (i == 9)
	cmdstr = cmdstr + " -T comment";
    else if (i == 10)
	cmdstr = cmdstr + " -T file";
    else if (i == 11)
    {
	cmdstr = cmdstr + " -T id";
	// find all the on/off thingies
	prop = to_ui_prop("REIdentifiersOnly", "IdentifierFunctions");
	cmdstr = cmdstr + " -func " + ui_choice_value(prop);

	prop = to_ui_prop("REIdentifiersOnly", "IdentifierGlobals");
	cmdstr = cmdstr + " -glob " + ui_choice_value(prop);

	prop = to_ui_prop("REIdentifiersOnly", "IdentifierConstants");
	cmdstr = cmdstr + " -const " + ui_choice_value(prop);

	prop = to_ui_prop("REIdentifiersOnly", "IdentifierTypedefs");
	cmdstr = cmdstr + " -typedef " + ui_choice_value(prop);

	prop = to_ui_prop("REIdentifiersOnly", "IdentifierVolatiles");
	cmdstr = cmdstr + " -volatile " + ui_choice_value(prop);
    }

    // scope
    prop = to_ui_prop("RECCodeBrowse", "FuncTypeScope");
    name = ui_text_value(prop);
    if (name != NULL && name != "")
        cmdstr = cmdstr + " -scope " + name;

    // key.  This is from the currently highlighted entry, if there is one.
    if (CurKey != -1)
	cmdstr = cmdstr + " -key " + CurKey;

    // focus 
    cmdstr = cmdstr + get_focus_file();

    // read
    prop = to_ui_prop("RECCodeBrowse", "ReadAccess");
    if (ui_choice_value(prop))
	cmdstr = cmdstr + " -read";

    // write
    prop = to_ui_prop("RECCodeBrowse", "WriteAccess");
    if (ui_choice_value(prop))
	cmdstr = cmdstr + " -write";

    // definitions
    prop = to_ui_prop("RECCodeBrowse", "Definitions");
    if (ui_choice_value(prop))
	cmdstr = cmdstr + " -def";

    // references
    prop = to_ui_prop("RECCodeBrowse", "References");
    if (ui_choice_value(prop))
	cmdstr = cmdstr + " -ref";

    // case sensitive
    prop = to_ui_prop("RECCodeBrowse", "CaseSensitive");
    if (ui_choice_value(prop))
	cmdstr = cmdstr + " -ncs";

    // exact match
    prop = to_ui_prop("RECCodeBrowse", "ExactMatch");
    if (ui_choice_value(prop))
	cmdstr = cmdstr + " -tal";

    tf = temp_file("flist", "se");
    cmdstr = cmdstr + " -of " + tf;

    prop = to_ui_prop("RECCodeBrowse", "Status");
    ui_busy(to_ui_owner(prop), True);
    ui_footer_set_value(prop, "Searching..." );

    system( cmdstr );

    execute_chooser(tf, to_ui_prop("RECCodeBrowse", "Status"), 0);
    delete_file(tf);

    ui_busy(to_ui_owner(to_ui_prop("RECCodeBrowse", "Status")), False);

    return 0;
}


int
REIdentifier_cb(ui_event_tp event, ui_prop theprop, int id, string value)
 
{
    ui_prop_sheet_pop_up("REIdentifiersOnly");

    return 0;
}



 
int
FocusPopUp_cb(ui_event_tp event, ui_prop theprop, int id, string value)
 
{
    ui_prop_sheet_pop_up("REFocusFiles");
 
    return 0;
}
 


 
 
int
SE_generic_reset_cb(ui_event_tp event, ui_prop theprop, int id, string value)
 
{
    return 0;
}
 

 
int
ReferenceDeSelected_cb(ui_event_tp event, ui_prop theprop, int id, string value)
 
{
    // if deselected, be sure to select definitions
    ui_choice_set_value(to_ui_prop("RECCodeBrowse", "Definitions"), 1);

    return 0;
}

 
int
DefinitionDeSelected_cb(ui_event_tp event, ui_prop theprop, int id, string value)
 
{
    ui_choice_set_value(to_ui_prop("RECCodeBrowse", "References"), 1);

    return 0;
}


