//
// 	sccsid[] = %W% %Y% %D%
// 	StP/Core
// 	Confidential property of Interactive Development Environments, Inc.
// 	Copyright (c) 1992-1994
// 	All rights reserved
//


void
gte_rqtbl_global_object_rename_init(int row, int col)
{
    string	label = gte_get_current_cell_label();

    ui_set_value(to_ui_prop("RqtblObjectRename", "NewLabel"), label);
}

void
requirement_rename(int hsect, int vsect, int row, int col)
{
    gte_stpem("PropertySheetShow RqtblObjectRename");

    gte_rqtbl_global_object_rename_init(gte_get_row(hsect, row),
				    gte_get_col(vsect, col));
}

boolean
gte_rqtbl_rename_action_done_standard_checks(int row, int col, ui_prop p,
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
gte_rqtbl_object_rename_action_done()
{
    int row = gte_get_current_cell_row();
    int col = gte_get_current_cell_col();
    ui_prop p = to_ui_prop("RqtblObjectRename", "All");

    if (gte_rqtbl_rename_action_done_standard_checks(row, col, p, "requirement_rename") == False)
	return;
    
    gte_rqtbl_global_object_rename_init(row, col);
}

int 
gte_rqtbl_global_object_rename_doit_callback(ui_event_tp event, 
				       ui_prop theprop, 
				       int id,
				       string value)
{
  return gte_rqtbl_global_object_rename_callback(event,theprop,id,value,False);
}

int
gte_rqtbl_global_object_rename_check_impact_callback(ui_event_tp event, 
					       ui_prop theprop, 
					       int id,
					       string value)
{
  return gte_rqtbl_global_object_rename_callback(event,theprop,id,value,True);
}

int 
gte_rqtbl_global_object_rename_callback(ui_event_tp event, 
				  ui_prop theprop,
				  int id,
				  string value,
				  boolean check_impact)
{
  string owner;
  string new_label;
  string current_label;
  REQ_LabelType	curr_l, new_l;

 
  owner = ui_owner_name(theprop);
  ui_set_value(to_ui_prop(owner,"Status"),"");
  new_label = ui_value(to_ui_prop(owner,"NewLabel"));
    
  current_label = gte_get_current_cell_label();

  if(new_label == current_label)
  {
    ui_set_value(to_ui_prop(owner,"Status"),"Enter a Different Label.");
    return 0;
  }
  curr_l = REQ_ParseString(current_label);
  new_l = REQ_ParseString(new_label);

  if (curr_l.unique_id == new_l.unique_id)
  {
    if (check_impact)
    {
	ui_set_value(to_ui_prop(owner,"Status"),
		"Requirement name has no dependent objects");
	return 0;
    } else {
	CurrentCellLabel(new_label);
	return(1);
    }
  }


  if (check_impact)
  {	
      editor_set_is_busy(True);
      gte_global_rename_current_cell(new_label, check_impact);
      editor_set_is_busy (False);
      gte_refresh_selection();
      return 0;
  } else {
      editor_set_is_busy(True);
      gte_global_rename_current_cell(new_label, check_impact);
      editor_set_is_busy (False);
      gte_refresh_selection();
      return 1;
  }
}

int 
gte_rqtbl_global_object_rename_cancel_callback(ui_event_tp event,
					     ui_prop theprop, 
					     int id,
					     string value)
{
  return 1;
}


void
requirements_inverse_map_func(node req, 
			       list rename_data_list,
			       int hsect, int vsect, int row, int col,
			      gocha_tp gocha_type)
{
    string	new_label;
    int		i;
    rename_data	rn;
    string	rid, rname, rsep;
    REQ_LabelType	new_l;

    if ( gocha_type != gochaRename )
    {
	print_error( "requirements_inverse_map_func() can only handle renames" );
	return; 
    }

    rid = REQ_UniqueId(hsect, vsect, row, col);
    rname = REQ_InformalName(hsect, vsect, row, col);

    new_label = NULL;

    for(i = 0; i < list_count(rename_data_list); i = i + 1)
    {
	rn = list_get(rename_data_list, i);
	if (rn.dependency_name == "Self")
	{
	    new_label = rn.label;
	}
    }

    if (new_label == NULL)
    {
	rsep = REQ_unique_id_terminus();
	if (string_length(rsep) > 1)
	{
	    rsep = string_extract(rsep, 0, 1);
	}
	new_label = req.name + rsep + rname;
    }
    gte_invert_cell_label(new_label, hsect, vsect, row, col);
    gte_print_global_rename_message(rid, req.name);
}
