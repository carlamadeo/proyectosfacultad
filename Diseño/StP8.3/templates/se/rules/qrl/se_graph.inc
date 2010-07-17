//
//      sccsid[] = %W% %Y% %D%
//      StP/SE
//      Confidential property of Interactive Development Environments, Inc.
//      Copyright (c) 1993
//      All rights reserved
//

#include "rules/qrl/ct.inc"

graph
SEClassGraph(graph G, list hierarchy)
{
    int             i, j, m, n;
    node            node;
    link            link;
    item            item;
    file            file;
    string          parent, child, children_filename;
    list            parents, children;

    if (G == NULL)
	G = graph_create();

    for (i = 0, n = list_count(hierarchy); i < n; i = i + 1)
    {
	if (list_get_type(hierarchy, i) == "node")
	{
	    node = list_get(hierarchy, i);
	    if (node.type == "Module")
	    {
		parent = node.name;
		children = project_string_attribute_from_se_list(list_select("node[Module && in_links[SubprogramCall && from_node_id = ${node.id} && link_refs] && node_refs]"), "name");
	    } else if (node.type == "Process")
	    {
		file = find_by_query("file[node_refs[node_id = ${node.id}]]");
		if (file == NULL)
		    continue;
		parent = node.name;
		if (file.name == "top")
		{
		    children_filename = "0";
		} else
		{
		    item = find_by_query("item[RelativeIndex & obj_id=${node.id}]");
		    if (item == NULL)
			continue;
		    if (file.name == "0")
			children_filename = item.value;
		    else
			children_filename = file.name + "." + item.value;
		}
		children = project_string_attribute_from_se_list(list_select("node[Process & node_refs[file[name = '${children_filename}']]]"), "name");
	    } else
		continue;
	} else if (list_get_type(hierarchy, i) == "link")
	{
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
 
 
