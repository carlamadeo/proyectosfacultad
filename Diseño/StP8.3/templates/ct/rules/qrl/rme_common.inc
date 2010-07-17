//////////////////////////////////////////////////////////////////////////////
//
//      sccsid[] = @(#)rme.qrl
//      StP
//      Confidential property of Interactive Development Environments, Inc.
//      Copyright (c) 1992-2001
//      All rights reserved
//
//////////////////////////////////////////////////////////////////////////////

void
GenerateQRLReport( string the_file )
{
    string projdir = current_projdir();
    string system = current_system();
    string cmd = "qrp" + forward_messages_arg() + projdir_system_args() + 
        " qrl/basic/rme_gen.qrl -x DIAGNAME " + the_file +
        " -o '${projdir}${system}/${filestore}/" + the_file + ".qrl'";
    sys_call( cmd, "Generate QRL Report failed." );
    return;
}

void
EditQRLReport( string the_file )
{
    string cmd;
    string full_file = "${projdir}${system}/${filestore}/" + the_file + ".qrl";
    
    if( is_windows_platform() ) {
        cmd = "start notepad " + full_file;
    } else {
        cmd = "sh -c 'xterm -e vi " + full_file + "'";
    }
    sys_call( cmd, "Edit QRL Report failed." );
    return;
}

void
RunQRLReport( string the_file, string target )
{
    string projdir = current_projdir();
    string system = current_system();
    string ext = target;
    if( target == "ileaf" ) ext = "doc";
    if( target == "ascii" ) ext = "asc";

    string cmd = "qrp" + forward_messages_arg() + projdir_system_args() +
        " -t " + target + " ${projdir}${system}/${filestore}/" + the_file + ".qrl " +
        "-o '${projdir}${system}/qrl_files/" + the_file + "." + ext + "'";
    sys_call( cmd, "Run QRL Report failed." );
    return;
}


int
RmeCheckSemantics()
{
    string  filename = gde_diagram_name();
    int errors = 0;

    list filelist = file_list("RmeDiagram", filename);

    // Save the current state of the diagram before checking it.

    gde_builtin("FileSave");

    errors = RmeCheckSemanticsFromQrl(filelist);
    return(-1 * errors);
 
}

// This function is defined here to use the builtin message log function.
void out_error(string text)
{
    print_error(text);
}

// This function is defined here to use the builtin message log function.
void out_message(string text)
{
    print_message(text);
}

void
sys_call( string cmd, string err_msg )
{
    int ret;
    if( is_windows_platform() ) {
        ret = system( cmd );
        if( ret )
            print_error( err_msg );
    } else {
        system_async( cmd );
        // returned status wouldn't provide any info about the command ...
    }
    return;
}

