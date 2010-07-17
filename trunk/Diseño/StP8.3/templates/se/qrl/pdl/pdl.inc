list
GetCalledBys(int mod_id)
{
    string query;
    list vars;

    /* break apart query for performance reasons */
    query = "node[! " + OFFPAGE_CONNECTOR + " && node_refs && out_links[link_refs && to_node_id == ${mod_id}]]";
    vars = list_select(query);
    query = "node[! " + OFFPAGE_CONNECTOR + " && node_refs && out_links[link_refs && to_node[" + OFFPAGE_CONNECTOR + " && out_links[to_node_id == ${mod_id}]]]]";
    list_concatenate(vars, list_select(query));
    return vars;
}

list
GetCalls(int mod_id)
{
    list vars;

    /* break query apart for performance reasons */
    query = "node[! " + OFFPAGE_CONNECTOR + " && node_refs && in_links[link_refs && from_node_id == ${mod_id}]]";
    vars = list_select(query);
    query = "node[! " + OFFPAGE_CONNECTOR + " && node_refs && in_links[link_refs && from_node[" + OFFPAGE_CONNECTOR + " && in_links[from_node_id == ${mod_id}]]]]";
    list_concatenate(vars, list_select(query));
    return vars;
}
