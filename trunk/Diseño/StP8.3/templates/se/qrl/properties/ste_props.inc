//////////////////////////////////////////////////////////////////////////////
//
//      StP/SE
//      Confidential property of Interactive Development Environments, Inc.
//      Copyright (c) 1995
//      All rights reserved
//
// ste_props.inc: STE property sheet qrl hooks
//
//////////////////////////////////////////////////////////////////////////////


// callbacks for ste semantic checks

int
OkSteCheckSemantics(ui_event_tp event, ui_prop prop, int id, string value)
{
    ApplySteCheckSemantics(event, prop, id, value);
    return(1);
}

int
ApplySteCheckSemantics(ui_event_tp event, ui_prop prop, int id, string value)
{
    steprop_tp	props;

    props.verbose = ui_value(to_ui_prop("STESemanticChecks", "verbose"));

    props.check_one = ui_value(to_ui_prop("STESemanticChecks", "check_one"));
    props.check_two = ui_value(to_ui_prop("STESemanticChecks", "check_two"));
    props.check_three = ui_value(to_ui_prop("STESemanticChecks", "check_three"));
    props.check_four = ui_value(to_ui_prop("STESemanticChecks", "check_four"));
    props.check_five = ui_value(to_ui_prop("STESemanticChecks", "check_five"));
    props.check_six = ui_value(to_ui_prop("STESemanticChecks", "check_six"));

    props.diaglist = file_list("SteDiagram", gde_substitute_string("${file}"));

    checkSteSemanticsFromSte(props);

    return(0);

}

