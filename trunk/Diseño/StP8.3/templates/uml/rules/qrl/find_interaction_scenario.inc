//      StP/UML
//      Confidential property of Aonix
//      Copyright (c) 1997
//      All rights reserved

//const	string	NEW_C_CONTEXT_LABEL = "<<Create New Collaboration Context>>";

const string qFindAllScenariosByParent = "node[${siNodeType} && node_refs[file[${dg_type}]] && items[UmlParentTypeItem && value='${theParentNode.type}'] && items[UmlParentNameItem && value='${theParentNode.name}']]";

const	string	STRING_SEL_CANCEL = "Cancel";

struct structUmlScenarioInst
{
    string	label;		// scenario name in the scroilling list
    string	parentType;
    string	parentName;
    int		nId;		// scenario (if exists) or parent Id
};

list lExistingScenarios = list_create("structUmlScenarioInst", 0);
node theParentNode = NULL;
string dg_type = NULL;
string siNodeType = NULL;

void FindOrCreateScenarioForUseCase(int nodeId)
{
    siNodeType = "UmlScenarioInstance";
    FindOrCreateScenario(nodeId,"Use Case");
}

void FindOrCreateScenarioForCollaboration(int nodeId)
{
    siNodeType = "UmlScenarioInstance"; //"UmlCollaborationContext";
    FindOrCreateScenario(nodeId,"Collaboration");
}

void FindOrCreateScenario(int nodeId,string parentSymType)
{
    dg_type = current_editor_apptype();
    string	qFind_ById = "node[id==${nodeId}]";
    theParentNode = find_by_query(qFind_ById);
    if(theParentNode==NULL) {
	print_error("No "+parentSymType+" with Id " + nodeId, NULL);
	return;
    }

    EditorShowOnNavReceive();
    if (buffer_is_modified())
    {
        if ("" == gde_substitute_string("${file}"))
        {
            print_error("Please save unnamed diagram before navigating.");
            print_error("Navigation cancelled.");
            return;
        }
        if( editor_confirm("Save current diagram before proceeding?", "Yes", "No"))
        {
            file_save();
        }
    }

    //message("FindOrCreateScenario::theParentNode='"+theParentNode+"'");
    // check if such scenarios already exists.  Make sure we count
    // only this type of diagrams and check if the parent is the
    // same!!
    //message("FindOrCreateScenario::qFindAllScenariosByParent='"+qFindAllScenariosByParent+"'");
    node siNode = find_by_query(qFindAllScenariosByParent);
    //message("FindOrCreateScenario::siNode='"+siNode+"'");
    if(siNode==NULL) {
        umlCreateNewScenarioForUseCase(theParentNode);
        return;
    }
    //
    // Such a scenario already exists. Create a list of all such scenarios
    list_clear(lExistingScenarios);
    structUmlScenarioInst sn;
    sn.label = "<<Create New Scenario for "+parentSymType+">>";
    sn.nId = theParentNode.id;
    sn.parentType = theParentNode.type;
    sn.parentName = theParentNode.name;
    list_append(lExistingScenarios, sn);
    node scen;
    item ptItem, pnItem;
    string qFindParentTypeItemForScenario = "item[UmlParentTypeItem && obj_id == ${scen.id}]";
    string qFindParentNameItemForScenario = "item[UmlParentNameItem && obj_id == ${scen.id}]";
    for_each_in_select(qFindAllScenariosByParent, scen)
    {
	sn.nId = scen.id;
	sn.label = scen.name;
	ptItem = find_by_query(qFindParentTypeItemForScenario);
	pnItem = find_by_query(qFindParentNameItemForScenario);
	sn.parentType = ptItem.value;
	sn.parentName = pnItem.value;
	//message("FindOrCreateScenario:: sn='" + sn + "'");
	list_append(lExistingScenarios, sn);
    }

    if (list_count(lExistingScenarios) == 1)
    {
	umlCreateNewScenarioForUseCase(theParentNode);
	//message("####### +exit+ by RETURN from: FindOrCreateScenario");
	return;
    }
    list scenarios = list_create("string", 0);
    int ii;
    for(ii = 0; ii < list_count(lExistingScenarios); ii = ii + 1)
    {
	sn = list_get(lExistingScenarios, ii);
	list_append(scenarios, sn.label);
    }
    //message("DEBUG FindOrCreateScenario::allScenarios="+scenarios);
    ii = editor_item_selector(scenarios,
		"umlCreateNewScenarioForUseCaseByName();",
		"Navigate to Selected Scenario",
		True);

    //print_message("Scenario not found ii="+ii);
}


void
umlCreateNewScenarioForUseCase(node aNode)
{
    editor_string_selector("",			// init val
	    "umlCreateNamedScenarioCallback();",	// callback
	    "Create Scenario for " + aNode.name,	// window label
	    "Scenario Name:",			// Text label
	    "Create New Scenario",		// Apply label
	    STRING_SEL_CANCEL	     		// cancel label
	    );	 
    //message("###################### +exit+ umlCreateNewScenarioForUseCase");
}

int
umlCreateNamedScenarioCallback()
{
    string val;
    string name = "";
    node	aNode;

    // message("###################### +enter+ CreateNamedScenarioCallback");
    if (editor_string_selector_action() == STRING_SEL_CANCEL)
	return(0);

    val = editor_string_selector_value();
    if (val != NULL)
	name = strip_whitespace(val);

    if (name == "") {
	gde_print_error("Scenario name cannot be empty", NULL);
	return(1);
    }
    //
    // check if such a scenario already exists, for the same parent
    const string qFindAllNamedScenariosByParent = "node[${siNodeType} && name='${name}' && node_refs[file[${dg_type}]] && items[UmlParentTypeItem && value='${theParentNode.type}'] && items[UmlParentNameItem && value='${theParentNode.name}']]";
    if (theParentNode != NULL) {
	aNode = find_by_query(qFindAllNamedScenariosByParent);
	if (aNode != NULL) {
	    umlGoToScenarioForUseCase(aNode.id, name);
	    return(0);
	}
    }
    // There is no such scenario; create a new one.
    umlCreateNewNamedScenarioForUseCase(name, theParentNode);
    return(0);
}


void umlGoToScenarioForUseCase(int nodeId, string name)
{
    node_ref	nr;
    file	ff;
    string	qFindScenarioRef = "node_ref[node_id == ${nodeId} & file[${dg_type}]]";
    string	qFindScenarioFile = "file[id == ${nr.file_id}]";

    //message("DEBUG umlGoToScenarioForUseCase nodeId="+nodeId+" name='"+name+"'");
    nr = find_by_query(qFindScenarioRef);
    //message("DEBUG umlGoToScenarioForUseCase nr="+nr);
    if (nr == NULL) {
	umlCreateNewNamedScenarioForUseCase(name, theParentNode);
    } else {
	//
	// The scenario exists, find its file and load it
	ff = find_by_query(qFindScenarioFile);
	//message("DEBUG umlGoToScenarioForUseCase ff='"+ff+"'");
	if (ff == NULL) {
	    // file is not there?? Create a new scenario.
	    umlCreateNewNamedScenarioForUseCase(name, theParentNode);
	} else {
	    // Got everything we need; load the file and highlight the scenario
	    //message("DEBUG umlGoToScenarioForUseCase nr="+nr+" ff='"+ff+"'");
	    NavLoadAndFind(ff.name, nr.appid);
	}
    }
}

const	int	SCEN_FN_LEN = 50;

void umlCreateNewNamedScenarioForUseCase(string name, node parent)
{
    string	fn, tmp;
    file	f;
    int		scno = 1, v, len;

    if (string_length(parent.name) > SCEN_FN_LEN)
	fn = string_extract(parent.name, 0, SCEN_FN_LEN);
    else
	fn = parent.name;

    fn = create_new_file_in_sequence(fn, dg_type);

    gde_builtin("BufferClear 0");
    gde_builtin("EditorSetFileName \"" + fn + "\"");
    
    // JED - The following lines only pertain to loading EXISTING diagrams,
    // so instead I am just setting the name of the new diagram and
    // initializing the scenario.
    // NavLoadAndFind(fn, NULL);
    // Load terminated by user ....
    // if (gde_diagram_name() != fn)
	// return;

    umlScenarioInit(parent, name);
    // ECR 5191 start
    if(parent.type == "UmlUseCase") {
        file dg = find_by_query("file[${dg_type} & name = '${fn}']");
        if(dg != NULL) {
            string subsystem = computeSubsystemOfNode(parent);
            if(subsystem != NULL && subsystem != "unassigned") {
                int stpemHandle = editor_send_msg("ngoae", "EditorStart " + to_string(editor_get_msgd_name()) + " "
                                + to_string(editor_get_msgd_handle()), -1);
                string cmd = "EditorQrlEval setNoteAndItem(" + dg.id + ",\"UmlSubsystem\",\"UmlSubsystemName\",\""
                             + subsystem + "\",False);";
                editor_send_msg("ngoae", cmd, stpemHandle);
            }
        }
    }
    // ECR 5191 end
}


void umlScenarioInit(node parentNode, string scenarioName)
{
    list	old_symbols;
    gde_node	uc_node, scenario_node, n;
    string	name;
    int		i, id, x, y, dodefer;
    gde_symbol	psym;

    //message("###################### +enter+ umlScenarioInit***");
    string parentType = parentNode.type;
    string parentName = parentNode.name;
    gde_save_state();
    dodefer = gde_start_edit_transaction();
    //get_and_create_diag_mapping_prop();

    string targetSymbolType;// = "ScenarioInstance";
    if(parentType=="UmlUseCase")
	targetSymbolType = "ScenarioInstance";
    else
	targetSymbolType = "CollaborationContext";
    //message("umlScenarioInit::targetSymbolType='"+targetSymbolType+"'");
    uc_node = gde_node_create(targetSymbolType, 0, 0);
    string siLabel = scenarioName + "(" + parentType + ":" + parentName + ")";
    gde_node_set_label(uc_node,siLabel);
    gde_end_edit_transaction(GDE_CMD_INSERT, dodefer, True);

    id = gde_node_psymid(uc_node);
    gde_builtin("BufferFind " + id);
    gde_builtin("BufferCenter");
    gde_builtin("FileSave");
}


void
umlCreateNewScenarioForUseCaseByName()
{
    list	sel = editor_selector_selected_labels();
    string	scen;
    structUmlScenarioInst sn;

    //message("################# in umlCreateNewScenarioForUseCaseByName");

    if (sel == NULL || list_count(sel) == 0 || theParentNode == NULL)
	return;

    scen = list_get(sel, 0);
    sn = umlFindScenarioFromLabel(scen, lExistingScenarios);
    if (sn == NULL) {
	print_error("Did not find label in scenarios list (Internal Error)",
		    NULL);
	return;
    }
    //message("DEBUG umlCreateNewScenarioForUseCaseByName::selection="+sn+" parent="+theParentNode);
    if (sn.nId == theParentNode.id)
	umlCreateNewScenarioForUseCase(theParentNode);
    else
	umlGoToScenarioForUseCase(sn.nId, sn.label);
    //message("###################### +exit+ umlCreateNewScenarioForUseCaseByName");

}

structUmlScenarioInst
umlFindScenarioFromLabel(string str, list ll)
{
    structUmlScenarioInst sn;
    int	ii;

    //message("umlFindScenarioFromLabel::str='"+str+"' ll="+ll);
    for(ii = 0; ii < list_count(ll); ii++) {
	sn = list_get(ll, ii);
	if (sn.label == str)
	    return(sn);
    }
    return(NULL);
}

