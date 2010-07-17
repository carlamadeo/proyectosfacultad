list doorsExportableNodeTypes = NULL;
list doorsExportableCntxTypes = NULL;
list doorsExportableLinkTypes = NULL;


list
doors_exportable_node_types()
{
	if(doorsExportableNodeTypes == NULL)
	{
		doorsExportableNodeTypes = list_create("string",0);
		list_append(doorsExportableNodeTypes,"External");
		list_append(doorsExportableNodeTypes, "Process");
		list_append(doorsExportableNodeTypes, "OffPageProcess");
		list_append(doorsExportableNodeTypes, "OffPageExternal");
		list_append(doorsExportableNodeTypes, "Store");
		list_append(doorsExportableNodeTypes, "Cspec");
		list_append(doorsExportableNodeTypes,"Sequence");
		list_append(doorsExportableNodeTypes, "Selection");
		list_append(doorsExportableNodeTypes, "Enumeration");
		list_append(doorsExportableNodeTypes, "Typedef");
		list_append(doorsExportableNodeTypes,"State");
		list_append(doorsExportableNodeTypes, "Event");
		list_append(doorsExportableNodeTypes,"Module");
		list_append(doorsExportableNodeTypes, "LibraryModule");
		list_append(doorsExportableNodeTypes, "DataModule");


  	}
	return doorsExportableNodeTypes;
}


list
doors_exportable_link_types()
{
	if(doorsExportableLinkTypes == NULL)
	{
		doorsExportableLinkTypes = list_create("string",0);
		list_append(doorsExportableLinkTypes, "ControlFlow");
		list_append(doorsExportableLinkTypes, "DataFlow");

  	}
	return doorsExportableLinkTypes;
}


list
doors_exportable_cntx_types()
{
	if(doorsExportableCntxTypes == NULL)
	{
		doorsExportableCntxTypes = list_create("string",0);
		list_append(doorsExportableCntxTypes, "DataIn");
		list_append(doorsExportableCntxTypes, "DataInOut");
		list_append(doorsExportableCntxTypes, "DataOut");
		list_append(doorsExportableCntxTypes, "ControlIn");
		list_append(doorsExportableCntxTypes, "ControlOut");

  	}
	return doorsExportableCntxTypes;
}
