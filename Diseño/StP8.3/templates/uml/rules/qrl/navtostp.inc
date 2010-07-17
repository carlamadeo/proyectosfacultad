// New file for ECR 4179

// This file includes functions to navigate to StP
//

//
// Navigate to StP
//
int Nav2StP(string editor, string nodetype, string nodename)
{
    print_message("Nav2StP " + editor + " " +  nodetype + " " +  nodename);

    // Begin modify ECR 4731
    node search_node;
 
    if( nodename == "" &&
        nodetype != "UmlInitialState" && nodetype != "UmlFinalState" ) {
        print_error("Specify a " + nodetype + " to browse to ");
        return -1;
    }

    // ToDo:
    // Map nodetype from IDE to omstype.
    //
    //  string omstype = map_type(nodetype);

    string filetype = diag_type_from_editor_type( editor );

    if( nodetype == "UmlObjectInstance" )
        search_node = oi_node_from_label( nodename );
    else 
        search_node = find_by_query( "node[${nodetype} && node_refs[file[${filetype}]] && name == '${nodename}']" );

    if (search_node == NULL) {
       	print_error(nodetype + "  " + nodename + " not found.");
        return -1;
    }
    NavToNodeInFileType( editor, filetype, search_node );
    return 0;
    // End modify ECR 4731
}

// Begin Add ECR 5037
int Nav2StPByID(string editor, string nodetype, string id )
{
    print_message("Nav2StPByID " + editor + " " +  id );
    string filetype = diag_type_from_editor_type( editor );
    node search_node = find_by_query( "node[id==${id} && node_refs[file[${filetype}]]]" );
    if (search_node == NULL) {
       	print_error(nodetype + "  " + id + " not found.");
        return -1;
    }
    NavToNodeInFileType( editor, filetype, search_node );
    return 0;
}
// End Add ECR 5037

// Begin Add ECR 4731
node
oi_node_from_label( string nodename )
{
    node search_node = NULL;
    string object = "";
    string class = "";
    list candidates;
    int i;
    node oi, cs;
    int cpos = string_find( nodename, 0, ":" );

    if( cpos > 0 )
    {
        object = string_extract( nodename, 0, cpos );
        if( cpos < string_length( nodename ) )
            class = string_extract( nodename, cpos+1, string_length(nodename)-(cpos+1) );
    }
    else
        class = string_strip( nodename, "L", ":" );

    if( object == "" && class != "" )
        search_node = find_by_query( "node[UmlObjectClassScope && node_refs[file[${filetype}]] && name == '${class}']" );
    else if( class == "" && object != "" )
    {
        candidates = list_select( "node[UmlObjectInstance && node_refs && name == '${object}']" );
        for( i=0; i<list_count(candidates); i++ )
        {
            oi = list_get( candidates, i );
            cs = find_by_query( "node[UmlObjectClassScope && node_refs && id==${oi.scope_node_id}]" );
            // can't use $ "%%class%%" in OMS query
            if( cs != NULL && !string_find( cs.name, 0, "%%class%%") )
                search_node = oi;
        }
    }
    else if( class != "" && object != "" )
    {
        candidates = list_select( "node[UmlObjectInstance && node_refs && name == '${object}']" );
        for( i=0; i<list_count(candidates); i++ )
        {
            oi = list_get( candidates, i );
            cs = find_by_query( "node[UmlObjectClassScope && node_refs && name == '${class}' && id==${oi.scope_node_id}]" );
            if( cs != NULL )
                search_node = oi;
        }
    }
    else
    {
        print_error( "Illegal objectname " + nodename );
        return NULL;
    }
    
    return search_node;
}

string
diag_type_from_editor_type( string editor )
{
    //
    // Nav to StP/UML
    //
    if( editor == "uclassd" )
         return  "UmlClassDiagram";
    else if (editor == "ustated" )
         return  "UmlStateDiagram";
    else if( editor == "uclasst")
         return  "UmlClassTable";
    else if( editor == "usequenced")
         return  "UmlSequenceDiagram";
    else if( editor == "ucollaborationd")
         return  "UmlCollaborationDiagram";
    else if (editor == "uactivityd" )
         return  "UmlActivityDiagram";
    else if (editor == "uusecased" )
         return  "UmlUseCaseDiagram";
    else if (editor == "ucomponentd" )
         return  "UmlComponentDiagram";
    else if (editor == "udeploymentd" )
         return  "UmlDeploymentDiagram";
    else if (editor == "ustereotyped" )
         return  "UmlStereotypeDiagram";
    else if (editor == "rqtbl" )
         return  "REQTable";
    //
    // Nav to StP/OMT
    //
    else if( editor == "cte")
         return  "OMTClassTable"; 
    else if( editor == "ome")
         return  "OMTObjectDiagram";
    else
         return "";
}
// End Add ECR 4731

//
// Navigate to a Node in a specified filetype. 
//
void NavToNodeInFileType(string editor, string filetype, node search_node)
{
    file diag;
    node_ref nr;
    string query = "file[" + filetype + "  && node_refs[node_id == "  + search_node.id + "]]";

    list diag_list = list_select(query);

    if( list_count( diag_list ) > 1 ) {
        editor_item_selector( diag_list, "LoadSelectedFile(  " + search_node.id  + " );",
                              "Navigate to " + search_node.type +
                              " '" + search_node.name + "' in" , True );
    }
    else if( list_count(diag_list) == 1 ) {
	diag = list_get(diag_list, 0);
	nr = find_by_query("node_ref[file_id == ${diag.id} && node_id == ${search_node.id}]");
        NavLoadAndFind(diag.name, nr.appid);
    }
}

//
// Load the selected File.
// Get the selected Label from the selector list.
// Label contains diagram type and name of the node like
// UmlClassDiagram 'ClassName'
//
void LoadSelectedFile( int nodeid )
{ 
    string query;
    node_ref nr;
    string diag_name;
    string diag_type;
    list sel = editor_selector_selected_labels();

    if( list_count( sel ) != 1 ) {
	return;         // Don't know where to go.
    }

    diag_name = list_get( sel, 0 );
    
    message("full diag_name: " + diag_name);

    int sl = string_length( diag_name );
    int pos1 = string_find( diag_name, 0, " " );

    if( pos1 < sl ) {
	diag_type = string_extract( diag_name, 0, pos1);
	diag_name = string_extract( diag_name, pos1+1, sl-(pos1+1));
	diag_name = string_strip( diag_name, "B", "'" );
    }
    else
        return;        // something went wrong.
 
    message("diag_type: " + diag_type + " diag_name: " + diag_name);
    
    nr = find_by_query("node_ref[node_id == ${nodeid} && file[${diag_type} && name == '${diag_name}']]");
    if (nr != NULL)
	NavLoadAndFind(diag_name, nr.appid);
    else
        message("No noderef in selected diagram ? ");

    return;

} 
