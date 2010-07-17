//////////////////////////////////////////////////////////////////////////////
//
//      sccsid[] = @(#)dfe.inc
//      StP/IM
//      Confidential property of Interactive Development Environments, Inc.
//      Copyright (c) 1992-1993
//      All rights reserved
//
//      Description: RME Semantic checking is performed here. This routine
//      is called from three separate places:
//
//      1. From within the RME editor Repository->Check Diagram Semantics.
//      2. From within the RME editor Reports->Generate QRL Report.
//      3. From the StP desktop semantic checking.
//
//      Each behaves properly because the functions out_error and out_message
//      are redefined to work with the correct calls.
//
//////////////////////////////////////////////////////////////////////////////

int
RmeCheckSemanticsFromQrl(list files)
{
    file rme_file;
    file myfile;
    node rme_node;
    node_ref my_node_ref;
    string query, message, filename;
    set unique_set = set_create("int");
    int unique_count;
    int node_count, start_node_count, end_node_count, next_link_count;
    int errors;
    int i;

    for(i = 0; i < list_count(files); i = i +1)
    { 
        errors = 0;
        myfile = list_get(files, i);
        filename = myfile.name;

        out_message("Checking semantics for RME diagram: " + filename);

        if (filename == NULL || filename == "")
        {
            out_error("Save the diagram before running Check Semantics.");
            return(0);
        }

// The following types of checks are performed.
//
// 1. If (node_count > 1) && (next_link_count < node_count - 1) then error
// 2. If (node_count > 1) && (start_node_count > 1 || end_node_count > 1)
//    then error
// 3. If (node_count == next_link_count) then error (cyclic)
// 4. If the same node appears more than once on a diagram then error


        // get file id of this diagram
        query = "file[RmeDiagram && name = '" + filename + "']";
        rme_file = find_by_query(query);

        // get count of all nodes (comments aren't mapped and won't show)
        // &&
        // get count of unique node object id's
 
        query = "node_ref[file_id = " + rme_file.id + "]";
        node_count = selection_count(query);
        for_each_in_select(query, my_node_ref)
        {
            set_add(unique_set, my_node_ref.node_id);
        }
        unique_count = set_count(unique_set);
        set_clear(unique_set);

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
                out_error("Unconnected section detected.");
                errors = errors + 1;
            }
        }

        // --- Error Number 2 ---
        if(node_count > 1)
        {
            if(start_node_count > 1)
            {
                out_error("More than one starting node detected.");
                errors = errors + 1;
            }

            if(end_node_count > 1) 
            {
                out_error("More than one ending node detected.");
                errors = errors + 1;
            }
        }

        // --- Error Number 3 ---
        if((node_count >= 1) && node_count == next_link_count)
        {
             out_error("Equal number of sections and links. Possible cycle.");
             errors = errors + 1;
        }

        // --- Error Number 4 ---
        if(node_count > unique_count)
        {
            out_error("The same node cannot be used more than once.");
            errors = errors + 1;
        }

        message = "RME semantic checking completed with " +  errors + " error";

        if(errors != 1)
            message = message + "s.";
        else
            message = message + ".";
                
        out_message(message);
    }

    return(-1 * errors);
}
