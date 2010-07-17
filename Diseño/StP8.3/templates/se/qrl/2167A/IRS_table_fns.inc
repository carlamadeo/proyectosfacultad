#include "qrl/ted/tabgen.inc"

external boolean preview = False;

list ddt_data_list1;
list ddt_data_list2;

//
// initilize_data_definition_table()
// sets up the cells and headers for the data definition table 
//
void
initialize_data_definition_table1(string tabtype, int link_id, string tabname)
{
    int ix, num_cells;

    define_table_format(tabtype, tabname);
    
   // set up the cells in the table
    define_table_cell(tabtype, 150);
    define_table_cell(tabtype, 120);
    define_table_cell(tabtype, 200);
    define_table_cell(tabtype, 100);

    // set up header cells 
    define_table_header_span(tabtype, 1, BOLD, Center, "Structure");
    define_table_header_span(tabtype, 1, BOLD, Center, "Identifier");
    define_table_header_span(tabtype, 1, BOLD, Center, "Description");
    define_table_header_span(tabtype, 1, BOLD, Center, "Source CSCI");

    // set up invariant parts of body cells
    num_cells = define_table_body_cells(tabtype, STANDARD, Center);
}

//
// initilize_data_definition_table2()
// sets up the cells and headers for the data definition table 
//
void
initialize_data_definition_table2(string tabtype, int link_id, string tabname)
{
    int ix, num_cells;

    define_table_format(tabtype, tabname);
    
   // set up the cells in the table
    define_table_cell(tabtype, 150);
    define_table_cell(tabtype, 120);
    define_table_cell(tabtype, 180);
    define_table_cell(tabtype, 100);
    define_table_cell(tabtype, 100);
    define_table_cell(tabtype, 100);
    define_table_cell(tabtype, 100);

    // set up header cells 
    define_table_header_span(tabtype, 1, BOLD, Center, "Structure");
    define_table_header_span(tabtype, 1, BOLD, Center, "Identifier");
    define_table_header_span(tabtype, 1, BOLD, Center, "Destination CSCI");
    define_table_header_span(tabtype, 1, BOLD, Center, "Units of Measure");
    define_table_header_span(tabtype, 1, BOLD, Center, "Limit/Range");
    define_table_header_span(tabtype, 1, BOLD, Center, "Accuracy");
    define_table_header_span(tabtype, 1, BOLD, Center, "Precision/Resolution");

    // set up invariant parts of body cells
    num_cells = define_table_body_cells(tabtype, STANDARD, Center);

}


void
fill_data_definition_tables(int link_id, string link_name, string caption)
{
    fill_ddt_data_lists(link_id, link_name);
    define_table_caption("IRS_ddt1", caption);
    define_table_caption("IRS_ddt2", caption + " (cont)");
    print_data_definition_table("IRS_ddt1", ddt_data_list1, 4);
    print_data_definition_table("IRS_ddt2", ddt_data_list2, 7);
}

//
// fill_data_definition_table()
// Fills up the data definition table w/ the contents of the data list
//
void
print_data_definition_table(string tabtype, list data_list, int num_columns)
{

    int ix;    		// used to travers data_list
    int count;		// size of the data_list

    // generate the fill file
    generate_table_header(tabtype);
    generate_table_body_start(tabtype);

    count = list_count(data_list);
    for (ix = 0; ix < count; ix = ix + 1)
    {
        generate_body_cell(tabtype, list_get(data_list,ix));
        if (ix == num_columns)
            undefine_table_border(tabtype, BODY, 0, BOTTOM);
    }

    list_clear(data_list);
    generate_table_body_end(tabtype);
    generate_table_finish(tabtype);
 
    output_table(tabtype);
    
    clear_table_for_regeneration(tabtype);
}


//
// fill_ddt_data_lists()
// The data list is populated ONLY if there exists a data structure which
// defines the given interface. If a data structure exists, then it calls
// fill_ddt_data_list. It also passes the values of the source/destination
// csci.
//
void
fill_ddt_data_lists(int link_id, string link_name)
{
    string query;
    list table_data_list;
    node one_node = NULL;
    node csci_node = NULL;
    string source_csci;
    string dest_csci;
    
    //8/23/95 PEC--added Selection and Enumeration types
    query = "node[(Sequence||Selection||Enumeration) && name == '" + link_name + "']";
    one_node = find_by_query(query);
    if (one_node != NULL)
    {
	query = "node[out_links[${link_id}]]";
	csci_node = find_by_query(query);
	if (csci_node != NULL)
	{
	    source_csci = csci_node.name;
	}
	
	query = "node[in_links[${link_id}]]";
	csci_node = find_by_query(query);
	if (csci_node != NULL)
	{
	    dest_csci = csci_node.name;
	}

	ddt_data_list1 = list_create("string",0);
	ddt_data_list2 = list_create("string",0);
	fill_ddt_table_lists(one_node.name, one_node, source_csci, dest_csci);
    }
    else
    {
	display_warning("No Data Structure found for interface " +
	    link_name);
    }
}



//
// fill_ddt_table_lists()
// Recursively goes to the leaf nodes of the data structure and extracts
// the appropriate annotations. These values are then placed into the
// data list.
//
void
fill_ddt_table_lists(string parent, node structure, string source, string dest)
{

    int i;
    string query;
    node one_node = NULL;
    list children;
    int num_children;
    node child;
    note one_note = NULL;
    item one_item = NULL;
    string new_desc;
    string desc_string;

    query = "node[in_links[Component && from_node_id == ${structure.id}]]";

    // go thru all his children
    children = list_select(query);
    num_children = list_count(children);
    
    for (i = 0; i < num_children; i = i + 1)
    {
	child = list_get(children, i);
	fill_ddt_table_lists(parent, child, source, dest);
    }

    // now that we're at the leaf elements, put the right info into the list
    if (num_children == 0)
    {
	list_append(ddt_data_list1, parent);
	list_append(ddt_data_list2, parent);
	list_append(ddt_data_list1, structure.name);
	list_append(ddt_data_list2, structure.name);

	query = "note[DataDefinition && node[${structure.id}]]";
	one_note = find_by_query(query);
	if (one_note != NULL)
	{
	    if (one_note.desc != "")
	    {
		// add the note description to the list
		// after removing the newlines from it
		desc_string = strip_last_return(one_note.desc);
		new_desc = string_search_and_replace(desc_string, "\n","\\n ");
		list_append(ddt_data_list1, new_desc);
	    }
	    else
	    {
		list_append(ddt_data_list1, "no desc");
		display_warning("No Data Definition annot description " +
		    "found for the data element called " + structure.name);	
	    }

	    // add the source csci to the list
	    list_append(ddt_data_list1, source);

	    // add the destination csci to the list
	    list_append(ddt_data_list2, dest);

	    query = "item[Units && note_id == ${one_note.id}]";
	    one_item = find_by_query(query);
	    if (one_item != NULL)
		list_append(ddt_data_list2, one_item.value);
	    else
		list_append(ddt_data_list2, "N/A");

	    query = "item[Constraint && note_id == ${one_note.id}]";
	    one_item = find_by_query(query);
	    if (one_item != NULL)
		list_append(ddt_data_list2, one_item.value);
	    else
		list_append(ddt_data_list2, "N/A");

	    query = "item[Accuracy && note_id == ${one_note.id}]";
	    one_item = find_by_query(query);
	    if (one_item != NULL)
		list_append(ddt_data_list2, one_item.value);
	    else
		list_append(ddt_data_list2, "N/A");

	    query = "item[Precision && note_id == ${one_note.id}]";
	    one_item = find_by_query(query);
	    if (one_item != NULL)
		list_append(ddt_data_list2, one_item.value);
	    else
		list_append(ddt_data_list2, "N/A");
	}
	else
	{
	    // there are no annotations for this item. put in the defaults
	    list_append(ddt_data_list1, "no desc");
	    list_append(ddt_data_list1, source);
	    list_append(ddt_data_list2, dest); 
	    list_append(ddt_data_list2, "N/A");
	    list_append(ddt_data_list2, "N/A");
	    list_append(ddt_data_list2, "N/A");
	    list_append(ddt_data_list2, "N/A");
	    display_warning("No Data Definition note " +
                "found for the data element called " + structure.name);
	}
    }
}

