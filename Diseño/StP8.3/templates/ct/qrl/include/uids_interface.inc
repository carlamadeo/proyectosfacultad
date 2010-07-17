// ECR 4400 start

// To use these functions you must add the line
// #include "qrl/include/uids_interface.inc"
// in your QRL script file

/////////////////////////////////////////////////////////////////////////////
// U n i q u e   I D s   i n t e r f a c e   f u n c t i o n s
/////////////////////////////////////////////////////////////////////////////

// This function returns the unique ID of a PDM object with the specified
// OMS id. PDM object must be one of node, link and context.
// Return value: unique ID on success or empty string on error
string
get_unique_id(int nOmsId)
{
    note nt;

    nt = find_by_query("note[type == GloballyUniqueID && obj_id == " + to_string(nOmsId) + "]");
    if (nt == NULL)
        return "";

    return nt.name;
}

// This function returns a list of unique IDs for every OMS node in
// lstOmsNodes. It can be used together with list_select() function.
// Return value: List of unique IDs on success or NULL on error
list
get_node_list_unique_ids(list lstOmsNodes)
{
    node nd;
    list lstUniqueIds;
    int  nNodesCount;
    int  i;

    if (lstOmsNodes == NULL)
        return NULL;

    lstUniqueIds = list_create("string", 0);
    nNodesCount = list_count(lstOmsNodes);
    for (i = 0; i < nNodesCount; i++)
    {
        nd = list_get(lstOmsNodes, i);
        list_append(lstUniqueIds, get_unique_id(nd.id));
    }

    return lstUniqueIds;
}

// This function returns a list of unique IDs for every OMS link in
// lstOmsLinks. It can be used together with list_select() function.
// Return value: List of unique IDs on success or NULL on error
list
get_link_list_unique_ids(list lstOmsLinks)
{
    link ln;
    list lstUniqueIds;
    int  nLinksCount;
    int  i;

    if (lstOmsLinks == NULL)
        return NULL;

    lstUniqueIds = list_create("string", 0);
    nLinksCount = list_count(lstOmsLinks);
    for (i = 0; i < nLinksCount; i++)
    {
        ln = list_get(lstOmsLinks, i);
        list_append(lstUniqueIds, get_unique_id(ln.id));
    }

    return lstUniqueIds;
}

// This function returns a list of unique IDs for every OMS context in
// lstOmsCntxs. It can be used together with list_select() function.
// Return value: List of unique IDs on success or NULL on error
list
get_cntx_list_unique_ids(list lstOmsCntxs)
{
    cntx cx;
    list lstUniqueIds;
    int  nCntxsCount;
    int  i;

    if (lstOmsCntxs == NULL)
        return NULL;

    lstUniqueIds = list_create("string", 0);
    nCntxsCount = list_count(lstOmsCntxs);
    for (i = 0; i < nCntxsCount; i++)
    {
        cx = list_get(lstOmsCntxs, i);
        list_append(lstUniqueIds, get_unique_id(cx.id));
    }

    return lstUniqueIds;
}

// This function returns *all* unique IDs for the current OMS id.
// This could happen if there are more than one arcs between two
// nodes, each arc has multiple parts, and parts of different arcs
// have the same label.
// Use it wisely
// Return value: set fo unique IDs on success or empty set (not
// NULL!) on error (if there is no unique ID for this PDM object)
set
get_unique_ids_set(int nOmsId)
{
    int  i;
    note nt;
    list lstNotes;
    set  setUniqueIds;

    setUniqueIds = set_create("string");
    lstNotes = list_select("note[type == GloballyUniqueID && obj_id == " + to_string(nOmsId) + "]");
    for (i = 0; i < list_count(lstNotes); i++)
    {
        nt = list_get(lstNotes, i);
        set_add(setUniqueIds, nt.name);
    }

    list_clear(lstNotes);
    return setUniqueIds;
}

// ECR 4400 end

