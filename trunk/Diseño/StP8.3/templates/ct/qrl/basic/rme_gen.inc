//////////////////////////////////////////////////////////////////////////////
//
//      sccsid[] = @(#)dfe.inc
//      StP/IM
//      Confidential property of Interactive Development Environments, Inc.
//      Copyright (c) 1992-1993
//      All rights reserved
//
//      Description: 
//
//////////////////////////////////////////////////////////////////////////////

int
RmeCheckSemanticsFromQrl(string filename)
{
    file rme_file;
    node rme_node;
    string query, message;
    int node_count, start_node_count, end_node_count, next_link_count;
    int errors = 0;


    if (filename == NULL || filename == "")
    {
        message("Save the diagram before running Check Semantics.");
        return(0);
    }

// Three types of checks are performed.
//
// 1. If (node_count > 1) && (next_link_count < node_count - 1) then error
// 2. If (node_count > 1) && (start_node_count > 1 || end_node_count > 1)
//    then error
// 3. If (node_count == next_link_count) then error (cyclic)


    // get file id of this diagram
    query = "file[RmeDiagram && name = '" + filename + "']";
    rme_file = find_by_query(query);

    // get count of all nodes (comments aren't mapped and won't show)
    query = "node_ref[file_id = " + rme_file.id + "]";
    node_count = selection_count(query);

    // start nodes are nodes with outlinks only
    query = "node_ref[file_id = " + rme_file.id + "&& !in_link_refs]";
    start_node_count = selection_count(query);

    // end nodes are nodes with inlinks only
    query = "node_ref[file_id = " + rme_file.id + "&& !out_link_refs]";
    end_node_count = selection_count(query);

    query = "link[NextSection && link_refs[file_id = " + rme_file.id + "]]";
    next_link_count = selection_count(query);

    // --- Error Number 1 ---
    if(node_count > 1)
    {
        if(next_link_count < node_count - 1)
        {
            message("Unconnected section detected.");
            errors = errors + 1;
        }

    }

    // --- Error Number 2 ---
    if(node_count > 1)
    {
        if(start_node_count > 1)
        {
            message("More than one starting node detected.");
            errors = errors + 1;
        }

        if(end_node_count > 1) 
        {
            message("More than one ending node detected.");
            errors = errors + 1;
        }
    }


    // --- Error Number 3 ---
    if((node_count >= 1) && node_count == next_link_count)
    {
         message("Equal number of sections and links. Possible cycle.");
         errors = errors + 1;
    }

    message = "RME semantic checking completed with " +  errors + " error";

    if(errors != 1)
        message = message + "s.";
    else
        message = message + ".";
                
    message(message);

    return(errors);
 
}
