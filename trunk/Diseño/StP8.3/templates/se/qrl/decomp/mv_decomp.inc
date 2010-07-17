//
//      sccsid[] = %W% %Y% %D%
//      StP/SE
//      Confidential property of Interactive Development Environments, Inc.
//      Copyright (c) 1995
//      All rights reserved
//
// mv_decomp.inc: script to move a decomposition from one file name to another.  

//
// Move the decomposition specified.  If Both files have not been specified, fail. If there is 
// a pre-existing file in the way, fail.
//

// create a string list of all the decomps for this node.  Create the list in bottom up decompsition order,
// as those are the decomps that will need to be dealt with first, to avoid overwriting other decomps.

string SteDiags = "";
string CseDiags = "";

string RenameOpts = "";
boolean FromMain = False;

list
get_ordered_decomp_list(file f, boolean up)

{
    int i;
    node node;
    list nodel, curl, l;
    item item;
    file nextfile;
    string nextname;

    curl = list_create("string", 0);

    if (f == NULL)
	return curl;
    else
    {
	// see if there are cspecs or ste files	
	nextfile = find_by_query("file[SteDiagram && name = '${f.name}']");
	if (nextfile != NULL)
	    SteDiags = SteDiags + nextfile.name + " ";
			
	nextfile = find_by_query("file[CspecTable && name = '${f.name}']");
	if (nextfile != NULL)
	    CseDiags = CseDiags + nextfile.name + " ";

	nodel = list_select("node[Process && node_refs[file_id = ${f.id}]]");
	for (i = 0; i < list_count(nodel); i++)
	{
	    node = list_get(nodel, i);
	    item = find_by_query("item[RelativeIndex && obj_id = ${node.id}]");
	    if (item != NULL && item.value != NULL && item.value != "")
	    {
		if (f.name == "0")
		    nextname = item.value;
		else
		    nextname = f.name + "." + item.value;
		nextfile = find_by_query("file[DfeDiagram && name = '${nextname}']");
		l = get_ordered_decomp_list(nextfile, up);
		if (l != NULL)
		{
		    list_concatenate(l, curl);
		    curl = l;
		}

	    }
	}
	if (up)
	{
	    if (list_count(curl) == 0)
                list_append(curl, f.name);
            else
                list_insert(curl, 0, f.name);
        }
	else
            list_append(curl, f.name);
    }
    return curl;
}




void
really_move_decomps(list decomps, string ofile, string nfile, boolean quiet)
{
    int i;
    string command = "", ste_command = "", cse_command = "";
    string thisf, tof, s;
    int stripstart;
    list ste_l, cse_l;

    ste_l = string_to_list(SteDiags, " ");
    cse_l = string_to_list(CseDiags, " ");

    for (i = 0; i < list_count(decomps); i++)
    {
    	thisf = list_get(decomps, i);
	stripstart = string_length(ofile);
	if (ofile == "0" && thisf == "0")
	    tof = nfile;
	else if (ofile == "0")
	    tof = nfile + "." + thisf;
	else if (string_length(thisf) - stripstart == 0)
	    tof = nfile;
	else if (nfile == "0")
	    tof = string_extract(thisf, stripstart + 1, string_length(thisf) - stripstart - 1);
	else
	    tof = nfile + string_extract(thisf, stripstart, string_length(thisf) - stripstart);

    	// move this decomp
	print_message("Moving Dfe Diagram `" + thisf + "' to `" + tof + "'...");
    	command = command + " -C 'rename " + thisf + " " + tof + "'";

	if (list_find(ste_l, 0, thisf) != list_count(ste_l))  
	    ste_command = ste_command + " -C 'rename " + thisf + " " + tof + "'";

	if (list_find(cse_l, 0, thisf) != list_count(cse_l))  
	    cse_command = cse_command + " -C 'rename " + thisf + " " + tof + "'";
    }

    // dfe
    command = "stputil -p " + current_projdir() + " -s " + current_system() + " " + RenameOpts 
        + " -C 'e " + "dydfe" + "' -C 'force on' " + command + " -C 'quit'";

    if (! quiet)
        print_message(command);
    system(command);

    // ste
    if (ste_command != "")
    {
	command = "stputil -p " + current_projdir() + " -s " + current_system() + " " + RenameOpts 
            + " -C 'e " + "ste" + "' -C 'force on' " + ste_command + " -C 'quit'";

	if (! quiet)
	    print_message(command);
	system(command);
    }

    // cse
    if (cse_command != "")
    {
	command = "stputil -p " + current_projdir() + " -s " + current_system() + " " + RenameOpts 
            + " -C 'e " + "cse" + "' -C 'force on' " + cse_command + " -C 'quit'";

	if (! quiet)
	    print_message(command);
	system(command);
    }

    return;
}





int
do_mv_decomp(file oms_ofile, string nfile, boolean quiet, boolean move_up)

{
    list decompl;

    if (! FromMain)
    {
	RenameOpts = "-Force -Quiet";
	if (! quiet)
	    RenameOpts = RenameOpts + " -verbose";
    }

    decompl = get_ordered_decomp_list(oms_ofile, move_up);
    really_move_decomps(decompl, oms_ofile.name, nfile, quiet);

    return 0;
}

