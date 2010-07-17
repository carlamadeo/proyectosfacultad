#include "rules/qrl/qrl_std.inc"

const string STATE_TRANSITION_TABLE = "StateTransitionTable";
const string STATE_EVENT_MATRIX = "StateEventMatrix";
const string DECISION_TABLE = "DecisionTable";
const string EVENT_LOGIC_TABLE = "EventLogicTable";
const string ACTION_LOGIC_TABLE = "ActionLogicTable";
const string PROCESS_ACTIVATION_TABLE = "ProcessActivationTable";
const string PROCESS_ACTIVATION_MATRIX = "ProcessActivationMatrix";

const string EVENT = "Event";
const string ACTION = "Action";
const string STATE = "State";
const string CONTROL_IN = "ControlIn";
const string CONTROL_OUT = "ControlOut";
const string CONTROL_VALUE = "ControlValue";

list CSE_TABLE_TYPES = string_to_list(STATE_TRANSITION_TABLE + "," +
				      STATE_EVENT_MATRIX  + "," +
				      DECISION_TABLE + "," + 
				      EVENT_LOGIC_TABLE + "," + 
			 	      ACTION_LOGIC_TABLE + "," + 
				      PROCESS_ACTIVATION_TABLE + "," + 
				      PROCESS_ACTIVATION_MATRIX, ",");


/*
    Returns a list of the Control Flows flowing into the Cspec Bar.
*/
list
CspecControlIns (int cspec_id)
{
    string query;

    query = "link[ControlFlow && link_refs && to_node_id == ${cspec_id}]";
    return list_select(query);
}

/*
    Returns a list of the Control Flows flowing out of the Cspec Bar.
*/
list
CspecControlOuts (int cspec_id)
{
    string query;

    query = "link[ControlFlow && link_refs && from_node_id == ${cspec_id}]";
    return list_select(query);
}

/*
    Returns a list of names of the Control Flows flowing into the Cspec Bar.
*/
list
CspecControlInsByName (string cspec_name)
{
    string query;
    link the_link;
    list result = list_create("string", 0);

    query = "link[ControlFlow && link_refs && to_node[name == '${cspec_name}']]";
    for_each_in_select(query, the_link)
    	list_append(result, the_link.name);
    return result;
}

/*
    Returns a list of names of the Control Flows flowing into the Cspec Bar.
*/
list
CspecControlOutsByName (string cspec_name)
{
    string query;
    link the_link;
    list result = list_create("string", 0);

    query = "link[ControlFlow && link_refs && from_node[name == '${cspec_name}']]";
    for_each_in_select(query, the_link)
    	list_append(result, the_link.name);
    return result;
}

/*
    Returns a list of the Processes in the diagram with the Cspec.
*/
list
CspecProcesses (string cspec_name)
{
    string query;

    query = "node[Process && node_refs[file[DfeDiagram && name == '${cspec_name}']]]";
    return list_select(query);
}

/*
    Returns a list of the names of the Processes in the diagram with the Cspec.
*/
list
CspecProcessesByName (string cspec_name)
{
    string query;
    node the_node;
    list result = list_create("string", 0);

    query = "node[Process && node_refs[file[DfeDiagram && name == '${cspec_name}']]]";
    for_each_in_select(query, the_node)
    	list_append(result, the_node.name);
    return result;
}

/*
    Returns a list of the indexes of the Processes in the diagram with the Cspec.
*/
list
CspecProcessesByIndex (string cspec_name)
{
    string query;
    item the_item;
    list result = list_create("string", 0);

    query = "node[Process && node_refs[file[DfeDiagram && name == '${cspec_name}']]]";
    id_list_create(query, "process_nodes");

    query = "item[RelativeIndex && node[process_nodes]]";
    for_each_in_select(query, the_item)
    	list_append(result, FindDecompositionName(cspec_name, the_item.value));
    id_list_free("process_nodes");

    return result;
}

/*
    Returns a list of the nodes of the given type in the STE diagram which is 
    analogous to the DFE digram with the Cspec.
*/
list
SteDiagramNodes (string cspec_name, string type)
{
    string query;

    query = "node[${type} && node_refs[file[SteDiagram && name == '${cspec_name}']]]";
    return list_select(query);
}

/*
    Returns a list of the nodes of the given type in the STE diagram which is 
    analogous to the DFE digram with the Cspec.
*/
list
SteDiagramNodesByName (string cspec_name, string type)
{
    string query;   
    node the_node;
    list result = list_create("string", 0);

    query = "node[${type} && node_refs[file[SteDiagram && name == '${cspec_name}']]]";
    for_each_in_select(query, the_node)
    	list_append(result, the_node.name);
    return result;
}

/*
    Returns a list of the Actions.
*/
list
CspecActions (string cspec_name)
{
    return SteDiagramNodes(cspec_name, ACTION);
}

/*
    Returns a list of the Events in the STE diagram which is analogous to
    the DFE digram with the Cspec.
*/
list
CspecEvents (string cspec_name)
{
    return SteDiagramNodes(cspec_name, EVENT);
}

/*
    Returns a list of the names of the Events in the STE diagram 
    which is analogous to the DFE digram with the Cspec.
*/
list
CspecEventsByName (string cspec_name)
{
    return SteDiagramNodesByName(cspec_name, EVENT);
}

/*
    Returns a list of the names of the Actions in the STE diagram 
    which is analogous to the DFE digram with the Cspec.
*/
list
CspecActionsByName (string cspec_name)
{
    return SteDiagramNodesByName(cspec_name, ACTION);
}

/*
    Returns a list of the names of the States in the STE diagram 
    which is analogous to the DFE digram with the Cspec.
*/
list
CspecStatesByName (string cspec_name)
{
    return SteDiagramNodesByName(cspec_name, STATE);
}



//  Returns a list of the items which are Allowed Values of the Dse Structure
//  with the given name and qualification.

list 
CspecControlValues(string name, string qual, boolean verbose)
{
    list objl, iteml; 
    node obj;
    node_ref obj_ref;

    if (qual == "")
    {
	objl = list_select("node[node_refs[file[DseDiagram]] && (Sequence || Selection) && name == '" + to_oms_string(name) + "']");
	if (list_count(objl) > 1)
	{
	    if (verbose)
		print_message("Control `" + name + "' is not unique. Qualify it first.");
	    return list_create("item", 0);
	}
	else if (list_count(objl) == 0)
	{
	    if (verbose)
	        print_message("No DSE objects for Control `" + name + "' found.");
	    return list_create("item", 0);
	}
	else
	{
	    obj = list_get(objl, 0);
	    iteml = list_select("item[AllowedValue && note[DataDefinition] && obj_id = ${obj.id}]");
	    if (verbose && (list_count(iteml) == 0))
		print_message("No Allowed Value items found for `" + name + "'.");

	    return iteml;
	}
    }
    else
    {
	objl = find_qualified_object(qual);
	if (list_count(objl) == 0)
	{
	    if (verbose)
	        print_message("No DSE objects for qualified Control `" + name + "' found.");
	    return list_create("item", 0);
	}
	else
	{
	    obj_ref = list_get(objl, 0);
            iteml = list_select("item[AllowedValue && note[DataDefinition] && obj_id = ${obj_ref.node_id}]");
	    if (verbose && (list_count(iteml) == 0))
		print_message("No Allowed Value items found for `" + name + "'.");

	    return iteml;
        }
    }
}



//  Returns a list of the values of the items which are Allowed Values 
//  of the Dse Structure with the given name and qualification.

list 
CspecControlValueStrings(string name, string qual)
{
    item itm;
    list result = list_create("string", 0);
    int i, icnt;
    list l;

    l = CspecControlValues(name, qual, False);

    for (i = 0, icnt = list_count(l); i < icnt; i++)
    {
	itm = list_get(l, i);
	if (itm.value != NULL && itm.value != "")
	    list_append(result, itm.value);
    }
    
    return result;
}




string 
FillTableExpression(string type, string expression)
{
    return("FillTable " + type + "{" + expression + "}");
}

string
HsectExpression(string expression)
{
    return("{Hsect " + expression + "}");
}

string
VsectExpression(string expression)
{
    return("{Vsect " + expression + "}");
}

string
PrintRow (list elements, string type)
{
    if (type == "node")
    	return PrintNodeRow(elements);    	    
    if (type == "link")
    	return PrintLinkRow(elements);    	    
    return NULL;
}

string
PrintLinkRow (list elements)
{
    int ix, count;
    string expression = "";
    link cur_elem;

    for (ix = 0, count = list_count(elements); ix < count; ix = ix + 1)
    {
    	cur_elem = list_get(elements, ix);
    	expression = expression + "{Cell {Label \"" + escape_quotes(cur_elem.name) + "\"}}";
    }
    return "{Row " + expression + "}";
}

string
PrintNodeRow (list elements)
{
    int ix, count;
    string expression = "";
    node cur_elem;

    for (ix = 0, count = list_count(elements); ix < count; ix = ix + 1)
    {
    	cur_elem = list_get(elements, ix);
    	expression = expression + "{Cell {Label \"" + escape_quotes(cur_elem.name) + "\"}}";
    }
    return "{Row " + expression + "}";
}

string 
PrintControlRows (list elements)
{
    int ix, count;
    string expression = "";
    link cur_elem;
    list values;

    /* Now print each Control Value */
    for (ix = 0, count = list_count(elements); ix < count; ix = ix + 1)
    {
    	cur_elem = list_get(elements, ix);
    	values = CspecControlValues(cur_elem.name, "", False);
    	expression = expression + PrintColumn(values, "item", ix + 1);
    }
    return expression;
}


string
PrintColumn (list elements, string type, int rowindex)
{
    if (rowindex != NULL && rowindex < 0)
    {
    	message("PrintColumn: rowindex has negative value " + rowindex);
    	rowindex = NULL;
    }

    if (type == "node")
    	return PrintNodeColumn(elements, rowindex);    	    
    if (type == "link")
    	return PrintLinkColumn(elements, rowindex);    	    
    if (type == "item")
    	return PrintItemColumn(elements, rowindex);    	    
    return NULL;
}

string
PrintLinkColumn (list elements, int rowindex)
{
    int ix, count;
    string expression = "";
    link cur_elem;

    for (ix = 0, count = list_count(elements); ix < count; ix = ix + 1)
    {
    	cur_elem = list_get(elements, ix);
    	expression = expression + "{Row {Cell";
	if (rowindex != NULL)
	    expression = expression + "{Index " + rowindex + "}";
	expression = expression + "{Label \"" + escape_quotes(cur_elem.name) + "\"}}}";
    }
    return expression;
}

string
PrintNodeColumn (list elements, int rowindex)
{
    int ix, count;
    string expression = "";
    node cur_elem;

    for (ix = 0, count = list_count(elements); ix < count; ix = ix + 1)
    {
    	cur_elem = list_get(elements, ix);
    	expression = expression + "{Row {Cell";
	if (rowindex != NULL)
	    expression = expression + "{Index " + rowindex + "}";
	expression = expression + "{Label \"" + escape_quotes(cur_elem.name) + "\"}}}";
    }
    return expression;
}

string
PrintItemColumn (list elements, int rowindex)
{
    int ix, count;
    string expression = "";
    item cur_elem;

    for (ix = 0, count = list_count(elements); ix < count; ix = ix + 1)
    {
    	cur_elem = list_get(elements, ix);
    	expression = expression + "{Row {Cell";
	if (rowindex != NULL)
	    expression = expression + "{Index " + rowindex + "}";
	expression = expression + "{Label \"" + escape_quotes(cur_elem.value) + "\"}}}";
    }
    return expression;
}



