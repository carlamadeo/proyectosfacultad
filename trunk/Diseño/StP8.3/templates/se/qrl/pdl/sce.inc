#include "se/qrl/pdl/pdl.inc"

///////////////////////////////////////////////////////////////////////////////
// File: sce.inc
//
//  Prints a pdl for a single module. The output looks
//  like the output of the following R4.2 pdl template.
//
//  module      ${name}
//
//  Pdl generated
//     ${date} at ${time} by ${user}@${hostname}
//  This module has ${#param} formal parameters:
//     ${@param}
//
//  input data params
//      ${@indata}
//  input flags
//      ${@inflag}
//
//  output data params
//      ${@outdata}
//  output flags
//      ${@outflag}
//
// calls
//      ${@calls}
// called by
//      ${@called}
// description
// end module
//
///////////////////////////////////////////////////////////////////////////////

#include "se/qrl/include/sce_defs.inc"

// Shared generic global variables
string query;
list big_param_list;
list input_data_list, output_data_list, input_flag_list, output_flag_list;

struct parameter
{
    string name;
    int id;
};


///////////////////////////////////////////////////////////////////////////////
//
// do_sce_pdl()
//
///////////////////////////////////////////////////////////////////////////////
void 
do_sce_pdl(int mod_id, string mod_name)
{
    print_header(mod_name);
    do_parameter_stuff(mod_id);
    do_calls(mod_id);
    do_called_bys(mod_id);
    print_line("description");
    print_description(mod_id);
    // SPR 2284
    paragraph();
    print_line("end module");
}


///////////////////////////////////////////////////////////////////////////////
//
// print_header()
//
///////////////////////////////////////////////////////////////////////////////
void 
print_header(string mod_name)
{
    print_line("module\t" + mod_name);
    print_line();
    print_line("PDL generated");
    print_line("\t" + time_to_string(time_now(),NULL) + " by " + 
	    user() +"@"+hostname());
}


///////////////////////////////////////////////////////////////////////////////
//
// print_lists()
//
// print the given list
//
///////////////////////////////////////////////////////////////////////////////
void 
print_lists(list param_list)
{
    int count, i;

    count = list_count(param_list);
    for (i = 0; i < count; i = i + 1)
    {
	print_line("\t" + list_get(param_list, i));
    }
    print_line();
}


///////////////////////////////////////////////////////////////////////////////
//
// do_parameter_stuff()
//
// get all the different types of parameters
// Put them in different lists, and then print them out
//
///////////////////////////////////////////////////////////////////////////////
void 
do_parameter_stuff(int mod_id)
{
    cntx cntx_var;
    list input_data_list, output_data_list, input_flag_list, output_flag_list;
    int count;

    query = "cntx[cntx_refs &&  " +
	"link[to_node_id == ${mod_id} && from_node[" + FORMAL_CALLER + "]]]";

    count = selection_count(query);

    print_line("This module has " + count + " formal parameters:");

    input_data_list = list_create("string",0);
    output_data_list = list_create("string",0);
    input_flag_list = list_create("string",0);
    output_flag_list = list_create("string",0);
    
    if (count != 0)
    {
	for_each_in_select(query, cntx_var)
	{
	    print_line("\t" + cntx_var.name);
	    if (cntx_var.type == "DataIn")
	    {
		list_append(input_data_list,cntx_var.name);
	    }
	    else if (cntx_var.type == "DataOut")
	    {
		list_append(output_data_list,cntx_var.name);
	    }
	    else if (cntx_var.type == "ControlIn")
	    {
		list_append(input_flag_list,cntx_var.name);
	    }
	    else if (cntx_var.type == "ControlOut")
	    {
		list_append(output_flag_list,cntx_var.name);
	    }
	    else if (cntx_var.type == "DataInOut")
	    {
		list_append(input_data_list,cntx_var.name);
		list_append(output_data_list,cntx_var.name);
	    }
	}
    }

    print_line();
    print_line("input data params");
    print_lists(input_data_list);
    print_line("input flags");
    print_lists(input_flag_list);
    print_line("output data params");
    print_lists(output_data_list);
    print_line("output flags");
    print_lists(output_flag_list);

    list_clear(input_data_list);
    list_clear(output_data_list);
    list_clear(input_flag_list);
    list_clear(output_flag_list);
}


///////////////////////////////////////////////////////////////////////////////
//
// do_calls()
//
// print the nodes that the module calls
//
///////////////////////////////////////////////////////////////////////////////
void 
do_calls(int mod_id)
{
    int ix, count;
    list vars;
    node node_var;

    vars = GetCalls(mod_id);
    print_line("calls");
    for (ix = 0, count = list_count(vars); ix < count; ix = ix + 1)
    {
    	node_var = list_get(vars, ix);
	if (node_var.name != NULL) 
	    print_line("\t" + node_var.name);
    }
    print_line();
}


///////////////////////////////////////////////////////////////////////////////
//
// do_called_bys()
//
// Print the names of the nodes that call the given module
//
///////////////////////////////////////////////////////////////////////////////
void 
do_called_bys(int mod_id)
{
    int ix, count;
    list vars;
    node node_var;

    vars = GetCalledBys(mod_id);
    print_line("called by");
    for (ix = 0, count = list_count(vars); ix < count; ix = ix + 1)
    {
    	node_var = list_get(vars, ix);
	if (node_var.name != NULL)
    	    print_line("\t" + node_var.name);
    }
    print_line();
}


///////////////////////////////////////////////////////////////////////////////
//
// print_description()
//
// Print the module note description
//
///////////////////////////////////////////////////////////////////////////////
void 
print_description(int mod_id)
{
    string query;
    note pdl = NULL;

    query = "note[ModulePDL && obj_id == ${mod_id}]";
    pdl = find_by_query(query);
    if (pdl != NULL)
    {
	print(pdl.desc);
    }
}
