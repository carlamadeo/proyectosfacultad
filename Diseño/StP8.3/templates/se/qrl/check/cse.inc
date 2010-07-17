//////////////////////////////////////////////////////////////////////////////
//
// 	StP/SE
// 	Confidential property of Aonix
// 	Copyright (c) 1992-1999
// 	All rights reserved
//
//////////////////////////////////////////////////////////////////////////////

#include "qrl/include/appids.inc"
#include "se/rules/qrl/cspec.inc"

// Each Event must be defined in an ELT
// Each Action must be defined an an ALT

int
check_elt_alt_definitions(file fileobj, string node_type)
{
    int		errors = 0;
    node 	node;
    string	query, table_type;

    if (node_type == EVENT)
    	table_type = EVENT_LOGIC_TABLE;
    else
    	table_type = ACTION_LOGIC_TABLE;

    query = "node[${node_type} && node_refs[file_id = ${fileobj.id}] && " +
		"!node_refs[file[${table_type}]]]";

    for_each_in_select(query, node)
    {
	if (node.name != "")
	{
	    print_error("'" + node.name + "' " +
		    "is not defined in an " + table_type, 
		    node_appids(node, fileobj));
	    ++errors;
	}
    }
    return(errors);
}


//
//  Check for multiple links with the same start node and event name and
//  for multiple references to the same link.
//
//  5/9/99 MHS This function is used by ste, stt, and sem, so it is in a shared place.
//  Fixes Remedy request 0002309
//

int check_ste_duplicate_links(file fileobj)
{
    int errors = 0;
    int ix, count;
    string query;
    link linkobj;
    node nodeobj;
    list syms;                  // symbol references to links
    set names, duplicates;      // set of all names, set of duplicate names

    if (cseVerbose)
         print_message("Checking that no two transitions have the same start node and event and no transition is referenced more than once...");

    names = set_create("string");
    duplicates = set_create("string");
    syms = list_create("int", 0);

    // Only links from nodes with multiple outgoing links are contenders
    // for duplication
    query = "node[node_refs[file_id == ${fileobj.id}] && " +
                "out_links[link_refs[file_id == ${fileobj.id}] && " +
                                    "Transition && name != null]]";
 
    for_each_in_select(query, nodeobj)
    {
        query = "link[from_node_id == ${nodeobj.id} && Transition && " +
                    "link_refs[file_id == ${fileobj.id}]]";
 
        // collect a set of names; keep track of duplicates
        for_each_in_select(query, linkobj)
        {
            if (set_is_member(names, linkobj.name))
                set_add(duplicates, linkobj.name);
            else
                set_add(names, linkobj.name);
        }
 
        // for each duplicate, signal an error with symbol references
        for (ix = 0, count = set_count(duplicates); ix < count; ix = ix + 1)
        {
            query = "link[name == '" + set_get_element(duplicates, ix) +
                        "' && link_refs[file_id == ${fileobj.id}]]";
            for_each_in_select(query, linkobj)
                list_concatenate(syms, link_appids(linkobj, fileobj));
 
            print_error("Transition with start node '" + nodeobj.name + "' and Event name '" + linkobj.name + "' is defined more than once", syms);
            errors++;
            list_clear(syms);
        }
        set_clear(names);
        set_clear(duplicates);
    }
 
    // find links which have >1 reference
    // can't use an id_list inside a count
    query = "link[Transition && count[link_refs[file_id == ${fileobj.id}]] > 1]";
    for_each_in_select(query, linkobj)
    {
	list_clear(syms);
	list_concatenate(syms, link_appids(linkobj, fileobj));
	print_error("Transition with event `" + linkobj.name + "' appears more than once", syms);
	errors++;
    }

    return(errors);
}
