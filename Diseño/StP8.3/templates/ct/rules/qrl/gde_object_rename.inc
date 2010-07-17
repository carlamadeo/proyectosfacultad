//
// 	StP/Core
// 	Confidential property of Interactive Development Environments, Inc.
// 	Copyright (c) 1992-1993
// 	All rights reserved
//

#include "rules/qrl/test_clone.qrl"

gde_mapsymbol gor_current_mapsym;
list gor_objects;
node gor_node;
link gor_link;
cntx gor_cntx;
string gor_type;
string gor_current_label = "";
const	string		GDE_GOR_REN_DEP_SELF = "Self";
const	string		GDE_GOR_REN_DEP_NAME = "Name";
boolean gde_rename_in_progress_flag = False;

boolean
gde_rename_in_progress()
{
    return gde_rename_in_progress_flag;
}

void gde_set_rename_in_progress(boolean flag)
{
    gde_rename_in_progress_flag = flag;
}

void
gde_default_global_object_rename(gde_mapsymbol ms)
{
    gde_builtin("PropertySheetShow ObjectRename");
    gde_default_rename_clear_new_label("ObjectRename");
    gde_default_global_object_rename_init(ms);
}

struct gde_default_new_label_tp
{
    string oname;
    gde_mapsymbol ms;
    string label;
};

list h_gde_default_new_labels = list_create( "gde_default_new_label_tp", 0 );

void
gde_default_rename_clear_new_label(string oname)
{
    int i;
    gde_default_new_label_tp h_gde_default_new_label;
    for ( i = list_count(h_gde_default_new_labels) - 1; i >= 0 ; i = i - 1)
    {
        h_gde_default_new_label = list_get(h_gde_default_new_labels, i);
        if (oname == h_gde_default_new_label.oname)
        {
           list_delete(h_gde_default_new_labels,i);
        }
    }
}


string
gde_default_rename_fetch_new_label(string oname, gde_mapsymbol ms)
{
    int i;
    gde_default_new_label_tp h_gde_default_new_label;
    string label = "";
    for ( i = list_count(h_gde_default_new_labels) - 1; i >= 0 ; i = i - 1)
    {
        h_gde_default_new_label = list_get(h_gde_default_new_labels, i);
        if (oname == h_gde_default_new_label.oname &&
            ms == h_gde_default_new_label.ms)
            label = h_gde_default_new_label.label;
    }
    return label;
}

void
gde_default_rename_cache_new_label(string oname, gde_mapsymbol ms, string label)
{
    int i;
    gde_default_new_label_tp h_gde_default_new_label;
    for ( i = list_count(h_gde_default_new_labels) - 1; i >= 0 ; i = i - 1)
    {
        h_gde_default_new_label = list_get(h_gde_default_new_labels, i);
        if (oname == h_gde_default_new_label.oname)
        {
           list_delete(h_gde_default_new_labels,i);
        }
    }

    h_gde_default_new_label.oname = oname;
    h_gde_default_new_label.ms = ms;
    h_gde_default_new_label.label = label;

    list_append( h_gde_default_new_labels, h_gde_default_new_label);
    return ;
}

void
gde_default_global_object_rename_init(gde_mapsymbol ms)
{
    gor_current_mapsym = ms;
    ui_prop p;
    
    if (gde_mapsymbol_is_node(gor_current_mapsym))
	gor_current_label = 
	    gde_node_label(to_gde_node(gor_current_mapsym));
    else if (gde_mapsymbol_is_link(gor_current_mapsym))
	gor_current_label = 
	    gde_link_label(to_gde_link(gor_current_mapsym));
    else
	gor_current_label = 
	    gde_cntx_label(to_gde_cntx(gor_current_mapsym));

    // we are editing the new label in the dialog and do not
    // detect a global object rename
    if (gde_default_rename_fetch_new_label("ObjectRename", ms) == gor_current_label )
        return;

    p = to_ui_prop("ObjectRename", "NewLabel");
    if (ui_value(p) == gor_current_label)
	return;
    
    ui_set_value(p, gor_current_label);
    gde_default_rename_cache_new_label("ObjectRename", ms, gor_current_label);
}

    
boolean
gde_rename_action_done_standard_checks(gde_mapsymbol ms,
				       ui_prop p, string rename_func)
{
    if (ms == NULL)
    {
	ui_activate(p, False);
	return False;
    }
    
    if (gde_mapsym_rename_func(ms) != rename_func)
    {
	ui_activate(p, False);
	return False;
    }

    ui_activate(p, True);
    
    return True;
}

void
gde_object_rename_action_done()
{
    gde_mapsymbol ms = gde_selected_mapsymbol();
    ui_prop p = to_ui_prop("ObjectRename", "All");
    
    if (gde_rename_action_done_standard_checks(ms, p, "gde_default_global_object_rename") == False)
	return;
    
    gde_default_global_object_rename_init(ms);
}

int gde_object_rename_callback(ui_event_tp event, 
			  ui_prop theprop, int id, string value)
{
    int rc;

    gde_set_rename_in_progress(True);
    rc = gde_rename(event,theprop,id,value,False);
    gde_set_rename_in_progress(False);
    return rc;
}

int gde_rename_check_impact_callback(ui_event_tp event, 
			  ui_prop theprop, int id, string value)
{
    int rc;

    gde_set_rename_in_progress(True);
    rc = gde_rename(event,theprop,id,value,True);
    gde_set_rename_in_progress(False);
    return rc;
}

int gde_rename(ui_event_tp event, 
	       ui_prop theprop, int id, string value,boolean check_impact)
{
    string owner;
    string new_label;
    list new_label_lines;
    int i,count;
 
    owner = ui_owner_name(theprop);
    ui_set_value(to_ui_prop(owner,"Status"),"");
    new_label = ui_value(to_ui_prop(owner,"NewLabel"));
    
    if (new_label == gor_current_label)
    {
	ui_set_value(to_ui_prop(owner,"Status"),"Enter a Different Label.");
	return 0;
    }
     
    new_label_lines = string_to_list(new_label,"\n");
     
    if (check_impact)
    {
	editor_set_is_busy(True);
	gde_mapsym_global_rename(gor_current_mapsym, new_label,True);
	editor_set_is_busy (False);
	return 0;
    } else
    {
	editor_set_is_busy(True);
	gde_mapsym_global_rename(gor_current_mapsym, new_label,False);
	editor_set_is_busy(False);
	return 1;
    }
    
}

int gde_rename_cancel_callback(ui_event_tp event,
			       ui_prop theprop, int id, string value)
{
    gde_set_rename_in_progress(False);
    return 1;
}

string
gde_mapsymbol_symbol_type_printname(gde_mapsymbol ms)
{
    if (gde_mapsymbol_is_node(ms))
    {
	return gde_nt_printname(gde_node_nt(to_gde_node(ms)));
    } else if (gde_mapsymbol_is_link(ms))
    {
	return gde_lt_printname(gde_link_lt(to_gde_link(ms)));
    } else
    {
	return gde_ct_printname(gde_cntx_ct(to_gde_cntx(ms)));
    }
	
    return "";
}

boolean
gde_rename_label_contains_special(string label)
{
    int		ll;
    
    
    if (label != NULL)
    {
	ll = string_length(label);
	if (string_find(label, 0, ":") < ll)
	    return(True);
	if (string_find(label, 0, "<") < ll)
	    return(True);
	if (string_find(label, 0, ";") < ll)
	    return(True);
    }
    return(False);
}


void
do_rename_notification( gocha_tp gocha_type, gde_mapsymbol ms,
			string old_label, string label )
{
    if ( gocha_type == gochaRename )
    	gde_print_rename_notification_message(ms,
        	gde_mapsymbol_symbol_type_printname(ms) +
        	" renamed from '" + old_label + "' to '" + label + "'");
    else if ( gocha_type == gochaDelete )
	gde_print_rename_notification_message(ms, "'" + old_label +
					      "' deleted" );
}

void
do_global_node_op(gde_node gn, string label, gocha_tp gocha_type )
{
    if ( gocha_type == gochaRename )
	gde_node_set_label(gn, label);
    else if ( gocha_type == gochaDelete )
	gde_node_delete(gn);
}

void
do_global_link_op(gde_link gn, string label, gocha_tp gocha_type )
{
    if ( gocha_type == gochaRename )
	gde_link_set_label(gn, label);
    else if ( gocha_type == gochaDelete )
      {
	list arcs = gde_link_arcs( gn );
	list cntxs;
	int loop, loop2;
	gde_arc ga;
	for( loop = 0; loop < list_count( arcs ); loop++ )
	  {
	    ga = list_get(arcs, loop);
	    // only delete it if the arc only belongs to this link
	    if ( list_count( gde_arc_links( ga )) == 1 )
	      {
		cntxs = gde_arc_cntxs( ga );
		for( loop2 = 0; loop2 < list_count( cntxs ); loop2++ )
		  gde_cntx_delete( list_get( cntxs, loop2 ));
		gde_arc_delete( ga );
	      }
	  }
      }
}

void
do_global_cntx_op(gde_cntx gn, string label, gocha_tp gocha_type )
{
    if ( gocha_type == gochaRename )
	gde_cntx_set_label(gn, label);
    else if ( gocha_type == gochaDelete )
	gde_cntx_delete(gn);
}

void
gde_default_inverse_node_map_func(gde_mapsymbol ms, node n,
				  list rename_data_list,
				  gocha_tp gocha_type )
{
    
    int i;
    string label = NULL;
    string oldlabel = NULL;
    rename_data the_rename_data;

    if ( gocha_type == gochaRename )
    {
      for(i = 0;i < list_count(rename_data_list);i = i + 1)
	{
	  the_rename_data = list_get(rename_data_list,i);
	  if(the_rename_data.dependency_name == GDE_GOR_REN_DEP_SELF 
	     || the_rename_data.dependency_name == GDE_GOR_REN_DEP_NAME)
	    {
	      if (False ==
		  gde_rename_label_contains_special(the_rename_data.label))
		{
		  label = the_rename_data.label;
		}
	      break;
	    }
	}

      if(label == NULL)
	label = n.name;
    }

    if (gde_mapsymbol_is_node(ms))
    {
	gde_node gn = to_gde_node(ms);

	oldlabel = gde_node_label(gn);
	do_global_node_op(gn, label, gocha_type);
    } else if (gde_mapsymbol_is_link(ms))
    {
	gde_link gl = to_gde_link(ms);

	oldlabel = gde_link_label(gl);
	do_global_link_op(gl, label, gocha_type);
    } else
    {
	gde_cntx gc = to_gde_cntx(ms);

	oldlabel = gde_cntx_label(gc);
	do_global_cntx_op(gc, label, gocha_type);
    }

    do_rename_notification( gocha_type, ms, oldlabel, label );
}

//string_is_in_list ignores leading and trailing white space
boolean
string_is_in_list(string s, list l)
{
    int i;
    string stripped_string = string_strip(s,"B"," \t\n\r");

    for(i = 0;i < list_count(l); i++)
      if(stripped_string == string_strip(list_get(l,i),"B"," \t\n\r"))
	return True;
    return False;
}

//
// Find a single string in list l1, that is not also in list l2
// and return that string. If there is more than one string in l1
// that is not in l2, return NULL. If there are no strings in l1 that
// are not also in l2, return NULL. If the lengths of l1 and l2 are
// different, return NULL.
//

string
find_single_different_string(list l1, list l2)
{
    string different_string = NULL;
    string a_string;
    int i;
    
    if (list_count(l1) != list_count(l2))
	return NULL;

    for (i = 0; i < list_count(l1); i = i + 1)
    {
	a_string = list_get(l1, i);
	if (!string_is_in_list(a_string, l2))
	{
	    //
	    // a_string is not in l2.
	    //
	    
	    //
	    // If different_string is not NULL, then this is the second string
	    // from l1 that is not in l2, so return NULL.
	    //

	    if (different_string != NULL)
		return NULL;

	    //
	    // otherwise, a_string is a candidate for being the one different
	    // string
	    //

	    different_string = a_string;
	}
    }

    return different_string;
}

void
gde_link_default_inverse_link_map_func(gde_link gl, link l,
				       list rename_data_list,
				       gocha_tp gocha_type)
{
    string label = NULL,current_label, different_name, new_label = "", this_name;
    list current_names, new_names, all_link_objects;
    link one_link;
    int i;
    rename_data the_rename_data;

    for(i = 0;i < list_count(rename_data_list);i = i + 1)
    {
	the_rename_data = list_get(rename_data_list,i);
	if(the_rename_data.dependency_name == GDE_GOR_REN_DEP_SELF 
		   || the_rename_data.dependency_name == GDE_GOR_REN_DEP_NAME)
	{
	    if (False ==
		gde_rename_label_contains_special(the_rename_data.label))
	    {
		label = the_rename_data.label;
	    }
	    break;
	}
    }

    //
    // If neither the current label nor the new label contain a comma, then
    // we can just slap the new label onto the link symbol. Otherwise, we
    // need a more elaborate scheme to inject the new link name into the right
    // place in the label
    //

    current_label = gde_link_label(gl);

    if ( current_label == "" && gocha_type == gochaDelete )
      {
	do_global_link_op(gl, label, gocha_type);
	return;
      }

    if (gocha_type == gochaDelete &&
	string_find(current_label, 0, ",") == string_length(current_label))
      {
	do_global_link_op(gl, label, gocha_type);
	return;
      }
    
    if ((label != NULL) &&
	(((string_find(current_label, 0, ",") 
	   == string_length(current_label)) &&
	  (string_find(label, 0, ",") == string_length(label))) ||
	 the_rename_data.dependency_name == GDE_GOR_REN_DEP_SELF))
      {
	do_global_link_op(gl, label, gocha_type);
	return;
      }
    
    current_names = string_to_list(current_label, ",");

    new_names = list_create("string", 0);
    all_link_objects = gde_link_oms_links(gl);    
    for (i = 0; i < list_count(all_link_objects); i = i + 1)
    {
	one_link = list_get(all_link_objects, i);
	if (one_link.type == l.type)
	    list_append(new_names, one_link.name);
    }

    if ( gocha_type == gochaDelete )
      different_name = "";
    else
      different_name = find_single_different_string(new_names, current_names);
    
    if (different_name != NULL)
    {
	for (i = 0; i < list_count(current_names); i = i + 1)
	{
	    if (!string_is_in_list(list_get(current_names, i), new_names))
		this_name = different_name;
	    else
		this_name = list_get(current_names, i);
	    
	    if ( string_length( new_label ) == 0 )
		new_label = this_name;
	    else if ( string_length( this_name ) > 0 )
		new_label = new_label + ", " + this_name;
	}
    } else
    {
	for (i = 0; i < list_count(new_names); i = i + 1)
	{
	    if (i == 0)
		new_label = list_get(new_names, i);
	    else
		new_label = new_label + ", " + list_get(new_names, i);
	}
    }

    do_global_link_op(gl, new_label, gochaRename);
}

void
gde_default_inverse_link_map_func(gde_mapsymbol ms, link l,
				  list rename_data_list,
				  gocha_tp gocha_type)
{
    int i;
    string label = NULL;
    string oldlabel = NULL;
    rename_data the_rename_data;

    if ( gocha_type == gochaRename )
      {
	for(i = 0;i < list_count(rename_data_list);i = i + 1)
	  {
	    the_rename_data = list_get(rename_data_list,i);
	    if(the_rename_data.dependency_name == GDE_GOR_REN_DEP_SELF 
	       || the_rename_data.dependency_name == GDE_GOR_REN_DEP_NAME)
	      {
		if (False ==
		    gde_rename_label_contains_special(the_rename_data.label))
		  {
		    label = the_rename_data.label;
		  }
		break;
	      }
	  }

	if(label == NULL)
	  label = l.name;
      }

    if (gde_mapsymbol_is_node(ms))
    {
	gde_node gn = to_gde_node(ms);

	oldlabel = gde_node_label(gn);
	do_global_node_op(gn, label, gocha_type);
    } else if (gde_mapsymbol_is_link(ms))
    {
	gde_link gl = to_gde_link(ms);

	oldlabel = gde_link_label(gl);
	gde_link_default_inverse_link_map_func(gl, l,rename_data_list,
						gocha_type);
    } else
    {
	gde_cntx gc = to_gde_cntx(ms);

	oldlabel = gde_cntx_label(gc);
	do_global_cntx_op(gc, label, gocha_type);
    }

    do_rename_notification( gocha_type, ms, oldlabel, label );
}

void
gde_default_inverse_cntx_map_func(gde_mapsymbol ms, cntx c,
				  list rename_data_list,
				  gocha_tp gocha_type)
{
    int i;
    string label = NULL;
    string oldlabel = NULL;
    rename_data the_rename_data;

    if ( gocha_type == gochaRename )
      {
	for(i = 0;i < list_count(rename_data_list);i = i + 1)
	  {
	    the_rename_data = list_get(rename_data_list,i);
	    if(the_rename_data.dependency_name == GDE_GOR_REN_DEP_SELF 
	       || the_rename_data.dependency_name == GDE_GOR_REN_DEP_NAME)
	      {
		if (False ==
		    gde_rename_label_contains_special(the_rename_data.label))
		  {
		    label = the_rename_data.label;
		  }
		break;
	      }
	  }
	
	if(label == NULL)
	  label = c.name;
      }

    if (gde_mapsymbol_is_node(ms))
    {
	gde_node gn = to_gde_node(ms);

	oldlabel = gde_node_label(gn);
	do_global_node_op(gn, label, gocha_type);
    } else if (gde_mapsymbol_is_link(ms))
    {
	gde_link gl = to_gde_link(ms);

	oldlabel = gde_link_label(gl);
	do_global_link_op(gl, label, gocha_type);
    } else
    {
	gde_cntx gc = to_gde_cntx(ms);

	oldlabel = gde_cntx_label(gc);
	do_global_cntx_op(gc, label, gocha_type);
    }

    do_rename_notification( gocha_type, ms, oldlabel, label );
}


void
gde_default_inverse_note_map_func(gde_mapsymbol ms, note n,
				  list rename_data_list,
				  gocha_tp gocha_type)
{
    int i;
    string label = NULL;
    string oldlabel = NULL;
    rename_data the_rename_data;

    if ( gocha_type == gochaRename )
      {
	for (i = 0;i < list_count(rename_data_list);i = i + 1)
	  {
	    the_rename_data = list_get(rename_data_list,i);
	    if (the_rename_data.dependency_name == GDE_GOR_REN_DEP_SELF 
		|| the_rename_data.dependency_name == GDE_GOR_REN_DEP_NAME)
	      {
		if (False ==
		    gde_rename_label_contains_special(the_rename_data.label))
		  {
		    label = the_rename_data.label;
		  }
		break;
	      }
	  }
	
	if (label == NULL)
	  label = n.name;
      }

    if (gde_mapsymbol_is_node(ms))
    {
	gde_node gn = to_gde_node(ms);

	oldlabel = gde_node_label(gn);
	do_global_node_op(gn, label, gocha_type);
	
    } else if (gde_mapsymbol_is_link(ms))
    {
	gde_link gl = to_gde_link(ms);

	oldlabel = gde_link_label(gl);
	do_global_link_op(gl, label, gocha_type);
    } else
    {
	gde_cntx gc = to_gde_cntx(ms);

	oldlabel = gde_cntx_label(gc);
	do_global_cntx_op(gc, label, gocha_type);
    }

    do_rename_notification( gocha_type, ms, oldlabel, label );
}


void
gde_default_inverse_item_map_func(gde_mapsymbol ms, item it,
				  list rename_data_list,
				  gocha_tp gocha_type)
{
    
    int i;
    string label = NULL;
    string oldlabel = NULL;
    rename_data the_rename_data;

    if ( gocha_type == gochaRename )
      {
	for (i = 0;i < list_count(rename_data_list);i = i + 1)
	  {
	    the_rename_data = list_get(rename_data_list,i);
	    if (the_rename_data.dependency_name == GDE_GOR_REN_DEP_SELF 
		|| the_rename_data.dependency_name == GDE_GOR_REN_DEP_NAME)
	      {
		if (False ==
		    gde_rename_label_contains_special(the_rename_data.label))
		  {
		    label = the_rename_data.label;
		  }
		break;
	      }
	  }
	
	if (label == NULL)
	  label = it.value;
      }

    if (gde_mapsymbol_is_node(ms))
    {
	gde_node gn = to_gde_node(ms);

	oldlabel = gde_node_label(gn);
	do_global_node_op(gn, label, gocha_type);
	
    } else if (gde_mapsymbol_is_link(ms))
    {
	gde_link gl = to_gde_link(ms);

	oldlabel = gde_link_label(gl);
	do_global_link_op(gl, label, gocha_type);
    } else
    {
	gde_cntx gc = to_gde_cntx(ms);

	oldlabel = gde_cntx_label(gc);
	do_global_cntx_op(gc, label, gocha_type);
    }

    do_rename_notification( gocha_type, ms, oldlabel, label );
}

void
gde_no_op_inverse_node_map_func(gde_mapsymbol ms, node n,
				list rename_data_list,
				  gocha_tp gocha_type)
{
}

void
gde_no_op_inverse_link_map_func(gde_mapsymbol ms, link l,
				list rename_data_list,
				gocha_tp gocha_type)
{
}

void
gde_no_op_inverse_cntx_map_func(gde_mapsymbol ms, cntx c,
				list rename_data_list,
				gocha_tp gocha_type)
{
}

void
gde_no_op_inverse_note_map_func(gde_mapsymbol ms, note n,
				list rename_data_list,
				gocha_tp gocha_type)
{
}

void
gde_no_op_inverse_item_map_func(gde_mapsymbol ms, item i,
				list rename_data_list,
				gocha_tp gocha_type)
{
}

string
gde_mapsym_rename_func(gde_mapsymbol ms)
{
    if (gde_mapsymbol_is_node(ms))
	return (gde_nt_rename_func(gde_node_nt(to_gde_node(ms))));
    else if (gde_mapsymbol_is_link(ms))
	return (gde_lt_rename_func(gde_link_lt(to_gde_link(ms))));
    else if (gde_mapsymbol_is_cntx(ms))
	return (gde_ct_rename_func(gde_cntx_ct(to_gde_cntx(ms))));
    
    return NULL;
}
