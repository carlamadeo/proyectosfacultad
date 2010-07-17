////////////////////////////////////////////////////////////////////////////////
//
// 	sccsid[] = %W% %Y% %D%
// 	StP/Core
// 	Confidential property of Interactive Development Environments, Inc.
// 	Copyright (c) 1992-1995
// 	All rights reserved
//
//	Callbacks for the Version submenu.
//
////////////////////////////////////////////////////////////////////////////////

#include "ct/rules/qrl/subprocs.inc"

// stputil cmds for editor vcc menu entries

const string C_PRG = "stputil -p ${projdir} -s ${system}";
const string C_INI = " -C 'vcc init -e ${editor}'";
const string C_EDT = " -C 'vcc edit -e ${editor} -T ${filetype} ${file}'";
const string C_DTA = " -C 'vcc delta -e ${editor} -T ${filetype} -u ${file}'";
const string C_GET = " -C 'vcc get -e ${editor} -T ${filetype} ${file}'";
const string C_CRT = " -C 'vcc create -u -e ${editor} -T ${filetype} ${file}'";
const string C_CAN = " -C 'vcc unedit -f -e ${editor} -T ${filetype} ${file}'";

// these are used during auto-checkout of annotation files

const string C_ANNOT_EDT = " -C 'vcc edit -e oae ${file.name}'";
const string C_ANNOT_DTA = " -C 'vcc delta -e oae -u ${fname}'";
const string C_ANNOT_GET = " -C 'vcc get -e oae ${fname}'";

// cmd input/output redirection

const string REDIRECT_OUT = " > ${outfile} 2>&1";
const string REDIRECT_IN = " < ${infile}";
const string VCC_PREFIX = "vcc";

// suffix for tmpfiles (generate a unique one each time)

int vccSuffix = 0;

// Check out the current file

void
VccCheckOut()
{
    subproc_tp     proc;
    const string   cmd = C_PRG + C_EDT + REDIRECT_OUT;
    string         outfile = tmpfile(VCC_PREFIX, vccSuffix++);
    print_message("Executing Version Check Out...");
    proc.pid = editor_system(cmd, "vccDoneProc");
    proc.success_msg = "Successful Check Out In Progress.";
    proc.failure_msg = "Check Out Failed.";
    proc.tmpfile = NULL;
    proc.outfile = outfile;
    list_append(subProcs, proc);
}

// Check in the current file

void
VccCheckIn()
{
    print_message("Executing Version Check In...");

    if (buffer_is_modified() && !FileSaveAndCommit(True))
    {
	print_error("Version Check In Failed.");
	return;
    } else if (buffer_is_modified())
    {
	/*
	 * they chose not to save
	 */

	print_message("Version Check In Cancelled.");
	return;
    }
    editor_text_selector("", "VccCheckInCb();", 
			 "Version: Check In",
			 "Comments:",
			 "Check In",
			 "Cancel");
}

// Callback func for editor_text_selector...

void
VccCheckInCb()
{
    subproc_tp     proc;
    const string   cmd = C_PRG + C_DTA + C_GET + REDIRECT_IN + REDIRECT_OUT;
    string         infile = tmpfile(VCC_PREFIX, vccSuffix++);
    string         outfile = tmpfile(VCC_PREFIX, vccSuffix++);
    string	   comment;

    if (editor_text_selector_action() == "Cancel")
    {
	print_message("Version Check In Cancelled.");
	return;
    }
    comment = editor_text_selector_value();
    comment = list_to_string(string_to_list(comment, "\n"), "\\\n");
    write_file(infile, comment + "\n");
    proc.pid = editor_system(cmd, "vccDoneProc");
    proc.success_msg = "Check In Succeeded.";
    proc.failure_msg = "Check In Failed.";
    proc.tmpfile = infile;
    proc.outfile = outfile;
    list_append(subProcs, proc);
}

// Create initial version

void
VccCreate()
{
    subproc_tp     proc;
    const string   cmd = C_PRG + C_INI + C_CRT + REDIRECT_OUT;
    string         outfile = tmpfile(VCC_PREFIX, vccSuffix++);

    print_message("Creating Initial Version...");

    if (buffer_is_modified() && !FileSaveAndCommit(True))
    {
	print_error("Create Initial Version Failed.");
	return;
    } else if (buffer_is_modified())
    {
	/*
	 * they chose not to save
	 */

	print_message("Create Initial Version Cancelled.");
	return;
    }

    // if we are using RCS, then we need to get an initial string
    // here, so we need an extra pop-up
    string vcc_type = toolinfo_variable( "vcc_cmds" );
    if ( vcc_type != NULL &&
	 string_find( vcc_type, 0, "rcs" ) < string_length( vcc_type ))
      {
	editor_text_selector("", "VccCreateCb();", 
			     "Version: Create Initial Version",
			     "Comments:",
			     "Enter optional file description for RCS",
			     "Cancel");
      }
    else
      {
	proc.pid = editor_system(cmd, "vccDoneProc");
	proc.success_msg = "Create Initial Version Succeeded.";
	proc.failure_msg = "Create Initial Version Failed.";
	proc.tmpfile = NULL;
	proc.outfile = outfile;
	list_append(subProcs, proc);
      }
}


void
VccCreateCb()
{
    subproc_tp     proc;
    const string   cmd = C_PRG + C_INI + C_CRT + REDIRECT_IN + REDIRECT_OUT;
    string         infile = tmpfile(VCC_PREFIX, vccSuffix++);
    string         outfile = tmpfile(VCC_PREFIX, vccSuffix++);
    string	   comment;

    if (editor_text_selector_action() == "Cancel")
    {
	print_message("Version Create Cancelled.");
	return;
    }
    comment = editor_text_selector_value();
    comment = list_to_string(string_to_list(comment, "\n"), "\\\n");
    write_file(infile, comment + "\n");
    proc.pid = editor_system(cmd, "vccDoneProc");
    proc.success_msg = "Create Initial Version Succeeded.";
    proc.failure_msg = "Create Initial Version Failed.";
    proc.tmpfile = infile;
    proc.outfile = outfile;
    list_append(subProcs, proc);
}

// callback for subprocess completion

int
vccDoneProc(int pid, int status)
{
    // uncache the file object since it's version has likely changed

    FileReload();
    return(doneProc(pid, status));
}

// Cancel the check out

void
VccCancel()
{
    subproc_tp     proc;
    const string   cmd = C_PRG + C_CAN + REDIRECT_OUT;
    string         outfile = tmpfile(VCC_PREFIX, vccSuffix++);

    print_message("Executing Cancel Version Check Out...");
    proc.pid = editor_system(cmd, "vccDoneProc");
    proc.success_msg = "Cancel Check Out Succeeded.";
    proc.failure_msg = "Cancel Check Out Failed.";
    proc.tmpfile = NULL;
    proc.outfile = outfile;
    list_append(subProcs, proc);
}

