list doorsExportableNodeTypes = NULL;
list doorsExportableCntxTypes = NULL;
list doorsExportableLinkTypes = NULL;


list
doors_exportable_node_types()
{
  if(doorsExportableNodeTypes == NULL)
  {
    doorsExportableNodeTypes = list_create("string",0);
    list_append(doorsExportableNodeTypes,"UmlState");
   list_append(doorsExportableNodeTypes,"UmlActionState");
    list_append(doorsExportableNodeTypes, "UmlInitialState");
    list_append(doorsExportableNodeTypes, "UmlFinalState");
    list_append(doorsExportableNodeTypes, "UmlStateMachine");
    list_append(doorsExportableNodeTypes, "UmlSplitControl");
    list_append(doorsExportableNodeTypes, "UmlMergeControl");
    list_append(doorsExportableNodeTypes, "UmlHistoryState");
    list_append(doorsExportableNodeTypes, "UmlActor");
    list_append(doorsExportableNodeTypes, "UmlCollaboration");
    list_append(doorsExportableNodeTypes, "UmlObjectInstance");
    list_append(doorsExportableNodeTypes, "UmlPackage");
    list_append(doorsExportableNodeTypes,
                "UmlObjectInstanceAttribute");
    list_append(doorsExportableNodeTypes, "UmlScenarioInstance");
    list_append(doorsExportableNodeTypes, "UmlExtensionPoint");		
    list_append(doorsExportableNodeTypes, "UmlClass");
    list_append(doorsExportableNodeTypes, "UmlSystem");
    list_append(doorsExportableNodeTypes, "UmlComponentExecutable");
    list_append(doorsExportableNodeTypes, "UmlComponentBinary");
    list_append(doorsExportableNodeTypes, "UmlComponentSource");
    list_append(doorsExportableNodeTypes, "UmlComponentInterface");
    list_append(doorsExportableNodeTypes, "UmlComponentObject");
    list_append(doorsExportableNodeTypes, "UmlDeploymentNode");
    list_append(doorsExportableNodeTypes, "UmlStereotype");
    list_append(doorsExportableNodeTypes, "UmlUseCase");
    list_append(doorsExportableNodeTypes, "UmlOperation");
    list_append(doorsExportableNodeTypes, "UmlAttribute");
    list_append(doorsExportableNodeTypes, "UmlAssociation");
    list_append(doorsExportableNodeTypes, "UmlInstantiatedClass");
    
  	}
  return doorsExportableNodeTypes;
}


list
doors_exportable_link_types()
{
  if(doorsExportableLinkTypes == NULL)
  {
    doorsExportableLinkTypes = list_create("string",0);
    list_append(doorsExportableLinkTypes, "UmlTransition");		
    list_append(doorsExportableLinkTypes, "UmlContains");
    list_append(doorsExportableLinkTypes, "UmlObjectLink");
    list_append(doorsExportableLinkTypes, "UmlCollaborationRole");
    list_append(doorsExportableLinkTypes, "UmlAsynchronousMessage");
    list_append(doorsExportableLinkTypes, "UmlSynchronousMessage");
    list_append(doorsExportableLinkTypes, "UmlBalkingMessage");
    list_append(doorsExportableLinkTypes, "UmlSimpleMessage");
    list_append(doorsExportableLinkTypes, "UmlTimeoutMessage");
    list_append(doorsExportableLinkTypes, "UmlReturnMessage");
    list_append(doorsExportableLinkTypes, "UmlInput");
    list_append(doorsExportableLinkTypes, "UmlOutput");
    list_append(doorsExportableLinkTypes, "UmlComponentComposit");
    list_append(doorsExportableLinkTypes, "UmlComponentDependency");
    list_append(doorsExportableLinkTypes, "UmlDependency");
    list_append(doorsExportableLinkTypes, "UmlDeployment");
    list_append(doorsExportableLinkTypes, "UmlComponentInterface");
    list_append(doorsExportableLinkTypes, "UmlUseCaseInteraction");
    list_append(doorsExportableLinkTypes, "UmlUseCaseExtends");
    list_append(doorsExportableLinkTypes, "UmlUseCaseUses");
    list_append(doorsExportableLinkTypes, "UmlGeneralization");
    list_append(doorsExportableLinkTypes, "UmlImplements");
    list_append(doorsExportableLinkTypes, "UmlRefines");
    list_append(doorsExportableLinkTypes, "UmlRole");

  }
  return doorsExportableLinkTypes;
}


list
doors_exportable_cntx_types()
{
  if(doorsExportableCntxTypes == NULL)
  {
    doorsExportableCntxTypes = list_create("string",0);
    list_append(doorsExportableCntxTypes, "UmlTimeSent");
    list_append(doorsExportableCntxTypes, "UmlTimeDelivered");
  }
  return doorsExportableCntxTypes;
}
