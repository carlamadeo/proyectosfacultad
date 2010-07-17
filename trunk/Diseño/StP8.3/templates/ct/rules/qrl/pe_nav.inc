// Navigate to the programming environment and from
// the Programming environment to the editors
// Could eventually walk up scope hierarchy 
// for events/actions/states to class they are attached to

#include "rules/qrl/objectada_nav.inc"
#include "rules/qrl/cc_update.inc"
#include "rules/qrl/re_navigate.inc"
#include "rules/qrl/sniffstuff.inc"

list
BrowseViaSMDB(int id, int is_fn_defn_search)
{
    node	n;
    string      realname;
    
    n = node_from_oid(id);
    if (n == NULL)
	return NULL;

    // no good for operator definitions that have spaces in them
    //realname = nav_operation_identifier( n.name );
    realname = n.name;
    if ( realname == NULL || realname == "" )
      return NULL;

    if (n.type == "OMTClass" || n.type == "OMTSubsystem" || n.type == "UmlClass" || n.type == "UmlPackage")
    {
	return smdb_navigation( realname, NULL, "-T datas" );
    }
    if (n.type == "OMTOperation" || n.type == "UmlOperation")
    {
	if ( n.sig != NULL )
	  realname = realname + "(" + n.sig + ")";
	// don't replace anything with underscores - keep it as it
	// realname = nav_operation_identifier( realname );
	if ( is_fn_defn_search == 0 )
	  return smdb_navigation( get_class_members_classname( n ),
				  realname, "-T datam" );
	else
	  return smdb_navigation( get_class_members_classname( n ),
				  realname, "-T datam -fndefn" );
    }
    if (n.type == "OMTAttribute" || n.type == "UmlAttribute")
    {
	return smdb_navigation( get_class_members_classname( n ),
				realname, "-T datam" );
    }

    if (n.type == "OMTState" || n.type == "OMTConcurrentState" ||
	n.type == "OMTStartState" || n.type == "OMTFinalState" ||
	n.type == "UmlState" || n.type == "UmlConcurrentState" ||
	n.type == "UmlInitialState" || n.type == "UmlFinalState")
    {
      node sn = get_states_scope_class( n );
      if ( sn )
	return smdb_navigation(sn.name, NULL, "-T datas");
    }
    return NULL;
}

void
NavigateToPE(int id)
{
    node	n;
    
    n = node_from_oid(id);
    if (n == NULL)
    {
	return;
    }

    if (n.type == "OMTClass" || n.type == "OMTSubsystem" ||
        n.type == "UmlClass" || n.type == "UmlPackage" ||
	n.type == "UmlSubsystem")
    {
	NavigateToPEFromClass(n.name, n);
	return;
    }
    if (n.type == "OMTOperation" || n.type == "UmlOperation")
    {
	NavigateToPEFromClassComponent(n.name, n);
	return;
    }
    if (n.type == "OMTAttribute" || n.type == "UmlAttribute")
    {
	NavigateToPEFromClassAttribute(n.name, n);
	return;
    }

    if (n.type == "OMTState" || n.type == "OMTConcurrentState" ||
	n.type == "OMTStartState" || n.type == "OMTFinalState" ||
	n.type == "UmlState" || n.type == "UmlConcurrentState" ||
	n.type == "UmlStartState" || n.type == "UmlFinalState")
    {
	NavigateToPEFromState(n.name, n);
	return;
    }
}

void
NavigateToPEFromClass(string name, node n)
{
    if ( is_product_uml() )
        NavigateToPeByName(name, NULL, n);
    else
        NavigateToPeByName(name, NULL, NULL);
}
 
void
NavigateToPEFromClassComponent(string name, node n)
{
    string	sig;

    if (name == NULL)
	return;

    if (n.sig == NULL)
    {
	sig = "";
    } else {
	sig = "(" + n.sig + ")" ;
    }

    // get rid of the spaces and funny characters in name, so the browsing
    // has a chance to work.

    name = nav_operation_identifier(name);

    NavigateToPeByName(get_class_members_classname(n), name + sig, n);
}

void
NavigateToPEFromClassAttribute(string name, node n)
{
    node	sn;
    string	scname;

    if (name == NULL)
	return;

    NavigateToPeByName(get_class_members_classname(n), name, n);
}


void
NavigateToPEFromState(string name, node n)
{
    node	sn = get_states_scope_class(n);

    if ( sn )
      NavigateToPeByName(sn.name, NULL, NULL);
}

const string	PROG_ENV = toolinfo_variable("ProgrammingEnvironment");
const string	OBJECTCENTER_CLASS = "cc4d";
const string	SOFTBENCH_CLASS = "stpcid";
const string	PARCPLACE_CLASS = "parcplaced";
const string	SNIFF_CLASS = "sniffaccess";


void
NavigateToPeByName(string class_name, string member_name, node member)
{
    string          msg;

    if (class_name == NULL)
	return;

    /*------------------------------------------------------------------------*/
    // this is the new default - to do our own internal navigation

    if (PROG_ENV == NULL || PROG_ENV == "")
      {
	smdb_navigate( class_name, member_name, member );
	return;
      }

    /*------------------------------------------------------------------------*/
    if (PROG_ENV == "SoftBench")
    {
	msg = class_name;
	if (member != NULL)
	{
	    msg = msg + "::";
	    if (member.type == "OMTOperation")
		msg = msg + nav_operation_identifier(member_name);
	    else
		msg = msg + member_name;
	}
	editor_send_msg(SOFTBENCH_CLASS,
			"Navigate " +
			editor_get_msgd_name() + " " +
			editor_get_msgd_handle() +
			" \"" + msg + "\"",
			0);

    /*------------------------------------------------------------------------*/
    } else if (PROG_ENV == "ObjectAda")
      {
	// currently objectada only supports navigation from class symbols
	if ( member_name != NULL )
	{
	    print_error( "ObjectAda can only be navigated to from Class symbols." );
	    return;
	}
	nav_to_ada( class_name, 0, member );

    /*------------------------------------------------------------------------*/
    } else if (PROG_ENV == "ParcPlace")
    {
	/* protocol for stp/omt message server */
	msg = "SmalltalkEval \"" +
	    "\\\"" + editor_get_msgd_name() + "\\\" " +
	    "\\\"" + current_projdir() + "\\\" " +
	    "\\\"" + current_system() + "\\\" " +
	    "\\\"" + editor_get_msgd_handle() + "\\\" ";
	/* check if class exists */
	/* if it doesn't, send meself a message */
	string sender = "'-ed " + editor_get_msgd_name() + " -p " + current_projdir() + " -s " + current_system() + " -handle " + editor_get_msgd_handle() + "'";
	msg = msg + "(Smalltalk classNames includes: '" + class_name + "' asSymbol) ifFalse: [StPNavigations editor: " + sender + " PrintError: 'ParcPlace:  Class " + class_name + " does not exist.'] ifTrue: [";
	/* if it does, then browse */
	if (member != NULL && member.type == "OMTOperation")
	{
	    string          operation_name, operation_category, class_class;
	    item            item;
	    operation_name = "#" + string_extract(member_name, 0, string_find(member_name, 0, "("));
	    if (find_by_query("item[obj_id=${member.id} & OMTMemberIsClass & value='True']") != NULL)
		class_class = "class";
	    else
		class_class = "";
	    item = find_by_query("item[obj_id=${member.id} & OMTOperationSmalltalkCategory]");
	    if (item != NULL)
		operation_category = "#'" + item.value + "'";
	    else
		operation_category = "(" + class_name + " " + class_class + " whichCategoryIncludesSelector: " + operation_name + ")";
	    msg = msg +
		"Browser openClassBrowserOn: (((Browser new onClass: " +
		class_name + " " + class_class + ")" +
		"newProtocolList: " + operation_category + ")" +
		"selector: " + operation_name + ")";
	} else
	{
	    msg = msg +
		"Browser openClassBrowserOn: (Browser new onClass: " +
		class_name + ")";
	}
	msg = msg + "]" + "\"";
	editor_send_msg(PARCPLACE_CLASS, msg, 0);


    /*------------------------------------------------------------------------*/
    } else if (PROG_ENV == "SNiFF")
    {
	SNiFF__Navigate(NULL, class_name, member_name, member);
    /*------------------------------------------------------------------------*/
    } else if (PROG_ENV == "ObjectCenter" )
    {
	msg = class_name;
	/* ObjectCenter doesn't support navigation to attributes */
	if (member != NULL && member.type == "OMTOperation")
	{
	    msg = msg + "::";
	    msg = msg + nav_operation_identifier(member_name);
	}
	editor_send_msg(OBJECTCENTER_CLASS, "list \"" + msg + "\"", 0);
    }
}



void
NavigateToTTE()
 
{
    string val, query;
    string msg, fileName;
    file file;

    // find the typename by looking at the current place in the table
 
    val = gte_get_cell_label(gte_get_current_cell_row(),
                             gte_get_current_cell_col());
 
    if (val == NULL || val == "")
    {
        gte_print_error("Cannot navigate to TTE without a type name");
	return;
    }
 
    query = "file[node_refs[node[TTestType && name == '${val}']]]";
    file = find_by_query(query);
 
    if (file != NULL)
	fileName = file.name;
    else
	fileName = "";
    
    msg = "EditorQrlEval TTestTableInit(\"" + fileName + "\",\"" +
           val + "\");";
    editor_send_msg("tte", msg, CH_FIRST);

    return;
}	

