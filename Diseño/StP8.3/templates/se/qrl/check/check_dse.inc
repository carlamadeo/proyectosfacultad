//
// 	sccsid[] = %W% %Y% %D%
// 	StP/SE
// 	Confidential property of Interactive Development Environments, Inc.
// 	Copyright (c) 1992-1995
// 	All rights reserved
//

#include "qrl/include/symbol_messages.inc"
#include "qrl/include/files.inc"
#include "qrl/check/dse.inc"

boolean verbose = False;

int 
CheckDseSemantics(string file_string, boolean verbose_mode)
{
    int    ix, errors = 0;
    list   filelist;
    file   fileobj;
    list   nodelist;
    string message;

    verbose = verbose_mode;

    if (file_string == NULL)
	file_string = gde_substitute_string("${file}");

    filelist = list_select("file[DseDiagram && name = '${file_string}']");

    if (list_count(filelist) != 1) 
    { 
	print_message("Diagram must be saved before checking semantics.");
	return 1;
    } 

    else
    {
        for (ix = 0; ix < list_count(filelist); ix = ix + 1)
        {
            fileobj = list_get(filelist, ix);

			if (file_history(fileobj))
			{
				print_message("File '" + fileobj.name + "' needs to saved to the repository before checking semantics.");
        		return 1;
			}
    	}

    	print_message("Checking diagram " + fileobj.name + "...");
		errors = errors + file_history(fileobj);
    	nodelist = list_select("node[node_refs[file_id=${fileobj.id}]]");
		errors = errors + dse_check(fileobj, nodelist);
    }

    message = "DSE checking completed with " +  errors + " error";

	if (errors > 1)
		message = message + "s";
		
    print_message(message);

	return errors;

}





//
// The main routine for this script, which calls all the appriate checking
// routines.
//
int dse_check(file fileobj, list nodelist)
{
    int errors = 0; 	    // # errors

    errors = errors + check_elements(fileobj, nodelist);
    errors = errors + check_components(fileobj, nodelist);
    errors = errors + check_for_recursion(fileobj, nodelist);
    return errors;
}


//
// Check that all primitive data elements have a Leaf.LeafType Annotation.
//
int
check_elements(file fileobj, list nodelist)
{
    int errors = 0;		// # errors
    node nd;	    	    	// data elements
    string query;   		// query of all data elements
    int i;
    node one_node, bad_node;

    if (verbose)
	message("Checking for LeafType Annotation on all primitive data elements");

    for(i = 0; i < list_count(nodelist); i = i + 1)
    {
		one_node = list_get(nodelist, i);
		query = "node[" + one_node.id + " && " + 
				"!out_links && !notes[Leaf && items[LeafType]]]";
		bad_node = find_by_query(query);
	
		if (bad_node!=NULL)
		{
    		errors = errors + 1;
			print_error(bad_node.name + "' does not have a LeafType Annotation", 						node_appids(bad_node,fileobj));
		}
    }

    return(errors);
}



//
// Checks that no non-primitive data elements can have a 
// LeafType note item.
// Check that all components of a structure are of the same type.
// It also checks for multiple iterations as components of a structure.
//
int
check_components(file fileobj, list nodelist)
{
    int errors = 0;		
    item item;
    node node, bad_node;
    int i;

    if (verbose)
	message("Checking components of structures for types...");

    for(i = 0; i < list_count(nodelist); i = i + 1)
    {
		node = list_get(nodelist, i);

		bad_node = find_by_query("node[${node.id} && out_links && items[LeafType]]");
		if (bad_node!=NULL)
		{
    		errors = errors + 1;
			print_error(bad_node.name + "' cannot have both a LeafType item and children", node_appids(bad_node,fileobj));
		}
    }
    return(errors);
}



//
// Check that data definitions are not recursive
// A recursive structure is one who's name also appears as a child to that
// structure. 
int
check_for_recursion(file fileobj, list nodelist)
{
    int errors = 0;             // # errors
    string query;
    node one_node;
    node_ref one_node_ref;
    list node_list = list_create("node", 0);
    list list1 = list_create("node", 0);
    int i;
 
    if (verbose)
        message("Checking for recursive data definitions ...");

    // Find all root nodes (i.e. no in links) in file.
    query = "node_ref[!in_link_refs && file_id=${fileobj.id}]";
    id_list_create(query, "root_node_refs");

    query = "node[node_refs[root_node_refs]]";
    node_list = list_select(query);

    for(i = 0; i < list_count(node_list); i = i + 1)
    {
        one_node = list_get(node_list, i);
	errors = errors + check_children(fileobj, one_node, list1);
	list_clear(list1);
    }
    id_list_free("root_node_refs");
    return(errors);
}
 
//
// Returns error count if the node is already in the list of
// nodes in the tree.
//
int
check_children(file fileobj, node node, list tree)
{
    int		errors = 0;
    string 	query;
    list 	child_list = list_create("node", 0);
    node 	one_node;
    int   	i;


    if (list_find(tree, 0, node) != list_count(tree))
    {
    	errors = errors + 1;
		print_error(node.name + "' contains itself recursively",
					node_appids(node,fileobj));
    }

    list_append(tree, node);
   
    query = "node[in_links[link_refs && from_node_id==${node.id}]]";

    child_list = list_select(query);

    for(i = 0; i < list_count(child_list); i = i + 1)
    {
        one_node = list_get(child_list, i);
        errors = errors + check_children(fileobj, one_node, tree);
    }

    list_delete(tree, list_find(tree, 0, node));
    return errors;
}
 

