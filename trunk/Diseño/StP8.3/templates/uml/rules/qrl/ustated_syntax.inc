//      StP/UML
//      Confidential property of Aonix
//      Copyright (c) 1997
//      All rights reserved

#include "rules/qrl/ustated_mapping.inc"

// SYNTAX CHECKING FOR USTATED 

boolean
UStateDCheckSyntax()
{
  boolean diagramIsClean = True;
  
  
  /* split control destinations are parallel */
  if (!uml_check_split_controls())
    diagramIsClean = False;

  if(!uml_check_state_machine_syntax("State"))
	  diagramIsClean = False;


  /* only one initial state per composite_state, with transitions 
     entirely contained by composite_state;
     an initial state may not have both labeled and unlabeled transitions*/
  if (!uml_check_initial_states())
    diagramIsClean = False;


  /* merge control origins are parallel */
  if (!uml_check_merge_controls())
    diagramIsClean = False;
 
  /* states may be refined into disjoint substates or concurrent 
     substates, but not both */
  if(!uml_check_nested_states())
      diagramIsClean = False;

  // Compound States can have at most of each type of history indicator
  if(!uml_check_composite_states())
      diagramIsClean = False;

  /* Now run the default gde syntax checks. */
  if (gde_default_check_syntax() == False)
    diagramIsClean = False;

  return diagramIsClean;
}

//composite states may be decomposed into disjoint states or concurrent states
//but not both

boolean
uml_check_nested_states_by_type(string compound_state_symbol_type)
{
    list composite_states = gde_nodes_find_by_type(gde_all_symbols(),
					compound_state_symbol_type);
    int i;
    int j;
    list substates;
    string label;
    gde_node composite_state;
    boolean clean = True;
    boolean concurrent_substate_found = False;
    boolean disjoint_substate_found = False;
    string node_type_name;

    for(i = 0;i < list_count(composite_states);i++)
	{
        // ECR 6599 begin
        // These two flags must be reinitialized for every composite state
        concurrent_substate_found = False;
        disjoint_substate_found = False;
        // ECR 6599 end

	    composite_state = list_get(composite_states,i);
	    substates = uml_substates(composite_state);
	    for(j = 0;j < list_count(substates);j++)
		{
		    node_type_name = 
			gde_nt_name(gde_node_nt(list_get(substates,j)));
		    if(node_type_name ==  UML_CONCURRENT_STATE_SYMBOL_TYPE)
			concurrent_substate_found = True;
		    
		    else if(node_type_name == UML_STATE_SYMBOL_TYPE || 
			    node_type_name == UML_COMPOSITE_STATE_SYMBOL_TYPE)
			disjoint_substate_found = True;

		    if(concurrent_substate_found && disjoint_substate_found)
			{
			    gde_print_error(compound_state_symbol_type + 
					    " is refined into disjoint substates and concurrent substates."
					    ,gde_node_psymid(composite_state));
			    clean = False;
			    break;
			}
		}
	}
    return clean;
}


boolean
uml_check_nested_states()
{
    boolean clean = True;
    if(!uml_check_nested_states_by_type(UML_CONCURRENT_STATE_SYMBOL_TYPE))
	clean = False;

    if(!uml_check_nested_states_by_type(UML_COMPOSITE_STATE_SYMBOL_TYPE))
	clean = False;
	
    return clean;
       
}
//
// INITIAL STATE CHECKING
//

struct	initial_states_tp
{
	gde_node	state;
	int		parent;
};

/* check all the initial states */

boolean
uml_check_initial_states()
{
    list		parents = list_create("initial_states_tp", 0);
    list		t;
    list		states;
    initial_states_tp	is;
    int			parent, cnt, i;
    list		initial_states;
    gde_node		state;
    boolean		rc = True;

    initial_states = gde_nodes_find_by_type(gde_all_symbols(),
					UML_INITIAL_STATE_SYMBOL_TYPE);
    for (i = 0; i < list_count(initial_states); i = i + 1)
    {
	state = list_get(initial_states, i);
	if (!(uml_check_initial_state(state, parents)))
	{
	    rc = False;
	}
    }

    // build partion of initial states per parent ....
    // Now check if two transitions from start states inside the same
    // parent have non-deterministic transitions

    cnt = list_count(parents);
    while(cnt > 0)
    {
	is = list_get(parents, 0);
	states = list_create("gde_node", 0);
	list_append(states, is.state);
	parent = is.parent;
	list_delete(parents, 0);
	t = list_create("initial_states_tp", 0);
	for(i = 0; i < list_count(parents); i = i + 1)
	{
	    is = list_get(parents, i);
	    if (is.parent == parent)
	    {
		list_append(states, is.state);
	    } else {
		list_append(t, is);
	    }
	}
	if (uml_initial_states_have_deterministic_transitions(states) == False)
	{
	    rc = False;
	}
	parents = t;
	cnt = list_count(parents);
    }

    return rc;
}

// parents are used to not check multiple siblings in same context twice

boolean
uml_check_initial_state(gde_node initial_state, list parents)
{
  /* 
   * Returns True iff an initial state is valid.  The followin constraint is
   *	checked
   * The outgoing transition must stay within the composite_state.
   */
    list arcs, links;
    gde_link link;
    gde_arc arc;
    gde_node composite_state, substate, tonode;
    int i, j;
    boolean rc = True;
    initial_states_tp	is;
    boolean unlabeled_transition_found = False;
    boolean labeled_transition_found = False;
    
  /* search for a composite_state (i.e., find a state at the other end of an incoming scope link) */
    composite_state = uml_state_get_composite_state(initial_state);
    
    if (composite_state)
	{
	    is.state = initial_state;
	    is.parent = gde_node_psymid(composite_state);
	    list_append(parents, is);   
	}

    /* check for labeled and unlabeled transitions*/
    arcs = gde_node_outarcs(initial_state);
    for (i = 0; i < list_count(arcs); i = i + 1) {
	arc = list_get(arcs, i);
	if (gde_lt_name(gde_arc_lt(arc)) == 
	    UML_STATE_TRANS_LINK_SYMBOL_TYPE)
	    {
		links = gde_arc_links(arc);
		if (list_count(links) > 0) {
		    link = list_get(links,0);

		    /* transitions leaving an initial state must not
		       cross the contour of
		       the composite_state*/
		    if(composite_state && 
		       (uml_state_get_composite_state(gde_link_tonode(link)) !=
			composite_state))
			{
			    gde_print_error("Transitions leaving an initial state must stay within the parent state", gde_arc_psymid(arc));
			    rc = False;
			}
		    if(gde_link_label(link) == "" 
		       || gde_link_label(link) == NULL)
			unlabeled_transition_found = True;
		    else
			labeled_transition_found = True;
		}
	    }
    }
        
    
    if(unlabeled_transition_found && labeled_transition_found)
	{
	    gde_print_error("An initial state may not have both labeled and unlabeled transitions", gde_node_psymid(initial_state));
	    rc = False;
	}
    
    return rc;
}   


//
// CONCURRENCY-RELATED CHECKING
// (concurrent subregions, split control and merge control)
//

//
// concurrent subregions
//


//
// split and merge control
//

/* encodes the nesting of a state involved in merges or splits */
struct UMLStateConcurrency
{
  int state;
  int concurrent_parent;
  int sequential_parent;
};

boolean
uml_check_split_controls()
{
  list split_controls;
  gde_node split;
  int i;
  boolean rc = True;

  /* find all the initial states and check that they are unique-in-scope */
  split_controls = gde_nodes_find_by_type(gde_all_symbols(), UML_SPLIT_CONTROL_SYMBOL_TYPE);
  for (i = 0; i < list_count(split_controls); i = i + 1) {
    split = list_get(split_controls, i);
    if (!(uml_check_split_control(split))) {
      rc = False;
    }
  }

  return rc;
}

boolean
uml_check_split_control(gde_node split)
{
  list outlinks, concurrencies;
  UMLStateConcurrency concurrency;
  gde_node state;
  int i;
  boolean rc = True;

  /* get all the outgoing arcs */
  outlinks = uml_split_control_outgoing_transitions(split);

  /* maintain a list of <state, concurrent parent, sequential parent> elements for each destination state */
  concurrencies = list_create("UMLStateConcurrency", 0);
  for (i = 0; i < list_count(outlinks); i = i + 1) {
    state = gde_link_tonode(list_get(outlinks, i));

    /* get <state, concurrent parent, sequential parent> */
    concurrency = uml_state_concurrency(state);

    if (concurrency.concurrent_parent == 0) {
      
      /* not in a concurrent state */
      gde_print_error("Split control destination state must be concurrent or nested within a concurrent state",
		      gde_node_psymid(state));
      rc = False;

      /* don't bother to include this in the list of concurrent destinations */
      continue;
    }

    if (!(uml_state_is_concurrent(concurrency, concurrencies))) {

      /* not concurrent with other destination states */
      gde_print_error("Split control destination state must be concurrent with other split control destination states",
		      gde_node_psymid(state));
      rc = False;
    }
  }
  return rc;
}

boolean
uml_check_merge_controls()
{
  list merge_controls;
  gde_node merge;
  int i;
  boolean rc = True;

  /* find all the initial states and check that they are unique-in-scope */
  merge_controls = gde_nodes_find_by_type(gde_all_symbols(), UML_MERGE_CONTROL_SYMBOL_TYPE);
  for (i = 0; i < list_count(merge_controls); i = i + 1) {
    merge = list_get(merge_controls, i);
    if (!(uml_check_merge_control(merge))) {
      rc = False;
    }
  }
  return rc;
}

boolean
uml_check_merge_control(gde_node merge)
{
  list outlinks, concurrencies;
  UMLStateConcurrency concurrency;
  gde_node state;
  int i;
  boolean rc = True;

  /* get all the outgoing arcs */
  outlinks = uml_merge_control_incoming_transitions(merge);

  /* maintain a list of <state, concurrent parent, sequential parent> elements for each destination state */
  concurrencies = list_create("UMLStateConcurrency", 0);
  for (i = 0; i < list_count(outlinks); i = i + 1) {
    state = gde_link_fromnode(list_get(outlinks, i));

    /* get <state, concurrent parent, sequential parent> */
    concurrency = uml_state_concurrency(state);

    if (concurrency.concurrent_parent == 0) {
      
      /* not in a concurrent state */
      gde_print_error("Merge control origin state must be concurrent or nested within a concurrent state",
		      gde_node_psymid(state));
      rc = False;

      /* don't bother to include this in the list of concurrent destinations */
      continue;
    }

    if (!(uml_state_is_concurrent(concurrency, concurrencies))) {

      /* not concurrent with other destination states */
      gde_print_error("Merge control origin state must be concurrent with other merge control destination states",
		      gde_node_psymid(state));
      rc = False;
    }
  }

  return rc;
}

UMLStateConcurrency
uml_state_concurrency(gde_node state)
{
  /* 
   * the concurrency of a state includes the following pieces of information:
   * - the id of the state
   * - the id of the smallest concurrent state containing the state (the state itself if it is concurrent)
   * - the id of the smallest sequential state containing the concurrent state
   *
   * this information can be used to determine if a state is parallel with another state
   *
   */

  UMLStateConcurrency concurrency;
  gde_node parent;
  string parent_type;

  concurrency.state = 0;
  concurrency.concurrent_parent = 0;
  concurrency.sequential_parent = 0;
  
  /* null state */
  if (state == NULL)
    return concurrency;

  /* set state */
  concurrency.state = gde_node_psymid(state);

  /* get concurrent parent -- walk up the nesting chain until concurrent subregion is found */
  parent = state;
  parent_type = gde_nt_name(gde_node_nt(parent));
  while (parent && parent_type != UML_CONCURRENT_STATE_SYMBOL_TYPE) {
    parent = uml_state_get_composite_state(parent);
    if (parent != NULL) 
      parent_type = gde_nt_name(gde_node_nt(parent));
  }
  if (parent != NULL) 
    concurrency.concurrent_parent = gde_node_psymid(parent);

  /* get sequential parent -- walk up the nesting chain until sequential state is found */
  while (parent && !(parent_type == UML_STATE_SYMBOL_TYPE || parent_type == UML_COMPOSITE_STATE_SYMBOL_TYPE)) {
    parent = uml_state_get_composite_state(parent);
    if (parent != NULL) 
      parent_type = gde_nt_name(gde_node_nt(parent));
  }
  if (parent != NULL) 
    concurrency.sequential_parent = gde_node_psymid(parent);

  return concurrency;
}

boolean
uml_state_is_concurrent(UMLStateConcurrency concurrency, list concurrencies)
{
  /* 
   * Adds an uml state concurrency into a list of state concurrencies.
   * Returns True iff 
   * - there is a concurrent parent
   * - this parent is different from all of the other concurrent parents
   * - the sequential parent is the same as all the other sequential parents
   */ 

  UMLStateConcurrency c2;
  int i;
  boolean rc = True;
  
  if (concurrency.concurrent_parent == 0)
    /* should have already checked for this in the caller, but just to be safe */
    return False;
  
  for (i = 0; i < list_count(concurrencies); i = i + 1) {
    c2 = list_get(concurrencies, i);
    if (concurrency.concurrent_parent == c2.concurrent_parent || 
	concurrency.sequential_parent != c2.sequential_parent) {
      rc = False;
      break;
    }
  }
    
  list_append(concurrencies, concurrency);
  return rc;
}

boolean
uml_initial_states_have_deterministic_transitions(list i_states)
{
    list	arcs, links;
    list	parsed_transitions, duplicates, dups;
    gde_node	state;
    gde_link	link;
    gde_arc	arc;
    UMLParsedTransition pt;
    int		i, j, k;
    dup_pair_tp	dup;

    /* default assumption: outgoing transitions are valid */
    boolean rc = True;


    /* create a list, maintained in sorted order,
     * to check for non-deterministic transitions
     */
    
    parsed_transitions = list_create("UMLParsedTransition", 0);
    dups = list_create("dup_pair_tp", 0);

    for(k = 0; k < list_count(i_states); k = k + 1)
    {
	state = list_get(i_states, k);
	/* search the outgoing transitions */
	arcs = gde_node_outarcs(state);
	for (i = 0; i < list_count(arcs); i = i + 1)
	{
	    arc = list_get(arcs, i);
	    if (gde_lt_name(gde_arc_lt(arc)) == UML_STATE_TRANS_LINK_SYMBOL_TYPE)
	    {
		links = gde_arc_links(arc);
		if (list_count(links) > 0)
		{
		    link = list_get(links, 0);
		    pt = uml_parse_transition(link);
		    // pt.symid = gde_arc_psymid(arc);
		    if (pt.unmatched_parens_error)
		    {
			/* syntax error */
			gde_print_error("Transition syntax error: unmatched attribute parentheses",
					gde_arc_psymid(arc));
			rc = False;
		    }
		    if (pt.unmatched_brackets_error)
		    {
			/* syntax error */
			gde_print_error("Transition syntax error: unmatched guard brackets",
					gde_arc_psymid(arc));
			rc = False;
		    }
		    duplicates = list_create("UMLParsedTransition", 0);
		    if (!(uml_transition_is_deterministic(pt,
					parsed_transitions, duplicates)))
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
    }
    if (list_count(dups) > 0)
    {
	ustated_show_nondeterministic_errors(dups,
			"Non-deterministic Transitions from initial State");
    }
    return rc;
}


boolean
uml_check_composite_states()
{
    // make sure that each composite stae has no more than one of each history
    // symbol type.
    list histnodes;
    list hist_types = string_to_list(UML_SHALLOW_HISTORY_SYMBOL_TYPE +
            " " + UML_DEEP_HISTORY_SYMBOL_TYPE, " ");
    string histType;
    int i;
    int j;
    int k;
    gde_node aState;
    gde_node aNode;
    list states = gde_nodes_find_by_type(gde_all_symbols(),
            UML_COMPOSITE_STATE_SYMBOL_TYPE);
    list containedNodes;
    boolean clean = True;

    for (i = 0; i < list_count(states); i++)
    {
        aState = list_get(states,i);
        for (j = 0; j < list_count(hist_types); j++)
        {
            histType = list_get(hist_types,j);

            // get all contained nodes, cull out the non-history ones
            containedNodes = gde_node_contained_nodes(aState, False);
            histnodes = list_create("gde_node",0);
            for (k = 0; k < list_count(containedNodes); k++)
            {
                aNode = list_get(containedNodes,k);
                if (gde_nt_name(gde_node_nt(aNode)) == histType)
                    list_append(histnodes, aNode);
            }

            if (list_count(histnodes) > 1)
            {
                print_error("A composite state can have at most 1 '" +
                        histType + "' symbol");
                clean = False;
            }
        }
    }
    return clean;
}
