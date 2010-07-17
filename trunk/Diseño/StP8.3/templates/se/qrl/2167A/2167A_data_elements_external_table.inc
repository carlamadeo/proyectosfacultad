// Used to determine the direction of a interface w.r.t. CSCI
enum		direction {input, output};
enum		document_type {SRS, IRS, IDD, SDD};


// INCLUDE_DATA_ELEMENTS_EXTERNAL_TABLE

void
include_data_elements_external_table (document_type doc_type, string 
									  table_caption)
{
	set			unique_external_interfaces,
				temp_set,
				element_direction,
				capability_name_set,
				all_elements;
	list		interface_children;
	link		one_interface;
	int			i,
				j;
	string		interface_name,
				element_name,
				capability_name,
				capability_string,
				process_zero_name,
				query,
				interface_doc,
				table_type;
	item		doc_item,
				doc_num;
	boolean		debug = False;
	node		capability_node;
	
	
	message ("       ..CSCI data element external requirements table");
	
	// Slightly different for SRS vs SDD
	if (doc_type == SRS)
	{
		process_zero_name = 
			return_process_name_by_id (return_process_id_by_index("0"));
		interface_doc = "IRS";
	}
	else if (doc_type == SDD)
	{
		// the SRS uses DFD specific caches that aren't worthwhile setting
		// up for the SDD
		capability_node = find_by_query 
			("node[Process && node_refs[file[DfeDiagram && name='top']]]");
		if (capability_node == NULL)
		{
			display_warning ("No process found in DFD top");
			process_zero_name = "??";
		}
		else
			process_zero_name = capability_node.name;
		interface_doc = "IDD";
	}
	else
	{
		display_error ("No Data elements external requirements table for " +
			"document of type " + doc_type);
		return;
	}

	//
	// Set up lists/sets for tables
	
	// get unique set of external interfaces
	unique_external_interfaces = set_create ("string");
	for_each_in_select ("link[external_interfaces] sort by name", one_interface)
	{
		set_add (unique_external_interfaces, one_interface.name);
	}
	
	// - create parallel set of elements for each interface
	// - a set of all elements
	interface_children = list_create ("set", 0);
	temp_set = set_create("string");
	all_elements = set_create ("string");
	for (i=0; i<set_count(unique_external_interfaces); i=i+1)
	{
		interface_name = set_get_element(unique_external_interfaces, i);
		temp_set = get_interface_elements(interface_name);
		list_append (interface_children, set_copy(temp_set));
		set_union (all_elements, temp_set);
		set_clear (temp_set);
	}	

	if (set_count(all_elements) == 0)
	{
		display_error ("No data elements found for all external flows \n" +
			"No data element external requirements table will be included");
		return;
	}
	
	//
	// Table 1
	//   element name - all ext. interf. name, ids, & IRS/IDD document
	
	// initialize table 1
	if (debug)
		message("init table 1");
	table_type = "data_ex_elements_tab1";
	define_table_format (table_type, "");
	define_table_cell(table_type, 250);
	define_table_cell(table_type, 250);
	define_table_cell(table_type, 200);
	define_table_cell(table_type, 250);
	define_table_header_span(table_type, 1, BOLD, Center, "Name");
	define_table_header_span(table_type, 1, BOLD, Center, "Interface name");
	define_table_header_span(table_type, 1, BOLD, Center, "Interface ID");
	define_table_header_span(table_type, 1, BOLD, Center, 
		interface_doc + " Name");
    define_table_body_cells(table_type, STANDARD, Center);
	generate_table_header(table_type);
	generate_table_body_start(table_type);
		
	// For all elements
	for (i=0; i<set_count(all_elements); i=i+1)
	{
		element_name = set_get_element(all_elements, i);
		
		// look for unique interfaces that are parents
		for (j=0; j<list_count(interface_children); j=j+1)
		{
			temp_set = list_get(interface_children, j);
			if (set_is_member (temp_set, element_name))
			{
				interface_name = 
					set_get_element (unique_external_interfaces, j);
				query = "link[external_interfaces && name = " +
						"'${interface_name}']"; 
				for_each_in_select (query, one_interface)
				{
					if (debug)
					{
						message("tab1: name =>" + element_name);
						message("tab1: int/name =>" + interface_name);
						message("tab1: ident =>" + 
							find_unique_identifier(one_interface.id));
					}
					generate_body_cell(table_type, element_name);
					generate_body_cell(table_type, interface_name);
					generate_body_cell(table_type, 
						find_unique_identifier(one_interface.id));
					
					// IRS/IDD Name
					doc_item = find_by_query 
						("item[obj_id=${one_interface.id} && DocumentName && " +
						 "value = '${interface_doc}']");
					if (doc_item != NULL)
					{
						doc_num = find_by_query
							("item[note_id=${doc_item.note_id} && " +
							 "DocumentNumber]");
						if (doc_num != NULL)
						{
							if (debug)
								message("tab1: IRS/IDD Name =>" + "IRS/IDD " + 
									doc_num.value);
							generate_body_cell(table_type, interface_doc + 
								" " + doc_num.value);
						}
						else
						{
							if (debug)
								message("tab1: IRS/IDD Name => none");
							generate_body_cell(table_type, interface_doc + 
								" ???");
							display_warning 
								("No IRS/IDD document # for the flow " +
								one_interface.name);
						}
					}
					else
					{
						if (debug)
							message("tab1: IRS/IDD Name => none");
						generate_body_cell(table_type, "???");
						display_warning 
							("No IRS/IDD document name for the flow " +
							one_interface.name);
					}
				}	 
			}	
		}
	}	
	
	// complete and output table
	generate_table_body_end(table_type);
	generate_table_finish(table_type);
	define_table_caption(table_type, table_caption + " (1 of 2)");
	output_table(table_type);   
	
	//
	// Table 2
	//   element name - all source/dest process/capability names
	//   note: Algorithm for determining the source or destination
	//         process for a given element is easy if they always
	//         go between a process and an external.  On the other
	//         hand, because methodology allows data flow decomposition,
	//         any algorithm would have to recursively procede up/down
	//         data flow diagrams and take data structures into 
	//         consideration until 1 or more elements are found.
	//         Basically, this is rather CPU intensive and can still
	//         be ambiguous.  As a result, this section will simply
	//         determine the source/dest without traversing up/down
	//         DFDs.
	
	// initialize table 2
	if (debug)
		message("init table 2");
	table_type = "data_ex_elements_tab2";
	define_table_format (table_type, "");
	define_table_cell(table_type, 250);
	define_table_cell(table_type, 600);
	define_table_header_span(table_type, 1, BOLD, Center, "Name");
	define_table_header_span(table_type, 1, BOLD, Center, 
		"Source/Destination capability");
    define_table_body_cells(table_type, STANDARD, Center);
	generate_table_header(table_type);
	generate_table_body_start(table_type);
	
	// For all elements
	for (i=0; i<set_count(all_elements); i=i+1)
	{
		element_direction = set_create ("direction");
		element_name = set_get_element(all_elements, i);
		
		if (debug)
			message ("Find directions for " + element_name);
			
		// determine if this element is an input and/or output
		for (j=0; j<list_count(interface_children); j=j+1)
		{
			temp_set = list_get(interface_children, j);
			if (set_is_member (temp_set, element_name))
			{
				interface_name = 
					set_get_element (unique_external_interfaces, j);
				
				// determine if input external interface
				query = "link[external_interfaces && name = " +
						"'${interface_name}' && from_node[External]]"; 
				if (selection_count(query) > 0)
				{
					set_add (element_direction, input);
					if (debug)
						message ("  input: " + interface_name );
				}
					
				// determine if output external interface
				query = "link[external_interfaces && name = " +
						"'${interface_name}' && to_node[External]]"; 
				if (selection_count(query) > 0)
				{
					set_add (element_direction, output);
					if (debug)
						message ("  output: " + interface_name );
				}
					
				// if both are found, no need to look further
				if (set_is_member(element_direction, input) &&
					set_is_member(element_direction, output))
					break;
			}	
		}
		
		// Now for output flows:
		//   find any flow that goes from a process to an external or 
		//   off page external with the same name as the element in
		//   question.  Anchors are not checked as anchors can be either
		//   externals (desirable) or processes (undesirable).
		capability_name_set = set_create("string");
		if (set_is_member(element_direction, output))
		{
			if (debug)
				message ("find output element interface");
			query = "node[Process && out_links[name='${element_name}' && " +
					"to_node[External || OffPageExternal]]]";
			for_each_in_select (query, capability_node)
			{
				set_add (capability_name_set, capability_node.name);
				if (debug)
					message ("  " + capability_node.name);
			}
		}
		
		// Now for input flows:  ... reverse previous comment ...
		if (set_is_member(element_direction, input))
		{
			if (debug)
				message ("find input element interface");
			query = "node[Process && in_links[name='${element_name}' && " +
					"from_node[External || OffPageExternal]]]";
			for_each_in_select (query, capability_node)
			{
				set_add (capability_name_set, capability_node.name);
				if (debug)
					message ("  " + capability_node.name);
			}
		}
		
		// Check for neither
		if (set_count(element_direction) == 0)
		{
			// This should never occur
			display_error ("No external interface found for the element " +
				element_name);
		}
		
		// none found that is specific to the element so use process zero
		else if (set_count(capability_name_set) == 0)
		{
			if (debug)
			{
				message("tab1: Name => " + element_name);
				message("tab2: Capability => " + process_zero_name);
			}
			generate_body_cell(table_type, element_name);
			generate_body_cell(table_type, process_zero_name);
		}
		
		// go thru list
		else 
		{
			if (debug)
				message("tab1: ... => " + element_name);
				
			generate_body_cell(table_type, element_name);
			capability_string = "";
			for (j=0; j<set_count(capability_name_set); j=j+1)
			{
				capability_name = set_get_element(capability_name_set, j);
				if (debug)
					message("tab2: Capability => " + capability_name);
				if (j == 0)
					capability_string = capability_name;
				else
					capability_string = capability_string + "\\n" + 
						capability_name;
			}
			generate_body_cell(table_type, capability_string);
		}
	}	
	
	// complete and output table
	generate_table_body_end(table_type);
	generate_table_finish(table_type);
	define_table_caption(table_type, table_caption + " (2 of 2)");
	output_table(table_type);   

	// free lists/sets
	set_clear(unique_external_interfaces); 
	set_clear(all_elements); 
	list_clear(interface_children); 
}


// GET_INTERFACE_ELEMENTS
//
// This is a lil recursive function that returns a set of the leaf nodes
// (i.e. elements) of the "LINK_NAME" given to it.  This includes the
// "LINK_NAME", if appropriate.  We assume that any data structure
// with no children is an element.

set
get_interface_elements (string link_name)
{
	set		return_set;
	string	query;
	node	one_node;
	
	return_set = set_create ("string");
	
    //8/23/95 PEC--removed Iteration and added Enumeration
    query = "node[(Sequence | Selection | Enumeration) && " +
    		"name == '" + link_name + "']";
    one_node = find_by_query(query);
    if (one_node != NULL)
    {
    	set_union (return_set, get_interface_elements_help(one_node));
    }
    else
    {
    	display_warning ("No Data Definition found for the link called " +
    		link_name);
    }
    return return_set;
}

set
get_interface_elements_help (node root_node)
{
	set		return_set;
	string	query;
	node	child_node;
	
	return_set = set_create ("string");

	// if children then return combined set from recursive call
	if (selection_count("link[Component && from_node_id=${root_node.id}]") > 0)
	{
		query = "node[in_links[Component && from_node_id=${root_node.id}]]";
		for_each_in_select (query, child_node)
		{
			set_union (return_set, get_interface_elements_help(child_node));
		}
		
	}
	// else return set of this element
	else
	{
		set_add (return_set, root_node.name);
	}
	return return_set;
}

