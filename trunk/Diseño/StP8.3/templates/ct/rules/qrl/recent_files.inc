//
// 	sccsid[] = %W% %Y% %D%
// 	StP/Core
// 	Confidential property of Interactive Development Environments, Inc.
// 	Copyright (c) 1992-1994
// 	All rights reserved
//


//
// Handle recently loaded files
//


int
recent_menu_active()
{
    if (navStack == NULL)
	return(0);
    if (list_count(navStack) == 0)
	return(0);
    return(1);
}

list		file_load_editor_functions = list_create("string", 0);


boolean
EditorAddFileLoadFunction(string f)
{
    boolean	done = False;

    if (list_count(file_load_editor_functions) == 0)
    {
	list_append(file_load_editor_functions, f);
	done = True;
    } else if (list_find(file_load_editor_functions, 0, f) >= 
		list_count(file_load_editor_functions))
    {
	list_append(file_load_editor_functions, f);
	done = True;
    }
    return(done);
}

void
EditorCallClientLoadFuncs(string fn)
{
    int		i, cnt;
    string	func;

    cnt = list_count(file_load_editor_functions);
    for(i = 0; i < cnt; i = i + 1)
    {
	func = list_get(file_load_editor_functions, i);
	if (string_find(func, 0, "(") >= string_length(func))
	{
	    func = func + "(fn)";
	}
	if (string_find(func, 0, ";") >= string_length(func))
	{
	    func = func + ";";
	}
	qrl_eval(func);
    }
}

file	ThisIsTheCurrentFile = NULL;

file
EditorGetCurrentFile()
{
    return(ThisIsTheCurrentFile);
}

int
EditorLoadFileFunction(string fn)
{
    string	apptp = current_editor_apptype();

    NavPush(fn);
    ThisIsTheCurrentFile = find_by_query("file[name='${fn}' && ${apptp}]");
    EditorCallClientLoadFuncs(fn);
    // update recent menu if visible ....
    recent_menu_show_common();
    return(list_count(navStack));
}

const	string	RECENT_MENU_DLM = ": ";
boolean	recent_menu_initialized = False;

void
recent_menu_show_diag()
{
    gde_builtin("PropertySheetShow RecentFilesList");
    // ui_show(to_ui_prop("RecentFilesList", "LoadTable"), False);
    // ui_show(to_ui_prop("RecentFilesList", "LoadDiag"), True);

    recent_menu_initialized = True;
    recent_menu_show_common();
}

void
recent_menu_show_table()
{
    gte_stpem("PropertySheetShow RecentFilesList");
    // ui_show(to_ui_prop("RecentFilesList", "LoadDiag"), False);
    // ui_show(to_ui_prop("RecentFilesList", "LoadTable"), True);

    recent_menu_initialized = True;
    recent_menu_show_common();
}

void
recent_menu_show_common()
{
    int		i, ind = 1;
    string	n;
    list	l;
    string	pad;


    if (recent_menu_initialized == False)
	return;


    l = list_create("string", 0);
    for(i = 0; i < list_count(navStack); i = i + 1)
    {
	n = list_get(navStack, i);
	if (i > 0)
	{
	    if (list_find(navStack, 0, n) < i)
		continue;
	}
	if (ind < 10)
	    pad = "  ";
	else if (ind < 100)
	    pad = " ";
	else
	    pad = "";
	list_append(l, pad + ind + RECENT_MENU_DLM + n);
	ind = ind + 1;
    }
    if (list_count(l) > 0)
    {
	ui_set_value(to_ui_prop("RecentFilesList", "Files"), l);
    }
}



int
RecentListLoad(ui_event_tp event, ui_prop p, int id, string value)
{
    string	fn;
    int		ind, rl;
    ui_prop	flist = to_ui_prop("RecentFilesList", "Files");
    int		row;
    int		rc = 0;

    if (ui_button_is_ok(p))
	rc = 1;

    row = ui_list_selected_row(flist);
    if (row == NULL)
	return(0);
    fn = ui_list_row_label(flist, row);
    if (fn == NULL)
	return(0);

    rl = string_length(RECENT_MENU_DLM);

    ind = string_find(fn, 0, RECENT_MENU_DLM);
    if (ind < string_length(fn))
    {
	fn = string_extract(fn, ind + rl, string_length(fn) - (ind + rl));
	LoadADiagram(fn);
	if (! ui_button_is_apply(p))
	    if (ui_owner_visible(flist))
		ui_owner_dismiss(to_ui_owner("RecentFilesList"));
	return(rc);
    }
    return(0);
}
