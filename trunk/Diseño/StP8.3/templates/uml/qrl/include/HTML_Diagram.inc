////////////////////////////////////////////////////////////////////////////////
//
// HTML_Diagram
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// DEFINITIONS:
///////////////////////////////////////////////////////////////////////////////

struct diag_nodelist_extremes_type
{
int MinX;
int MaxX;
int MinY;
int MaxY;
};

struct diag_nodelist_info_type
{
int Id;
string Type;
string Name;
float ScaleX;
float ScaleY;
int MinX;
int MinY;
int MaxX;
int MaxY;
};

struct diag_nodelist_factor_type
{
float x;
float y;
};

///////////////////////////////////////////////////////////////////////////////
//  SOURCE:
///////////////////////////////////////////////////////////////////////////////

void diagram_start()
{
int				ccnt, cx;
file			Diagram;
int				DiagramId;

    ccnt = list_count(idlist_diagram);

    message("- Generating Diagram pages ..."); 

    // generate the index page first
    rep_gen_indexpage("Diagram", "diagrams/", idlist_diagram);

    // no diagrams in primary selection -> finished
    if (ccnt < 1)
    {
         message("\n\nNo Diagrams selected / specified !");
         return;
    }

// ECR 7153 start
string		settings = "";
set			display_marks_suppress;
int			i;

string		tv_postscript = NULL;
int			tv_pos;
int			tv_len;
float		tv_scale_factor;
string		tv_scale_factor_str="";
 
    display_marks_suppress = string_to_set( e_suppress_display_marks_in_Diagrams, " " );
 
	settings += "DiagramPrintSetting editor {\n";
	settings += "    { Version                            7.000                   }\n";
	settings += "    { OutputTarget                       JPEGFile                }\n";
 
	settings += "    { PageHeight                         504.000                 }\n";
	settings += "    { PageWidth                          324.000                 }\n";
	settings += "    { PageLeftMargin                     30.000                  }\n";
	settings += "    { PageRightMargin                    30.000                  }\n";
	settings += "    { PageTopMargin                      30.000                  }\n";
	settings += "    { PageBottomMargin                   30.000                  }\n";
	settings += "    { PageOrientation                    Automatic               }\n";
 
	settings += "    { CaptionShow                        True                    }\n";
	settings += "    { CaptionText                        \"${file} on ${date}\"    }\n";
	settings += "    { CaptionVerticalPlacement           Bottom                  }\n";
	settings += "    { CaptionAlignment                   Right                   }\n";
	settings += "    { CaptionParagraphFormat             \"StpDefault\"            }\n";

	tv_postscript = toolinfo_variable("postscript_conversion");
	if ( tv_postscript == NULL )
	{
		tv_scale_factor_str = "1.0";
	}
	else
	{
		tv_pos = string_find(tv_postscript,1,"-r");
		if ( tv_pos == string_length(tv_postscript) )
		{
			tv_scale_factor_str = "1.0";
		}
		else
		{
			tv_len = string_find(tv_postscript,tv_pos,"x");
			tv_scale_factor = 72.0 / to_float(string_extract(tv_postscript,tv_pos+2,tv_len-tv_pos-2));
			tv_scale_factor_str = to_string(tv_scale_factor);
		}
	}

    settings += "    { DiagramScale                       " + tv_scale_factor_str + " }\n";
    settings += "    { DiagramCaptionOrientation          AsDiagram               }\n";
    settings += "    { DiagramScaleMethod                 Fit                     }\n";
    settings += "    { DiagramPagesAcross                 1                       }\n";
    settings += "    { DiagramPagesDown                   1                       }\n";
    settings += "    { DiagramHorizontalAlignment         Center                  }\n";
    settings += "    { DiagramVerticalAlignment           Center                  }\n";
    settings += "    { DiagramFontScaleTuneFactor         1.000                   }\n";
    settings += "    { DiagramLabelFont\n";
    settings += "        { DiagramFontFamily              \"Helvetica\" }\n";
    settings += "        { DiagramFontWeight              \"Regular\" }\n";
    settings += "        { DiagramFontAngle               \"Regular\" }\n";
    settings += "        { DiagramFontSize                13.000 }\n";
    settings += "    }\n";
    settings += "    { DiagramDisplayMarkFont\n";
    settings += "        { DiagramFontFamily              \"Helvetica\" }\n";
    settings += "        { DiagramFontWeight              \"Regular\" }\n";
    settings += "        { DiagramFontAngle               \"Regular\" }\n";
    settings += "        { DiagramFontSize                13.000 }\n";
    settings += "    }\n";
    settings += "    { DiagramCaptionFont\n";
    settings += "        { DiagramFontFamily              \"Helvetica\" }\n";
    settings += "        { DiagramFontWeight              \"Regular\" }\n";
    settings += "        { DiagramFontAngle               \"Regular\" }\n";
    settings += "        { DiagramFontSize                13.000 }\n";
    settings += "    }\n";
    settings += "    { DiagramLabelCharacterFormat        \"StpDefault\"          }\n";
    settings += "    { DiagramDisplayMarkCharacterFormat  \"StpDefault\"          }\n";
 
	if ( set_count( display_marks_suppress ) > 0 )
	{
		settings += "    { DiagramNoUpdateDisplayMarks\n";
		for ( i=0; i<set_count( display_marks_suppress ); i++ )
		{
			settings += "        { Name \"" + set_get_element(display_marks_suppress,i) + "\" }\n";
		}
		settings += "    }\n";
	}
	settings += "}\n";
 
	write_file( path_compose(HTML_BASEPATH, "rules.tmp"), settings );
// ECR 7153 end

    for (cx = 0; cx < ccnt; cx++) 
    {
		DiagramId = list_get(idlist_diagram, cx);

		Diagram = find_by_query("file[id=" + DiagramId + "]");

		message( "- Generating " + Diagram.type + " " + Diagram.name + " (" + to_string(cx+1) + " of " + to_string(ccnt) + ")" );

		// write one file for each diagram
		diagram_gen_page( Diagram );
    }

// ECR 7153 start
	delete_file( path_compose(HTML_BASEPATH, "rules.tmp") );
// ECR 7153 end
 
    return;
}

// ECR 7153 start

set string_to_set(string the_string, string seperator)
{
set		the_set = set_create( "string" );
list	the_list = string_to_list( the_string, seperator );
int		i;
	
	if (list_count( the_list ) == 0)
	{
		list_clear( the_list );
		return( the_set );
	}
	
	for ( i=0; i < list_count( the_list ); i++ )
	{
		set_add( the_set, list_get( the_list, i ) );
	}
 
	list_clear( the_list );
	return( the_set );
}

// ECR 7153 end 
 
void diagram_gen_page( file Diagram )
{
string		header;
string		editor_type;
string		sys_call;

	editor_type = diagram_type_convert(Diagram.type);

// ECR 7153 start

	sys_call =	"dprint -s " + current_system() +
				" -p " + current_projdir() +
				" -ed " + editor_type + " " + 
				forward_messages_arg() + " " + Diagram.name +
				" -o " + path_compose(HTML_BASEPATH, "diagrams", editor_type + "_" + Diagram.name + ".jpg") +
				" -rules_print " + path_compose(HTML_BASEPATH, "rules.tmp");

	system( sys_call );

 
// ECR 7153 end
 
	diagram_gen_links( Diagram, Diagram.id, editor_type );
	
	return;
}

void diagram_gen_links( file diag_file, int diag_id, string diag_header )
{
diag_nodelist_extremes_type		diag_Pan;
list							diag_filelist;
diag_nodelist_info_type			diag_node;
int								i,cnt;
string							html_content = "";
string							html_file = "/diagrams/" + diag_header + "_" + diag_file.name + "_" + diag_id + ".html";
list							diag_operations,
								diag_attributes,
								diag_classes,
								diag_stereotypes,
								diag_packages,
								diag_usecases,
								diag_actors,
								diag_statemacs,
								diag_sceninsts;
node							diag_operation = NULL;
node							diag_class = NULL;
node							diag_usecase = NULL;
    
    diag_Pan=diagram_find_extremes(diag_id);

// ECR 7153 comments - no more attribute and operation clickables
//	diag_operations=list_create("diag_nodelist_info_type",0);
//	diag_attributes=list_create("diag_nodelist_info_type",0);

	diag_classes=list_create("diag_nodelist_info_type",0);
	diag_stereotypes=list_create("diag_nodelist_info_type",0);
	diag_usecases=list_create("diag_nodelist_info_type",0);
	diag_actors=list_create("diag_nodelist_info_type",0);
	diag_packages=list_create("diag_nodelist_info_type",0);
	diag_statemacs=list_create("diag_nodelist_info_type",0);
	diag_sceninsts=list_create("diag_nodelist_info_type",0);

    html_content = html_page_start("StP Diagram " + diag_file.name );
    html_content += html_ref_heading("../");
    html_content += html_style ( "H1", "StP Diagram " + diag_file.name ) + "\n";

    html_content += "<MAP NAME=\"map\">\n";
    diag_filelist=diag_nodelist_init(diag_id);
    cnt=diag_nodelist_count( diag_filelist );
    for ( i=0; i<cnt; i++ )
    {
		diag_node=diag_nodelist_get(diag_filelist,i,diag_id);

// ECR 7153 comments - no more attribute and operation clickables
//		if (diag_node.Type=="Operation")
//			list_append(diag_operations,diag_node);		else
//		if (diag_node.Type=="Attribute")
//			list_append(diag_attributes,diag_node); 	else

		if (diag_node.Type=="Class")
			list_append(diag_classes,diag_node);		else
		if ((diag_node.Type=="ParameterizedClass")||(diag_node.Type=="InstantiatedClass"))
		{
			diag_class = find_by_query("node[node_refs & type=UmlClass & name='${diag_node.Name}']");
			diag_node.Id = diag_class.id;
			diag_node.Type = "Class";
			list_append(diag_classes,diag_node);
		}
		else
		if (diag_node.Type=="Stereotype")
			list_append(diag_stereotypes,diag_node);	else
		if (diag_node.Type=="UseCase")
			list_append(diag_usecases,diag_node);		else
		if (diag_node.Type=="Actor")
			list_append(diag_actors,diag_node);			else
		if (diag_node.Type=="Package")
			list_append(diag_packages,diag_node);		else
		if (diag_node.Type=="StateMachine")
			list_append(diag_statemacs,diag_node);		else
		if (diag_node.Type=="ScenarioInstance")
			list_append(diag_sceninsts,diag_node);
    }

// ECR 7153 comments - no more attribute and operation clickables
//	for ( i=0; i<list_count(diag_operations); i++ )
//	{
//		diag_node=list_get(diag_operations,i);
//		diag_operation=find_by_query("node[${diag_node.Id}]");
//		diag_class=find_by_query("node[${diag_operation.scope_node_id}]");
//		if ( idlist_link("UmlClass",diag_class.id) )
//			html_content +=	html_link_area(
//								"../classes/" + diag_class.name + "_" + diag_class.id +
//								".html#" + diag_node.Name + "_" + diag_node.Id,
//								"Operation '" + idlist_get_nodename(diag_operation.id) + "'",
//								diag_node.MinX-diag_Pan.MinX+40,
//								diag_node.MinY-diag_Pan.MinY+40,
//								diag_node.MaxX-diag_Pan.MinX+40,
//								diag_node.MaxY-diag_Pan.MinY+40		);
//	}
//
//	for ( i=0; i<list_count(diag_attributes); i++ )
//	{
//		diag_node=list_get(diag_attributes,i);
//		diag_operation=find_by_query("node[${diag_node.Id}]");
//		diag_class=find_by_query("node[${diag_operation.scope_node_id}]");
//		if ( idlist_link("UmlClass",diag_class.id) )
//			html_content +=	html_link_area(
//								"../classes/" + diag_class.name + "_" + diag_class.id + 
//								".html#" + diag_node.Name + "_" + diag_node.Id,
//								"Attribute '" + idlist_get_nodename(diag_operation.id) + "'",
//								diag_node.MinX-diag_Pan.MinX+40,
//								diag_node.MinY-diag_Pan.MinY+40,
//								diag_node.MaxX-diag_Pan.MinX+40,
//								diag_node.MaxY-diag_Pan.MinY+40		);
//	}

	for ( i=0; i<list_count(diag_classes); i++ )
	{
		diag_node=list_get(diag_classes,i);
		if ( idlist_link("Uml" + diag_node.Type,diag_node.Id) )
			html_content +=	html_link_area(
								"../classes/" + diag_node.Name + "_" + diag_node.Id + ".html",
								"Class '" + idlist_get_nodename(diag_node.Id) + "'",
								diag_node.MinX-diag_Pan.MinX+40,
								diag_node.MinY-diag_Pan.MinY+40,
								diag_node.MaxX-diag_Pan.MinX+40,
								diag_node.MaxY-diag_Pan.MinY+40		);
	}

	for ( i=0; i<list_count(diag_stereotypes); i++ )
	{
		diag_node=list_get(diag_stereotypes,i);
		if ( idlist_link("Uml" + diag_node.Type,diag_node.Id) )
			html_content +=	html_link_area(
								"../stereotypes/" + diag_node.Name + "_" + diag_node.Id + ".html",
								"Stereotype '" + diag_node.Name + "'",
								diag_node.MinX-diag_Pan.MinX+40,
								diag_node.MinY-diag_Pan.MinY+40,
								diag_node.MaxX-diag_Pan.MinX+40,
								diag_node.MaxY-diag_Pan.MinY+40		);
	}

	for ( i=0; i<list_count(diag_usecases); i++ )
	{
		diag_node=list_get(diag_usecases,i);
		if ( idlist_link("Uml" + diag_node.Type,diag_node.Id) )
			html_content +=	html_link_area(
								"../usecases/" + diag_node.Name + "_" + diag_node.Id + ".html",
								"Use Case '" + diag_node.Name + "'",
								diag_node.MinX-diag_Pan.MinX+40,
								diag_node.MinY-diag_Pan.MinY+40,
								diag_node.MaxX-diag_Pan.MinX+40,
								diag_node.MaxY-diag_Pan.MinY+40		);
	}

	for ( i=0; i<list_count(diag_actors); i++ )
	{
		diag_node=list_get(diag_actors,i);
		if ( idlist_link("Uml" + diag_node.Type,diag_node.Id) )
			html_content +=	html_link_area(
								"../actors/" + diag_node.Name + "_" + diag_node.Id + ".html",
								"Actor '" + diag_node.Name + "'",
								diag_node.MinX-diag_Pan.MinX+40,
								diag_node.MinY-diag_Pan.MinY+40,
								diag_node.MaxX-diag_Pan.MinX+40,
								diag_node.MaxY-diag_Pan.MinY+40		);
	}

	for ( i=0; i<list_count(diag_packages); i++ )
	{
		diag_node=list_get(diag_packages,i);
		if ( idlist_link("Uml" + diag_node.Type,diag_node.Id) )
			html_content +=	html_link_area(
								"../packages/" + diag_node.Name + "_" + diag_node.Id + ".html",
								"Package '" + diag_node.Name + "'",
								diag_node.MinX-diag_Pan.MinX+40,
								diag_node.MinY-diag_Pan.MinY+40,
								diag_node.MaxX-diag_Pan.MinX+40,
								diag_node.MaxY-diag_Pan.MinY+40		);
	}

string		name_class;
string		name_operation;
string		name_signature;
boolean		operation_written;

	for ( i=0; i<list_count(diag_statemacs); i++ )
	{
		diag_node=list_get(diag_statemacs,i);
		diag_class=NULL;
		operation_written=False;
		if (string_find(diag_node.Name,0,"::")!=string_length(diag_node.Name))
		{
			name_class = string_extract(	diag_node.Name, 
											0, 
											string_find(diag_node.Name,0,"::")	);
			name_operation = string_extract(	diag_node.Name, 
												string_find(diag_node.Name,0,"::")+2,
												string_find(diag_node.Name,0,"(")-
												string_find(diag_node.Name,0,"::")-2	);
			name_signature = string_extract(	diag_node.Name,
												string_find(diag_node.Name,0,"(")+1,
												string_length(diag_node.Name)-
												string_find(diag_node.Name,0,"(")-2	);

			diag_class=find_by_query(	"node[node_refs & UmlClass & name='${name_class}']"	);
			diag_operation=find_by_query(	"node[node_refs & UmlOperation & " +
												"scope_node_id=${diag_class.id} & name='${name_operation}' & " +
												"sig='${name_signature}']"	);
			if (diag_operation!=NULL)
			{
				if ( idlist_link(diag_class.type,diag_class.id) )
				{
					html_content +=	html_link_area(
										"../classes/" + diag_class.name + "_" + diag_class.id + ".html#" + 
										diag_operation.name + "_" + diag_operation.id,
										"State Machine '" + diag_node.Name + "'",
										diag_node.MinX-diag_Pan.MinX+40,
										diag_node.MinY-diag_Pan.MinY+40,
										diag_node.MaxX-diag_Pan.MinX+40,
										diag_node.MaxY-diag_Pan.MinY+40		);
				}
				operation_written=True;
			}
		}
		if (!operation_written)
		{
			if (diag_class==NULL)
			{
				diag_class=find_by_query("node[node_refs & UmlClass & name='${diag_node.Name}' & sig='']");
				diag_usecase=find_by_query("node[node_refs & UmlUseCase & name='${diag_node.Name}']");
			}
			if (diag_class!=NULL)
			{
				if (diag_usecase!=NULL)
				{
// Classa and Use Case
					if ( idlist_link(diag_class.type,diag_class.id) )
					{
						html_content +=	html_link_area(
											"../classes/" + diag_class.name + "_" + diag_class.id + ".html",
											"State Machine '" + diag_node.Name + "'",
											diag_node.MinX-diag_Pan.MinX+40,
											diag_node.MinY-diag_Pan.MinY+40,
											diag_node.MaxX-diag_Pan.MinX+40,
											diag_node.MaxY-diag_Pan.MinY+40		);
					}
					else
					if ( idlist_link(diag_usecase.type,diag_usecase.id) )
					{
						html_content +=	html_link_area(
											"../usecases/" + diag_usecase.name + "_" + diag_usecase.id + ".html",
											"State Machine '" + diag_node.Name + "'",
											diag_node.MinX-diag_Pan.MinX+40,
											diag_node.MinY-diag_Pan.MinY+40,
											diag_node.MaxX-diag_Pan.MinX+40,
											diag_node.MaxY-diag_Pan.MinY+40		);
					}
				}
				else
				{
// only Class
					if ( idlist_link(diag_class.type,diag_class.id) )
					{
						html_content +=	html_link_area(
											"../classes/" + diag_class.name + "_" + diag_class.id + ".html",
											"State Machine '" + diag_node.Name + "'",
											diag_node.MinX-diag_Pan.MinX+40,
											diag_node.MinY-diag_Pan.MinY+40,
											diag_node.MaxX-diag_Pan.MinX+40,
											diag_node.MaxY-diag_Pan.MinY+40		);
					}
				}
			}
			else
			{
				if (diag_usecase!=NULL)
				{
// only Use Case
					if ( idlist_link(diag_usecase.type,diag_usecase.id) )
					{
						html_content +=	html_link_area(
											"../usecases/" + diag_usecase.name + "_" + diag_usecase.id + ".html",
											"State Machine '" + diag_node.Name + "'",
											diag_node.MinX-diag_Pan.MinX+40,
											diag_node.MinY-diag_Pan.MinY+40,
											diag_node.MaxX-diag_Pan.MinX+40,
											diag_node.MaxY-diag_Pan.MinY+40		);
					}
				}
			}
		}
	}

item		item_name,
			item_type;

	for ( i=0; i<list_count(diag_sceninsts); i++ )
	{
		diag_node=list_get(diag_sceninsts,i);
		item_name=find_by_query("item[UmlParentNameItem & obj_id=${diag_node.Id}]");
		item_type=find_by_query("item[UmlParentTypeItem & obj_id=${diag_node.Id}]");
		diag_usecase=find_by_query("node[type=${item_type.value} & name='${item_name.value}']");
		if (diag_usecase!=NULL)
    		if (diag_usecase.type=="UmlUseCase")
    			if ( idlist_link(diag_usecase.type,diag_usecase.id) )
    				html_content +=	html_link_area(
    									"../usecases/" + diag_usecase.name + "_" + diag_usecase.id + ".html",
    									"Scenario Instance '" + diag_node.Name + "'",
    									diag_node.MinX-diag_Pan.MinX+40,
    									diag_node.MinY-diag_Pan.MinY+40,
    									diag_node.MaxX-diag_Pan.MinX+40,
    									diag_node.MaxY-diag_Pan.MinY+40		);
	}

    diag_nodelist_done(diag_filelist);

// ECR 7153 comments - no more attribute and operation clickables
//	list_clear(diag_operations);
//	list_clear(diag_attributes);

	list_clear(diag_classes);
	list_clear(diag_stereotypes);
	list_clear(diag_usecases);
	list_clear(diag_actors);
	list_clear(diag_packages);
	list_clear(diag_statemacs);
	list_clear(diag_sceninsts);

    html_content += "</MAP>\n";
    html_content += "<TABLE CELLSPACING=0 CELLPADDING=0 BORDER=2>\n<TR>\n<TD>\n";
    html_content += "<IMG SRC=\"" + html_escape_chars(diag_header + "_" + diag_file.name) + ".jpg\" USEMAP=\"#map\" ALT=\"" +
					html_escape_chars(diag_file.type + " '" + diag_file.name + "'") + "\" BORDER=0>\n";
    html_content += "</TD>\n</TR>\n</TABLE>\n";

    html_content += html_page_end();
    write_file(HTML_BASEPATH + "/" + html_file, html_content);

    return;
}

diag_nodelist_info_type diag_nodelist_get( list diag_filelist, int idx, int diag_id )
{
diag_nodelist_info_type		ret_node=NULL;
node						node_var;
int							i,j;
int							brackets;
int							cur_x,cur_y;
diag_nodelist_factor_type	fct;

    i=-1;
    for (j=-1;j<idx;j++)
    {
		i++;
		while( ((list_get(diag_filelist, i) != "Node")  && (list_get(diag_filelist, i) != "Cntx")) || 
			   (list_get(diag_filelist, i+1)=="}") )
			i++;
    }

    brackets=1;
    i+=3;
    ret_node.Id=NULL;
    while ((brackets>0)&&(i<list_count(diag_filelist)))
    {
		if (list_get(diag_filelist, i) == "{") brackets++;
		if (list_get(diag_filelist, i) == "}") brackets--;
        if (brackets==2)
        {
			if (list_get(diag_filelist, i) == "Type")
				ret_node.Type=list_get(diag_filelist,i+1);
			if (list_get(diag_filelist, i) == "Loc")
			{
				ret_node.MinX=ret_node.MaxX=to_int(list_get(diag_filelist, i+1));
				ret_node.MinY=ret_node.MaxY=to_int(list_get(diag_filelist, i+2));
			}
			if (list_get(diag_filelist, i) == "Scale")
			{
				ret_node.ScaleX=to_float(list_get(diag_filelist, i+1));
				ret_node.ScaleY=to_float(list_get(diag_filelist, i+2));
			}
		}
        if (brackets==3)
		{
			if (list_get(diag_filelist, i) == "PDMType")
				if (list_get(diag_filelist, i+1) == "Node")
				{
					while (list_get(diag_filelist, i) != "ID")
					{
						if (list_get(diag_filelist, i) == "{") brackets++;
						if (list_get(diag_filelist, i) == "}") brackets--;
						i++;
					}
					ret_node.Id=to_int(list_get(diag_filelist,i+1));
				}
		}
		i++;
    }
    if (ret_node.Id!=NULL)
    {
		node_var=find_by_query("Node[id=${ret_node.Id}]");
		ret_node.Name=node_var.name;
	}
    fct=diag_nodelist_get_factor(ret_node.Type,diag_id);
    ret_node.MinX-=to_int(ret_node.ScaleX*fct.x/256.0);
    ret_node.MinY-=to_int(ret_node.ScaleY*fct.y/256.0);
    ret_node.MaxX+=to_int(ret_node.ScaleX*fct.x/256.0);
    ret_node.MaxY+=to_int(ret_node.ScaleY*fct.y/256.0);
	
// ECR 7153 start
	
	if (ret_node.Type=="Actor")					{	ret_node.MinX-=5;	ret_node.MaxX-=5;
													ret_node.MinY+=10;	ret_node.MaxY+=10;	}	else
	if (ret_node.Type=="UseCase")				{	ret_node.MinX-=5;	ret_node.MaxX-=5;
													ret_node.MinY+=10;	ret_node.MaxY+=10;	}	else
    if (ret_node.Type=="Package")				{	ret_node.MinX-=5;	ret_node.MaxX-=5;
													ret_node.MinY-=5;	ret_node.MaxY-=5;	}	else
	if (ret_node.Type=="Class")					{	ret_node.MinX-=15;	ret_node.MaxX+=5;
													ret_node.MinY+=5;	ret_node.MaxY+=15;	}	else
	if (ret_node.Type=="Attribute")				{	ret_node.MinX-=10;	ret_node.MaxX-=10;	
													ret_node.MinY+=15;	ret_node.MaxY+=15;	}	else
	if (ret_node.Type=="Operation")				{	ret_node.MinX-=10;	ret_node.MaxX-=10;	
													ret_node.MinY+=15;	ret_node.MaxY+=15;	}	else
	if (ret_node.Type=="InstantiatedClass")		{	ret_node.MinX-=15;	ret_node.MaxX+=5;
													ret_node.MinY+=5;	ret_node.MaxY+=15;	}	else
	if (ret_node.Type=="ParameterizedClass")	{	ret_node.MinX-=5;	ret_node.MaxX+=10;
													ret_node.MinY+=5;	ret_node.MaxY+=15;	}	else
	if (ret_node.Type=="Stereotype")			{	ret_node.MinX-=5;	ret_node.MaxX-=5;
													ret_node.MinY+=5;						}	else
	if (ret_node.Type=="System")				{	ret_node.MinY-=30;						}
	else										{	ret_node.MinY-=10;	ret_node.MaxY-=10;
													ret_node.MinX-=10;	ret_node.MaxX-=10;	}
 
// ECR 7153 end 
 
    return( ret_node );
}

int diag_nodelist_count ( list diag_filelist )
{
int		i,j;

    j=0;
    for (i=0;i<list_count(diag_filelist);i++)
        if (((list_get(diag_filelist, i) == "Node")||(list_get(diag_filelist, i) == "Cntx")) &&
			(list_get(diag_filelist, i+1)!="}") ) 
			j++;

    return( j );
}

list diag_nodelist_init ( int diag_id )
{
file		diag;
string		diag_filename;
string		diag_filetext;
list		diag_filelist;

    
    diag = find_by_query("file[${diag_id}]");
    diag_filename = current_projdir() + 
		    current_system() + 
		    "/" + diagram_type_convert(diag.type) + "_files/" +
		    diag.name + "." + diagram_type_convert(diag.type);
    diag_filetext = read_file( diag_filename );
    diag_filelist = string_to_list( diag_filetext, " \n" );
    return( diag_filelist );
}

void diag_nodelist_done ( list diag_filelist )
{
    list_clear( diag_filelist );
}

diag_nodelist_extremes_type diagram_find_extremes( int diag_id )
{
list							diag_filelist;
diag_nodelist_extremes_type		ret_extr=NULL;
diag_nodelist_info_type			diag_node=NULL;
int								i,j,cnt;

    diag_filelist=diag_nodelist_init(diag_id);    
    cnt=diag_nodelist_count( diag_filelist );
    for ( i=0; i<cnt; i++ )
    {
		diag_node=diag_nodelist_get(diag_filelist,i,diag_id);
		if (ret_extr==NULL)
		{
			ret_extr.MinX=diag_node.MinX;
			ret_extr.MinY=diag_node.MinY;
			ret_extr.MaxX=diag_node.MaxX;
			ret_extr.MaxY=diag_node.MaxY;
		}
		else
		{
			if (ret_extr.MinX>diag_node.MinX) ret_extr.MinX=diag_node.MinX;
			if (ret_extr.MinY>diag_node.MinY) ret_extr.MinY=diag_node.MinY;
			if (ret_extr.MaxX<diag_node.MaxX) ret_extr.MaxX=diag_node.MaxX;
			if (ret_extr.MaxY<diag_node.MaxY) ret_extr.MaxY=diag_node.MaxY;
		}
	}
    diag_nodelist_done(diag_filelist);
    return(ret_extr);
}

string diagram_type_convert( string oms_type )
{
  if( oms_type == "UmlUseCaseDiagram" )
    return( "uusecased" );
  if( oms_type == "UmlClassDiagram" )
    return( "uclassd" );
  if( oms_type == "UmlSequenceDiagram" )
    return( "usequenced" );
  if( oms_type == "UmlActivityDiagram" )
    return( "uactivityd" );
  if( oms_type == "UmlComponentDiagram" )
    return( "ucomponentd" );
  if( oms_type == "UmlCollaborationDiagram" )
    return( "ucollaborationd" );
  if( oms_type == "UmlStateDiagram" )
    return( "ustated" );
  if( oms_type == "UmlDeploymentDiagram" )
    return( "udeploymentd" );
  if( oms_type == "UmlStereotypeDiagram" )
    return( "ustereotyped" );
  if( oms_type == "OMTDiagram" )
    return( "omt" );
  if( oms_type == "OMTObjectDiagram" )
    return( "ome" );
  if( oms_type == "OMTStateDiagram" )
    return( "dme" );
  if( oms_type == "BoochObjectDiagram" )
    return( "object" );

  message( "Unknown Type " + oms_type );
  return( "" );
}

diag_nodelist_factor_type diag_nodelist_get_factor( string name, int diag_id )
{
diag_nodelist_factor_type	fct=NULL;

    if (name=="Class")                    { fct.x=50.0;  fct.y=50.0;  return (fct); }
    if (name=="Stereotype")				  { fct.x=90.0;  fct.y=50.0;  return (fct); }
    if (name=="InstantiatedClass")        { fct.x=50.0;  fct.y=50.0;  return (fct); }
    if (name=="ParameterizedClass")       { fct.x=50.0;  fct.y=50.0;  return (fct); }
    if (name=="Vertex")                   { fct.x=0.0;   fct.y=0.0;   return (fct); } 
    if (name=="Attribute")                { fct.x=30.0;  fct.y=50.0;  return (fct); }
    if (name=="Operation")                { fct.x=30.0;  fct.y=50.0;  return (fct); }
    if (name=="Interface")                { fct.x=10.0;  fct.y=10.0;  return (fct); }
    if (name=="Package")                  { fct.x=50.0;  fct.y=40.0;  return (fct); }
    if (name=="UseCase")                  { fct.x=50.0;  fct.y=50.0;  return (fct); }
    if (name=="Actor")               
    {
	if(selection_count("file[UmlSequenceDiagram & ${diag_id}]") != 0)
	{ fct.x=20.0;  fct.y=20.0; }
	else
	if(selection_count("file[UmlCollaborationDiagram & ${diag_id}]") != 0)
	{ fct.x=50.0;  fct.y=50.0; }
	else
	{ fct.x=63.0;  fct.y=125.0; }
	return (fct);
    }
    if (name=="System")                   { fct.x=50.0;  fct.y=50.0;  return (fct); }
    if (name=="Object")                   { fct.x=50.0;  fct.y=50.0;  return (fct); }
    if (name=="PassiveObject")            { fct.x=20.0;  fct.y=20.0;  return (fct); }
    if (name=="ActiveObject")             { fct.x=20.0;  fct.y=20.0;  return (fct); }
    if (name=="ScenarioInstance")         { fct.x=30.0;  fct.y=50.0;  return (fct); }
    if (name=="StateMachine")             { fct.x=30.0;  fct.y=50.0;  return (fct); }
    if (name=="State")                    { fct.x=50.0;  fct.y=50.0;  return (fct); }
    if (name=="InitialState")             { fct.x=50.0;  fct.y=50.0;  return (fct); }
    if (name=="FinalState")               { fct.x=50.0;  fct.y=50.0;  return (fct); }
    if (name=="CompositeState")           { fct.x=75.0;  fct.y=110.0; return (fct); }
    if (name=="SplitControl")             { fct.x=20.0;  fct.y=20.0;  return (fct); }
    if (name=="MergeControl")             { fct.x=50.0;  fct.y=50.0;  return (fct); }
    if (name=="Decision")                 { fct.x=50.0;  fct.y=50.0;  return (fct); }
    if (name=="Note")                     { fct.x=40.0;  fct.y=20.0;  return (fct); }
    if (name=="Ellipsis")                 { fct.x=25.0;  fct.y=5.0;   return (fct); }
    if (name=="InstanceAttribute")        { fct.x=30.0;  fct.y=50.0;  return (fct); }
    if (name=="RelLabelFrom")             { fct.x=30.0;  fct.y=50.0;  return (fct); }
    if (name=="sourceComponent")          { fct.x=88.0;  fct.y=92.0;  return (fct); }
    if (name=="executableComponent")      { fct.x=88.0;  fct.y=92.0;  return (fct); }
    if (name=="binaryComponent")          { fct.x=88.0;  fct.y=92.0;  return (fct); }
    if (name=="interfaceComponent")       { fct.x=50.0;  fct.y=50.0;  return (fct); }
    if (name=="objectComponent")          { fct.x=50.0;  fct.y=50.0;  return (fct); }
    if (name=="deploymentComponent")      { fct.x=100.0; fct.y=100.0; return (fct); }
    if (name=="ExtensionPoint")           { fct.x=30.0;  fct.y=50.0;  return (fct); }
    if (name=="TimeSent")                 { fct.x=30.0;  fct.y=50.0;  return (fct); }
    if (name=="TimeDelivered")            { fct.x=30.0;  fct.y=50.0;  return (fct); }
    if (name=="Collaboration")            { fct.x=50.0;  fct.y=50.0;  return (fct); }
    if (name=="PassiveCompositeObject")   { fct.x=50.0;  fct.y=50.0;  return (fct); }
    if (name=="MultiObject")              { fct.x=60.0;  fct.y=60.0;  return (fct); }
    if (name=="TypeElement")              { fct.x=50.0;  fct.y=50.0;  return (fct); }
    if (name=="SimpleMessage")            { fct.x=50.0;  fct.y=30.0;  return (fct); }
    if (name=="ReturnMessage")            { fct.x=50.0;  fct.y=30.0;  return (fct); }
    if (name=="SynchronousMessage")       { fct.x=50.0;  fct.y=30.0;  return (fct); }
    if (name=="AsynchronousMessage")      { fct.x=50.0;  fct.y=30.0;  return (fct); }
    if (name=="BalkingMessage")           { fct.x=50.0;  fct.y=30.0;  return (fct); }
    if (name=="TimeoutMessage")           { fct.x=50.0;  fct.y=30.0;  return (fct); }
    if (name=="NaryAssociationHub")       { fct.x=50.0;  fct.y=50.0;  return (fct); }
    if (fct==NULL)
    {
//	message(name);
	fct.x=0.0;
	fct.y=0.0;
    }
    return (fct);
}
