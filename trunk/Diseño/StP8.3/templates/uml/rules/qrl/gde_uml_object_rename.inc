//      StP/Uml
//      Confidential property of Aonix
//      Copyright (c) 1997
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
uml_rename_set_curr_label(string oname, gde_mapsymbol ms)
{
    gor_current_mapsym = ms;

    if (gde_mapsymbol_is_node(gor_current_mapsym))
	gor_current_label = 
	    gde_node_label(to_gde_node(gor_current_mapsym));
    else if (gde_mapsymbol_is_link(gor_current_mapsym))
	gor_current_label = 
	    gde_link_label(to_gde_link(gor_current_mapsym));
    else
	gor_current_label = 
	    gde_cntx_label(to_gde_cntx(gor_current_mapsym));

    // we are editing the new label in the dialog and do not
    // detect a global object rename
    if (gde_default_rename_fetch_new_label(oname, ms) == gor_current_label )
        return;

    ui_set_value(to_ui_prop(oname, GOR_UI_NEWLABEL_PROP), gor_current_label);
    gde_default_rename_cache_new_label(oname, ms, gor_current_label);
}

// State Transition Rename
const	string	TRANSITION_RENAME_PROP_SHEET = "TransitionRename";

void
uml_transition_rename_init(gde_mapsymbol this)
{
    uml_rename_set_curr_label(TRANSITION_RENAME_PROP_SHEET, this);

    uml_rename_set_dep_ui(TRANSITION_RENAME_PROP_SHEET,
		    "dep_UmlTransition_UmlEvent_TransitionEvent");
    uml_rename_set_dep_ui(TRANSITION_RENAME_PROP_SHEET,
		    "dep_UmlTransition_UmlTransition_TransitionInSameCntx");
}


void
uml_transition_rename(gde_mapsymbol this)
{
    gde_builtin("PropertySheetShow " + TRANSITION_RENAME_PROP_SHEET);
    gde_default_rename_clear_new_label(TRANSITION_RENAME_PROP_SHEET);
    uml_transition_rename_init(this);
}

void
uml_transition_rename_action_done()
{
    gde_mapsymbol ms = gde_selected_mapsymbol();
    ui_prop p = to_ui_prop(TRANSITION_RENAME_PROP_SHEET, "All");
    
    if (gde_rename_action_done_standard_checks(ms, p, "uml_transition_rename") == False)
	return;
    
    uml_transition_rename_init(ms);
}

// Event Rename
const	string	EVENT_RENAME_PROP_SHEET = "EventRename";

void
uml_event_rename_init(gde_mapsymbol this)
{
    uml_rename_set_curr_label(EVENT_RENAME_PROP_SHEET, this);

    uml_rename_set_dep_ui(EVENT_RENAME_PROP_SHEET,
		    "dep_UmlEventLink_Message_Name");
}

void
uml_event_rename(gde_mapsymbol this)
{
    gde_builtin("PropertySheetShow " + EVENT_RENAME_PROP_SHEET);
    gde_default_rename_clear_new_label(EVENT_RENAME_PROP_SHEET);
    uml_event_rename_init(this);
}

void
uml_event_rename_action_done()
{
    gde_mapsymbol ms = gde_selected_mapsymbol();
    ui_prop p = to_ui_prop(EVENT_RENAME_PROP_SHEET, "All");
    
    if (gde_rename_action_done_standard_checks(ms, p, "uml_event_rename") == False)
	return;

    uml_event_rename_init(ms);
}

// Message Rename
const	string	MESSAGE_RENAME_PROP_SHEET = "MessageRename";

void
uml_message_rename_init(gde_mapsymbol this)
{
    uml_rename_set_curr_label(MESSAGE_RENAME_PROP_SHEET, this);

}

void
uml_message_rename(gde_mapsymbol this)
{
    gde_builtin("PropertySheetShow " + MESSAGE_RENAME_PROP_SHEET);
    gde_default_rename_clear_new_label(MESSAGE_RENAME_PROP_SHEET);
    uml_message_rename_init(this);
}

void
uml_message_rename_action_done()
{
    gde_mapsymbol ms = gde_selected_mapsymbol();
    ui_prop p = to_ui_prop(MESSAGE_RENAME_PROP_SHEET, "All");
    
    if (gde_rename_action_done_standard_checks(ms, p, "uml_message_rename") == False)
	return;

    uml_message_rename_init(ms);
}
