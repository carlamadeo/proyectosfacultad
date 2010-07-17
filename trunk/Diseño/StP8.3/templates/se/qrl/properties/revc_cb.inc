//
//      StP/SE
//      Confidential property of Interactive Development Environments, Inc.
//      Copyright (c) 1995
//      All rights reserved
//

#include "qrl/properties/c_browse_cb.inc"
#include "rules/qrl/re_navigate.inc"

// callbacks for revc UI

struct sync_diff_tp
{
    int id0; 
    int id1; 
    int id2; 
    int id3; 
    int id4;
    int id5;
    string message;
};


list DiffList = list_create("sync_diff_tp", 0);
ui_image_tp LeftArrowImage = to_ui_image("arrow_left");
ui_image_tp RightArrowImage = to_ui_image("arrow_right");
ui_image_tp DoneImage = to_ui_image("check");
ui_image_tp NotDoneImage = to_ui_image("bad");

// Need a more general mechanism for this
const string STPEM_CL = "sce";



// differences

void
load_sync_diffs(string diffstr)

{
    string line;
    list diffl, line_list;
    sync_diff_tp sync_diff;
    int i;
    ui_prop diffprop;
    ui_list_item_tp item;
    list l = list_create("ui_list_item_tp", 0);

    if (diffstr == NULL || diffstr == "")
    {
	ui_footer_set_value(to_ui_prop("SyncDiffList", "Status"), "No synchronization differences found.");
	return;
    }

    diffprop = to_ui_prop("SyncDiffList", "Diffs");

    diffl = list_create("string", 0);
    line_list = list_create("string", 0);
    list_clear(DiffList);

    diffl = string_to_list(diffstr, "\n");

    ui_list_clear(diffprop);

    for (i = 0; i < list_count(diffl); i++)
    {
	line = list_get(diffl, i);
	line_list = string_to_list(line, " ");
	sync_diff.id0 = to_int(list_get(line_list, 0));
	sync_diff.id1 = to_int(list_get(line_list, 1));
	sync_diff.id2 = to_int(list_get(line_list, 2));
	sync_diff.id3 = to_int(list_get(line_list, 3));
	sync_diff.id4 = to_int(list_get(line_list, 4));
	sync_diff.id5 = to_int(list_get(line_list, 5));

	list_delete(line_list, 0);    
	list_delete(line_list, 0);    
	list_delete(line_list, 0);    
	list_delete(line_list, 0);    
	list_delete(line_list, 0);    
	list_delete(line_list, 0);    
	sync_diff.message = list_to_string(line_list, " ");
    
	list_append(DiffList, sync_diff);
	
	if (sync_diff.id0 == 0)
            item.image = LeftArrowImage;
	else if (sync_diff.id0 == 1)
            item.image = RightArrowImage;
	else if (sync_diff.id0 == 2)
            item.image = DoneImage;
	else
	    item.image = NotDoneImage;

        item.label = sync_diff.message;
	list_append(l, item);
    }

    if (list_count(l) > 0)
	ui_list_append(diffprop, l);

    if (list_count(diffl) == 1)
        ui_footer_set_value(to_ui_prop("SyncDiffList", "Status"), "1 synchronization difference found.");
    else
        ui_footer_set_value(to_ui_prop("SyncDiffList", "Status"), i + " synchronization differences found.");

    return;
}


// Browsing from the Sync Differences window.  At the functional level, shares the browsing 
// routines that the C Code browser uses.

int
RevCRunSyncCb(ui_event_tp event, ui_prop theprop, int id, string value)

{
    return RevCLoadSyncCb(event, theprop, id, value);
}


list BrowseFiles = list_create("file", 0);


int 
SyncBrowseDesign_cb(ui_event_tp event, ui_prop theprop, int id, string value)

{
    return SyncBrowse(True);
}



int 
SyncBrowseCode_cb(ui_event_tp event, ui_prop theprop, int id, string value)

{
    return SyncBrowse(False);
}

// browse callbacks
int 
SyncBrowse(boolean to_design)

{
    int row, i, num_items, smdbid;
    ui_prop prop, chooserprop;
    sync_diff_tp sync_diff;
    node_ref nr;
    list nrs, l;
    string cmdstr, msg, tf;
    file file;
    node node;
    ui_list_item_tp item;

    prop = to_ui_prop("SyncDiffList", "Diffs");
    l = ui_list_selected_items(prop);
    if (list_count(l) != 1)
    {
	print_error("Please select a single difference to browse");
	return 0; 
    }
    row = ui_list_selected_row(prop);

    if (row < ui_list_num_items(prop) && row >= 0)
    {
	sync_diff = list_get(DiffList, row);

	if (to_design)
	{
	    if (sync_diff.id2 == -1)
	    {
	        print_error("No StP Repository Object for this Difference.");
		// change the image to a check mark
		//item.image = DoneImage;
		//item.label = sync_diff.message;    
		//ui_list_delete(prop, row);
		//ui_list_insert(prop, row, item);
	        return 0;
	    }

	    nrs = list_select("node_ref[node_id = " + sync_diff.id2 + "]");
	    navigate_to_editor(nrs);
	}
	else
	{
	    smdbid = sync_diff.id3;
	    if (smdbid == -1)
	    {
		if (sync_diff.id5 == -1)
		{
	            print_error("No StP Semantic Model Object for this Difference.");
	            return 0;
		}
		else 
		{
	            print_error("No StP Semantic Model Object for this Difference, going to parent.");
		    smdbid = sync_diff.id5;
		}
	    }

	    // call xrb_search with this smdb_id as the key.  Based on the number of the diff message,
            // use the proper -T argument. These numbers are defined in revc/Sync/re_diffs.h

            cmdstr = "xrb_search -forward_messages " + editor_get_msgd_handle() + " -p " + current_projdir() + " -s " + current_system();
	    if (sync_diff.id1 >= 0 && sync_diff.id1 <= 6)
		cmdstr = cmdstr + " -T func";
	    else if (sync_diff.id1 >= 10 && sync_diff.id1 <= 11)
                cmdstr = cmdstr + " -T datas";
	    else if (sync_diff.id1 >= 12 && sync_diff.id1 <= 14)
                cmdstr = cmdstr + " -T datam";
	    else if (sync_diff.id1 >= 18 && sync_diff.id1 <= 20)
                cmdstr = cmdstr + " -T glob";
	    else if (sync_diff.id1 == 23 )
		cmdstr = cmdstr + " -T datas";
	    else if (sync_diff.id1 == 24 )
		cmdstr = cmdstr + " -T datam";
	    else
	        cmdstr = cmdstr + " -T id";

            cmdstr = cmdstr + " -key " + smdbid;
            cmdstr = cmdstr + " -def -ref";
	    tf = temp_file("flist", "se");
	    cmdstr = cmdstr + " -of " + tf;
 
	    if ( system(cmdstr) == 0 )
	      execute_chooser(tf, to_ui_prop("SyncDiffList", "Status"), 0);
	    delete_file(tf);
	} 
    }
    return 0;
}


// saving and removing differences

int 
SyncSaveDiffs_cb(ui_event_tp event, ui_prop theprop, int id, string value)

{
    string diffs;
    diffs = save_diffs(-1);

    return 0; 
}

	// a local variable that gets set when items are deleted from
	// the list

int sync_diffs_changed = 0;

// for to_check:
// -1: leave alone
//  0: 0 --> 2 (right arrows)
//  1: 1 --> 2 (left arrows)
//  2: 0,1 --> 2 (all)
string
save_diffs(int to_check)

{
    string s = "", fname = path_compose(current_projdir(),
					current_system(),
					"revc_files",
					"Differences." + user());
    int i, direction;
    sync_diff_tp diff;

    sync_diffs_changed = 0;

    // it is not so easy to rebuild the string, because these are structures, not lists of strings

    for (i = 0; i < list_count(DiffList); i++)
    {
	diff = list_get(DiffList, i);

	if (to_check == -1)
	    direction = diff.id0;
	else if (to_check == 0 && diff.id0 == 0)
	    direction = 2;
	else if (to_check == 1 && diff.id0 == 1)
	    direction = 2;
	else if (to_check = 2)
	    direction = 2;
	else
	    direction = diff.id0;

	s = s + direction + " " + diff.id1 + " " + diff.id2 + " " + diff.id3 + " " + diff.id4 + " " + 
            diff.id5 + " " + diff.message + "\n";
    }

    if (file_exists(fname) && (! write_file_access(fname)))
    {
	print_error("Cannot write file " + fname + " (check permissions). Differences not saved.");
	ui_footer_set_value(to_ui_prop("SyncDiffList", "Status"), "Cannot write file " + fname + " (check permissions). Differences not saved.");
    }
    else
    {
	write_file(fname, s);
	print_message("Differences saved.");
	ui_footer_set_value(to_ui_prop("SyncDiffList", "Status"), "Differences saved.");
    }

    return s;
}



int
SyncRemoveDiffs_cb(ui_event_tp event, ui_prop theprop, int id, string value)
 
{
    int i, delno;
    list selected;
    ui_prop prop = to_ui_prop("SyncDiffList", "Diffs");

    selected = ui_list_selected(prop);

    // delete these from the master list
    for (i = 0; i < list_count(selected); i++)
    {
	delno = list_get(selected, i); 
	list_delete(DiffList, delno - i);
    }

    // delete these from the UI
    if (list_count(selected) > 0)
	ui_list_delete(prop, selected);

    // set a flag to show that things have changed
    sync_diffs_changed = 1;

    return 0;
}



//[-f <abs filename>] [-no_adds] [-no_dels]

int
do_synchronize(string opt, int to_check)

{
    string cmdstr;
    string fname;

    // if there are changes, then do a save first
    if ( sync_diffs_changed == 1 )
    {
	save_diffs(-1);
    }
    
    fname = path_compose(current_projdir(),
			 current_system(),
			 "revc_files",
			 "Differences." + user());
    cmdstr = "synchronize -forward_messages " + editor_get_msgd_handle() +
	     " -p " + current_projdir() + " -s " + current_system() +
	     " -f " + fname + " " + opt;

    ui_busy(to_ui_owner(to_ui_prop("SyncDiffList", "Diffs")), True);


    // call syncronize
    if (is_windows_platform())
        system( cmdstr );
    else
        system( "sh -c \"" + cmdstr + " & \"" );

    return 0;
}


// this gets called from the synchronize program itself when it has finished
//
int
done_synchronize()
{
    // and redraw them
    string s = read_file( path_compose(current_projdir(),
				current_system(),
				"revc_files",
				"Differences." + user()));

    load_sync_diffs( s );
    ui_busy(to_ui_owner(to_ui_prop("SyncDiffList", "Diffs")), False);

    // tell them it is all over
    ui_footer_set_value(to_ui_prop("SyncDiffList", "Status"), "Synchronization Updates Completed.");

    return 0;
}


int
SyncDesignAdd_cb(ui_event_tp event, ui_prop theprop, int id, string value)
 
{
    return do_synchronize("-no_dels", 0);
}

int
SyncDesignDelete_cb(ui_event_tp event, ui_prop theprop, int id, string value)
 
{
    return do_synchronize("-no_adds", 1);
}

int
SyncDesignAddDelete_cb(ui_event_tp event, ui_prop theprop, int id, string value)
 
{
    return do_synchronize("", 2);
}

    



int
DiagramOptionsReset_cb(ui_event_tp event, ui_prop theprop, int id, string v)

{
    string fname, opt, val;
    ui_prop prop;
    int i, ival; 
    boolean b = False, g = False, l = False, gs = False, w = False, h = False, c = False, 
        Iu = False, Is = False, arc = False, ncalls = False, source = False, ncomments = False,
	scale = False, xscale = False, yscale = False;
    list lopts;

    // first set to baseline, then reset based on previous diagram options.

    fname = path_compose(current_projdir(),
			 current_system(),
			 "revc_files",
			 "DiagramOptions");

    if (read_file_access(fname))
    {
	opt = read_file(fname);

	lopts = string_to_list(opt, " =\"");
	for (i = 0; i < list_count(lopts); i++)
	{
	    opt = list_get(lopts, i);
	    if (opt == "-ant_source")
	    {
		ui_choice_set_value(to_ui_prop("DiagramOptionsSet", "GenBody"), 1);
		b = True;
		continue; 
	    }
	    if (opt == "-sce_noglobals")
	    {
		ui_choice_set_value(to_ui_prop("DiagramOptionsSet", "IncludeGlobals"), 0);
		g = True;
		continue; 
	    }
	    if (opt == "-sce_nolibs")
	    {
		i++;
		ival = to_int( list_get(lopts, i));
		ui_choice_set_value(to_ui_prop("DiagramOptionsSet", "IncludeLibraries"), ival);
		l = True;
		continue; 
	    }
	    if (opt == "-sce_sawtooth")
	    {
		ui_choice_set_value(to_ui_prop("DiagramOptionsSet", "GenSawtooth"), 1);
		gs = True;
		continue; 
	    }
	    if (opt == "-sce_width")
	    {
		i++;
		opt = list_get(lopts, i);
		w = True;

		if (opt == "5")
		    ui_choice_set_value(to_ui_prop("DiagramOptionsSet", "SCEWidth"), 0);
		if (opt == "8")
		    ui_choice_set_value(to_ui_prop("DiagramOptionsSet", "SCEWidth"), 1);
		if (opt == "10")
		    ui_choice_set_value(to_ui_prop("DiagramOptionsSet", "SCEWidth"), 2);
		if (opt == "12")
		    ui_choice_set_value(to_ui_prop("DiagramOptionsSet", "SCEWidth"), 3);
		if (opt == "15")
		    ui_choice_set_value(to_ui_prop("DiagramOptionsSet", "SCEWidth"), 4);
		if (opt == "18")
		    ui_choice_set_value(to_ui_prop("DiagramOptionsSet", "SCEWidth"), 5);
		if (opt == "20")
		    ui_choice_set_value(to_ui_prop("DiagramOptionsSet", "SCEWidth"), 6);
		if (opt == "25")
		    ui_choice_set_value(to_ui_prop("DiagramOptionsSet", "SCEWidth"), 7);
		if (opt == "30")
		    ui_choice_set_value(to_ui_prop("DiagramOptionsSet", "SCEWidth"), 8);
		if (opt == "40")
		    ui_choice_set_value(to_ui_prop("DiagramOptionsSet", "SCEWidth"), 9);
		if (opt == "50")
		    ui_choice_set_value(to_ui_prop("DiagramOptionsSet", "SCEWidth"), 10);
		if (opt == "75")
		    ui_choice_set_value(to_ui_prop("DiagramOptionsSet", "SCEWidth"), 11);
		if (opt == "100")
		    ui_choice_set_value(to_ui_prop("DiagramOptionsSet", "SCEWidth"), 12);
		continue; 
	    }
	    if (opt == "-sce_height")
	    {
		i++;
		opt = list_get(lopts, i);
		h = True;

		if (opt == "1")
		    ui_choice_set_value(to_ui_prop("DiagramOptionsSet", "SCEHeight"), 0);
		if (opt == "2")
		    ui_choice_set_value(to_ui_prop("DiagramOptionsSet", "SCEHeight"), 1);
		if (opt == "3")
		    ui_choice_set_value(to_ui_prop("DiagramOptionsSet", "SCEHeight"), 2);
		if (opt == "4")
		    ui_choice_set_value(to_ui_prop("DiagramOptionsSet", "SCEHeight"), 3);
		if (opt == "5")
		    ui_choice_set_value(to_ui_prop("DiagramOptionsSet", "SCEHeight"), 4);
		if (opt == "6")
		    ui_choice_set_value(to_ui_prop("DiagramOptionsSet", "SCEHeight"), 5);
		if (opt == "7")
		    ui_choice_set_value(to_ui_prop("DiagramOptionsSet", "SCEHeight"), 6);
		if (opt == "8")
		    ui_choice_set_value(to_ui_prop("DiagramOptionsSet", "SCEHeight"), 7);
		if (opt == "9")
		    ui_choice_set_value(to_ui_prop("DiagramOptionsSet", "SCEHeight"), 8);
		if (opt == "10")
		    ui_choice_set_value(to_ui_prop("DiagramOptionsSet", "SCEHeight"), 9);
		continue;
	    }
	    if (opt == "-sce_controls")
	    {
		i++;
		opt = list_get(lopts, i);
		c = True;

		if (opt == "0")
		    ui_choice_set_value(to_ui_prop("DiagramOptionsSet", "SCEControlLevels"), 0);
		if (opt == "1")
		    ui_choice_set_value(to_ui_prop("DiagramOptionsSet", "SCEControlLevels"), 1);
		if (opt == "2")
		    ui_choice_set_value(to_ui_prop("DiagramOptionsSet", "SCEControlLevels"), 2);
		if (opt == "3")
		    ui_choice_set_value(to_ui_prop("DiagramOptionsSet", "SCEControlLevels"), 3);
		if (opt == "4")
		    ui_choice_set_value(to_ui_prop("DiagramOptionsSet", "SCEControlLevels"), 4);
		if (opt == "5")
		    ui_choice_set_value(to_ui_prop("DiagramOptionsSet", "SCEControlLevels"), 5);
		if (opt == "6")
		    ui_choice_set_value(to_ui_prop("DiagramOptionsSet", "SCEControlLevels"), 6);
		if (opt == "7")
		    ui_choice_set_value(to_ui_prop("DiagramOptionsSet", "SCEControlLevels"), 7);
		if (opt == "8")
		    ui_choice_set_value(to_ui_prop("DiagramOptionsSet", "SCEControlLevels"), 8);
		if (opt == "9")
		    ui_choice_set_value(to_ui_prop("DiagramOptionsSet", "SCEControlLevels"), 9);
		if (opt == "10")
		    ui_choice_set_value(to_ui_prop("DiagramOptionsSet", "SCEControlLevels"), 10);
		continue;
	    }
	    if (opt == "-sce_xscale")
	    {
		i++;
		ival = to_int( list_get(lopts, i));
		ui_choice_set_value(to_ui_prop("DiagramOptionsSet", "XScale"), ival);
		xscale = True;
		continue;
	    }
	    if (opt == "-sce_yscale")
	    {
		i++;
		ival = to_int( list_get(lopts, i));
		ui_choice_set_value(to_ui_prop("DiagramOptionsSet", "YScale"), ival);
		yscale = True;
		continue;
	    }

	    // DSE
	    if (opt == "-dse_showall")
	    {
		ui_choice_set_value(to_ui_prop("DiagramOptionsSet", "IgnoreUnused"), 1);
		Iu = True;
		continue;
	    }
	    if (opt == "-dse_nosystem")
	    {
		ui_choice_set_value(to_ui_prop("DiagramOptionsSet", "IgnoreSysStructs"), 0);
		Is = True;
		continue;
	    }
	    if (opt == "-dse_arc")
	    {
		i++;
		ival = to_int( list_get(lopts, i));
		if (ival == 10000)
		    ui_choice_set_value(to_ui_prop("DiagramOptionsSet", "ArcKids"), 0);
		else if (ival == 0)
		    ui_choice_set_value(to_ui_prop("DiagramOptionsSet", "ArcKids"), 2);
		else
		    ui_choice_set_value(to_ui_prop("DiagramOptionsSet", "ArcKids"), 1);
		arc = True;
		continue;
	    }
            if (opt == "-dse_scale")
            {
                i++;
                ival = to_int( list_get(lopts, i));
                ui_choice_set_value(to_ui_prop("DiagramOptionsSet", "Scale"), ival);
                scale = True;
                continue;
            }

	    // FCE
	    if (opt == "-fce_nocalls")
	    {
		ui_choice_set_value(to_ui_prop("DiagramOptionsSet", "ShowCalls"), 0);
		ncalls = True;
		continue;
	    }
	    if (opt == "-fce_source")
	    {
		i++;
		ival = to_int( list_get(lopts, i));
		ui_choice_set_value(to_ui_prop("DiagramOptionsSet", "ShowSource"), ival);
		source = True;
		continue;
	    }
	    if (opt == "-fce_nocomments")
	    {
		ui_choice_set_value(to_ui_prop("DiagramOptionsSet", "ShowComments"), 0);
		ncomments = True;
		continue;
	    }
	}
        if (! b)
    	    ui_choice_set_value(to_ui_prop("DiagramOptionsSet", "GenBody"), 0);
	if (! g)    
	    ui_choice_set_value(to_ui_prop("DiagramOptionsSet", "IncludeGlobals"), 1);
	if (! l)    
	    ui_choice_set_value(to_ui_prop("DiagramOptionsSet", "IncludeLibraries"), 0);
	if (! gs)    
	    ui_choice_set_value(to_ui_prop("DiagramOptionsSet", "GenSawtooth"), 0);
	if (! w)    
	    ui_choice_set_value(to_ui_prop("DiagramOptionsSet", "SCEWidth"), 7);
	if (! h)    
	    ui_choice_set_value(to_ui_prop("DiagramOptionsSet", "SCEHeight"), 2);
	if (! c)    
	    ui_choice_set_value(to_ui_prop("DiagramOptionsSet", "SCEControlLevels"), 3);
	if (! Iu)    
	    ui_choice_set_value(to_ui_prop("DiagramOptionsSet", "IgnoreUnused"), 0);
	if (! Is)    
	    ui_choice_set_value(to_ui_prop("DiagramOptionsSet", "IgnoreSysStructs"), 1);
	if (! arc)    
	    ui_choice_set_value(to_ui_prop("DiagramOptionsSet", "ArcKids"), 1);
	if (! ncalls)    
	    ui_choice_set_value(to_ui_prop("DiagramOptionsSet", "ShowCalls"), 1);
	if (! source)    
	    ui_choice_set_value(to_ui_prop("DiagramOptionsSet", "ShowSource"), 2);
	if (! ncomments)    
	    ui_choice_set_value(to_ui_prop("DiagramOptionsSet", "ShowComments"), 1);
	if (! scale)    
	    ui_choice_set_value(to_ui_prop("DiagramOptionsSet", "Scale"), 1);
	if (! xscale)    
	    ui_choice_set_value(to_ui_prop("DiagramOptionsSet", "XScale"), 1);
	if (! yscale)    
	    ui_choice_set_value(to_ui_prop("DiagramOptionsSet", "YScale"), 1);
    }
    else
    {
	ui_choice_set_value(to_ui_prop("DiagramOptionsSet", "GenBody"), 1);
	ui_choice_set_value(to_ui_prop("DiagramOptionsSet", "IncludeGlobals"), 1);
	ui_choice_set_value(to_ui_prop("DiagramOptionsSet", "IncludeLibraries"), 0);
	ui_choice_set_value(to_ui_prop("DiagramOptionsSet", "GenSawtooth"), 1);
	ui_choice_set_value(to_ui_prop("DiagramOptionsSet", "SCEWidth"), 7);
	ui_choice_set_value(to_ui_prop("DiagramOptionsSet", "SCEHeight"), 2);
	ui_choice_set_value(to_ui_prop("DiagramOptionsSet", "SCEControlLevels"), 3);
	ui_choice_set_value(to_ui_prop("DiagramOptionsSet", "IgnoreUnused"), 0);
	ui_choice_set_value(to_ui_prop("DiagramOptionsSet", "IgnoreSysStructs"), 0);
	ui_choice_set_value(to_ui_prop("DiagramOptionsSet", "ArcKids"), 1);
	ui_choice_set_value(to_ui_prop("DiagramOptionsSet", "ShowCalls"), 1);
	ui_choice_set_value(to_ui_prop("DiagramOptionsSet", "ShowSource"), 2);
	ui_choice_set_value(to_ui_prop("DiagramOptionsSet", "ShowComments"), 1);
	ui_choice_set_value(to_ui_prop("DiagramOptionsSet", "XScale"), 1);
	ui_choice_set_value(to_ui_prop("DiagramOptionsSet", "YScale"), 1);
	ui_choice_set_value(to_ui_prop("DiagramOptionsSet", "Scale"), 1);
    }

    return 0;
}


void
OLDdiagram_options_env_set()

{
    string fname, str;

    fname = path_compose(current_projdir(),
			 current_system(),
			 "revc_files",
			 "DiagramOptions");

    if (read_file_access(fname))
    {
	str = read_file(fname);
        put_environment_variable("SE_DIAGRAM_OPTS=" + str);
    }

    return;
}



int
DiagramOptionsSet_cb(ui_event_tp event, ui_prop theprop, int id, string v)

{
    ui_prop prop;
    string s = "SE_DIAGRAM_OPTS=\"", fname;
    int i, rc;

    if (ui_button_is_ok(theprop))
	rc = 1;
    else
	rc = 0;

    // SCE

    if (ui_choice_isset(to_ui_prop("DiagramOptionsSet", "GenBody"), 0))
    s += "-ant_source ";

    if (!ui_choice_isset(to_ui_prop("DiagramOptionsSet", "IncludeGlobals"), 0))
	s += "-sce_noglobals ";

    prop = to_ui_prop("DiagramOptionsSet", "IncludeLibraries");
    s += "-sce_nolibs " + ui_choice_value(prop) + " ";

    if (ui_choice_isset(to_ui_prop("DiagramOptionsSet", "GenSawtooth"), 0))
	s += "-sce_sawtooth ";

    prop = to_ui_prop("DiagramOptionsSet", "SCEWidth");
    i = ui_choice_value(prop);
    if (i == 0)
        s += "-sce_width 5 ";
    else if (i == 1)
        s += "-sce_width 8 ";
    else if (i == 2)
        s += "-sce_width 10 ";
    else if (i == 3)
        s += "-sce_width 12 ";
    else if (i == 4)
        s += "-sce_width 15 ";
    else if (i == 5)
        s += "-sce_width 18 ";
    else if (i == 6)
        s += "-sce_width 20 ";
    else if (i == 7)
        s += "-sce_width 25 ";
    else if (i == 8)
        s += "-sce_width 30 ";
    else if (i == 9)
        s += "-sce_width 40 ";
    else if (i == 10)
        s += "-sce_width 50 ";
    else if (i == 11)
        s += "-sce_width 75 ";
    else if (i == 12)
        s += "-sce_width 100 ";

    prop = to_ui_prop("DiagramOptionsSet", "SCEHeight");
    i = ui_choice_value(prop);
    if (i == 0)
        s += "-sce_height 1 ";
    else if (i == 1)
        s += "-sce_height 2 ";
    else if (i == 2)
        s += "-sce_height 3 ";
    else if (i == 3)
        s += "-sce_height 4 ";
    else if (i == 4)
        s += "-sce_height 5 ";
    else if (i == 5)
        s += "-sce_height 6 ";
    else if (i == 6)
        s += "-sce_height 7 ";
    else if (i == 7)
        s += "-sce_height 8 ";
    else if (i == 8)
        s += "-sce_height 9 ";
    else if (i == 9)
        s += "-sce_height 10 ";
   
    prop = to_ui_prop("DiagramOptionsSet", "SCEControlLevels");
    i = ui_choice_value(prop);
    if (i == 0)
        s += "-sce_controls 0 ";
    else if (i == 1)
        s += "-sce_controls 1 ";
    else if (i == 2)
        s += "-sce_controls 2 ";
    else if (i == 3)
        s += "-sce_controls 3 ";
    else if (i == 4)
        s += "-sce_controls 4 ";
    else if (i == 5)
        s += "-sce_controls 5 ";
    else if (i == 6)
        s += "-sce_controls 6 ";
    else if (i == 7)
        s += "-sce_controls 7 ";
    else if (i == 8)
        s += "-sce_controls 8 ";
    else if (i == 9)
        s += "-sce_controls 9 ";
    else if (i == 10)
        s += "-sce_controls 10 ";

    prop = to_ui_prop("DiagramOptionsSet", "XScale");
    s += "-sce_xscale " + ui_choice_value(prop) + " ";

    prop = to_ui_prop("DiagramOptionsSet", "YScale");
    s += "-sce_yscale " + ui_choice_value(prop) + " ";

    // DSE

    if (ui_choice_isset(to_ui_prop("DiagramOptionsSet", "IgnoreUnused"), 0))
	s += "-dse_showall ";

    if (! ui_choice_isset(to_ui_prop("DiagramOptionsSet", "IgnoreSysStructs"), 0))
	s += "-dse_nosystem ";

    prop = to_ui_prop("DiagramOptionsSet", "ArcKids");
    i = ui_choice_value(prop);
    if (i == 0)
        s += "-dse_arc 10000 ";
    else if (i == 1)
        s += "-dse_arc 12 ";
    else
        s += "-dse_arc 0 ";
 
    prop = to_ui_prop("DiagramOptionsSet", "Scale");
    s += "-dse_scale " + ui_choice_value(prop) + " ";

    // FCE

    if (! ui_choice_isset(to_ui_prop("DiagramOptionsSet", "ShowCalls"), 0))
	s += "-fce_nocalls ";

    prop = to_ui_prop("DiagramOptionsSet", "ShowSource");
    s += "-fce_source " + ui_choice_value(prop) + " ";

    if (!ui_choice_isset(to_ui_prop("DiagramOptionsSet", "ShowComments"), 0))
	s += "-fce_nocomments ";

    s += "\"";

    fname = path_compose(current_projdir(),
			 current_system(),
			 "revc_files",
			 "DiagramOptions");
    if (file_exists(fname) && (! write_file_access(fname)))
	print_error("Cannot write file " + fname + " (check permissions).  Diagram generation will not be correct.");
    else
        write_file(fname, s);

    return rc;
}



 
int
DiagramOptionsShow_cb(ui_event_tp event, ui_prop theprop, int id, string v)
 
{
    ui_prop_sheet_pop_up("DiagramOptionsSet");
    DiagramOptionsReset_cb(event, theprop, id, v);

    return 0;
}

// the SyncSetOpts_cb pops up the SyncDiffList and
// the command specified, gense, is executed.
// gense creates  a diff file and calls back the SyncSetOpts_extend

string se_revc_sync_msgf;
string se_revc_msgf;
ui_prop se_revc_prop;

int
SyncSetOpts_cb(ui_event_tp event, ui_prop theprop, int id, string value)
 
{
    string optstr = "SE_FILE_OPTS=\"", cmdstr, syncfile, syncoptsfile;
    int rc = 0;

    ui_footer_set_value(to_ui_prop("SyncDiffList", "Status"), "");

    // first be certain that a sync is not already running
    syncfile = path_compose( current_projdir(),
			     current_system(), ".SE_gen" );

    if (read_file_access(syncfile))
    {
	print_error("The SE Synchronizer appears to be already running on this system.");
	print_error("If this is not the case, remove " + syncfile + " and try again.");
	write_file(syncfile, "rc=-1" );
	return rc;
    }

    ui_prop_sheet_pop_up("SyncDiffList");
    //ui_footer_set_value(to_ui_prop("SyncDiffList", "Status"), "Searching for differences...");
    print_message("Searching for differences...");
    ui_list_clear(to_ui_prop("SyncDiffList", "Diffs"));

    ui_busy(to_ui_owner(to_ui_prop("SyncDiffList", "Diffs")), True);
    se_revc_sync_msgf = se_revc_msgf = path_compose(current_projdir(),
				    current_system(),
				    "revc_files",
				    "Differences." + user());

    se_revc_prop = to_ui_prop("RESynchronizer", "Operation");
    if (ui_choice_value(se_revc_prop) != 0)
    {
	ui_prop focus = to_ui_prop("RESynchronizer", "PartialSync");
	if ( ui_choice_value(focus) != 0 )
	    optstr += get_focus_file();

	se_revc_sync_msgf = temp_file("sync", "se");
	optstr += " -output_file " + se_revc_sync_msgf + "\"" ;

        syncoptsfile = path_compose(current_projdir(),
				    current_system(),
				    "revc_files",
				    "SyncOptions");

        if (file_exists(syncoptsfile) && (! write_file_access(syncoptsfile)))
	    print_error("Cannot write file " + syncoptsfile + " (check permissions).  Syncronization results will not be correct.");
        else
	    write_file(syncoptsfile, optstr);

        print_message("Synchronization difference gathering in progress...");

    }
    write_file(syncfile, "rc=0" );
    return 1;
}

void
SyncSetOpts_extend()
{
    string msgs;
    if (! read_file_access(se_revc_sync_msgf))
    {
	//ui_footer_set_value(to_ui_prop("SyncDiffList", "Status"), "No synchronization differences found.");
	print_message("No synchronization differences found.");
    }
    else
    {
        msgs = read_file(se_revc_sync_msgf);

        // clean up the temp file
        if (ui_choice_value(se_revc_prop) != 0)
        {
            write_file(se_revc_msgf, msgs);
            delete_file(se_revc_sync_msgf);
        }
        if (msgs == "")
	    print_message("No synchronization differences found.");
        else
           load_sync_diffs(msgs);
    }

    
    ui_busy(to_ui_owner(to_ui_prop("SyncDiffList", "Diffs")), False);

}

int
GenDiag_cb(ui_event_tp event, ui_prop theprop, int id, string value)
 
{
    int rc;
    string filename = path_compose( current_projdir(),
				    current_system(), ".SE_gen" );
 

    rc = 0;
    if ( ui_choice_value( to_ui_prop("REGen", "StructureCharts")) == 0 &&
	 ui_choice_value( to_ui_prop("REGen", "DataStructureCharts")) == 0 &&
	 ui_choice_value( to_ui_prop("REGen", "FlowCharts")) == 0 )
    {
	print_error( "echo No diagram types selected, nothing to do." );
	write_file( filename, "rc=-1" );
	return rc;
    }

    if ( toolinfo_variable( "ide_qa_force" ) == NULL )
    {
	if ( editor_confirm( "Generate New Model will lose all information\nin the current system.\n\nAre you sure you want to proceed?", "Continue", "Cancel" ) == True )
        {
	    write_file(filename, "rc=0" );
            rc = 1;
        }
	else
        {
	    write_file(filename, "rc=-1" );
            rc = 0;
        }
    }
    else
        write_file(filename, "rc=0" );

    return rc;
}

