//
//      sccsid[] = %W% %Y% %D%
//      StP/T
//      Confidential property of Interactive Development Environments, Inc.
//      Copyright (c) 1994
//      All rights reserved
//
// basic_textract_dt.inc: basic datatypes the textraction facilities use.
/*
 * Public Functions:
 *	clear_td
 *
 * Structure Definitions: 
 *      constant_tp
 *	master_tp
 *	module_tp
 *      object_tp
 *	process_tp
 *      rt_state_tp   
 *	state_tp
 *	type_detail_tp	
*/

// Data types for database information

struct object_tp
{
    // for classes
    node class;
    list attributes;
    list operations;
};



struct module_tp
{
    link module;   // the subprogram call to the formal
    list params;
    list globals_used;
    list globals_produced;
};



struct state_tp
{
    // for states
    node super; 
    node start; 
    node stop;
    list states;
};



struct process_tp
{
    node process;
    string fname;
    list flows_in;
    list flows_out;
};



struct master_tp
{
    list objects;
    list modules; 
    list states;
    list processes;
    list control_bar_diags;
};


 
struct type_detail_tp
{
    node from_node;
    link from_link;
    string name;
    string scope;
    cntx from_cntx;
    int annot_id;
};
 

type_detail_tp 
clear_td()

{
    type_detail_tp t;

    t.from_node = NULL;
    t.from_link = NULL;
    t.name = t.scope = NULL;
    t.from_cntx = NULL;
    t.annot_id = -1;

    return t;

} // end of clear_td()


struct constant_tp
{
    int id;
    string name;
    string value;
};


struct rt_state_tp
{
    string name;
    list allows;
};
