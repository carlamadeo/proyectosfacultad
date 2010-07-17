
//      StP/UML
//      Confidential property of Aonix
//      Copyright (c) 1997
//      All rights reserved
//

///////////////////////////////////////////////////////
// GOR messages - handling custom prop sheet
///////////////////////////////////////////////////////

//#include "rules/qrl/uml_inherit_ops.inc"

const	string	INT_MESSAGE_RENAME_PROP_SHEET = "InteractMessageRename";


void uml_interact_message_rename(gde_mapsymbol the_message)
{
    gde_builtin("PropertySheetShow " + INT_MESSAGE_RENAME_PROP_SHEET);
    gde_default_rename_clear_new_label(INT_MESSAGE_RENAME_PROP_SHEET);
    uml_interact_message_rename_init(the_message);
}

void uml_interact_message_rename_init(gde_mapsymbol the_message)
{
    uml_rename_set_curr_label(INT_MESSAGE_RENAME_PROP_SHEET, the_message);
    uml_rename_set_dep_ui(INT_MESSAGE_RENAME_PROP_SHEET,
			  "dep_UmlMessage_UmlMessage_sameSignature");
    uml_rename_set_dep_ui(INT_MESSAGE_RENAME_PROP_SHEET,
			  "dep_UmlMessage_UmlOperationUmlSignal");
}


void uml_interact_message_rename_action_done()
{
    gde_mapsymbol ms = gde_selected_mapsymbol();
    ui_prop p = to_ui_prop(INT_MESSAGE_RENAME_PROP_SHEET, "All");
    
    if (gde_rename_action_done_standard_checks(ms, p, "uml_interact_message_rename") == False)
	return;
    
    uml_interact_message_rename_init(ms);
}

