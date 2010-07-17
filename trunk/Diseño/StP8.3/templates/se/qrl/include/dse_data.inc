
//
//  Gets full primitive DSE definition of data flowing through links returned
//  by the query.
//
void
link_dse_data (set data, string query, string data_defs)
{
    link cur_link;
    set names = set_create("string");

    for_each_in_select(query, cur_link)
    {
    	set_add(names, cur_link.name);
    }

    name_dse_data(data, NULL, data_defs, names);
}

//
//  Gets full primitive DSE definition of data flowing through links in
//  the links set.
//
void
link_set_dse_data (set data, set links, string data_defs)
{
    int i, count;
    link cur_link;
    set names = set_create("string");

    for (i = 0, count = set_count(links); i < count; i = i + 1)
    {
    	cur_link = set_get_element(links, i);
    	set_add(names, cur_link.name);
    }
    name_dse_data(data, NULL, data_defs, names);
}

//
//  Gets full primitive definition of named data.  Can pass in a string
//  (if only one name) or a set (if many names).
//
void
name_dse_data (set data, string one_name, string data_defs, set names)
{
    string cur_data;
    node   cur_data_node = NULL;
    string query, clause = "";
    int i, count;

    if (names == NULL)
	names = set_create("string");

    if (one_name != NULL && one_name != "")
    {
    	set_add(names, one_name);
    }
    else if (names == NULL)
    {
    	ide_error("Internal error: no names passed to name_dse_data");
    }
    else
    {
    	/* no names in set --> do nothing */
    	if ((count = set_count(names)) == 0)
    	    return;
    }
    
    for(i = 0; i < set_count(names); i = i + 1)
    {
	cur_data = set_get_element(names, i);
	/* if data is primitive element in set, don't need to refetch it */
	if (!set_is_member(data, cur_data))
	{
	    // Check dse nodes first, then ere.
    	    // Had to separate this into two queries because there is a
    	    // bug that prevents joins on id lists.

	    query = "node[data_defs && name = '" + to_oms_string(cur_data) + "']";
	    cur_data_node = find_by_query(query);

	    if (cur_data_node!=NULL)
	    	get_elements(data, cur_data_node, get_next_nodes(cur_data_node));
	}
    }
}

//
// Get all leaf nodes in the tree starting at node root and add them to
// the set leaves.
//
void
get_elements(set leaves, node root, list next_nodes)
{
    boolean root_is_leaf = True;
    node cur_node;	
    int i;

    for (i = 0; i < list_count(next_nodes); i = i + 1)
    {
	cur_node = list_get(next_nodes, i);
    	get_elements(leaves, cur_node, get_next_nodes(cur_node));
    	root_is_leaf = False;
    }

    if (root_is_leaf)
    	set_add(leaves, root);
}

//
//  Returns a list containing the next nodes for the_node.
//
list
get_next_nodes(node the_node)
{
    list list = list_create("node", 0);
    list templist = list_create("node", 0);
    string query; 
    link one_link;

    if ((the_node.type != "EntityGroup") && (the_node.type != "Entity"))
    {
	query = "link[link_refs && from_node_id == ${the_node.id}]";
	for_each_in_select(query, one_link)
	{
	    query = "node[node_refs && in_links[${one_link.id}]]";  
	    templist = list_select(query);
	    list_concatenate(list, templist);
	}
        return list;
    }	
    else  // Traverse IM definitions opposite from above. Use EntityGroupInclusion to avoid 
          // association links.
    {
        query = "link[link_refs && from_node_id == ${the_node.id}]";
        for_each_in_select(query, one_link) 
        { 
            query = "node[node_refs && in_links[EntityGroupInclusion && ${one_link.id}]]";
            templist = list_select(query);
            list_concatenate(list, templist);
        } 
        return list;
    }
}

//
//  Filters set1 and set2 so that each contains only elements not in 
//  the other.
//
void
find_disparate_elements(set set1, set set2)
{
    set common_data;

    common_data = set_copy(set1);
    set_intersection(common_data, set2);
    
    // any elements left in these sets are disparate
    set_difference(set1, common_data);
    set_difference(set2, common_data);
}

//
//  Need a special case for selections because
//  links which are defined as selections only
//  have to contain one of the components, not all
//

boolean
is_selection(set set1, set set2)
{
   int i, count;
   string element, query;
   node node=NULL;

   // If an element of set2 is a member of set1
   // and the element is a selection
   // delete the element from both sets since
   // 

   if (set_count(set1)==0 || set_count(set2)==0)
	return (False);

   for (i = 0, count = set_count(set2); i < count; i = i + 1)
   {
	element = set_get_element(set2, i);
	query = "node[name='" + to_oms_string(element) + "' && type = Selection && node_refs]";
	node=find_by_query(query);
	if (node!=NULL)
	{
	    if(!set_is_member(set1, element))
	    {
	        return (False);
	    }
	}
	else return (False);
   }

   set_clear(set1);
   set_clear(set2);
   return (True);

}

void
get_splitflow_links (set links, string direction)
{
    int i, count;
    link cur_link;
    set splitflow_links;
    string query;
	
  if (direction == "into")
  {
    // account for splitflows
    for (i = 0, count = set_count(links); i < count; i = i + 1)
    {
        cur_link = set_get_element(links, i);
        query="link[link_refs && to_node[id=${cur_link.from_node_id} && type=SplitFlow]]";
        splitflow_links = set_select(query);
        set_union(links, splitflow_links);
    }
  }

  else
  {
    for (i = 0, count = set_count(links); i < count; i = i + 1)
    {
        cur_link = set_get_element(links, i);
        query="link[link_refs && from_node[id=${cur_link.to_node_id} && type=SplitFlow]]";
        splitflow_links = set_select(query);
        set_union(links, splitflow_links);
    }
  }

    return;
}

