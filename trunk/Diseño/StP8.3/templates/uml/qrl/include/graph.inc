//
//      StP/UML
//      Confidential property of Aonix
//      Copyright (c) 1997
//      All rights reserved


#include "qrl/include/utilities.inc"

//
// Uml graph constructor
//

graph
UmlClassGraph(graph G, list hierarchy)
{
    int             d, i, j, l, m, n;
    node            node;
    link            link;
    string          parent, child;
    list            children;

    //!! This needs more work.
    //!! See oo/qrl/include/graph.inc

    const string    PACKAGE_PARTS = "node[UmlClass & node_refs & out_links[to_node_id=${node.id} & link_refs]]";
    const string    SUPERIOR = "node[id=${link.to_node_id}]";
    const string    INFERIOR = "node[id=${link.from_node_id}]";

    if (G == NULL)
	G = graph_create();	// if no input graph then create it, else
				// "append" to it

    //
    // given a relationship implied by the list element type, add related
    // objects and relationships to graph
    //
    for (i = 0, n = list_count(hierarchy); i < n; i = i + 1)
    {
	children = list_create("string", 0);
	
	if (list_get_type(hierarchy, i) == "node")
	{
	    node = list_get(hierarchy, i);

	    if (node.type == "UmlPackage")
	    {
		parent = class_base_name(node);
		children = class_base_name_list_from_class_list(list_select(PACKAGE_PARTS));
	    } else
		graph_add_node(G, node.name);
	} else if (list_get_type(hierarchy, i) == "link")
	{
	    link = list_get(hierarchy, i);
	    if (link.type == "UmlGeneralization")
	    {
		parent = node_name(find_by_query(SUPERIOR));
		children = class_base_name_list_from_class_list(list_select(INFERIOR));
	    } else
		continue;
	} else
	    continue;

	m = list_count(children);
	if (m > 0)
	{
	    graph_add_node(G, parent);
	    for (j = 0; j < m; j = j + 1)
	    {
		child = list_get(children, j);
		graph_add_node(G, child);
		graph_add_arc(G, parent, child);
	    }
	}
    }

    return G;
}

// builds a level 2 graph by removing all intermediary nodes 

graph
flatten_graph (graph G)
{
   string Node ;
   list AllNodes = graph_all_nodes (G) ;
   int NodeCount = list_count (AllNodes) ;

   int i ;
   int j ;

   string Ancestor ;
   list AncestorList ;
   int AncestorCount ;

   graph Result = graph_create () ;

   for (i=0; i<NodeCount; i=i+1)
   {
      Node      = list_get(AllNodes,i) ;

      if (0==graph_node_rank (G, Node))
      {
	 if (!graph_is_node (Result, Node))
	 {
	    graph_add_node (Result, Node) ;
	 }

         AncestorList  = get_graph_roots (G, Node) ;
         AncestorCount = list_count (AncestorList) ;

         for (j=0; j< AncestorCount; j=j+1)
         {
	    Ancestor = list_get (AncestorList, j) ;
	    if (!graph_is_node (Result, Ancestor))
	    {
	       graph_add_node (Result, Ancestor) ;
	    }

	    if (Node != Ancestor)
	    {
	       graph_add_arc (Result, Ancestor, Node) ;
	    }
         }
      }
   }
   return (Result) ;
}


list
get_graph_leaves (graph G, string Node)
{
   list result = list_create ("string", 0) ;

   if (graph_is_node (G, Node))
   {
      list successors = graph_node_successors (G, Node) ;

      int count = list_count (successors) ;

      if (count == 0)
      {
	 list_append (result, Node) ;
      }
      else
      {
         int i ;
         for (i=0; i<count; i=i+1)
         {
	    list_concatenate (result, 
	         get_graph_leaves (G, list_get(successors,i))) ;
         }
      }
   }
   return (result) ;
}

list
get_graph_roots (graph G, string Node)
{
   list result = list_create ("string", 0) ;

   if (graph_is_node (G, Node))
   {
      list predecessors = graph_node_predecessors (G, Node) ;

      int count = list_count (predecessors) ;

      if (count == 0)
      {
	 list_append (result, Node) ;
      }
      else
      {
         int i ;
         for (i=0; i<count; i=i+1)
         {
	    list_concatenate (result, 
	         get_graph_roots (G, list_get(predecessors,i))) ;
         }
      }
   }
   return (result) ;
}


