//
//      sccsid[] = %W% %Y% %D%
//      StP/Booch
//      StP/OMT
//      StP/OO
//	StP Core
//      Confidential property of Interactive Development Environments, Inc.
//      Copyright (c) 1992-1995
//      All rights reserved
//

#include        "rules/qrl/uml_optional_dependencies.inc"

/************************************************************
 * Names of check boxes for optional dependencies must be
 * named the same the boolean is named.
 * Then we just use qrl_eval to assign the value from the ui
 ***************************************************************/
int
uml_rename_opt_dep_cb(ui_event_tp ev, ui_prop p, int id, string val)
{
    string	var = ui_prop_name(p);
    string	code = var + " = ui_choice_isset(p, 0);";

    qrl_eval(code);

    return 0;
    
}

void
uml_rename_set_dep_ui(string oname, string cname)
{
    boolean	v;
    string	code = "v = " + cname + ";";

    qrl_eval(code);

    ui_set_value(to_ui_prop(oname, cname), to_int(v));
}

const	string	GOR_UI_NEWLABEL_PROP = "NewLabel";

void
uml_rename_set_curr_label(string oname, int row, int col)
{
  string gor_current_label =	
    gte_get_cell_label(row, col);	

    // we are editing the new label in the dialog and do not
    // detect a global object rename
    if (gte_default_rename_fetch_new_label(oname, row, col) == gor_current_label )
        return;

  ui_set_value(to_ui_prop(oname, GOR_UI_NEWLABEL_PROP), gor_current_label);
  gte_default_rename_cache_new_label(oname, row, col, gor_current_label);
}


const	string	CLASS_RENAME_PROP_SHEET = "ClassRename";

// Class rename 
void
uml_class_rename_init(int row, int col)
{
    uml_rename_set_curr_label(CLASS_RENAME_PROP_SHEET, 
			     row, col);
    uml_rename_set_dep_ui(CLASS_RENAME_PROP_SHEET,
		    "dep_UmlClass_UmlOperation_OpConstructor");
}

void
uml_class_rename(int hsect, int vsect, int row, int col)
{
    gte_disable_action_completed();
    gte_stpem("PropertySheetShow " + CLASS_RENAME_PROP_SHEET);
    gte_default_rename_clear_new_label(CLASS_RENAME_PROP_SHEET);
    gte_enable_action_completed();

    uml_class_rename_init(gte_get_row(hsect, row), gte_get_col(vsect, col));
}

void
uml_class_rename_action_done()
{
    int row = gte_get_current_cell_row();
    int col = gte_get_current_cell_col();
    ui_prop p = to_ui_prop(CLASS_RENAME_PROP_SHEET, "All");
    
    if (gte_rename_action_done_standard_checks(row, col, p, "uml_class_rename") == False)
	return;
    
    uml_class_rename_init(row, col);
}

const	string	OPERATION_RENAME_PROP_SHEET = "OperationRename";


// Operation rename 

void
uml_operation_rename_init(int row, int col)
{
    uml_rename_set_curr_label(OPERATION_RENAME_PROP_SHEET,
			     row, col);
    uml_rename_set_dep_ui(OPERATION_RENAME_PROP_SHEET,
		    "dep_UmlOperation_UmlOperation_OpOverloaded");
}

void
uml_operation_rename(int hsect, int vsect, int row, int col)
{
    gte_disable_action_completed();
    gte_stpem("PropertySheetShow " + OPERATION_RENAME_PROP_SHEET);
    gte_default_rename_clear_new_label(OPERATION_RENAME_PROP_SHEET);
    gte_enable_action_completed();

    uml_operation_rename_init(gte_get_row(hsect, row), gte_get_col(vsect, col));
}

void
uml_operation_rename_action_done()
{
    int row = gte_get_current_cell_row();
    int col = gte_get_current_cell_col();
    ui_prop p = to_ui_prop(OPERATION_RENAME_PROP_SHEET, "All");
    
    if (gte_rename_action_done_standard_checks(row, col, p, "uml_operation_rename") == False)
	return;
    
    uml_operation_rename_init(row, col);
}

const	string	MESSAGE_RENAME_PROP_SHEET = "MessageRename";


// Message rename 

void
uml_message_rename_init(int row, int col)
{
    uml_rename_set_curr_label(MESSAGE_RENAME_PROP_SHEET,
			     row, col);
}

void
uml_message_rename(int hsect, int vsect, int row, int col)
{
    gte_disable_action_completed();
    gte_stpem("PropertySheetShow " + MESSAGE_RENAME_PROP_SHEET);
    gte_default_rename_clear_new_label(MESSAGE_RENAME_PROP_SHEET);
    gte_enable_action_completed();

    uml_message_rename_init(gte_get_row(hsect, row), gte_get_col(vsect, col));
}

void
uml_message_rename_action_done()
{
    int row = gte_get_current_cell_row();
    int col = gte_get_current_cell_col();
    ui_prop p = to_ui_prop(MESSAGE_RENAME_PROP_SHEET, "All");
    
    if (gte_rename_action_done_standard_checks(row, col, p, "uml_message_rename") == False)
	return;
    
    uml_message_rename_init(row, col);
}


