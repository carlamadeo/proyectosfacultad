////////////////////////////////////////////////////////////////////////////////
//
// HTML_Package
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
//  SOURCE:
///////////////////////////////////////////////////////////////////////////////

void package_start()
{
int		ccnt, cx;
node	Package;

    ccnt = list_count(idlist_package);

    message("- Generating Package pages ..."); 

    // generate the index page first
    rep_gen_indexpage("Package", "packages/", idlist_package);

    // packages in primary selection -> finished
    if (ccnt < 1)
    {
        message("\n\nNo Packages selected / specified !");
        return;
    }

    for (cx = 0; cx < ccnt; cx++) 
    {
        Package = find_by_query("node[UmlPackage & id=" + list_get(idlist_package, cx) + "]");

        message( "- Generating Package " + Package.name + " (" + to_string(cx+1) + " of " + to_string(ccnt) + ")" );

        // write one file for each use case
        package_gen_page(Package);
    }
    return;
}

void package_gen_page(node Package)
{
string	html_file = "packages/" + Package.name + "_" + Package.id + ".html";
string	html_content;

    // Print standard HTML header
    html_content = html_page_start("StP Package Description: " + Package.name);

    // Print reference section at top of the page
    html_content += html_ref_prevnext(Package.id, idlist_package);

    // Print subsystem / package heading
    html_content += html_style ("H1", "Package: " + Package.name) + "\n";

	html_content += html_gen_info(Package);

    html_content += html_print_obj_description("GenericObject", Package.id);

list	diags=list_create("string",0);

	list_append(diags,"UmlUseCaseDiagram");
	list_append(diags,"UmlClassDiagram");
	list_append(diags,"UmlSequenceDiagram");
	list_append(diags,"UmlCollaborationDiagram");
	html_content += html_ref_diag( Package, diags );
	list_clear(diags);

    html_content += html_seperator("");
    html_content += "<BR>\n";

    // Print package in tree

    html_content += package_gen_tree(Package);

    html_content += html_seperator("");
    html_content += "<BR>\n";

    // fill list of part classes

    html_content += package_gen_parts(Package);

    html_content += html_page_end();
    // write the HTML file
    write_file(HTML_BASEPATH + "/" + html_file, html_content);

    return;
}

string package_gen_tree(node Package)
{
string	query;
string	content;

    // is part of package
    query = "node[UmlPackage && node_refs && out_links[UmlContains && "
           + "link_refs && to_node_id = " + Package.id + "]] sort by name"; 

    content = html_ref_line(query, "Part of Package: ", "", 2);

    // contains of package
    query = "node[UmlPackage && node_refs && in_links[UmlContains && "
       + "link_refs && from_node_id = " + Package.id + "]] sort by name"; 

    content += html_ref_line(query, "Contains Package: ", "", 2);

    return(content);
}

string package_gen_parts(node Package)
{
string		content="";
int			ccnt, cx;
list		part_list;
node		part_node;
int			heading_nr=0;

    part_list = list_select(	"node[node_refs && in_links[UmlContains && link_refs && from_node_id=" +
								Package.id + "]] sort by type,name" );

    ccnt = list_count( part_list );

    if (ccnt == 0)
    {
	    list_clear( part_list );
		return( "" );
	}

	for (cx = 0; cx < ccnt; cx++)
	{
		part_node = list_get( part_list, cx );

		if (part_node.type=="UmlClass")
		{
			if (heading_nr < 1)
			{
				if (heading_nr > 0)
					content += "</UL>\n";
				heading_nr=1;
				content += html_style( "H2", "Class Index" ) + "\n";
				content += "<UL>\n";
			}
			if (idlist_link(part_node.type,part_node.id))
				content += "<LI>" + html_link(	"../classes/" + part_node.name + "_" + part_node.id + ".html", 
												idlist_get_nodename(part_node.id), "" ) + "</LI>\n";
			else
				content += "<LI>" + html_style( "B", idlist_get_nodename(part_node.id) ) + "</LI>\n";
		}
		else
		if (part_node.type=="UmlUseCase")
		{
			if (heading_nr < 2)
			{
				if (heading_nr > 0)
					content += "</UL>\n";
				heading_nr=2;
				content += html_style( "H2", "Use Case Index" ) + "\n";
				content += "<UL>\n";
			}
			if (idlist_link(part_node.type,part_node.id))
				content += "<LI>" + html_link(	"../usecases/" + part_node.name + "_" + part_node.id + ".html", 
												part_node.name, "" ) + "</LI>\n";
			else
				content += "<LI>" + html_style( "B", part_node.name ) + "</LI>\n";
		}
	}
	content += "</UL>\n";

    list_clear( part_list );

    return( content );
}
