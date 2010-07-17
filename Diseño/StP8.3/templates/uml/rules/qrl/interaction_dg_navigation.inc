
//      StP/UML
//      Confidential property of Aonix
//      Copyright (c) 1997
//      All rights reserved
//

// Begin Add ECR 6633
void UmlClassTableInitializeFromSeqCollab( int nodeId ) {
    node class_scope = find_by_query( "node[UmlObjectClassScope && id==${nodeId}]" );
    if( class_scope != NULL ) {
        if( string_find( class_scope.name, 0, "%%class%%" ) < string_length( class_scope.name ) )
            print_error( "Can't create Class Table for an anonymous Object/Actor !" );
        else
            NavigateSendQrl("uclasst","UmlClassTableInitializeFromSeqCollab(" + nodeId + ");");
    }
    return;
}
// End Add ECR 6633

list FindClassDiagramWithCorrespondingClass(int nodeId)
{
    //node thisNode = find_by_query("node[${nodeId}]");
    //node objClassScope = find_by_query("node[id=${thisNode.scope_node_id}]");
    node objClassScope = find_by_query("node[${nodeId}]");
    string qFindClassRefs = "node_ref[file[UmlClassDiagram] && node[UmlClass && name='${objClassScope.name}']]";
    list result = list_select(qFindClassRefs);
    if(list_count(result)==0)
	print_message("Target (Class Diagram Where The Class Is Referenced) not found");
    return(result);
}

list FindStateDiagramWithCorrespondingClass(int nodeId)
{
    node objClassScope = find_by_query("node[${nodeId}]");
    // this would assume that state dg has explicit ref to a class ??
    //string qFindClassRefs = "node_ref[file[UmlStateDiagram] && node[UmlClass && name=${objClassScope.name}]]";
    //string qFindClassRefs = "node_ref[file[UmlStateDiagram] && node[id=${objClassScope.id}]]";
    //list result = list_select(qFindClassRefs);
    //
    // check if there is a state machine with this name
    string qFindNamedStateMachine = "node_ref[file[UmlStateDiagram] && node[UmlStateMachine && name='${objClassScope.name}']]";
    list result = list_select(qFindNamedStateMachine);
    if(list_count(result)==0)
	print_message("Target (State Diagram Where The Class Is Referenced) not found");
    return(result);
}

list FindActivityDiagramWithCorrespondingClass(int nodeId)
{
    node objClassScope = find_by_query("node[${nodeId}]");
    // check if there is a state machine with this name
    string qFindNamedStateMachine = "node_ref[file[UmlActivityDiagram] && node[UmlStateMachine && name='${objClassScope.name}']]";
    list result = list_select(qFindNamedStateMachine);
    if(list_count(result)==0)
	print_message("Target (Activity Diagram Where The Class Is Referenced) not found");
    return(result);
}

list FindScenarioParent(int nodeId)
{
    list result = NULL;
    boolean found = True;
    node scen = find_by_query("node[${nodeId}]");
    string qFindParentTypeItemForScenario = "item[UmlParentTypeItem && obj_id == ${scen.id}]";
    string qFindParentNameItemForScenario = "item[UmlParentNameItem && obj_id == ${scen.id}]";
    item ptItem = find_by_query(qFindParentTypeItemForScenario);
    item pnItem = find_by_query(qFindParentNameItemForScenario);
    if(ptItem==NULL || pnItem==NULL || ptItem.value=="") {
	found = False;
    } else {
	string qFindAllRefs = "node_ref[node[type=${ptItem.value} && name='${pnItem.value}']]";
	result = list_select(qFindAllRefs);
	if(list_count(result)==0)
	    found = False;
    }
    if(!found) {
	print_message("Target (parent diagram) not found");
	return NULL;
    } else 
        return(result);
}

list FindScenarioSibling(int nodeId)
{
    list result = NULL;
    boolean found = True;
    node scen = find_by_query("node[${nodeId}]");
    string qFindParentTypeItemForScenario = "item[UmlParentTypeItem && obj_id == ${scen.id}]";
    string qFindParentNameItemForScenario = "item[UmlParentNameItem && obj_id == ${scen.id}]";
    item ptItem = find_by_query(qFindParentTypeItemForScenario);
    item pnItem = find_by_query(qFindParentNameItemForScenario);
    if(ptItem==NULL || pnItem==NULL || ptItem.value=="") {
	found = False;
    } else {
	//string qFindClassRefs = "node_ref[file[UmlActivityDiagram] && node[UmlClass && name=${objClassScope.name}]]";
	string qFindAllRefs = "node_ref[file[UmlSequenceDiagram || UmlCollaborationDiagram] && node[UmlScenarioInstance && items[UmlParentTypeItem && value='${ptItem.value}'] && items[UmlParentNameItem && value='${pnItem.value}']]]";
	result = list_select(qFindAllRefs);
	if(list_count(result)==0)
	    found = False;
    }
    if(!found) {
	print_message("Target (sibling diagram) not found");
	return NULL;
    } else 
	return(result);
}

list FindMessageOperation(int message_id)
{
    node classScopeNode;
    string className;
    list opRefs = list_create("node_ref",0);
    item messageNameItem;
    item messageArgsItem;
    item messageRetTypeItem;
    item opRetTypeItem;
    node opNode;
    int nArgs;
    list opArgs;
    boolean argsMatch = False;
    boolean retTypesMatch = False;
    
    classScopeNode = 
    find_by_query("node[scoped_nodes[in_links[${message_id}]]]");
    
    if(classScopeNode == NULL)
        return opRefs;
    
    className = classScopeNode.name;
    
    messageNameItem = 
    find_by_query("item[UmlObjectName && obj_id = ${message_id}]");
    
    if(messageNameItem == NULL || messageNameItem.value == "" || 
       messageNameItem.value == NULL)
        return opRefs;
    
    messageArgsItem = 
    find_by_query("item[UmlMessageArgs && obj_id = ${message_id}]");
    messageRetTypeItem = 
    find_by_query("item[UmlReturnType && obj_id = ${message_id}]");
    
    for_each_in_select("node[UmlOperation && 
name = '${messageNameItem.value}' && 
scope_node[name = 
'${className}']]",opNode)
    {
        opArgs = list_create("string",0);
        nArgs = -1;
        if (uml_split_arguments(opNode.sig,opArgs)) 
            nArgs = list_count(opArgs);
        
        //no arguments
        argsMatch = (NULL == messageArgsItem && nArgs == 0);
        
        //or same number of arguments
        if(!argsMatch && messageArgsItem != NULL)
            argsMatch = list_count(string_to_list(messageArgsItem.value,
                                                  ",")) == nArgs;
        
        if(!argsMatch)
            continue;
        
        retTypesMatch = False;
        
        opRetTypeItem = find_by_query("item[UmlOperationReturnType &
obj_id = ${opNode.id}]");
        
        if((opRetTypeItem == NULL || opRetTypeItem.value == "")
           && (messageRetTypeItem == NULL || 
               messageRetTypeItem.value == ""))
            retTypesMatch = True;
        else if((opRetTypeItem != NULL && messageRetTypeItem != NULL) &&
                (opRetTypeItem.value == messageRetTypeItem.value))
            retTypesMatch = True;
        
        if(retTypesMatch)
            list_concatenate(opRefs,list_select("node_ref[node_id =
${opNode.id}]"));
        
    }
    return opRefs;
}

list FindMessageStateMachine(int message_id, string fileType)
{
    node classScopeNode;
    string className;
    string stateMachineWildcard;
    list stateMachineRefs = list_create("node_ref",0);
    item messageNameItem;
    item messageArgsItem;
    node stateMachineNode;
    uml_operation parsedOperation;
    int nArgs;
    list opArgs;
    boolean argsMatch = False;
    
    classScopeNode = 
    find_by_query("node[scoped_nodes[in_links[${message_id}]]]");
    
    if(classScopeNode == NULL)
        return stateMachineRefs;
    
    className = classScopeNode.name;
    
    messageNameItem = 
    find_by_query("item[UmlObjectName && obj_id = ${message_id}]");
    
    if(messageNameItem == NULL || messageNameItem.value == "" || 
       messageNameItem.value == NULL)
        return stateMachineRefs;
    
    messageArgsItem = 
    find_by_query("item[UmlMessageArgs && obj_id = ${message_id}]");
    
    
    stateMachineWildcard = className + UML_PATH_SEPARATOR + 
    messageNameItem.value + "(*";
    for_each_in_select("node[UmlStateMachine && 
                           node_refs[file[${fileType}]] && 
                           name $ '${stateMachineWildcard}']",
                       stateMachineNode)
    {
        opArgs = list_create("string",0);
        nArgs = -1;
        parsedOperation = 
        uml_state_machine_parse_operation(stateMachineNode.name);
        if (uml_split_arguments(parsedOperation.opArgs,opArgs)) 
            nArgs = list_count(opArgs);
        
        //no arguments
        argsMatch = (NULL == messageArgsItem && nArgs == 0);
        
        //or same number of arguments
        if(!argsMatch && messageArgsItem != NULL)
            argsMatch = list_count(string_to_list(messageArgsItem.value,
                                                  ",")) == nArgs;
        
        if(argsMatch)
            list_concatenate(stateMachineRefs,
                list_select("node_ref[node[${stateMachineNode.id}] &&
file[${fileType}]]"));
    }

    return stateMachineRefs;
}

list FindMessageSignal(int message_id,string direction)
{
    node classNode;
    list sigRefs = list_create("node_ref",0);
    item messageNameItem;
    item messageArgsItem;
    node sigNode;
    int nArgs;
    list sigArgs;
    boolean argsMatch = False;

    messageNameItem = 
    find_by_query("item[UmlObjectName && obj_id = ${message_id}]");

    if(messageNameItem == NULL || messageNameItem.value == "" || 
       messageNameItem.value == NULL)
        return sigRefs;

    if(direction == "UmlSignalReceives")
        classNode = 
        find_by_query("node[scoped_nodes[in_links[${message_id}]]]");
    else
        classNode = 
        find_by_query("node[scoped_nodes[out_links[${message_id}]]]");

    if(classNode == NULL)
        return sigRefs;

    messageArgsItem = 
    find_by_query("item[UmlMessageArgs && obj_id = ${message_id}]");

    if(classNode != NULL)
    {
        for_each_in_select("node[UmlSignal && 
                            name = '${messageNameItem.value}' && 
                            in_links[${direction} && 
                            from_node[name = '${classNode.name}']]]"
                           ,sigNode)
        {
            sigArgs = list_create("string",0);
            nArgs = -1;
            if (uml_split_arguments(sigNode.sig,sigArgs)) 
                nArgs = list_count(sigArgs);
            
            //no arguments
            argsMatch = (NULL == messageArgsItem && nArgs == 0);
            
            //or same number of arguments
            if(!argsMatch && messageArgsItem != NULL)
                argsMatch = list_count(string_to_list(messageArgsItem.value,
                                                      ",")) == nArgs;
            
            if(argsMatch)
                list_concatenate(sigRefs,list_select("node_ref[node_id =
                                                     ${sigNode.id}]"));
            
        }
    }
    
    return sigRefs;
}

void FindOrCreateScenarioForCollaborationFromRole(int linkId)
{
    link roleLink = find_by_query("link[id=${linkId}]");
    FindOrCreateScenarioForCollaboration(roleLink.from_node_id);
}




