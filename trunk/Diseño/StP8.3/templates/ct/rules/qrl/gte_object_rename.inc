//
// 	sccsid[] = %W% %Y% %D%
// 	StP/Core
// 	Confidential property of Interactive Development Environments, Inc.
// 	Copyright (c) 1992-1994
// 	All rights reserved
//

string gor_type;
string gor_current_label = "";

void
gte_default_global_object_rename_init(int row, int col)
{
  string gor_current_label =
    gte_get_current_cell_label();

    // we are editing the new label in the dialog and do not
    // detect a global object rename
    if (gte_default_rename_fetch_new_label("ObjectRename", row, col) == gor_current_label )
        return;
  ui_set_value(to_ui_prop("ObjectRename", "NewLabel"), gor_current_label);
  gte_default_rename_cache_new_label("ObjectRename", row, col, gor_current_label);
}

struct gte_default_new_label_tp
{
    string oname;
    int row;
    int col;
    string label;
};

list h_gte_default_new_labels = list_create( "gte_default_new_label_tp", 0 );

void
gte_default_rename_clear_new_label(string oname)
{
    int i;
    gte_default_new_label_tp h_gte_default_new_label;
    for ( i = list_count(h_gte_default_new_labels) - 1; i >= 0 ; i = i - 1)
    {
        h_gte_default_new_label = list_get(h_gte_default_new_labels, i);
        if (oname == h_gte_default_new_label.oname)
        {
           list_delete(h_gte_default_new_labels,i);
        }
    }
}

string
gte_default_rename_fetch_new_label(string oname, int row, int col)
{
    int i;
    gte_default_new_label_tp h_gte_default_new_label;
    string label = "";
    for ( i = list_count(h_gte_default_new_labels) - 1; i >= 0 ; i = i - 1)
    {
        h_gte_default_new_label = list_get(h_gte_default_new_labels, i);
        if (oname == h_gte_default_new_label.oname &&
            row == h_gte_default_new_label.row &&
            col == h_gte_default_new_label.col)
            label = h_gte_default_new_label.label;
    }
    return label;
}

void
gte_default_rename_cache_new_label(string oname, int row, int col, string label)
{
    int i;
    gte_default_new_label_tp h_gte_default_new_label;
    for ( i = list_count(h_gte_default_new_labels) - 1; i >= 0 ; i = i - 1)
    {
        h_gte_default_new_label = list_get(h_gte_default_new_labels, i);
        if (oname == h_gte_default_new_label.oname)
        {
           list_delete(h_gte_default_new_labels,i);
        }
    }

    h_gte_default_new_label.oname = oname;
    h_gte_default_new_label.row = row;
    h_gte_default_new_label.col = col;
    h_gte_default_new_label.label = label;

    list_append( h_gte_default_new_labels, h_gte_default_new_label);
    return ;
}

void
gte_default_global_object_rename(int hsect, int vsect, int row, int col)
{
  gte_stpem("PropertySheetShow ObjectRename");
  gte_default_rename_clear_new_label("ObjectRename");
  gte_default_global_object_rename_init(gte_get_row(hsect, row),
					gte_get_col(vsect, col));
}

boolean
gte_rename_action_done_standard_checks(int row, int col, ui_prop p,
				       string rename_func)
{
    if (row == 0 && col == 0)
    {
	ui_activate(p, False);
	return False;
    }
    
    if (gte_get_cell_rename_func(row, col) != rename_func)
    {
	ui_activate(p, False);
	return False;
    }
    
    ui_activate(p, True);

    return True;
}

void
gte_object_rename_action_done()
{
    int row = gte_get_current_cell_row();
    int col = gte_get_current_cell_col();
    ui_prop p = to_ui_prop("ObjectRename", "All");

    if (gte_rename_action_done_standard_checks(row, col, p, "gte_default_global_object_rename") == False)
	return;
    
    gte_default_global_object_rename_init(row, col);
}

int 
gte_global_object_rename_doit_callback(ui_event_tp event, 
				       ui_prop theprop, 
				       int id,
				       string value)
{
  int result = gte_global_object_rename_callback(event,theprop,id,value,False);
  return result;
}

int
gte_global_object_rename_check_impact_callback(ui_event_tp event, 
					       ui_prop theprop, 
					       int id,
					       string value)
{
  return gte_global_object_rename_callback(event,theprop,id,value,True);
}

int 
gte_global_object_rename_callback(ui_event_tp event, 
				  ui_prop theprop,
				  int id,
				  string value,
				  boolean check_impact)
{
  string owner;
  string new_label;
  string current_label;
 
  owner = ui_owner_name(theprop);
  ui_set_value(to_ui_prop(owner,"Status"),"");
  new_label = ui_value(to_ui_prop(owner,"NewLabel"));
    
  current_label = gte_get_current_cell_label();

  if(new_label == current_label) {
    ui_set_value(to_ui_prop(owner,"Status"),"Enter a Different Label.");
    return 0;
  }

  if (check_impact) {	
      editor_set_is_busy(True);
      gte_global_rename_current_cell(new_label, check_impact);
      editor_set_is_busy (False);
      return 0;
  } 
  
  else {
      editor_set_is_busy(True);
      gte_global_rename_current_cell(new_label, check_impact);
      editor_set_is_busy (False);
      return 1;
  }
}

int 
gte_global_object_rename_cancel_callback(ui_event_tp event,
					     ui_prop theprop, 
					     int id,
					     string value)
{
  return 1;
}

// This really doesn't need to be here except because of a qrl interpreter problem
// with recognizing builtin functions as active funcs

int
idehook_rename_object_systemwide_active_func()
{
  return gte_global_rename_current_cell_active_func();
}



