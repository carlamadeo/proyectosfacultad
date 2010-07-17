struct transition_info
{
    string cur_state;
    string event;
    string action;
    string next_state;
};


/* 
    Goes through a State Transition Diagram and returns a list of 
    transition infos for each transition 
*/
list
StateDiagramtoList(int std_id)
{
    list current;
    string query;
    list transitions = list_create("transition_info", 0);

    query = "node[StartState && node_refs[file_id == ${std_id}]]";
    current = list_select(query);

    if (list_count(current) == 0)
    {
	query = "node[State && node_refs[!in_link_refs && file_id == ${std_id}]]";
	current = list_select(query);

	if (list_count(current) == 0)
	{
	    print_error("No start state found on corresponding STD. Initialized table is empty.");
	    return transitions;
	}
    }

    FindTransitionInfo(std_id, current, transitions);
    return transitions;
}

void
FindTransitionInfo (int std_id, list current_nodes, list transitions)
{
    link cur_link;
    string query;
    node current;

    while (list_count(current_nodes) != 0)
    {
    	current = list_get(current_nodes, 0);
    	query = "link[link_refs[file_id == ${std_id}] && " + 
			"from_node_id == ${current.id}]";
        for_each_in_select(query, cur_link)
    	    SetTransitionInfo(std_id, transitions, current_nodes, current, cur_link);
    	list_delete(current_nodes, 0);             /* done with current node */
    }
}

void
SetTransitionInfo (int std_id, list transitions, list current_nodes, 
		    node current, link cur_link)
{
    transition_info info;
    string query;
    node next_state;
    cntx event_action;
    int slash;

    info.event = info.action = "";

    info.cur_state = current.name;
    query = "node[node_refs[file_id == ${std_id}] && in_links[id == ${cur_link.id}]]";
    next_state = find_by_query(query);
    info.next_state = next_state.name;

    query = "cntx[ActionInstance && cntx_refs[file_id == ${std_id}] && link_id == ${cur_link.id}]";
    event_action = find_by_query(query);

    if (event_action == NULL)
    {
    	info.event = info.action = "";
    }
    else
    {
    	slash = string_find(event_action.name, 0, "/");

    	info.event = string_extract(event_action.name, 0, slash);

	if (slash != string_length(event_action.name) - 1)
	    info.action = string_extract(event_action.name, slash + 1, 
    	    	    	    string_length(event_action.name) - (slash + 1));
    }

    list_append(transitions, info);
    if (!InTransitions(next_state.name, transitions))
    	list_append(current_nodes, next_state);
}


boolean 
InTransitions (string cur_name, list transitions)
{
    int ix, count;
    transition_info cur_trans;

    for (ix = 0, count = list_count(transitions); ix < count; ix = ix + 1)
    {
    	cur_trans = list_get(transitions, ix);
    	if (cur_trans.cur_state == cur_name)
    	    return True;
    }
    return False;
}

