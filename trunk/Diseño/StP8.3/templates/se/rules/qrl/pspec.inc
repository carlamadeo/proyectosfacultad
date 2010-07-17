
//      StP/SE
//      Confidential property of Aonix
//      Copyright (c) 1995 - 1999
//      All rights reserved
 

#include "rules/qrl/qrl_std.inc"
#include "qrl/include/files.inc"
#include "qrl/include/pindex.inc"
#include "qrl/include/dfe_defs.inc"
#include "qrl/include/decomp.inc"


// This is the duplicate of GetDfeFileStoreDir() in dfe.qrl. This is copied
// to here and name of the function is changed to avoid unnecessary include error.
string
GetDfeFileStoredir()
{
    return path_compose(current_projdir(), current_system(), "dfe_files");
}


int
idehook_gen_pspec_active_func()
{
    list symbols = gde_selected_symbols();
    
    // No symbols selected; gen pspec for all processes which need it
    if (list_count(symbols) == 0)
    	return ACTIVE_FUNC_IS_ACTIVE;

    // Only Processes selected
    if (list_count(symbols) == 
    	list_count(gde_symbols_find_with_types(symbols, 
    	    	    	    	    	      string_to_list(PROCESS, ","))))
    	return ACTIVE_FUNC_IS_ACTIVE;

    return ACTIVE_FUNC_IS_INACTIVE;
}




// Generate a Pspec for all selected processes which have a Pspec note.
// This function is virtually identical to GenerateSelectedPdls, any
// changes need to be made there as well.

void
GenerateSelectedPspecs()

{
    list processes = gde_selected_symbols();
    int i;
    list nodel = list_create("node", 0);
    node n;
    gde_node gn;
    string fname;
    file f;

    fname = gde_diagram_name();

    if (str_no_val(fname))
    {
        print_error("Diagram must have a name before generating PSpecs.");
        return;
    }

    f = find_by_query("file[DfeDiagram && name == '${fname}']");
    if (f == NULL)
    {
        print_error("File '" + fname + "' not found in repository. Save without syntax errors first.");
        return;
    }

    editor_set_is_busy(True);
    // Get process node objects for symbols
    if (list_count(processes) == 0)
    {
        nodel = list_select("node[Process && node_refs[file_id = ${f.id}] && notes[Pspec]]");
        if (list_count(nodel) == 0)
        {
            print_error("No Processes in this diagram have Pspec notes. No pspecs generated.");
            editor_set_is_busy(False);
            return;
        }
    }
    else
    {
        for (i=0; i<list_count(processes); i++)
        {
	    gn = to_gde_node(list_get(processes, i));
            n = get_proper_oms_node(gn);
            if (find_by_query("note[Pspec && obj_id = ${n.id}]") == NULL)
                print_error("Process '" + n.name + "' does not have a Pspec note. Use Edit Pspec Note first.");
            else
    	        list_append(nodel, n);
        }
    }

    for (i=0; i<list_count(nodel); i++)
    {
        n = list_get(nodel, i);
    	generate_pspec(fname, n, "", True);
    }
    print_message("Generate Pspec(s) complete.");
    editor_set_is_busy(False);
    return;
}





//
// Print out a process specification for the given Process node.
//
void 
generate_pspec(string filename, node process, string output_dir, 
	       boolean called_from_editor)
{
    string outfile, command, msg, procname;
    item pindex = get_process_index(process);

    if (output_dir == "")
        output_dir = GetDfeFileStoredir();

    procname = string_search_and_replace(FindDecompositionName(filename, to_int(pindex.value)), " ", "_");
    outfile = path_compose(output_dir, procname + ".pspec");

    if (file_exists(outfile) && (! write_file_access(outfile)))
    {
	print_error("Cannot write Pspec file '" + outfile + "'.");
	return;
    }
    print_message("Generating Pspec for '" + process.name + "' in '" + outfile + "'.");

    write_file(outfile, generate_pspec_primitive_diag(procname, process.name, filename, process.id));

    return;
}

//    Fix for SPR 2414
//    Find the Pspec annotation associated with the give process 

void
OLDFindOrCreatePspecNote(string filename )
{
    gde_node gn;
    node n;
    int handle;
    string nname;
    file returned_oms_query;
//    node process;
    item pindex; 
//    gde_node process_gde_node;
//    list cur_process_nodes;
//    gde_symbol process_sym;

    //if (list_count(processes) > 1)
    //{
//	print_error("Pepec can't be created more than one.");
 //       return;
    //}

    // The following commented codes are supposed to do same thing as
    // below. But somehow last two lines don't seem to work as I expected.

    //cur_process_nodes = list_create("node",0);
    //process_gde_node = to_gde_node(list_get(gde_selected_symbols(), 0));
    //cur_process_nodes = gde_node_oms_nodes(process_gde_node);
    //process = list_get(cur_process_nodes(), 0);
    

 
    // find the current object, and it's oms node

    gn = to_gde_node(list_get(gde_selected_symbols(), 0));
    //if (gde_nt_name(gde_node_nt(gn)) == "Module")
    //	nname = "NoteSelect ModulePDL";
    //else
	nname = "NoteSelect Pspec";
    n = get_proper_oms_node(gn);
    
    if (n == NULL)
    {
	print_error("Database object for current selection not found.  Nothing to edit.");
	return;
    }

    // check for the decomp digram exist
    // Selected object will be process.

    pindex = get_process_index(n);   // the process index from the rule file is not returning 
			            //	correct value after decomp digram is created. Hence I must
				    // get the pid this way. 
    
    returned_oms_query = FindDecomposition(filename, to_int(pindex.value));

    if (returned_oms_query != NULL)
    {
       print_error("Process already has a decomposition diagram. Creating Pspec failed.");
       return;
    } 


    handle = editor_send_msg(OAE_CLASS, OAE_CMD_ANNOT_EDIT + n.id, CH_FIRST);
    editor_send_msg(OAE_CLASS, nname, handle);
    editor_send_msg(OAE_CLASS, "NoteEditDesc", handle);

    return;
}


// This is taken from qrl/include/pspec_primitive.inc, but handles it's own output. 

string
generate_pspec_primitive_diag(string process_index, string process_name,
                                         string file_name, int process_id)
{
    string          query, outs;
    note            pspec;
    
    outs = "\nProcess " + process_index + ": " + process_name + "\n";
 
    outs += "Pspec generated from data flow diagram \"" + file_name + "\" of the \"" +
            current_system() + "\" system of the\n\"" +
            current_projdir() + "\" projdir at " + time_to_string(time_now(), NULL) +
            " by\n" + user() + "@" + hostname() + "\n";
 
    // Data Flows
    outs += "\n" + print_flows_diag(process_id, "DataFlow", "data");
 
    // Control Flows
    outs += "\n" + print_flows_diag(process_id, "ControlFlow", "control");
 
    // Description
    outs += "\ndescription\n";
    query = "note[Pspec && obj_id == ${process_id}]";
    pspec = find_by_query(query);
    if (pspec != NULL)
    {
        outs += "\n" + pspec.desc + "\n";
    }
    outs += "end pspec\n";

    return outs;
}


//
// Print all flows of type flowtype which are connected to 
//
string
print_flows_diag(int id, string flowtype, string flow)
{
    string query, outs = "";   	    
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

    outs += "\nThis process has " + num_flows + " " + flow + " flow";
    if (num_flows != 1)
    	outs += "s";
    if (num_flows == 0)
	return outs + "\n";

    outs += ":\n";
    outs += print_list_diag(all_flows);

    // Input flows
    outs += "\ninput " + flow + " flows\n";
    query = "link[link_refs && to_node_id == ${id} && ${flowtype}]";
    flows = list_select(query);
    outs += print_list_with_type_diag(flows);

    // Output flows
    outs += "\noutput " + flow + " flows\n";
    query = "link[link_refs && from_node_id == ${id} && ${flowtype}]";
    flows = list_select(query);
    outs += print_list_with_type_diag(flows);

    return outs;
}


//
// print a comma-separated list of all flows in the set
//
string
print_list_diag(list flows)
{
    int ix, count;
    link flow;
    string outs = "";

    // don't print a comma after the last flow
    for (ix = 0, count = list_count(flows); ix < count; ix = ix + 1)
    {
    	if (ix > 0) 
    	    outs += ",\n";
    	flow = list_get(flows, ix);
    	outs += flow.name;    
    	if (ix == count - 1)
    	    outs += "\n";
    }

    return outs;
}

//
//  print a list of flow names and types, one to a line.
//
string
print_list_with_type_diag(list flows)
{
    int ix, count;
    link flow;
    string outs = "";

    for (ix = 0, count = list_count(flows); ix < count; ix = ix + 1)
    {
    	flow = list_get(flows, ix);
    	outs += flow.name + " : " + flow.type + "\n";
    }

    return outs;
}
