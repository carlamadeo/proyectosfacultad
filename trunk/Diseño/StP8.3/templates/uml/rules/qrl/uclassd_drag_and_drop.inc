
void
createObjectWithAttributes (
                string dropEditor, string dropEditorClass, int dropStpmHandle,
                string projdir, string system,
                string omsName, string omsType, string omsClass, int oid,
                int xpos, int ypos,
                string info)
{
   list symbols = gde_selected_symbols();

   //   message( "uclassd: entering createObjectWithAttributes" );

   boolean status = False ;

   if (list_count(symbols) == 1)
   {
       gde_symbol symbol = list_get(symbols, 0);

       if (gde_symbol_is_node(symbol))
       {
	   gde_node gnode = to_gde_node(symbol);
	   gde_nt   gnt   = gde_node_nt(gnode);

	   string gnt_name = gde_nt_name( gnt );

	   if( gnt_name == "Class" ) 
	   {
	       string qrlCommand;
// Removed for ECR 7175
//	         if (dropEditor == "uclassd")
//	            qrlCommand = 
//                   "obj_with_attrs_create_and_persist(" +
//                    "\"" + gde_node_label(gnode) + "\"," +
//                    "\"" + get_attr_names(gnode) + "\"," +
//                           xpos                  + "," +
//                           ypos                  + 
//                    ");" 	;
//	         else 
	          qrlCommand = 
                 "obj_with_attrs_create(" +
                  "\"" + gde_node_label(gnode) + "\"," +
                  "\"" + get_attr_names(gnode) + "\"," +
                         xpos                  + "," +
                         ypos                  + 
                  ");" ;

	       // this command gets sent to IM and uses the obj_with_attrs_create
	       // functions from IM !!!
               SendToRemoteQrl (qrlCommand, dropEditorClass, dropStpmHandle) ;
	       status = True ;
	   }
       }
   }

   if (!status)
   {
      print_message ("Can only drag a single object to this editor.") ;
   }
}


void
obj_with_attrs_create_and_persist(string objName, string attrNames, int x, int y)
{
    gde_node attrNode;
    gde_node objNode;
    int dodefer;

    //    message( "uclassd: entering obj_with_attrs_create_and_persist" );

    gde_save_state();
    dodefer = gde_start_edit_transaction();

    objNode = gde_node_create("Class",x,y);
    gde_node_set_label(objNode,objName);

    list attrList = string_to_list(attrNames, ",");
    
    int i, count = list_count(attrList);
    
    for (i = 0; i < count; i = i + 1)
    {
	attrNode = gde_node_create("Attribute", x,y);
	gde_node_set_label(attrNode,list_get(attrList, i));
    }

    ////////////////////////////////
    // force mapping 
    // get mapped results 
    // set its annotation
    //
    //
    gde_defer_action_completed(0);
    gde_action_completed();
    gde_defer_action_completed(1);
    list            maps = gde_symbol_mapresults(to_gde_symbol(objNode));
    gde_mapresult   map;
    if (maps != NULL)
	for (i = 0; i < list_count(maps); i = i + 1)
	{
	    map = list_get(maps, 0);
	    if (map.mappingname == "ClassMapping")
	    {
		OAECommand c;
		list items, annotations;
		items = list_create("string", 0);
    		annotations = list_create("OAECommand", 0);
		annlist_append_value(items, "UmlStereotype", "Persistent", NULL);
		OAECommand__ctorNoteUpdate(c, map.node.id,  gde_node_psymid(objNode), 
					   OAE_CMD_NOTE_SEL + "UmlExtensibilityDefinition", items);
		list_append(annotations, c);
		OAECommand__send(annotations, NULL);
		break;
	    }
	}
    //
    //
    ////////////////////////////////

    gde_end_edit_transaction(GDE_CMD_INSERT, dodefer, False);
}

void
obj_with_attrs_create(string objName, string attrNames, int x, int y)
{
    gde_node attrNode;
    gde_node objNode;
    int dodefer;

    //    message( "uclassd: entering obj_with_attrs_create" );

    gde_save_state();
    dodefer = gde_start_edit_transaction();

    objNode = gde_node_create("Class",x,y);
    gde_node_set_label(objNode,objName);

    list attrList = string_to_list(attrNames, ",");
    
    int i, count = list_count(attrList);
    
    for (i = 0; i < count; i = i + 1)
    {
	attrNode = gde_node_create("Attribute", x,y);
	gde_node_set_label(attrNode,list_get(attrList, i));
    }

    gde_end_edit_transaction(GDE_CMD_INSERT, dodefer, False);
}

string
get_attr_names(gde_node class_sym)
{
    list outarcs;
    string comp_nodes = "";
    gde_arc outarc;
    gde_node comp_node;
    int i;

    // iterate through the list of outgoing arcs
    outarcs = gde_node_outarcs(class_sym);

    // First find all of the component nodes and store them in a list
    for (i = 0; i < list_count(outarcs); i = i + 1) 
    {
	outarc = list_get(outarcs, i);

	// if the arc is a class component arc, delete the tonode
	// trial&error said this is "Contains" ..., a.m.
	if (gde_lt_name(gde_arc_lt(outarc)) == "Contains")
	{
	    comp_node = gde_arc_tonode(outarc);
	    if (gde_nt_name(gde_node_nt(comp_node)) == "Attribute")
	    {
		if (comp_nodes != "") comp_nodes = comp_nodes + ",";
		comp_nodes = comp_nodes + gde_node_label(comp_node);
	    }
	}
    }
    return comp_nodes;
}


