//
//      sccsid[] = %W% %Y% %D%
//      StP/OMT
//	StP Core
//      Confidential property of Interactive Development Environments, Inc.
//      Copyright (c) 1992-1995
//      All rights reserved
//

ui_prop	FileSelectionBrowser = NULL;

void FileSelectionNameInitialize(list attrs)
{
    if (FileSelectionBrowser == NULL)
    {
	FileSelectionBrowser = to_ui_prop("selectDiagram", "ListComponentDiagrams");
	ui_activate(to_ui_prop("selectDiagram", "AllGroup"), True);
    }
    if (attrs == NULL)
    {
	attrs = list_create("string", 0);
    }
    ui_set_value(FileSelectionBrowser, attrs);
    IsSelectionValid(FileSelectionBrowser);
}

boolean IsSelectionValid(ui_prop p)
{
    list	l;
    boolean	nv = False;
    ui_prop	applybtn;

    if (p == NULL)
    {
	p = to_ui_prop("selectDiagram", "ListFileSelection");
    }
    l = ui_list_selected(p);
    if (l != NULL && list_count(l) > 0)
    {
	nv = True;
    }

    applybtn = to_ui_prop("selectDiagram", "Apply");
    ui_activate(applybtn, nv);
    applybtn = to_ui_prop("selectDiagram", "OK");
    ui_activate(applybtn, nv);
    return(nv);
}


int RowSelected(ui_event_tp event, ui_prop theprop, int id, string value)
{
    list	sel;

    IsSelectionValid(theprop);
    return(0);
}

int ApplySelection(ui_event_tp event, ui_prop theprop, int id, string v)
{
    OkSelection(event, theprop, id, v);
    return(0);
}


int OkSelection(ui_event_tp event, ui_prop theprop, int id, string v)
{
    string	s;
    list	strings;

    if (IsSelectionValid(FileSelectionBrowser) == True)
    {
	strings = ui_list_selected_items(FileSelectionBrowser);
	s = list_get(strings,0);
	if (s != "")
	{
 	    sequenceThisCollaborationDiagram(s);
	    return(1);
	}
    }
    return(0);
}

int CancelSelection(ui_event_tp event, ui_prop theprop, int id, string v)
{
    return(1);
}
