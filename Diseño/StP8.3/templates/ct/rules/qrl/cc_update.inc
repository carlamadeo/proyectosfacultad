
/* reverse engineering update stuff (gde&gte) */

////////////////////////////////////////////////////////////////////////////////

#include "rules/qrl/billoo.inc"

// #include "rules/qrl/class_prop_common.inc"

//ECR 197 begin
const string    CC_tokenizer = ";";  // see also update_from_cc
//ECR 197 end

////////////////////////////////////////////////////////////////////////////////


gde_symbol
CC_editor_find_class_by_node_id(int id)
{
    if (PropSheet__editor_type() == "gte")
    {
    	gde_symbol      symbol;
	symbol.name = OMTClassTableGetClassCellLabel();
	if (symbol.name == NULL || symbol.name == "")
	    return NULL;
	return symbol;
    } else if (PropSheet__editor_type() == "gde")
	return find_class_by_psymid(id);
    return NULL;
}



////////////////////////////////////////////////////////////////////////////////

void
CC_run_update(string operation)
{
    int             sniffd, i, psymid;
    string          classname, classnames = "", cmd;
    gde_node        class;
    list            classsymbols = PropSheet___selected_class_symbols();

    if (classsymbols == NULL || list_count(classsymbols) > 1)
    {
	print_error("Please select just one class to pass to Reverse Engineering.");
	return;
    }
    for (i = 0; i < list_count(classsymbols); i = i + 1)
	classnames = classnames + strip_whitespace(gde_node_label(list_get(classsymbols, i)));
    if (classnames == "")
    {
	print_error("Please select one named class to pass to Reverse Engineering.");
	return;
    }

    class = list_get(classsymbols,0);

    cmd = "update_from_cc -forward_messages " + editor_get_msgd_handle() +
	" -p " + current_projdir() +
      " -s " + current_system() + " -handle " + editor_get_msgd_handle() +
      " -class \"" + classnames + "\" -psym " + gde_node_psymid( class ) +
      " -editor " + editor_get_msgd_name() + operation;

    if (is_windows_platform())
        cmd = "cmd /c start /b " + cmd;
    else
        cmd += " & ";

    system( cmd );

    print_message( "Sent message to RE and awaiting reply..." );

    return;
}


////////////////////////////////////////////////////////////////////////////////


void
CC_query_class_members()
{
    CC_run_update( " -members " );
}



////////////////////////////////////////////////////////////////////////////////

void
CC_update_member_cb( string classname, int psymid,
		     string attributes, string operations )
{
    gde_symbol      class;

    if ( attributes == "" && operations == "" )
    {
    	print_message("RE could not find attributes & operations of class " + classname + ".");
	return;
    }

    print_message("RE found attributes/operations of class " + classname + ".");
    class = CC_editor_find_class_by_node_id(psymid);
    if (class == NULL)
    {
	print_message("Could not find class symbol in editor with id " + psymid + ".");
	return;
    }
    list            attrs = string_to_list(attributes, CC_tokenizer);
    list            ops = string_to_list(operations, CC_tokenizer);

    if (PropSheet__editor_type() == "gte")
    {
	OMTClassTableInitializeFromStrings(classname, attrs, ops);
    } else if (PropSheet__editor_type() == "gde")
    {
      if ( is_product_omt() )
	{
	  if (decorate_class_loaded == False)
	    decorate_class_loaded = qrl_addcode(decorate_class_qrl_file);
	  gde_save_state();
	  DecorateOMTClassSymbolFromStrings(to_gde_node(class), attrs, ops);
	}
      else
	DecorateBoochClassSymbolFromStrings1(to_gde_node(class), attrs, ops);
    }

    return;
}


////////////////////////////////////////////////////////////////////////////////

graph           CC_classes_graph;
boolean         CC_inheritance_subclasses;

void
CC_query_class_subclasses()
{
    CC_classes_graph = graph_create();
    CC_inheritance_subclasses = True;
    CC_run_update( " -show_system -inheritance -below " );
}


void
CC_query_class_superclasses()
{
    CC_classes_graph = graph_create();
    CC_inheritance_subclasses = False;
    CC_run_update( " -show_system -inheritance -above " );
}


void
CC_query_class_allclasses()
{
    CC_classes_graph = graph_create();
    CC_inheritance_subclasses = False;
    CC_run_update( " -show_system -inheritance -all " );
}


//
void
CC_update_inheritance_cb(string classname, int psymid, string subclasses)
{
    string subclass;
    gde_symbol      class;
    string things = "subclasses";
    int i;

    if ( CC_inheritance_subclasses == False )
      things = "superclasses";

    if ( classname != "" )
      {
	list subs = string_to_list(subclasses, CC_tokenizer);
	graph_add_node( CC_classes_graph, classname );
	for( i = 0; i < list_count( subs ); ++i )
	  {
	    subclass = list_get(subs, i);
	    if ( subclass == "" )
	      continue;
	    graph_add_node( CC_classes_graph, subclass );
	    graph_add_arc( CC_classes_graph, classname, subclass );
	  }
      }
    else
      {
	classname = subclasses;
	if ( graph_count( CC_classes_graph ) < 2 )
	  {
	    print_message("RE could not find any " + things + " for class " +
			  classname + ".");
	    return;
	  }
	print_message("RE found " + things + " of class " + classname + ".");

	string product;
	if ( is_product_uml())
	  product = "UML";
	else if ( is_product_omt())
	  product = "OMT";
	else
	  product = "Booch";

	list namelist = list_create( "string", 1 );

	class = CC_editor_find_class_by_node_id(psymid);
	if (class == NULL)
	  {
	    print_message("Could not find class symbol in editor with id " +
			  psymid + ".");
	    return;
	  }

	if ( CC_inheritance_subclasses == False )
	  {
	    // so these are supclasses
	    list tops = string_to_list(subclasses, CC_tokenizer);
	    string this_name;
	    for( i = 0; i < list_count( tops ); ++i )
	      {
		this_name = list_get( tops, i );
		if ( graph_is_node( CC_classes_graph, this_name ))
		  list_append( namelist, this_name );
	      }
	  }
	else
	  list_append( namelist, classname );

    //ECR 197 - commented out next line
	//gde_node_delete( to_gde_node( class ));
	DrawClassHierarchyFromGraph( namelist, CC_classes_graph, product );
      }
    return;
}

