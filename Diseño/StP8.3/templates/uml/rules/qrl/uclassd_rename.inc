//      StP/Uml
//      Confidential property of Aonix
//      Copyright (c) 1997
//      All rights reserved
//


// Class rename 
const	string	CLASS_RENAME_PROP_SHEET = "ClassRename";

void
uml_class_rename_init(gde_mapsymbol the_class)
{
    uml_rename_set_curr_label(CLASS_RENAME_PROP_SHEET, the_class);
    uml_rename_set_dep_ui(CLASS_RENAME_PROP_SHEET,
		    "dep_UmlClass_UmlOperation_OpConstructor");
}

void
uml_class_rename(gde_mapsymbol the_class)
{
    gde_builtin("PropertySheetShow " + CLASS_RENAME_PROP_SHEET);
    gde_default_rename_clear_new_label(CLASS_RENAME_PROP_SHEET);
    uml_class_rename_init(the_class);
}

void
uml_class_rename_action_done()
{
    gde_mapsymbol ms = gde_selected_mapsymbol();
    ui_prop p = to_ui_prop(CLASS_RENAME_PROP_SHEET, "All");
    
    if (gde_rename_action_done_standard_checks(ms, p, "uml_class_rename") == False)
	return;
    uml_class_rename_init(ms);
}



// Operation rename 

const	string	OPERATION_RENAME_PROP_SHEET = "OperationRename";


void
uml_operation_rename_init(gde_mapsymbol this)
{
    uml_rename_set_curr_label(OPERATION_RENAME_PROP_SHEET, this);

    uml_rename_set_dep_ui(OPERATION_RENAME_PROP_SHEET,
		    "dep_UmlOperation_UmlOperation_OpOverloaded");
}

void
uml_operation_rename(gde_mapsymbol this)
{
    gde_builtin("PropertySheetShow " + OPERATION_RENAME_PROP_SHEET);
    gde_default_rename_clear_new_label(OPERATION_RENAME_PROP_SHEET);

    uml_operation_rename_init(this);
}

void
uml_operation_rename_action_done()
{
    gde_mapsymbol ms = gde_selected_mapsymbol();
    ui_prop p = to_ui_prop(OPERATION_RENAME_PROP_SHEET, "All");

    if (gde_rename_action_done_standard_checks(ms, p, "uml_operation_rename") == False)
	return;
    
    uml_operation_rename_init(ms);
}

