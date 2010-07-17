//
//      sccsid[] = @(#)codegen.inc	5.10  96/07/23
//      StP/SE
//      Confidential property of Interactive Development Environments, Inc.
//      Copyright (c) 1993
//      All rights reserved
//

// codegen.inc


struct prop_tp
{
    string obj_name;
    string obj_type;
    string kandr_or_ansi;
    int delete_old_code;
    int verbose;
    int gen_code_body;
    int gen_comment;
    int alternate_directory;
    string file_directory;
    int alternate_file;
    string file_name;
    int view_file;
    string view_editor;
    string file_extension;
    string file_interface_extension;
    int check_only;
};

