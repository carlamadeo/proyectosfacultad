
//
//      sccsid[] = %W% %Y% %D%
//      StP/SE
//      Confidential property of Interactive Development Environments, Inc.
//      Copyright (c) 1995
//      All rights reserved
//

// callbacks for codegen UI

int
OkGenerateCode(ui_event_tp event, ui_prop prop, int id, string val)
{
    generateCode(event, prop, id, val);
    return (1);
}

int
ApplyGenerateCode(ui_event_tp event, ui_prop prop, int id, string val)
{
    generateCode(event, prop, id, val);
    return (0);
}

int
ResetGenerateCode(ui_event_tp event, ui_prop prop, int id, string val)
{

    ui_choice_set_value(to_ui_prop("SECodeGeneration", "kandr_or_ansi"), 0);
    ui_choice_set_value(to_ui_prop("SECodeGeneration", "alternate_directory"), 0);
    ui_text_set_value(to_ui_prop("SECodeGeneration", "file_directory"), 
		    current_projdir() + current_system() + "/src_files");
    ui_choice_set_value(to_ui_prop("SECodeGeneration", "alternate_file"), 0);
    ui_text_set_value(to_ui_prop("SECodeGeneration", "file_name"), "");
    ui_choice_set_value(to_ui_prop("SECodeGeneration", "view_file"), 0);
    ui_text_set_value(to_ui_prop("SECodeGeneration", "view_editor"), "vi");
    ui_text_set_value(to_ui_prop("SECodeGeneration", "file_interface_extension"), ".h");
    ui_text_set_value(to_ui_prop("SECodeGeneration", "file_extension"), ".c");
    ui_choice_set_value(to_ui_prop("SECodeGeneration", "delete_old_code"), 0);
    ui_choice_set_value(to_ui_prop("SECodeGeneration", "verbose"), 0);
    ui_choice_set_value(to_ui_prop("SECodeGeneration", "gen_code_body"), 0);
    ui_choice_set_value(to_ui_prop("SECodeGeneration", "gen_comment"), 0);
    ui_choice_set_value(to_ui_prop("SECodeGeneration", "check_only"), 0);

    return (0);
}

int
GenCodefromDesktop_cb(ui_event_tp event, ui_prop theprop, int id, string value)
{
    int rc;

    ui_prop check = to_ui_prop( to_ui_owner( theprop ), "check_only" );

    if ( ui_value( check ) == 0 )
    {
	if ( editor_confirm( "Generate Code overwrites or modifies source code\n" +
		"Be careful not to overwrite reverse engineered source code.\n\n\nAre you sure you want to proceed?", "Continue", "Cancel" ) == False )
	    return 0;
    }

    return 1;
}

void
generateCode(ui_event_tp ev, ui_prop prop, int id, string val)
{
    string owner;
    prop_tp props;
    int ansi;

    if ( ui_value(to_ui_prop(ui_owner_name(prop),"check_only")) == 0 )
	if ( editor_confirm( "Generate Code overwrites or modifies source code\n" +
		"Be careful not to overwrite reverse engineered source code.\n\n\nAre you sure you want to proceed?", "Continue", "Cancel" ) == False )
	    return;

    owner = ui_owner_name(prop);
    ui_set_value(to_ui_prop(owner,"Status"),"");

    props.obj_name = "";
    props.obj_type = "";

    ansi = ui_value(to_ui_prop(owner,"kandr_or_ansi"));

    if (!ansi)
	props.kandr_or_ansi = "ansi";
    else props.kandr_or_ansi = "kandr";

    props.delete_old_code = ui_value(to_ui_prop(owner,"delete_old_code"));
    props.verbose = ui_value(to_ui_prop(owner,"verbose"));
    props.gen_code_body = ui_value(to_ui_prop(owner,"gen_code_body"));
    props.gen_comment = ui_value(to_ui_prop(owner,"gen_comment"));

    props.file_directory = ui_value(to_ui_prop(owner,"file_directory"));

    props.alternate_file = ui_value(to_ui_prop(owner,"alternate_file"));
    if (props.alternate_file)
        props.file_name = ui_value(to_ui_prop(owner,"file_name"));
    else
        props.file_name = "";

    props.view_file = ui_value(to_ui_prop(owner,"view_file"));
    props.view_editor = ui_value(to_ui_prop(owner,"view_editor"));
    props.file_extension = ui_value(to_ui_prop(owner,"file_extension"));
    props.file_interface_extension = ui_value(to_ui_prop(owner,"file_interface_extension"));
    props.check_only = ui_value(to_ui_prop(owner,"check_only"));

    generate_code_from_sce(props);

}

