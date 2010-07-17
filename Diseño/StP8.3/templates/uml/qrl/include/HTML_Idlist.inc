////////////////////////////////////////////////////////////////////////////////
//
// HTML_IDList
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// DEFINITIONS:
///////////////////////////////////////////////////////////////////////////////

// IDLists
list idlist_actor = NULL;
list idlist_usecase = NULL;
list idlist_package = NULL;
list idlist_class = NULL;
list idlist_stereotype = NULL;
list idlist_diagram = NULL;

///////////////////////////////////////////////////////////////////////////////
//  SOURCE:
///////////////////////////////////////////////////////////////////////////////

void idlist_create()
{
list		node_list;
node		node_var;
list		node_list_1;
node		node_find;
list		node_list_2;
list		item_list;
item		item_var;
file		file_var;
int			i,j,temp;
boolean		found;

// Actor IDList
	if (e_gen_Actor_Pages)
	{
		idlist_actor = list_create( "int", 0 );
		node_list = list_select(	"node[UmlActor && node_refs" + 
									idlist_filter(e_limit_to_Actors_named, "name")  + 
									"] sort by name"	);
		for (i=0;i<list_count(node_list);i++)
		{
			node_var = list_get( node_list, i );
			list_append( idlist_actor, node_var.id );
		}
		list_clear( node_list);
	}

// UseCase IDList
	if (e_gen_Use_Case_Pages)
	{
		idlist_usecase = list_create( "int", 0 );
		node_list = list_select(	"node[UmlUseCase && node_refs" + 
									idlist_filter(e_limit_to_Use_Cases_named, "name")  + 
									"] sort by name"	);
		for (i=0;i<list_count(node_list);i++)
		{
			node_var = list_get( node_list, i );
			list_append( idlist_usecase, node_var.id );
		}
		list_clear( node_list);
	}

// Package IDList
	if (e_gen_Package_Pages)
	{
		idlist_package = list_create( "int", 0 );
		node_list = list_select(	"node[UmlPackage && node_refs" + 
									idlist_filter(e_limit_to_Packages_named, "name")  + 
									"] sort by name"	);
		for (i=0;i<list_count(node_list);i++)
		{
			node_var = list_get( node_list, i );
			list_append( idlist_package, node_var.id );
		}
		list_clear( node_list);
	}

// Class IDList
	if (e_gen_Class_Pages)
	{
		idlist_class = list_create( "int", 0 );
		node_list = list_select(	"node[type=UmlClass && node_refs" + 
									idlist_filter(e_limit_to_Classes_named, "name")  + 
									"] sort by name"	);
		node_list_1 = list_create( "node", 0 );
		node_list_2 = list_create( "node", 0 );

		for (i=0;i<list_count(node_list);i++)
		{
			node_var = list_get( node_list, i );
			
			if (selection_count("node[node_refs & type=UmlParameterizedClass & name='${node_var.name}']")>0)
			{
				node_find=find_by_query("node[node_refs & type=UmlParameterizedClass & name='${node_var.name}']");
				list_append( node_list_1, node_var );
			}
			else
			if (selection_count("node[node_refs & type=UmlInstantiatedClass & name='${node_var.name}']")>0)
			{
				node_find=find_by_query("node[node_refs & type=UmlInstantiatedClass & name='${node_var.name}']");
				list_append( node_list_2, node_var );
			}
			else
			{
				list_append( idlist_class, node_var.id );
			}
		}
		list_clear( node_list );

		for (i=0;i<list_count(node_list_1);i++)
		{
			node_var = list_get( node_list_1, i );
			list_append( idlist_class, node_var.id );
		}
		list_clear( node_list_1 );

		for (i=0;i<list_count(node_list_2);i++)
		{
			node_var = list_get( node_list_2, i );
			list_append( idlist_class, node_var.id );
		}
		list_clear( node_list_2 );
	}

// Stereotype IDList
	init_stereot_def();

	if (e_gen_Stereotype_Pages)
	{
		idlist_stereotype = list_create( "int", 0 );
		node_list = list_select(	"node[UmlStereotype && node_refs" + 
									idlist_filter(e_limit_to_Stereotypes_named, "name") +
									"] sort by name"	);
		for (i=0;i<list_count(node_list);i++)
		{
			node_var = list_get( node_list, i );
			list_append( idlist_stereotype, node_var.id );
		}

		item_list = list_select(	"item[node[node_refs] & UmlStereotype" +
									idlist_filter(e_limit_to_Stereotypes_named, "value") +
									"] sort by value" );
		for (i=0;i<list_count(item_list);i++)
		{
			item_var = list_get( item_list, i );
			found = False;
			for (j=0;j<list_count(node_list);j++)
			{
				node_var = list_get( node_list, j );
				if (item_var.value==node_var.name)
				{
					found = True;
					break;
				}
			}
			if (!found)
			{
				node_var = find_by_query("node[id = ${item_var.obj_id}]");
				if (node_var!=NULL)
				{
					temp=idlist_predef_stereot_id(item_var.value,node_var.type);
					if (temp!=0)
						if ( list_find(idlist_stereotype, 0, temp)==list_count(idlist_stereotype) )
							list_append( idlist_stereotype, temp );
				}
			}
		}
		list_clear( node_list );
		list_clear( item_list );
	}

// Diagram IDList
	if (e_gen_Diagram_Pages)
	{
		idlist_diagram = list_create( "int", 0 );
		node_list = list_select(	"file[(" + 
									"UmlUseCaseDiagram|" + 
									"UmlClassDiagram|" + 
									"UmlSequenceDiagram|" + 
									"UmlCollaborationDiagram|" + 
									"UmlStateDiagram|" + 
									"UmlActivityDiagram|" + 
									"UmlComponentDiagram|" + 
									"UmlDeploymentDiagram|" + 
									"UmlStereotypeDiagram)" + 
									idlist_filter(e_limit_to_Diagrams_named, "name")  + "] sort by type,name" );
		for (i=0;i<list_count(node_list);i++)
		{
			file_var = list_get( node_list, i );
			list_append( idlist_diagram, file_var.id );
		}
		list_clear( node_list );
	}
    return;
}

void idlist_free()
{
	if (idlist_actor!=NULL)
		list_clear( idlist_actor );
	if (idlist_usecase!=NULL)
		list_clear( idlist_usecase );
	if (idlist_package!=NULL)
		list_clear( idlist_package );
	if (idlist_class!=NULL)
		list_clear( idlist_class );
	if (idlist_stereotype!=NULL)
		list_clear( idlist_stereotype );
	if (idlist_diagram!=NULL)
		list_clear( idlist_diagram );
    return;
}

boolean idlist_link( string link_to_type, int id )
{
int		id_copy;
int		posi;

// UmlActor
	if (link_to_type=="UmlActor")
	{
		if (!e_gen_Actor_Pages)
			return( False );
		posi=list_find( idlist_actor, 0, id );
		if (posi==list_count(idlist_actor))
			return( False );
		else
			return( True );
	}

// UmlUseCase
	if (link_to_type=="UmlUseCase")
	{
		if (!e_gen_Use_Case_Pages)
			return( False );
		posi=list_find( idlist_usecase, 0, id );
		if (posi==list_count(idlist_usecase))
			return( False );
		else
			return( True );
	}

// UmlPackage
	if (link_to_type=="UmlPackage")
	{
		if (!e_gen_Package_Pages)
			return( False );
		posi=list_find( idlist_package, 0, id );
		if (posi==list_count(idlist_package))
			return( False );
		else
			return( True );
	}

// UmlClass
	if (link_to_type=="UmlClass")
	{
		if (!e_gen_Class_Pages)
			return( False );
		posi=list_find( idlist_class, 0, id );
		if (posi==list_count(idlist_class))
			return( False );
		else
			return( True );
	}

// UmlStereotype
	if (link_to_type=="UmlStereotype")
	{
		if (!e_gen_Stereotype_Pages)
			return( False );
		posi=list_find( idlist_stereotype, 0, id );
		if (posi==list_count(idlist_stereotype))
			return( False );
		else
			return( True );
	}

// UmlDiagram
	if (	(link_to_type=="UmlDiagram")||
			(link_to_type=="UmlUseCaseDiagram")||
			(link_to_type=="UmlClassDiagram")||
			(link_to_type=="UmlSequenceDiagram")||
			(link_to_type=="UmlCollaborationDiagram")||
			(link_to_type=="UmlStateDiagram")||
			(link_to_type=="UmlActivityDiagram")||
			(link_to_type=="UmlComponentDiagram")||
			(link_to_type=="UmlDeploymentDiagram")||
			(link_to_type=="UmlStereotypeDiagram")	)
	{
		if (!e_gen_Diagram_Pages)
			return( False );
		posi=list_find( idlist_diagram, 0, id );
		if (posi==list_count(idlist_diagram))
			return( False );
		else
			return( True );
	}
	
	return( False );
}

void idlist_get_hierarchie(string query, int id, boolean add_to_list, list hiera_list)
{
list	depth_list = list_select(query);
int		idx;
node	node_ref;

	if (add_to_list == False)
		hiera_list = list_create("int", 0);
	for(idx = 0; idx < list_count(depth_list); idx++)
    {
        node_ref = list_get(depth_list, idx);
        if(list_find(hiera_list, 0, node_ref.id) == list_count(hiera_list))
		{
			if(add_to_list == True)
				list_append(hiera_list, node_ref.id);
			idlist_get_hierarchie(query, node_ref.id, True, hiera_list);
		}
    }
    return;
}

string idlist_filter(string reduction, string elem)
{
int		ocnt, ox;
list	red_to_objects;

string	prefix;
string	request  = "";
string	operator = "";
string	one_obj  = "";
string	op_link  = "";

    if (reduction == "*")
        return("");				// nothing to do
    
    red_to_objects = string_to_list(reduction, " ");
    ocnt           = list_count(red_to_objects);
    
    if (ocnt==0)
        return("");				// nothing to do

    for (ox = 0; ox < ocnt; ox ++ ) 
    {
        one_obj = list_get(red_to_objects, ox);

        if   (string_find(one_obj, 0, "*") < string_length(one_obj))
             operator = " $ ";
        else operator = " = ";

        request = request + op_link + elem + operator +  "'" + one_obj + "'";
        op_link = " || "; 
    } 

   return(" && (" + request + ")");
}

int idlist_predef_stereot_id( string stereot_name, string stereot_type )
{
int				i;
stereot_def		stereot_var;

	stereot_type=string_strip(stereot_type,"L","Uml");
	for (i=0;i<list_count(builtin_stereotypes);i++)
	{
		stereot_var=list_get(builtin_stereotypes,i);
		if ((stereot_var.name==stereot_name)&&(stereot_var.stp_symbol==stereot_type))
			return(-1-i);
	}
	return(0);
}

string idlist_get_nodelink( int node_id )
{
node			node_var;
stereot_def		stereot_var;

	if (node_id>=0)
	{
		node_var=find_by_query("node[id=${node_id}]");
		return( node_var.name + "_" + node_var.id );
	}
	else
	{
		stereot_var=list_get(builtin_stereotypes,(-1-node_id));
		return( stereot_var.name + "_pre" );
	}
}

string idlist_get_nodename( int node_id )
{
node	node_var;
node	node_class;
item	item_var;
string	content="";
string	str_sig="";

	node_var=find_by_query("node[node_refs & id=${node_id}]");
	if (node_var.type=="UmlClass")
	{
		item_var=find_by_query("item[UmlClassParameters && obj_id=" + node_var.id + "]");
		if (item_var!=NULL) str_sig="<" + item_var.value + ">";
		else
			if ( node_var.sig!="" ) str_sig="<" + node_var.sig + ">";

		content = node_var.name + str_sig;
	}
	else
	if (node_var.type=="UmlAttribute")
	{
		node_class=find_by_query("node[UmlClass & node_refs & out_links[link_refs & to_node[id=${node_var.id}]]]");
		item_var=find_by_query("item[UmlClassParameters & obj_id=" + node_class.id + "]");
		if (item_var!=NULL) str_sig="<" + item_var.value + ">";

		content = node_class.name + str_sig + "::" + node_var.name;
	}
	else
	if (node_var.type=="UmlOperation")
	{
		node_class=find_by_query("node[UmlClass & node_refs & out_links[link_refs & to_node[id=${node_var.id}]]]");
		item_var=find_by_query("item[UmlClassParameters & obj_id=" + node_class.id + "]");
		if (item_var!=NULL) str_sig="<" + item_var.value + ">";

		content = node_class.name + str_sig + "::" + node_var.name + "(" + node_var.sig + ")";
	}
	else
	{
		content = node_var.name;
	}

	return(content);
}
