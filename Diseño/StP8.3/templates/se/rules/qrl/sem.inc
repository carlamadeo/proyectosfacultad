#include "rules/qrl/ste.inc"

/* for testing
external int id = 0;

void
main()
{
    print(StateEventMatrixHsectExpression(StateDiagramtoList(id)));
}
*/

string
StateEventMatrixExpression(list transitions)
{
    string expression;

    expression = StateEventMatrixHsectExpression(transitions);
    return FillTableExpression(STATE_EVENT_MATRIX, expression);
}


string
StateEventMatrixHsectExpression(list transitions)
{
    int ix, count;
    transition_info transition;	
    string event_expression = "", state_expression = "", 
    	   action_expression = "";
    list events = list_create("string", 0);
    list states = list_create("string", 0);

    for (ix = 0, count = list_count(transitions); ix < count; ix = ix + 1)
    {
    	transition = list_get(transitions, ix);
    	event_expression = event_expression + 
    	    StateEventMatrixEventRowExpression(events, transition);
    	state_expression = state_expression + 
    	    StateEventMatrixStateRowExpression(states, transition);
    }
    event_expression = "{Row {Cell }} {Row " + event_expression + "}";

    for (ix = 0, count = list_count(transitions); ix < count; ix = ix + 1)
    {
    	transition = list_get(transitions, ix);
    	action_expression = action_expression + 
    	    StateEventMatrixActionRowExpression(events, states, transition);
    }

    return("{Hsect {Index SEMEventHsect} {Vsect {Index SEMEventVsect} " +
	    event_expression + "}} {Hsect {Index SEMStateActionHsect}" + 
    	    "{Vsect {Index SEMStateVsect} " + state_expression + "}" + 
    	    "{Vsect {Index SEMActionVsect} " + action_expression + "}}");
}

string
StateEventMatrixEventRowExpression(list events, transition_info info)
{
    string expression = "";

    if (info.cur_state == "" 
    	|| list_find(events, 0, info.event) != list_count(events)
	|| info.event == "")
    	return expression;

    list_append(events, info.event);    
    expression = expression + "{Cell {Label \"" + escape_quotes(info.event) + "\"}}";
    return expression;
}    

string
StateEventMatrixStateRowExpression(list states, transition_info info)
{
    string expression = "";

    if (info.cur_state == "" 
    	|| list_find(states, 0, info.cur_state) != list_count(states))
    	return expression;

    list_append(states, info.cur_state);
    expression = expression + "{Cell {Label \"" + escape_quotes(info.cur_state) + "\"}}";
    return ("{Row " + expression + "}");
}    

string
StateEventMatrixActionRowExpression(list events, list states, transition_info info)
{
    string expression = "";
    int cell_index, row_index;

    if (info.cur_state == "")
    	return expression;

    /* table indexes are 1-based */
    cell_index = list_find(events, 0, info.event) + 1;
    row_index = list_find(states, 0, info.cur_state) + 1;
    expression = expression + "{Cell {Index " + cell_index + "} " + 
    	    	    	      	"{Label \"" + escape_quotes(info.action) + "\"}" + 
			      	"{Label2 \"" + escape_quotes(info.next_state) + "\"}}";
    return ("{Row {Index " + row_index + "}" + expression + "}");
}    
