
void
generate_pspec_primitive (string process_index, string process_name, 
					 string file_name, int process_id)
{
	string		query;
	note		pspec;
	
    // Header
    paragraph(SectionFormat);

    print_line("Process " + process_index + ": " + process_name);

    print("Pspec generated from the data flow diagram \"" + file_name + "\" ");
    print("of the \"" + current_system() + "\" system of the \"" + 
    	    	current_projdir() + "\" projdir ");
    print_line("at " + time_to_string(time_now(), NULL) + 
    	       " by " + user() + "@" + hostname());

    paragraph(FlowsFormat);

    // Data Flows
    print_flows(process_id, "DataFlow", "data");

    // Control Flows
    print_flows(process_id, "ControlFlow", "control");

    // Description
    paragraph(SectionFormat);
    print_line("description");
    query = "note[Pspec && obj_id == ${process_id}]";
    pspec = find_by_query(query);
    if (pspec != NULL)
    {
    	paragraph();
    	print(pspec.desc);
    	paragraph();
    }
    print_line("end pspec");
}

//
// Print all flows of type flowtype which are connected to 
//
void print_flows(int id, string flowtype, string flow)
{
    string query;   	    
    int num_flows; 	    // # flows
    list flows, all_flows;  // list of input or output Flows
			    // list of all Flows

    flows = list_create("link", 0);
    all_flows = list_create("link", 0);

    // All flows
    query = "link[link_refs && ${flowtype} && " + 
    	    	"(to_node_id == ${id} || from_node_id == ${id})]";
    all_flows = list_select(query);
    num_flows = list_count(all_flows);

    paragraph();
    print("This process has " + num_flows + " " + flow + " flow");
    if (num_flows != 1)
    	print("s");
    if (num_flows == 0)
	return;

    print_line(":");
    print_list(all_flows);

    // Input flows
    paragraph();
    print_line("input " + flow + " flows");
    query = "link[link_refs && to_node_id == ${id} && ${flowtype}]";
    flows = list_select(query);
    print_list_with_type(flows);

    // Output flows
    paragraph();
    print_line("output " + flow + " flows");
    query = "link[link_refs && from_node_id == ${id} && ${flowtype}]";
    flows = list_select(query);
    print_list_with_type(flows);
}


//
// print a comma-separated list of all flows in the set
//
void print_list(list flows)
{
    int ix, count;
    link flow;

    // don't print a comma after the last flow
    for (ix = 0, count = list_count(flows); ix < count; ix = ix + 1)
    {
    	if (ix > 0) 
    	    print(", ");
    	flow = list_get(flows, ix);
    	print(flow.name);    
    	if (ix == count - 1)
    	    print_line();
    }
}

//
//  print a list of flow names and types, one to a line.
//
void print_list_with_type(list flows)
{
    int ix, count;
    link flow;

    for (ix = 0, count = list_count(flows); ix < count; ix = ix + 1)
    {
    	flow = list_get(flows, ix);
    	print_line(flow.name + " : " + flow.type);
    }
}
