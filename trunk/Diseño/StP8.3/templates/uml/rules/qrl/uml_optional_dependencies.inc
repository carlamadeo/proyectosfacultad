//
//      StP/UML
//      Confidential property of Aonix
//      Copyright (c) 1997
//      All rights reserved
//

// Optional dependency variable declaration
// Dependent link objects of type SimpleMessage
// Dependent link objects of type SynchronousMessage
// Dependent link objects of type BalkingMessage
// Dependent link objects of type TimeoutMessage
// Dependent link objects of type AsynchronousMessage
// Dependency name: Name
boolean dep_UmlEventLink_Message_Name = False;

// Optional dependency variable declaration
// Dependent node objects of type UmlEvent
// Dependency name: TransitionEvent
boolean dep_UmlTransition_UmlEvent_TransitionEvent = False;

// Optional dependency variable declaration
// Dependent link objects of type UmlTransition
// Dependency name: TransitionInSameCntx
boolean dep_UmlTransition_UmlTransition_TransitionInSameCntx = False;

// Optional dependency variable declaration
// Dependent node objects of type UmlOperation
// Dependency name: OperationOperation
boolean dep_UmlOperation_UmlOperation_OpOverloaded = False;


// Optional dependency variable declaration
// Dependent node objects of type UmlOperation
// Dependency name: OpConstructor
boolean dep_UmlClass_UmlOperation_OpConstructor = False;


// Optional dependency variable declaration
// Dependent node objects of type file
// Dependency name: UseCaseScenarioFile
boolean dep_UmlUseCase_UmlScenarioInstance_FileName = False;


// Optional dependency variable declaration
// Dependent link objects of type UmlSimpleMessage
// Dependent link objects of type UmlSynchronousMessage
// Dependent link objects of type UmlBalkingMessage
// Dependent link objects of type UmlTimeoutMessage
// Dependent link objects of type UmlAsynchronousMessage
// Dependency name: UmlMessage_UmlMessage
boolean dep_UmlMessage_UmlMessage_sameSignature = False;
//boolean dep_UmlMessage_UmlMessage_diffSignature = False;

// Optional dependency variable declaration
// Dependent node objects of type UmlOperation and UmlSignal
// Dependency name: UmlMessage_UmlOperation
boolean dep_UmlMessage_UmlOperationUmlSignal = False;

//--- START ECR3829
// Optional dependency variable declaration
// Dependent node objects of type node
// Dependency name: ActionStateName
boolean dep_UmlUseCase_UmlActionState_Name = True;
//--- END  ECR3829

// ECR 4052
// set defaults, common for Model Management and multi gor
void
set_multi_gor_defaults()
{
    dep_UmlClass_UmlOperation_OpConstructor = True;
    dep_UmlOperation_UmlOperation_OpOverloaded = True;
    dep_UmlUseCase_UmlScenarioInstance_FileName = True;
}

// ECR 7318 moved from uml/rules/qrl/uml.inc
const string UML_PATH_SEPARATOR = "::";

// ECR 7318 moved from uml/rules/qrl/state_machine.inc
string uml_state_machine_name(node operation)
{
    if(operation == NULL)
        return NULL;

    node class = find_by_query("node[id = ${operation.scope_node_id}]");
    if(class == NULL)
        return NULL;
    
    return class.name + UML_PATH_SEPARATOR + operation.name + "(" + operation.sig + ")";
}

string uml_state_machine_class_name(string state_machine)
{
    if(state_machine == "" || state_machine == NULL)
        return state_machine;

    if(uml_state_machine_is_class(state_machine))
        return state_machine;

    return string_extract(state_machine, 0, string_find(state_machine, 0, UML_PATH_SEPARATOR));

}

boolean uml_state_machine_is_class(string state_machine)
{
    if(state_machine == "" || state_machine == NULL)
        return False;
    return (string_find(state_machine, 0, UML_PATH_SEPARATOR) == string_length(state_machine));
}
// ECR 7318 end