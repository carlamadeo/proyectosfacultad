#include "rules/qrl/requirements_nav.inc"
#include "rules/qrl/doors_nav.inc"
#include "rules/qrl/ct.inc"
#include "rules/qrl/message_label.inc"
#include "rules/qrl/ustated_transition.inc"
#include "rules/qrl/oper_parse.inc"
#include "rules/qrl/state_machine.inc"

// Bug 14312. (Carlos Lin). Added external var for user to toggle
// on/off namespace generation

// --- START PATCH ECR 5025 ---
// This was defined as an external variable. But this results
// in problems when run in an editor. The fix is: Defined this locally,
// define a new var in the codegen, which is external, and then do an
// assignment
//
boolean generate_namespace = True;
// --- END PATCH ECR 5025 ---external boolean generate_namespace = True;

// End Bug 14312.

// ECR 7318 moved UML_PATH_SEPARATOR to uml/rules/qrl/uml_optional_dependencies.inc
// const string UML_PATH_SEPARATOR = "::";

const string UML_BAD_CH = "!@#$%^&*()|\\{}[]/?~`:'\";";
const string UML_GOOD_CH = string_repeat("_", string_length(UML_BAD_CH));
const string UML_BAD_CH_NO_COL = "!@#$%^&*()|\\{}[]/?~`'\";";
const string UML_GOOD_CH_NO_COL = string_repeat("_", string_length(UML_BAD_CH_NO_COL));
// --- START ECR 6635
const string UML_BAD_CH_NO_COL_WITH_BRACES = "!@#$%^&*()|\\{}/?~`'\";";
const string UML_GOOD_CH_NO_COL_WITH_BRACES = string_repeat("_", string_length(UML_BAD_CH_NO_COL_WITH_BRACES));
// --- END ECR 6635
// I have removed a period (.) from this list so that RE can create unique
// suffixes on identically named classes. It should be put back in when
// scoping is correctly implemented
const string UML_CLASS_BAD_CH = "`~!@#$%^&*()-=+[{]}\\|;:'\",<>/?";
const string UML_CLASS_GOOD_CH = string_repeat("_", string_length(UML_CLASS_BAD_CH));

void 
uml_node_test_and_fix_label(gde_node gn, string label, string fixed_label)
{
    
    if (label != fixed_label)
    {
        print_error("Name '" + label + "' has invalid characters. Fixing name ...");
        if(!gde_rename_in_progress()) 
            gde_node_set_label(gn, fixed_label);
    }
    
}


// global mapping function for UML names

string
uml_gde_node_with_spaces_name_func(gde_mapsymbol obj)
{
    gde_node gn = to_gde_node(obj);
    string result = gde_node_label(gn);
    string newres;
 
    if (result == "")
        return NULL;
 
    newres = string_translate(result, UML_BAD_CH, UML_GOOD_CH);
    
    uml_node_test_and_fix_label(gn, result, newres);
    return list_to_string(string_to_list(newres, " \n\t"), " ");
}

// global mapping function for UML names

string
uml_gde_node_name_func(gde_mapsymbol obj)
{
    gde_node gn = to_gde_node(obj);
    string result = gde_node_label(gn);
    string newres;
 
    if (result == "")
        return NULL;
 
    newres = string_translate(result, UML_BAD_CH, UML_GOOD_CH);
    uml_node_test_and_fix_label(gn, result, newres);
 
    // ECR 2478 - begin (rolled back)
    newres = strip_whitespace(newres);
    // newres = replace_whitespace(newres, "_");
    // ECR 2478 - end
    return newres;
}

// given a string in the form a::b::c, return the rightmost name.
string
uml_base_name(string name)
{
	string newres;
	int sep_loc = -1;
	int last_loc;
	int look_loc = 0;
	int len;

	newres = name;
	len = string_length(newres);
	last_loc = 0;
	do {
		last_loc = sep_loc;
		if(last_loc>=0)
			look_loc = last_loc+1;
		sep_loc = string_find(newres,look_loc,UML_PATH_SEPARATOR);
	} while(sep_loc != len) ;

	if (last_loc>=0)
		newres = string_extract(newres,last_loc+2,len-last_loc-2);

    newres = string_translate(newres, UML_CLASS_BAD_CH, UML_CLASS_GOOD_CH);
	return newres;
}


// ECR 6676 begin (rolled back)
//
// Function strips any whitespace characters except newline ('\n')
//
//string strip_whitespace_except_nl(string s)
//{
//    if (s == NULL)
//        return NULL;
//
//    return (list_to_string(string_to_list(s, " \r"), ""));
//}
// ECR 6676 end


// This function chops off everything but the base name
// ex: a::b::c becomes c
string
uml_gde_node_base_name_func(gde_mapsymbol obj)
{
    gde_node gn = to_gde_node(obj);
    string result = gde_node_label(gn);
    int sep_loc;
    int len;
    string newres;
    
    if (result == "")
        return NULL;
    
    newres = uml_base_name(result);

// ECR 6676 begin (rolled back)
//    newres = strip_whitespace_except_nl(newres);
// ECR 6676 end

    uml_node_test_and_fix_label(gn, result, newres);
    newres = strip_whitespace(newres);

    return newres;
}



//decomposes 
list uml_path_to_list(string path)
{
    return string_to_list(path,":");
}


int UML_CHILD_ID = 0;
const string UML_SCOPE_QUERY = "node[out_links[UmlContains && " +
	" link_refs && to_node_id == ${UML_CHILD_ID}]]";

// returns a list of all packages containing this node
list
uml_get_containing_packages(node class)
{
	UML_CHILD_ID = class.id;
	list containers = list_select(UML_SCOPE_QUERY);
	return containers;
}


// returns a list of node names divided by commas
string nodes_names(list nodes)
{
	int index = 0;
	string result = "";
	node cur_node;

	for (index = 0; index < list_count(nodes); index++)
	{
		cur_node = list_get(nodes, index);
		result += cur_node.name;

		if (index < list_count(nodes)-1)
			result += ", ";
	}

	return result;
}

// returns a string representing the fully qualified class name of the
// class node. The format returned is:
//	base1::base2::...::classname
// NOTE - if there is more than one containing class or package, the
// results may differ from execution to execution
// NOTE: If a cycle is detected, an errormessage will be printed and only
// the class name will be returned.
string
uml_class_qualified_name(node class)
{
	if (NULL == class) {
		return "";
	}

	list visited = list_create("int",0);

	list_append(visited,class.id);
	string cont_name=class.name;	

	list packages;
	node container = class;
	packages = uml_get_containing_packages(class);
	while (NULL != packages && list_count(packages) > 0) {
		if (list_count(packages)>1) {
			print_error("Class '" + class.name + "' is contained by more than one package.\n"+
			"Packages: " + nodes_names(packages) + ".");
			cont_name = class.name;
			break;
		}
		container = list_get(packages,0);

		// Bug 14312. (Carlos Lin). If user specifies no namespaces
		// and container is a package then delete it from container
		// list; skip the check for circularity and do not add 
		// container to qualified name.
		if ( !generate_namespace && container.type == "UmlPackage" ) 
		{
		   list_delete(packages,0);
		}
		else
		{
		    if (list_find(visited,0,container.id) != list_count(visited)) 
		    {
			// circularity detected
			print_error("Circular containment found for class '" +
					class.name + "'. Cannot determine fully qualified name.");
			cont_name = class.name;
			break;
		    } else {
			list_append(visited,container.id);
		    }
		    cont_name = container.name + UML_PATH_SEPARATOR + cont_name;
		}
		packages = uml_get_containing_packages(container);
	}
	return cont_name;
}


int
idehook_uml_reverse_engineering_active_func()
{
    if ( is_smdb_available() && idehook_class_selected_active_func() )
      return ACTIVE_FUNC_IS_ACTIVE;
    return ACTIVE_FUNC_IS_INACTIVE;
}


list
uml_selected_class_symbols()
{
    list classes, symbols = gde_selected_symbols();
    gde_node class;
    int i;

    classes = list_create("gde_node", 0);
    for (i = 0; i < list_count(symbols); i = i + 1)
      {
	class = uml_symbol_to_class(list_get(symbols, i));
	if (class != NULL)
	  list_append(classes, class);
      }
    return classes;
}


list
uml_stereotype_nav_targets(string stereotype)
{
	node stype_node = find_by_query("node[UmlStereotype && name='" + stereotype + "']");
	list targets;
	if(NULL != stype_node) {
		targets = list_select("node_ref[node_id == " + stype_node.id + "]");
	} else {
		targets = list_create("all",0);
	}
	return targets;
}

void 
uml_diag_check_semantics(string diag_semantic_check_function)
{
    int err_count;
    
    err_count = qrl_eval(diag_semantic_check_function + "();");
    if(err_count == 0)
	print_message("Semantic check succeeded.");
    else
	if(err_count > 1)
	    print_error(err_count + " semantic errors;  semantic check failed.");
	else
	    print_error(err_count + " semantic error;  semantic check failed.");
}


string uml_label_mark_func_get_default(list matched_items,string defaultVal)
{
    string result = defaultVal;
    item anItem;
    int howMany = list_count(matched_items);
    if(howMany==1) {
	anItem = list_get(matched_items, 0);
	result = anItem.value;
    }
    result = "«" + result + "»";
    //message("uml_label_mark_func_get_default::count="+howMany+"result='"+result+"'");
    return(result);
}

void
UmlPopUpPropSheet(string editor)

{
    // ECR 6880 Property sheets from table editors
    if(editor == "uclasst") {
		list objects = gte_cell_mapped_oms_nodes(1, 1, gte_get_current_cell_row(), gte_get_current_cell_col());
        if(gte_get_hsect(gte_get_current_cell_row()) == gte_lookup_symbol("ClassHsect")
            && gte_get_vsect(gte_get_current_cell_col()) == gte_lookup_symbol("NameVsect")
            && list_count(objects) != 0) {
			// unfortunately gte_cell_mapped_oms_nodes() returns only UmlClass ...
			node class = list_get( objects, 0 );
			class = find_by_query( "node[UmlParameterizedClass && name == '${class.name}']" );
    		if( class != NULL ) {
    			UmlParamClass_ps_show();
    		} else {
	        	UmlClass_ps_show();
	        }
        } else if(gte_get_hsect(gte_get_current_cell_row()) == gte_lookup_symbol("AttributeHsect")
            && gte_get_vsect(gte_get_current_cell_col()) == gte_lookup_symbol("AttributeNameCol")
            && list_count(objects) != 0)
  	    UmlAttribute_ps_show();
        else if(gte_get_hsect(gte_get_current_cell_row()) == gte_lookup_symbol("OperationHsect")
            && gte_get_vsect(gte_get_current_cell_col()) == gte_lookup_symbol("OperationNameCol")
            && list_count(objects) != 0)
            UmlOperation_ps_show();
        return;
    }
    // ECR 6880 end

    gde_symbol sym;
    string s;

    list syms = gde_selected_symbols();

    if (NULL == syms || 0 == list_count(syms))
    {
        return;
    }
    sym = list_get(syms, 0);

    // the symbol label may be modified by editing 
    // the symbol, after the property sheet is closed.  
    // the property sheet can have stale label.
    // force a refresh.
    ps_reset_last_fill();

    if (editor == "uclassd")
    { 
        if (gde_symbol_is_node(sym))
        {
            s = gde_nt_name(gde_node_nt(to_gde_node(gde_selected_mapsymbol())));
            if (s == "Package")
                UmlPackage_ps_show();
            else if (s == "ParameterizedClass")
                UmlParamClass_ps_show();
            // --- START PATCH ECR 6726 ---
            else if (s == "Object")
                UmlObjectInstance_ps_show();
            else if (s == "Attribute")
                UmlAttribute_ps_show();
            else if (s == "Operation")
                UmlOperation_ps_show();
            // --- END PATCH ECR 6726 ---
            else
                UmlClass_ps_show();
        }
        else 
        {
            gde_link gl;

// ECR 2653 begin
            if (uml_is_shared_generalization(syms))
            {
                // Shared Generalization
                UmlClassLinkGenHierImpl_ps_show();
                return;
            }
// ECR 2653 end
            gl = to_gde_link(gde_selected_mapsymbol());
            s = gde_lt_name(gde_link_lt(gl));

            if (s == "Association")
                UmlClassLinkAssociation_ps_show(1); // ECR 5011
            // ECR 3856 - begin
            else if (s == "Aggregation")
                UmlClassLinkAssociation_ps_show(0); // ECR 5011
            // ECR 3856 - end
            // ECR 7480 start
            else if (s == "NavigableAssociation")
                UmlClassLinkAssociation_ps_show(0);
            // ECR 7480 end
            else if (s == "Dependency")
                UmlClassLinkDependency_ps_show();
            else if (s == "Refines")
                UmlClassLinkRefines_ps_show();
            else 
                UmlClassLinkGenImpl_ps_show();
        }
    }
    else if (editor == "uusecased")
    { 
        if (gde_symbol_is_node(sym))
            UmlUsecase_ps_show();
        else 
            UmlUsecaseLink_ps_show();
    }
    else if (editor == "ucollaborationd")
    { 
        if (gde_symbol_is_node(sym))
            UmlCollaboration_ps_show();
        else if (gde_symbol_is_arc(sym))
            UmlCollaborationLink_ps_show();
        else
            UmlCollaborationCntx_ps_show();
    }
    else if (editor == "usequenced")
    { 
        if (gde_symbol_is_node(sym))
            UmlSequence_ps_show();
        else 
            UmlSequenceLink_ps_show();
    }
    else if (editor == "ustated")
    { 
        if (gde_symbol_is_node(sym))
            UmlState_ps_show();
        else 
            UmlStateLink_ps_show();
    }
    else if (editor == "uactivityd")
    { 
        if (gde_symbol_is_node(sym))
            UmlActivity_ps_show();
        else 
            UmlActivityLink_ps_show();
    }
    else if (editor == "ucomponentd")
    { 
        if (gde_symbol_is_node(sym))
        {
            // ECR 5184 start
            // UmlComponent_ps_show();
            s = gde_nt_name(gde_node_nt(to_gde_node(gde_selected_mapsymbol())));
            if (s == "Class")
                UmlClass_ps_show();
            else
                UmlComponent_ps_show();
            // ECR 5184 end
        }
        else 
            UmlComponentLink_ps_show();
    }
    else if (editor == "udeploymentd")
    { 
        if (gde_symbol_is_node(sym))
        {
	    // ECR 5184 start
	    // UmlDeployment_ps_show();
	    s = gde_nt_name(gde_node_nt(to_gde_node(gde_selected_mapsymbol())));
        if (s == "Class")
            UmlClass_ps_show();
        else
		    UmlDeployment_ps_show();
	    // ECR 5184 end
	}
        else 
            UmlDeploymentLink_ps_show();
    }
    else if (editor == "ustereotyped")
    { 
        UmlStereotype_ps_show();
    }
    return;
}

// returns a list of strings corresponding to the names of messages sent to 
// the named class. pattern is used to cull this list because this function is
// called from name completion, among other places
list
uml_messages_for_class(string pattern, node class)
{
	// The message types were cloned from the usequenced code. If you change
	// this, make sure it's in sync with the thing from which it was copied.
	string qry = "item[UmlObjectName && " +
		"link[(UmlSimpleMessage|UmlSynchronousMessage|UmlBalkingMessage|" +
		"UmlTimeoutMessage|UmlAsynchronousMessage) && name $ '${pattern}'" +
		" && to_node[scope_node[name='${class.name}']] && link_refs]" +
		"]";

	list l = list_create("string",0);

	item msgName;

	for_each_in_select(qry,msgName) 
            // duplicate items can occur for two reasons :
            // 1. the message for a class may be modeled in 
            //    multiple sequence diagrams
            // 2. the message for a class may be modeled in
            //    an un-named diagram and named later. the
            //    current mapping generates diff notes and
            //    items accordingly. (12793)
            if (list_find(l, 0, msgName.value) == list_count(l))
		list_append(l,msgName.value);

	return l;
}

// Begin add ECR 3785
list
FindAnyNodeSymbolWithTheSameName( string node_name, int file_id )
{
    if( node_name == NULL || node_name == "" )
    {
        print_error( "Unlabeled symbol selected." );
        return list_create( "node_ref", 0 );
    }
    return list_select( "node_ref[node[name = '${node_name}'] && file[id != ${file_id}]]" );
}

list
FindAnyViewOfSelectedNodeSymbol( int node_id, int file_id )
{
    if( node_id == NULL || node_id == 0 )
    {
        print_error( "Unlabeled symbol selected" );
        return list_create( "node_ref", 0 );
    }
    return list_select( "node_ref[node_id = ${node_id} && file[id != ${file_id}]]" );
}
// End add ECR 3785

// Begin Add ECR 6389
boolean
symbol_has_description( int id  )
{
    note gen_obj = find_by_query( "note[GenericObject && obj_id == ${id}]" );
    if( gen_obj )
    {
        if( string_length( gen_obj.desc ) > 0 )
            return True;
    }
    return False;
}
// End Add ECR 6389
