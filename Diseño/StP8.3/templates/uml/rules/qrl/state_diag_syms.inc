
//      StP/UML
//      Confidential property of Aonix
//      Copyright (c) 1997
//      All rights reserved
//
// Functions for symbols belonging to state and activity diagram editors

#include	"rules/qrl/ustated_transition.inc"
#include	"rules/qrl/oper_parse.inc"
#include	"rules/qrl/object_instance.inc" // ECR 7223

const string    UML_STATE_MACHINE_SYMBOL_TYPE = "StateMachine";
const string    UML_STATE_SYMBOL_TYPE = "State";
const string    UML_COMPOSITE_STATE_SYMBOL_TYPE = "CompositeState";
const string    UML_INITIAL_STATE_SYMBOL_TYPE = "InitialState";
const string    UML_FINAL_STATE_SYMBOL_TYPE = "FinalState";
const string    UML_SPLIT_CONTROL_SYMBOL_TYPE = "SplitControl";
const string    UML_MERGE_CONTROL_SYMBOL_TYPE = "MergeControl";
const string    UML_SHALLOW_HISTORY_SYMBOL_TYPE = "History";
const string    UML_DEEP_HISTORY_SYMBOL_TYPE = "DeepHistory";
const string    UML_OBJECT_SYMBOL_TYPE = "Object";
const string    UML_STATE_TRANS_LINK_SYMBOL_TYPE = "StateTransitionLink";
const string    UML_SPLIT_CONTROL_LINK_SYMBOL_TYPE = "SplitControlLink";
const string    UML_MERGE_CONTROL_LINK_SYMBOL_TYPE = "MergeControlLink";

list
uml_state_machine_to_uclassd(string state_machine)
{
    list targets = list_create("all",0);
    string target_class;
    
    if(uml_state_machine_is_class(state_machine))
        target_class = state_machine;
    else
        target_class = uml_state_machine_class_name(state_machine);
    
    targets = 
    list_select("node_ref[node[name = '${target_class}' && UmlClass] &&
file[UmlClassDiagram]]");
    
    return targets;
    
}


list
uml_state_machine_to_operation(string state_machine)
{
    list targets = list_create("node_ref",0);
    string target_operation;
    string class;
    uml_operation parsed_operation;
    
    if(uml_state_machine_is_class(state_machine))
    {
        print_error("Can't Navigate to Operation from Class State Machine.");
        return targets;
    }

    target_operation = 
    uml_state_machine_canonical_operation(state_machine);
    parsed_operation = uml_parse_operation(target_operation);

    class = uml_state_machine_class_name(state_machine);
    
    if(target_operation != NULL)
        targets = list_select("node_ref[node[name = '${parsed_operation.opName}' &&
UmlOperation] && scope_node_ref[node[name = '${class}']]]");
    
    return targets;
    
}


list
uml_state_machine_to_uusecased(string state_machine)
{
    list targets = list_create("all",0);
    string target_usecase;
    
    if(uml_state_machine_is_class(state_machine))
        target_usecase = state_machine;
    else
        target_usecase = uml_state_machine_class_name(state_machine);
    
    //message("DEBUG uml_state_machine_to_uusecased:: target_usecase='"+target_usecase+"'");
    targets = 
    list_select("node_ref[node[name = '${target_usecase}' && UmlUseCase]]");
    
    return targets;
}

list
uml_state_machine_to_uclasst(string state_machine)
{
    list targets = list_create("all",0);
    string target_class;
    
    if(uml_state_machine_is_class(state_machine))
        target_class = state_machine;
    else
        target_class = uml_state_machine_class_name(state_machine);
    
    targets = 
    list_select("node_ref[node[name = '${target_class}' && UmlClass] &&
file[UmlClassTable]]");
    
    return targets;
    
}


list
uml_state_machine_to_usequenced(string state_machine)
{
    list targets = list_create("all",0);
    string target_class;
    
    if(uml_state_machine_is_class(state_machine))
        target_class = state_machine;
    else
        target_class = uml_state_machine_class_name(state_machine);

    targets = list_select("node_ref[node[name = '${target_class}' && UmlObjectClassScope] && file[UmlSequenceDiagram]]");
    return targets;
}


list
uml_state_machine_to_ucollaborationd(string state_machine)
{
    list targets = list_create("all",0);
    string target_class;
    
    if(uml_state_machine_is_class(state_machine))
        target_class = state_machine;
    else
        target_class = uml_state_machine_class_name(state_machine);
    
    //message("DEBUG uml_state_machine_to_ucollaborationd:: target_class='"+target_class+"'");
    targets = list_select("node_ref[node[name = '${target_class}' && UmlObjectClassScope] && file[UmlCollaborationDiagram]]");
    return targets;
    
}


void
UmlStateDiagramInitFromClass(int oid)
{
    node class = find_by_query("node[UmlClass && id = ${oid}]");
    gde_node state_machine;
    int dodefer,id;
    string filename;

    EditorShowOnNavReceive();
    
    if(class == NULL)
	{
	    print_error("No class node with id " + oid + ".");
	    return;
	}
    filename = create_new_file_in_sequence(class.name,NULL);
    gde_builtin("FileLoad " + filename);

    gde_builtin("BufferFindOpt -center");    

    gde_save_state();
    dodefer = gde_start_edit_transaction();
    state_machine =  gde_node_create(UML_STATE_MACHINE_SYMBOL_TYPE,
				  0, 0);
    gde_node_set_label(state_machine, class.name);
    gde_end_edit_transaction(GDE_CMD_INSERT, dodefer, False);
    /* center the buffer */
    gde_builtin("BufferZoom FitDiagram");
    
    /* save all this work we did */

    gde_builtin("FileSaveas " + filename);
    id = gde_node_psymid(state_machine);
    gde_builtin("BufferFind " + id);

}

void
UmlStateDiagramInitFromUseCase(int oid)
{
    node use_case = find_by_query("node[UmlUseCase && id = ${oid}]");
    gde_node state_machine;
    int dodefer,id;
    string filename;

    EditorShowOnNavReceive();
    
    if(use_case == NULL)
	{
	    print_error("No use case node with id " + oid + ".");
	    return;
	}
    filename = create_new_file_in_sequence(use_case.name,NULL);
    gde_builtin("FileLoad " + filename);

    gde_builtin("BufferFindOpt -center");    

    gde_save_state();
    dodefer = gde_start_edit_transaction();
    state_machine =  gde_node_create(UML_STATE_MACHINE_SYMBOL_TYPE,
				  0, 0);
    gde_node_set_label(state_machine, use_case.name);

    gde_end_edit_transaction(GDE_CMD_INSERT, dodefer, False);
    /* center the buffer */
    gde_builtin("BufferZoom FitDiagram");
    
    /* save all this work we did */

    gde_builtin("FileSaveas " + filename);
    id = gde_node_psymid(state_machine);
    gde_builtin("BufferFind " + id);
    
}

void
UmlStateDiagramInitFromOperation(int oid)
{
    node operation = find_by_query("node[UmlOperation && id = ${oid}]");
    node class;
    gde_node state_machine;
    int dodefer,id;
    string filename;
    
    EditorShowOnNavReceive();
    
    if(operation == NULL)
	{
	    print_error("No operation node with id " + oid + ".");
	    return;
	}
    
    class = find_by_query("node[id = ${operation.scope_node_id}]");
    filename = create_new_file_in_sequence(class.name,NULL);
    gde_builtin("FileLoad " + filename);

    gde_builtin("BufferFindOpt -center");       
    gde_save_state();
    dodefer = gde_start_edit_transaction();
    state_machine =  gde_node_create(UML_STATE_MACHINE_SYMBOL_TYPE,
				     0, 0);
    gde_node_set_label(state_machine, uml_state_machine_name(operation));
    gde_end_edit_transaction(GDE_CMD_INSERT, dodefer, False);
    /* center the buffer */
    gde_builtin("BufferZoom FitDiagram");
    
    /* save all this work we did */
    gde_builtin("FileSaveas " + filename);
    id = gde_node_psymid(state_machine);
    gde_builtin("BufferFind " + id);
}

string
uml_state_name_func(gde_mapsymbol obj)
{
    gde_node gn = to_gde_node(obj);
    string result = gde_node_label(gn);
    
    if (result == "")
        return NULL;
    
    return replace_newlines(result," ");
}

string
uml_state_machine_name_func(gde_mapsymbol obj)
{
    gde_node gn = to_gde_node(obj);
    string label = gde_node_label(gn);
    string fixed_class_name = NULL;
    string class_name;
    string operation_name_sig = NULL;
    boolean replace_label = False;
    string state_machine_name;
    
    if (label == "")
        return NULL;
    
    class_name = uml_state_machine_class_name(label);
    fixed_class_name = 
	strip_whitespace(string_translate(class_name,UML_BAD_CH,UML_GOOD_CH));
    
    if(fixed_class_name != class_name)
	{
	    gde_print_error("Class `" + class_name
			    + "' has invalid characters. Fixing name ...",
			    gde_node_psymid(gn));
	    replace_label = True;
	}
    
    
    if(!uml_state_machine_is_class(label))
	{
	    operation_name_sig = uml_state_machine_canonical_operation(label);
	    if(operation_name_sig != NULL)
		state_machine_name = 
		    fixed_class_name + UML_PATH_SEPARATOR + operation_name_sig;
	    else
		{
		    gde_print_error("State Machine has syntax errors.",
				    gde_node_psymid(gn));
		    return NULL;
		    
		}
	}
    else
	state_machine_name = fixed_class_name;
    
    if(replace_label && !gde_rename_in_progress())
	gde_node_set_label(gn,state_machine_name);
    return state_machine_name;
}

gde_node
uml_state_machine()
{
    list nodes;
    
    nodes = gde_nodes_find_by_application_tag(gde_all_symbols(),
					      UML_STATE_MACHINE_SYMBOL_TYPE);
    if(list_count(nodes) == 0)
	return NULL;
    
    return list_get(nodes,0);
    
}

int uml_check_state_machine_semantics()
{
    gde_node sm = uml_state_machine();
    string label;
    string class_name;
    string operation_name;
    list path_components;
    uml_operation parsed_operation;
    int pos;
    int err_count = 0;

    if(sm == NULL)
	return err_count;

    label = gde_node_label(sm);
    if(label == NULL || label == "")
	return err_count;
	
    print_message("Checking State Machine...");

    if(uml_state_machine_is_class(label))
	{
	    if(find_by_query("node[UmlClass && name = '" + label + "']") == NULL)
		{
		    gde_print_error("No Class " + label + " corresponding to State Machine",gde_node_psymid(sm));
		    err_count++;
		}
	}
    else
	{	
	    //It must correspond to an operation
	    pos = string_find(label,0,UML_PATH_SEPARATOR);
	    class_name = string_extract(label,0,pos);
	    pos = pos + string_length(UML_PATH_SEPARATOR);
	    if(pos >= string_length(label))
		return err_count;

	    operation_name = 
		string_extract(label,pos,
			       string_length(label) - pos);
	    
	    parsed_operation = 
		uml_parse_operation(operation_name);
	    
	    if(parsed_operation.error)
		return err_count;

	    if(find_by_query("node[UmlOperation && sig = '${parsed_operation.opArgs}' && name = '${parsed_operation.opName}' && scope_node[name = '${class_name}']]") == NULL)
		{	
		    gde_print_error("No Operation " + label + " corresponding to State Machine",gde_node_psymid(sm));
		    err_count++;
		}
	}
    return err_count;
}

boolean uml_check_state_machine_syntax(string diag)
{
    boolean clean = True;
    list nodes;
    gde_node state_machine;
    int count;

    nodes = gde_nodes_find_by_application_tag(gde_all_symbols(),
					      UML_STATE_MACHINE_SYMBOL_TYPE);
    count = list_count(nodes);

    if(count == 0)
	{
	    print_error(diag + " Diagrams must have a state machine");	
	    clean = False;
	}
    
    if(count > 1)
	{	   
	    print_error(diag + " Diagrams must have one state machine");	
	    clean = False;
	}
    
    return clean;
}

gde_mapsymbol uml_state_machine_symbol(gde_mapsymbol sym)
{
    gde_node state_machine;
    state_machine = uml_state_machine();

    if (state_machine != NULL)
	return to_gde_mapsymbol(state_machine);
    return NULL;
}

// Begin change ECR 7223
string
uml_split_control_name(gde_mapsymbol the_sym)
{
    string node_name = uml_gde_node_unique_name( to_gde_node( the_sym ), "", 
        "UmlSplitControl", False );
    return( node_name );
}

//string
//uml_split_control_name(gde_mapsymbol split_control_sym)
//{
//  /* the name of the split control node is the name of the origin 
//     state for the incoming transition */
//
//  gde_link transition;
//
//  transition = uml_split_control_incoming_transition(to_gde_node(split_control_sym));
//  if (transition == NULL)
//    return (NULL);
//
//  return (replace_newlines(strip_whitespace(gde_node_label(gde_link_fromnode(transition))), " "));
//}
//
//string
//uml_split_control_signature(gde_mapsymbol split_control_sym)
//{
//  /* the signature of the split control node is the label of the incoming transition */
//
//  gde_link transition;
//  list arcs;
//  int i;
//  string label = "";
//
//  transition = uml_split_control_incoming_transition(to_gde_node(split_control_sym));
//  if (transition == NULL)
//    return (NULL);
//
//  arcs = gde_link_arcs(transition);
//  for (i = 0; i < list_count(arcs); i = i + 1) {
//    label = gde_arc_label(list_get(arcs, i));
//    if (label != NULL && label != "")
//      break;
//  }
//
//  return (replace_newlines(label, " "));
//}
// End change ECR 7223

gde_link
uml_split_control_incoming_transition(gde_node split_control)
{
  list inarcs, links;
  gde_arc arc;
  int i;

  inarcs = gde_node_inarcs(split_control);
  for (i = 0; i < list_count(inarcs); i = i + 1) {
    arc = list_get(inarcs, i);  
    if (gde_lt_name(gde_arc_lt(arc)) == UML_STATE_TRANS_LINK_SYMBOL_TYPE) {
      links = gde_arc_links(arc);
      if (list_count(links) > 0)
	return list_get(links, 0);
    }
  }
  return (NULL);
}


list
uml_split_control_outgoing_transitions(gde_node split_control)
{
  list outarcs, links, transition_links;
  gde_arc arc;
  int i, j;

  transition_links = list_create("gde_link", 0);
  outarcs = gde_node_outarcs(split_control);

  for (i = 0; i < list_count(outarcs); i = i + 1) {
    arc = list_get(outarcs, i);  
    if (gde_lt_name(gde_arc_lt(arc)) == UML_SPLIT_CONTROL_LINK_SYMBOL_TYPE) {
      links = gde_arc_links(arc);
      for (j = 0; j < list_count(links); j = j + 1)
	list_append(transition_links, list_get(links, j));
    }
  }
  return (transition_links);
}

// Begin change ECR 7223
string
uml_merge_control_name(gde_mapsymbol the_sym)
{
    string node_name = uml_gde_node_unique_name( to_gde_node( the_sym ), "", 
        "UmlMergeControl", False );
    return( node_name );
  }

//string
//uml_merge_control_name(gde_mapsymbol merge_control_sym)
//{
//  list transitions;
//  gde_link transition;
//  string name;
//  int i;
//
//  /* the name of the merge control node is the concatenation of the names of the origin states for the incoming transitions */
//
//  transitions = uml_merge_control_incoming_transitions(to_gde_node(merge_control_sym));
//  if (list_count(transitions) == 0)
//    return (NULL);
//
//  transition = list_get(transitions, 0);
//  name = gde_node_label(gde_link_fromnode(transition));
//  for (i = 1; i < list_count(transitions); i = i + 1) {
//    transition = list_get(transitions, i);
//    name = name + "," + strip_whitespace(gde_node_label(gde_link_fromnode(transition)));
//  }
//
//  return (replace_newlines(name, " "));
//}
//
//string
//uml_merge_control_signature(gde_mapsymbol merge_control_sym)
//{
//  /* the signature of the merge control node is the concatenation of the labels of the incoming transitions */
//
//  list transitions, arcs;
//  gde_link transition;
//  string sig = "", label = "";
//  int i, j;
//
//  transitions = uml_merge_control_incoming_transitions(to_gde_node(merge_control_sym));
//  if (list_count(transitions) == 0)
//    return (NULL);
//
//  transition = list_get(transitions, 0);
//  arcs = gde_link_arcs(transition);
//  for (j = 0; j < list_count(arcs); j = j + 1) {
//    label = gde_arc_label(list_get(arcs, j));
//    if (label != NULL && label != "") {
//      sig = label;
//      break;
//    }
//  }
//  
//  for (i = 1; i < list_count(transitions); i = i + 1) {
//    transition = list_get(transitions, i);
//    arcs = gde_link_arcs(transition);
//    for (j = 0; j < list_count(arcs); j = j + 1) {
//      label = gde_arc_label(list_get(arcs, j));
//      if (label != NULL && label != "") {
//	sig = sig + "," + label;
//	break;
//      }
//    }
//  }
//
//  return (replace_newlines(sig, " "));
//}
// End change ECR 7223

list
uml_merge_control_incoming_transitions(gde_node merge_control)
{
  list inarcs, links, transitions;
  gde_arc arc;
  int i, j;

  transitions = list_create("gde_link", 0);
  inarcs = gde_node_inarcs(merge_control);

  for (i = 0; i < list_count(inarcs); i = i + 1) {
    arc = list_get(inarcs, i);  
    if (gde_lt_name(gde_arc_lt(arc)) == UML_STATE_TRANS_LINK_SYMBOL_TYPE) {
      links = gde_arc_links(arc);
      for (j = 0; j < list_count(links); j = j + 1)
	list_append(transitions, list_get(links, j));
    }
  }
  return (transitions);
}

gde_link
uml_merge_control_outgoing_transition(gde_node merge_control)
{
  list outarcs, links;
  gde_arc arc;
  int i;

  outarcs = gde_node_outarcs(merge_control);
  for (i = 0; i < list_count(outarcs); i = i + 1) {
    arc = list_get(outarcs, i);  
    if (gde_lt_name(gde_arc_lt(arc)) == UML_MERGE_CONTROL_LINK_SYMBOL_TYPE) {
      links = gde_arc_links(arc);
      if (list_count(links) > 0) 
	return list_get(links, 0);
    }
  }
  
  return (NULL);
}


list state_machine_symbolic_dependents(gde_mapsymbol ms)
{
    list symbols = gde_all_symbols();
    list dependents = list_create("gde_mapsymbol", 0);
    int i,j;
    gde_node gn;
    gde_link gl;
    gde_cntx gc;
    gde_symbol gs;
    gde_mapsymbol dependent;
    list links;

    if(list_count(symbols) == 1)
	return dependents;

    for(i = 0;i < list_count(symbols);i++)
	{
	    gs = list_get(symbols,i);
	    if(gde_symbol_is_node(gs))
		{
                    if(gde_nt_name(gde_node_nt(to_gde_node(gs))) ==
                       UML_OBJECT_SYMBOL_TYPE)
                        continue;

		    dependent = to_gde_mapsymbol(to_gde_node(gs));
		    if(ms == dependent)
			continue;
		    list_append(dependents,dependent);
		}
	    else if(gde_symbol_is_arc(gs))
		{
		    links = gde_arc_links(to_gde_arc(gs));
		    for(j = 0;j < list_count(links);j++)
			{
			    dependent = to_gde_mapsymbol(list_get(links,j));
			    list_append(dependents,dependent);
			}
		}
	}
	    
    return dependents;
}

//
// STATE TRANSITION CHECKING
//

list
uml_make_trans_appids(list l, int v)
{
    list	res = list_create("int", 0);
    int		i;
    UMLParsedTransition	pt;
    if (v != NULL)
    {
	list_append(res, v);
    }
    for(i = 0; i < list_count(l); i = i + 1)
    {
	pt = list_get(l, i);
	if (pt.symid != NULL && pt.symid != 0)
	{
	    list_append(res, pt.symid);
	}
    }
    return(res);
}

struct		dup_pair_tp
{
	UMLParsedTransition	pt;
	UMLParsedTransition	pt1;
};



void
ustated_show_nondeterministic_errors(list dups, string msg)
{
    UMLParsedTransition		pt, pt1;
    int				i, cnt;
    list			t, dup;
    dup_pair_tp			p;
    list			duplicates;

    dup = list_copy(dups);
    cnt = list_count(dup);

    if (msg == NULL)
    {
	msg = "Non-deterministic Transitions from State";
    }

    while(cnt > 0)
    {
	p = list_get(dup, 0);
	pt = p.pt;
	pt1 = p.pt1;
	duplicates = list_create("UMLParsedTransition", 0);
	list_append(duplicates, p.pt1);
	list_append(duplicates, p.pt);
	list_delete(dup, 0);
	t = list_create("dup_pair_tp", 0);
	for(i = 0; i < list_count(dup); i = i + 1)
	{
	    p = list_get(dup, i);
	    if (p.pt == pt || p.pt1 == pt)
	    {
		list_append(duplicates, p.pt1);
		list_append(duplicates, p.pt);
	    } else if (p.pt == pt1 || p.pt1 == pt1)
	    {
		list_append(duplicates, p.pt1);
		list_append(duplicates, p.pt);
	    } else {
		list_append(t, p);
	    }
	}
	/* non-determinism error */
	gde_print_error(msg, uml_make_trans_appids(duplicates, 0));
	dup = t;
	cnt = list_count(dup);
    }
}

int
uml_check_transitions()
{
  list symbols;
  gde_symbol symbol;
  gde_node node;
  string node_type;
  int i,err_count = 0;

  print_message("Checking State Transitions...");

  /* map through the symbols, searching for states which allow outgoing transitions */
  symbols = gde_all_symbols();
  for (i = 0; i < list_count(symbols); i = i + 1) {
    symbol = list_get(symbols, i);
    if (gde_symbol_is_node(symbol)) {
      node = to_gde_node(symbol);
      node_type = gde_nt_name(gde_node_nt(node));
      if (uml_state_has_outgoing_transition(node_type))

	/* it is a state which allows outgoing transitions: check it */
	if (!uml_state_has_valid_outgoing_transitions(node)) 
	  err_count++;
        else
	  if (!uml_state_has_valid_outgoing_transitions_from_repos(node)) 
	    err_count++;
    }
  }

  return err_count;
}

boolean
uml_state_has_valid_outgoing_transitions(gde_node state)
{
  list arcs, links;
  list parsed_transitions, duplicates, dups;
  gde_link link;
  gde_arc arc;
  UMLParsedTransition pt;
  int i, j;
  dup_pair_tp	dup;

  /* default assumption: outgoing transitions are valid */
  boolean rc = True;

  dups = list_create("dup_pair_tp", 0);

  /* create a list, maintained in sorted order, to check for non-deterministic transitions */
  parsed_transitions = list_create("UMLParsedTransition", 0);

  /* search the outgoing transitions */
  arcs = gde_node_outarcs(state);
  for (i = 0; i < list_count(arcs); i = i + 1) {
    arc = list_get(arcs, i);
    if (gde_lt_name(gde_arc_lt(arc)) == UML_STATE_TRANS_LINK_SYMBOL_TYPE) {
      links = gde_arc_links(arc);
      if (list_count(links) > 0) {
	link = list_get(links, 0);
	pt = uml_parse_transition(link);
	if (pt.unmatched_parens_error) {
	  /* syntax error */
	  gde_print_error("Transition syntax error: unmatched attribute parentheses", gde_arc_psymid(arc));
	  rc = False;
	}
	if (pt.unmatched_brackets_error) {
	  /* syntax error */
	  gde_print_error("Transition syntax error: unmatched guard brackets", gde_arc_psymid(arc));
	  rc = False;
	}
	duplicates = list_create("UMLParsedTransition", 0);
	if (!(uml_transition_is_deterministic(pt, parsed_transitions,
				duplicates)))
	{
	    dup.pt = pt;
	    if (list_count(duplicates) > 0)
	    {
		dup.pt1 = list_get(duplicates, 0);
	    } else {
		dup.p1 = NULL;
	    }
	    list_append(dups, dup);
	  rc = False;
	}
      }
    }
  }

    if (list_count(dups) > 0)
    {
	ustated_show_nondeterministic_errors(dups, NULL);
    }
  
  return rc;
}

boolean
uml_state_has_valid_outgoing_transitions_from_repos(gde_node state) 
{
    int x, y;
    int error_count = 0;

    node current_state;
    link current_transition_link;
    list transition_links_list;
    list transition_name_list = list_create("string", 0);
    string current_sorted_name;
    string previous_name;
    string msg;

    list oms_node_list = gde_node_oms_nodes(state);

    if (list_count(oms_node_list) > 0)
        current_state = list_get(oms_node_list, 0);
    else
        return True;

    transition_links_list = list_select("link[link_refs && UmlTransition && from_node[" +
                                         current_state.id + "]]");

    for (y=0; y<list_count(transition_links_list); y++)
    {
        current_transition_link = list_get(transition_links_list, y);
        list_append(transition_name_list, current_transition_link.name);
    }

    transition_name_list = list_sort(transition_name_list);
    previous_name = NULL;

    UMLParsedTransition pt; // ECR 6369
    int cnt;                // ECR 6369
    
    for (y=0; y<list_count(transition_name_list); y++)
    {
        current_sorted_name = list_get(transition_name_list, y);
        if (current_sorted_name == previous_name)
        {
	    msg = "State has Non-deterministic Transitions from References on Other Diagrams";
            gde_print_error(msg, gde_node_psymid(state));

            error_count++;
            list_clear(transition_name_list);
            break;
        }
        previous_name = current_sorted_name;
        
        // ECR 6369 begin
        pt = uml_parse_transition_label(current_sorted_name);
        cnt = count_comma_separated_expressions(pt.event);
        if (cnt > 1)
        {
            msg = "Transition link '" + current_sorted_name + "' has " + cnt + " events!";
            gde_print_error(msg);
            error_count++;
        }
        // ECR 6369 end
    }
    list_clear(transition_name_list);

    if (error_count > 0) 
        return False;
    return True;
}


boolean
uml_transition_is_deterministic(UMLParsedTransition pt, list parsed_transitions,
list duplicates)
{
  /* Inserts a parsed transition into a list of parsed transitions leaving a state.
   * Transitions are sorted by event, then by guard. 
   * Return True iff the transition has different event[guard] from all other parsed_transitions.
   */

  UMLParsedTransition pt2;
  int i;

  /* ignore transitions with syntax errors to avoid confusing the user */
  if (uml_transition_syntax_error(pt))
    return True;

  for (i = 0; i < list_count(parsed_transitions); i = i + 1) {
    pt2 = list_get(parsed_transitions, i);
    if (pt.event < pt2.event) {

      /* distinct from any other transition leaving this state */
      list_insert(parsed_transitions, i, pt);
      return True;
    }
    if (pt.event == pt2.event) {
      if (pt.guard == pt2.guard) {
	list_insert(parsed_transitions, i, pt);
	if (duplicates != NULL)
	{
	    list_append(duplicates, pt2);
	}

	/* non-determinism detected -- same event and same guard as another transition */
	return False;
      }
      if (pt.guard < pt2.guard) {

	/* distinct from any other transition leaving this state */
	list_insert(parsed_transitions, i, pt);
	return True;
      }
    }
  }

  /* distinct from any other transition leaving this state */
  list_append(parsed_transitions, pt);
  return True;
}

// return a string in the format:
//  ${eventName1} / defer\n${eventName2} / defer\n...\n${eventNameN} /  defer
//
// This is used as a display mark on states to show the events they
// defer
string
uml_state_deferred_events_dmark(int state_id)
{
    string qry = "node[in_links[UmlDeferredEventLink && from_node_id=" +
        state_id + " && link_refs]] sort name";
    node n;
    string ret="";

    for_each_in_select(qry,n)
    {
        ret += string_search_and_replace(n.name,"\n","") + " / defer\n";
    }
    return ret;
}

string
uml_action_state_deferred_events_dmark(int state_id)
{
    node actState = find_by_query("node[id=" + state_id + "]");

    if (NULL == actState)
        return "";

    node realState = find_by_query("node[UmlState && scope_node_id=${actState.scope_node_id} && name='${actState.name}' && node_refs]");

    if (NULL == realState)
        return "";

    return uml_state_deferred_events_dmark(realState.id);
}

// Begin Add ECR 3786
int
StateTransIsSelectedActiveFunc()
{
    list	symbols, tr;

    symbols = gde_selected_symbols();

    tr = gde_arcs_find_by_type(symbols, UML_STATE_TRANS_LINK_SYMBOL_TYPE);

    if (list_count(tr) == 1)
    	return ACTIVE_FUNC_IS_ACTIVE;

    return ACTIVE_FUNC_IS_INACTIVE;
}

//  ECR 6596
int
StateMachineIsSelectedActiveFunc()
{
    list	symbols, tr;

    symbols = gde_selected_symbols();

    tr = gde_nodes_find_by_type(symbols, UML_STATE_MACHINE_SYMBOL_TYPE);

    if (list_count(tr) == 1)
    	return ACTIVE_FUNC_IS_ACTIVE;

    return ACTIVE_FUNC_IS_INACTIVE;
}
// ECR 6596 end

// uactivityd/ustated specifc SymbolNameSearch function
// for state transitions it brings up the prop instead of the generic
void
uactivityd_ustated_name_search()
{
    string editor = current_editor_apptype();
    // ECR 6596 start
    if( StateMachineIsSelectedActiveFunc() ) {
        gde_builtin("PropertySheetShow StateMachineChooseNames_ps");
        StateMachineSelection_Reset();
    }
    else
    // ECR 6596 end
    if( editor == "UmlActivityDiagram" ) 
    {
        if( StateTransIsSelectedActiveFunc() )
            UmlPopUpPropSheet("uactivityd");
        else
            gde_builtin( "SymbolNameSearch" );
    }
    else if( editor == "UmlStateDiagram" )
    {
        if( StateTransIsSelectedActiveFunc() )
            UmlPopUpPropSheet("ustated");
        else
            gde_builtin( "SymbolNameSearch" );
    }
    else
        gde_builtin( "SymbolNameSearch" );        
    return;
}
// End Add ECR 3786

// ECR 7215 - begin
// 'uml_decision_outgoing_transitions' and 'uml_decision_name' moved from 'uactivityd.qrl'
list
uml_decision_outgoing_transitions(gde_node decision)
{
  list outarcs, links, transitions;
  gde_arc arc;
  int i, j;

  transitions = list_create("gde_link", 0);
  outarcs = gde_node_outarcs(decision);

  for (i = 0; i < list_count(outarcs); i = i + 1) {
    arc = list_get(outarcs, i);  
    if (gde_lt_name(gde_arc_lt(arc)) == UML_STATE_TRANS_LINK_SYMBOL_TYPE) {
      links = gde_arc_links(arc);
      for (j = 0; j < list_count(links); j = j + 1)
	list_append(transitions, list_get(links, j));
    }
  }
  return (transitions);
}

// Begin change ECR 7223
string
uml_decision_name(gde_mapsymbol the_sym)
{
    string node_name = uml_gde_node_unique_name( to_gde_node( the_sym ), "", 
        "UmlDecision", False );
    return( node_name );
}

// ECR 6929
string
uml_junction_point_name(gde_mapsymbol the_sym)
{
    string node_name = uml_gde_node_unique_name( to_gde_node( the_sym ), "", 
        "UmlJunctionPoint", False );
    return( node_name );
}

string
uml_dynamic_choice_point_name(gde_mapsymbol the_sym)
{
    string node_name = uml_gde_node_unique_name( to_gde_node( the_sym ), "", 
        "UmlDynamicChoicePoint", False );
    return( node_name );
}

//string uml_decision_name(gde_mapsymbol decision)
//{
//  list transitions, arcs;
//  gde_link transition;
//  string name = "", label = "";
//  int i, j;
//
//  transitions = uml_decision_outgoing_transitions(to_gde_node(decision));
//  if (list_count(transitions) == 0)
//    return (NULL);
//
//  transition = list_get(transitions, 0);
//  arcs = gde_link_arcs(transition);
//  for (j = 0; j < list_count(arcs); j = j + 1) {
//    label = gde_arc_label(list_get(arcs, j));
//    if (label != NULL && label != "") {
//      name = label;
//      break;
//    }
//  }
//  
//  for (i = 1; i < list_count(transitions); i = i + 1) {
//    transition = list_get(transitions, i);
//    arcs = gde_link_arcs(transition);
//    for (j = 0; j < list_count(arcs); j = j + 1) {
//      label = gde_arc_label(list_get(arcs, j));
//      if (label != NULL && label != "") {
//	name = name + "," + label;
//	break;
//      }
//    }
//  }
//
//  return (replace_newlines(name, " "));
//}    list transitions;
// End Change ECR 7223
// ECR 7215 - end
