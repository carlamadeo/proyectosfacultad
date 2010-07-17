//      StP/UML
//      Confidential property of Aonix
//      Copyright (c) 1997
//      All rights reserved
//
//
// Editor independent state machine functions

#include "rules/qrl/oper_parse.inc"

string
uml_state_or_act_nav_query(int target_id, string fileType)
{
	string sm_name;
	node target = find_by_query("node[${target_id}]");

	if ("UmlClass" == target.type) {
		sm_name = target.name;
	} else {
		sm_name = uml_state_machine_name(target);
	}
	string qry = "node_ref[node[UmlStateMachine && name ='" + sm_name + 
		"'] && file[" + fileType + "] ]";
	return qry;
}

list
uml_state_or_act_nav_targets(int target_id, string fileType)
{
	string qry = uml_state_or_act_nav_query(target_id,fileType);
	list targets;
	targets = list_select(qry);
	return targets;
}

// ECR 7318 moved uml_state_machine_name()
// and uml_state_machine_class_name()
// and uml_state_machine_is_class()
// to uml/rules/qrl/uml_optional_dependencies.inc
#include "rules/qrl/uml_optional_dependencies.inc"

//The following function prints errors and returns NULL
//if the state_machine doesn't parse correctly.
uml_operation
uml_state_machine_parse_operation(string state_machine)
{
    string operation_name_sig = NULL;
    uml_operation parsed_operation;
    int pos;
    
    if(state_machine == "" || state_machine == NULL)
	return NULL;
    
    pos = string_find(state_machine,0,UML_PATH_SEPARATOR);
    pos = pos + string_length(UML_PATH_SEPARATOR);
    if(pos >= string_length(state_machine))
	{
	    print_error("State Machine name has illegal format.");
	    return NULL;
	}
    operation_name_sig = 
	string_extract(state_machine,pos,
		       string_length(state_machine) - pos);
    
    if(string_find(operation_name_sig,0,UML_PATH_SEPARATOR) !=
       string_length(operation_name_sig))
	{
	    print_error("State Machine name " +state_machine + 
			" has too many path components.");
	    return NULL; 
	}
    parsed_operation = 
	uml_parse_operation(operation_name_sig);
    if(parsed_operation.error)
	    return NULL;
    
    return parsed_operation;
}

string
uml_state_machine_canonical_operation(string state_machine)
{
    string operation_name_sig = NULL;
    uml_operation parsed_operation;

    parsed_operation = uml_state_machine_parse_operation(state_machine);
    
    if(parsed_operation == NULL)
        return NULL;

    operation_name_sig = parsed_operation.opName + "(" + 
	parsed_operation.opArgs + ")";
    
    return operation_name_sig;
}






