
//      StP/UML
//      Confidential property of Aonix
//      Copyright (c) 1997
//      All rights reserved
//

int
uml_interaction_dg_completeness_check_messages(string constraint_dg_name)
{
    const string qFindMessages = "link[(type=UmlSimpleMessage || type=UmlSynchronousMessage || type=UmlBalkingMessage || type=UmlTimeoutMessage || type=UmlAsynchronousMessage) && link_refs"+constraint_dg_name+"]";
    const string qFindOperation = "node_ref[node[UmlOperation && name='${mssg.name}']]";
    int ii;
    int count;
    int errCount = 0;
    link mssg;
    node_ref anOp;
    for_each_in_select(qFindMessages, mssg) {
	anOp = find_by_query(qFindOperation);
	if(anOp==NULL) {
	    errCount++;
	    print_error("Message '"+mssg.name+"' does not have a corresponding Operation defined");
	}
    }
    return errCount;
}

int
interaction_dg_completeness_check_objects(string constraint_dg_name)
{
    print_message("Checking Objects ...");
    const string qFindObjScopes = "node[UmlObjectClassScope && node_refs["+constraint_dg_name+"]]";
    const string qFindClass = "node_ref[node[UmlClass && name='${objScope.name}']]";
    const string qFindClassStereotype = "item[UmlStereotype & node[UmlClass && name='${objScope.name}'] & value='utility']";
    int ii;
    int count;
    int errCount = 0;
    node objScope;
    node_ref aClass, objRef;
    item aStereotype;
    for_each_in_select(qFindObjScopes, objScope) {
	if ((string_length(objScope.name) < 2) || string_extract(objScope.name,0,2)!="%%") {
	    aClass = find_by_query(qFindClass);
	    if(aClass==NULL) {
		errCount++;
		objRef = find_by_query("node_ref[node_id=${objScope.id} & "+constraint_dg_name+"]");
		//message("interaction_dg_completeness_check_objects::objScope="+objScope+" objRef="+objRef);
		gde_print_error("Object (scope) class '"+objScope.name+"' does not have a corresponding Class defined",to_int(objRef.appid));
	    } else { // check if the class is <<utility>>
		// UML1.0 Semantics 7.4, p.53 
		aStereotype = find_by_query(qFindClassStereotype);
		if(aStereotype!=NULL) {
		    errCount++;
		    objRef = find_by_query("node_ref[node_id=${objScope.id} & "+constraint_dg_name+"]");
		    gde_print_error("Class '"+objScope.name+"' is of stereotype <<"+aStereotype.value+">> and cannot have instances",to_int(objRef.appid));
		}
	    }
	}
    }
    return errCount;
}

const string UML_ACTOR = "Actor";
int
interaction_dg_completeness_check_actors(string constraint_dg_name)
{
    print_message("Checking Actors ...");
    //list gnActors = gde_nodes_find_by_type(gde_all_symbols(),UML_ACTOR);
    //list nodes;
    int ii, count, errCount = 0;
    node actor;
    node_ref actorRef;
    string actorStr = " Actors do";
    list actors = list_select("node[UmlActor && node_refs[file["+constraint_dg_name+"]] && !node_refs[file[UmlUseCasediagram]]]");
    if(actors!=NULL) {
	count = list_count(actors);
	if(count==1)
	    actorStr = " Actor does";
// 	if(count>0) {
// 	    print_message(count + actorStr + " not appear in any Use Case diagram:"+actors);
// 	}
	for(ii=0; ii<count; ii++) {
	    actor = list_get(actors,ii);
	    actorRef = find_by_query("node_ref[node_id=${actor.id} & file["+constraint_dg_name+"]]");
	    gde_print_error("Actor '"+actor.name+"' does not appear in any Use Case diagram",to_int(actorRef.appid));
	}
    }
    errCount = count;
    return errCount;
}

int
interaction_dg_completeness_check_parent(string dg_name, string dg_type,string constraint)
{
    print_message("Checking Parent ...");
    int result = 0;
    string qFindScenarioInstance = "node[UmlScenarioInstance && node_refs"+constraint+"]";
    node aScenario = find_by_query(qFindScenarioInstance);
    if(aScenario==NULL) {
	print_message("Warning? "+dg_type+" Diagram '"+dg_name+"' does not have a ScenarioInstance object (does not have a parent)");
	return 1;
    }
    return result;
}

// int uml_interaction_dg_check_extension_points(string constraint_dg_name) {}

const string UML_EXTENSION_POINT = "ExtensionPoint";
const string UML_SCENARIO_INSTANCE = "ScenarioInstance";
int interaction_dg_check_extension_points()
{
    print_message("Checking Extension Points ...");
    int ii, errCount = 0;
    gde_node gNode, gnScenario;
    list extPoints = gde_nodes_find_by_type(gde_all_symbols(),UML_EXTENSION_POINT);
    list scenarios = gde_nodes_find_by_type(gde_all_symbols(),UML_SCENARIO_INSTANCE);
    if(list_count(scenarios)==0) {
	if(list_count(extPoints)>0) {
	    errCount++;
	    gNode = list_get(extPoints,0);
	    gde_print_error("Scenario has an Extension Point'"+gde_node_label(gNode)+"' but not a parent [there is no ScenarioInstance in a diagram]",gde_node_psymid(gNode));
	}
	return errCount;
    }
    if(list_count(scenarios)>1) {
	for(ii=0; ii<list_count(scenarios); ii++) {
	    gnScenario = list_get(scenarios,ii);
	    gde_print_error("Scenario has more than ScenarioInstance '"+gde_node_label(gnScenario)+"'",gde_node_psymid(gnScenario));
	}
	return errCount;
    }
    if(list_count(extPoints)==0)
	return errCount;

    // we are here because there is at least one extension point
    // check if all have  corresponding Extend Use Cases

    // first, find scenario instance node
    gnScenario = list_get(scenarios,0);
    list nodes = gde_node_oms_nodes(gnScenario);
    if(list_count(nodes) == 0) {
	errCount++;
	gde_print_error("ScenarioInstance '"+gde_node_label(gnScenario)+"' does not have mapped object",gde_node_psymid(gnScenario));
	return errCount;
    }
    node aScenario = list_get(nodes,0);
    item parentType = find_by_query("item[UmlParentTypeItem & obj_id=${aScenario.id}]");    item parentName = find_by_query("item[UmlParentNameItem & obj_id=${aScenario.id}]");
    if(parentType==NULL || parentName==NULL) {
	errCount++;
	gde_print_error("ScenarioInstance '"+gde_node_label(gnScenario)+"' has incomplete mappings:: UmlParentTypeItem='"+parentType+"' UmlParentNameItem='"+parentName+"'",gde_node_psymid(gnScenario));
	return errCount;
    }
    if(parentType.value!="UmlUseCase") {
	errCount++;
	gde_print_error("Scenario's parent is not a Use Case (it is '"+parentType.value+"')",gde_node_psymid(gnScenario));
	return errCount;
    }
    // next, find the parent use case of this scenario
    
    node useCaseParent = find_by_query("node[UmlUseCase & node_refs & name='${parentName.value}']");
    if(useCaseParent==NULL) {
	errCount++;
	gde_print_error("Scenario's parent is not found (it does not exist or perhaps you have unsaved files?)",gde_node_psymid(gnScenario));
	return errCount;
    }
    // now check if one of its extend use cases, if any, matched an extension point.
    node extPoint; //extendUseCase;
    link theExtendsLink;
    const string qFindExtendsLink = "link[UmlUseCaseExtends & link_refs & from_node[UmlUseCase && name='${extPoint.name}'] & to_node_id=${useCaseParent.id}]";
    
    for(ii = 0; ii < list_count(extPoints); ii++) {
	gNode = list_get(extPoints,ii);
	//print_node("checking node "+ii+":",ucGNode);
	nodes = gde_node_oms_nodes(gNode);
	if(list_count(nodes) == 0) {
	    errCount++;
	    gde_print_error("Extension Point '"+gde_node_label(gNode)+"' does not have mapped object",gde_node_psymid(gNode));
	    continue;
	}
	extPoint = list_get(nodes,0);
	theExtendsLink = find_by_query(qFindExtendsLink);
	if(theExtendsLink==NULL) {
	    errCount++;
	    gde_print_error("Extension Point '"+gde_node_label(gNode)+"' does not have a corresponding Extend Use Case - there is no such extension for this scenario's parent Use Case '"+parentName.value+"'",gde_node_psymid(gNode));
	}
    }
    return errCount;
}

const string UML_TYPE_ELEMENT = "TypeElement";

int
interaction_dg_check_typeDef(string constraint_dg_name)
{
    int errCount = 0;
    print_message("Checking Type definitions ...");
    const string messageNames = "SimpleMessage SynchronousMessage BalkingMessage TimeoutMessage AsynchronousMessage RevSimpleMessage RevSynchronousMessage RevBalkingMessage RevTimeoutMessage RevAsynchronousMessage";
    list typeDefs = gde_nodes_find_by_type(gde_all_symbols(),UML_TYPE_ELEMENT);
    if(list_count(typeDefs)>0) {
	// get all messages
	if(allMessageLinkTypes==NULL)
	    allMessageLinkTypes = string_to_list(messageNames," ");
	int ii;
	list allMessages = gde_symbols_find_with_types(gde_all_symbols(),
						       allMessageLinkTypes);
	if(list_count(allMessages)>0) {
	    print_error("Diagram contains Type definition and should not contain message flows");
	    // this can be replaced with a more detailed check: check
	    // all from/to nodes for each arc; if any is contained
	    // (directly or indirectly, nested in another object?)
	    // within the type element, print additional error for
	    // that arc/link
	    // AND/OR: an error for each type element
	    errCount++;
	}
    }
    return errCount;
}

int
interaction_dg_check_collaborationContext(string constraint_dg_name)
{
    // disable this check for now
    // check against roles and/or classes??
//     item nameItem = find_by_query("item[UmlParentNameItem && obj_id=${aScenario.id}]");
//     string qCollaboration = "node[UmlCollaboration && node_refs && name='${nameItem.value}']";
//     node collab = find_by_query(qCollaboration);
    return 0;
}

int
uml_interaction_dg_completeness_check_externalEvents(string constraint_dg_name)
{
    string qFindActors = "node[UmlActor && node_refs"+constraint_dg_name+"]";
    list actors = list_select(qFindActors);
    if(list_count(actors)==0)
	return 0;
    node aScenario = find_by_query("node[UmlScenarioInstance && node_refs"+constraint_dg_name+"]");
    if(aScenario==NULL) {
	print_error("Diagram has External events, but does not have a parent Use Case");
	return 1;
    }
    item nameItem = find_by_query("item[UmlParentNameItem && obj_id=${aScenario.id}]");
    string qFindUseCase = "node[UmlUseCase && node_refs && name='${nameItem.value}']";
    node useCase = find_by_query(qFindUseCase);
    //ECR 5147 begin
    list extEvents = list_select("link[link_refs"+constraint_dg_name+" && from_node[UmlActor]]");
    list ll = list_select("link[link_refs"+constraint_dg_name+" && to_node[UmlActor]]");
    list_append(extEvents, ll);
    //ECR 5147 begin
    if(list_count(extEvents)>0) {
	link commLink = find_by_query("link[UmlUseCaseInteraction && link_refs && from_"+qFindActors+" && to_"+qFindUseCase+"]");
	if(commLink==NULL) {
	    print_error("Diagram contains external events without corresponding Interaction links in any Use Case diagram");
	    return 1;
	}
    }
    return 0;
}


int
interaction_dg_check_single_message(link theLink,string prefix,int psymId)
{
    int errCount = 0;
    boolean found = False;
    int count, ii, opArgsCnt;
    string opSig;
    // do not use mssgLabel because of multi-segment arc - some
    // segments may have null label and the message as a whole is
    // supposed to comine individual labels
    string linkName = theLink.name;
    node anOp;
    const string qFindOperation = "node[UmlOperation & node_refs & name='${mssgName}']";
    string retType, mssgArgs, mssgName;
    item ReturnType = find_by_query("item[UmlReturnType & obj_id=${theLink.id}]");
    item MessageArgs = find_by_query("item[UmlMessageArgs & obj_id=${theLink.id}]");
    item MessageName = find_by_query("item[UmlObjectName & obj_id=${theLink.id}]");
    retType = ReturnType.value;
    mssgArgs = MessageArgs.value;
    mssgName = MessageName.value;
    int argCount = list_count(string_to_list(mssgArgs,","));
    list operations = list_select(qFindOperation);
    //found = False;
    //message("check_single_message::'"+linkName+"'args="+mssgArgs+theLink);
    if(list_count(operations)==0) {//{if(anOp==NULL)
	errCount++;
	gde_print_error(prefix+"Message '"+linkName+"' does not have a corresponding Operation defined",psymId);
    } else {
	count = list_count(operations);
	for(ii=0; ii<count && !found; ii++) {
	    // check arguments and the owner/class
	    // check op signature and count arguments 
	    anOp = list_get(operations,ii);
	    opSig = anOp.sig;
	    opArgsCnt = list_count(string_to_list(opSig,","));
	    if(opArgsCnt==argCount) {
		found = True;
		break;
	    }
	}
	if(!found) {
	    errCount++;
	    gde_print_error("Message '"+linkName+"' does not have a corresponding Operation defined, with the same number of arguments ("+argCount+")",psymId);
	}
    }
    return errCount;
}


int 
interaction_dg_completeness_check_messages()
{
    list processedLinks = list_create("link", 0);
    int linkCnt = 0;
    const string messageNames = "SimpleMessage SynchronousMessage BalkingMessage TimeoutMessage AsynchronousMessage RevSimpleMessage RevSynchronousMessage RevBalkingMessage RevTimeoutMessage RevAsynchronousMessage";
    print_message("Checking Messages ...");
    // get all messages
    if(allMessageLinkTypes==NULL)
	allMessageLinkTypes = string_to_list(messageNames," ");
    int ii;
    list allMessages = gde_symbols_find_with_types(gde_all_symbols(),
						   allMessageLinkTypes);

    gde_cntx gCntx; //, aMessage;
    gde_arc gArc;
    gde_symbol gSym;
    gde_node toNode;
    string symLabel, symType;
    list links, gdeArcLinks; // nodes;
    node omsToNode;
    link commLink, mssgLink;
    int psymId, errCount = 0;
    for(ii=0; ii<list_count(allMessages); ii++) {
	gSym = list_get(allMessages,ii);
	if(gde_symbol_is_cntx(gSym)) {
	    gCntx = to_gde_cntx(gSym);
	    symLabel = gde_cntx_label(gCntx);
	    psymId = gde_cntx_psymid(gCntx);
	    links = gde_cntx_oms_links(gCntx);
	} else if(gde_symbol_is_arc(gSym)) {
	    gArc = to_gde_arc(gSym);
	    symLabel = gde_arc_label(gArc);
	    psymId = gde_arc_psymid(gArc);
	    //links = gde_arc_oms_links(gArc);
	    gdeArcLinks = gde_arc_links(gArc);
	    links = gde_link_oms_links(list_get(gdeArcLinks,0));
	} else {
	    print_message("Message symbol found - neither cntx nor arc");
	    continue;
	}
	if(list_count(links) == 0) {
	    errCount++;
	    gde_print_error("Message '"+symLabel+"' does not have mapped object",psymId);
	    continue; //return errCount;
	}
	// check if this is a message whose target is an object
	mssgLink = list_get(links,0);
	omsToNode = find_by_query("node[id=${mssgLink.to_node_id}]");
	symType = omsToNode.type;
	//message("checking message '"+mssgLink+"' to node"+omsToNode);
	//print_message("checking message '"+mssgLink+"' to node"+omsToNode);
	if(symType=="UmlActor") {
	    continue;
	} else {
	    linkCnt = list_count(processedLinks);
	    if(linkCnt>0 && list_find(processedLinks,0,mssgLink)<linkCnt) {
		// repeated link - multi-segment arc.  Do not count as
		// a new error (if it is error), but print error
		// message because we want to associate all segments
		// with the error (message label can be spread across
		// segments anyway).
		interaction_dg_check_single_message(mssgLink,"[repeated error] ",psymId);
	    } else {
		errCount += interaction_dg_check_single_message(mssgLink,"",psymId);
		list_append(processedLinks,mssgLink);
	    }
	}
    }
    return errCount;
}


int 
interaction_dg_completeness_check_externalEvents(string constraint_dg_name)
{
    const string messageNames = "SimpleMessage SynchronousMessage BalkingMessage TimeoutMessage AsynchronousMessage RevSimpleMessage RevSynchronousMessage RevBalkingMessage RevTimeoutMessage RevAsynchronousMessage";
    print_message("Checking External Events ...");
    string qFindActors = "node[UmlActor && node_refs"+constraint_dg_name+"]";
    list actors = list_select(qFindActors);
    if(list_count(actors)==0)
	return 0;
    node aScenario = find_by_query("node[UmlScenarioInstance && node_refs"+constraint_dg_name+"]");
    item nameItem;
    string qFindUseCase = "node[UmlUseCase && node_refs && name='${nameItem.value}']";
    node useCase;
    if(aScenario!=NULL) {
	nameItem = find_by_query("item[UmlParentNameItem && obj_id=${aScenario.id}]");
	useCase = find_by_query(qFindUseCase);
    }

    // now get all messages
    if(allMessageLinkTypes==NULL)
	allMessageLinkTypes = string_to_list(messageNames," ");
    int ii;
    list allMessages = gde_symbols_find_with_types(gde_all_symbols(),
						   allMessageLinkTypes);
    gde_cntx gCntx; //, aMessage;
    gde_arc gArc;
    gde_symbol gSym;
    gde_node gnActor;
    string symLabel;
    list links, gdeArcLinks; // nodes;
    node omsActor;
    link commLink, mssgLink;
    int psymId, errCount = 0;
    for(ii=0; ii<list_count(allMessages); ii++) {
	gSym = list_get(allMessages,ii);
	if(gde_symbol_is_cntx(gSym)) {
	    gCntx = to_gde_cntx(gSym);
	    symLabel = gde_cntx_label(gCntx);
	    psymId = gde_cntx_psymid(gCntx);
	    links = gde_cntx_oms_links(gCntx);
	} else if(gde_symbol_is_arc(gSym)) {
	    gArc = to_gde_arc(gSym);
	    symLabel = gde_arc_label(gArc);
	    psymId = gde_arc_psymid(gArc);
	    //links = gde_arc_oms_links(gArc);
	    gdeArcLinks = gde_arc_links(gArc);
	    links = gde_link_oms_links(list_get(gdeArcLinks,0));
	} else {
	    print_message("Message symbol found - neither cntx nor arc");
	    continue;
	}
	if(list_count(links) == 0) {
	    errCount++;
	    gde_print_error("Message '"+symLabel+"' does not have mapped object",psymId);
	    continue; //return errCount;
	}
	// check if this is an extenral event
	mssgLink = list_get(links,0);
	omsActor = find_by_query("node[UmlActor & id=${mssgLink.from_node_id}]");
	if(omsActor==NULL) {
	    omsActor = find_by_query("node[UmlActor & id=${mssgLink.to_node_id}]");
	    if(omsActor==NULL)
		continue;	// no, it is not
	}
	// yes, it is external event
	if(aScenario==NULL) {
	    print_error("Diagram has External events, but does not have a parent Use Case");
	    return 1;
	}
	// this is an external event.  Check if it has a corresponding
	// communicates link
// 	nodes = gde_node_oms_nodes(gnActor);
// 	omsActor = list_get(nodes,0);
	commLink = find_by_query("link[UmlUseCaseInteraction && link_refs && from_node_id=${omsActor.id} & to_"+qFindUseCase+"]");
	if(commLink==NULL) {
	    errCount++;
	    gde_print_error("External event "+symLabel+"' does not have a corresponding communicates link",psymId);
	}
    }
    return errCount;
}


// int
// interaction_dg_consistency_check_actors_scenarios(string constraint_dg_name)
// {
//     const string qFindCommunicatesLinks = "link[UmlActor && link_refs"+constraint_dg_name+"]";
//     const string qFindActors = "link[UmlUseCaseInteraction && link_refs"+constraint_dg_name+"]";
//     const string qFindExternalEventLink = "link_ref[file[(UmlSequenceDiagram || UmlCollaborationDiagram) && node_refs[node[UmlScenarioInstance && items[UmlParentTypeItem && value='${theUseCase.type}'] && items[UmlParentNameItem && value='${theUseCase.name}']]]] && link[from_node_id=${theActor.id}]]";
//     int errCount = 0;
//     link commLink;
//     node theActor;
//     for_each_in_select(qFindActors, theActor) {
// 	node theUseCase;
// 	list extEventLinkRefs_list;
// 	for_each_in_select(qFindAllScenariosByParent, commLink) {
// 	    theActor = find_by_query("node[${commLink.from_node_id}]");
// 	    theUseCase = find_by_query("node[${commLink.to_node_id}]");
// 	    extEventLinkRefs_list = list_select(qFindExternalEventLink);
// 	    if(list_count(extEventLinkRefs_list)==0) {
// 		errCount++;
// 		print_error("Interaction between Actor '"+theActor.name+"' and Use Case '"+theUseCase.name+"' does not have a corresponding external event in any scenario");
// 	    }
// 	}
//     }
//     return errCount;
// }

//
// ucollaborationd uses cntx symbols, while usequenced uses arc
// so we cannot reuse exactly the same functions for both
//

//list allMessageLinkTypes = NULL;
int 
usequenced_completeness_check_externalEvents(string constraint_dg_name)
{
    const string messageNames = "SimpleMessage SynchronousMessage BalkingMessage TimeoutMessage AsynchronousMessage";
    print_message("Checking External Events ...");
    string qFindActors = "node[UmlActor && node_refs"+constraint_dg_name+"]";
    list actors = list_select(qFindActors);
    if(list_count(actors)==0)
	return 0;
    node aScenario = find_by_query("node[UmlScenarioInstance && node_refs"+constraint_dg_name+"]");
    item nameItem;
    string qFindUseCase = "node[UmlUseCase && node_refs && name='${nameItem.value}']";
    node useCase;
    if(aScenario!=NULL) {
	nameItem = find_by_query("item[UmlParentNameItem && obj_id=${aScenario.id}]");
	useCase = find_by_query(qFindUseCase);
    }

    // now get all messages
    if(allMessageLinkTypes==NULL)
	allMessageLinkTypes = string_to_list(messageNames," ");
    int ii;
    string symTypeName;
    list allMessages = gde_arcs_find_by_type(gde_all_symbols(),
					     list_get(allMessageLinkTypes,0));
    for(ii=1; ii<list_count(allMessageLinkTypes); ii++) {
	symTypeName = list_get(allMessageLinkTypes,ii);
	list_concatenate(allMessages,
			 gde_arcs_find_by_type(gde_all_symbols(),symTypeName));
    }

    gde_arc aMessage;
    gde_node gnActor;
    string symType;
    list nodes;
    node omsActor;
    link commLink;
    int errCount = 0;
    for(ii=0; ii<list_count(allMessages); ii++) {
	aMessage = list_get(allMessages,ii);
	// check if this is an extenral event
	gnActor = gde_arc_fromnode(aMessage);
	symType = gde_get_symbol_type(to_gde_symbol(gnActor));
	if(symType!="Actor") {
	    gnActor = gde_arc_tonode(aMessage);
	    symType = gde_get_symbol_type(to_gde_symbol(gnActor));
	    if(symType!="Actor") {
		continue;	// no, it is not
	    }
	}
	// yes, it is external event
	if(aScenario==NULL) {
	    print_error("Diagram has External events, but does not have a parent Use Case");
	    return 1;
	}
	// this is an external event.  Check if it has a corresponding communicates link
	nodes = gde_node_oms_nodes(gnActor);
	omsActor = list_get(nodes,0);
	commLink = find_by_query("link[UmlUseCaseInteraction && link_refs && from_node_id=${omsActor.id} & to_"+qFindUseCase+"]");
	if(commLink==NULL) {
	    errCount++;
	    gde_print_error("External event "+gde_arc_label(aMessage)+"' does not have a corresponding communicates link",gde_arc_psymid(aMessage));
	}
    }
    return errCount;
}

int
usequenced_dg_completeness_check_messages()
{
    const string messageNames = "SimpleMessage SynchronousMessage BalkingMessage TimeoutMessage AsynchronousMessage";
    print_message("Checking Messages ...");
    // get all messages
    if(allMessageLinkTypes==NULL)
	allMessageLinkTypes = string_to_list(messageNames," ");
    int ii;
    string symTypeName;
    list allMessages = gde_arcs_find_by_type(gde_all_symbols(),
					     list_get(allMessageLinkTypes,0));
    for(ii=1; ii<list_count(allMessageLinkTypes); ii++) {
	symTypeName = list_get(allMessageLinkTypes,ii);
	list_concatenate(allMessages,
			 gde_arcs_find_by_type(gde_all_symbols(),symTypeName));
    }

    gde_arc aMessage;
    gde_node toNode;
    string symType;
    list nodes;
    node omsActor;
    link commLink;
    int errCount = 0;
    for(ii=0; ii<list_count(allMessages); ii++) {
	aMessage = list_get(allMessages,ii);
	toNode = gde_arc_tonode(aMessage);
	symType = gde_get_symbol_type(to_gde_symbol(toNode));
	if(symType=="Actor") {
	    continue;
	} else {
	    errCount += usequenced_check_single_message(aMessage);
	}
    }
    return errCount;
}

int
usequenced_check_single_message(gde_arc mssg)
{
    int errCount = 0;
    boolean found = False;
    int count, ii, opArgsCnt;
    string opSig;
    node anOp;
    const string qFindOperation = "node[UmlOperation & node_refs & name='${mssgName}']";
    list gdeArcLinks = gde_arc_links(mssg);
    //print_arc("checking arc "+ii+":",gArc);
    list links = gde_link_oms_links(list_get(gdeArcLinks,0));
    if(list_count(links) == 0) {
	errCount++;
	gde_print_error("Message '"+gde_arc_label(mssg)+"' does not have mapped object",gde_arc_psymid(mssg));
    } else {
	link theLink = list_get(links,0);
	string retType, mssgArgs, mssgName;
	item ReturnType = find_by_query("item[UmlReturnType & obj_id=${theLink.id}]");
	item MessageArgs = find_by_query("item[UmlMessageArgs & obj_id=${theLink.id}]");
	item MessageName = find_by_query("item[UmlObjectName & obj_id=${theLink.id}]");
	retType = ReturnType.value;
	mssgArgs = MessageArgs.value;
	mssgName = MessageName.value;
	int argCount = list_count(string_to_list(mssgArgs,","));
	list operations = list_select(qFindOperation);
	//found = False;
	if(list_count(operations)==0) {//{if(anOp==NULL)
	    errCount++;
	    gde_print_error("Message '"+gde_arc_label(mssg)+"' does not have a corresponding Operation defined",gde_arc_psymid(mssg));
	} else {
	    count = list_count(operations);
	    for(ii=0; ii<count && !found; ii++) {
		// check arguments and the owner/class
		// check op signature and count arguments 
		anOp = list_get(operations,ii);
		opSig = anOp.sig;
		opArgsCnt = list_count(string_to_list(opSig,","));
		if(opArgsCnt==argCount) {
		    found = True;
		    break;
		}
	    }
	    if(!found) {
		errCount++;
		gde_print_error("Message '"+gde_arc_label(mssg)+"' does not have a corresponding Operation defined, with the same number of arguments ("+argCount+")",gde_arc_psymid(mssg));
	    }
	}
    }
    return errCount;
}
