////////////////////////////////////////////////////////////////////////////////
//
// HTML_Report
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Externals 
////////////////////////////////////////////////////////////////////////////////

external boolean e_gen_Actor_Pages = True;
external_help = "Generate actor index and actor pages ?";

external string e_limit_to_Actors_named = "*";
external_help = "Names of actors to report about";


external boolean e_gen_Use_Case_Pages = True;
external_help = "Generate use case index and use case pages ?";

external string e_limit_to_Use_Cases_named = "*";
external_help = "Names of use cases to report about";


external boolean e_gen_Package_Pages = True;
external_help = "Generate package index and package pages ?";

external string e_limit_to_Packages_named = "*";
external_help = "Names of packages to report about";


external boolean e_gen_Class_Pages = True;
external_help = "Generate class index and class pages ?";

external string e_limit_to_Classes_named = "*";
external_help = "Names of classes to report about";


external boolean e_gen_Stereotype_Pages = True;
external_help = "Generate stereotype index and stereotype pages ?";

external string e_limit_to_Stereotypes_named = "*";
external_help = "Names of stereotypes to report about";


external boolean e_gen_Diagram_Pages = True;
external_help = "Generate diagram index and diagram pages ?";

external string e_limit_to_Diagrams_named = "*";
external_help = "Names of diagrams to report about";

//ECR 7153 start
external string e_suppress_display_marks_in_Diagrams =
"UseCaseScenariosExist ClassTableExists RefinedStateExists StateTableExists";
external_help = "List of Display Marks which should be suppressed in Diagrams";
 //ECR 7153 end

external string e_Programming_Language = "Java";
external_help = "Your destination programming language (i.e. Java, C++, IDL, ...)";


external boolean e_Link_in_Paralist = True;
external_help = "Scan parameters for known classes ?";

///////////////////////////////////////////////////////////////////////////////
// INCLUDE-FILES:
///////////////////////////////////////////////////////////////////////////////

#include "uml/rules/qrl/stereotype.ps.inc"
#include "uml/rules/qrl/oper_parse.inc"

#include "uml/qrl/include/HTML_Util.inc"
#include "uml/qrl/include/HTML_Idlist.inc"

#include "uml/qrl/include/HTML_Actor.inc"
#include "uml/qrl/include/HTML_UseCase.inc"
#include "uml/qrl/include/HTML_Package.inc"
#include "uml/qrl/include/HTML_Class.inc"
#include "uml/qrl/include/HTML_Stereotype.inc"
#include "uml/qrl/include/HTML_Diagram.inc"

///////////////////////////////////////////////////////////////////////////////
// DEFINITIONS:
///////////////////////////////////////////////////////////////////////////////

string HTML_BASEPATH;

///////////////////////////////////////////////////////////////////////////////
//  SOURCE:
///////////////////////////////////////////////////////////////////////////////

void rep_start ()
{
string	cmd;

	if ((toolinfo_variable( "postscript_conversion" )==NULL) && (e_gen_Diagram_Pages == True))
	{
		print_error("Postscript conversion tool not defined in Toolinfo variable");
		print_error("No Diagrams will be created");
		e_gen_Diagram_Pages = False;
	}

    HTML_BASEPATH = current_projdir() + current_system();

    message("Generating HTML pages ...");

    // make sure the html subdirectories exists
    if( ! rep_dir_make_all( HTML_BASEPATH ) )
    {
        message( "\nCannot write the generated HTML files to\n" + HTML_BASEPATH + "/html_files !\n" );
        return;
    }

    HTML_BASEPATH = path_compose(HTML_BASEPATH, "html_files");
    
    // create the IDLists of all elements, which are described in the report
    idlist_create();

    // always generate the start page
	rep_gen_startpage();
   
    //  generate actor
    if (e_gen_Actor_Pages)
        actor_start();

    //  generate use case pages
    if (e_gen_Use_Case_Pages)
        usecase_start();

    //  generate package pages
    if (e_gen_Package_Pages)
        package_start();

    //  generate class pages
    if (e_gen_Class_Pages)
        class_start();

    //  generate stereotype pages
    if (e_gen_Stereotype_Pages)
        stereotype_start();

    //  generate diagram pages
    if (e_gen_Diagram_Pages)
        diagram_start();

    idlist_free();
    
// ECR 7676
    if (view_after_run())
    {
        cmd = path_compose(HTML_BASEPATH, "index.html");
        if (is_windows_platform())
            system("explorer " + cmd); // FIXME: may be we should create new QRL function open_URL()
        else
            system("netscape " + cmd);
    }
    return;
}

boolean rep_dir_make_all( string system_path )
{
    // Check if system path exists
    if( ! read_dir_access( system_path ) )
    {
        message( "Cannot access " + system_path );
        return(False);
    }

    // Create html_files directory first
    string html_base = system_path + "/html_files";
    if( ! rep_dir_create( html_base ) ) 
        return(False);

    if( ! rep_dir_create( html_base + "/actors" ) ) 
        return(False);
    if( ! rep_dir_create( html_base + "/usecases" ) ) 
        return(False);
    if( ! rep_dir_create( html_base + "/packages" ) ) 
        return(False);
    if( ! rep_dir_create( html_base + "/classes" ) ) 
        return(False);
    if( ! rep_dir_create( html_base + "/stereotypes" ) ) 
        return(False);
    if( ! rep_dir_create( html_base + "/diagrams" ) ) 
        return(False);

    return(True);
}

boolean rep_dir_create( string dir )
{
boolean		ret = True;
boolean		can_write;

    if( ! read_dir_access( dir ) )
    {
        // try to create only if it doesn't exist yet
        message( "Creating " + dir );
        if( mkdir( dir ) == -1 )
        {
            message( "Cannot create " + dir );
            ret = False;
        }
    }
    else
    {
        // else check if we can write 
        if( ! rep_dir_access( dir ) )
        {
            message( "Cannot write in existing dir " + dir );
            ret = False;
        }
    }
    return(ret);
}

boolean rep_dir_access( string dir )
{
    // write_file() doesn't return a value :-(((
	write_file( dir + "/check", "check" );
	boolean can_write = write_file_access( dir + "/check" );
	if( ! can_write )
		return(False);
	else
	{
		delete_file( dir + "/check" );
		return(True);
	}
}

void rep_gen_startpage()
{
string	html_file = "Index.html";
string	html_content = "";
string	sys_name = current_system();
     
    // Print standard HTML header
    html_content += html_page_start("Start Page " + sys_name);

    // Print start page heading
    html_content += "<BR><BR>\n";
    html_content += html_style ("H1", "StP System " + sys_name);
    html_content += "<BR>\n";
    html_content += html_seperator("");

    // Links to index pages
    html_content += "<UL>\n<H3>\n";

    if (e_gen_Actor_Pages)
        html_content += "<LI>" + html_link(sys_name + "_Actor_Index.html", "Actor Index", "" ) + "</LI><BR>\n<BR>\n";

    if (e_gen_Use_Case_Pages)
        html_content += "<LI>" + html_link(sys_name + "_UseCase_Index.html", "Use Case Index", "") + "</LI><BR>\n<BR>\n";

    if (e_gen_Package_Pages)
        html_content += "<LI>" + html_link(sys_name +"_Package_Index.html", " Package Index", "") + "</LI><BR>\n<BR>\n";

    if (e_gen_Class_Pages)
        html_content += "<LI>" + html_link(sys_name + "_Class_Index.html", "Class Index", "") + "</LI><BR>\n<BR>\n";

    if (e_gen_Stereotype_Pages)
        html_content += "<LI>" + html_link(sys_name + "_Stereotype_Index.html", "Stereotype Index", "") + "</LI><BR>\n<BR>\n";

    if (e_gen_Diagram_Pages)
        html_content += "<LI>" + html_link(sys_name + "_Diagram_Index.html", "Diagram Index", "") + "</LI><BR>\n<BR>\n";

    html_content += ("</H3>\n</UL>\n");

    html_content += html_page_end();
    // write the HTML file
    write_file(HTML_BASEPATH + "/" + html_file, html_content);

    message( "- Start Page created/updated in:  " + HTML_BASEPATH );
    
    return;
}

void rep_gen_indexpage(string idx_type, string idx_path, list idx_idlist)
{
int		icnt, ix;
node	htmlObject;
string	html_file = current_system() + "_" + idx_type + "_Index.html";
string	html_content;

    // number of objects now
    icnt = list_count(idx_idlist);

    // Print standard HTML header
    html_content = html_page_start("StP " + idx_type + " Index: " + current_system());

    // Print reference section at top of the page
    html_content += html_ref_heading("");

    // Print index page heading
    html_content += html_style("H1", idx_type + " Index of System " + current_system());

    html_content += html_seperator("");

    if   (icnt < 1)
    {
		html_content += "<BR>\n";
        html_content += html_style(	"B,I", "No " + idx_type + "s found in System " 
									+ current_system() + " !"	);
		html_content += "<BR>\n<BR>\n";
    }
    else  
		html_content += rep_gen_node_index(idx_type, idx_idlist, idx_path);

	html_content += html_page_end();
    // write the HTML file
    write_file(HTML_BASEPATH + "/" + html_file, html_content);

	return;
}

string rep_gen_node_index(string idx_type, list idx_idlist, string idx_path)
{
int				icnt, ix, jx;
int				obj_id,obj_ido;
node			obj_node;
node			obj_find;
file			obj_file;
item			obj_item;
string			content;
stereot_def		obj_ste;
boolean			obj_heading=False;
string			obj_type;
boolean			obj_found;

	icnt = list_count(idx_idlist);
	content = "<UL>\n";

    if (idx_type == "Class")
    {
		obj_heading=False;
		obj_node = find_by_query("node[id=" + list_get(idx_idlist,0) + "]");
		obj_find = find_by_query("node[node_refs & name='${obj_node.name}' & (type=UmlParameterizedClass|type=UmlInstantiatedClass)]");
		if (obj_find==NULL)
			obj_type = "UmlClass";
		else
			obj_type = obj_find.type;
		
		for (ix=0;ix<icnt;ix++)
		{
			obj_node=find_by_query("node[id=" + list_get(idx_idlist,ix) + "]");
			if (	(	(obj_type != "UmlClass") && 
						(selection_count("node[node_refs & type=${obj_type} & name='${obj_node.name}']")==0) ) ||
					(	(obj_type == "UmlClass") && 
						(selection_count(	"node[node_refs & (type=UmlParameterizedClass|type=UmlInstantiatedClass) & " +
											"name='${obj_node.name}']")>0) )	)
			{
				obj_heading = False;
				obj_find = find_by_query(	"node[node_refs & name='${obj_node.name}' & " +
											"(type=UmlParameterizedClass|type=UmlInstantiatedClass)]" );
				obj_type = obj_find.type;
			}
			if (!obj_heading)
			{
				obj_heading=True;
				if (obj_type=="UmlClass")				content += html_style ("H2", "Classes") + "\n";
				if (obj_type=="UmlParameterizedClass")	content += html_style ("H2", "Parameterized Classes") + "\n";
				if (obj_type=="UmlInstantiatedClass")	content += html_style ("H2", "Instantiated Classes") + "\n";
			}
			content += "<LI>" + html_link(	idx_path + obj_node.name + "_" + obj_node.id + ".html",
												idlist_get_nodename(obj_node.id), "B" ) + "</LI>\n";
		}
    }
    else
    if (idx_type == "Stereotype")
    {
        obj_heading = True;
        obj_ido = list_get(idx_idlist, 0);
		for (ix = 0; ix < icnt; ix++) 
		{
			obj_id = list_get(idx_idlist, ix);
			if (ix>0)
                obj_heading = (((obj_id>0) && (obj_ido<0))||
                               ((obj_id<0) && (obj_ido>0)));
			if (obj_id<0)
			{
				if (obj_heading)
				{
					content += html_style ("H2", "Predefined Stereotypes") + "\n";
					obj_heading=False;
				}
				obj_ste = list_get(builtin_stereotypes,(-1-obj_id));
				// write one list element for each predefined stereotype
				content += "<LI>" + html_link(	idx_path + obj_ste.name + "_pre.html", 
												obj_ste.name, "B" ) + "</LI>\n";
			}
			else
			{
				if (obj_heading)
				{
					content += html_style ("H2", "User defined Stereotypes") + "\n";
					obj_heading=False;
				}
				obj_node = find_by_query("node[id=${obj_id}]");
				// write one list element for each node
				content += "<LI>" + html_link(	idx_path + obj_node.name + "_" + obj_node.id + ".html", 
												obj_node.name, "B" ) + "</LI>\n";
			}
            obj_ido = obj_id;
		}
    }
    else
    if (idx_type == "Diagram")
    {
		for (jx = 0; jx < 9; jx++)
		{
			if (jx==0)	obj_type = "UmlUseCaseDiagram";
			if (jx==1)	obj_type = "UmlClassDiagram";
			if (jx==2)	obj_type = "UmlSequenceDiagram";
			if (jx==3)	obj_type = "UmlCollaborationDiagram";
			if (jx==4)	obj_type = "UmlStateDiagram";
			if (jx==5)	obj_type = "UmlActivityDiagram";
			if (jx==6)	obj_type = "UmlComponentDiagram";
			if (jx==7)	obj_type = "UmlDeploymentDiagram";
			if (jx==8)	obj_type = "UmlStereotypeDiagram";
			
			ix=0;
			obj_found=False;
			do
			{
				obj_id = list_get(idx_idlist, ix);
				obj_file = find_by_query("file[id=${obj_id}]");
				if (obj_file.type==obj_type) obj_found=True;
				ix++;
			}
			while ((!obj_found) && (ix<icnt));
			ix--;
			
			if (obj_found)
			{
				if (obj_type == "UmlUseCaseDiagram")		content += html_style ("H2", "Use Case Diagrams") + "\n";
				if (obj_type == "UmlClassDiagram")			content += html_style ("H2", "Class Diagrams") + "\n";
				if (obj_type == "UmlSequenceDiagram")		content += html_style ("H2", "Sequence Diagrams") + "\n";
				if (obj_type == "UmlCollaborationDiagram")	content += html_style ("H2", "Collaboration Diagrams") + "\n";
				if (obj_type == "UmlStateDiagram")			content += html_style ("H2", "State Diagrams") + "\n";
				if (obj_type == "UmlActivityDiagram")		content += html_style ("H2", "Activity Diagrams") + "\n";
				if (obj_type == "UmlComponentDiagram")		content += html_style ("H2", "Component Diagrams") + "\n";
				if (obj_type == "UmlDeploymentDiagram")		content += html_style ("H2", "Deployment Diagrams") + "\n";
				if (obj_type == "UmlStereotypeDiagram")		content += html_style ("H2", "Stereotype Diagrams") + "\n";

				while((obj_file.type==obj_type) && (ix<icnt))
				{
					// write one list element for each node
					content += "<LI>" + html_link(	idx_path + diagram_type_convert(obj_type) + "_" + 
													obj_file.name + "_" + obj_file.id + ".html", 
													obj_file.name, "B" ) + "</LI>\n";
					ix++;
					if (ix<icnt)
					{
						obj_id = list_get(idx_idlist, ix);
						obj_file = find_by_query("file[id=${obj_id}]");
					}
				}
			}
		}
    }
    else
    {
		for (ix = 0; ix < icnt; ix++) 
		{
			obj_id = list_get(idx_idlist, ix);
			obj_node = find_by_query("node[id=${obj_id}]");
			// write one list element for each node
			content += "<LI>" + html_link(	idx_path + obj_node.name + "_" + obj_node.id + ".html", 
											obj_node.name, "B") + "</LI>\n";
		}
	}
	content += "</UL>\n";
	
	return(content);
}
