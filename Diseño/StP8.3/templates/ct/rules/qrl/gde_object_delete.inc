//
// 	sccsid[] = %W% %Y% %D%
// 	StP/Core
// 	Confidential property of Interactive Development Environments, Inc.
// 	Copyright (c) 1992-1993
// 	All rights reserved
//


void
gde_default_global_object_delete(gde_mapsymbol ms)
{
    gde_builtin("PropertySheetShow ObjectDelete");

    gde_default_global_object_delete_init(ms);
}

void
gde_default_global_object_delete_init(gde_mapsymbol ms)
{
    gor_current_mapsym = ms;
}

    
boolean
gde_delete_action_done_standard_checks(gde_mapsymbol ms,
				       ui_prop p, string delete_func)
{
    if (ms == NULL)
    {
	ui_activate(p, False);
	return False;
    }
    
    if (gde_mapsym_delete_func(ms) != delete_func)
    {
	print_error( "No delete function set - global delete not happening." );
	ui_activate(p, False);
	return False;
    }

    ui_activate(p, True);
    
    return True;
}

void
gde_object_delete_action_done()
{
    gde_mapsymbol ms = gde_selected_mapsymbol();
    ui_prop p = to_ui_prop("ObjectDelete", "All");
    
    if (gde_delete_action_done_standard_checks(ms, p, "gde_default_global_object_delete") == False)
	return;
    
    gde_default_global_object_delete_init(ms);
}

int gde_object_delete_callback(ui_event_tp event, 
			  ui_prop theprop, int id, string value)
{
    return gde_global_delete(event,theprop,id,False);
}

int gde_delete_check_impact_callback(ui_event_tp event, 
			  ui_prop theprop, int id, string value)
{
    return gde_global_delete(event,theprop,id,True);
}

int gde_global_delete(ui_event_tp event, 
	       ui_prop theprop, int id, boolean check_impact)
{
    string owner;
    int result = 1;
 
    owner = ui_owner_name(theprop);
    ui_set_value(to_ui_prop(owner,"Status"),"");

    editor_set_is_busy(True);
    if (check_impact)
    {
	gde_mapsym_delete_activation_func(gor_current_mapsym, True);
	result = 0;
    }
    else
      gde_mapsym_delete_activation_func(gor_current_mapsym, False);

    editor_set_is_busy (False);
    return result;
}

// this is the default routine to call when a symbol is selected,
// and global object delete is called - it finds all the OMS objects
// that the symbol maps to, and calls delete on each one
//
void
gde_default_global_delete_for_mapsym(gde_mapsymbol ms, boolean check_impact )
{
    gde_mapresult gms;
    int loop;
	// get all the objects that this maps to
    list l = gde_mapsym_get_mapresults(ms);

    for( loop = 0; loop < list_count(l); loop++ )
      {
	gms = list_get(l, loop);
	gde_mapsym_global_delete( ms, gms, check_impact );
      }
   
}

void
se_global_delete_module(gde_mapsymbol ms, boolean check_impact )
{
    gde_mapresult gms;
    node file_node;
    gde_node gde_file_node;
    list oms_file_nodes;
    int loop;
	// get all the objects that this maps to
    list l = gde_mapsym_get_mapresults(ms);

    for( loop = 0; loop < list_count(l); loop++ )
      {
	gms = list_get(l, loop);
	if ( gms.mappingname == "LinkEnd" )
	  gde_mapsym_global_delete( ms, gms, check_impact );
	else if ( gms.mappingname == "SEFileMapping" )
	  {
	    if ( selection_count( "node[scope_node_id=" + gms.node.id + "]" )
		 <= 1 )
	      gde_mapsym_global_delete( ms, gms, check_impact );
	  }
	else if ( gms.mappingname == "SEDirectoryMapping" )
	  print_message( "Not deleting directory." );
      }
}


int gde_delete_cancel_callback(ui_event_tp event,
			       ui_prop theprop, int id, string value )
{
    return 1;
}


string
gde_mapsym_delete_func(gde_mapsymbol ms)
{
    if (gde_mapsymbol_is_node(ms))
	return (gde_nt_delete_func(gde_node_nt(to_gde_node(ms))));
    else if (gde_mapsymbol_is_link(ms))
	return (gde_lt_delete_func(gde_link_lt(to_gde_link(ms))));
    else if (gde_mapsymbol_is_cntx(ms))
	return (gde_ct_delete_func(gde_cntx_ct(to_gde_cntx(ms))));
    
    return NULL;
}

void
gde_mapsym_delete_activation_func(gde_mapsymbol ms, boolean check)
{
    if (gde_mapsymbol_is_node(ms))
	gde_nt_delete_activation_func(gde_node_nt(to_gde_node(ms)), check);
    else if (gde_mapsymbol_is_link(ms))
	gde_lt_delete_activation_func(gde_link_lt(to_gde_link(ms)), check);
    else if (gde_mapsymbol_is_cntx(ms))
	gde_ct_delete_activation_func(gde_cntx_ct(to_gde_cntx(ms)), check);
    
    return;
}

list
gde_mapsym_get_mapresults(gde_mapsymbol ms)
{
    if (gde_mapsymbol_is_node(ms))
      return gde_node_mapresults(to_gde_node(ms));
    else if (gde_mapsymbol_is_link(ms))
      return gde_link_mapresults(to_gde_link(ms));
    else if (gde_mapsymbol_is_cntx(ms))
      return gde_cntx_mapresults(to_gde_cntx(ms));
    return NULL;
}



string
gde_mapsym_delete(gde_mapsymbol ms)
{
    if (gde_mapsymbol_is_node(ms))
	return (gde_node_mapresults(gde_node_nt(to_gde_node(ms))));
    else if (gde_mapsymbol_is_link(ms))
	return (gde_link_mapresults(gde_link_lt(to_gde_link(ms))));
    else if (gde_mapsymbol_is_cntx(ms))
	return (gde_cntx_mapresults(gde_cntx_ct(to_gde_cntx(ms))));
    
    return NULL;
}
