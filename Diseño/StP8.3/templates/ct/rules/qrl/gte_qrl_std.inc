//
// 	sccsid[] = %W% %Y% %D%
// 	StP/Core
// 	Confidential property of Interactive Development Environments, Inc.
// 	Copyright (c) 1992-1994
// 	All rights reserved
//

#include "rules/qrl/version.inc"
#include "rules/qrl/recent_files.inc"
#include "rules/qrl/gte_object_rename.inc"
#include "rules/qrl/qrl_std.inc"
#include "rules/qrl/commands.inc"
#include "rules/qrl/gte_drag.inc"
#include "rules/qrl/gte_drop.inc"
// ECR 3789 begin
#include "ct/rules/qrl/edit_note_desc.inc"
#include "ct/rules/qrl/common_set_annot.qrl"
// ECR 3789 end

void 
trace_all()
{
  trace("all");
}

void
untrace_all()
{
  untrace("all");
}

void
print_warning(string msg)
{
    gte_print_message(msg);
}

void
FileReload()
{
    gte_stpem("FileLoad " + gte_substitute_string("${file}"));
}

boolean
buffer_is_modified()
{
    return gte_is_modified();
}

int
file_save()
{
  int rc = gte_stpem("FileSave");
  return rc;
}

int
file_save_no_update()
{
  int rc = gte_stpem("FileSaveNoUpdate");
  return rc;
}

// horizontal span at current cell

void
CurrentCellHorzSpan (int num_cells)
{
    gte_filltable_string("FillTable { { Hsect { Index Current}" +
		         "{ Vsect { Index Current } { Row { Index Current }" +
		         "{ Cell { Index Current } { Hspan " + num_cells + 
			 "}}}}}}");
}


// vertical span at current cell

void
CurrentCellVertSpan (int num_cells)
{
    gte_filltable_string("FillTable { { Hsect { Index Current}" +
		         "{ Vsect { Index Current } { Row { Index Current }" +
		         "{ Cell { Index Current } { Vspan " + num_cells + 
			 "}}}}}}");
}


// Label Current Cell

int
CurrentCellLabel(string label)
{
    gte_filltable_string("FillTable { { Hsect { Index Current}" +
		         "{ Vsect { Index Current } { Row { Index Current }" +
		         "{ Cell { Index Current } { Label \"" + 
			 filltable_string_escape(label) + "\"}}}}}}");
    gte_refresh_selection();
    return(0);
}

// Label Cell at (row, col) absolute

int
CoordCellLabel(int row, int col, string label)
{
    int hsect = gte_get_hsect(row);
    int vsect = gte_get_vsect(col);

    gte_filltable_string("FillTable { { Hsect { Index " + hsect + 
		         "} { Vsect { Index " + vsect +
		         "} { Row { Index " + 
			 (1 + row - gte_get_first_row(hsect)) +
		         "} { Cell { Index " + 
			 (1 + col - gte_get_first_col(vsect)) +
	     	         "} { Label \"" + filltable_string_escape(label) + "\"}}}}}}");
    return(0);
}

// Change Read Only Status of Cell at (row, col) absolute

int
CoordCellSetReadOnly(int row, int col, boolean read_only)
{
    int hsect = gte_get_hsect(row);
    int vsect = gte_get_vsect(col);
    string read_only_string;
    if (read_only)
      read_only_string = "True";
    else
      read_only_string = "False";

    gte_filltable_string("FillTable { { Hsect { Index " + hsect + 
		         "} { Vsect { Index " + vsect +
		         "} { Row { Index " + 
			 (1 + row - gte_get_first_row(hsect)) +
		         "} { Cell { Index " + 
			 (1 + col - gte_get_first_col(vsect)) +
	     	         "} { ReadOnly " + read_only_string + "}}}}}}");
    return(0);
}

// Find and Replace a Cell Label

int
LabelFindAndReplace(string search, string replace)
{
    gte_stpem("findopt -c -i +l +m -r");

    if (!gte_find(search)) 
	return(-1);

    return(CoordCellLabel(gte_get_current_cell_row(), 
			  gte_get_current_cell_col(), 
			  replace));
}

// replace a cell label by the next valid value

void CellToggleValue()
{
  int r, c;
  int i;
  list values;
  string current_value, next_value;
  
  r = gte_get_current_cell_row();
  c = gte_get_current_cell_col();
  values = gte_get_cell_valid_values(r, c);
  if (list_count(values) == 0)
    return;

  current_value = gte_get_cell_label(r, c);
  
  next_value = list_get(values, 0);
  
  for (i = 0; i < list_count(values); i = i + 1) 
    if (current_value == list_get(values, i)) {
      if (i < list_count(values) - 1)
	next_value = list_get(values, i + 1);
      break;
    }
      
  CoordCellLabel(r, c, next_value);
  gte_refresh_selection();
}

void UpdateTableHeaders()
{
    list vis_hsects = list_create("boolean",0), 
	 vis_vsects = list_create("boolean",0);
    list vis_rows = list_create("int",0), 
	 vis_cols = list_create("int",0);
    list first_vis_row = list_create("int",0), 
	 first_vis_col = list_create("int",0);
    int  sect, last_sect;

    // save visibility of table as displayed; fill table overwrites it

    for ( sect      = 0, // start with number row
	  last_sect = gte_get_hsect(gte_get_last_row()); 
	  sect <= last_sect ; sect = sect + 1)
    {
	list_append(vis_hsects, gte_get_hsect_visibility(sect));
	list_append(first_vis_row, gte_get_hsect_first_vis_row(sect) - 
		gte_get_first_row(sect) );
	list_append(vis_rows, gte_get_hsect_num_vis_rows(sect));
    }

    for ( sect      = 0, // start with number col
	  last_sect = gte_get_vsect(gte_get_last_col()); 
	  sect <= last_sect ; sect = sect + 1)
    {
	list_append(vis_vsects, gte_get_vsect_visibility(sect));
	list_append(first_vis_col, gte_get_vsect_first_vis_col(sect) - 
		gte_get_first_col(sect) );
	list_append(vis_cols, gte_get_vsect_num_vis_cols(sect));
    }

    gte_filltable_file(path_compose("table_fill",
				    gte_substitute_string("${editor}")));

    // restore section visibilty

    for (sect = 0; sect < list_count(vis_hsects); sect = sect + 1)
    {
	gte_set_hsect_visibility(sect, list_get(vis_hsects,sect));
	gte_set_hsect_num_vis_rows(sect, list_get(vis_rows,sect));
	gte_set_hsect_first_vis_row(sect, gte_get_first_row(sect) +
		list_get(first_vis_row,sect));
    }

    for (sect = 0; sect < list_count(vis_vsects); sect = sect + 1)
    {
	gte_set_vsect_visibility(sect,list_get(vis_vsects,sect));
	gte_set_vsect_num_vis_cols(sect,list_get(vis_cols,sect));
	gte_set_vsect_first_vis_col(sect,gte_get_first_col(sect) +
		list_get(first_vis_col,sect));
    }

    return;
}

string
gte_visibility_filltable_expression()
{
  /* Returns a filltable expression 
   * encoding the visibility of all the vertical and horizontal sections.
   * Can be used to restore current visibility after a destructive table operation. */

  int vs;
  int vsmax = gte_get_last_vsect();
  int hs;
  int hsmax = gte_get_last_hsect();
  string hideshow_hs = "";
  string hideshow_vs = "";
  string hidden;

  for (vs = 1; vs <= vsmax; vs = vs + 1) {
    if (gte_get_vsect_visibility(vs))
      hidden = "{Hidden False}";
    else
      hidden = "{Hidden True}";
    hideshow_vs = hideshow_vs + "{Vsect{Index " + vs + "}" + hidden + "}";
  }

  for (hs = 1; hs <= hsmax; hs = hs + 1) {
    if (gte_get_hsect_visibility(hs))
      hidden = "{Hidden False}";
    else
      hidden = "{Hidden True}";
    if (hs == 1)
      hideshow_hs = "{Hsect{Index " + hs + "}" + hidden + hideshow_vs + "}";
    else
      hideshow_hs = hideshow_hs + "{Hsect{Index " + hs + "}" + hidden + "}";
  }

  return "FillTable " + gte_get_table_type() + "{" + hideshow_hs + "}";
}


string
filltable_string_escape(string name)
{
  if (name == NULL)
    return (NULL);

  return string_escape(name, "\"");
}



/* -------------------------------------------------------------------------- */
/* 
 * for supporting navigation to tables in editor file groups 
 */
void
NavToggleAndFind(string filename, string appid, string type)
{
    string          currfn;

    currfn = gte_substitute_string("${file}");

    EditorShowOnNavReceive();

    if (currfn != filename)
    {
	gte_stpem("EditorSetFileName " + filename);
	NavPush(currfn);
    }
    gte_stpem("TableToggleType " + type);
    SymbolFind(appid);
}

/*
 * for supporting navigation ala GDE
 */

void
LoadADiagram(string fn)
{
    if (fn != NULL && fn != "")
    {
	gte_stpem("FileLoad " + fn);
	NavPush(fn);
    }
}



void
NavLoadAndFind(string filename, string appid)
{
    string          currfn;

    currfn = gte_substitute_string("${file}");

    EditorShowOnNavReceive();

    if (currfn != filename)
    {
	gte_stpem("FileLoad " + filename);
	NavPush(currfn);
    }
    SymbolFind(appid);

    if (gte_get_table_type() == "AttributeTable")
    update_old_ate_table();

    return;
}

// JED - This was added to support navigation in the DOORS integration
void
NavLoadAndFindAndQuitTBR(string filename, string appid, int tbrhandle)
{
	NavLoadAndFind (filename, appid) ;
	editor_send_msg ("tbr","EditorQrlEval \"doors_Quit_cb()\;\"", tbrhandle) ;
}



// EH -- added for rts 11719. Appearantly some folks 
// still have old tables from before the new inh key 
// columns were added.
void
update_old_ate_table()
{
    int vs = gte_lookup_symbol("InheritedKeyInfo_vs");
    if (gte_get_cell_label(1, gte_get_col(vs, 1)) == "")
    {
        print_message("Updating table to current release..."); 
        UpdateTableHeaders();
        gte_refresh_table();
        print_message(" "); 
    }
}

/* Find a symbol in the current editor */
void
SymbolFind (string appid)
{
    if (appid == NULL || appid == "")
	return;
    /*
     * we need a real interface to find in the table editor, but find would
     * need to be cleaned up in order to do that.  for now, the best we can
     * do is go through the stpem/builtin interface
     */
    gte_stpem("BufferFindOpt -CLS +IM");
    gte_stpem("BufferFind " + appid);
}

/* Eventually the stack may need a table type as well */
list navStack = list_create("string", 0);


void
NavPush(string filename)
{
    string	ln;
    int		ind;

    if (filename != NULL && filename != "")
    {
	ind = list_count(navStack) - 1;
	if (ind >= 0)
	{
	    ln = list_get(navStack, ind);
	} else {
	    ln = "";
	}
	if (ln != filename)
	{
	    list_append(navStack, filename);
	}
    }
}


void
NavPop()
{
    string filename;
    int    stacksize;

    stacksize = list_count(navStack);

    if (stacksize == 0)
    {
	gte_print_error("Navigation stack is empty, pop failed.");
	return;
    }
    filename = list_get(navStack, (stacksize - 1));
    list_delete(navStack, (stacksize - 1));

    EditorShowOnNavReceive();

    if (filename != gte_substitute_string("${file}"))
    {
	gte_stpem("FileLoad " + filename);
    }
}

/* -------------------------------------------------------------------------- */

/* Functions related to table selections */

int
gte_selection_is_one_whole_row() 
{
  if (gte_get_first_row_in_selected() == gte_get_last_row_in_selected() &&
      gte_get_first_col_in_selected() == gte_get_first_col() &&
      gte_get_last_col_in_selected() == gte_get_last_col())
    return true;
  return false;
}



void
EditorShowOnNavReceive()
{
    gte_stpem("EditorShowDeiconify");
}

/* For some table editors we want to disable the standard hide/show feature,
 * especially if groups of sections need to be hidden or shown atomically */

int idehook_hideshow_dialog_is_active = 1;

void 
gte_enable_hideshow_dialog()
{
  idehook_hideshow_dialog_is_active = 1;
}

void 
gte_disable_hideshow_dialog()
{
  idehook_hideshow_dialog_is_active = 0;
}

int
idehook_hideshow_dialog_active_func()
{
    if (idehook_hideshow_dialog_is_active != 0)
      return ACTIVE_FUNC_IS_ACTIVE;
    else
      return ACTIVE_FUNC_IS_INACTIVE;
}


int
gte_name_search_active_func()
{
    if (gte_get_current_cell_is_readonly())
	return(ACTIVE_FUNC_IS_INACTIVE);

    if (!gte_get_current_cell_has_semantics())
	return(ACTIVE_FUNC_IS_INACTIVE);
	
    return(ACTIVE_FUNC_IS_ACTIVE);
}

int
replace_value_in_hsect(string text, int hs, int clow, int chigh, string nv, boolean stopwhenempty)
{
    int		r, rmin, rmx, c, cnt;
    string	l;
    boolean	allempty;

    rmin = gte_get_first_row(hs);
    rmx = gte_get_last_row(hs);
    c = gte_get_last_row();
    if (rmx > c)
    {
	rmx = c;
    }
    if (clow < 0)
	clow = 0;
    if (chigh > gte_get_last_col())
	chigh = gte_get_last_col;
    cnt = 0;
    
    for(c = clow; c <= chigh; c = c + 1)
    {
	allempty = True;
	for(r = rmin; r <= rmx; r = r + 1)
	{
	    l = gte_get_cell_label(r, c);
	    if (l == text)
	    {
		CoordCellLabel(r, c, nv);
		cnt = cnt + 1;
	    }
	    if (l != "")
	    {
		allempty = False;
	    }
	}
	if (allempty == True && stopwhenempty == True)
	{
	    return(cnt);
	}
    }
    return(cnt);
}

boolean
find_value_in_hsect(string text, int hs, int clow, int chigh)
{
    int		r, rmin, rmx, c;
    string	l;

    rmin = gte_get_first_row(hs);
    rmx = gte_get_last_row(hs);
    c = gte_get_last_row();
    if (rmx > c)
    {
	rmx = c;
    }
    if (clow < 0)
	clow = 0;
    if (chigh > gte_get_last_col())
	chigh = gte_get_last_col;
    
    for(r = rmin; r <= rmx; r = r + 1)
    {
	for(c = clow; c <= chigh; c = c + 1)
	{
	    l = gte_get_cell_label(r, c);
	    if (l == text)
		return(True);
	}
    }
    return(False);
}

// Return True if all cell labels in the given horizontal
// section in the area (hsstart_row+rlow, clow)(hsstart_row+rhigh,chigh)
// are empty.

boolean
hsect_values_empty(int hs, int rlow, int rhigh, int clow, int chigh)
{
    int		r, rmin, rmx, c;
    string	l;

    rmin = gte_get_first_row(hs);
    rmx = gte_get_last_row(hs);
    c = gte_get_last_row();
    if (rmx > c)
    {
	rmx = c;
    }
    if (clow < 0)
	clow = 0;
    if (chigh > gte_get_last_col())
	chigh = gte_get_last_col;
    c = rmin + rhigh;
    if (c < rmx)
	rmx = c;
    rmin = rmin + rlow;
    
    for(r = rmin; r <= rmx; r = r + 1)
    {
	for(c = clow; c <= chigh; c = c + 1)
	{
	    l = gte_get_cell_label(r, c);
	    if (l != "")
	    {
		l = string_search_and_replace(l, " ", "");
		l = string_search_and_replace(l, "\n", "");
		l = string_search_and_replace(l, "\t", "");
		if (l != "")
		    return(False);
	    }
	}
    }
    return(True);
}

//
// Internal stpem interface for navigations (instead of using 
// command line stpem ...
//


void
NavigateSend(string editor, string command)
{
    editor_send_msg(editor, command, CH_FIRST);
}

void
NavigateSendQrl(string editor, string command)
{
    command = "EditorQrlEvalNoSave " + command;
    editor_send_msg(editor, command, CH_FIRST);
}


// Added gte_save_hook for the CTE for request 10450 (cim 8/12/97)

boolean gte_save_hook = False;

void
gte_enable_save_hook()
{
    gte_save_hook = True;
}

//
// Save the file and commit it's contents to the repository. Return
// False if we can't save the file, return True otherwise.
//

boolean
FileSaveAndCommit(boolean saveas)
{
    string name;
    boolean syntax_errors = False;	

// ECR 7336 begin
    string sysbusy_file = path_compose(current_projdir(), current_system(), ".sysbusy");
    if (file_exists(sysbusy_file))
    {
        gde_print_error("System busy; administration in progress.");
	    gde_print_error("Could not save table.");
	    return False;
    }
// ECR 7336 end

    name = gte_table_name();
    if (name == NULL || name == "")
    {
	if (gte_stpem("SaveasDialogShow"))
	    return False;
	else return True;
    }

    if (gte_save_hook && !gte_save_hook_func(name))
      return False;

    editor_set_is_busy(True);		

    if (!gte_check_syntax())
    {    
        editor_set_is_busy(False);		
	if (editor_confirm(
	       "The table has syntax errors, do you want to save it anyway?",
	       "Yes", "No") == False)
	    return False;
	else {
	  editor_set_is_busy(True);		
	  syntax_errors = True;
	}
    }

    // SPR 2535 - replace spaces with underscores - removed code segment here
    
// ECR 6379 begin
    boolean dest_exists = True;
    string dest_dir = path_part(gte_file_name());
    if (!file_exists(dest_dir))
    {
        // try to create it
        if (mkdir(dest_dir) == -1)
            dest_exists = False;
    }
// ECR 6379 end

    if (!dest_exists || file_save_no_update() != 0)
    {
        // SPR 2535 - replace spaces with underscores - added line below
        name = gte_table_name();
        editor_set_is_busy(False);
    gte_print_error("Could not save table '" + name + "'");
    return False;
    }

    gte_commit_to_repository(syntax_errors);
    editor_set_is_busy(False);

    // SPR 2535 - replace spaces with underscores - added line below
    name = gte_table_name();
    gte_print_message("The table '" + name + "' has been saved.");

    if (gte_get_table_type() == "AttributeTable")
    update_old_ate_table();

    return True;
}


//
// The following routines are used to implement the standard name completion
// behavior for gte.
//
 
void
nc_get_node_names_from_buffer(list names, string query, string pattern,
			      string extent, string apptype, int scopeid, boolean ignore_scope)
{
    node n, cn;
    int i;
    list nodes;
    list candidate_buffer_nodes = list_create("node", 0);
    
    nodes = gte_all_mapped_oms_nodes();
    for (i = 0; i < list_count(nodes); i = i + 1)
    {
	n = list_get(nodes, i);
	
	if (n.type == apptype && (ignore_scope || n.scope_node_id == scopeid))
	    list_append(candidate_buffer_nodes, n);
    }
    
    
    for_each_in_select(query, n)
    {
	for (i = 0; i < list_count(candidate_buffer_nodes); i = i + 1)
	{
	    cn = list_get(candidate_buffer_nodes, i);
	    if (cn.id == n.id)
	    {
		list_append(names, n.name);
		break;
	    }
	}
    }
}

void
nc_get_link_names_from_buffer(list names, string query, string pattern,
			      string extent, string apptype, int scopeid, boolean ignore_scope)
{
    link l, cl;
    int i;
    list links;
    list candidate_buffer_links = list_create("link", 0);
    
    links = gte_all_mapped_oms_links();
    for (i = 0; i < list_count(links); i = i + 1)
    {
	l = list_get(links, i);
	
	if (l.type == apptype && (ignore_scope || l.scope_node_id == scopeid))
	    list_append(candidate_buffer_links, l);
    }
    
    
    for_each_in_select(query, l)
    {
	for (i = 0; i < list_count(candidate_buffer_links); i = i + 1)
	{
	    cl = list_get(candidate_buffer_links, i);
	    if (cl.id == l.id)
	    {
		list_append(names, l.name);
		break;
	    }
	}
    }
}

void
nc_get_cntx_names_from_buffer(list names, string query, string pattern,
			      string extent, string apptype, int scopeid, boolean ignore_scope)
{
    cntx c, cc;
    int i;
    list cntxs;
    list candidate_buffer_cntxs = list_create("cntx", 0);
    
    cntxs = gte_all_mapped_oms_cntxs();
    for (i = 0; i < list_count(cntxs); i = i + 1)
    {
	c = list_get(cntxs, i);
	
	if (c.type == apptype && (ignore_scope || c.scope_node_id == scopeid))
	    list_append(candidate_buffer_cntxs, c);
    }
    
    
    for_each_in_select(query, c)
    {
	for (i = 0; i < list_count(candidate_buffer_cntxs); i = i + 1)
	{
	    cc = list_get(candidate_buffer_cntxs, i);
	    if (cc.id == c.id)
	    {
		list_append(names, c.name);
		break;
	    }
	}
    }
}


void
nc_get_names_from_buffer(list names, string pattern, string extent,
			 string apptype, int scopeid, boolean ignore_scope)
{
    string query;
    
    //
    // What I would like to do in this routine is just iterate through all
    // the mapped objects in the buffer (retrieved using the
    // gte_all_mapped_oms_* functions) and test their apptype,
    // name and scopeid attributes against those supplied as parameters to this function.
    // The problem with this is that there is no pattern matching in the
    // qrl language, so I've got to rely on the oms query language to do
    // the pattern matching for me.
    //
    // So the algorithm is to query the oms for a set of candidate objects
    // using the pattern, apptype and scopeid as qualifiers and then for each
    // object in the returned set look to see if the object id matches the id of
    // one of the objects in the buffer, if so, then it is a match. (Note that
    // we do not
    // require that candidate objects have references, since the in general
    // there will be new objects in the buffer since the last time a save
    // was done)
    //

    query = extent + "[" + apptype
                   + nc_name_clause(extent, pattern)
		   + nc_scope_clause(extent, scopeid, ignore_scope)
		   + "]";
    

    if (extent == "node")
	nc_get_node_names_from_buffer(names, query, pattern, extent, apptype, scopeid, ignore_scope);
    else if (extent == "link")
	nc_get_link_names_from_buffer(names, query, pattern, extent, apptype, scopeid, ignore_scope);
    else if (extent == "cntx")
	nc_get_cntx_names_from_buffer(names, query, pattern, extent, apptype, scopeid, ignore_scope);
}

list
gte_default_name_completion_func(string pattern, string label, string extent,
				 string apptype, int scopeid)
{
    list names;
    list result;

    names = list_create("string", 0);

    if (extent == NULL || extent == "" || apptype == NULL || apptype == "")
	return (names);

    if(extent == "item" && app_type_is_choice_item(apptype))
       {
           result = app_type_get_item_choices(apptype);
           return result;
       }
           

    nc_get_names_from_repository(names, pattern, extent, apptype, scopeid, False);
    nc_get_names_from_buffer(names, pattern, extent, apptype, scopeid, False);

    return (names);
}

list
gte_ignore_scope_name_completion_func(string pattern, string label,
				      string extent, string apptype,
				      int scopeid)
{
    list names;

    //
    // the only thing that makes this different from the default name
    // completion function is that it ignores scope when searching for objects
    // of the specified type..
    //

    names = list_create("string", 0);

    nc_get_names_from_repository(names, pattern, extent, apptype, scopeid, True);
    nc_get_names_from_buffer(names, pattern, extent, apptype, scopeid, True);

    return (names);
}

list
gte_item_name_completion_func(string pattern, string label, string extent,
			      string apptype, int scopeid)
{
    int hsect, vsect, firstrow, lastrow, firstcol, lastcol, i, j;
    int currrow, currcol;
    string itemval;
    list names;

    //
    // Since item mappings are not calculated on the fly in the gte
    // by default, we need to do something extra to make sure that
    // name completion on items will consider all of the item values that
    // have currently been entered into the table by the user.
    //
    // So this function first uses the standard name completion function
    // to get the usual name completion targets. Then it supplements
    // this list with all of the cell values in the section that contains
    // the currently selected cell. This assumes that all of the cells
    // in the current hsect/vsect are homogenous and contain item values
    // of the appropriate type.
    //

    names = gte_default_name_completion_func(pattern, label, extent,
						  apptype, scopeid);

    hsect = gte_get_hsect(gte_get_current_cell_row());
    vsect = gte_get_vsect(gte_get_current_cell_col());
    
    firstrow = gte_get_first_row(hsect);
    lastrow = gte_get_last_row(hsect);
    firstcol = gte_get_first_col(vsect);
    lastcol = gte_get_last_col(vsect);

    currrow = gte_get_current_cell_row();
    currcol = gte_get_current_cell_col();
    
    for (i = firstrow; i <= lastrow; i = i + 1)
	for (j = firstcol; j <= lastcol; j = j + 1)
	{
	    //
	    // It's debatable whether or not we should include the label from
	    // the cell currently being completed on. For now, I do include it.
	    // If we want to change that behavior, just uncomment the next two
	    // lines.
	    //
	    // if (i == currrow && j == currcol)
	    //     continue;
	    
	    itemval = gte_get_cell_label(i, j);
	    if (itemval == NULL || itemval == "")
		continue;
	    
	    if (string_match_sh(itemval, pattern))
		list_append(names, itemval);
	}
    
    return (names);
}

list
gte_col_item_name_completion_func(string pattern, string label, string extent,
			      string apptype, int scopeid)
{
    int hsect, firstrow, lastrow, currrow, currcol, i;
    string itemval;
    list names;

    //
    // This function is the same as gte_item_name_completion_func,
    // so see the comments there.
    // The only difference is that this function only adds all the
    // values from the current column as oppposes to all the values
    // from the current section.
    //

    names = gte_default_name_completion_func(pattern, label, extent,
						  apptype, scopeid);

    
    hsect = gte_get_hsect(gte_get_current_cell_row());
    firstrow = gte_get_first_row(hsect);
    lastrow = gte_get_last_row(hsect);
    currrow = gte_get_current_cell_row();
    currcol = gte_get_current_cell_col();
    
    for (i = firstrow; i <= lastrow; i = i + 1)
    {
	//
	// It's debatable whether or not we should include the label from
	// the cell currently being completed on. For now, I do include it.
	// If we want to change that behavior, just uncomment the next two
	// lines.
	//
	// if (i == currrow)
	//     continue;
	    
	itemval = gte_get_cell_label(i, currcol);
	if (itemval == NULL || itemval == "")
	    continue;
	
	if (string_match_sh(itemval, pattern))
	    list_append(names, itemval);
    }
    
    return (names);
}

// Begin Add ECR 4072 functions for automatically setting /saving the hidden sections

// two function bodys stolen from gte_qrl_std.inc:UpdateHeaders and put into
// extra functions

string
gte_get_cte_vis()
{
	list vis_hsects = list_create("boolean",0), 
	 vis_vsects = list_create("boolean",0);
    list vis_rows = list_create("int",0), 
	 vis_cols = list_create("int",0);
    list first_vis_row = list_create("int",0), 
	 first_vis_col = list_create("int",0);
    int  sect, last_sect,i;
    string res ="";
    
   for ( sect      = 0, // start with number row
	  last_sect = gte_get_hsect(gte_get_last_row()); 
	  sect <= last_sect ; sect = sect + 1)
    {
		list_append(vis_hsects, gte_get_hsect_visibility(sect));
		list_append(first_vis_row, gte_get_hsect_first_vis_row(sect) - 
			gte_get_first_row(sect) );
		list_append(vis_rows, gte_get_hsect_num_vis_rows(sect));
    }

    for ( sect      = 0, // start with number col
	  last_sect = gte_get_vsect(gte_get_last_col()); 
	  sect <= last_sect ; sect = sect + 1)
    {
		list_append(vis_vsects, gte_get_vsect_visibility(sect));
		list_append(first_vis_col, gte_get_vsect_first_vis_col(sect) - 
			gte_get_first_col(sect) );
		list_append(vis_cols, gte_get_vsect_num_vis_cols(sect));
    }
    
    for( i = list_count( vis_hsects )-1; i>=0;i--)
    	res = res + "H "+to_string(i)+" "+list_get(vis_hsects, i)+" "+list_get(first_vis_row,i)+" "+list_get(vis_rows,i)+";";
    for( i = list_count( vis_vsects )-1; i>=0;i--)
    	res = res + "V "+to_string(i)+" "+list_get(vis_vsects, i)+" "+list_get(first_vis_col,i)+" "+list_get(vis_cols,i)+";";
    	
	return res;    	
}

void
gte_set_cte_vis( string vis )
{
	list inlines = string_to_list(vis,";" );
	//message(inlines);
	list line;
	int i, sect;
	for( i = 0 ; i <list_count(inlines); i++)
	{
		line = string_to_list(list_get(inlines,i)," " );
		//message(line);
		sect = to_int(list_get(line,1 ));
		if( "H" == list_get( line, 0 ) )
		{

			gte_set_hsect_visibility(sect, "True"==list_get(line,2));
			gte_set_hsect_num_vis_rows(sect, to_int(list_get(line,4)));
			gte_set_hsect_first_vis_row(sect, gte_get_first_row(sect) +
				to_int(list_get(line,3)));
    	}
		else
		{
			gte_set_vsect_visibility(sect,"True"==list_get(line,2));
			gte_set_vsect_num_vis_cols(sect,to_int(list_get(line,4)));
			gte_set_vsect_first_vis_col(sect,gte_get_first_col(sect) +
			to_int(list_get(line,3)));
    	}
    }
    
    for( i = 0 ; i <list_count(inlines) && 0; i++)
	{
		line = string_to_list(list_get(inlines,i)," " );
		//message(line);
		sect = to_int(list_get(line,1 ));
		if( "H" == list_get( line, 0 ) )
		{

			gte_set_hsect_num_vis_rows(sect, to_int(list_get(line,3)));
    	}
		else
		{
			message("X:"+sect+":"+to_int(list_get(line,3)));
			gte_set_vsect_num_vis_cols(sect,to_int(list_get(line,3)));
    	}
    }
    //gte_set_vsect_num_vis_cols(3,5);

    gte_refresh_table();
    // The following is necessary, because it the above code scrolls around
    // this sets us back to the start, which should be quite nice.
    editor_builtin( "BufferMoveTo 1 1" );
}

void
gte_save_default_visibility()
{
	editor_write_profile_string( "default_visibility", gte_get_cte_vis() );

}

void
gte_load_default_visibility()
{
	gte_set_cte_vis( editor_read_profile_string( "default_visibility" ) );
}
// End Add ECR 4072

// ECR 3789 begin
boolean ps_NoteDescription_cb_debug = True;

// ECR 6880 Property sheets from table editors
// The following struct definition is a kluge to be
// able to use the same code fragements for gte and
// gde (where _ps_field_tp is defined)
//
// struct _ps_field_tp 
// {
//    string x;
// };
// ECR 6880 end

int ps_NoteDescriptionExternal = 12;

int
ps_EOD_cb(string notename )
{ 
    int row = gte_get_current_cell_row();
    int col = gte_get_current_cell_col();
    int hsect = gte_get_hsect(gte_get_current_cell_row());
    int vsect = gte_get_vsect(gte_get_current_cell_col());

    int objid;
    string objectType;
    node nod;

    // We use hsect/vsect as 1,1 - using the hsect and vsect variables
    // yields incorrect results. Don't know if this is a bug in
    // the gte_... function
    //
    list objects = gte_cell_mapped_oms_nodes( 1, 1, 
					      gte_get_current_cell_row(),
					      gte_get_current_cell_col() );
    //message( hsect );     message( vsect );
    //message( row );     message( col );
    //message( objects );
    if( list_count( objects ) == 0 )
    {
	gte_print_error( "No object selected." );
	gte_print_error( "Edit description failed." );
	return 0;
    }
    
    if( list_count( objects ) > 1 )
    {
	gte_print_error( "Invalid selection" );
	gte_print_error( "Edit description failed." );
	return 0;
    }
	    
   
    nod = list_get( objects, 0 );
    objid = nod.id;
    objectType = to_string( nod.type );

    // Now do the editing. This is outfactored, since it is the same for gde
    //

    return ps_EOD_logic( objid, objectType, notename );
}
// ECR 3789 end

// ECR 4061
string
SymbolNameSearchLabelFunc()
{
    int row = gte_get_current_cell_row();
    int col = gte_get_current_cell_col();
    int hsect = gte_get_hsect(gte_get_current_cell_row());
    int vsect = gte_get_vsect(gte_get_current_cell_col());

    int objid;
    string objectType;
    node nod;
    item it;
    string type;
    list objects;

    objects = gte_cell_mapped_oms_nodes( 1, 1, row, col );
    if( list_count( objects ) > 0 )
    {
        nod = list_get( objects, 0 );
        if( nod != NULL )
        {
            type = nod.type; 
            type = string_strip( type, "L", "Uml" );       
            return "Choose " + type + " &Names...";
        }
    }

    objects = gte_cell_mapped_oms_items( 1, 1, row, col );
    if( list_count( objects ) > 0 )
    {
        it = list_get( objects, 0 );
        if( it != NULL )
        {
            type = it.type;
            type = string_strip( type, "L", "Uml" );
            return "Choose " + type + " &Names...";
        }
    }

    return "Choose &Names...";
}
// ECR 4061

// ECR 7475
string
SymbolRenameLabelFunc()
{
    int row = gte_get_current_cell_row();
    int col = gte_get_current_cell_col();
    int hsect = gte_get_hsect(gte_get_current_cell_row());
    int vsect = gte_get_vsect(gte_get_current_cell_col());

    int objid;
    string objectType;
    node nod;
    item it;
    string type;
    list objects;

    objects = gte_cell_mapped_oms_nodes( 1, 1, row, col );
    if( list_count( objects ) > 0 )
    {
        nod = list_get( objects, 0 );
        if( nod != NULL )
        {
            type = nod.type;
            type = string_strip( type, "L", "Uml" );
            return "Rena&me " + type + " Systemwide...";
        }
    }
        
    return "Rena&me Systemwide...";
}
// ECR 7475

/* -------------------------------------------------------------------------- */

//  Add any user customizations to the file included below:
#include_if_exists "user/ct/rules/qrl/user_gte_qrl_std.inc"
