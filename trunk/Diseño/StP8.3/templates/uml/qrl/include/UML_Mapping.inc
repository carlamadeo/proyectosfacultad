//*****************************************************************************
// DESCRIPTION: UML_Mapping.inc
//              ----------------------
//              (c) 1997 by AONIX
//*****************************************************************************
//
// Include-file for uml reports with the mapping of UML application types   
//*****************************************************************************

//-----------------------------------------------------------------------------
// Requirements
//-----------------------------------------------------------------------------

const string  m_REQ_FILE               = "REQTable";            
const string  m_REQ_NODE               = "REQRequirement";      
const string  m_REQ_PARENT_LINK        = "ReqParent";           
const string  m_REQ_DEFINITION_NOTE    = "REQDefinitionNote";      
const string  m_REQ_SOURCE_NOTE        = "REQSourceNote";      
const string  m_REQ_ANALYSIS_ASS_ITEM  = "REQAnalysisAssignmentItem";     
const string  m_REQ_DESIGN_ASS_ITEM    = "REQDesignAssignmentItem";     
const string  m_REQ_IMPL_ASS_ITEM      = "REQImplementationAssignmentItem";     
const string  m_REQ_TEST_ASS_ITEM      = "REQTestAssignmentItem";  
const string  m_REQ_INF_NAME_ITEM      = "REQInformalNameItem";  

//-----------------------------------------------------------------------------
// System and Packages
//-----------------------------------------------------------------------------
const string  m_SYSTEM_NODE            = "UmlSystem";            
const string  m_PACKAGE_NODE           = "UmlPackage";            
const string  m_CONTAINS_LINK          = "UmlContains";    
const string  m_ALL_MEMBERS_VP         = "UmlAllMembers";        

//-----------------------------------------------------------------------------
// Stereotype and other Information
//-----------------------------------------------------------------------------
const string  m_STEREOTYPE_FILE        = "UmlStereotypeDiagram";            
const string  m_STEREOTYPE_NODE        = "UmlStereotype";            
const string  m_STEREOTYPE_INH_LINK    = "UmlStereotypeInheritance";            
   
//-----------------------------------------------------------------------------
// Use Cases
//-----------------------------------------------------------------------------
const string  m_USECASE_FILE           = "UmlUseCaseDiagram";            
const string  m_USECASE_NODE           = "UmlUseCase";            
const string  m_ACTOR_NODE             = "UmlActor"; 
const string  m_UC_INTERACTION_LINK    = "UmlUseCaseInteraction";           
const string  m_UC_EXTENDS_LINK        = "UmlUseCaseExtends";           
const string  m_UC_USES_LINK           = "UmlUseCaseUses";           
const string  m_USECASE_DEF_NOTE       = "UmlUseCaseDefinition";            
const string  m_UC_PRECONDITION_ITEM   = "UmlUseCasePrecondition";            
const string  m_UC_POSTCONDITION_ITEM  = "UmlUseCasePostcondition";            
const string  m_UC_EXCEPTION_ITEM      = "UmlUseCaseException";            

//-----------------------------------------------------------------------------
// Sequence and Collaboration
//-----------------------------------------------------------------------------
const string  m_SEQUENCE_FILE          = "UmlSequenceDiagram";            
const string  m_COLLABORATION_FILE     = "UmlCollaborationDiagram";            
const string  m_SCENARIO_NODE          = "UmlScenarioInstance";
const string  m_EXTENSION_NODE         = "UmlExtensionPoint";
const string  m_COLABORATION_NODE      = "UmlCollaboration";
const string  m_TYPE_NODE              = "UmlType";
const string  m_OBJECT_NODE            = "UmlObjectInstance";
const string  m_ACTIVE_OBJECT_NODE     = "UmlActiveObject";
const string  m_OBJECT_CLASS_NODE      = "UmlObjectClassScope";
const string  m_SIMPLE_MESSAGE_LINK    = "UmlSimpleMessage";
const string  m_SYNCHR_MESSAGE_LINK    = "UmlSynchronousMessage";
const string  m_ASYNCHR_MESSAGE_LINK   = "UmlAsynchronousMessage";
const string  m_BALKING_MESSAGE_LINK   = "UmlBalkingMessage";
const string  m_TIMEOUT_MESSAGE_LINK   = "UmlTimeoutMessage";
const string  m_RETURN_MESSAGE_LINK    = "UmlReturnMessage";
const string  m_PARENT_NAME_ITEM       = "UmlParentNameItem";            
const string  m_PARENT_TYPE_ITEM       = "UmlParentTypeItem";            

//-----------------------------------------------------------------------------
// State Diagrams
//-----------------------------------------------------------------------------
const string  m_STATE_DIAGRAM          = "UmlStateDiagram";      
const string  m_ACTIVITY_FILE          = "UmlActivityDiagram"; 
const string  m_STATE_MACHINE_NODE     = "UmlStateMachine";
const string  m_STATE_NODE             = "UmlState";     
const string  m_INIT_STATE_NODE        = "UmlInitialState";     
const string  m_ACTION_STATE_NODE      = "UmlActionState";     
const string  m_FINAL_STATE_NODE       = "UmlFinalState";     
const string  m_HISTORY_STATE_NODE     = "UmlHistoryState";     
const string  m_MERGE_CTRL_NODE        = "UmlMergeControl";     
const string  m_SPLIT_CTRL_NODE        = "UmlSplitControl";     
const string  m_TRANSITION_LINK        = "UmlTransition";     
// ECR 7215
const string  m_JUNCTION_POINT         = "UmlJunctionPoint";
const string  m_DYNAMIC_CHOICE_POINT   = "UmlDynamicChoicePoint";

//-----------------------------------------------------------------------------
// Classes
//-----------------------------------------------------------------------------
const string  m_CLASS_FILE             = "UmlClassDiagram";            
const string  m_CLASS_TABLE            = "UmlClassTable";            
const string  m_CLASS_NODE             = "UmlClass";            
const string  m_ATTRIBUTE_NODE         = "UmlAttribute";            
const string  m_OPERATION_NODE         = "UmlOperation";            
const string  m_GENERALIZATION_LINK    = "UmlGeneralization";            
const string  m_DEPENENCY_LINK         = "UmlDependency";            
const string  m_IMPLEMENTS_LINK        = "UmlImplements";            
const string  m_REFINES_LINK           = "UmlRefines";   
const string  m_CLASS_DEF_NOTE         = "UmlClassDefinition";         
const string  m_AT_DEF_NOTE            = "UmlAttributeDefinition";         
const string  m_AT_TYPE_ITEM           = "UmlAttributeType";            
const string  m_OP_DEF_NOTE            = "UmlOperationDefinition";         
const string  m_CL_VISIBILITY_ITEM     = "UmlClassVisibility";            
const string  m_CL_PARAMETERS_ITEM     = "UmlClassParameters";            
const string  m_CL_IS_ABSTRACT_ITEM    = "UmlClassIsAbstract";            
const string  m_CL_IS_IMPORTED_ITEM    = "UmlClassIsImported";            
const string  m_AT_DEFAULT_VALUE_ITEM  = "UmlAttributeDefaultValue";            
const string  m_AT_IS_DERIVED_ITEM     = "UmlAttributeIsDerived";            
const string  m_AT_CXX_VOLATILE_ITEM   = "UmlAttributeCxxIsVolatile";            
const string  m_AT_JAV_TRANS_ITEM      = "UmlAttributeJavaIsTransient";            
const string  m_AT_JAV_VOLATILE_ITEM   = "UmlAttributeJavaIsVolatile";            
const string  m_AT_ADA_ALIAS_ITEM      = "UmlAttributeAda95IsAliased";            
const string  m_AT_IDL_TYPE_ITEM       = "UmlAttributeIDLType";            
const string  m_AT_IDL_IS_RO_ITEM      = "UmlAttributeIDLIsReadOnly";            
const string  m_OP_RETURN_TYPE_ITEM    = "UmlOperationReturnType";            
const string  m_OP_IS_ABSTRACT_ITEM    = "UmlOperationIsAbstract";            
const string  m_OP_THROWS_ITEM         = "UmlOperationThrows";            
const string  m_OP_CXX_VIRTUAL_ITEM    = "UmlOperationCxxIsVirtual";            
const string  m_OP_CXX_INLINE_ITEM     = "UmlOperationCxxIsInline";            
const string  m_OP_CXX_CTILIST_ITEM    = "UmlOperationCxxCtorInitList";            
const string  m_OP_JAV_NATIVE_ITEM     = "UmlOperationJavaIsNative";            
const string  m_OP_JAV_SYNCH_ITEM      = "UmlOperationJavaIsSynchronized";            
const string  m_OP_ADA_SUBUNIT_ITEM    = "UmlOperationAda95IsSubunit";            
const string  m_OP_ADA_INLINE_ITEM     = "UmlOperationAda95IsInline";            
const string  m_OP_IDL_SIGNATURE_ITEM  = "UmlOperationIDLSignature";            
const string  m_OP_IDL_RTYPE_ITEM      = "UmlOperationIDLReturnType";            
const string  m_OP_IDL_ATTRIBUTE_ITEM  = "UmlOperationIDLAttribute";            
const string  m_OP_IDL_CONTEXT_ITEM    = "UmlOperationIDLContext";            
const string  m_MB_VISIBILITY_ITEM     = "UmlMemberVisibility";            
const string  m_MB_IS_CLASS_ITEM       = "UmlMemberIsClass";            
const string  m_MB_CXX_CONST_ITEM      = "UmlMemberCxxIsConst";            
const string  m_MB_CXX_VISIB_ITEM      = "UmlMemberCxxVisibility";            
const string  m_MB_JAV_VISIB_ITEM      = "UmlMemberJavaVisibility";            
const string  m_MB_JAV_FINAL_ITEM      = "UmlMemberJavaIsFinal";            
const string  m_MB_ADA_VISIB_ITEM      = "UmlMemberAda95Visibility";            
const string  m_ENC_SCOPE_ITEM         = "UmlEnclosingScope";            
const string  m_MULTIPLICITY_ITEM      = "UmlMultiplicity";            

//-----------------------------------------------------------------------------
// Implementation Diagrams
//-----------------------------------------------------------------------------
const string  m_COMPONENT_FILE         = "UmlComponentDiagram";            
const string  m_DEPLOYMENT_FILE        = "UmlDeploymentDiagram";  
const string  m_DEPLOYMENT_NODE        = "UmlDeploymentNode";          
const string  m_COMPONENT_IF_NODE      = "UmlComponentInterface";          
const string  m_COMPONENT_BIN_NODE     = "UmlComponentBinary";          
const string  m_COMPONENT_EX_NODE      = "UmlComponentExecutable";          
const string  m_COMPONENT_SRC_NODE     = "UmlComponentSource";          
const string  m_COMPONENT_OBJECT_NODE  = "UmlComponentObject";          

//-----------------------------------------------------------------------------
// Glossary
//-----------------------------------------------------------------------------
const string  m_GLOSSARY_NOTE          = "GlossaryDefinition";      
const string  m_GLOSSARY_ITEM          = "GlossaryItem";      

//-----------------------------------------------------------------------------
// OTHERS
//-----------------------------------------------------------------------------
const string  m_EXTENSIBILITY_NOTE     = "UmlExtensibilityDefinition";            
const string  m_STEREOTYPE_ITEM        = "UmlStereotype";            
const string  m_CONSTRAINT_ITEM        = "UmlConstraintItem";            
const string  m_TAGGED_VALUE_ITEM      = "UmlTaggedValue";            
const string  m_AUTHOR                 = "Author";      
const string  m_GENERATED              = "Generated";      
