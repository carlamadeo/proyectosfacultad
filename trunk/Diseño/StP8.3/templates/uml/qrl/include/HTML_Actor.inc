////////////////////////////////////////////////////////////////////////////////
//
// HTML_Actor
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//  SOURCE:
///////////////////////////////////////////////////////////////////////////////

void actor_start()
{
int		ccnt, cx;
node	Actor;

    ccnt = list_count(idlist_actor);

    message("- Generating Actor pages ...");

    // generate the index page first
    rep_gen_indexpage("Actor", "actors/", idlist_actor);

    // no actors in primary selection -> finished
    if (ccnt < 1)
    {
         message("\n\nNo Actors selected / specified !");
         return;
    }

    for (cx = 0; cx < ccnt; cx++) 
    {
        Actor = find_by_query("node[UmlActor & id=" + list_get(idlist_actor, cx) + "]");

        message( "- Generating Actor " + Actor.name + " (" + to_string(cx+1) + " of " + to_string(ccnt) + ")" );

        // write one file for each actor
		actor_gen_page(Actor);
    }
    return;
}

void actor_gen_page(node Actor)
{
string		html_file = "actors/" + Actor.name + "_" + Actor.id + ".html";
string		html_content;

    // Print standard HTML header
    html_content = html_page_start("StP Actor Description: " + Actor.name);

    // Print reference section at top of the page
    html_content += html_ref_prevnext(Actor.id, idlist_actor);

    // Print actor heading
    html_content += html_style("H1", "Actor: " + Actor.name) + "\n";

	html_content += html_gen_info(Actor);
	
    html_content += html_print_obj_description("UmlActorDefinition", Actor.id);

list	diags=list_create("string",0);

	list_append(diags,"UmlUseCaseDiagram");
	list_append(diags,"UmlSequenceDiagram");
	list_append(diags,"UmlCollaborationDiagram");
	html_content += html_ref_diag( Actor, diags );
	list_clear(diags);

    html_content += html_seperator("");
    html_content += "<BR>\n";
	
	html_content += actor_gen_tree(Actor);

    html_content += html_page_end();
    // write the HTML file
    write_file(HTML_BASEPATH + "/" + html_file, html_content);

    return;
}

string actor_gen_tree(node Actor)
{
string	content="";
string	query;
list	ids=NULL;
string idList; // ECR 7496

	// Actor Inheritances
    query = "node[UmlActor && node_refs && in_links[UmlGeneralization && link_refs && from_node_id = " 
              + Actor.id + "]] sort by name";
    content += html_ref_line(query, "Inherits: ", "", 2);

    query = "node[UmlActor && node_refs && in_links[UmlGeneralization && link_refs && from_node_id = ${id}]]";
	idlist_get_hierarchie(query, Actor.id, False, ids);
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

    query = "node[UmlActor && node_refs && out_links[UmlGeneralization && link_refs && to_node_id = " 
            + Actor.id + "]] sort by name"; 
    content += html_ref_line(query, "Inherited by: ", "", 2);

    query = "node[UmlActor && node_refs && out_links[UmlGeneralization && link_refs && to_node_id = ${id}]]";
	idlist_get_hierarchie(query, Actor.id, False, ids);
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

    // use cases invoked by actor
    // in Uml, several interaction links with different names may exist
    // between actor and use case, we use a set here to avoid duplicates
    query = "node[UmlUseCase && node_refs && in_links[UmlUseCaseInteraction && from_node_id = "
                + Actor.id + " && link_refs]] sort by name";
    content += html_ref_line(query, "Interacts with Use Cases: ", "../usecases/", 2);

	return(content);
}
