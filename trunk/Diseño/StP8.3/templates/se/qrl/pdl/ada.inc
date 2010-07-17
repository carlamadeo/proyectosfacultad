#include "se/qrl/pdl/pdl.inc"

// ADA.INC
//
// Prints a pdl for a given module in the ada target lanuguage. The ouput
// will look like the output of the following R4.2D ada pdl template.
//
// ----------------------------------------------------------------
// --
// --  ${name} was generated on ${date} at ${time}
// --  by ${user}@${hostname}
// --
// --  Contained in: ${@called}
// --
// --  Contains:  ${calls[1]}
// --             ${calls[2]}
// --             ${calls[3]}
// --             ${calls[4]}
// --             ${calls[5]}
// --
// ----------------------------------------------------------------
//
// with ${@calls};
//
// package body  ${name} is
//
// end ${name};
//
///////////////////////////////////////////////////////////////////////////////

list calls_list;

///////////////////////////////////////////////////////////////////////////////
//
// do_ada_pdl()
//
///////////////////////////////////////////////////////////////////////////////
void
do_ada_pdl(int mod_id, string mod_name)
{
    print_ada_header(mod_name);
    print_description(mod_id);
    do_ada_called_bys(mod_id);
    do_ada_calls(mod_id);
    print_line("--");
    print_line("----------------------------------------------------------------");
    do_with_stmt();
    do_package_stmt(mod_name);
}

///////////////////////////////////////////////////////////////////////////////
//
// print_ada_header()
//
///////////////////////////////////////////////////////////////////////////////
void
print_ada_header(string mod_name)
{
    print_line("# line 1 \"" + mod_name + "\"");
    print_line("-------------------------------------------------------------");
    print_line("--");
    print_line("-- " + mod_name + " was generated at " + 
	    time_to_string(time_now(),NULL)); 
    print_line("-- by " + user() +"@" + hostname());
    print_line("--");
}

///////////////////////////////////////////////////////////////////////////////
//
// print_ada_list()
//
// Prints the given list, with comma's separating each element.
//
///////////////////////////////////////////////////////////////////////////////
void
print_ada_list(list param_list)
{
    int count, i;

    count = list_count(param_list);
    if (count >= 0)
    {
	print(list_get(param_list,0));
    }
        
    for (i = 1; i < count; i = i + 1)
    {
	print(", " + list_get(param_list, i));
    }
}


///////////////////////////////////////////////////////////////////////////////
//
// do_ada_calls()
//
// Print the nodes that the given module calls
//
///////////////////////////////////////////////////////////////////////////////
void
do_ada_calls(int mod_id)
{
    int ix, count;
    list vars;
    node node_var;

    calls_list = list_create("string",0);

    vars = GetCalls(mod_id);
    print_line("-- Contains:");
    for (ix = 0, count = list_count(vars); ix < count; ix = ix + 1)
    {
    	node_var = list_get(vars, ix);
	if (node_var.name != NULL) 
	{
	    print_line("--\t" + node_var.name);
	    list_append(calls_list, node_var.name);
	}
    }
    print_line("--");
}


///////////////////////////////////////////////////////////////////////////////
//
// do_ada_called_bys()
//
// Prints the names of the nodes that call the given module.
//
///////////////////////////////////////////////////////////////////////////////
void
do_ada_called_bys(int mod_id)
{
    int ix, count;
    list vars;
    node node_var;

    vars = GetCalledBys(mod_id);
    print_line("-- Contained in:");
    for (ix = 0, count = list_count(vars); ix < count; ix = ix + 1)
    {
    	node_var = list_get(vars, ix);
	if (node_var.name != NULL)
    	    print_line("--\t" + node_var.name);
    }
    print_line("--");
}


///////////////////////////////////////////////////////////////////////////////
//
// do_with_stmt()
//
///////////////////////////////////////////////////////////////////////////////
void
do_with_stmt()
{
    int count;

    count = list_count(calls_list);
    if (count > 0)
    {
	print("with ");
	print_ada_list(calls_list);
	print_line(";");
	print_line();
    }
}


///////////////////////////////////////////////////////////////////////////////
//
// do_package_stmt()
//
///////////////////////////////////////////////////////////////////////////////
void
do_package_stmt(string mod_name)
{
    print_line("package body " + mod_name + " is");
    print_line("\nend " + mod_name + ";");

}




