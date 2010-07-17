//
// 	StP/UML
// 	Confidential property of Aonix
// 	Copyright (c) 2000
// 	All rights reserved
//

// ECR 4052 Model Management

void
setDiagramSubsystem()
{
    list labels = list_create("string", 0);
    node N;
    int i;
    list subsystems;
    node subsystem;

    if(gde_diagram_name() == "") { // ECR 7571
    	print_error("Diagram must be saved before assigning it to a subsystem"); // ECR 7571
    	return;
    }
    list_append(labels, "unassigned");
    subsystems = list_select( "node[UmlPackage & node_refs & items[UmlStereotype & value = 'subsystem']] sort by name" );
    for( i=0; i<list_count(subsystems); i++ )
    {
        subsystem = list_get( subsystems, i );
    	list_append( labels, subsystem.name );
    }
    editor_item_selector(labels, "setDiagramSubsystemCallback();",
                                 "Assign Diagram to Subsystem...",
                                 True);
}

void
setDiagramSubsystemCallback()
{
    string subsystem = list_get(editor_selector_selected_labels(), 0);
    int file_id = gde_file_id();
    // ECR 5076 start
    item I = find_by_query("item[UmlSubsystemName & obj_id = ${file_id}]");

    if(subsystem == "unassigned") {
	if(I == NULL)
	    return;
    }
    else {
	if(I != NULL && I.value == subsystem)
	    return;

        if(selection_count( "node[UmlPackage & name = '${subsystem}' & items[UmlSubsystemIsLocked & value = 'True']]" ) > 0) {
            print_error("The subsystem " + subsystem + " is locked, you cannot assign a diagram to it");
            return;
        }
    }

    if(I != NULL) {
        if(selection_count( "node[UmlPackage & name = '${I.value}' & items[UmlSubsystemIsLocked & value = 'True']]" ) > 0) {
            print_error("The diagram is assigned to the locked subsystem " + I.value + ", you cannot reassign this diagram");
            return;
        }
    }
    // ECR 5076 end

    int stpemHandle = editor_send_msg("ngoae", "EditorStart " + to_string(editor_get_msgd_name()) + " "
		    + to_string(editor_get_msgd_handle()), -1);
    // ECR 5009
    if( subsystem != "unassigned" )
        editor_send_msg("ngoae",
		    "EditorQrlEval setNoteAndItem(" + file_id + ",\"UmlSubsystem\",\"UmlSubsystemName\",\"" + 
		    subsystem + "\",False);", stpemHandle);
    else
        editor_send_msg("ngoae","EditorQrlEval removeNote(" + file_id + ",\"UmlSubsystem\",False);", stpemHandle);
}

int
idehook_diagram_assigned_to_subsystem_active_func()
{
    int file_id = gde_file_id();
    // save before checking (new file?)
    if(file_id == 0)
	return 0;
    // check if the file is assigned to a subsystem
    if(selection_count("file[id=${file_id} & items[UmlSubsystemName & value != 'unassigned']]") == 1)
        return 1;
    // check if there is a reference of some external element
    if(selection_count("file[id=${file_id} & node_refs[node[annot_file[file_locks[Permanent & user = 'stp@aonix']]]]]") == 1)
        return 1;
    return 0;
}

// ECR 5114 start
struct subsystemMessage {
    file F;
    string msg;
    int psymid;
    string hint;
};

list subsystemMessages;

// Check one diagram
int
CheckSubsystemSemantics()
{
    int ret = 0;
    int file_id = gde_file_id();

    subsystemMessages = list_create("subsystemMessage", 0);
    editor_set_is_busy(True);
    if(errorInAssignmentGraph(file_id))
        ++ ret;

    list subsystem_list = list_create("string", 0);

    file F;
    node N;
    string subsystem;
    F = find_by_query("file[${file_id}]");

    if(F.type == "UmlClassTable")
        N = find_by_query("node[UmlClass & node_refs[file_id = ${F.id}]]");
    else if(F.type == "UmlStateTable" || F.type == "UmlStateDiagram" || F.type == "UmlActivityDiagram")
        N = find_by_query("node[UmlStateMachine & node_refs[file_id = ${F.id}]]");
    else {
        N = NULL;
        item I = find_by_query("item[UmlSubsystemName & value != 'unassigned' & obj_id = ${file_id}]");
        if(I != NULL)
            list_append(subsystem_list, I.value);
    }

    if(N != NULL) {
        subsystem = computeSubsystemOfNode(N);
        if(subsystem != NULL)
            list_append(subsystem_list, subsystem);
    }

    int idx;
    for_each_in_select("node[UmlPackage & items[UmlStereotype & value = 'subsystem'] & node_refs[file_id = ${file_id}]]", N) {
        if(list_find(subsystem_list, 0, N.name) == list_count(subsystem_list))
            list_append(subsystem_list, N.name);
    }

    for(idx = 0; idx < list_count(subsystem_list); ++ idx) {
        subsystem = list_get(subsystem_list, idx);
        // ECR 5140
        createIdLists(subsystem, idx == 0);
        if(CheckOneSubsystem(file_id, subsystem, subsystem_list) == False)
            ++ ret;
        id_list_free("SubsystemFiles");
        id_list_free("NotSubsystemFiles");
    }
    editor_set_is_busy(False);

    return ret;
}

// check one subsystem
int
CheckTotalSubsystemSemantics(string subsystem)
{
    int ret = 0;

    subsystemMessages = list_create("subsystemMessage", 0);
    editor_set_is_busy(True);
    if(errorInAssignmentGraph(0))
        ++ ret;

    // ECR 5140
    createIdLists(subsystem, True);
    if(CheckOneSubsystem(0, subsystem, NULL) == False)
        ++ ret;
    id_list_free("SubsystemFiles");
    id_list_free("NotSubsystemFiles");

    print_message("Completed: Checking Subsystem Semantics");

    editor_set_is_busy(False);

    return ret;
}

// check list of subsystems
int
CheckListSubsystemSemantics(list subsystem_list)
{
    int ret = 0;

    subsystemMessages = list_create("subsystemMessage", 0);
    editor_set_is_busy(True);
    if(errorInAssignmentGraph(0))
        ++ ret;

    string subsystem;
    int idx;
    for(idx = 0; idx < list_count(subsystem_list); ++ idx) {
        subsystem = list_get(subsystem_list, idx);
        // ECR 5140
        createIdLists(subsystem, idx == 0);
        if(CheckOneSubsystem(0, subsystem, NULL) == False) // ECR 7725
            ++ ret;
        id_list_free("SubsystemFiles");
        id_list_free("NotSubsystemFiles");
    }

    print_message("Completed: Checking Subsystem Semantics");

    editor_set_is_busy(False);

    return ret;
}

// Add the implicit assigned tables/diagrams to the idlists
void
createIdLists(string subsystem, boolean fileWarnings)
{
    const string specialFiles = "!UmlStateTable & !UmlStateDiagram & !UmlActivityDiagram & !UmlClassTable";
    id_list_create("file[${specialFiles} & items[UmlSubsystemName & value = '${subsystem}']]",
                   "SubsystemFiles");
    id_list_create("file[${specialFiles} & !ObjectAnnotation & !items[UmlSubsystemName & value = '${subsystem}']]",
                   "NotSubsystemFiles");
    file F;
    node N;
    set addToIn = set_create("int");
    set addToNot = set_create("int");
    for_each_in_select("file[UmlStateTable | UmlStateDiagram | UmlActivityDiagram | UmlClassTable]", F) {
        if(F.type == "UmlClassTable") {
            N = find_by_query("node[UmlClass & node_refs[file_id = ${F.id}]]");
            // ECR 5140
            if(N == NULL) {
                if(fileWarnings == True)
                    printOtherWarning(F, "Warning: Table contains no Class");
                continue;
            }
            if(fileWarnings == True && selection_count("node[id = ${N.id} & node_refs[file[UmlClassDiagram]]]") == 0) {
                printOtherWarning(F, "Warning: " + formatNode(N)
                                  + " is not referenced in any diagram, file cannot be assigned to any subsystem");
                continue;
            }
        }
        else {
            N = find_by_query("node[UmlStateMachine & node_refs[file_id = ${F.id}]]");
            // ECR 5140
            if(N == NULL) {
                if(fileWarnings == True)
                    printOtherWarning(F, "Warning: Diagram/Table contains no State Machine");
                continue;
            }
            if(fileWarnings == True && existsParentOfStateMachine(N) == False) {
                printOtherWarning(F, "Warning: " + formatNode(N)
                                  + " has no corresponding Class, file cannot be assigned to any subsystem");
                continue;
            }
        }
        if(computeSubsystemOfNode(N) == subsystem)
            set_add(addToIn, F.id);
        else
            set_add(addToNot, F.id);
    }

    if(set_count(addToIn) > 0)
        id_list_update("SubsystemFiles", "file", addToIn, NULL);

    if(set_count(addToNot) > 0)
        id_list_update("NotSubsystemFiles", "file", addToNot, NULL);
}

// This function checks if a class is contained in multiple subsystems.
// There are still some situations, where a class has many owners (with owner
// I mean the base objects where a class is contained, which can be a Package
// with link UmlContains or item UmlEnclosingScope, another class with item
// UmlEnclosingScope, or for state machines an operation, class or use case
// with matching name).
struct SubsystemContains {
    boolean isSubsystem;
    set member;
    string system;
};

boolean
errorInAssignmentGraph(int file_id)
{
    graph G = graph_create();
    set subsystems = set_create("string");
    list subsystemContains = list_create("SubsystemContains", 0);
    list predecessors, allNodes, loopMembers, loopPredecessors;
    SubsystemContains SC1, SC2;
    node N, FN, TN, EN, SN;
    set commonMember;
    item E;
    link L;
    string from, to, msg, id;
    boolean found;
    boolean isAcyclic;
    boolean errorFound = False;
    int i, j, k;

    // build the UmlEnclosing hierarchy
    for_each_in_select("node[UmlClass & node_refs & items[UmlEnclosingScope]]", N) {
        E = find_by_query("item[UmlEnclosingScope & obj_id = ${N.id}]");
        if(E.value == NULL || E.value == "") {
            // Warning because we can ignore this
            printNodeWarning(file_id, N, formatNode(N) +
                " has an empty annotation item UmlEnclosingScope", True, "UmlEnclosingScope");
        }
        else {
            found = False;

            // There might be an UmlPackage and UmlClass with the same name
            for_each_in_select("node[(UmlPackage | UmlClass) & node_refs & name = '${E.value}']", EN) {
                found = True;
                from = N.type + " " + N.id;
                to = E.type + " " + E.id;
                graph_add_arc(G, from, to);
                from = to;
                to = EN.type + " " + EN.id;
                graph_add_arc(G, from, to);
                // if the from node is a subsystem, add it to the set of subsystems
                if(EN.type == "UmlPackage" &&
                    (FN = find_by_query("node[id = ${EN.id} & items[UmlStereotype & value = 'subsystem']]")) != NULL) {
                    set_add(subsystems, to);
                }
            }

            if(found == False) {
                // Warning because we can ignore this
                printNodeWarning(file_id, N, formatNode(N) +
                    " has an annotation item UmlEnclosingScope with value " + E.value +
                    ", but no such Class or Package exists", True, NULL);
            }
        }
    }

    // build the UmlContains hierarchy (restricted to Packages)
    for_each_in_select("link[UmlContains & link_refs & from_node[UmlPackage]]", L) {
        TN = find_by_query("node[id = ${L.to_node_id}]");
        // stop on subsystems
        if(TN.type == "UmlPackage" && selection_count("item[UmlStereotype & value = 'subsystem']"))
            continue;
        from = TN.type + " " + TN.id;
        to = L.type + " " + L.id;
        graph_add_arc(G, from, to);
        from = to;
        to = "UmlPackage " + L.from_node_id;
        graph_add_arc(G, from, to);

        // if the from node is a subsystem, add it to the set of subsystems
        if((FN = find_by_query("node[id = ${L.from_node_id} & items[UmlStereotype & value = 'subsystem']]")) != NULL) {
            set_add(subsystems, to);
        }
    }

    // for UmlStateMachines build the implicit 'by-name' hierarchy 
    for_each_in_select("node[UmlStateMachine]", N) {
	// ECR 5080
	// ECR 6911 added "scope_node_id != ${N.id} & "
	// for_each_in_select("node[(UmlUseCase | UmlClass | UmlOperation | UmlActionState) & node_refs & name = '${N.name}']", SN) {
	for_each_in_select("node[(UmlUseCase | UmlClass | UmlOperation | UmlActionState) & node_refs & scope_node_id != ${N.id} & name = '${N.name}']", SN) {
            from = N.type + " " + N.id;
            to = SN.type + " " + SN.id;
            graph_add_arc(G, from, to);
            // for an operation we need also the operation's class
            if(SN.type == "UmlOperation") {
                from = to;
                to = "UmlClass " + SN.scope_node_id;
                graph_add_arc(G, from, to);
            }
	    // ECR 5080
            // for an action state we need also an edge to the state's statemachine
            else if(SN.type == "UmlActionState") {
                from = to;
                to = "UmlStateMachine " + SN.scope_node_id;
                graph_add_arc(G, from, to);
            }
        }
    }

    // now there exists a graph which holds the subsystem assignment tree (and more)
    // and a set of subsystems
    // first check if there is a cycle
    isAcyclic = graph_is_acyclic(G);
    if(isAcyclic == False) {
        print_error("Error: A cycle is detected in the ownership graph of objects");
        print_error("This can happen when a class or package is member of more than one package or");
        print_error("when a class has a wrong item UmlEnclosingScope or");
        print_error("when there is a class or operation and an use case with the same name as a state machine");
        errorFound = True;
    }

    // create a list of pairs of some top level object and the contained nodes
    // Now check the last case, where a class may be contained in other top-level objects
    allNodes = graph_all_nodes(G);
    for(i = list_count(allNodes) - 1; i >= 0; -- i) {
        from = list_get(allNodes, i);

        // if there is any, try to find the cycle
        if(isAcyclic == False) {
            to = list_get(string_to_list(from, " "), 0);
            if(to != "UmlEnclosingScope" && to != "UmlContains") {
                predecessors = graph_node_indirect_predecessors(G, from);
                if(list_find(predecessors, 0, from) != list_count(predecessors)) {
                    print_error("Warning: the following list of objects cause a cycle:");
                    // now I have found a node which sure is member of a loop
                    // check for each node in the list if it is a member of a loop to
                    // add all positive candidates to a new list
                    loopMembers = list_create("string", 0);
                    for(j = 0; j < list_count(predecessors); ++ j) {
                        to = list_get(predecessors, j);
                        loopPredecessors = graph_node_indirect_predecessors(G, to);
                        if(list_find(loopPredecessors, 0, to) != list_count(loopPredecessors)) {
                            list_append(loopMembers, to);
                        }
                    }
                    printLoop(G, loopMembers, from, "", file_id);
                }
            }
        }

        // ignore all non-top-level entries
        if(list_count(graph_node_successors(G, from)) > 0)
            continue;

        if(set_is_member(subsystems, from))
            SC1.isSubsystem = True;
        else
            SC1.isSubsystem = False;
        SC1.system = from;
        SC1.member = set_create("string");

        predecessors = graph_node_indirect_predecessors(G, SC1.system);
        for(j = list_count(predecessors) - 1; j >= 0; -- j)
            set_add(SC1.member, list_get(predecessors, j));
        list_append(subsystemContains, SC1);
    }

    // check each pair of subsystems if there is a common entry
    // Hmm, quadratic time used :-(
//    string sub1, sub2;
    for(i = list_count(subsystemContains) - 1; i > 0; -- i) {
        SC1 = list_get(subsystemContains, i);
//        if(SC1.isSubsystem == True)
//            sub1 = "subsystem ";
//        else
//            sub1 = "";
        for(j = i - 1; j >= 0; -- j) {
            SC2 = list_get(subsystemContains, j);
            commonMember = set_copy(SC1.member);
            if(set_intersection(commonMember, SC2.member) > 0) {
                for(k = set_count(commonMember) - 1; k >= 0; -- k) {
                    from = set_get_element(commonMember, k);
                    to = list_get(string_to_list(from, " "), 0);
                    // Maybe we need this additional information later
                    if(to == "UmlEnclosingScope" || to == "UmlContains")
                        continue;

//                    if(SC2.isSubsystem == True)
//                        if(SC1.isSubsystem == True)
//                            sub2 = "";
//                        else
//                            sub2 = "subsystem ";
//                    else
//                        sub2 = "";

//                    msg = " is contained in " + sub1;
                    msg = " is contained in ";
                    id = list_get(string_to_list(SC1.system, " "), 1);
                    N = find_by_query("node[id = ${id}]");
//                    msg += "Package " + N.name + " and " + sub2;
                    msg += "Package " + N.name + " and ";
                    id = list_get(string_to_list(SC2.system, " "), 1);
                    N = find_by_query("node[id = ${id}]");
                    msg += "Package " + N.name;

                    id = list_get(string_to_list(from, " "), 1);
                    N = find_by_query("node[id = ${id}]");
                    if(printNodeWarning(file_id, N, formatNode(N) + msg, True, NULL) == True)
                        errorFound = True;
                }
            }
        }
    }

    if(list_count(subsystemMessages) > 0)
        sortAndPrintAllWarnings();

    if(errorFound == True)
        print_error("The errors above can cause unexpected assignment of objects to subsystems");

    return errorFound;
}

string
formatNode(node N)
{
    string name, char;
    if(string_extract(N.type, 0, 3) == "Uml")
        name = string_extract(N.type, 3, string_length(N.type) - 3);
    else
        name = N.type;

    int i;
    for(i = 1; i < string_length(name); ++ i) {
        char = string_extract(name, i, 1);
        if(char >= "A" && char <= "Z") {
            name = string_extract(name, 0, i) + " " + string_extract(name, i, string_length(name) - i);
            ++ i;
        }
    }

    if(N.name != "" && (N.type != "UmlAssociation" || string_find(N.name, 0, ",") == string_length(N.name))) {
        if(N.type == "UmlObjectInstance" && string_length(N.name) > 10 && string_extract(N.name, 0, 10) == "%%object%%")
            name = "anonymous " + name;
        else
            name = name + " " + N.name;
    }

    if(N.sig != "")
        name = name + "( " + N.sig + " )";

    return name;
}

string
formatLink(link L)
{
    string name, char;
    if(string_extract(L.type, 0, 3) == "Uml")
        name = string_extract(L.type, 3, string_length(L.type) - 3);
    else
        name = L.type;

    int i;
    for(i = 1; i < string_length(name); ++ i) {
        char = string_extract(name, i, 1);
        if(char >= "A" && char <= "Z") {
            name = string_extract(name, 0, i) + " " + string_extract(name, i, string_length(name) - i);
            ++ i;
        }
    }

    name += " link";
    if(L.name != "")
        name = name + " " + L.name;

    if(L.sig != "")
        name = name + "( " + L.sig + " )";

    return name;
}

// ECR 5140
string
formatFile(file F)
{
    string name, char;
    if(string_extract(F.type, 0, 3) == "Uml")
        name = string_extract(F.type, 3, string_length(F.type) - 3);
    else if(F.type == "REQTable")
        name = "RequirementsTable";
    else
        name = F.type;

    int i;
    for(i = 1; i < string_length(name); ++ i) {
        char = string_extract(name, i, 1);
        if(char >= "A" && char <= "Z") {
            name = string_extract(name, 0, i) + " " + string_extract(name, i, string_length(name) - i);
            ++ i;
        }
    }

    if(F.name != "")
        name = name + " " + F.name;

    return name;
}

string
getHint(string hint)
{
    // sorry, I am too lazy to write this over and over again
    const string front = "In a diagram assigned to a subsystem, all ";
    const string rear = " must be assigned to the same subsystem as the diagram";

    // from_node
    if(hint == "UmlAssociationClassLink")
        return front + "association classes" + rear;
    if(hint == "UmlGeneralization")
        return front + "subclasses of generalization links" + rear;
    if(hint == "UmlRefines")
        return front + "refined classes" + rear;
    if(hint == "UmlReturnMessage")
        return front + "object instances returning messages to other objects" + rear;
    if(hint == "UmlUseCaseUses")
        return front + "use cases using other use cases" + rear;
    if(hint == "UmlDependency")
        return front + "elements depending on other elements" + rear;
    if(hint == "UmlImplements")
        return front + "classes implementing an interface" + rear;
    if(hint == "UmlAsynchronousMessage" || hint == "UmlBalkingMessage" || hint == "UmlSimpleMessage"
       || hint == "UmlSynchronousMessage" || hint == "UmlTimeoutMessage")
        return front + "object instances sending messages" + rear;

    // from_node and to_node
    if(hint == "FriendDependency")
        return front + "classes connected with a friend dependencies" + rear;

    // to_node
    if(hint == "ToUmlAsynchronousMessage" || hint == "ToUmlBalkingMessage" || hint == "ToUmlSimpleMessage"
       || hint == "ToUmlSynchronousMessage" || hint == "ToUmlTimeoutMessage")
        return front + "object instances receiving messages must have a corresponding operation in the class definition or" + rear;
    if(hint == "UmlCollaborationRole")
        return front + "association classes" + rear;
    if(hint == "UmlUseCaseExtends")
        return front + "extended use cases" + rear;
    if(hint == "UmlUseCaseInteraction")
        return front + "interacting use cases" + rear;

    // associations
    if(hint == "NavigableAssociation")
        return front + "classes connected with navigable associations" + rear;
    if(hint == "Association")
        return front + "classes connected with associations of unspecified navigability" + rear;
    if(hint == "Aggregation" || hint == "NavigableAggregation")
        return front + "classes connected with aggregations" + rear;
    if(hint == "Composition" || hint == "NavigableComposition")
        return front + "classes connected with compositions" + rear;
    if(hint == "UmlAssociationClassLink")
        return front + "association classes" + rear;

    // contains
    if(hint == "ContainLocked")
        return "It is illegal to put a locked symbol into an unlocked.";
    if(hint == "ContainUnlocked")
        return "It is illegal to put an unlocked symbol into a locked.";
    return "No hint for " + hint;
}

void
sortAndPrintAllWarnings()
{
    list_sort_by_field(subsystemMessages, "F");
    set hints = set_create("string");
    int i;
    subsystemMessage M;
    file lastF = NULL;
    for(i = 0; i < list_count(subsystemMessages); ++ i) {
        M = list_get(subsystemMessages, i);
        if(M.F != NULL && (lastF == NULL || lastF != M.F)) {
            print_message("In " + M.F.type + " " + M.F.name);
            lastF = M.F;
        }
        if(M.psymid == 0)
            print_error(M.msg);
        else
            gde_print_error(M.msg, M.psymid);
        if(M.hint != NULL && set_is_member(hints, M.hint) == False) {
            print_message(getHint(M.hint));
            set_add(hints, M.hint);
        }
    }
    list_clear(subsystemMessages);
}

// ECR 5140 start
boolean
printOtherWarning(file F, string msg)
{
    subsystemMessage M;
    M.msg = msg;
    M.hint = NULL;
    M.F = F;
    M.psymid = 0;

    list_append(subsystemMessages, M);

    return True;
}
// ECR 5140 end

boolean
printNodeWarning(int file_id, node N, string msg, boolean printOnlyWhenInCurrentDiagram, string hint)
{
    subsystemMessage M;
    M.msg = msg;
    M.hint = hint;

    // global check?
    // ECR 5114 -->
    if(file_id == 0 || editor_get_msgd_name() == "stp") {
        list files;
        if(file_id == 0)
            files = list_select("file[node_refs[node_id = ${N.id}]] sort by type");
        else
            files = list_select("file[id = ${file_id}]");
        // ECR 5114 <--
        int i;
        M.psymid = 0;
        for(i = 0; i < list_count(files); ++ i) {
            M.F = list_get(files, i);
            list_append(subsystemMessages, M);
        }
        return True;
    }

    // check a certain diagram
    M.F = NULL;
    if(selection_count("node_ref[node_id = ${N.id} & file_id = ${file_id}]") == 0) {
        if(printOnlyWhenInCurrentDiagram == True)
            return False;

        list_append(subsystemMessages, M);
        return True;
    }

    M.psymid = omsNodeToPsymId(N);
    list_append(subsystemMessages, M);

    return True;
}

boolean
printLinkWarning(int file_id, link L, string msg, boolean printOnlyWhenInCurrentDiagram, string hint)
{
    subsystemMessage M;
    M.msg = msg;
    M.hint = hint;

    // global check?
    if(file_id == 0) {
        string last_type = "";
        string refmsg = "";
        list files = list_select("file[link_refs[link_id = ${L.id}]] sort by type");
        int i;
        M.psymid = 0;
        for(i = 0; i < list_count(files); ++ i) {
            M.F = list_get(files, i);
            list_append(subsystemMessages, M);
        }
        return True;
    }

    // check a certain diagram
    M.F = NULL;
    if(selection_count("link_ref[link_id = ${L.id} & file_id = ${file_id}]") == 0) {
        if(printOnlyWhenInCurrentDiagram == True)
            return False;

        list_append(subsystemMessages, M);
        return True;
    }

    M.psymid = omsLinkToPsymId(L);
    list_append(subsystemMessages, M);

    return True;
}

void
printLoop(graph G, list loopMembers, string from, string start, int file_id)
{
    string type, id;
    int j;
    list succ;

    type = list_get(string_to_list(from, " "), 0);
    id = list_get(string_to_list(from, " "), 1);

    if(type == "UmlContains") {
        print_message("Containment link");
    }
    else if(type == "UmlEnclosingScope") {
        item I = find_by_query("item[id = ${id}]");
        print_message("Item Enclosing Scope with value '" + I.value + "'");
    }
    else {
        node N = find_by_query("node[id = ${id}]");
        printNodeWarning(file_id, N, formatNode(N), False, NULL);
    }

    if(from == start)
        return;

    if(start == "")
        start = from;

    succ = graph_node_successors(G, from);
    for(j = 0; j < list_count(succ); ++ j) {
        if(list_find(loopMembers, 0, list_get(succ, j)) != list_count(loopMembers)) {
            printLoop(G, loopMembers, list_get(succ, j), start, file_id);
        }
    }
}

// check if a class corresponding to an object has a corresponding operation
// returns True if
// it is called with a node of type UmlObjectInstance and there is a corresponding UmlClass
// o) and the link has no label
// o) and the link's label has no parameters and
//                         there is a operation with the same name as the link's label in that class
// o) and the number of parameters in the link's label matches the number of parameters
//                         in an operation of the class with the same name
boolean
checkObjectsClassOperations(node Obj, link Message)
{
    if(Obj.type != "UmlObjectInstance")
        return False;
    node ClassScope = find_by_query("node[id = ${Obj.scope_node_id}]");
    node Class = find_by_query("node[UmlClass & node_refs & name = '${ClassScope.name}']");

    if(Class == NULL)
        return False;

    // an unspecified message is ok
    if(Message.name == "")
        return True;

    item messageName = find_by_query("item[UmlObjectName & obj_id = ${Message.id}]");
    list operations = list_select("node[UmlOperation & node_refs & name = '${messageName.value}' & scope_node_id = ${Class.id}]");

    // is there an operation with the same name
    if(operations == NULL || list_count(operations) == 0)
        return False;

    item messageParams = find_by_query("item[UmlMessageArgs & obj_id = ${Message.id}]");
    // are there any parameters specified in the message
    if(messageParams.value == "")
        return True;

    // count the parameters (this is done by counting the commas in the message and the commas or semicolons in the operation)
    int opCnt, msgCnt = 1;
    int oix, ix;
    string char;
    node Operation;

    for(ix = string_length(messageParams.value) - 1; ix >= 0; -- ix) {
        char = string_extract(messageParams.value, ix, 1);
        if(char == ",")
            ++ msgCnt;
    }

    for(oix = list_count(operations) - 1; oix >= 0; -- oix) {
        Operation = list_get(operations, oix);
        if(Operation.sig == "")
            continue;

        opCnt = 1;
        for(ix = string_length(Operation.sig) - 1; ix >= 0; -- ix) {
            char = string_extract(Operation.sig, ix, 1);
            if(char == "," || char == ";")
                ++ opCnt;
        }
        if(opCnt == msgCnt)
            return True;
    }

    return False;
}

boolean
CheckOneSubsystem(int file_id, string subsystem, list subsystem_list)
{
    boolean ret = True;
    list fileList;
    file theFile;
    link L;
    string msg;
    string tmpType;
    int idx;

    print_message("Checking Subsystem " + subsystem);

    // first check, if the package containing the subsystem is visible
    // in one of the diagrams
    // If the defining package is not included, we do not have a subsystem after loading the set of files into a fresh system
    if(selection_count("node[UmlPackage & name = '${subsystem}' & items[UmlStereotype & value = 'subsystem'] & node_refs[file[SubsystemFiles]]]") == 0) {
        print_error("At least one file which contains the subsystem package must be assigned to the subsystem");
        fileList = list_select("file[node_refs[node[UmlPackage & name = '${subsystem}' & items[UmlStereotype & value = 'subsystem']]]] sort by type");
        if(list_count(fileList) == 1) {
            theFile = list_get(fileList, 0);
            print_message("Assign file " + theFile.type + " " + theFile.name);
        }
        else {
            msg = "";
            tmpType = "";
            for(idx = 0; idx < list_count(fileList); ++ idx) {
                theFile = list_get(fileList, idx);
                if(msg != "") {
                    if(idx == list_count(fileList) - 1)
                        msg += " or ";
                    else
                        msg += ", ";
                }
                if(tmpType != theFile.type)
                    msg += theFile.type + " ";
                msg += theFile.name;
            }
            print_message("Assign at least one of these files: " + msg);
        }
    
        // this is a fatal error, so stop immediately
        return False;
    }

    // in all files search for other subsystem packages, check if these packages are somehow
    // contained in the main subsystem
    node N, Nsub;
    node_ref Nref;
    for_each_in_select("node[UmlPackage & name != '${subsystem}' & items[UmlStereotype & value = 'subsystem'] & node_refs[file[SubsystemFiles]]]", N) {
        // N is a different subsystem referenced in any of my subsystem's files
        // first check if in any of these files my subsystem is also defined
        for_each_in_select("node_ref[node_id = ${N.id} & file[SubsystemFiles]]", Nref) {
            Nsub = find_by_query("node[UmlPackage & name = '${subsystem}' & node_refs[file_id = ${Nref.file_id}]]");
            if(Nsub == NULL) {
                // User has assigned a diagram without my subsystem but with an external subsystem
                printNodeWarning(Nref.file_id, N, "Error: A diagram with other subsystem " + N.name + " cannot be assigned to subsystem "
                                 + subsystem, True, NULL);  // ECR 5114
                ret = False;
            }
            else {
                // ECR 7725 -->
                if(isSubsystemContainedBySubsystem(N, subsystem) == False && isSubsystemContainedBySubsystem(Nsub, N.name) == False) {
                    // User has assigned a diagram with both, subsystem and external subsystem side by side
                    printNodeWarning(Nref.file_id, N, "Error: A diagram with subsystems " + N.name + " and " + Nsub.name
                                     + " cannot be assigned to a subsystem, unless one subsystem contains the other", True, NULL); // ECR 5114
                    ret = False;
                }
                else if(subsystem_list == NULL || (list_count(subsystem_list) == 1 && list_get(subsystem_list, 0) == subsystem)) {
                    if(isSubsystemContainedBySubsystem(N, subsystem) == False) {
                        // User has assigned a diagram with both, subsystem and external subsystem but the other subsystem is not in the list
                        printNodeWarning(Nref.file_id, N, "Error: A diagram with subsystems " + N.name + " and " + Nsub.name
                                         + " cannot be assigned to a subsystem, unless one subsystem contains the other", True, NULL); // ECR 5114
                        ret = False;
                    }
                }
                else if(list_find(subsystem_list, 0, N.name) == list_count(subsystem_list)) {
                    // User has assigned a diagram with both, subsystem and external subsystem but the other subsystem is not in the list
                    printNodeWarning(Nref.file_id, N, "Error: A diagram with subsystems " + N.name + " and " + Nsub.name
                                     + " cannot be assigned to a subsystem, unless one subsystem contains the other", True, NULL); // ECR 5114
                    ret = False;
                }
                // ECR 7725 <--
            }
        }
    }

    // for each node with a locked annotation (Permanent & owner = stp@aonix)
    // check if the value of item UmlExternalSubsystem matches the computed (via link, scope and UmlContains)
    // this can happen if the user creates an UmlContains link for a foreign node
    node Nparent;
    item setSubsystem;
    string computedSubsystem;
    for_each_in_select("node[annot_file[file_locks[Permanent & user = 'stp@aonix']] && node_refs[file[SubsystemFiles]]]", N) {
        setSubsystem = find_by_query("item[UmlExternalSubsystemName & obj_id = ${N.id}]");
        // node has no set subsystem by itself, look at the logical owner
        if(setSubsystem == NULL) {
            Nparent = computeSubsystemParentOfNode(N);
            setSubsystem = find_by_query("item[UmlExternalSubsystemName & obj_id = ${Nparent.id}]");
        }
        computedSubsystem = computeSubsystemOfNode(N);
        if(computedSubsystem == NULL) {
            // if I did not find any owning subsystem package, than everything except the current subsystem is valid
            if(setSubsystem != NULL && setSubsystem.value == subsystem) {
                ret = False;
                // Hmm, how can a user set this ?
                // a locked node was modified ??
                printNodeWarning(file_id, N, "Internal Error: " + formatNode(N) + " is not owned by subsystem " + subsystem +
                                 ", but is annotated that way", True, NULL);
            }
        }
        else {
            // ECR 5114
            // filter out locked subsystems
            if(selection_count("node[UmlPackage & name = '${computedSubsystem}' & items[UmlSubsystemIsLocked & value = 'True']]") == 0) {
                if(setSubsystem == NULL) {
                    // something is wrong here?
                    // an external node always has an annotation UmlExternalSubsystemName, so how is it lost?
                    ret = False;
                    printNodeWarning(file_id, N, "Internal Error: " + formatNode(N)
                                     + " should not be owned by any subsystem, but is member of " + computedSubsystem, True, NULL);
                }
                else if(setSubsystem.value != computedSubsystem) {
                    // at least two possible scenarios:
                    // o) the node is external, and has been moved into a subsystem
                    // o) the node is external (owned by subsystem x), later the node was removed from that
                    //    subsystem x and this subsystem got imported.
                    ret = False;
                    printNodeWarning(file_id, N, "Error: " + formatNode(N) + " is owned by subsystem "
                                     + subsystem + ", but is member of " + computedSubsystem, True, NULL);
                }
            }
        }
    }

    // check those special nodes which are assigned by the enclosing diagram
    string specialNodes = "UmlComponentBinary|UmlComponentExecutable|UmlComponentInterface|UmlComponentObject|UmlComponentSource"
                        + "|UmlDeploymentNode|UmlStereotype|UmlSignal|RequirementSubsystem|REQRequirement|ParentRequirement|LeafRequirement";
    file F;
    item I;
    for_each_in_select("node[(${specialNodes}) & node_refs[file[SubsystemFiles]]]", N) {
        F = find_by_query("file[node_refs[node_id = ${N.id}] & NotSubsystemFiles & items[UmlSubsystemName & value != '' & value != 'unassigned']]");
        if(F != NULL) {
            I = find_by_query("item[UmlSubsystemName && obj_id = ${F.id}]");
            // a special node is contained in more than one subsystem
            printNodeWarning(file_id, N, "Error: " + formatNode(N) + " must not be contained in files of different subsystems "
                             + subsystem + " and " + I.value + " (" + F.type + " " + F.name + ")", True, NULL);
            ret = False;
        }
    }

    // check links
    // ECR 5246, moved UmlAssociationClassLink to toLinks below
    string fromLinks = "UmlGeneralization|UmlRefines|UmlUseCaseUses|UmlDependency"
                     + "|UmlImplements|UmlAsynchronousMessage|UmlBalkingMessage|UmlSimpleMessage|UmlSynchronousMessage|UmlTimeoutMessage";
    string hint;
    for_each_in_select("link[(${fromLinks}) & link_refs[file[SubsystemFiles]]]", L) {
        if(L.type == "UmlDependency" && selection_count("item[UmlStereotype & value = 'friend' && obj_id = ${L.id}]") > 0)
            hint = "FriendDependency";
        else
            hint = L.type;
        for_each_in_select("node[(!UmlEllipsis & !UmlActor) & out_links[id = ${L.id}]]", N) {
            computedSubsystem = computeSubsystemOfNode(N);
            if(computedSubsystem == NULL) {
                ret = False;
                printLinkWarning(file_id, L, "Error: " + formatLink(L) + " starts from " +
                                 formatNode(N) + " which is not member of any subsystem ", True, hint);
            }
            else if(computedSubsystem != subsystem) {
                ret = False;
                printLinkWarning(file_id, L, "Error: " + formatLink(L) + " starts from " +
                                 formatNode(N) + " which is member of different subsystem " + computedSubsystem, True, hint);
            }
        }
    }

    // ECR 5246, moved UmlAssociationClassLink from fromLinks above
    string toLinks = "UmlAssociationClassLink|UmlCollaborationRole|UmlUseCaseExtends|UmlUseCaseInteraction"
                   + "|(UmlDependency & items[UmlStereotype & value = 'friend'])";
    for_each_in_select("link[(${toLinks}) & link_refs[file[SubsystemFiles]]]", L) {
        if(L.type == "UmlDependency")
            hint = "FriendDependency";
        else
            hint = L.type;
        for_each_in_select("node[in_links[id = ${L.id}]]", N) {
            computedSubsystem = computeSubsystemOfNode(N);
            if(computedSubsystem == NULL) {
                ret = False;
                printLinkWarning(file_id, L, "Error: " + formatLink(L) + " ends at " +
                                 formatNode(N) + " which is not member of any subsystem ", True, hint);
            }
            else if(computedSubsystem != subsystem) {
                ret = False;
                printLinkWarning(file_id, L, "Error: " + formatLink(L) + " ends at " +
                                 formatNode(N) + " which is member of different subsystem " + computedSubsystem, True, hint);
            }
        }
    }

    toLinks = "UmlAsynchronousMessage|UmlBalkingMessage|UmlSimpleMessage|UmlSynchronousMessage|UmlTimeoutMessage";
    for_each_in_select("link[(${toLinks}) & link_refs[file[SubsystemFiles]]]", L) {
        hint = "To" + L.type;
        for_each_in_select("node[!UmlActor & in_links[id = ${L.id}]]", N) {
            if(checkObjectsClassOperations(N, L) == True)
                continue;
            computedSubsystem = computeSubsystemOfNode(N);
            if(computedSubsystem == NULL) {
                ret = False;
                printLinkWarning(file_id, L, "Error: " + formatLink(L) + " ends at " +
                                 formatNode(N) + " which is not member of any subsystem ", True, hint);
            }
            else if(computedSubsystem != subsystem) {
                ret = False;
                printLinkWarning(file_id, L, "Error: " + formatLink(L) + " ends at " +
                                 formatNode(N) + " which is member of different subsystem " + computedSubsystem, True, hint);
            }
        }
    }

    // ECR 5080
    string scopeLinks = "UmlTransition";
    for_each_in_select("link[(${scopeLinks}) & link_refs[file[SubsystemFiles]]]", L) {
        for_each_in_select("node[id = ${L.scope_node_id}]", N) {
            computedSubsystem = computeSubsystemOfNode(N);
            if(computedSubsystem == NULL) {
                ret = False;
                printLinkWarning(file_id, L, "Error: " + formatLink(L) + " is scoped to " +
                                 formatNode(N) + " which is not member of any subsystem ", True, L.type);
            }
            else if(computedSubsystem != subsystem) {
                ret = False;
                printLinkWarning(file_id, L, "Error: " + formatLink(L) + " is scoped to " +
                                 formatNode(N) + " which is member of different subsystem " + computedSubsystem, True, L.type);
            }
        }
    }

    // Associations and Roles
    list Roles;
    node C1, C2;
    boolean toC1, toC2;
    item aggC1, aggC2;
    string hintC1, hintC2;
    link R;
    for_each_in_select("node[UmlAssociation & node_refs[file[SubsystemFiles]]]", N) {
        Roles = list_select("link[UmlRole & link_refs & to_node_id = ${N.id}]");
        if(Roles == NULL || list_count(Roles) == 0) {
            ret = False;
            printLinkWarning(file_id, N, "Error: " + formatNode(N) + " without any roles found!", True, L.type);
            continue;
        }

        if(list_count(Roles) <= 2) {
            // simple association
            R = list_get(Roles, 0);
            C1 = find_by_query("node[UmlClass & id = ${R.from_node_id}]");
            aggC1 = find_by_query("item[UmlAggregationType & obj_id = ${R.id}]");
            if(aggC1 == NULL || aggC1.value == "None")
                hintC1 = "Association";
            else
                hintC1 = aggC1.value;
            if(selection_count("item[UmlRoleNavigability & value = 'True' & obj_id = ${R.id}]") > 0)
                toC1 = True;
            else
                toC1 = False;

            if(list_count(Roles) == 2) {
                R = list_get(Roles, 1);
                C2 = find_by_query("node[UmlClass & id = ${R.from_node_id}]");
                aggC2 = find_by_query("item[UmlAggregationType & obj_id = ${R.id}]");
                if(aggC2 == NULL || aggC2.value == "None")
                    hintC2 = "Association";
                else
                    hintC2 = aggC2.value;
                if(selection_count("item[UmlRoleNavigability & value = 'True' & obj_id = ${R.id}]") > 0)
                    toC2 = True;
                else
                    toC2 = False;
                // ECR 5114 -->
                // in case of Composition/Aggregation, set some default navigability if both
                // ends hold the same nav. information
                if(toC2 == toC1) {
                    if(hintC1 != "Association" && hintC2 == "Association") {
                        toC1 = False;
                        toC2 = True;
                    }
                    else if(hintC2 != "Association" && hintC1 == "Association") {
                        toC2 = False;
                        toC1 = True;
                    }
                }
                // ECR 5114 <--
            }
            else {
                C2 = C1;
                toC2 = toC1;
                aggC2 = aggC1;
            }

            // now I have the two endpoints, check if the association is navigable in some direction(s)
            // with no navigability, I assume navigability in both directions
            if(toC1 == False && toC2 == False) {
                if(aggC1 != NULL && aggC1.value != "None") {
                    computedSubsystem = computeSubsystemOfNode(C1);
                    if(computedSubsystem == NULL) {
                        ret = False;
                        printLinkWarning(file_id, R, "Error: " + hintC1 + " starts from " + formatNode(C1)
                                         + " which is not member of any subsystem ", True, aggC1.value);
                    }
                    else if(computedSubsystem != subsystem) {
                        ret = False;
                        printLinkWarning(file_id, R, "Error: " + hintC1 + " starts from " + formatNode(C1)
                                         + " which is member of different subsystem " + computedSubsystem, True, aggC1.value);
                    }
                }
                else if(aggC2 != NULL && aggC2.value != "None") {
                    computedSubsystem = computeSubsystemOfNode(C1);
                    if(computedSubsystem == NULL) {
                        ret = False;
                        printLinkWarning(file_id, R, "Error: " + hintC2 + " starts from " + formatNode(C2)
                                         + " which is not member of any subsystem ", True, hintC2);
                    }
                    else if(computedSubsystem != subsystem) {
                        ret = False;
                        printLinkWarning(file_id, R, "Error: " + hintC2 + " starts from " + formatNode(C2)
                                         + " which is member of different subsystem " + computedSubsystem, True, hintC2);
                    }
                }
                else {
                    computedSubsystem = computeSubsystemOfNode(C1);
                    if(computedSubsystem == NULL) {
                        ret = False;
                        printLinkWarning(file_id, R, "Error: " + hintC1 + " starts from " + formatNode(C1)
                                         + " which is not member of any subsystem ", True, hintC1);
                    }
                    else if(computedSubsystem != subsystem) {
                        ret = False;
                        printLinkWarning(file_id, R, "Error: " + hintC1 + " starts from " + formatNode(C1)
                                         + " which is member of different subsystem " + computedSubsystem, True, hintC1);
                    }
                    else {
                        computedSubsystem = computeSubsystemOfNode(C2);
                        if(computedSubsystem == NULL) {
                            ret = False;
                            printLinkWarning(file_id, R, "Error: " + hintC2 + " starts from " + formatNode(C2)
                                             + " which is not member of any subsystem ", True, hintC2);
                        }
                        else if(computedSubsystem != subsystem) {
                            ret = False;
                            printLinkWarning(file_id, R, "Error: " + hintC2 + " starts from " + formatNode(C2)
                                             + " which is member of different subsystem " + computedSubsystem, True, hintC2);
                        }
                    }
                }
            }

            // The class from where the navigability starts must be in the same subsystem as the referenced diagram
            if(toC2 == True) {
                computedSubsystem = computeSubsystemOfNode(C1);
                if(computedSubsystem == NULL) {
                    ret = False;
                    printLinkWarning(file_id, R, "Error: " + hintC1 + " starts from " + formatNode(C1)
                                     + " which is not member of any subsystem ", True, "Navigable" + hintC1);
                }
                else if(computedSubsystem != subsystem) {
                    ret = False;
                    printLinkWarning(file_id, R, "Error: " + hintC1 + " starts from " + formatNode(C1)
                                     + " which is member of different subsystem " + computedSubsystem, True, "Navigable" + hintC1);
                }
            }
            if(toC1 == True) {
                computedSubsystem = computeSubsystemOfNode(C2);
                if(computedSubsystem == NULL) {
                    ret = False;
                    printLinkWarning(file_id, R, "Error: " + hintC2 + " starts from " + formatNode(C2)
                                     + " which is not member of any subsystem ", True, "Navigable" + hintC2);
                }
                else if(computedSubsystem != subsystem) {
                    ret = False;
                    printLinkWarning(file_id, R, "Error: " + hintC2 + " starts from " + formatNode(C2)
                                     + " which is member of different subsystem " + computedSubsystem, True, "Navigable" + hintC2);
                }
            }
        }
        else {
            // nary association
            // since there is no navigability for an nary association, all
            // participating classes must be in the same subsystem as the diagram
            for(idx = 0; idx < list_count(Roles); ++ idx) {
                R = list_get(Roles, idx);
                C1 = find_by_query("node[UmlClass & id = ${R.from_node_id}]");
                computedSubsystem = computeSubsystemOfNode(C1);
                if(computedSubsystem == NULL) {
                    ret = False;
                    printLinkWarning(file_id, R, "Error: N-ary " + formatNode(N) + " starts from " +
                                     formatNode(C1) + " which is not member of any subsystem ", True, N.type);
                }
                else if(computedSubsystem != subsystem) {
                    ret = False;
                    printLinkWarning(file_id, R, "Error: N-ary " + formatNode(N) + " starts from " +
                                     formatNode(C1) + " which is member of different subsystem " + computedSubsystem, True, N.type);
                }
            }
        }
    }

    for_each_in_select("node[UmlClass & in_links[UmlAssociationClassLink & link_refs[file[SubsystemFiles]]]]", N) {
        computedSubsystem = computeSubsystemOfNode(N);
        if(computedSubsystem == NULL) {
            ret = False;
            printNodeWarning(file_id, N, "Error: Association " + formatNode(N)
                             + " must be member of the same subsystem as the diagram, but is not member of any subsystem ",
                             True, "UmlAssociationClassLink");
        }
        else if(computedSubsystem != subsystem) {
            ret = False;
            printNodeWarning(file_id, N, "Error: Association " + formatNode(N)
                             + " must be member of the same subsystem as the diagram, but is member of different subsystem "
                             + computedSubsystem, True, "UmlAssociationClassLink");
        }
    }

    // Contains link
    node Nfrom, Nto;
    boolean fromLocked, toLocked;
    for_each_in_select("link[UmlContains & link_refs[file[SubsystemFiles]]]", L) {
        Nfrom = find_by_query("node[!UmlSwimLane && id = ${L.from_node_id}]");
        if(Nfrom == NULL)
            continue;
        if(Nfrom.annot_file_id != 0
           && selection_count("file_lock[Permanent & user = 'stp@aonix' & file_id = ${Nfrom.annot_file_id}]") > 0)
            fromLocked = True;
        else
            fromLocked = False;
        Nto = find_by_query("node[id = ${L.to_node_id}]");
        if(Nto.annot_file_id != 0
           && selection_count("file_lock[Permanent & user = 'stp@aonix' & file_id = ${Nto.annot_file_id}]") > 0)
            toLocked = True;
        else
            toLocked = False;
        
        if(fromLocked == True && toLocked == False) {
            ret = False;
            printNodeWarning(file_id, Nto, "Error: unlocked " + formatNode(Nto) + " is contained by locked " +
                             formatNode(Nfrom), True, "ContainUnlocked");
        }
        else if(fromLocked == False && toLocked == True) {
            ret = False;
            printNodeWarning(file_id, Nto, "Error: locked " + formatNode(Nto) + " is contained by unlocked " +
                             formatNode(Nfrom), True, "ContainLocked");
        }
    }

    if(list_count(subsystemMessages) > 0)
        sortAndPrintAllWarnings();

    return ret;
}

// retrieve the owning subsystem-package of a node
// This function assumes that there is only one way from a node to a subsystem defining package,
// it also assumes, that there is no dead end when going that way. These assumptions a checked in
// function errorInAssignmentGraph()
string
computeSubsystemOfNode(node N)
{
    // climb up the scope
    node SN;
    while(N.scope_node_id != 0) {
	SN = find_by_query("node[id = ${N.scope_node_id}]");
	if(SN != NULL) // should be always true ?
            N = SN;
    }

    // find the corresponding UmlClass, UmlOperation or UseCase for the Statemachine 
    if(N.type == "UmlStateMachine") {
	// ECR 5080 start
	while(True) {
            // ECR 6911 added "scope_node_id != ${N.id} & "
	    // N = find_by_query("node[(UmlUseCase | UmlClass | UmlOperation | UmlActionState) & node_refs & name = '${N.name}']");
	    N = find_by_query("node[(UmlUseCase | UmlClass | UmlOperation | UmlActionState) & node_refs & scope_node_id != ${N.id} & name = '${N.name}']");
	    if(N == NULL)
		return NULL;
	    if(N.type != "UmlActionState")
		break;
	    N = find_by_query("node[id = ${N.scope_node_id}]");
	    if(N == NULL)
		return NULL;
	}
	// ECR 5080 end

        // climb up the scope (UmlOperation)
        while(N.scope_node_id != 0) {
            SN = find_by_query("node[id = ${N.scope_node_id}]");
            if(SN != NULL) // should be always true ?
                N = SN;
        }
    }
    // find the corresponding UmlClass for the UmlObjectClassScope 
    else if(N.type == "UmlObjectClassScope") {
	N = find_by_query("node[UmlClass & node_refs & name = '${N.name}']");
	if(N == NULL)
	    return NULL;
    }

    // climb up the UmlEnclosingScope item
    item E;
    node EN;
    while(N.type == "UmlClass") {
	E = find_by_query("item[UmlEnclosingScope & obj_id = ${N.id}]");
	if(E == NULL)
            break;

        if(E.value == NULL || E.value == "") {
            // Warning because we can ignore this
            print_error("Warning: " + N.type + " has an empty annotation UmlEnclosingScope");
            break;
        }

        EN = find_by_query("node[(UmlPackage | UmlClass) & node_refs & name = '${E.value}']");
        if(EN == NULL) {
            // Warning because we can ignore this
            print_error("Warning: " + N.type + " has an annotation UmlEnclosingScope with value " + E.value +
                        ", but no such UmlClass or UmlPackage exists");
            break;
        }
        N = EN;
    }

    if(N != NULL && N.type == "UmlPackage") {
        if(selection_count("item[UmlStereotype & value = 'subsystem' & obj_id = ${N.id}]") == 1)
            return N.name;
    }
    // climb up the UmlContains link
    while(N != NULL) {
        N = find_by_query("node[UmlPackage & out_links[UmlContains & link_refs & to_node_id = ${N.id}]]");
        if(N != NULL) {
            if(selection_count("item[UmlStereotype & value = 'subsystem' & obj_id = ${N.id}]") == 1)
                return N.name;
        }
    }

    return NULL;
}

// ECR 5140 start
boolean
existsParentOfStateMachine(node N)
{
    // find the corresponding UmlClass, UmlOperation or UseCase for the Statemachine 
    if(N.type == "UmlStateMachine") {
	while(True) {
            // ECR 6911 added "scope_node_id != ${N.id} & "
	    // N = find_by_query("node[(UmlUseCase | UmlClass | UmlOperation | UmlActionState) & node_refs[file[!UmlClassTable]] & name = '${N.name}']");
	    N = find_by_query("node[(UmlUseCase | UmlClass | UmlOperation | UmlActionState) & node_refs[file[!UmlClassTable]] & scope_node_id != ${N.id} & name = '${N.name}']");
	    if(N == NULL)
		return False;
	    if(N.type != "UmlActionState")
		break;
	    N = find_by_query("node[id = ${N.scope_node_id}]");
	    if(N == NULL)
		return False;
	}

        // climb up the scope (UmlOperation)
        while(N.scope_node_id != 0) {
            SN = find_by_query("node[id = ${N.scope_node_id}]");
            if(SN != NULL) // should be always true ?
                N = SN;
        }
    }

    return True;
}
// ECR 5140 end

// finds the model management parent of a node.
node
computeSubsystemParentOfNode(node N)
{
    // find the corresponding UmlClass, UmlOperation or UseCase for the Statemachine 
    if(N.type == "UmlStateMachine") {
	// ECR 5080 start
	while(True) {
            // ECR 6911 added "scope_node_id != ${N.id} & "
	    // N = find_by_query("node[(UmlUseCase | UmlClass | UmlOperation | UmlActionState) & node_refs & name = '${N.name}']");
	    N = find_by_query("node[(UmlUseCase | UmlClass | UmlOperation | UmlActionState) & node_refs & scope_node_id != ${N.id} & name = '${N.name}']");
	    if(N == NULL)
		return NULL;
	    if(N.scope_node_id == 0)
                return N;
	    if(N.type != "UmlActionState")
		break;
	    N = find_by_query("node[id = ${N.scope_node_id}]");
	    if(N == NULL) // should never happen
		return NULL;
	}
	// ECR 5080 end
    }
    // find the corresponding UmlClass for the UmlObjectClassScope 
    else if(N.type == "UmlObjectClassScope") {
	return find_by_query("node[UmlClass & node_refs & name = '${N.name}']");
    }

    // climb up the scope
    node SN;
    while(N.scope_node_id != 0) {
	SN = find_by_query("node[id = ${N.scope_node_id}]");
	if(SN != NULL) // should be always true ?
            N = SN;
    }

    return N;
}

// is subsystem x owned by subsystem y
boolean // ECR 7725
isSubsystemContainedBySubsystem(node N, string Sub)
{
    // climb up the UmlContains link
    while(N != NULL) {
        N = find_by_query("node[UmlPackage & out_links[UmlContains & link_refs & to_node_id = ${N.id}]]");
        if(N != NULL) {
            if(selection_count("item[UmlStereotype & value = 'subsystem' & obj_id = ${N.id}]") == 1) {
                if(N.name == Sub)
                    return True;
            }
        }
    }

    return False;
}

// find one psymid of an oms_node
int
omsNodeToPsymId(node N)
{
    list symbols = gde_all_symbols();
    list nodes;
    gde_symbol symbol;
    gde_node gnode;
    node n;
    int i, j;
    for (i = 0; i < list_count(symbols); ++ i) {
        symbol = list_get(symbols, i);
        if(gde_symbol_is_node(symbol) || gde_symbol_is_cntx(symbol)) {
            gnode = to_gde_node(symbol);
            nodes = gde_node_oms_nodes(gnode);
            for(j = 0; j < list_count(nodes); ++ j) {
                n = list_get(nodes, j);
                if(n.id == N.id)
                    return gde_node_psymid(gnode);
            }
        }
    }

    return 0;
}

// find one psymid of an oms_link
int
omsLinkToPsymId(link L)
{
    list symbols = gde_all_symbols();
    list links;
    gde_symbol symbol;
    gde_arc garc;
    link l;
    int i, j;
    for (i = 0; i < list_count(symbols); ++ i) {
        symbol = list_get(symbols, i);
        if(gde_symbol_is_arc(symbol)) {
            garc = to_gde_arc(symbol);
            links = gde_link_oms_links(list_get(gde_arc_links(garc), 0));
            for(j = 0; j < list_count(links); ++ j) {
                l = list_get(links, j);
                if(l.id == L.id)
                    return gde_arc_psymid(garc);
            }
        }
    }

    return 0;
}
// ECR 5114 end
