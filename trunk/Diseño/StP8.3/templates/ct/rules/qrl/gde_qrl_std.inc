// 	sccsid[] = %W% %Y% %D%
// 	StP/Core
// 	Confidential property of Interactive Development Environments, Inc.
// 	Copyright (c) 1992-1994
// 	All rights reserved
//

#include "rules/qrl/qrl_std.inc"
#include "rules/qrl/version.inc"
#include "rules/qrl/recent_files.inc"
#include "rules/qrl/gde_object_rename.inc"
#include "rules/qrl/gde_object_delete.inc"
#include "rules/qrl/commands.inc"
#include "rules/qrl/gde_drop.inc"
#include "rules/qrl/gde_drag.inc"
// ECR 3789 begin
#include "ct/rules/qrl/common_set_annot.qrl"
// ECR 3789 end

// ECR 6880 Property sheets from table editors
// moved from ps.inc
const int ps_NoteDescriptionExternal = 99;
const boolean ps_NoteDescription_cb_debug = False;
// ECR 6880 end


const string BUF_ALIGN_MSG = "BufferAlignArcs ";
const string MSG_SYM_SEL_APPID = "SymbolSelect AppId ";
const string SYM_REPLACE_MSG = "SymbolReplace ";
const string SYM_START_APPID_MSG = "SymbolStart AppId ";
const string VERTEX_SYMBOL_TYPE = "Vertex";
const string UPDATE_DMARK_MSG = "BufferUpdateDmarks";
const string SYM_SEL_APPID_MSG = "SymbolSelect AppId ";
const string SYM_ADJ_APPID_MSG = "SymbolAdjust AppId ";
const string SYM_ATTACH_PA_MSG = "SymbolAttachParams ";
const string SYM_UPDATE_DMARK_MSG = "SymbolUpdateDmarks";
const string SYM_SCALE_TO_LABEL = "SymbolScaleToLabel";

const int	DISABLE_SAVE_STATE = 1000;
const int	ENABLE_SAVE_STATE = -1000;

const int	GDE_CMD_INSERT		= 1;
const int	GDE_CMD_REPLACE		= 2;
const int	GDE_CMD_MOVE		= 3;
const int	GDE_CMD_DELETE		= 4;
const int	SEL_ITEM_LIMIT	= 15;

// Begin Add ECR 4980
void
GdeStartupFunction()
{
    if( gde_draw_ortho() )
        ui_toolbar_button_select("Standard", "OrthogonalArcs",True);
}
// End Add ECR 4980

void
FileReload()
{
    gde_builtin("FileLoad " + gde_substitute_string("${file}"));
}

boolean
buffer_is_modified()
{
    return(gde_is_modified());
}

int
file_save()
{
    int rc;

    rc = gde_builtin("FileSave");
    return(rc);
}

int
file_save_no_update()
{
    int rc;
    
    rc = gde_builtin("FileSaveNoUpdate");
    return(rc);
}

int
writeRepsyncFile(string fname)
{
    if (!gde_write_repsync_file(fname))
    {
	print_error("Write repsync file '" + fname + "' failed.");
	gde_set_status(1);
	return(1);
    }
    return(0);
}


void
LoadADiagram(string fn)
{
    if (fn != NULL && fn != "")
    {
	gde_builtin("FileLoad " + fn);
	NavPush(fn);
	
    }
}


void
NavLoadAndFind(string filename, string appid)
{
    string	currfn;
    gde_node	gnode;
    gde_arc	garc;
    gde_cntx    gcntx;
    list	symbols;
    int		i, id, id1, id2;
    gde_symbol	symbol;
    list	ids;

    EditorShowOnNavReceive();

    currfn = gde_diagram_name();

    if (currfn != filename)
    {
	if(gde_builtin("FileLoad " + filename))
	    return;	// FileLoad failed, probably cancelled by user
	NavPush(currfn);
    }

    if (appid == NULL)
    {
	return;
    }
    if (appid == "")
    {
	return;
    }
    ids = string_to_list(appid, ", ");
    if (list_count(ids) == 0)
    {
	return;
    }
    id1 = to_int(list_get(ids, 0));
    id2 = id1;
    if (list_count(ids) > 1)
    {
	id2 = to_int(list_get(ids, 0));
    }

    symbols = gde_all_symbols();

    for (i = 0; i < list_count(symbols); i = i + 1)
    {
	symbol = list_get(symbols, i);
	if (gde_symbol_is_node(symbol))
	{
	    gnode = to_gde_node(symbol);
	    id = gde_node_psymid(gnode);
	    if (id == id1)
	    {
		gde_node_bring_into_view(gnode);
		gde_builtin(MSG_SYM_SEL_APPID + id);
		gde_node_feedback(gnode);
		return;
	    }
	} else if (gde_symbol_is_arc(symbol))
	{
	    garc = to_gde_arc(symbol);
	    id = gde_arc_psymid(garc);
	    if (id == id1 || id == id2)
	    {
		gde_arc_bring_into_view(garc);
		gde_builtin(MSG_SYM_SEL_APPID + id);
		gde_arc_feedback(garc);
		return;
	    }
	} else if (gde_symbol_is_cntx(symbol))
	{
	    gcntx = to_gde_cntx(symbol);
	    id = gde_cntx_psymid(gcntx);
	    if (id == id1)
	    {
		gde_cntx_bring_into_view(gcntx);
		gde_builtin(MSG_SYM_SEL_APPID + id);
		gde_cntx_feedback(gcntx);
		return;
	    }
	}
	else {
	    id = -1;
	}
    }
}

// JED - This was added to support navigation in the DOORS integration
void
NavLoadAndFindAndQuitTBR(string filename, string appid, int tbrhandle)
{
	NavLoadAndFind (filename, appid) ;
	editor_send_msg ("tbr","EditorQrlEval \"doors_Quit_cb()\;\"", tbrhandle) ;
}



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
	gde_print_error("Navigation stack is empty, pop failed.");
	return;
    }
    filename = list_get(navStack, (stacksize - 1));
    list_delete(navStack, (stacksize - 1));
    EditorShowOnNavReceive();

    if (filename != gde_diagram_name())
    {
	gde_builtin("FileLoad " + filename);
    }
}

void
EditorShowOnNavReceive()
{
    gde_builtin("EditorShowDeiconify");
}


//
// ERRORS AND WARNINGS
//

void
gde_print_mapping_error(string message, int appid)
{
    if (gde_mapping_is_verbose())
	gde_print_error(message, appid);
}

void
print_warning(string message)
{
    gde_print_message(message);
}

//
// ARC tracing
//

gde_node
gde_arc_get_real_fromnode(gde_arc arc, boolean single, list endn, list syms)
{
    gde_arc	t;
    gde_node	fn;
    list	arcs;
    int		i;


    // message("enter gde_arc_get_real_fromnode");

    if (syms != NULL)
    {
	list_append(syms, to_gde_symbol(arc));
    }

    fn = gde_arc_fromnode(arc);
    if (gde_nt_name(gde_node_nt(fn)) == VERTEX_SYMBOL_TYPE)
    {
	if (syms != NULL)
	{
	    list_append(syms, to_gde_symbol(fn));
	}
	arcs = gde_node_inarcs(fn);
	if (single == True && list_count(arcs) > 1)
	{
	    return(NULL);
	}
	for(i = 0; i < list_count(arcs); i = i + 1)
	{
	    t = list_get(arcs, i);
	    if (syms != NULL)
	    {
		list_append(syms, to_gde_symbol(t));
	    }
	    fn = gde_arc_get_real_fromnode(t, single, endn, syms);
	    if (endn == NULL)
	    {
		return(fn);
	    }
	    if (fn != NULL && list_find(endn, 0, fn) == list_count(endn))
	    {
		list_append(endn, fn);
	    }
	}
	return(NULL);
    }
    if (endn != NULL && fn != NULL)
    {
	if (list_find(endn, 0, fn) == list_count(endn))
	{
	    list_append(endn, fn);
	}
    }
    return(fn);
}


gde_node
gde_arc_get_real_tonode(gde_arc arc, boolean single, list endn, list syms)
{
    gde_arc	t;
    gde_node	fn;
    list	arcs;
    int		i;

    if (syms != NULL)
    {
	list_append(syms, to_gde_symbol(arc));
    }
    fn = gde_arc_tonode(arc);
    if (gde_nt_name(gde_node_nt(fn)) == VERTEX_SYMBOL_TYPE)
    {
	if (syms != NULL)
	{
	    list_append(syms, to_gde_symbol(fn));
	}
	arcs = gde_node_outarcs(fn);
	if (single == True && list_count(arcs) > 1)
	{
	    return(NULL);
	}

	for(i = 0; i < list_count(arcs); i = i + 1)
	{
	    t = list_get(arcs, i);
	    if (syms != NULL)
	    {
		list_append(syms, to_gde_symbol(t));
	    }
	    fn = gde_arc_get_real_tonode(t, single, endn, syms);
	    if (endn == NULL)
	    {
		return(fn);
	    }
	    if (fn != NULL && list_find(endn, 0, fn) == list_count(endn))
	    {
		list_append(endn, fn);
	    }
	}
	return(NULL);
    }
    if (endn != NULL && fn != NULL)
    {
	if (list_find(endn, 0, fn) == list_count(endn))
	{
	    list_append(endn, fn);
	}
    }
    return(fn);
}

//
// FIND NODES, ARCS, CNTXS
//

list
gde_nodes_find_by_type(list symbols, string type_name)
{
  // returns a list of all the gde_nodes in symbols whose type matches type_name

  list matched_symbols;
  int i;
  gde_symbol symbol;
  gde_node gnode;
  gde_nt gnt;

  matched_symbols = list_create("gde_node", 0);
  if (symbols == NULL)
    return (matched_symbols);

  for (i = 0; i < list_count(symbols); i = i + 1)
    {
      symbol = list_get(symbols, i);

      if (gde_symbol_is_node(symbol))
	{
	  gnode = to_gde_node(symbol);
	  gnt = gde_node_nt(gnode);

	  if (gde_nt_name(gnt) == type_name)
	    list_append(matched_symbols, gnode);
	}
    }

  return (matched_symbols);
}

list
gde_nodes_find_by_application_tag(list symbols, string tag)
{
  // returns a list of all the gde_nodes or gde_cntxs in symbols whose application tag matches tag
  // returns all of these as nodes, i.e. symbols are cast to gde_node whether or not they are cntxs

  list matched_symbols;
  int i;
  gde_symbol symbol;
  gde_node gnode;
  gde_nt gnt;

  matched_symbols = list_create("gde_node", 0);
  if (symbols == NULL)
    return (matched_symbols);

  for (i = 0; i < list_count(symbols); i = i + 1)
    {
      symbol = list_get(symbols, i);

      if (gde_symbol_is_node(symbol) || gde_symbol_is_cntx(symbol))
	{
	  gnode = to_gde_node(symbol);
	  gnt = gde_node_nt(gnode);

	  if (gde_nt_application_tag(gnt) == tag)
	    list_append(matched_symbols, gnode);
	}
    }

  return (matched_symbols);
}

gde_node
gde_find_node_by_label(list nodes, string tag)
{
  // returns the node whose label matches tag

  int i;
  gde_node gnode;

  if (nodes == NULL)
    return (NULL);

  for (i = 0; i < list_count(nodes); i = i + 1) {
    gnode = list_get(nodes, i);
    if (gde_node_label(gnode) == tag)
      return (gnode);
  }
  return (NULL);
}

list
gde_arcs_find_by_type(list symbols, string type_name)
{
    list matched_symbols;
    int i;
    gde_symbol symbol;
    gde_arc garc;
    gde_lt glt;

    matched_symbols = list_create("gde_arc", 0);
    if (symbols == NULL)
      return (matched_symbols);

    for (i = 0; i < list_count(symbols); i = i + 1)
    {
	symbol = list_get(symbols, i);

	if (gde_symbol_is_arc(symbol))
	{
	    garc = to_gde_arc(symbol);
	    glt = gde_arc_lt(garc);

	    if (gde_lt_name(glt) == type_name)
		list_append(matched_symbols, garc);
	}
    }

    return (matched_symbols);
}

list
gde_symbols_find_by_application_tag(list symbols, string tag)
{
    list	matched_symbols;
    int		i;
    gde_symbol	symbol;
    gde_arc	garc;
    gde_node	gnode;
    gde_nt	gnt;
    gde_lt	glt;

    matched_symbols = list_create("gde_symbol", 0);
    if (symbols == NULL)
      return (matched_symbols);

    for (i = 0; i < list_count(symbols); i = i + 1)
    {
	symbol = list_get(symbols, i);

	if (gde_symbol_is_node(symbol) || gde_symbol_is_cntx(symbol))
	{
	  gnode = to_gde_node(symbol);
	  gnt = gde_node_nt(gnode);

	  if (gde_nt_application_tag(gnt) == tag)
	    list_append(matched_symbols, symbol);
	} else if (gde_symbol_is_arc(symbol))
	{
	    garc = to_gde_arc(symbol);
	    glt = gde_arc_lt(garc);
	    if (gde_lt_application_tag(glt) == tag)
		list_append(matched_symbols, symbol);
	}
    }
    return (matched_symbols);
}

list
gde_nodes_find_equivalent(list gdenodes, list nodes, node node)
{
    list            matched_symbols;
    int             i;
    node            n;

    matched_symbols = list_create("gde_node", 0);

    if (gdenodes == NULL || nodes == NULL || node == NULL)
	return (matched_symbols);

    if (list_count(gdenodes) != list_count(nodes))
	return (matched_symbols);

    for (i = 0; i < list_count(nodes); i = i + 1)
    {
	if (list_get(nodes, i) == node)
	    list_append(matched_symbols, list_get(gdenodes, i));
    }
    return (matched_symbols);
}

list
gde_symbols_find_with_types(list symbols, list types)
{
    list	matched_symbols;
    int		i, j;
    gde_symbol	symbol;
    gde_arc	garc;
    gde_node	gnode;
    gde_nt	gnt;
    gde_lt	glt;
    string	type_name;

    matched_symbols = list_create("gde_symbol", 0);
    if (symbols == NULL || types == NULL)
	return (matched_symbols);

    for (j = 0; j < list_count(types); j = j + 1)
    {
	type_name = list_get(types, j);
	for (i = 0; i < list_count(symbols); i = i + 1)
	{
	    symbol = list_get(symbols, i);

	    if (gde_symbol_is_node(symbol) || gde_symbol_is_cntx(symbol))
	    {
	      gnode = to_gde_node(symbol);
	      gnt = gde_node_nt(gnode);

	      if (gde_nt_name(gnt) == type_name)
		list_append(matched_symbols, symbol);
	    } else if (gde_symbol_is_arc(symbol))
	    {
		garc = to_gde_arc(symbol);
		glt = gde_arc_lt(garc);
		if (gde_lt_name(glt) == type_name)
		    list_append(matched_symbols, symbol);
	    }
	}
    }
    return (matched_symbols);
}

void
gde_select_symbols_in_list(list symbols)
{
    int		i;
    boolean     first_time = True;
    gde_symbol	symbol;
    gde_arc	selarc;
    gde_node	selnode;

    if (symbols == NULL)
	return;

    for (i = 0; i < list_count(symbols); i = i + 1)
    {
	symbol = list_get(symbols, i);

	if (gde_symbol_is_node(symbol) || gde_symbol_is_cntx(symbol))
	{
	    selnode = to_gde_node(symbol);
	    if (first_time)
	    {
		gde_node_select(selnode);
		first_time = False;
	    } else
		gde_node_adjust(selnode);
	} else if (gde_symbol_is_arc(symbol))
	{
	    selarc = to_gde_arc(symbol);
	    if (first_time)
	    {
		gde_arc_select(selarc);
		first_time = False;
	    } else
		gde_arc_adjust(selarc);
	}
    }
}

void
gde_select_node(gde_node selnode)
{
    if (selnode != NULL)
    {
	gde_builtin(SYM_SEL_APPID_MSG + gde_node_psymid(selnode));
    }
}

void
gde_select_arc(gde_arc selarc)
{
    if (selarc != NULL)
    {
	gde_builtin(SYM_SEL_APPID_MSG + gde_arc_psymid(selarc));
    }
}

void
gde_select_cntx(gde_cntx selcntx)
{
    if (selcntx != NULL)
    {
	gde_builtin(SYM_SEL_APPID_MSG + gde_cntx_psymid(selcntx));
    }
}

gde_node
gde_cntx_to_gde_node(gde_cntx c)
{
    return(to_gde_node(to_gde_symbol(c)));
}



 
gde_cntx
create_cntx_from_arc(gde_arc a, string type)
{
    int         fx, fy, tx, ty, x, y, i, dx, dy, nx, ny;
 
    fx = gde_arc_fromx(a);
    fy = gde_arc_fromy(a);
    tx = gde_arc_tox(a);
    ty = gde_arc_toy(a);
 
    x = (fx + tx) / 2;
    y = (fy + ty) / 2;
 
    return gde_cntx_create(type, a, x, y);
}
 
 

list
gde_node_list_to_sym_list(list symbols)
{
    int		i;
    gde_node	n;
    list	res;


    res = list_create("gde_symbol", 0);
    if (symbols == NULL)
	return res;

    for (i = 0; i < list_count(symbols); i = i + 1)
    {
	n = list_get(symbols, i);
	list_append(res, to_gde_symbol(n));
    }

    return(res);
}

list
gde_cntx_list_to_sym_list(list symbols)
{
    int		i;
    gde_cntx	n;
    list	res;


    res = list_create("gde_symbol", 0);
    if (symbols == NULL)
	return res;

    for (i = 0; i < list_count(symbols); i = i + 1)
    {
	n = list_get(symbols, i);
	list_append(res, to_gde_symbol(n));
    }

    return(res);
}


list
gde_arc_list_to_sym_list(list symbols)
{
    int		i;
    gde_arc	a;
    list	res;


    res = list_create("gde_symbol", 0);
    if (symbols == NULL)
	return res;

    for (i = 0; i < list_count(symbols); i = i + 1)
    {
	a = list_get(symbols, i);
	list_append(res, to_gde_symbol(a));
    }

    return(res);
}



string
gde_arc_get_first_label_dir(gde_arc arc, boolean forward)
{
    string	label;
    gde_node	fn;
    list	arcs;
    int		i;
    gde_arc	ta;


    label = gde_arc_label(arc);

    if (label != NULL)
    {
	if (label != "")
	{
	    return(label);
	}
    }

    if (forward == False)
    {
	fn = gde_arc_fromnode(arc);
	if (gde_nt_name(gde_node_nt(fn)) == VERTEX_SYMBOL_TYPE)
	{
	    arcs = gde_node_inarcs(fn);
	    ta = list_get(arcs, 0);
	    label = gde_arc_get_first_label_dir(ta, forward);
	    if (label != NULL)
	    {
		return(label);
	    }
	}
    } else {
	fn = gde_arc_tonode(arc);
	if (gde_nt_name(gde_node_nt(fn)) == VERTEX_SYMBOL_TYPE)
	{
	    arcs = gde_node_outarcs(fn);
	    ta = list_get(arcs, 0);
	    label = gde_arc_get_first_label_dir(ta, forward);
	    if (label != NULL)
	    {
		return(label);
	    }
	}
    }
    return(NULL);
}

string
gde_arc_get_first_label(gde_arc arc)
{
    string	label;
    gde_arc	c;

    // call by ref in gde types ...
    c = to_gde_arc(to_gde_symbol(arc));

    label = gde_arc_get_first_label_dir(c, True);
    if (label == NULL)
    {
	c = to_gde_arc(to_gde_symbol(arc));
	label = gde_arc_get_first_label_dir(c, False);
    }
    return(label);
}

//
// LIST SEARCHING
//

list
gde_arcs_in_list(list symbols)
{
    list	matched_symbols;
    int		i;
    gde_symbol	symbol;
    gde_arc	garc;

    matched_symbols = list_create("gde_arc", 0);
    if (symbols == NULL)
	return matched_symbols;

    for (i = 0; i < list_count(symbols); i = i + 1)
    {
	symbol = list_get(symbols, i);

	if (gde_symbol_is_arc(symbol))
	{
	    garc = to_gde_arc(symbol);
	    list_append(matched_symbols, garc);
	}
    }

    return (matched_symbols);
}

list
gde_nodes_in_list(list symbols)
{
    list	matched_symbols;
    int		i;
    gde_symbol	symbol;
    gde_node	gnode;

    matched_symbols = list_create("gde_node", 0);
    if (symbols == NULL)
	return matched_symbols;

    for (i = 0; i < list_count(symbols); i = i + 1)
    {
	symbol = list_get(symbols, i);

	if (gde_symbol_is_node(symbol))
	{
	    gnode = to_gde_node(symbol);
	    list_append(matched_symbols, gnode);
	}
    }

    return (matched_symbols);
}


list
gde_contexts_in_list(list symbols)
{
    list	matched_symbols;
    int		i;
    gde_symbol	symbol;
    gde_cntx	gcntx;

    matched_symbols = list_create("gde_cntx", 0);
    if (symbols == NULL)
	return matched_symbols;

    for (i = 0; i < list_count(symbols); i = i + 1)
    {
	symbol = list_get(symbols, i);

	if (gde_symbol_is_cntx(symbol))
	{
	    gcntx = to_gde_cntx(symbol);
	    list_append(matched_symbols, gcntx);
	}
    }

    return (matched_symbols);
}

//
// ARC CREATION
//

string	start_arc_from_target_link_type;
boolean start_an_arc_from_doalign;
const	int	start_an_arc_from_id = 998;
int	defer_save_states_count;
gde_symbol	symbol_arc_started_from;

void
start_an_arc_from(gde_symbol s, string type, string prompt, boolean align)
{
    gde_defer_action_completed(1);
    gde_save_state(); // ECR 6344
    defer_save_states_count = gde_defer_save_state(DISABLE_SAVE_STATE);

    start_arc_from_target_link_type = type;
    start_an_arc_from_doalign = align;
    symbol_arc_started_from = s;

    gde_builtin(SYM_START_APPID_MSG + sym_get_psymid(s));
    print_warning(prompt);
    gde_builtin("EditorActionDone " + start_an_arc_from_id +
		" CreateArcFromCompleted();");

}

int
sym_get_psymid(gde_symbol s)
{
    gde_node	n;
    gde_arc	a;
    int		rc = 0;

    if (gde_symbol_is_node(s) || gde_symbol_is_cntx(s))
    {
	n = to_gde_node(s);
	rc = gde_node_psymid(n);
    } else if (gde_symbol_is_arc(s))
    {
	a = to_gde_arc(s);
	rc = gde_arc_psymid(a);
    }
    return(rc);
}

string		client_arc_completed_cb = NULL;

void
end_an_arc_from_callback(string cb)
{
    client_arc_completed_cb = cb;
}

boolean alreadyInCreateArcFromCompleted = False;

void
CreateArcFromCompleted()
{
    list	selected_symbols;
    int		cnt, i;
    gde_symbol	symbol;
    gde_node	fn;
    gde_arc	garc;
    string	ct;

    //
    // We need to watch for the possibility of recursive entry into this
    // function. This function calls gde_builtin, which results in
    // action_completed getting called. And we have registered this function
    // to be called whenever action_completed gets called.
    // What we used to do to avoid this was cancel the registration of this
    // function as an action_completed callback by calling
    // gde_builin("EditorActionDone ...") as soon as we got into this
    // function. But that caused problems because it resulted in the
    // premature evaluation of dirty mappings (the mappings were evaluated
    // during the action_completed that resulted from calling gde_builtin).
    // We want to make sure that the mappings are not evaluated until the
    // gde_builtin(SYM_REPLACE_MSG ...) is executed. Otherwise commands like
    // Create Aggregation hierarchy in the OMT ome editor will not work
    // properly.
    //

    if (alreadyInCreateArcFromCompleted)
	return;
    
    alreadyInCreateArcFromCompleted = True;
    
    selected_symbols = gde_selected_symbols();

    cnt = 0;
    if (start_arc_from_target_link_type != NULL || 
	client_arc_completed_cb != NULL)
    {
	ct = start_arc_from_target_link_type;
	for (i = 0; i < list_count(selected_symbols); i = i + 1)
	{
	    symbol = list_get(selected_symbols, i);

	    if (gde_symbol_is_arc(symbol))
	    {
		garc = to_gde_arc(symbol);
		fn = gde_arc_fromnode(garc);
		if (fn == NULL)
		    continue;
		// This is the only way to find out if the
		// operation was successful or if it was terminated
		// with an undo ....
		if (symbol_arc_started_from == to_gde_symbol(fn))
		{
		    if (ct != gde_lt_name(gde_arc_lt(garc)))
		    {
			ct = "";
		    }
		    cnt = cnt + 1;
		}
	    }
	}
	if (start_arc_from_target_link_type != NULL)
	{
	    if (cnt > 0 && ct != start_arc_from_target_link_type)
	    {
		gde_builtin(SYM_REPLACE_MSG + start_arc_from_target_link_type);
	    }
	}
    }
    if (start_an_arc_from_doalign == True)
    {
	gde_builtin(BUF_ALIGN_MSG); 
    }
    if (client_arc_completed_cb != NULL && cnt > 0)
    {
	qrl_eval(client_arc_completed_cb);
    }
    client_arc_completed_cb = NULL;

    //
    // Now cancel the registration of this function as a callback upon
    // action_completed
    //

    gde_builtin("EditorActionDone " + start_an_arc_from_id);

    alreadyInCreateArcFromCompleted = False;
    
    //
    // We used to reset the defer action completed flag here and then
    // explicitly call action completed. But it is simpler to just leave
    // the editor in a state where it is deferring action completed
    // calls until the arc insertion is terminitated interactively.
    // When that happens, the defer action completed flag is reset and
    // action completed is called. (Actually, since the defer action
    // completed flag has already been reset due to the fact that we
    // interactively completed an arc insertion, any of the above
    // gde_builtin calls will cause action_completed to get called.)
    //

    gde_defer_save_state(-defer_save_states_count);
}

int
gde_do_defer_save_state()
{
    int		rc;

    gde_defer_action_completed(1);
    rc = gde_defer_save_state(DISABLE_SAVE_STATE);
    return(rc);
}

void
gde_do_allow_save_state(int v)
{
    gde_defer_action_completed(0); // Allow more action completed calls
    gde_action_completed(); // Call action completed now
    gde_defer_save_state(-v);
}

int
gde_start_edit_transaction()
{
    int		dodefer;
    dodefer = gde_defer_handle_node_sym(1);
    gde_defer_action_completed(1);
    gde_defer_save_state(DISABLE_SAVE_STATE);
    return(dodefer);
}

int
gde_end_edit_transaction(int action, int diddefer, boolean updatedisplaymarks)
{
    int		rc = 0;

    gde_defer_action_completed(0); // Allow more action completed calls
    gde_defer_save_state(ENABLE_SAVE_STATE);
    if (diddefer == 0)
    {
	rc = gde_defer_handle_node_sym(0);
    }
    if (action > 0)
    {
	gde_handle_node_sym(action, 0);
    }
    gde_action_completed(); // Call action completed now
    if (updatedisplaymarks)
    {
	gde_builtin(UPDATE_DMARK_MSG);
    }
    return(rc);
}

int
idehook_single_link_selected_active_func()
{
    int		rc = ACTIVE_FUNC_IS_INACTIVE;
    list	symbols;

    symbols = gde_selected_symbols();

    if (list_count(gde_arcs_in_list(symbols)) == 1)
    {
	rc = ACTIVE_FUNC_IS_ACTIVE;
    }

    return(rc);
}

int
gde_name_search_active_func()
{
    int		rc = ACTIVE_FUNC_IS_INACTIVE;
    gde_mapsymbol ms;

    ms = gde_selected_mapsymbol();

    if (ms == NULL)
	return(ACTIVE_FUNC_IS_INACTIVE);
    
    if (gde_mapsymbol_is_node(ms))
    {
	gde_node n = to_gde_node(ms);
	
	if (gde_nt_can_have_label(gde_node_nt(n)))
	    rc = ACTIVE_FUNC_IS_ACTIVE;
    } else if (gde_mapsymbol_is_link(ms))
    {
	gde_link l = to_gde_link(ms);
	
	if (gde_lt_can_have_label(gde_link_lt(l)))
	    rc = ACTIVE_FUNC_IS_ACTIVE;
    } else if (gde_mapsymbol_is_cntx(ms))
    {
	gde_cntx c = to_gde_cntx(ms);
	
	if (gde_ct_can_have_label(gde_cntx_ct(c)))
	    rc = ACTIVE_FUNC_IS_ACTIVE;
    }

    return(rc);
}

int
gde_update_dmarks_active_func()
{
    if (gde_dmark_eval_state() == False)
	return(ACTIVE_FUNC_IS_INACTIVE);

    return(ACTIVE_FUNC_IS_ACTIVE);
}

list
find_cntxs_on_parent(gde_arc a, string type)
{
    gde_cntx	ca;
    int		fx, fy, tx, ty, x, y, i, dx, dy, nx, ny;
    list	symbols;
    gde_symbol	s, as, ps;
    gde_node	n;
    list	cxs;

    cxs = list_create("gde_cntx", 0);

    as = to_gde_symbol(a);
    symbols = gde_all_symbols();

    for(i = 0; i < list_count(symbols); i = i + 1)
    {
	s = list_get(symbols, i);
	if (gde_symbol_is_cntx(s))
	{
	    n = to_gde_node(s);
	    if (type == NULL || gde_nt_name(gde_node_nt(n)) == type)
	    {
		ps = gde_node_parent(n);
		if (ps == as)
		{
		    ca = to_gde_cntx(s);
		    if (ca != NULL)
		    {
			list_append(cxs, ca);
		    }
		}
	    }
	}
    }

    return(cxs);
}

void
gde_move_contexts(string type, gde_arc a, gde_arc sa)
{
    list	cxs, symbols;

    cxs = find_cntxs_on_parent(a, type);

    if (list_count(cxs) > 0)
    {
	symbols = gde_cntx_list_to_sym_list(cxs);
	list_append(symbols, to_gde_symbol(sa));
	gde_select_symbols_in_list(symbols);
	gde_builtin(SYM_ATTACH_PA_MSG);
    }
}


set	the_reversed_link_segments = NULL;


void
reverse_arcs_link(gde_arc garc)
{
    string	tp;

    if (the_reversed_link_segments == NULL)
    {
	the_reversed_link_segments = set_create("gde_arc");
    }

    set_add(the_reversed_link_segments, garc);
    tp = gde_lt_name(gde_arc_lt(garc));
    add_links_of_same_type(tp, garc);

    reverse_arcs_in_set(the_reversed_link_segments, tp);

    set_clear(the_reversed_link_segments);
}


void
add_links_of_same_type(string type, gde_arc arc)
{
    gde_node	fn, tn;
    list	arcs;
    int		i;

    fn = gde_arc_fromnode(arc);
    tn = gde_arc_tonode(arc);

    if (gde_nt_name(gde_node_nt(fn)) == VERTEX_SYMBOL_TYPE)
    {
	arcs = gde_node_inarcs(fn);
	for(i = 0; i < list_count(arcs); i = i + 1)
	{
	    arc = list_get(arcs, i);
	    if (gde_lt_name(gde_arc_lt(arc)) == type)
	    {
		if (set_is_member(the_reversed_link_segments, arc) == False)
		{
		    set_add(the_reversed_link_segments, arc);
		    add_links_of_same_type(type, arc);
		}
	    }
	}
    }

    if (gde_nt_name(gde_node_nt(tn)) == VERTEX_SYMBOL_TYPE)
    {
	arcs = gde_node_outarcs(tn);
	for(i = 0; i < list_count(arcs); i = i + 1)
	{
	    arc = list_get(arcs, i);
	    if (gde_lt_name(gde_arc_lt(arc)) == type)
	    {
		if (set_is_member(the_reversed_link_segments, arc) == False)
		{
		    set_add(the_reversed_link_segments, arc);
		    add_links_of_same_type(type, arc);
		}
	    }
	}
    }
}

void
reverse_arcs_in_set(set arcs, string type)
{
    gde_arc	a, na = NULL;
    boolean	selthis = False;
    gde_node	fn, tn;
    int		i;
    boolean	ok = True;
    string	label;
    int		arc_id;
    list	syms;

    gde_defer_save_state(DISABLE_SAVE_STATE);

    syms = list_create("gde_symbol", 0);

    for(i = 0; i < set_count(arcs); i = i + 1)
    {
	a = set_get_element(arcs, i);
	label = gde_arc_label(a);
	fn = gde_arc_fromnode(a);
	tn = gde_arc_tonode(a);
	na = gde_arc_create(type, tn, fn);
	if (na == NULL)
	{
	    ok = False;
	} else {
	    if (selthis == False)
	    {
		list_append(syms, to_gde_symbol(na));
		selthis = True;
	    }
	    gde_move_contexts(NULL, a, na);
	    if (label == NULL)
	    {
		continue;
	    }
	    if (string_length(label) > 0)
	    {
		gde_arc_set_label(na, label);
	    }
	}
    }

    if (ok == True)
    {
	for(i = 0; i < set_count(arcs); i = i + 1)
	{
	    a = set_get_element(arcs, i);
	    gde_arc_delete(a);
	}
    }
    
    if (list_count(syms) != 0)
    {
	gde_select_symbols_in_list(syms);
    }

    gde_defer_save_state(ENABLE_SAVE_STATE);
}

//
// ALIGN RELATED FUNCTIONS
//

void
AlignAllArcs()
{
    DoAlignArcsOfType(NULL);
}


void
DoAlignArcsOfType(string arctp)
{
    int		i;
    int		fx, fy, tx, ty;
    gde_symbol	symbol;
    list	symbols;
    list	selected_symbols;
    list        align_operands;
    gde_arc	selarc;
    string	symtype;

    selected_symbols = gde_selected_symbols();
    symbols = gde_all_symbols();

    align_operands = list_create("gde_symbol", 0);
    
    for (i = 0; i < list_count(symbols); i = i + 1)
    {
	symbol = list_get(symbols, i);

	if (gde_symbol_is_arc(symbol))
	{
	    selarc = to_gde_arc(symbol);
	    if (arctp != NULL)
	    {
		symtype = gde_lt_name(gde_arc_lt(selarc));
		if (arctp != symtype)
		{
		    continue;
		}
	    }
	    fx = gde_arc_fromx(selarc);
	    fy = gde_arc_fromy(selarc);
	    tx = gde_arc_tox(selarc);
	    ty = gde_arc_toy(selarc);

	    if (fx == tx || fy == ty)
	    {
		continue;
	    }
	    if (gde_arc_canbevisible(selarc) == 0)
	    {
		continue;
	    }
	    
	    list_append(align_operands, symbol);
	}
    }

    if (list_count(align_operands) > 0)
    {
	gde_select_symbols_in_list(align_operands);
	gde_builtin(BUF_ALIGN_MSG);
    }

    gde_select_symbols_in_list(selected_symbols);
}

gde_symbol local_rename_symbol;
int        local_rename_psymid;

const string INVALID_RENAME_SELECTION = "original object rename selection has been changed, rename cancelled.";

//
// this function makes sure that the user has not changed the selection between
// the time the Rename dialog was invoked and the time the Apply button
// was hit. If the selection has changed, then cancel the whole operation.
//

boolean
validate_rename_selection()
{
    list selected_symbols;
    gde_symbol symbol;
    gde_node node;
    gde_arc arc;
    gde_cntx cntx;

    selected_symbols = gde_selected_symbols();
    if (list_count(selected_symbols) != 1)
    {
	gde_print_error(INVALID_RENAME_SELECTION);
	return (False);
    }

    symbol = list_get(selected_symbols, 0);
    if (gde_symbol_is_node(symbol))
    {
	node = to_gde_node(symbol);
	if (gde_node_psymid(node) != local_rename_psymid)
	{
	    gde_print_error(INVALID_RENAME_SELECTION);
	    return (False);
	}
    } else if (gde_symbol_is_arc(symbol))
    {
	arc = to_gde_arc(symbol);
	if (gde_arc_psymid(arc) != local_rename_psymid)
	{
	    gde_print_error(INVALID_RENAME_SELECTION);
	    return (False);
	}
    } else
    {
	cntx = to_gde_cntx(symbol);
	if (gde_cntx_psymid(cntx) != local_rename_psymid)
	{
	    gde_print_error(INVALID_RENAME_SELECTION);
	    return (False);
	}
    }

    return (True);
}

void
gde_local_object_rename_cb()
{
    string new_label;

    if (editor_text_selector_action() == "Cancel")
	return;

    if (validate_rename_selection() == False)
	return;

    new_label = editor_text_selector_value();

    if (gde_symbol_is_node(local_rename_symbol))
	gde_node_local_rename(to_gde_node(local_rename_symbol), new_label);
    else if (gde_symbol_is_arc(local_rename_symbol))
	gde_arc_local_rename(to_gde_arc(local_rename_symbol), new_label);
    else
	gde_cntx_local_rename(to_gde_cntx(local_rename_symbol), new_label);
}

void
gde_local_object_rename()
{
    list selected_symbols;
    gde_symbol symbol;
    gde_node node;
    gde_arc arc;
    gde_cntx cntx;
    string label;

    //
    // we're assuming that this function will not be called
    // unless a single symbol is selected. This should be enforced
    // by the ActiveFunc that controls the Object Rename menu item.
    //

    selected_symbols = gde_selected_symbols();
    symbol = list_get(selected_symbols, 0);
    local_rename_symbol = symbol;

    if (gde_symbol_is_node(symbol))
    {
	node = to_gde_node(symbol);
	local_rename_psymid = gde_node_psymid(node);
	label = gde_node_label(node);
    } else if (gde_symbol_is_arc(symbol))
    {
	arc = to_gde_arc(symbol);
	local_rename_psymid = gde_arc_psymid(arc);
	label = gde_arc_label(arc);
    } else
    {
	cntx = to_gde_cntx(symbol);
	local_rename_psymid = gde_cntx_psymid(cntx);
	label = gde_cntx_label(cntx);
    }
    
    editor_text_selector(label, "gde_local_object_rename_cb();",
			"Rename Object", "New Name:", "Rename", "Cancel");
}

string
gde_get_symbol_type(gde_symbol symbol)
{
    gde_arc	a;
    gde_node	n;

    if (!gde_symbol_valid(symbol))
    {
	return(NULL);
    }
    if (gde_symbol_is_node(symbol) || gde_symbol_is_cntx(symbol))
    {
	n = to_gde_node(symbol);
	return(gde_nt_name(gde_node_nt(n)));
    }
    if (gde_symbol_is_arc(symbol))
    {
	a = to_gde_arc(symbol);
	return(gde_lt_name(gde_arc_lt(a)));
    }
    return(NULL);
}

struct	RevReplVertTp
{
    gde_node	orig;
    gde_node	new;
};

gde_node
reverse_arc_find_replaced_node(list l, gde_node o)
{
    int		i;
    RevReplVertTp	n;

    for(i = 0; i < list_count(l); i = i + 1)
    {
	n = list_get(l, i);
	if (n.orig == o)
	    return(n.new);
    }
    return(o);
}

void
reverse_arcs_in_set_with_new_type(set arcs, string type)
{
    gde_arc	a, na = NULL;
    boolean	selthis = False;
    gde_node	fn, tn, nv;
    int		i;
    boolean	ok = True;
    string	label;
    int		arc_id;
    list	syms, repl;
    RevReplVertTp	rn;

    repl = list_create("RevReplVertTp", 0);
    gde_defer_save_state(DISABLE_SAVE_STATE);

    syms = list_create("gde_symbol", 0);

    for(i = 0; i < set_count(arcs); i = i + 1)
    {
	a = set_get_element(arcs, i);
	label = gde_arc_label(a);
	fn = gde_arc_fromnode(a);
	if (gde_nt_name(gde_node_nt(fn)) == VERTEX_SYMBOL_TYPE)
	{
	    nv = reverse_arc_find_replaced_node(repl, fn);
	    if (nv == fn)
	    {
		nv = gde_node_create(VERTEX_SYMBOL_TYPE,
			gde_node_x(fn), gde_node_y(fn));
		if (nv != NULL)
		{
		    rn.new = nv;
		    rn.orig = fn;
		    list_append(repl, rn);
		    fn = nv;
		}
	    } else {
		fn = nv;
	    }
	}
	tn = gde_arc_tonode(a);
	if (gde_nt_name(gde_node_nt(tn)) == VERTEX_SYMBOL_TYPE)
	{
	    nv = reverse_arc_find_replaced_node(repl, tn);
	    if (nv == tn)
	    {
		nv = gde_node_create(VERTEX_SYMBOL_TYPE,
			gde_node_x(tn), gde_node_y(tn));
		if (nv != NULL)
		{
		    rn.new = nv;
		    rn.orig = tn;
		    list_append(repl, rn);
		    tn = nv;
		}
	    } else {
		tn = nv;
	    }
	}
	na = gde_arc_create(type, tn, fn);
	if (na == NULL)
	{
	    ok = False;
	} else {
	    if (selthis == False)
	    {
		list_append(syms, to_gde_symbol(na));
		selthis = True;
	    }
	    gde_move_contexts(NULL, a, na);
	    if (label == NULL)
	    {
		continue;
	    }
	    if (string_length(label) > 0)
	    {
		gde_arc_set_label(na, label);
	    }
	}
    }

    if (ok == True)
    {
	for(i = 0; i < set_count(arcs); i = i + 1)
	{
	    a = set_get_element(arcs, i);
	    gde_arc_delete(a);
	}
    }
    
    if (list_count(syms) != 0)
    {
	gde_select_symbols_in_list(syms);
    }

    gde_defer_save_state(ENABLE_SAVE_STATE);
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


////////////////////////////////////////////////////////////////////////////////
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
	    gde_print_error("Could not save diagram.");
	    return False;
    }
// ECR 7336 end

    gde_reset_canvas_state();
    name = gde_diagram_name();
    if (name == NULL || name == "")
    {
	if (gde_builtin("SaveasDialogShow"))
	    return False;
	else return True;
    }

    // SPR 2535 - replace spaces with underscores - removed code segment here

    if (gde_builtin("BufferCheckSyntax"))
    {
	if (editor_confirm(
	       "The diagram has syntax errors, do you want to save it anyway?",
	       "Yes", "No") == False)
	    return False;
	syntax_errors = True;
    }

// ECR 6379 begin
    boolean dest_exists = True;
    string dest_dir = path_part(gde_file_name());
    if (!file_exists(dest_dir))
    {
        // try to create it
        if (mkdir(dest_dir) == -1)
            dest_exists = False;
    }
// ECR 6379 end

    if (!dest_exists || file_save_no_update())
    {
	// SPR 2535 - replace spaces with underscores - added line below
	name = gde_diagram_name();
	gde_print_error("Could not save diagram '" + name + "'");
	return False;
    }

    editor_set_is_busy(True);

    gde_commit_to_repository(syntax_errors);

    editor_set_is_busy(False);

    // SPR 2535 - replace spaces with underscores - added line below
    name = gde_diagram_name();
    gde_print_message("The diagram '" + name + "' has been saved.");

    return True;
}

//
// The following routines are used to implement the standard name completion
// behavior for gde.
//
 
void
nc_get_node_names_from_buffer(list names, string query, string pattern,
			      string extent, string apptype, int scopeid, boolean ignore_scope)
{
    node n, cn;
    int i;
    list nodes;
    list candidate_buffer_nodes = list_create("node", 0);
    
    nodes = gde_all_mapped_oms_nodes();
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
    
    links = gde_all_mapped_oms_links();
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
    
    cntxs = gde_all_mapped_oms_cntxs();
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
    // gde_all_mapped_oms_* functions) and test their apptype,
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
gde_default_name_completion_func(string pattern, string label, string extent,
				 string apptype, int scopeid)
{
    list names;

    names = list_create("string", 0);

    if (extent == NULL || extent == "" || apptype == NULL || apptype == "")
	return (names);

    nc_get_names_from_repository(names, pattern, extent, apptype, scopeid, False);
    nc_get_names_from_buffer(names, pattern, extent, apptype, scopeid, False);

    return (names);
}

list
gde_ignore_scope_name_completion_func(string pattern, string label,
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

void
debug_mapsyms(string msg, list l)
{
    int		i;
    list	arcs;
    gde_mapsymbol	ms;

    message("\n" + msg + ": " + list_count(l) + " mapsymbols");

    for(i = 0; i < list_count(l); i = i + 1)
    {
	ms = list_get(l, i);
	if (gde_mapsymbol_is_node(ms))
	{
	    print_node("\t", to_gde_node(ms));
	} else if (gde_mapsymbol_is_link(ms))
	{
	    arcs = gde_link_arcs(to_gde_link(ms));
	    print_arc("\t", list_get(arcs, 0));
	} else if (gde_mapsymbol_is_cntx(ms))
	{
	    print_cntx("\t", to_gde_cntx(ms));
	    continue;
	}
    }
}

list
gde_symbol_mapresults(gde_symbol s)
{
    list mapresults;
    list links;
    list result;
    int i;
    gde_link link;

    if (gde_symbol_is_node(s))
    {
	result = gde_node_mapresults(to_gde_node(s));
	return result;
    }

    if (gde_symbol_is_cntx(s))
    {
	result = gde_cntx_mapresults(to_gde_cntx(s));
	return result;
    }

    mapresults = list_create("gde_mapresult", 0);

    links = gde_arc_links(to_gde_arc(s));
    for (i = 0; i < list_count(links); i = i + 1)
    {
	link = list_get(links, i);
	list_concatenate(mapresults, gde_link_mapresults(link));
    }

    return mapresults;
}

int
symbol_object_id(gde_symbol s, string mappingname)
{
    int		id, i;
    list	r;
    gde_mapresult	amap;

    id = 0;

    if (s != NULL)
    {
	r = gde_symbol_mapresults(s);
	if (r == NULL)
	{
	    return(id);
	}
	for(i = 0; id == 0 && i < list_count(r); i = i + 1)
	{
	    amap = list_get(r, i);
	    if (amap.mappingname == mappingname)
	    {
		if (amap.node != NULL)
		{
		    id = amap.node.id;
		} else if (amap.link != NULL)
		{
		    id = amap.link.id;
		} else if (amap.cntx != NULL)
		{
		    id = amap.cntx.id;
		}
	    }
	}
    }
    return(id);
}

list
symbol_object(gde_symbol s, string mappingname)
{
    int             i;
    list            r;
    gde_mapresult   amap;
    list            object;

    object = list_create("all", 0);
    list_append(object, NULL);

    if (s != NULL)
    {
	r = gde_symbol_mapresults(s);
	if (r == NULL)
	{
	    return (object);
	}
	for (i = 0; i < list_count(r); i = i + 1)
	{
	    amap = list_get(r, i);
	    if (amap.mappingname == mappingname)
	    {
		if (amap.node != NULL)
		{
		    list_set(object, 0, amap.node);
		    return object;
		} else if (amap.link != NULL)
		{
		    list_set(object, 0, amap.link);
		    return object;
		} else if (amap.cntx != NULL)
		{
		    list_set(object, 0, amap.cntx);
		    return object;
		}
	    }
	}
    }
    return (object);
}

gde_cntx
gde_first_arcs_links_cntx_from_ct(gde_arc a, string ct)
{
    int             i, j;
    gde_cntx        cntx;
    list            cntxs;
    list            links;

    if (a == NULL)
	return NULL;

    links = gde_arc_links(a);
    for (i = 0; i < list_count(links); i = i + 1)
    {
	cntxs = gde_link_cntxs(list_get(links, i));
	for (j = 0; j < list_count(cntxs); j = j + 1)
	{
	    cntx = list_get(cntxs, j);
	    if (gde_ct_name(gde_cntx_ct(cntx)) == ct)
		return cntx;
	}
    }
    return NULL;
}

void
gde_mapsymbol_default_double_click(gde_mapsymbol ms)
{
    gde_builtin("SymbolAnnotEdit");
}

void
double_click()
{
    list links;
    gde_symbol sym;
    gde_mapsymbol ms;
    list symbols = gde_selected_symbols();
    
    if (list_count(symbols) != 1)
    {
	gde_print_error("Please select a single symbol or arc before double clicking.");
	return;
    }
    
    sym = list_get(symbols, 0);
    
    if (gde_symbol_is_node(sym))
	ms = to_gde_mapsymbol(to_gde_node(sym));
    else if (gde_symbol_is_cntx(sym))
	ms = to_gde_mapsymbol(to_gde_cntx(sym));
    else
    {
	links = gde_arc_links(to_gde_arc(sym));

	if (list_count(links) != 1)
	{
	    gde_print_error("Please select a single symbol or arc before double clicking.");
	    return;
	}

	ms = to_gde_mapsymbol(list_get(links, 0));
    }
    
    editor_set_is_busy(True);
    gde_mapsymbol_invoke_method(ms, "DoubleClick");
    editor_set_is_busy(False);
}

void
LoadFileByObjNameType(string name, string app_type, string pdm_type)
{
    file            fo;
    node_ref	    nr;

    name = to_oms_string(name);
    for_each_in_select("${pdm_type}_ref[${pdm_type}[${app_type} & name='${name}']]", nr)
    {
	fo = find_by_query("file[${nr.file_id}]");
	if (fo != NULL)
	{
	    if (current_editor_apptype() == fo.type)
	    {
		NavLoadAndFind(fo.name, nr.appid);
		return;
	    }
	}
    }
    return;
}


// routines to filter out comment links from the normal gde_in and gde_out
// links stuff. 

list
gde_node_inarcs_no_comments(gde_node gn)
 
{
    int i;
    list arcs;
    gde_arc arc;
 
    arcs = gde_node_inarcs(gn);
 
    for (i = 0; i < list_count(arcs); i++)
    {
        arc = list_get(arcs, i);
        if (gde_lt_name(gde_arc_lt(arc)) == "CommentLink")
        {
            list_delete(arcs, i);
            i--;
        }
    }
    return arcs;
}

list
gde_node_outarcs_no_comments(gde_node gn)
 
{
    int i;
    list arcs;
    gde_arc arc;
 
    arcs = gde_node_outarcs(gn);
 
    for (i = 0; i < list_count(arcs); i++)
    {
        arc = list_get(arcs, i);
        if (gde_lt_name(gde_arc_lt(arc)) == "CommentLink")
        {
            list_delete(arcs, i);
            i--;
        }
    }
    return arcs;
}


// this is used by Booch, OMT and UML so I have moved it into the Core
gde_symbol
find_class_by_psymid(int psymid)
{
    gde_node        gnode;
    gde_cntx        gcntx;
    gde_symbol      class = NULL;

    gnode = gde_node_find_by_psymid(psymid);
    if (gnode != NULL)
    {
	class = to_gde_symbol(gnode);
    } else
    {
	gcntx = gde_cntx_find_by_psymid(psymid);
	if (gcntx != NULL)
	    class = to_gde_symbol(gcntx);
    }

    return class;
}


const	int	LABEL_LINE_LENGTH = 80;

string
make_multi_line_label_from_string(string label)
{
    string	l, r, t, s1, s2;
    list	ll;
    int		i, b;
    int		chrs;

    // message("input: " + label);
    if (string_length(label) <= LABEL_LINE_LENGTH)
    {
	return(label);
    }
    ll = string_to_list(label, "\n \t");
    r = "";
    l = "";
    chrs = 0;
    for(i = 0; i < list_count(ll); i = i + 1)
    {
	t = list_get(ll, i);
	if ((chrs + string_length(t) + string_length(l)) <= LABEL_LINE_LENGTH)
	{
	    if (chrs > 0)
	    {
		l = l + " ";
	    }
	    l = l + t;
	    chrs = 1;
	} else {
	    if (chrs > 0)
	    {
		r = r + l + "\n";
	    }
	    while(string_length(t) > LABEL_LINE_LENGTH)
	    {
		s1 = string_extract(t, 0, LABEL_LINE_LENGTH);
		r = r + s1 + "\n";
		b = string_length(t);
//< ECR fix:3377 begin>
//		if (b > 2*LABEL_LINE_LENGTH)
//		{
		    b = b-LABEL_LINE_LENGTH;
//		} else {
//		    b = LABEL_LINE_LENGTH;
//		}
//< ECR fix:3377 end>
		t = string_extract(t, LABEL_LINE_LENGTH, b); 
	    }
	    l = t;
	    if (string_length(l) > 0)
	    {
		chrs = 1;
	    } else {
		chrs = 0;
	    }
	}
    }
    if (chrs > 0)
    {
	r = r + l;
    }
    // message("output: " + r);
    return(r);
}

string
make_balanced_multi_line_label_from_string(string label, int numlines, int minl, string sep)
{
    string	l, r, t, s1, s2;
    list	ll;
    int		i, b, cpln;
    int		chrs;

    if (sep == NULL)
    {
	sep = "\n \t";
    }
    cpln = string_length(label) / numlines;
    if (cpln < minl)
	cpln = minl;
    if (cpln > LABEL_LINE_LENGTH)
	cpln = LABEL_LINE_LENGTH;
    if (string_length(label) <= cpln)
    {
	return(label);
    }

    ll = string_to_list(label, sep);
    r = "";
    l = "";
    chrs = 0;
    for(i = 0; i < list_count(ll); i = i + 1)
    {
	t = list_get(ll, i);
	if ((chrs + string_length(t) + string_length(l)) <= cpln)
	{
	    if (chrs > 0)
	    {
		l = l + " ";
	    }
	    l = l + t;
	    chrs = 1;
	} else {
	    if (chrs > 0)
	    {
		r = r + l + "\n";
	    }
	    while(string_length(t) > cpln)
	    {
		s1 = string_extract(t, 0, cpln);
		r = r + s1 + "\n";
		b = string_length(t);
		if (b > 2*cpln)
		{
		    b = cpln;
		} else {
		    b = b-cpln;
		}
		t = string_extract(t, cpln, b); 
	    }
	    l = t;
	    if (string_length(l) > 0)
	    {
		chrs = 1;
	    } else {
		chrs = 0;
	    }
	}
    }
    if (chrs > 0)
    {
	r = r + l;
    }
    // message("output: " + r);
    return(r);
}

boolean
AlwaysTrue()
{
    return True;
}

// Begin Add ECR 4049
int
never_active()
{
    return ACTIVE_FUNC_IS_INACTIVE;
}
// End Add ECR 4049

// Begin Add ECR4048
void
new_file()
{
    gde_builtin( "BufferClear" ); // calls file save if modified on its own
    gde_builtin( "EditorSetFileName \"\"" );
    gde_print_message( "New diagram created." );
    // gde_builtin( "FileSave" );
}
// End Add ECR4048

// Begin Add ECR 149
void
gde_toggle_draw_ortho()
{
    // Added write_profile for ECR 4980
    if( ui_toolbar_is_button_selected("Standard", "OrthogonalArcs") )
    {
        gde_draw_ortho(True);
        editor_write_profile_string( "DrawOrthoArcs", "True" );
    }
    else
    {
        gde_draw_ortho(False);
        editor_write_profile_string( "DrawOrthoArcs", "False" );
    }
}
// End Add ECR 149

// ECR 3789 begin
const boolean debug_edit_nd_shortcut = False;

void ps_external_edit_nd( int objid, string notetype )
{

    string notedesctxt = "";
	
	
    note descNote = find_by_query( "note[${notetype} && obj_id = ${objid}]" );
    if( descNote != NULL )
	notedesctxt = descNote.desc;

    // If the string is the emptystring, i.e. there is no or an empty notedescription
    // we check, if there is an description template and include that, if it is
    // available
    //		
    if( "" == notedesctxt )
    {
	if( debug_edit_nd_shortcut )
	    message( "Checking for document template..." );
	node nod = find_by_query( "node[id = ${objid}]" );
	if( debug_edit_nd_shortcut )
	{		  
	    message( app_type_print_string( nod.type ) );
	    message( to_string( nod.type ) );
	    message( "GetDefaultDescription( "+to_string(objid)+
		     ", "+app_type_print_string( nod )+", "+notetype+" );");
	}
		
	notedesctxt = GetDefaultDescription( objid, app_type_print_string( nod ), 
					     notetype );
    }
	

    string tmp_file = temp_file( "note","" );	
    write_file( tmp_file, notedesctxt );
    ps_edit_note_desc( tmp_file );
    string newtext = read_file( tmp_file );
	
    // Only call oae if value changed....
	
    if( newtext != notedesctxt )
    {
	if( debug_edit_nd_shortcut )
	    message( "Description changed" );
					
	string user_set_annot_file = temp_file( "PSSET", "" );
	write_file( user_set_annot_file,
		    to_string(objid) +":"+notetype+":NULL:NULL:"+tmp_file+"\n" );
						
	int handle = editor_send_msg( "ngoae",
				      "EditorQrlEval SetAnnotValuesFromFiles(\""+
				      string_escape(user_set_annot_file,"\\")+"\");", 
				      -1 );
			
	editor_send_msg( "ngoae",
			 "EditorQrlEval 	CleanAnnotTmpFiles(\""+
			 string_escape(user_set_annot_file,"\\")+"\");", 
			 handle );
    }
    else
    {
	// 000328-02 AG: Improve tmp_file cleanup
	//
	delete_file( tmp_file );
    }
}


void 
ps_gde_edit_nd_cb(string notetype )
{
	gde_symbol sym = list_get(gde_selected_symbols(), 0);

    gde_node gn = NULL;
    gn = to_gde_node(sym);
    list l = gde_node_oms_nodes(gn);
    node n = list_get( l, 0 );
    
    ps_external_edit_nd( n.id, notetype );
    
}
// ECR 3789 end

// ECR 4061
string
SymbolNameSearchLabelFunc()
{
    string type = "";
    list sel = gde_selected_symbols();
    if( list_count( sel ) > 0 )
        type = gde_get_symbol_type( list_get( sel, 0 ) );
    if( type != "" && type != NULL )        
        return "Choose " + type + " &Names...";
    else
        return "Choose &Names...";
}
// ECR 4961

// ECR 7475
string
SymbolRenameLabelFunc()
{
    string type = "";
    list sel = gde_selected_symbols();
    if( list_count( sel ) > 0 )
        type = gde_get_symbol_type( list_get( sel, 0 ) );
    if( type != "" && type != NULL )        
        return "Rena&me " + type + " Systemwide...";
    else
        return "Rena&me Systemwide...";
}
// ECR 7475 end

/* ------------------------------------------------------------------------- */
 
//  Add any user customizations to the file included below:
#include_if_exists "user/ct/rules/qrl/user_gde_qrl_std.inc"

