#include "qrl/ted/tabgen.inc"


list dedt_data_list1;
list dedt_data_list2;
list dedt_data_list3;

//
// initilize_data_element_def_tab1()
// sets up the cells and headers for the first data definition table 
//
void
initialize_data_element_def_tab1(string tabtype, int link_id, string tabname)
{
    int ix, num_cells;

    define_table_format(tabtype, tabname);
    
   // set up the cells in the table
    define_table_cell(tabtype, 150);
    define_table_cell(tabtype, 120);
    define_table_cell(tabtype, 150);
    define_table_cell(tabtype, 100);
    define_table_cell(tabtype, 100);

    // set up header cells 
    define_table_header_span(tabtype, 1, BOLD, Center, "Data Element\n (PUI)");
    define_table_header_span(tabtype, 1, BOLD, Center, "Structure");
    define_table_header_span(tabtype, 1, BOLD, Center, "Description");
    define_table_header_span(tabtype, 1, BOLD, Center, "CSCI/HWCI Source");
    define_table_header_span(tabtype, 1, BOLD, Center, "CSCI/HWCI User");

    // set up invariant parts of body cells
    num_cells = define_table_body_cells(tabtype, STANDARD, Center);
}


//
// initilize_data_element_def_tab2()
// sets up the cells and headers for the second data definition table 
//
void
initialize_data_element_def_tab2(string tabtype, int link_id, string tabname)
{
    int ix, num_cells;

    define_table_format(tabtype, tabname);
    
   // set up the cells in the table
    define_table_cell(tabtype, 150);
    define_table_cell(tabtype, 120);
    define_table_cell(tabtype, 100);
    define_table_cell(tabtype, 100);
    define_table_cell(tabtype, 100);
    define_table_cell(tabtype, 100);
    define_table_cell(tabtype, 100);

    // set up header cells 
    define_table_header_span(tabtype, 1, BOLD, Center, "Data Element\n (PUI)");
    define_table_header_span(tabtype, 1, BOLD, Center, "Structure");
    define_table_header_span(tabtype, 1, BOLD, Center, "Units of Measure");
    define_table_header_span(tabtype, 1, BOLD, Center, "Limit/Range");
    define_table_header_span(tabtype, 1, BOLD, Center, "Accuracy");
    define_table_header_span(tabtype, 1, BOLD, Center, "Precision/Resolution");
    define_table_header_span(tabtype, 1, BOLD, Center, "Frequency");

    // set up invariant parts of body cells
    num_cells = define_table_body_cells(tabtype, STANDARD, Center);

}


//
// initilize_data_element_def_tab3()
// sets up the cells and headers for the third data definition table 
//
void
initialize_data_element_def_tab3(string tabtype, int link_id, string tabname)
{
    int ix, num_cells;

    define_table_format(tabtype, tabname);
    
   // set up the cells in the table
    define_table_cell(tabtype, 150);
    define_table_cell(tabtype, 120);
    define_table_cell(tabtype, 100);
    define_table_cell(tabtype, 100);
    define_table_cell(tabtype, 100);
    define_table_cell(tabtype, 100);

    // set up header cells 
    define_table_header_span(tabtype, 1, BOLD, Center, "Data Element\n (PUI)");
    define_table_header_span(tabtype, 1, BOLD, Center, "Structure");
    define_table_header_span(tabtype, 1, BOLD, Center, "Legality Checks");
    define_table_header_span(tabtype, 1, BOLD, Center, "Data Type");
    define_table_header_span(tabtype, 1, BOLD, Center, "Data Representation");
    define_table_header_span(tabtype, 1, BOLD, Center, "Priority");

    // set up invariant parts of body cells
    num_cells = define_table_body_cells(tabtype, STANDARD, Center);

}


//
// fill_data_element_def_tabs()
// fills up the list which contains the table data
//
void
fill_data_element_def_tabs(int link_id, string link_name, string caption)
{
    fill_dedt_data_lists(link_id, link_name);
    define_table_caption("IRS_dedt1", caption);
    define_table_caption("IRS_dedt2", caption + " (cont)");
    define_table_caption("IRS_dedt3", caption + " (cont)");
    print_data_definition_table("IRS_dedt1", dedt_data_list1, 5);
    print_data_definition_table("IRS_dedt2", dedt_data_list2, 7);
    print_data_definition_table("IRS_dedt3", dedt_data_list3, 6);
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
// fill_dedt_data_lists()
// The data list is populated ONLY if there exists a data structure which
// defines the given interface. If a data structure exists, then it calls
// fill_dedt_data_list. It also passes the values of the source/destination
// csci.
//
void
fill_dedt_data_lists(int link_id, string link_name)
{
    string query;
    list table_data_list;
    node one_node = NULL;
    node csci_node = NULL;
    string source_csci;
    string dest_csci;
    
    //8/23/95 PEC--added Selection and Enum types
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

	dedt_data_list1 = list_create("string",0);
	dedt_data_list2 = list_create("string",0);
	dedt_data_list3 = list_create("string",0);
	fill_dedt_table_lists(one_node.name, one_node, source_csci, dest_csci);
    }
    else
    {
	display_warning("No Data Structure found for interface " +
	    link_name);

    }
}



//
// fill_dedt_table_lists()
// Recursively goes to the leaf nodes of the data structure and extracts
// the appropriate annotations. These values are then placed into the
// data list.
//
void
fill_dedt_table_lists(string parent, node structure, string source, string dest)
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
	fill_dedt_table_lists(parent, child, source, dest);
    }

    // now that we're at the leaf elements, put the right info into the list
    if (num_children == 0)
    {
	list_append(dedt_data_list1, structure.name + "\n("+ find_unique_identifier(structure.id)+")");
	list_append(dedt_data_list2, structure.name + "\n("+ find_unique_identifier(structure.id)+")");
	list_append(dedt_data_list3, structure.name + "\n("+ find_unique_identifier(structure.id)+")");
	list_append(dedt_data_list1, parent);
	list_append(dedt_data_list2, parent);
	list_append(dedt_data_list3, parent);

	query = "note[DataDefinition && node[${structure.id}]]";
	one_note = find_by_query(query);
	if (one_note != NULL)
	{
	    // if a description exists, add it to the list, 
	    // after removing the newlines from it
	    if (one_note.desc != "")
	    {
		desc_string = strip_last_return(one_note.desc);
		new_desc = string_search_and_replace(desc_string, "\n","\\n");
		list_append(dedt_data_list1, new_desc);
	    }
	    else
	    {
		list_append(dedt_data_list1, "no desc");
		display_warning("No Data Definition annot description " +
		    "found for the data element called " + structure.name);
	    }

	    // add the source csci to the list
	    list_append(dedt_data_list1, source);

	    // add the destination csci to the list
	    list_append(dedt_data_list1, dest);

	    add_item_to_data_list(dedt_data_list2, 
		"Units",one_note.id);

	    add_item_to_data_list(dedt_data_list2, 
		"Constraint",one_note.id);

	    add_item_to_data_list(dedt_data_list2, 
		"Accuracy",one_note.id);

	    add_item_to_data_list(dedt_data_list2, 
		"Precision",one_note.id);

	    add_item_to_data_list(dedt_data_list2, 
		"Frequency",one_note.id);

	    add_item_to_data_list(dedt_data_list3, 
		"Legality",one_note.id);

	    add_item_to_data_list(dedt_data_list3, 
		"DataType",one_note.id);

	    add_item_to_data_list(dedt_data_list3, 
		"Representation",one_note.id);

	    add_item_to_data_list(dedt_data_list3, 
		"Priority",one_note.id);
	}
	else
	{
	    // there are no annotations for this item. put in the defaults
	    list_append(dedt_data_list1, "no desc");
	    list_append(dedt_data_list1, source);
	    list_append(dedt_data_list1, dest); 
	    list_append(dedt_data_list2, "N/A");
	    list_append(dedt_data_list2, "N/A");
	    list_append(dedt_data_list2, "N/A");
	    list_append(dedt_data_list2, "N/A");
	    list_append(dedt_data_list2, "N/A");
	    list_append(dedt_data_list3, "N/A");
	    list_append(dedt_data_list3, "N/A");
	    list_append(dedt_data_list3, "N/A");
	    list_append(dedt_data_list3, "N/A");
	    display_warning("No Data Definition note " +
		"found for the data element called " + structure.name);
	}
    }
}


//
// add_item_to_data_list()
// 
void
add_item_to_data_list(list data_list, string item_type, int note_id)
{
    string query;
    item one_item = NULL;

    query = "item[${item_type} && note[" + note_id + "]]"; 
    one_item = find_by_query(query);
    if (one_item != NULL)
	list_append(data_list, one_item.value);
    else 
	list_append(data_list, "N/A");
}

//
//PEC 10/12/95 function to print message cross reference tables.
//Tables will be filled from MSG_LIST.
//
void
message_cross_reference_tables()
{

    // Do a message cross reference table
    initialize_messages_xref_tab("IDD_msg", "msg_xref1" );
    fill_xref_tab("IDD_msg", "Message Cross Reference Table") ;


    // Do a data element cross reference table
    initialize_elem_xref_tab("IDD_elem", "elem_xref1" );
    fill_elem_xref_tab("IDD_elem", "Data Element Cross Reference Table") ;

    //clear MSG_LIST
    list_clear(MSG_LIST);
}

//
// initilize_messages_xref_tab()
// PEC 10/12/95 Set up messages to data elements cross-reference table 
//
void
initialize_messages_xref_tab(string tabtype, string tabname)
{
    int ix, num_cells;

    if (debug)
        message("Initializing xref table for:  " + tabname);

    define_table_format(tabtype, tabname);

   // set up the cells in the table
    define_table_cell(tabtype, 200);
    define_table_cell(tabtype, 200);

    // set up header cells
    define_table_header_span(tabtype, 1, BOLD, Center, "Message");
    define_table_header_span(tabtype, 1, BOLD, Center, "Data Elements");

    // set up invariant parts of body cells
    num_cells = define_table_body_cells(tabtype, STANDARD, Center);
}

//PEC 10/12/95 function to fill msg xref table
void
fill_xref_tab(string tabtype, string caption)
{
    msg_struct one_msg_str;
    node elem ;
    string msg_name, elements;
    int i,j;
 
    define_table_caption(tabtype, caption);

    // generate the fill file
    generate_table_header(tabtype);
    generate_table_body_start(tabtype);

    //Sort MSG_LIST so we get our messages in alphabetical order
    MSG_LIST=my_list_sort(MSG_LIST);

    for(i=0;i<list_count(MSG_LIST);i++)
    {
        one_msg_str=list_get(MSG_LIST,i);
	msg_name=one_msg_str.msg_node.name;
	//Put data elements into a string of names
	for(j=0;j<list_count(one_msg_str.elements);j++)
	{
	   elem=list_get(one_msg_str.elements,j);
	   if (j==0)
		elements=elem.name;
	   else
		elements=elements + ", " + elem.name;
	}
	//Output row
	generate_body_cell(tabtype, msg_name);
	generate_body_cell(tabtype, elements);
    }
    generate_table_body_end(tabtype);
    generate_table_finish(tabtype);
    output_table(tabtype);
    clear_table_for_regeneration(tabtype);
}

//
// PEC 10/12/95 Set up data element cross-reference table 
//
void
initialize_elem_xref_tab(string tabtype, string tabname)
{
    int ix, num_cells;

    if (debug)
        message("Initializing element xref table:  " + tabname);

    define_table_format(tabtype, tabname);

   // set up the cells in the table
    define_table_cell(tabtype, 200);
    define_table_cell(tabtype, 200);

    // set up header cells
    define_table_header_span(tabtype, 1, BOLD, Center, "Data Element");
    define_table_header_span(tabtype, 1, BOLD, Center, "Messages");

    // set up invariant parts of body cells
    num_cells = define_table_body_cells(tabtype, STANDARD, Center);
}

//
//PEC 10/12/95 fill data element to message xref table
//
void
fill_elem_xref_tab(string tabtype, string caption)
{
    node one_node, elem ;
    list select_list;
    int i,k;
    msg_struct one_msg;
    list element_msgs=list_create("string",0);
    string msg_string="N/A";

    define_table_caption(tabtype, caption);

    // generate the fill file
    generate_table_header(tabtype);
    generate_table_body_start(tabtype);
    select_list=list_select("node[DSE_NODES && items[DataType]] sort by name");
    for(i=0;i<list_count(select_list);i=i+1)
    {
        elem=list_get(select_list,i);
        //Find this node in MSG_LIST, keep a list of any msgs it belongs to
        for(k=0;k<list_count(MSG_LIST);k=k+1)
        {
           one_msg=list_get(MSG_LIST,k);
           //See if our element is in this message's list of elements
           if (list_find(one_msg.elements,0,elem)<list_count(one_msg.elements))
                list_append(element_msgs,one_msg.msg_node.name);
        }
        //Now we should have a list of msgs that our element belongs to
        //and we can print the row
        if ( list_count(element_msgs) > 0 )
        {
           msg_string=list_to_string(element_msgs,", ");

           generate_body_cell(tabtype, elem.name);
           generate_body_cell(tabtype, msg_string);
           if (debug)
                message("Printing row:  msgs="+msg_string+" & elem="+elem.name);        }
        else
           message("WARNING: No messages found for data element:  "+elem.name);

        //Clear the list of msgs for the next element
        list_clear(element_msgs);
    }
    generate_table_body_end(tabtype);
    generate_table_finish(tabtype);
    output_table(tabtype);
    clear_table_for_regeneration(tabtype);
}


//*** ALL below are functions to perform a list sort *****


//Functions in this file support sorting of lists

list
ordered_list_from_index_list (list List, list IndexList)
{
   list Result ;
   int i, index ;
   int ListCount = list_count (List) ;
   int IndexCount = list_count (IndexList) ;
 
   if (ListCount > 0)
       Result = list_create (list_get_type (List, 0), 0) ;
   else
       Result = list_copy(List);
 
 
   for (i=0; i< IndexCount; i=i+1)
   {
      index = list_get (IndexList, i) ;
      if (index < ListCount)
      {
         list_append (Result, list_get (List, index)) ;
      }
   }
   return (Result) ;
}
 

 
list
sorted_index_list (list List)
{
   list Result =list_create ("int", 0) ;
   int Count = list_count (List) ;
   list IndexList = list_create ("int", 0) ;
   int MinIndexListIndex ;
 
   int i ;
   for (i=0; i< Count; i=i+1)
   {
      list_append (IndexList, i) ;
   }
 
   while (Count > 0)
   {
       MinIndexListIndex = index_list_minimum_index (List, IndexList) ;
       list_append (Result, list_get (IndexList, MinIndexListIndex)) ;
       list_delete (IndexList, MinIndexListIndex) ;
       Count = Count - 1 ;
   }
 
   return (Result) ;
}
 
 
int
index_list_minimum_index (list List, list Indexes)
{
   int Count = list_count (Indexes) ;
   int i ;
   int index ;
   boolean is_less_than ;
   string type ;
 
   if (Count > 0)
   {
      type = list_get_type (List, list_get(Indexes, 0)) ;
   }
 
   int Result = 0 ;
 
   // for each index in Indexes
   for (i=1; i<Count; i=i+1)
   {
      if (type == "msg_struct")
      {
         is_less_than = msg_is_less_than
           (list_get(List, list_get (Indexes, i)),
           list_get(List, list_get(Indexes,Result))) ;
      }
      else if (type == "node")
      {
         is_less_than = node_is_less_than
           (list_get(List, list_get (Indexes, i)),
           list_get(List, list_get(Indexes,Result))) ;
      }
      else if (type == "link")
      {
         is_less_than = link_is_less_than
           (list_get(List, list_get (Indexes, i)),
           list_get(List, list_get(Indexes,Result))) ;
      }
      else if (type == "file")
      {  
         is_less_than = file_is_less_than
           (list_get(List, list_get (Indexes, i)),
           list_get(List, list_get(Indexes,Result))) ;
      }  
      else if (type == "string")
      {
        is_less_than = string_is_less_than
          (list_get(List, list_get (Indexes, i)),
           list_get(List, list_get(Indexes,Result))) ;
      }
 
      if (is_less_than)
      {
         Result = i ;
      }
   }
   return (Result) ;
}


boolean
msg_is_less_than (msg_struct one, msg_struct two)
{
   return (one.msg_node.name < two.msg_node.name) ;
}
boolean
string_is_less_than (string one, string two)
{
   return (one < two) ;
}

boolean
file_is_less_than (file one, file two)
{
   return (one.name < two.name);
}

boolean
link_is_less_than (link one, link two)
{
   return (one.name < two.name);
}
boolean
node_is_less_than (node one, node two)
{
   return (one.name < two.name);
}
 
list
my_list_sort (list List)
{
   return (ordered_list_from_index_list (List, sorted_index_list (List))) ;
}
