//
//      StP/UML
//      Confidential property of Aonix
//      Copyright (c) 1997
//      All rights reserved
//

// SYNTAX CHECKING FOR uclassd

#include "qrl/include/utilities.inc"


gde_node
uml_symbol_to_class(gde_symbol symbol)
{
  /* if the symbol is a class, return it as a gde_node, otherwise return NULL */
 
  gde_node node;
 
  /* classes are nodes */
  if (gde_symbol_is_node(symbol)) {
    node = to_gde_node(symbol);
    if (gde_nt_application_tag(gde_node_nt(node)) == "ClassSymbol" )
      return (node);
  }
  return (NULL);
}


boolean 
uclassd_CheckSyntax()
{
    boolean diagramIsClean = True;
    list symbols;
    gde_symbol symbol;
    gde_node class, operation, object;
    gde_cntx role;
    int i;
    
    /* iterate through all the symbols in one pass, checking each checked type */
    symbols = gde_all_symbols();
    for (i = 0; i < list_count(symbols); i = i + 1) {
        symbol = list_get(symbols, i);
        
        /* class checks */
        class = uml_symbol_to_class(symbol);
        if (class != NULL) {
            /* classes in a generalization or association must be named */
            if (uml_class_has_generalization_or_association(class) && !(uml_class_has_name(class))) {
                gde_print_error("Classes which participate in an association, refinement, implementation, or generalization must be named", gde_node_psymid(class));
                diagramIsClean = False;
            }
            // Begin Add ECR 6241
            if( uml_class_has_realizes_inlink( class ) ) {
                gde_print_error("Normal Class cannot have an incoming Realizes link.", gde_node_psymid(class));
                diagramIsClean = False;
            }
            // End Add ECR 6241
            continue;
        }
    }
    
    
    /* Now run the default gde syntax checks. */
    
    if (gde_default_check_syntax() == False)
        diagramIsClean = False;
    
    return diagramIsClean;
}

// Begin Add ECR 6241
boolean
uml_class_has_realizes_inlink(gde_node class)
{
    list arcs;
    gde_arc arc;
    string linktype;
    int i;
    node oms_class, one_node;

    list mapped_nodes = gde_node_oms_nodes( class );
    for( i=0; i<list_count( mapped_nodes ); i++ ) {
        one_node = list_get( mapped_nodes, i );
        if( one_node.type == "UmlInterface" )
            return False; // don't care about bubble interfaces
        if( one_node.type == "UmlClass" )
            oms_class = one_node;
    }
    
    if( oms_class != NULL ) {
        item st = find_by_query( "item[UmlStereotype && obj_id == ${oms_class.id}]" );
        arcs = gde_node_inarcs(class);
        for( i = 0; i < list_count( arcs ); i = i + 1 ) {
            arc = list_get( arcs, i );
            linktype = gde_lt_name( gde_arc_lt( arc ) );
            if( linktype == "Implements" ) {
                if( st == NULL )
                    return True;
                if( st.value != "interface" && 
                    st.value != "type" && 
                    st.value != "implementationClass" )
                    return True;
            }
        }
    }
    return False;
}
// End Add ECR 6241

boolean
uml_class_has_generalization_or_association(gde_node class)
{

  /* return true iff a class participates in an association or a generalization */

  list arcs;
  gde_arc arc;
  string linktype;
  int i;

  arcs = gde_node_inarcs(class);
  for (i = 0; i < list_count(arcs); i = i + 1) {
    arc = list_get(arcs, i);
    linktype = gde_lt_name(gde_arc_lt(arc));
    if (linktype == "Generalization" || 
	linktype == "Association" ||
	linktype == "Aggregation" ||
	linktype == "NavigableAssociation") // ECR 7480
      return (True);
  }

  arcs = gde_node_outarcs(class);
  for (i = 0; i < list_count(arcs); i = i + 1) {
    arc = list_get(arcs, i);
    linktype = gde_lt_name(gde_arc_lt(arc));
    if (linktype == "Generalization" || 
	linktype == "Refines" ||
	linktype == "Implements" ||
	linktype == "Association" ||
	linktype == "Aggregation" ||
	linktype == "NavigableAssociation") // ECR 7480
      return (True);
  }

  return (False);
}

boolean
uml_class_has_name(gde_node class)
{
  /* true iff the class has a name */

  string name;

  name = strip_whitespace(gde_node_label(class));

  if (name == NULL || name == "")
    return (False);
  else
    return (True);
}

