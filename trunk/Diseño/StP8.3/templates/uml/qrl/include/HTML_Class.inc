////////////////////////////////////////////////////////////////////////////////
//
// HTML_Class
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// DEFINITIONS:
///////////////////////////////////////////////////////////////////////////////

struct class_assorole_type
{
node	a_class;
link	a_role;
// boolean nav;
string  type;

};

struct class_asso_type
{
node	assoc;				// association node
list    link_list;
};

///////////////////////////////////////////////////////////////////////////////
//  SOURCE:
///////////////////////////////////////////////////////////////////////////////

void class_start()
{
int		ccnt, cx;
node	Class;

    ccnt      = list_count(idlist_class);

    message("- Generating Class pages ..."); 

     // generate the index page first
    rep_gen_indexpage("Class", "classes/", idlist_class);

    // no classes in primary selection -> finished
    if (ccnt < 1)
    {
        message("\n\nNo Classes selected / specified !");
        return;
    }
     
    for (cx = 0; cx < ccnt; cx++) 
    {
		Class = find_by_query("node[UmlClass & id=" + list_get(idlist_class, cx) + "]");
		
		message( "- Generating Class " + idlist_get_nodename(Class.id) + " (" + to_string(cx+1) + " of " + to_string(ccnt) + ")" );

        // write one file for each class
        class_gen_page(Class);
    }
    return;
}

void class_gen_page(node Class)
{
string	html_file = "classes/" + Class.name + "_" + Class.id + ".html";
string	html_content;
string	class_heading="";
item	infoItem;

    // Print standard HTML header
    html_content = html_page_start( "StP Class Description: " + idlist_get_nodename(Class.id) );

    // Print reference section at top of the page
    html_content += html_ref_prevnext(Class.id, idlist_class);

    // Print class heading
	if (selection_count("node[node_refs & type=UmlParameterizedClass & name='${Class.name}']")>0)
		class_heading+="parameterized ";
	else
	if (selection_count("node[node_refs & type=UmlInstantiatedClass & name='${Class.name}']")>0)
		class_heading+="instantiated ";
    infoItem = find_by_query("item[UmlClassIsImported && obj_id=" + Class.id + "]");
    if (infoItem != NULL && infoItem.value == "True")
		class_heading+="external ";
    infoItem = find_by_query("item[UmlClassIsAbstract && obj_id=" + Class.id + "]");
    if (infoItem != NULL && infoItem.value == "True")
		class_heading+="abstract ";
	class_heading += "class "+ Class.name;
	if (selection_count("node[node_refs & type=UmlParameterizedClass & name='${Class.name}']")>0)
	{
		infoItem = find_by_query("item[UmlClassParameters && obj_id=" + Class.id + "]");
		class_heading = html_escape_chars(class_heading);
		if (infoItem != NULL)
			class_heading+="&lt;" + class_scan_for_links(infoItem.value) + "&gt;";
		else
			class_heading+="&lt;&gt;";
	}
	else
	if ((selection_count("node[node_refs & type=UmlInstantiatedClass & name='${Class.name}']")>0) && (Class.sig!=""))
	{
		class_heading = html_escape_chars(class_heading);
		class_heading += "&lt;" + class_scan_for_links(Class.sig) + "&gt;";
	}
	
    html_content += "<H1>" + class_heading + "</H1>\n";

    // Print class info
    html_content += html_gen_info (Class);

list	diags=list_create("string",0);

	list_append(diags,"UmlClassDiagram");
	list_append(diags,"UmlComponentDiagram");
	list_append(diags,"UmlDeploymentDiagram");
	html_content += html_ref_diag( Class, diags );
	list_clear(diags);

	html_content += html_ref_statemac( Class );

    html_content += html_seperator("");
	html_content += "<BR>\n";

    // Print class in tree
    html_content += class_gen_tree (Class);

    html_content += html_seperator("");
	html_content += "<BR>\n";

    // Print Class Member info
    html_content += class_gen_member_info(Class);

    html_content += html_page_end();
    // write the HTML file
    write_file(HTML_BASEPATH + "/" + html_file, html_content);
    return;
}

string class_gen_member_info(node Class)
{
string	content;
set		attributeSet;
set		constructorSet;
set		methodSet;
set		allOperationSet;
node	oneOp;
int		i;

    // populate class member sets
    attributeSet = set_select("node[UmlAttribute && node_refs && scope_node_id=" + Class.id + "] sort by name"); 
    allOperationSet = set_select("node[UmlOperation && node_refs && scope_node_id=" + Class.id + "] sort by name");
    constructorSet = set_create("node");
    methodSet = set_create("node");
    
    for (i=0; i<set_count(allOperationSet); i++)
    {
        oneOp = set_get_element(allOperationSet, i);
        
        if (oneOp.name == Class.name)                // Constructor
            set_add(constructorSet, oneOp);
        else if (string_find(oneOp.name, 0, "~") == 0)  // Destructor in C++
             set_add(constructorSet, oneOp);
        else if (oneOp.name == "finalize")           // Destructor in Java
             set_add(constructorSet, oneOp);
        else
             set_add(methodSet, oneOp);
    }
    
    // Print class member index
    content = class_gen_member_index (Class, attributeSet, constructorSet, methodSet);

    // Print class variables
    if (set_count(attributeSet) > 0)
        content += class_gen_att (attributeSet);

    // Print class constructors
    if (set_count(constructorSet) > 0)
        content += class_gen_op (constructorSet, "Constructors");

    // Print class methods
    if (set_count(methodSet) > 0)
        content += class_gen_op (methodSet, "Methods");

    // clean up
    set_clear(attributeSet);
    set_clear(constructorSet);
    set_clear(methodSet);
    set_clear(allOperationSet);

    return(content);
}


string class_gen_tree(node Class)
{
string					content;
string					query;
list					ids=NULL,jds=NULL;
     
string					part_content;
list					ref_list;
node					ref_node;
item					ref_item;
boolean					ref_first=True;

list					association_list=NULL;
node					association_node;
class_asso_type			association_var=NULL;
class_assorole_type		association_role_var=NULL;
item					association_item;
int						i,j;

string					assotype;
int						assonumber;
string					assoname;
node					assoclass_node=NULL;
boolean					assowritten=False;

list					assoclass_assos=NULL;
node					assoclass_assovar;
set						assoclass_classes;
node					assoclass_classvar;
string idList; // ECR 7496

    // Package Class belongs to
    query = "node[UmlPackage && node_refs && out_links[UmlContains && "
            + "link_refs && to_node_id = " + Class.id + "]] sort by name"; 
    content = html_ref_line(query, "Packages: ", "../packages/", 2);

	// Refinement of Class
	part_content = "";
	ref_list = list_select(	"node[type=UmlClass & node_refs & in_links[type=UmlRefines & link_refs & from_node_id=" + 
							Class.id + "]] sort by name"	);
	for (i=0;i<list_count(ref_list);i++)
	{
		if (ref_first)
			ref_first=False;
		else
			part_content += "<BR>\n";
		
		ref_node = list_get(ref_list, i);
		
		if (idlist_link("UmlClass",ref_node.id))
			part_content += html_link( ref_node.name + "_" + ref_node.id + ".html", idlist_get_nodename(ref_node.id), "" );
		else
			part_content += html_escape_chars( idlist_get_nodename(ref_node.id) );
		part_content += html_escape_chars( " - " );
		part_content += html_style( "B", idlist_get_nodename(Class.id) );
		part_content +=	html_escape_chars( " refines arguments of " );
		part_content += html_style( "B", idlist_get_nodename(ref_node.id) );
		part_content +=	html_escape_chars( " to " );
		ref_item = find_by_query("item[type=UmlRefineArgs & link[link_refs & from_node[id=${Class.id}] & to_node[id=${ref_node.id}]]]");
		if (ref_item!=NULL)
			part_content += html_style( "B", "<" + ref_item.value + ">" );
		else
			part_content += html_style( "B", "<>" );
	}
	if (part_content!="")
		content += html_table( html_style("B", "Refinement of Class: "), part_content) + "<BR>\n";

	// superclasses of Class
    query = "node[UmlClass && node_refs && in_links[UmlGeneralization && link_refs && from_node_id = " 
             + Class.id + "]] sort by name";
    content += html_ref_line(query, "Superclasses: ", "", 2);

	// indirect superclasses of Class
    query = "node[UmlClass && node_refs && in_links[UmlGeneralization && link_refs && from_node_id = ${id}]]";
	idlist_get_hierarchie(query, Class.id, False, ids);
	if(list_count(ids) != 0)
	{
                // ECR 7496 -->
		idList = list_to_idlist(ids, "node");
		// query = "node[id = " + list_to_string(ids, " || id = ") + "] sort by name";
		query = "node[" + idList + "] sort by name";
		content += html_ref_line(query, "Indirect Superclasses: ", "", 2);
		id_list_free(idList);
                // ECR 7496 <--
	}
	list_clear(ids);

	// Refined Classes
	if (selection_count("node[node_refs & type=UmlParameterizedClass & name='" + Class.name + "']")>0)
	{
	    query = "node[type=UmlClass & node_refs & out_links[link_refs & type=UmlRefines & to_node_id=" + 
				Class.id + "]] sort by name";
		content += html_ref_line(query, "Refined Classes: ", "", 2);
	}

    // subclasses of Class
    query = "node[UmlClass && node_refs && out_links[UmlGeneralization && link_refs && to_node_id = " 
            + Class.id + "]] sort by name"; 
    content += html_ref_line(query, "Subclasses: ", "", 2);

	// indirect subclasses of Class
    query = "node[UmlClass && node_refs && out_links[UmlGeneralization && link_refs && to_node_id = ${id}]]";
	idlist_get_hierarchie(query, Class.id, False, ids);
	if(list_count(ids) != 0)
	{
                // ECR 7496 -->
		idList = list_to_idlist(ids, "node");
		// query = "node[id = " + list_to_string(ids, " || id = ") + "] sort by name";
		query = "node[" + idList + "] sort by name";
		content += html_ref_line(query, "Indirect Subclasses: ", "", 2);
		id_list_free(idList);
                // ECR 7496 <--
	}
	list_clear(ids);
	
	// Class Dependencies to
	query = "node[node_refs && in_links[UmlDependency && link_refs && from_node_id = "
			+ Class.id + "]] sort by name";
    content += html_ref_line(query, "Dependency to: ", "", 2);

	// Class Dependencies from
	query = "node[node_refs && out_links[UmlDependency && link_refs && to_node_id = "
			+ Class.id + "]] sort by name";
    content += html_ref_line(query, "Dependency from: ", "", 2);

	// Class Associations, Compositions and Aggregations
	ids=list_select("node[UmlAssociation && in_links[UmlRole && link_refs && from_node_id = " + Class.id + "]]");
	association_list=list_create("class_asso_type",0);
	for ( i=0; i<list_count(ids); i++ )
	{
		association_node=list_get(ids,i);
		association_var.assoc=association_node;
		association_var.link_list=list_create("class_assorole_type",0);
		jds=list_select("link[UmlRole && link_refs && to_node[UmlAssociation && node_refs && id = " + association_var.assoc.id + "]]");
		for ( j=0; j<list_count(jds); j++)
		{
			association_role_var.a_role=list_get(jds,j);
			association_role_var.a_class=find_by_query("node[UmlClass && out_links[UmlRole && id = " +
														association_role_var.a_role.id + " && link_refs]]");
			association_item=find_by_query("item[UmlAggregationType && link[id = " + 
											association_role_var.a_role.id + "]]");
			if (association_item!=NULL)
				association_role_var.type=association_item.value; 
			else
				association_role_var.type="None";
//			association_item=find_by_query("item[UmlRoleNavigability && link[id = " + 
//											association_role_var.a_role.id + "]]");
//			if (association_item!=NULL)
//				association_role_var.nav=to_boolean(association_item.value);
//			else
//				association_role_var.nav=False;
			list_append(association_var.link_list,association_role_var);
		}
		list_append(association_list,association_var);
		list_clear(jds);
	}

	if (list_count(association_list)>0)
	{
		content+=html_style("B", "Associations:") + "<BR>\n";
		assowritten=True;
		for (i=0;i<list_count(association_list);i++)
		{
			association_var=list_get(association_list,i);
			for (j=0;j<list_count(association_var.link_list);j++)
			{
				association_role_var=list_get(association_var.link_list,j);
				if (association_role_var.a_class.id==Class.id) assotype=association_role_var.type;
			}
			assonumber=0;
			if (i>0) content += "<BR>\n";
			content+="<LI>" + html_style("B",Class.name) + " is connected to ";
			for (j=0;j<list_count(association_var.link_list);j++)
			{
				association_role_var=list_get(association_var.link_list,j);
				if (association_role_var.a_class.id!=Class.id)
				{
					if (assonumber>0) content += ", ";
					assonumber++;
					if (idlist_link("UmlClass",association_role_var.a_class.id))
						content += html_link(	association_role_var.a_class.name + "_" + 
												association_role_var.a_class.id + ".html",
												idlist_get_nodename(association_role_var.a_class.id), "" );
					else
						content += html_style("B", idlist_get_nodename(association_role_var.a_class.id) );
				}
			}
			if (assonumber==0) content+="itself";
			
			if (string_find(association_var.assoc.name,0,":")==string_length(association_var.assoc.name))
			{
				if (assonumber>1) content+=" through a n-ary association";
				else
				{
					if (assotype=="Composition") content+=" through a composition";
					if (assotype=="Aggregation") content+=" through an aggregation";
					if (assotype=="None") content+=" through an association";
				}
			}
			else
			{
				j=-1;
				while (string_find(association_var.assoc.name,j+1,":")!=string_length(association_var.assoc.name))
					j=string_find(association_var.assoc.name,j+1,":");
				assoname=string_extract(association_var.assoc.name,0,j);
				if (assonumber>1) content+=" through the n-ary association ";
				else
				{
					if (assotype=="Composition") content+=" through the composition ";
					if (assotype=="Aggregation") content+=" through the aggregation ";
					if (assotype=="None") content+=" through the association ";
				}
				content+=html_style("B",assoname);
			}
			
			assoclass_node=find_by_query(	"node[UmlClass && node_refs && in_links[UmlAssociationClassLink & link_refs && " +
											"from_node[id=${association_var.assoc.id}]]]"	);
			if (assoclass_node!=NULL)
			{
				content += " with association class ";
				if (idlist_link("UmlClass",assoclass_node.id))
					content += html_link(	assoclass_node.name + "_" +
											assoclass_node.id + ".html",
											assoclass_node.name , "");
				else
					content += html_style("B", assoclass_node.name);
			}
			
			content+=".</LI>";
		}
	}
	list_clear(ids);
	list_clear(association_list);
	
	// Class is association class ?
	assoclass_assos=list_select(	"node[UmlAssociation && node_refs &&" +
									"out_links[UmlAssociationClassLink && link_refs && to_node[id=${Class.id}]]]");
	if (list_count(assoclass_assos)>0)
	{
		if (!assowritten)
		{
			content+=html_style("B", "Associations:");
		}
		for (i=0;i<list_count(assoclass_assos);i++)
		{
			assoclass_assovar=list_get(assoclass_assos,i);
			assoclass_classes=set_select(	"node[UmlClass && node_refs && out_links[UmlRole && link_refs && " +
											"to_node[id=${assoclass_assovar.id}]]]");
			if (string_find(assoclass_assovar.name,0,":")==string_length(assoclass_assovar.name))
			{
				content +=	"<BR>\n<LI>" + html_style("B",Class.name) + " is an association class for the association" +
							", which connects the class ";
			}
			else
			{
				j=-1;
				while (string_find(assoclass_assovar.name,j+1,":")!=string_length(assoclass_assovar.name))
					j=string_find(assoclass_assovar.name,j+1,":");
				assoname=string_extract(assoclass_assovar.name,0,j);
				content +=	"<BR>\n<LI>" + html_style("B",Class.name) + " is an association class for the association " +
							html_style("B",assoname) + ", which connects the class ";
			}
			assoclass_classvar=set_get_element(assoclass_classes,0);
			if (idlist_link("UmlClass",assoclass_classvar.id))
				content += html_link(	assoclass_classvar.name + "_" +
										assoclass_classvar.id + ".html",
										idlist_get_nodename(assoclass_classvar.id), "") + " to ";
			else
				content += html_style("B", idlist_get_nodename(assoclass_classvar.id)) + " to ";
			if (set_count(assoclass_classes)==1)
			{
				content+="itself";
			}
			else
			{
				for (j=1;j<set_count(assoclass_classes);j++)
				{
					if (j>1) content+=", ";
					assoclass_classvar=set_get_element(assoclass_classes,j);
					if (idlist_link("UmlClass",assoclass_classvar.id))
						content += html_link(	assoclass_classvar.name + "_" +
												assoclass_classvar.id + ".html",
												idlist_get_nodename(assoclass_classvar.id), "");
					else
						content += html_style("B", idlist_get_nodename(assoclass_classvar.id));
				}
			}
			content+=".</LI>";
			set_clear(assoclass_classes);
		}
		list_clear(assoclass_assos);
		content+="<BR>\n<BR>\n";
	}
	else
	{
		if (assowritten)
		{
			content+="<BR>\n<BR>\n";
		}
	}

	return(content);
}

string class_gen_member_index(node htmlClass, set attributeSet, set constructorSet, set methodSet)
{
int		acnt = 0,
		scnt;
node	memberNode;
string	content = "";

    // Print variable index
    scnt = set_count(attributeSet);
    acnt = acnt + scnt;

    if (scnt > 0)
        content += class_gen_member_index_print(attributeSet, scnt, "Attribute Index");

    // Print constructor index
    scnt = set_count(constructorSet);
    acnt = acnt + scnt;

    if (scnt > 0)
        content += class_gen_member_index_print(constructorSet, scnt, "Constructor Index");

    // Print method index
    scnt = set_count(methodSet);
    acnt = acnt + scnt;

    if (scnt > 0)
        content += class_gen_member_index_print(methodSet, scnt, "Method Index");

    if (acnt > 0)
        content += html_seperator("");

    return(content);
}

string class_gen_member_index_print(set memSet, int mcnt, string memTitle)
{
int		mx;
node	memNode;
string	content;

    content = html_style ("H2", memTitle) + "\n";
    content += "<UL>\n";

    for (mx = 0; mx < mcnt; mx++) {
        memNode = set_get_element(memSet, mx);    

        if (memTitle == "Attribute Index")   
            content += "<LI>" + html_link ("#" + memNode.name + "_" + memNode.id, memNode.name, "") + "</LI>\n";
        else {
            content += "<LI>" + html_link("#" + memNode.name + "_" + memNode.id, memNode.name + "(" + memNode.sig + ")", "") + "</LI>\n";
        }
    }  
    content += "</UL>\n<BR>\n";
    return(content);
}

string class_gen_att(set attributeSet)
{
int		scnt, sx, i;
item	memberItem;
note	memberNote;
node	memberNode;
node	stereot;
string	content="";
string  content2="";

    scnt = set_count(attributeSet);

    // Print variables
    content += html_style ("H2", "Attributes") + "\n";

    for (sx = 0; sx < scnt; sx++) 
    {
		content2="";

        memberNode = set_get_element(attributeSet, sx);
        content +=	"<UL><LI><B>" + 
					html_label( memberNode.name + "_" + memberNode.id, memberNode.name ) +
					"</B></LI></UL>\n";

        content2 += class_gen_att_lang_info(memberNode);

        memberItem = find_by_query("item[UmlStereotype && obj_id=" + memberNode.id + "]");
        if (memberItem != NULL && class_strip_info(memberItem.value) != "")
        {
			stereot = find_by_query("node[type=UmlStereotype && node_refs && name='${memberItem.value}']");
			if (stereot!=NULL)
			{
				if (idlist_link("UmlStereotype",stereot.id))
					content2 += html_style("B", "Stereotype:") + " " +
								html_link(	"../stereotypes/" + memberItem.value + "_" + stereot.id +
											".html", memberItem.value, "") +
								"<BR>\n";
				else
					content2 += html_style("B", "Stereotype:") + " " +
								memberItem.value + "<BR>\n";
			}
			else
			{
				if (idlist_link("UmlStereotype",idlist_predef_stereot_id(memberItem.value,memberNode.type)))
					content2 += html_style("B", "Stereotype:") + " " +
								html_link(	"../stereotypes/" + memberItem.value + "_pre" +
											".html", memberItem.value, "") + 
								"<BR>\n";
				else
					content2 += html_style("B", "Stereotype:") + " " +
								memberItem.value + "<BR>\n";
			}
		}

        memberItem = find_by_query("item[UmlAttributeDefaultValue && obj_id=" + memberNode.id + "]");
        if (memberItem != NULL && class_strip_info(memberItem.value) != "")
        {
            content2 += html_print_description("Default Value: ", memberItem.value, 1);
		}

        memberItem = find_by_query("item[UmlAttributeIsDerived && obj_id=" + memberNode.id + "]");
        if (memberItem != NULL && memberItem.value == "True")
            content2 += html_print_description("Derived:", memberNode.name + " is derived", 1);

        content2 += html_print_obj_description("GenericObject", memberNode.id);
        
        content += html_indent(content2,1);

        content += "<BR>\n";  // more distance to next Attribute
    }
    content += html_seperator("") + "<BR>\n";
    
    return(content);
}

string class_gen_att_lang_info(node memberNode)
{
string	content="";
string	PrLang=to_upper(e_Programming_Language);
item	memberItem;
    
    if (PrLang == "C++") {
        memberItem = find_by_query("item[UmlMemberCxxVisibility && obj_id=" + memberNode.id + "]");
        if (memberItem != NULL && memberItem.value != "")
            content += html_escape_chars(to_lower(memberItem.value)) + " ";
        else 
            content += class_add_val_to_string(memberNode.id, "UmlMemberVisibility",  "", " ", True, False);

        content += class_add_const_to_string("static ", memberNode.id, "UmlMemberIsClass");
        content += class_add_const_to_string("const ", memberNode.id, "UmlMemberCxxIsConst");
        content += class_add_const_to_string("volatile ", memberNode.id, "UmlAttributeCxxIsVolatile");
        content += class_add_val_to_string(memberNode.id, "UmlAttributeType",  "", " ", False, True);

        content += html_escape_chars(memberNode.name);
        return(html_style("B","C++:")+"<BR>\n"+html_indent(content, 1));

    }
    
    if (PrLang == "JAVA") {
        memberItem = find_by_query("item[UmlMemberJavaVisibility && obj_id=" + memberNode.id + "]");
        if (memberItem != NULL && memberItem.value != "")
            content += html_escape_chars(to_lower(memberItem.value)) + " ";
        else 
            content += class_add_val_to_string(memberNode.id, "UmlMemberVisibility",  "", " ", True, False);

        content += class_add_const_to_string("static ", memberNode.id, "UmlMemberIsClass");
        content += class_add_const_to_string("final ", memberNode.id, "UmlMemberJavaIsFinal");
        content += class_add_const_to_string("transient ", memberNode.id, "UmlAttributeJavaIsTransient");
        content += class_add_const_to_string("volatile ", memberNode.id, "UmlAttributeJavaIsVolatile");
        content += class_add_val_to_string(memberNode.id, "UmlAttributeType",  "", " ", False, True);

        content += html_escape_chars(memberNode.name);
        return(html_style("B","Java:")+"<BR>\n"+html_indent(content, 1));
    }

    if (PrLang == "IDL") {
        content += class_add_const_to_string("readonly ", memberNode.id, "UmlAttributeIDLIsReadOnly");
        content += "attribute "; // always
        memberItem = find_by_query("item[UmlAttributeIDLType && obj_id=" + memberNode.id + "]");
        if (memberItem != NULL && memberItem.value != "")
            content += html_escape_chars(memberItem.value) + " ";
        else 
            content += class_add_val_to_string(memberNode.id, "UmlAttributeType",  "", " ", False, True);

        content += html_escape_chars(memberNode.name);
        return(html_style("B","IDL:")+"<BR>\n"+html_indent(content, 1));
    }
    return(content);
}

string class_gen_op(set memberSet, string memTitle)
{
int		scnt, sx, i;
item	memberItem;
note	memberNote;
node	memberNode;
node	stereot;
string	content="";
string  content2="";

    scnt = set_count(memberSet);

    // Print title
    content += html_style ("H2", memTitle) + "\n";

    for (sx = 0; sx < scnt; sx++) 
    {
		content2="";
		
        memberNode   = set_get_element(memberSet, sx);
        content +=	"<UL><LI><B>" + 
					 html_label( memberNode.name + "_" + memberNode.id, memberNode.name ) + 
					"</B></LI></UL>\n";

        content2 += class_gen_op_lang_info(memberNode);

        if (memTitle == "Constructors")   // Constructors only
        {
            memberItem = find_by_query("item[UmlOperationCxxCtorInitList && obj_id=" + memberNode.id + "]");
            if (memberItem != NULL && memberItem.value != "")
                content2 += html_print_description("Constructor init values: ", memberItem.value, 1);
        }

        memberItem = find_by_query("item[UmlStereotype && obj_id=" + memberNode.id + "]");
        if (memberItem != NULL && class_strip_info(memberItem.value) != "")
        {
			stereot = find_by_query("node[type=UmlStereotype && node_refs && name='${memberItem.value}']");
			if (stereot!=NULL)
			{
				if (idlist_link("UmlStereotype",stereot.id))
					content2 +=	html_style("B", "Stereotype:") + " " +
								html_link(	"../stereotypes/" + memberItem.value + "_" + stereot.id +
											".html", memberItem.value, "") + 
								"<BR>\n";
				else
					content2 +=	html_style("B", "Stereotype:") + " " + memberItem.value + "<BR>\n";
			}
			else
			{
				if (idlist_link("UmlStereotype",idlist_predef_stereot_id(memberItem.value,memberNode.type)))
					content2 +=	html_style("B", "Stereotype:") + " " +
								html_link(	"../stereotypes/" + memberItem.value + "_pre" +
											".html", memberItem.value, "") + 
								"<BR>\n";
				else
					content2 +=	html_style("B", "Stereotype:") + " " + memberItem.value + "<BR>\n";
			}
		}

        content2 += html_print_obj_description("UmlOperationDefinition", memberNode.id);

		content2 += html_ref_statemac( memberNode );
		
		content += html_indent(content2,1);
    }

    if (memTitle == "Constructors")   // Constructors only
        content += html_seperator("") + "<BR>\n";
	else
        content += "<BR>\n";

    return(content);
}

string class_gen_op_lang_info(node memberNode)
{
item	memberItem;
string	content="";
string	part_content="";
string	PrLang=to_upper(e_Programming_Language);
    
    if (PrLang == "C++")
    {
        content += html_style("B","C++:") + "<BR>\n";
    
        memberItem = find_by_query("item[UmlMemberCxxVisibility && obj_id=" + memberNode.id + "]");
        if (memberItem != NULL && memberItem.value != "")
            part_content += html_escape_chars(to_lower(memberItem.value)) + " ";
        else 
            part_content += class_add_val_to_string(memberNode.id, "UmlMemberVisibility",  "", " ", True, False);

        part_content += class_add_const_to_string("inline ", memberNode.id, "UmlOperationCxxIsInline");
        part_content += class_add_const_to_string("static ", memberNode.id, "UmlMemberIsClass");
        part_content += class_add_const_to_string("virtual ", memberNode.id, "UmlOperationCxxIsVirtual");
        part_content += class_add_val_to_string(memberNode.id, "UmlOperationReturnType",  "", " ", False, True);
        part_content += html_escape_chars(memberNode.name) + "(";
        part_content += class_add_arg_to_string(memberNode.sig) + ")";
        part_content += class_add_const_to_string("const ", memberNode.id, "UmlMemberCxxIsConst ");
        part_content += class_add_const_to_string("= 0", memberNode.id, "UmlOperationIsAbstract");

        content += html_indent(part_content, 1);

        part_content = class_add_val_to_string(memberNode.id, "UmlOperationThrows", "throw (", ") ", False, True);
        if (part_content != "") content += html_indent(part_content, 2);
        return(content);
    }

    if (PrLang == "JAVA")
    {
        content += html_style("B","Java:") + "<BR>\n";

        memberItem = find_by_query("item[UmlMemberJavaVisibility && obj_id=" + memberNode.id + "]");
        if (memberItem != NULL && memberItem.value != "")
            part_content += html_escape_chars(to_lower(memberItem.value)) + " ";
        else 
            part_content += class_add_val_to_string(memberNode.id, "UmlMemberVisibility",  "", " ", True, False);

        part_content += class_add_const_to_string("static ", memberNode.id, "UmlMemberIsClass");
        part_content += class_add_const_to_string("abstract ", memberNode.id, "UmlOperationIsAbstract");
        part_content += class_add_const_to_string("final ", memberNode.id, "UmlMemberJavaIsFinal");
        part_content += class_add_const_to_string("native ", memberNode.id, "UmlOperationJavaIsNative");
        part_content += class_add_const_to_string("synchronized ", memberNode.id, "UmlOperationJavaIsSynchronized");
        part_content += class_add_val_to_string(memberNode.id, "UmlOperationReturnType",  "", " ", False, True);
        part_content += html_escape_chars(memberNode.name) + "(";
        part_content += class_add_arg_to_string(memberNode.sig) + ")";

        content += html_indent(part_content, 1);

        part_content = class_add_val_to_string(memberNode.id, "UmlOperationThrows", "throws ", "", False, True);
        if (part_content != "") content += html_indent(part_content, 2);
        return(content);
    }

    if (PrLang == "IDL")
    {
        content += html_style("B","IDL:") + "<BR>\n";

        part_content += class_add_val_to_string(memberNode.id, "UmlOperationIDLAttribute",  "", " ", False, False);

        memberItem = find_by_query("item[UmlOperationIDLReturnType && obj_id=" + memberNode.id + "]");
        if (memberItem != NULL && memberItem.value != "")
            part_content += html_escape_chars(memberItem.value) + " ";
        else 
            part_content += class_add_val_to_string(memberNode.id, "UmlOperationReturnType",  "", " ", False, True);

        part_content += html_escape_chars(memberNode.name + "(");

        memberItem = find_by_query("item[UmlOperationIDLSignature && obj_id=" + memberNode.id + "]");
        if   (memberItem != NULL && memberItem.value != "")
            part_content += class_add_arg_to_string(memberItem.value)+ ") ";
        else 
            part_content += class_add_arg_to_string(memberNode.sig)+ ") ";

        content += html_indent(part_content, 1);
    
        part_content = class_add_val_to_string(memberNode.id, "UmlOperationThrows", "raises (", ")", False, True);
        part_content += class_add_val_to_string(memberNode.id, "UmlOperationIDLContext", "context (", ")", False, False);
        if (part_content != "")
            content += html_indent(part_content, 2);

        return(content);
    }
    return("");
}

string  class_strip_info(string the_string)
{
int		scnt, sx;
string	the_char;

   the_string = string_strip(the_string, "B", " ");
   the_string = string_strip(the_string, "B", "\n");
   the_string = string_strip(the_string, "B", " ");
   scnt       = string_length(the_string);

   for (sx = 0; sx < scnt; sx = sx + 1)
   {
       the_char = string_extract(the_string, sx, 1);
       if ((the_char != " ") && (the_char != "\n") && (the_char != "\t"))
          break;
   }

   if   (sx < scnt)
        the_char = the_string;
   else the_char = "";

   return(the_char);
}

string class_add_val_to_string(int memberId, string itemType,
                      string prefix, string postfix, boolean toLower, boolean scan4links)
{
item	memberItem;
string	valString;

    memberItem = find_by_query("item[" + itemType + " && obj_id=" + memberId + "]");

	if (memberItem != NULL && memberItem.value != "")
	{
		valString = memberItem.value;
		if (toLower)
		{
			valString = html_escape_chars(prefix + to_lower(valString) + postfix);
		}
		else
		{
			if (scan4links)
			{
				valString =	html_escape_chars(prefix) +
							class_scan_for_links(valString) +
							html_escape_chars(postfix);
			}
			else
			{
				valString = html_escape_chars(prefix + valString + postfix);
			}
		}

		return(valString);
	}

	return("");
}

string class_add_const_to_string(string myConst, int memberId, string itemType) 
{
item	memberItem;

	memberItem = find_by_query("item[" + itemType + " && obj_id=" + memberId + "]");

    if (memberItem != NULL && memberItem.value == "True")
		return(html_escape_chars(myConst));

       return("");
}

string class_add_arg_to_string(string para_str)
{
	if (!e_Link_in_Paralist)
		return(html_escape_chars(para_str));

string			content="";
list			para_list=NULL;
string			para_one;
uml_argument	arg_struct;
int				i;
boolean			first=True;

	para_list = list_create("string", 0);
	if(uml_split_arguments(para_str, para_list))
	{
		for (i=0;i<list_count(para_list);i++)
		{
			if (!first)
				content += ", ";
			else
				first=False;

			para_one = list_get(para_list,i);
			arg_struct = uml_arg_string_to_struct(para_one);

			if (arg_struct.argKind!="") content += html_escape_chars(arg_struct.argKind + " ");
			content += html_escape_chars(arg_struct.argName + ":");
			content += class_scan_for_links(arg_struct.argType);		// Scanner !!!
			if (arg_struct.argVal!="") content += html_escape_chars("=" + arg_struct.argVal);
		}
	}
	else
	{
		content += html_escape_chars(para_str);
	}

	list_clear(para_list);
	
	return(content);
}

string class_scan_for_links(string input)
{
string		output="";
string		in_char="";
string		in_str="";
int			i;
node		node_class;
boolean		flag_quot1=False;
boolean		flag_quot2=False;

// Scan chars init
set			scan_chars = set_create("string");
	set_add(scan_chars,"<");
	set_add(scan_chars,">");
	set_add(scan_chars,"[");
	set_add(scan_chars,"]");
	set_add(scan_chars,"(");
	set_add(scan_chars,")");
	set_add(scan_chars,"*");
	set_add(scan_chars,"&");
	set_add(scan_chars,",");
	set_add(scan_chars,";");
	set_add(scan_chars,":");
	set_add(scan_chars," ");
	set_add(scan_chars,"=");
	set_add(scan_chars,"'");
	set_add(scan_chars,"\"");
	set_add(scan_chars,"");

	for (i=0;i<string_length(input)+1;i++)
	{
		if (i<string_length(input))
			in_char=string_extract(input,i,1);
		else
			in_char="";

		if (in_char=="\\")
		{															// don't read backslash and next char
			output+=html_escape_chars(in_str);
			output+=html_escape_chars(in_char);
			i++;
			if (i<string_length(input))
				in_char=string_extract(input,i,1);
			else
				in_char="";
			output+=html_escape_chars(in_char);
			in_str="";
		}
		else
		{
			if (set_is_member(scan_chars,in_char))
			{
				if (in_char == "'")
				{
					if (!flag_quot2)
						flag_quot1=!(flag_quot1);
					output+=html_escape_chars(in_str);
					in_str = "";
				}

				if (in_char == "\"")
				{
					if (!flag_quot1)
						flag_quot2=!(flag_quot2);
					output+=html_escape_chars(in_str);
					in_str = "";
				}
				
				if ((in_char != "'")&&(in_char != "\"")&&(in_str!=""))
					if ((!flag_quot1)&&(!flag_quot2))
					{
						node_class=find_by_query("node[UmlClass & node_refs & name='${in_str}']");
						if (node_class!=NULL)
						{													// found link !!!
							if (idlist_link("UmlClass", node_class.id))
							{
								output += html_link( node_class.name + "_" + node_class.id + ".html", in_str, "" );
								in_str="";
							}
							else
							{
								output += html_style("B", in_str);
								in_str="";
							}
						}
						else
						{													// no link !!!
							output += html_escape_chars(in_str);
							in_str="";
						}
					}
					else
					{														// no link in quotations !!!
						output += html_escape_chars(in_str);
						in_str="";
					}
				
				output+=html_escape_chars(in_char);
			}
			else
			{
				in_str+=in_char;
			}
		}
	}

	set_clear(scan_chars);

//	message("Scanner:");
//	message(input);
//	message(output);

	return(output);
}
