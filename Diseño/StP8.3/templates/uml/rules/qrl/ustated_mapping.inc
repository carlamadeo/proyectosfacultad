
//      StP/UML
//      Confidential property of Aonix
//      Copyright (c) 1997
//      All rights reserved


#include 	"rules/qrl/state_diag_syms.inc"

const string    UML_JUNCTION_POINT_SYMBOL_TYPE = "JunctionPoint";
const string    UML_DYNAMIC_CHOICE_POINT_SYMBOL_TYPE = "DynamicChoicePoint";
const string    UML_CONCURRENT_STATE_SYMBOL_TYPE = "ConcurrentSubregion";
const string    UML_STATE_AMBASSADOR_SYMBOL_TYPE = "StateAmbassador";
/*TODO*/
const string    UML_STATES_WITH_ACTION_TABLES_SYMBOL_TYPE = "StatesWithActionTables";
const string    UML_STATE_SCOPE_LINK_TYPE = "StateComponentLink";
const string    UML_STATE_AMBASSADOR_SCOPE_LINK_TYPE = "StateAmbassadorScopeLink";
const string    UML_SCOPE_AMBASSADOR_APPLICATION_TAG = "ScopeAmbassador";
const int       STATE_AMBASSADOR_HORZ_SPACING = 150;
const int       STATE_AMBASSADOR_VERT_SPACING = 150;


// SCOPING OF STATES

gde_mapsymbol
uml_state_scope_symbol(gde_mapsymbol state_sym)
{
  gde_node parent = uml_state_parent_state(to_gde_node(state_sym));
  if (parent)
    return to_gde_mapsymbol(parent);

  return NULL;
}

gde_node
uml_state_parent_state(gde_node state)
{
  /* Return a scope symbol for an uml state.
   * If the state is nested within another state, return the enclosing state.
   * If the state is top level, return the proper scope ambassador 
   *if there is one, o/w the state machine. */

  gde_node composite_state, ambassador;

  /* first search for an enclosing state */
  composite_state = uml_state_get_composite_state(state);
  if (composite_state)
    return composite_state;

  /* If there is no enclosing state,
   * look for the last state ambassador symbol in the
   * scope chain*/
  ambassador = uml_state_scope_ambassador();
  if(ambassador)
      return ambassador;

  return uml_state_machine();
}

list uml_substates(gde_node composite_state)
{
    list substates = list_create("gde_node",0);
    list arcs, links;
    gde_arc arc;
    int i,j;
    
    if (composite_state == NULL)
	return substates;
    
    arcs = gde_node_outarcs(composite_state);
    for (i = 0; i < list_count(arcs); i = i + 1) {
	arc = list_get(arcs, i);
	if (gde_lt_name(gde_arc_lt(arc)) == UML_STATE_SCOPE_LINK_TYPE) {
	    links = gde_arc_links(arc);
	    for(j = 0;j < list_count(links);j++)
		list_append(substates,gde_link_tonode(list_get(links, 0)));
	}
    }
    return substates;
}

gde_node
uml_state_get_composite_state(gde_node state)
{
  /* Return the composite_state of a state (i.e., find a state at the other end of an incoming scope link),
   * or NULL if there isn't one.
   * Does not take into account scope ambassadors. */

  list arcs, links;
  gde_arc arc;
  int i;

  if (state == NULL)
    return NULL;

  arcs = gde_node_inarcs(state);
  for (i = 0; i < list_count(arcs); i = i + 1) {
    arc = list_get(arcs, i);
    if (gde_lt_name(gde_arc_lt(arc)) == UML_STATE_SCOPE_LINK_TYPE) {
      links = gde_arc_links(arc);
      if (list_count(links) > 0)
	return gde_link_fromnode(list_get(links, 0));
    }
  }
  return NULL;
}

boolean
uml_state_has_composite_state(gde_node state)
{
  list arcs;
  gde_arc arc;
  int i;

  arcs = gde_node_inarcs(state);
  for (i = 0; i < list_count(arcs); i = i + 1) {
    arc = list_get(arcs, i);
    if (gde_lt_name(gde_arc_lt(arc)) == UML_STATE_SCOPE_LINK_TYPE)
      return True;
  }
  return False;
}


gde_mapsymbol
uml_ambassador_scope_symbol(gde_mapsymbol ambassador_sym)
{
    int		i;
    list	links, arcs;
    gde_arc     arc;

    gde_node ambassador = to_gde_node(ambassador_sym);
    
    arcs = gde_node_inarcs(ambassador);
    for (i = 0; i < list_count(arcs); i = i + 1) 
	{
	    arc = list_get(arcs, i);
	    if (gde_lt_name(gde_arc_lt(arc)) == UML_STATE_AMBASSADOR_SCOPE_LINK_TYPE)
		{
		    links = gde_arc_links(arc);
		    if (list_count(links) > 0)
			return to_gde_mapsymbol(gde_link_fromnode(list_get(links, 0)));
		}
	}
	    
    return to_gde_mapsymbol(uml_state_machine());
    
}

/* look for the last symbol in the state ambassador scope chain*/
gde_node
uml_state_scope_ambassador()
{
    int		i;
    list	nodes, arcs;
    gde_node	a_node;

     nodes = 
	gde_nodes_find_by_application_tag(gde_all_symbols(),
					  UML_SCOPE_AMBASSADOR_APPLICATION_TAG);

    for(i = 0; i < list_count(nodes); i = i + 1)
    {
	a_node = list_get(nodes, i);
	arcs = gde_node_outarcs(a_node);
	if (list_count(arcs) == 0)
	    return (a_node);
    }
    return NULL;

}

boolean
uml_state_ambassador_has_parent(gde_node ambassador)
{
  list arcs;
  gde_arc arc;
  int i;

  arcs = gde_node_inarcs(ambassador);
  for (i = 0; i < list_count(arcs); i = i + 1) {
    arc = list_get(arcs, i);
    if (gde_lt_name(gde_arc_lt(arc)) == UML_STATE_AMBASSADOR_SCOPE_LINK_TYPE)
      return True;
  }
  return False;
}



/* value funcs for transition item mappings: event, attribs, guard, actions */ 

gde_mapsymbol
uml_split_control_scope_symbol(gde_mapsymbol split_control_sym)
{
  /* the scope of the split control node 
   * is the same as the scope of the origin state for the incoming transition */

  gde_link transition;
  gde_node parent;

  transition = uml_split_control_incoming_transition(to_gde_node(split_control_sym));
  if (transition == NULL)
    return (NULL);

  parent = uml_state_parent_state(gde_link_fromnode(transition));
  if (parent)
    return to_gde_mapsymbol(parent);

  return NULL;
}

gde_mapsymbol
uml_merge_control_scope_symbol(gde_mapsymbol merge_control_sym)
{
  /* the scope of the merge control node 
   * is the same as the scope of the destination state of the outgoing transition */

  gde_link transition;
  gde_node parent;

  transition = uml_merge_control_outgoing_transition(to_gde_node(merge_control_sym));
  if (transition == NULL) 
    return (NULL);

  parent = uml_state_parent_state(gde_link_tonode(transition));
  if (parent)
    return to_gde_mapsymbol(parent);
  
  return NULL;
}

list compound_state_symbolic_dependents(gde_mapsymbol ms)
{
    int i;
    list dependents, arcs, links;
    gde_node gn = to_gde_node(ms);
    gde_arc arc;
    gde_node target;
    
    dependents = list_create("gde_mapsymbol",0);
    arcs = gde_node_outarcs(gn);
    for(i = 0; i < list_count(arcs);i++)
	{
	    arc = list_get(arcs, i);
	    if (gde_lt_name(gde_arc_lt(arc)) == UML_STATE_SCOPE_LINK_TYPE)
		{
		    links = gde_arc_links(arc);
		    if (list_count(links) > 0)
			list_append(dependents,
			    to_gde_mapsymbol(gde_link_tonode(list_get(links, 0))));
		}
	    else 
		if (gde_lt_name(gde_arc_lt(arc)) == UML_STATE_TRANS_LINK_SYMBOL_TYPE)
		    {
			links = gde_arc_links(arc);
			if (list_count(links) > 0)
			    {
				target = gde_link_tonode(list_get(links, 0));
				if(gde_nt_name(gde_node_nt(target)) == 
				   UML_SPLIT_CONTROL_SYMBOL_TYPE || 
				   gde_nt_name(gde_node_nt(target)) == 
				   UML_MERGE_CONTROL_SYMBOL_TYPE)
				    list_append(dependents,to_gde_mapsymbol(target));
				
			    }
		    }
	}
    return dependents;
}

list state_symbolic_dependents(gde_mapsymbol ms)
{
    int i;
    list dependents, arcs, links;
    gde_node gn = to_gde_node(ms);
    gde_arc arc;
    gde_node target;
    
    dependents = list_create("gde_mapsymbol",0);
    arcs = gde_node_outarcs(gn);
    for(i = 0; i < list_count(arcs);i++)
	{
	    arc = list_get(arcs, i);
	    if (gde_lt_name(gde_arc_lt(arc)) == UML_STATE_TRANS_LINK_SYMBOL_TYPE)
		{
		    links = gde_arc_links(arc);
		    if (list_count(links) > 0)
			{
			    target = gde_link_tonode(list_get(links, 0));
			    if(gde_nt_name(gde_node_nt(target)) == 
			       UML_SPLIT_CONTROL_SYMBOL_TYPE || 
			       gde_nt_name(gde_node_nt(target)) == 
			       UML_MERGE_CONTROL_SYMBOL_TYPE)
				list_append(dependents,to_gde_mapsymbol(target));
			    
			}
		}
	}
    return dependents;
}

list transition_symbolic_dependents(gde_mapsymbol ms)
{
    list dependents = list_create("gde_mapsymbol", 0);
    gde_link transition = to_gde_link(ms);
    gde_node target = gde_link_tonode(transition);

    if(gde_nt_name(gde_node_nt(target)) == UML_SPLIT_CONTROL_SYMBOL_TYPE ||
       gde_nt_name(gde_node_nt(target)) == UML_MERGE_CONTROL_SYMBOL_TYPE ||
       // ECR 7215 - Added Junction Points
       gde_nt_name(gde_node_nt(target)) == UML_JUNCTION_POINT_SYMBOL_TYPE ||
       gde_nt_name(gde_node_nt(target)) == UML_DYNAMIC_CHOICE_POINT_SYMBOL_TYPE)
	list_append(dependents,to_gde_mapsymbol(target));

    return dependents;
}

// ECR 6879 begin
list component_link_symbolic_dependents(gde_mapsymbol ms)
{
    list     dependents;
    gde_link contains;
    gde_node tonode;
    string   tonode_type;

    dependents = list_create("gde_mapsymbol", 0);
    contains = to_gde_link(ms);
    tonode = gde_link_tonode(contains);
    tonode_type = gde_nt_name(gde_node_nt(tonode));

    if (tonode_type != UML_SPLIT_CONTROL_SYMBOL_TYPE &&
        tonode_type != UML_MERGE_CONTROL_SYMBOL_TYPE)
    {
        list_append(dependents, to_gde_mapsymbol(tonode));
    }

    return dependents;
}
// ECR 6879 end

