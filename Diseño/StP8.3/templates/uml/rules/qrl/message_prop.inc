
//      StP/UML
//      Confidential property of Aonix
//      Copyright (c) 1997
//      All rights reserved
//

///////////////////////////////////////////////////////
// based on OO object_prop.inc
///////////////////////////////////////////////////////

#include "rules/qrl/uml_inherit_ops.inc"

const	string	OPER_NAME_PATTERN_PROP = "Pattern";
const	string	OPER_NAME_OPERNAME_PROP = "OperationName";
const	string  OP_APPLY_DEF_PROP = "Apply";
const	string	OPER_NAME_BROWSER_PROP = "OperationNameBrowser";

//ui_prop	uml_operations_name_browser = NULL;
//ui_prop	uml_oper_name_list = NULL;
boolean	uml_operations_name_browser_defined = False;
boolean	UML_MESSAGE_SYMBOL_OK = False;
string globalPattern = NULL;

boolean uml_ui_browser_owner_visible()
{
    ui_prop uml_operations_name_browser = to_ui_prop(OPER_NAME_BROWSER_PROP,
						     OPER_NAME_BROWSER_PROP);
    boolean isVisible = ui_owner_visible(uml_operations_name_browser);
    return isVisible;
}

void UmlMessageNameFromClassOperations()
{
    gde_builtin("PropertySheetShow " + OPER_NAME_BROWSER_PROP);
    // Is displayed now ....
    //MessageOperationNameInitialize();
    uml_operations_name_browser_defined = uml_ui_browser_owner_visible();
    //ui_prop uml_oper_name_list = to_ui_prop(OPER_NAME_BROWSER_PROP, "Operations");
    //MessageOperationPatternCb(NULL, NULL, 0, "");
    globalPattern = NULL;
    IsMessageOperationValid(NULL);
    uml_message_prop_active();
}

gde_symbol message_symbol_to_be_labeled;
list allMessageSymTypes = NULL;
int allMessageSymTypesCount = 0;

int uml_message_prop_active()
{

    list	symbols;
    gde_symbol	s;
    int		found;
    string	tp;

// ECR 6577 begin
    //const string messageNames = "SimpleMessage SynchronousMessage BalkingMessage TimeoutMessage AsynchronousMessage RevSimpleMessage RevSynchronousMessage RevBalkingMessage RevTimeoutMessage RevAsynchronousMessage ReturnMessage";
    const string messageNames = "SimpleMessage SynchronousMessage BalkingMessage TimeoutMessage AsynchronousMessage RevSimpleMessage RevSynchronousMessage RevBalkingMessage RevTimeoutMessage RevAsynchronousMessage ReturnMessage RevReturnMessage";
// ECR 6577 end
    if(allMessageSymTypes==NULL) {
	allMessageSymTypes = string_to_list(messageNames," ");
	allMessageSymTypesCount = list_count(allMessageSymTypes);
    }

    symbols = gde_selected_symbols();
    message_symbol_to_be_labeled = NULL;

    if (list_count(symbols) != 1) {
	uml_operations_browser_activate(False);
	return(ACTIVE_FUNC_IS_INACTIVE);
    }
    s = list_get(symbols, 0);
    tp = gde_get_symbol_type(s);
    found = list_find(allMessageSymTypes, 0, tp);
    if (found < allMessageSymTypesCount) {
	message_symbol_to_be_labeled = s;
	uml_operations_browser_activate(True);
	return(ACTIVE_FUNC_IS_ACTIVE);
    } else {
	uml_operations_browser_activate(False);
	return(ACTIVE_FUNC_IS_INACTIVE);
    }
}

void uml_operations_browser_activate(boolean act)
{
    if (uml_operations_name_browser_defined == True)
    {
	if (uml_ui_browser_owner_visible() == True)
	{
	    MessageOperationSetSymOk(act);
	    IsMessageOperationValid(NULL);
	    globalPattern = NULL;
	    MessageOperationsBrowserPopulate(message_symbol_to_be_labeled);
	} else {
	    uml_operations_name_browser_defined = False;
	    MessageOperationsBrowserPopulate(NULL);
	}
    }
}


void MessageOperationSetSymOk(boolean act)
{
    UML_MESSAGE_SYMBOL_OK = act;
}

boolean MessageOperationSymOk()
{
    return(UML_MESSAGE_SYMBOL_OK);
}


boolean IsMessageOperationValid(ui_prop p)
{
    string	s;
    int		l;
    boolean	nv;
    ui_prop	applybtn;

    if (p == NULL)
    {
	p = to_ui_prop(OPER_NAME_BROWSER_PROP, OPER_NAME_OPERNAME_PROP);
    }
    if (MessageOperationSymOk() == True)
    {
	s = ui_value(p);
	l = string_length(s);
	if (l > 0)
	{
	    nv = True;
	} else {
	    nv = False;
	}
    } else {
	nv = False;
    }
    applybtn = ui_apply_button(OPER_NAME_BROWSER_PROP);
    ui_activate(applybtn, nv);
    applybtn = ui_ok_button(OPER_NAME_BROWSER_PROP);
    ui_activate(applybtn, nv);
    return(nv);
}

int
MessageOperationCharTyped(ui_event_tp event, ui_prop theprop, int id, string value)
{
    IsMessageOperationValid(theprop);
    return(0);
}

list            MESSAGE_OPERATIONS_OPS_LIST = NULL;

void MessageOperationPopulateList(list l)
{
    MESSAGE_OPERATIONS_OPS_LIST = l;
    MessageOperationPatternCb(NULL, NULL, 0, "");
}

void uml_message_name_set_global_pattern(string pattern)
{
    string pp = "*";
    if(pattern!=NULL) 
	pp = pattern;
    ui_set_value(to_ui_prop(OPER_NAME_BROWSER_PROP,OPER_NAME_PATTERN_PROP),pp);
    globalPattern = pp;
}

int uml_message_patternCB(ui_event_tp event, ui_prop theprop, int id, string value)
{
    ui_prop	p, f;
    string	pattern, val;
    list	sl;
    int		i, cnt, lcnt;
    ui_owner	o;

    p = to_ui_prop(OPER_NAME_BROWSER_PROP, OPER_NAME_PATTERN_PROP);
    pattern = ui_value(p);
    if(pattern=="")
	globalPattern = "*";
    else
	globalPattern = pattern;
    //message("DEBUG: MessageOperationPatternCb::pattern='"+pattern+"' MESSAGE_OPERATIONS_OPS_LIST="+MESSAGE_OPERATIONS_OPS_LIST);
    MessageOperationsBrowserPopulate(message_symbol_to_be_labeled);
    return 0;
}

int MessageOperationPatternCb(ui_event_tp event, ui_prop theprop, int id, string value)
{
    ui_prop	p, f;
    string	pattern, val;
    list	sl;
    int		i, cnt, lcnt;
    ui_owner	o;

    ui_prop uml_oper_name_list = to_ui_prop(OPER_NAME_BROWSER_PROP, "Operations");
    if (MESSAGE_OPERATIONS_OPS_LIST == NULL) {
	ui_list_clear(uml_oper_name_list);
	ui_set_value(uml_oper_name_list, list_create("string", 0));
	return(0);
    }

    o = to_ui_owner(OPER_NAME_BROWSER_PROP);
//     p = to_ui_prop(OPER_NAME_BROWSER_PROP, OPER_NAME_PATTERN_PROP);
//     pattern = ui_value(p);
//     if(pattern=="")
// 	globalPattern = "*";
//     else
// 	globalPattern = pattern;
//     //message("DEBUG: MessageOperationPatternCb::pattern='"+pattern+"' MESSAGE_OPERATIONS_OPS_LIST="+MESSAGE_OPERATIONS_OPS_LIST);
    pattern = NULL;
    ui_busy(o);
    f = to_ui_prop(OPER_NAME_BROWSER_PROP, "Status");
    lcnt = list_count(MESSAGE_OPERATIONS_OPS_LIST);

    ui_list_clear(uml_oper_name_list);

    if (pattern == NULL || pattern == "" || pattern == "*") {
	ui_set_value(f, "Found " + lcnt + " operations");
	ui_set_value(uml_oper_name_list, MESSAGE_OPERATIONS_OPS_LIST);
	cnt = lcnt;
    } else {
	sl = list_create("string", 0);
	cnt = 0;
	for(i = 0; i < lcnt; i = i + 1) {
	    val = list_get(MESSAGE_OPERATIONS_OPS_LIST, i);
	    if (string_match_any(val, pattern) == True) {
		list_append(sl, val);
		cnt = cnt + 1;
	    }
	}
	ui_set_value(f, "Found " + cnt + " operations");
	ui_set_value(uml_oper_name_list, sl);
    }
    ui_busy(o, False);
    if (cnt > 0) {
	MessageOperationRowSelected(NULL, uml_oper_name_list, 0, "0");
    }
    return(0);
}

int
MessageOperationRowSelected(ui_event_tp event, ui_prop theprop, int id, string value){
    int		row;
    string	v;

    row = to_int(value);
    v = ui_list_row_label(theprop, row);

    ui_set_value(to_ui_prop(OPER_NAME_BROWSER_PROP,OPER_NAME_OPERNAME_PROP),v);
    IsMessageOperationValid(NULL);
    return(0);
}

// int
// MessageOperationDialog_Help(ui_event_tp event, ui_prop theprop, int id, string v)
// {
//     return(0);
// }

int
MessageOperationDialog_Apply(ui_event_tp event, ui_prop theprop, int id, string v)
{
    ui_prop	p;
    string	s;
    int		rc = 0;

    if (ui_button_is_ok(theprop))
	rc = 1;

    p = to_ui_prop(OPER_NAME_BROWSER_PROP, OPER_NAME_OPERNAME_PROP);
    if (IsMessageOperationValid(p) == True)
    {
	s = ui_value(p);
	if(uml_update_message_label_oper(message_symbol_to_be_labeled,s))
	    return(rc);
    }
    return(0);
}

boolean uml_update_message_label_oper(gde_symbol gSym, string newLabel)
{
    gde_cntx gCntx;
    gde_arc gArc;
    string symLabel;
    int theTransaction;

    // save current state for editor
    gde_save_state();
    theTransaction = gde_start_edit_transaction();

    //trace("all");
    if(gde_symbol_is_cntx(gSym)) {
	gCntx = to_gde_cntx(gSym);
	symLabel = gde_cntx_label(gCntx);
	newLabel = uml_change_label_using_new_name(symLabel,newLabel);
	gde_cntx_set_label(gCntx, newLabel);
    } else if(gde_symbol_is_arc(gSym)) {
	gArc = to_gde_arc(gSym);
	symLabel = gde_arc_label(gArc);
	newLabel = uml_change_label_using_new_name(symLabel,newLabel);
	gde_arc_set_label(gArc, newLabel);
    } else {
	print_message("Selected symbol is  neither cntx nor arc");
	return False;
    }
    // end transaction
    gde_end_edit_transaction(GDE_CMD_INSERT, theTransaction, False);
    return True;
}

void MessageOperationsBrowserPopulate(gde_symbol gSym)
{
    string namePattern = globalPattern;
    gde_cntx gCntx;
    gde_arc gArc;
    string symLabel, symType;
    list links, gdeArcLinks;
    node omsToNode, objScopeClass,serverClass, omsOper;

    list operations = list_create("string", 0);

    if(gSym==NULL) {
	MessageOperationPopulateList(operations);
	symLabel = "";
	return;
    }

    if(gde_symbol_is_cntx(gSym)) {
	gCntx = to_gde_cntx(gSym);
	symLabel = gde_cntx_label(gCntx);
	links = gde_cntx_oms_links(gCntx);
    } else if(gde_symbol_is_arc(gSym)) {
	gArc = to_gde_arc(gSym);
	symLabel = gde_arc_label(gArc);
	//links = gde_arc_oms_links(gArc);
	gdeArcLinks = gde_arc_links(gArc);
	links = gde_link_oms_links(list_get(gdeArcLinks,0));
    } else {
	print_message("Selected symbol is  neither cntx nor arc");
	MessageOperationPopulateList(operations);
	return;
    }
    if(list_count(links) == 0) {
	print_message("Message '"+symLabel+"' does not have mapped message");
	MessageOperationPopulateList(operations);
	return;
    }

    link mssgLink = list_get(links,0);
    item mssgNameItem = find_by_query("item[UmlObjectName & obj_id=${mssgLink.id}]");
    if(mssgNameItem == NULL) {
	print_message("Message '"+symLabel+"' does not have mapped UmlObjectName item");
	MessageOperationPopulateList(operations);
	return;
    }

    omsToNode = find_by_query("node[id=${mssgLink.to_node_id}]");
    item MessageName = find_by_query("item[UmlObjectName & obj_id=${mssgLink.id}]");
    if(namePattern==NULL) {
	namePattern = mssgNameItem.value + "*"; //symLabel + "*";
	uml_message_name_set_global_pattern(namePattern);
    }
    symType = omsToNode.type;
    //message("checking message '"+mssgLink+"' to node"+omsToNode);
    //print_message("checking message '"+mssgLink+"' to node"+omsToNode);
    if(symType=="UmlObjectInstance") {
	set allClasses = set_create("node");
	objScopeClass = find_by_query("node[id=${omsToNode.scope_node_id}]");
	serverClass = find_by_query("node[UmlClass & name='${objScopeClass.name}']");
	//message("DEBUG serverClass='"+serverClass+"'");
	if(serverClass!=NULL) {
	    allClasses = uml_class_super_classes(serverClass,"",allClasses);
	    //message("DEBUG allClasses/before='"+allClasses+"'");
	    set_add(allClasses,serverClass);
	    //message("DEBUG allClasses/after='"+allClasses+"'");
	}
	int ii;
	node theClass;
	set allOps = set_create("node");
	for(ii=0; ii<set_count(allClasses); ii++) {
	    theClass = set_get_element(allClasses,ii);
	    //message("DEBUG Found:class='"+theClass+"'");
	    for_each_in_select("node[UmlOperation & node_refs & scope_node_id=${theClass.id} & name $ '${namePattern}']", omsOper) {
		set_add(allOps, omsOper);
		//message("DEBUG Found:operation='"+omsOper+"'");
	    }
	}
	set allOpNames = set_create("string");
	string anOpName;
	item retType;
	for(ii=0; ii<set_count(allOps); ii++) {
	    omsOper = set_get_element(allOps,ii);
	    retType = find_by_query("item[UmlOperationReturnType & obj_id=${omsOper.id}]");
	    if(retType!=NULL)
		anOpName = retType.value + ":=" + omsOper.name;
	    else
		anOpName = omsOper.name;
	    set_add(allOpNames, anOpName);
	    //message("DEBUG Found:operation='"+omsOper.name+"'");
	}
	for(ii=0; ii<set_count(allOpNames); ii++) {
	    anOpName = set_get_element(allOpNames,ii);
	    list_append(operations, anOpName);
	    //message("DEBUG Found:operation='"+omsOper.name+"'");
	}
    }
    MessageOperationPopulateList(operations);
}

// ECR 4110
// usequenced/ucollaborationd specifc SymbolNameSearch function
// for messages it brings up the message prop instead of the generic
void
useq_ucollab_name_search()
{
    string editor = current_editor_apptype();
    if( editor == "UmlSequenceDiagram" ) 
    {
        if( messageIsSelectedActiveFunc() )
            UmlPopUpPropSheet("usequenced");
        else
            gde_builtin( "SymbolNameSearch" );
    }
    else if( editor == "UmlCollaborationDiagram" )
    {
        if( object_any_message_selected() )
            UmlPopUpPropSheet("ucollaborationd");
        else
            gde_builtin( "SymbolNameSearch" );
    }
    else
        gde_builtin( "SymbolNameSearch" );        
    return;
}
// End ECR 4110

