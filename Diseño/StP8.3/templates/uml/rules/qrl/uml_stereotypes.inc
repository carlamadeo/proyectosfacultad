//
//      sccsid[] = %W% %Y% %D%
//      StP/UML
//      Confidential property of Aonix
//      Copyright (c) 1997
//      All rights reserved
//

//
// Stereotype inheritance functions
//

// This function builds a stereotype graph from parameters. It was 
// designed to take parameters so that it can be reused in semantics checks
graph
uml_build_stereotype_graph(set links, set nodes)
{
	graph g = graph_create();
	node fromNode, toNode, curNode;
	link curLink;
	int i,j;
	int gotVals;

	int countNodes = set_count(nodes);
	int countLinks = set_count(links);

	// for each link, find the nodes on either and and add
	// the link to the graph
	for(i=0;i<countLinks;i++) {
		curLink = set_get_element(links,i);
		fromNode = toNode = NULL;
		gotVals = 0;

		// loop until we find both nodes
		for(j=0; j<countNodes && gotVals < 2 ;j++) {
			curNode = set_get_element(nodes,j);
			if (curLink.from_node_id == curNode.id) {
				fromNode = curNode;
				gotVals++;
			}
			if (curLink.to_node_id == curNode.id) {
				toNode = curNode;
				gotVals++;
			}
		}
		if (gotVals != 2) {
			// didn't find at least one node. Ignore this link
			continue;
		}
		graph_add_arc(g,fromNode.name,toNode.name);
	}
	return g;
}

// this function gets a graph of all stereotypes in the system
graph
uml_get_full_stereotype_graph()
{
	return uml_build_stereotype_graph(
			set_select("link[type=UmlStereotypeInheritance && link_refs]"),
			set_select("node[type=UmlStereotype && node_refs]"));
}

// This function returns True if the stereotype sType inherits from
// sBaseType somewhere in the hierarchy.
// If stGraph is NULL, it is created.
boolean
uml_is_stereotype_of(string sType, string sBaseType, graph stGraph)
{
	boolean ret;
	if (sType == sBaseType) {
		return True;
	}

	if (NULL == stGraph) {
		stGraph = uml_get_full_stereotype_graph();
	}

	if (graph_is_node(stGraph,sType) && graph_is_node(stGraph,sBaseType)) {
		ret = graph_is_indirect_arc(stGraph,sType,sBaseType);
	} else {
		ret = False;
	}

	return ret;
}

