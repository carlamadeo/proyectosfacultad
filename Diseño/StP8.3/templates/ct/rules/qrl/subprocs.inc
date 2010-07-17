//////////////////////////////////////////////////////////////////////////////
//
// 	sccsid[] = %W% %Y% %D%
// 	StP/Core
// 	Confidential property of Interactive Development Environments, Inc.
// 	Copyright (c) 1992-1994
// 	All rights reserved
//
//	Subprocess handling via qrl function editor_system.
//
//////////////////////////////////////////////////////////////////////////////

// this structure is used to keep track of running subprocesses.  Each
// time one's started we throw one of these structures into a list and
// then when the callback arrives indicating it's completion we can
// look through the list and figure out what message to print, what
// files to delete etc.

struct subproc_tp
{
    int 	pid;			// process id
    string	failure_msg;		// message to print if it succeeds
    string	success_msg;		// message to print if it fails
    string	tmpfile;		// tmpfile to delete when it's done
    string	outfile;		// output to show in the log
    string	client_data;		// ignored here - use it for anything
};

list subProcs = list_create("subproc_tp", 0);

// Callback for when subprocess terminates

int
doneProc(int pid, int status)
{
    subproc_tp  proc;
    int         i;
    list        messages = list_create("string", 0);

    for (i = list_count(subProcs); i > 0; i = i - 1)
    {
        proc = list_get(subProcs, i - 1);

        if (proc.pid == pid)
        {
            list_delete(subProcs, i - 1);
	    break;
	}
    }
    if (proc.tmpfile != NULL)
    {
	delete_file(proc.tmpfile);
    }
    if (proc.outfile != NULL)
    {
	messages = string_to_list(read_file(proc.outfile), "\n");

	for (i = 0; i < list_count(messages); i = i + 1)
	{
	    print_message("    " + list_get(messages, i));
	}
	delete_file(proc.outfile);
    }
    if (status)
    {
	print_error(proc.failure_msg, NULL);
    } else
    {
	print_message(proc.success_msg);
    }
    return(0);
}

subproc_tp 
subProc(int pid)
{
    subproc_tp  proc;
    int         i;

    for (i = list_count(subProcs); i > 0; i = i - 1)
    {
        proc = list_get(subProcs, i - 1);

        if (proc.pid == pid)
	    return(proc);
    }
    return(NULL);
}

string
tmpfile(string prefix, int n)
{
    return( temp_dir() + prefix + "_" + hostname() + "_" + getpid() + "_" + n);
}


