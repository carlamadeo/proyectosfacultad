// Utility functions

//
// get the Process Index of a Process
//
item get_process_index(node process)
{
    string query;   	

    query = "item[obj_id == ${process.id} && RelativeIndex]";

// $$  may be able to dump error check eventually
//    return(find_by_query(query));

    item index = find_by_query(query);
    if (index == NULL)
    	node_error(process, " has no pindex!", NULL);
    else if (index.value == NULL)
    	node_error(process, " has no pindex value!", NULL);
    return index;
}
