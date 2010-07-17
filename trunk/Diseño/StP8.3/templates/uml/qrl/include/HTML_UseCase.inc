////////////////////////////////////////////////////////////////////////////////
//
// HTML_UseCase
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//  SOURCE:
///////////////////////////////////////////////////////////////////////////////

void usecase_start()
{
int		ccnt, cx;
node	UseCase;

    message("- Generating Use Case pages ..."); 

    ccnt = list_count(idlist_usecase);

    // generate the index page first
    rep_gen_indexpage("UseCase", "usecases/", idlist_usecase);

    // no use cases in primary selection -> finished
    if (ccnt < 1)
    {
        message("\n\nNo Use Cases selected / specified !");
        return;
    }

    for (cx = 0; cx < ccnt; cx++) 
    {
        UseCase = find_by_query("node[UmlUseCase & id=" + list_get(idlist_usecase, cx) + "]");

        message( "- Generating UseCase " + UseCase.name + " (" + to_string(cx+1) + " of " + to_string(ccnt) + ")" );

        // write one file for each use case
        usecase_gen_page(UseCase);
    }
    return;
}

void usecase_gen_page(node UseCase)
{
string			html_file = "usecases/" + UseCase.name + "_" + UseCase.id + ".html";
string			html_content;

    // Print standard HTML header
    html_content = html_page_start("StP Use Case Description: " + UseCase.name);

    // Print reference section at top of the page
    html_content += html_ref_prevnext(UseCase.id, idlist_usecase);

    // Print use case heading
    html_content += html_style("H1", "Use Case: " + UseCase.name) + "\n";

	html_content += html_gen_info(UseCase);

    html_content += html_print_obj_description("UmlUseCaseDefinition", UseCase.id);

list	diags=list_create("string",0);

	list_append(diags,"UmlUseCaseDiagram");
	html_content += html_ref_diag( UseCase, diags );
	list_clear(diags);
	
	html_content += html_ref_statemac( UseCase );
	
    html_content += usecase_gen_scenario(UseCase);

    html_content += html_seperator("");
    html_content += "<BR>\n";
	
	html_content += usecase_gen_tree(UseCase);

    html_content += html_seperator("");
    html_content += "<BR>\n";

    // Print use case preconditions
    html_content += usecase_gen_infotext(UseCase.id, "UmlUseCasePrecondition", "Preconditions:");

    // Print use case postconditions
    html_content += usecase_gen_infotext(UseCase.id, "UmlUseCasePostcondition", "Postconditions:");

    // Print use case exceptions 
    html_content += usecase_gen_infotext(UseCase.id, "UmlUseCaseException", "Exceptions:");

    html_content += html_page_end();
    // write the HTML file
    write_file(HTML_BASEPATH + "/" + html_file, html_content);

    return;
}

string usecase_gen_scenario(node UseCase)
{
string	content;
file	diag_file;
list	diag_filelist;
int		scnt, sx;
    
    diag_filelist = list_select("file[node_refs[node[UmlScenarioInstance && " + 
                "(items[UmlParentTypeItem && value='UmlUseCase'] && " + 
                "items[UmlParentNameItem && value='${UseCase.name}'])]]]");

    scnt = list_count( diag_filelist );

    if (scnt == 0)
    {
		list_clear(diag_filelist);
        return"";                 // Nothing to do
	}
    
    content = html_style( "B", "Use Case Scenarios: \n" );

    for( sx = 0; sx < scnt; sx++ )
    {
        diag_file = list_get( diag_filelist, sx );
		if (idlist_link(diag_file.type,diag_file.id))
			content += "<LI>" + html_escape_chars(diag_file.type) + " " +
						html_link(	"../diagrams/" + diagram_type_convert(diag_file.type) + "_" + 
									diag_file.name + "_" + diag_file.id + ".html",
									diag_file.name, "" ) + "</LI>";
		else
			content +=	"<LI>" + html_escape_chars(diag_file.type) + 
						" " + html_style("B", diag_file.name) + "</LI>";
    }
    content += "<BR>\n<BR>\n";
    
    return(content);
}

string usecase_gen_infotext(int useCaseId, string infoApptype, string infoTitle)
{
item	infoItem;
string	content = "";

    infoItem = find_by_query("item[" + infoApptype + " && obj_id=" + useCaseId + "]");

    if ((infoItem != NULL) && (infoItem.value != ""))
    {
        content += html_print_description(infoTitle, infoItem.value, 1);
        content += "<BR>\n";
	}

    return(content);
}

string usecase_gen_tree(node UseCase)
{
string	content="";
string	query;
list	ids=NULL;
string idList;

    // Package UseCase belongs to
    query = "node[UmlPackage && node_refs && out_links[UmlContains && "
            + "link_refs && to_node_id = " + UseCase.id + "]] sort by name"; 
    content += html_ref_line(query, "Packages: ", "../packages/", 2);

	// UseCase Inheritances
    query = "node[UmlUseCase && node_refs && in_links[UmlGeneralization && link_refs && from_node_id = " 
             + UseCase.id + "]] sort by name";
    content += html_ref_line(query, "Inherits: ", "", 2);

    query = "node[UmlUseCase && node_refs && in_links[UmlGeneralization && link_refs && from_node_id = ${id}]]";
	idlist_get_hierarchie(query, UseCase.id, False, ids);
	if(list_count(ids) != 0)
	{
                // ECR 7496 -->
		idList = list_to_idlist(ids, "node");
		// query = "node[id = " + list_to_string(ids, " || id = ") + "] sort by name";
		query = "node[" + idList + "] sort by name";
		content += html_ref_line(query, "Inherits indirectly: ", "", 2);
		id_list_free(idList);
                // ECR 7496 <--
	}
	list_clear(ids);

    query = "node[UmlUseCase && node_refs && out_links[UmlGeneralization && link_refs && to_node_id = " 
            + UseCase.id + "]] sort by name"; 
    content += html_ref_line(query, "Inherited by: ", "", 2);

    query = "node[UmlUseCase && node_refs && out_links[UmlGeneralization && link_refs && to_node_id = ${id}]]";
	idlist_get_hierarchie(query, UseCase.id, False, ids);
	if(list_count(ids) != 0)
	{
                // ECR 7496 -->
		idList = list_to_idlist(ids, "node");
		// query = "node[id = " + list_to_string(ids, " || id = ") + "] sort by name";
		query = "node[" + idList + "] sort by name";
		content += html_ref_line(query, "Inherited indirectly by: ", "", 2);
		id_list_free(idList);
                // ECR 7496 <--
	}
	list_clear(ids);

    // actors interacting with our use case
    // in Uml, several interaction links with different names may exist
    // between actor and use case, we use a set here to avoid duplicates
    query = "node[UmlActor && node_refs && out_links[UmlUseCaseInteraction && to_node_id = " 
            + UseCase.id + " && link_refs]] sort by name";
    content += html_ref_line(query, "Actors: ", "../actors/", 2);

    // use cases <<extending>> our use case
    query     = "node[UmlUseCase && node_refs && in_links[UmlUseCaseExtends && from_node_id = " 
               + UseCase.id + " && link_refs]] sort by name";
    content += html_ref_line(query, "Extends: ", "", 2);

    // use cases <<extended>> by our use case 
    query  = "node[UmlUseCase && node_refs && out_links[UmlUseCaseExtends && to_node_id = " 
            + UseCase.id + " && link_refs]] sort by name";
    content += html_ref_line(query, "Extended by: ", "", 2);

    // use cases <<using>> our use case
    query  = "node[UmlUseCase&& node_refs && in_links[UmlUseCaseUses && from_node_id = " 
            + UseCase.id + " && link_refs]] sort by name";
    content += html_ref_line(query, "Includes: ", "", 2);

    // use cases <<used>> by use case
     query  = "node[UmlUseCase && node_refs && out_links[UmlUseCaseUses && to_node_id = " 
            + UseCase.id + " && link_refs]] sort by name";
    content += html_ref_line(query, "Included by: ", "", 2);

	return(content);
}
