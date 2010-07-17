////////////////////////////////////////////////////////////////////////////////
//
// HTML_Stereotype
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//  SOURCE:
///////////////////////////////////////////////////////////////////////////////

void stereotype_start()
{
int				ccnt, cx;
node			Stereotype;
stereot_def		StereotypePreDef;

    ccnt = list_count(idlist_stereotype);

    message("- Generating Stereotype pages ...");

    // generate the index page first
    rep_gen_indexpage("Stereotype", "stereotypes/", idlist_stereotype);

    // no stereotypes in primary selection -> finished
    if (ccnt < 1)
    {
         message("\n\nNo Stereotypes selected / specified !");
         return;
    }

    for (cx = 0; cx < ccnt; cx++) 
    {
		if (list_get(idlist_stereotype,cx)<0)
		{
			StereotypePreDef = list_get(builtin_stereotypes, (-1-list_get(idlist_stereotype,cx)));

			message( "- Generating Stereotype " + StereotypePreDef.name + " (" + to_string(cx+1) + " of " + to_string(ccnt) + ")" );

			// write one file for each stereotype
			stereotype_gen_page_predef(StereotypePreDef, list_get(idlist_stereotype,cx));
		}
		else
		{
			Stereotype = find_by_query("node[UmlStereotype & id=" + list_get(idlist_stereotype, cx) + "]");

			message( "- Generating Stereotype " + Stereotype.name + " (" + to_string(cx+1) + " of " + to_string(ccnt) + ")" );

			// write one file for each stereotype
			stereotype_gen_page(Stereotype);
		}
    }
    return;
}

void stereotype_gen_page(node Stereotype)
{
string		html_file = "stereotypes/" + Stereotype.name + "_" + Stereotype.id + ".html";
string		html_content;

    // Print standard HTML header
    html_content = html_page_start("StP Stereotype Description: " + Stereotype.name);

    // Print reference section at top of the page
    html_content += html_ref_prevnext(Stereotype.id, idlist_stereotype);

    // Print stereotype heading
    html_content += html_style("H1", "User defined Stereotype: " + Stereotype.name) + "\n";

    html_content += html_print_obj_description("GenericObject", Stereotype.id);

list	diags=list_create("string",0);

	list_append(diags,"UmlStereotypeDiagram");
	html_content += html_ref_diag( Stereotype, diags );
	list_clear(diags);

	html_content += html_seperator("");
	html_content += "<BR>\n";

	html_content += stereotype_gen_links(Stereotype.name);

	html_content += html_seperator("");
	html_content += "<BR>\n";
	
	html_content += stereotype_gen_tree(Stereotype);

    html_content += html_page_end();
    // write the HTML file
    write_file(HTML_BASEPATH + "/" + html_file, html_content);

    return;
}

void stereotype_gen_page_predef(stereot_def StereotypePreDef, int StereotypeId)
{
string		html_file = "stereotypes/" + StereotypePreDef.name + "_pre.html";
string		html_content;

    // Print standard HTML header
    html_content = html_page_start("StP Stereotype Description: " + StereotypePreDef.name);

    // Print reference section at top of the page
    html_content += html_ref_prevnext(StereotypeId, idlist_stereotype);

    // Print stereotype heading
    html_content += html_style("H1", "Predefined Stereotype: " + StereotypePreDef.name) + "\n";

	if (StereotypePreDef.desc != "")
	{
		html_content += html_print_description("Description:", StereotypePreDef.desc, 1);
		if (StereotypePreDef.page != "")
			html_content += html_indent(html_style("B", StereotypePreDef.page), 1);
		html_content += "<BR>\n";
	}

	html_content += html_seperator("");
	html_content += "<BR>\n";

	html_content += stereotype_gen_links(StereotypePreDef.name);

    html_content += html_page_end();
    // write the HTML file
    write_file(HTML_BASEPATH + "/" + html_file, html_content);

    return;
}

string stereotype_gen_links ( string stereo_name )
{
string		content = "";
list		node_list;
node		node_var;
node		class_var;
int			i;

	node_list=list_select(	"node[(" +
							"type=UmlAttribute|" +
							"type=UmlOperation|" +
							"type=UmlClass|" +
							"type=UmlUseCase|" +
							"type=UmlPackage|" +
							"type=UmlActor" +
							") & node_refs & items[type=UmlStereotype & value='${stereo_name}']] sort by type,name" );
	if (list_count(node_list)>0)
	{
		content += html_style( "B", "The Stereotype is used by:" ) + "<BR>\n";
		for (i=0;i<list_count(node_list);i++)
		{
			node_var=list_get(node_list,i);
			if ( node_var.type=="UmlClass" )
			{
				if ( idlist_link( node_var.type, node_var.id ) )
					content += 	"<LI>" + html_escape_chars(node_var.type) + " " +
								html_link(	"../classes/" + node_var.name + "_" + node_var.id + ".html",
											idlist_get_nodename(node_var.id), "") + "</LI>\n";
				else
					content += 	"<LI>" + html_escape_chars(node_var.type) + " " + 
								html_style("B", idlist_get_nodename(node_var.id)) + "</LI>\n";
			}
			if ( node_var.type=="UmlAttribute" )
			{
				class_var=find_by_query("node[UmlClass & node_refs & out_links[link_refs & to_node[id=${node_var.id}]]]");

				if ( idlist_link( class_var.type, class_var.id ) )
					content += 	"<LI>" + html_escape_chars(node_var.type) + " " +
								html_link(	"../classes/" + class_var.name + "_" + class_var.id + 
											".html#" + node_var.name + "_" + node_var.id,
											idlist_get_nodename(node_var.id), "") + "</LI>\n";
				else
					content += 	"<LI>" + html_escape_chars(node_var.type) + " " + 
								html_style("B", idlist_get_nodename(node_var.id)) + "</LI>\n";
			}
			if ( node_var.type=="UmlOperation" )
			{
				class_var=find_by_query("node[UmlClass & node_refs & out_links[link_refs & to_node[id=${node_var.id}]]]");

				if ( idlist_link( class_var.type, class_var.id ) )
					content += 	"<LI>" + html_escape_chars(node_var.type) + " " +
								html_link(	"../classes/" + class_var.name + "_" + class_var.id +
											".html#" + node_var.name + "_" + node_var.id,
											idlist_get_nodename(node_var.id), "") + "</LI>\n";
				else
					content += 	"<LI>" + html_escape_chars(node_var.type) + " " + 
								html_style("B",	idlist_get_nodename(node_var.id)) + "</LI>\n";
			}
			if ( node_var.type=="UmlUseCase" )
				if ( idlist_link( node_var.type, node_var.id ) )
					content += 	"<LI>" + html_escape_chars(node_var.type) + " " +
								html_link(	"../usecases/" + node_var.name + "_" + node_var.id + ".html",
											node_var.name, "") + "</LI>\n";
				else
					content += 	"<LI>" + html_escape_chars(node_var.type) + " " + html_style("B", node_var.name) + "</LI>\n";
			if ( node_var.type=="UmlPackage" )
				if ( idlist_link( node_var.type, node_var.id ) )
					content += 	"<LI>" + html_escape_chars(node_var.type) + " " +
								html_link(	"../packages/" + node_var.name + "_" + node_var.id + ".html",
											node_var.name, "") + "</LI>\n";
				else
					content += 	"<LI>" + html_escape_chars(node_var.type) + " " + html_style("B", node_var.name) + "</LI>\n";
			if ( node_var.type=="UmlActor" )
				if ( idlist_link( node_var.type, node_var.id ) )
					content += 	"<LI>" + html_escape_chars(node_var.type) + " " +
								html_link(	"../actors/" + node_var.name + "_" + node_var.id + ".html",
											node_var.name, "") + "</LI>\n";
				else
					content += 	"<LI>" + html_escape_chars(node_var.type) + " " + html_style("B", node_var.name) + "</LI>\n";
		}
		content += "<BR>\n<BR>\n";
	}
	return(content);
}

string stereotype_gen_tree(node Stereotype)
{
string	content="";
string	query;
list	ids=NULL;
string idList; // ECR 7496

	// Stereotype Inheritances
    query = "node[UmlStereotype && node_refs && in_links[UmlStereotypeInheritance && link_refs && from_node_id = " 
             + Stereotype.id + "]] sort by name";
    content += html_ref_line(query, "Inherits: ", "", 2);

    query = "node[UmlStereotype && node_refs && in_links[UmlStereotypeInheritance && link_refs && from_node_id = ${id}]]";
	idlist_get_hierarchie(query, Stereotype.id, False, ids);
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

    query = "node[UmlStereotype && node_refs && out_links[UmlStereotypeInheritance && link_refs && to_node_id = " 
            + Stereotype.id + "]] sort by name"; 
    content += html_ref_line(query, "Inherited by: ", "", 2);

    query = "node[UmlStereotype && node_refs && out_links[UmlStereotypeInheritance && link_refs && to_node_id = ${id}]]";
	idlist_get_hierarchie(query, Stereotype.id, False, ids);
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
	
	return(content);
}
