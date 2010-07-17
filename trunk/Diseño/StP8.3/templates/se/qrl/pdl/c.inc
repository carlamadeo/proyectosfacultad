#include "se/qrl/pdl/pdl.inc"

/////////////////////////////////////////////////////////////////////////////////
// File: c.inc
//
// Prints a pdl for a given module in the C target lanuguage. The ouput
// will look like the output of the following R4.2D C pdl template.
//
//
//  # line 1 "${name}"
//  /****************************************************************
//   *
//   *  ${type} ${name}
//   *  was generated on ${date} at ${time} by ${user}@${hostname}
//   *
//   *  Called by:  ${@called}
//   *
//  ..LOOP callnum 1 ${#calls} 1
//   *      Calls:  ${calls[${callnum}]}
//  ..LEND
//   *
//   ***************************************************************/
//  /*module type*/  ${name} (${@param})
//
//  ..LOOP parmnum 1 ${#inflag} 1
//  ..DEFINE parmname ${inflag[${parmnum}]}
//  ..DEFINE parmtype ${parmname.el_type}
//  ..DEFINE parmcons ${parmname.el_cons}
//  /*  input flag ${parmnum} */  ${parmtype}  ${parmname} ${parmcons};
//  ..LEND
//
//  ..LOOP parmnum 1 ${#indata} 1
//  ..DEFINE parmname ${indata[${parmnum}]}
//  ..DEFINE parmtype ${parmname.el_type}
//  ..DEFINE parmcons ${parmname.el_cons}
//  /*  input data ${parmnum} */  ${parmtype}  ${parmname} ${parmcons};
//  ..LEND
//
//  ..LOOP parmnum 1 ${#outflag} 1
//  ..DEFINE parmname ${outflag[${parmnum}]}
//  ..DEFINE parmtype ${parmname.el_type}
//  ..DEFINE parmcons ${parmname.el_cons}
//  /* output flag ${parmnum} */  ${parmtype}  ${parmname} ${parmcons};
//  ..LEND
//
//  ..LOOP parmnum 1 ${#outdata} 1
//  ..DEFINE parmname ${outdata[${parmnum}]}
//  ..DEFINE parmtype ${parmname.el_type}
//  ..DEFINE parmcons ${parmname.el_cons}
//  /* output data ${parmnum} */  ${parmtype}  ${parmname} ${parmcons};
//  ..LEND
//
//      {
//      /* local variables */
//      /* logic */
//      }
//
///////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////
//
// do_c_pdl()
//
///////////////////////////////////////////////////////////////////////////////
void
do_c_pdl(int mod_id, string mod_name, string mod_type)
{
    do_c_parameter_stuff(mod_id);
    print_c_header(mod_type, mod_name);
    do_c_called_bys(mod_id);
    do_c_calls(mod_id);
    print_description(mod_id);
    print_line("*");
    print_line("****************************************************************/");
    do_c_signature_stuff(mod_id, mod_name);
    get_c_input_flags();
    get_c_input_data();
    get_c_output_flags();
    get_c_output_data();
    do_c_body_stuff();
}



///////////////////////////////////////////////////////////////////////////////
//
// print_header()
//
///////////////////////////////////////////////////////////////////////////////
void
print_c_header(string mod_type, string mod_name)
{
    print_line("# line 1 \"" + mod_name + "\"");
    print_line("/****************************************************************");
    print_line("*");
    print_line("* " + mod_type + " " + mod_name);
    print_line("* was generated at " + time_to_string(time_now(),NULL) + 
	    " by " + user() +"@"+hostname());
    print_line("*");
}


///////////////////////////////////////////////////////////////////////////////
//
// print_list()
//
// Prints the following list, with comma's separating each element.
//
///////////////////////////////////////////////////////////////////////////////
void
print_c_list(list param_list)
{
    int count = 0;
    int i;
    parameter temp_param;

    count = list_count(param_list);
    if (count > 0)
    {
	temp_param = list_get(param_list,0);
	print(temp_param.name);
    }
        
    for (i = 1; i < count; i = i + 1)
    {
	temp_param = list_get(param_list, i);
	print(", " + temp_param.name);
    }
}


///////////////////////////////////////////////////////////////////////////////
//
// do_parameter_stuff()
//
// Gets all the different types of parameters,
// puts them in different lists, and then prints them out
//
///////////////////////////////////////////////////////////////////////////////
void
do_c_parameter_stuff(int mod_id)
{
    cntx cntx_var;
    int count;
    parameter temp_param;

    query = "cntx[cntx_refs && " +
	    "link[to_node_id == ${mod_id} && from_node[FormalCaller]]]";

    count = selection_count(query);

    big_param_list = list_create("parameter",0);
    input_data_list = list_create("parameter",0);
    output_data_list = list_create("parameter",0);
    input_flag_list = list_create("parameter",0);
    output_flag_list = list_create("parameter",0);
    
    if (count != 0)
    {
	for_each_in_select(query, cntx_var)
	{
	    temp_param.name = cntx_var.name;
	    temp_param.id = cntx_var.id;
	    if (cntx_var.type == "DataIn")
	    {
		list_append(input_data_list,temp_param);
		list_append(big_param_list, temp_param);
	    }
	    else if (cntx_var.type == "DataOut")
	    {
		list_append(output_data_list,temp_param);
		list_append(big_param_list, temp_param);
	    }
	    else if (cntx_var.type == "ControlIn")
	    {
		list_append(input_flag_list,temp_param);
		list_append(big_param_list, temp_param);
	    }
	    else if (cntx_var.type == "ControlOut")
	    {
		list_append(output_flag_list,temp_param);
		list_append(big_param_list, temp_param);
	    }
	    else if (cntx_var.type == "DataInOut")
	    {
		list_append(input_data_list,temp_param);
		list_append(output_data_list,temp_param);
		list_append(big_param_list, temp_param);
	    }
	}
    }

}


///////////////////////////////////////////////////////////////////////////////
//
// do_c_calls()
//
// print the nodes that the module calls
//
///////////////////////////////////////////////////////////////////////////////
void
do_c_calls(int mod_id)
{
    int ix, count;
    list vars;
    node node_var;

    vars = GetCalls(mod_id);
    print_line("* Calls:");
    for (ix = 0, count = list_count(vars); ix < count; ix = ix + 1)
    {
    	node_var = list_get(vars, ix);
	if (node_var.name != NULL) 
    	    print_line("*\t" + node_var.name);
    }
    print_line("*");
}


///////////////////////////////////////////////////////////////////////////////
//
// do_c_called_bys()
//
// print the names of the nodes that call the module
//
///////////////////////////////////////////////////////////////////////////////
void do_c_called_bys(int mod_id)
{
    int ix, count;
    list vars;
    node node_var;

    vars = GetCalledBys(mod_id);
    print_line("* Called by:");
    for (ix = 0, count = list_count(vars); ix < count; ix = ix + 1)
    {
    	node_var = list_get(vars, ix);
	if (node_var.name != NULL)
    	    print_line("*\t" + node_var.name);
    }
    print_line("*");
}


///////////////////////////////////////////////////////////////////////////////
//
// do_c_signature_stuff()
//
///////////////////////////////////////////////////////////////////////////////
void
do_c_signature_stuff(int mod_id, string mod_name)
{
    print("/*module type*/ " + mod_name + " (");
    print_c_list(big_param_list);
    print_line(")");
    print_line();
}


///////////////////////////////////////////////////////////////////////////////
//
// get_c_input_data()
//
// Retrieves all input data parameters from the input_data_list and then 
// prints the parameter type followed by its name.
//
///////////////////////////////////////////////////////////////////////////////
void
get_c_input_data()
{
    int count, i;
    parameter temp_param;
    item item_var = NULL;

    count = list_count(input_data_list);

    for (i = 0; i < count; i = i + 1)
    {
	temp_param = list_get(input_data_list, i);
	query = "item[DataType && obj_id == " + temp_param.id + "]";
        item_var = find_by_query(query);
	if (item_var != NULL)
	{
	    print_line("/* input data " + i + " */ "  + item_var.value + 
		" " + temp_param.name + ";"); 
	}
	else
	{
	    print_line("/* input data " + i + " */ "  + temp_param.name + ";"); 
	}
    }
    if (count > 0) print_line();
}
    

///////////////////////////////////////////////////////////////////////////////
//
// get_c_input_flags()
//
// Retrieves all input parameter flags from the input_flag_list and then prints
// the parameter type followed by its name.
//
///////////////////////////////////////////////////////////////////////////////
void
get_c_input_flags()
{
    int count, i;
    parameter temp_param;
    item item_var = NULL;

    count = list_count(input_flag_list);

    for (i = 0; i < count; i = i + 1)
    {
	temp_param = list_get(input_flag_list, i);
	query = "item[DataType && obj_id == " + temp_param.id + "]";
        item_var = find_by_query(query);
	if (item_var != NULL)
	{
	    print_line("/* input flag " + i + " */ " + item_var.value + 
		" " + temp_param.name + ";"); 
	}
	else
   	{
	    print_line("/* input flag " + i + " */ " + temp_param.name + ";"); 
	} 
    }
    if (count > 0) print_line();
}


///////////////////////////////////////////////////////////////////////////////
//
// get_c_output_flags()
//
// Retrieves all output parameter flags from the output_flag_list and then 
// prints the parameter type followed by its name.
//
///////////////////////////////////////////////////////////////////////////////
void
get_c_output_flags()
{
    int count, i;
    parameter temp_param;
    item item_var = NULL;

    count = list_count(output_flag_list);

    for (i = 0; i < count; i = i + 1)
    {
	temp_param = list_get(output_flag_list, i);
	query = "item[DataType && obj_id == " + temp_param.id + "]";
        item_var = find_by_query(query);
	if (item_var != NULL)
	{
	    print_line("/* output flag " + i + " */ " + item_var.value + 
		" " + temp_param.name + ";"); 
	}
	else
	{
	    print_line("/* output flag " + i + " */ " + temp_param.name + ";"); 
	}
    }
    if (count > 0) print_line();
}


///////////////////////////////////////////////////////////////////////////////
//
// get_c_output_data()
//
// Retrieves all output data parameters from the output_data_list and then 
// prints the parameter type followed by its name.
//
///////////////////////////////////////////////////////////////////////////////
void
get_c_output_data()
{
    int count, i;
    parameter temp_param;
    item item_var = NULL;

    count = list_count(output_data_list);

    for (i = 0; i < count; i = i + 1)
    {
	temp_param = list_get(output_data_list, i);
	query = "item[DataType && obj_id == " + temp_param.id + "]";
        item_var = find_by_query(query);
	if (item_var != NULL)
	{
	    print_line("/* output data " + i + " */ " + item_var.value + 
		" " + temp_param.name + ";"); 
	}
	else
	{
	    print_line("/* output data " + i + " */ " + temp_param.name + ";"); 
	}
    
    
    }
    if (count > 0) print_line();
}


///////////////////////////////////////////////////////////////////////////////
//
// do_c_body_stuff()
//
///////////////////////////////////////////////////////////////////////////////
void
do_c_body_stuff()
{
    print_line("{");
    print_line("/* local variables */");
    print_line("/* logic */");
    print_line("}");
}



