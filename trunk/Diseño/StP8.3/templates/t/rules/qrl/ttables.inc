//
//      sccsid[] = %W% %Y% %D%
//      StP/T
//      Confidential property of Interactive Development Environments, Inc.
//      Copyright (c) 1994
//      All rights reserved
//
 
// common functions for the two T table editors.
// 

#include "rules/qrl/gte_qrl_std.inc"



 
void
TTestTableInit(string fname, string tname)
 
{
    if (fname != "")
        gte_stpem("FileLoad " + fname);

    EditorShowOnNavReceive(); 

    if (! T_find_and_highlight_item(tname))
    {
	// if the first selection is blank
	if (gte_get_cell_label(3, 1) == "")
            T_add_name_to_table(tname,  False);
	else
            T_add_name_to_table(tname,  True);
    }
 
    return;
 
} // end of TTestTableInit()
 


void 
T_highlight_testunit(string name)

{
    TTestTableInit("", name);
    return;
}



void 
T_LoadAndFind(string filename, string name)

{
    TTestTableInit(filename, name);
    return;
}


void
T_add_name_to_table(string name, boolean addrow)
 
{
    gte_disable_refresh();

    if (addrow)
        gte_insert_row(3, 1);

    CoordCellLabel(3, 1, name);

    if (! T_find_and_highlight_item(name))
    {
	gte_enable_refresh();
	gte_refresh_table();
    }
 
    return;
 
} // end of T_add_name_to_table()
 
 


boolean 
T_find_and_highlight_item(string name)

{
    gte_stpem("BufferFindOpt -c -i +l +m +r");
    if (gte_find("^" + name + "$", gte_get_first_row(3), 1, 
                 gte_get_last_row(3), 1)) 
    {
	gte_set_selection(gte_get_current_cell_row(), 1, gte_get_current_cell_row(), 1);
	gte_enable_refresh();
	gte_refresh_table();
	return True;
    }
    else
	return False;
	
} // end of T_find_and_highlight_item() 





void 
T_delete_testunit_row(string name)
 
{
    gte_disable_refresh();
    // this gobbledegook searches the first column only
    gte_stpem("BufferFindOpt -c -i +l +m +r");
    if (gte_find("^" + name + "$", gte_get_first_row(3), 1, 
                 gte_get_last_row(3), 1)) 
    {
	if (gte_get_first_row(3) == gte_get_last_row(3))
	{
	    print_error("Cannot delete the last TestUnit.");
	}
	else
	{
	    gte_print_error("Deleting TestUnit `" + name + "'.");
	    gte_delete_row(gte_get_current_cell_row());
	    gte_set_selection(3, 1, 3, 1);
	    file_save();
	    // then delete the files, using tdriver in the same way the UI does
	    // it.
	    print_message("Deleting " + current_projdir() + current_system() + "/test_files/" + name);
	    system("tdriver -quiet -p " + current_projdir() + " -s " + current_system() + " -U " + name + " -X");
	}
    }
    else
	print_error("Could not find TestUnit `" + name + "'.");
 
    gte_enable_refresh();
    gte_refresh_table();


    return;

} // end of T_delete_testunit_row()
