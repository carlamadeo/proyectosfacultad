#include "rules/qrl/ste.inc"

string
StateTransitionTableExpression(list transitions)
{
    string expression;

    expression = StateTransitionTableHsectExpression(transitions);
    return FillTableExpression(STATE_TRANSITION_TABLE, expression);
}


string
StateTransitionTableHsectExpression(list transitions)
{
    int ix, count;
    string expression = "";

    for (ix = 0, count = list_count(transitions); ix < count; ix = ix + 1)
    {
    	expression = expression + 
    	    StateTransitionTableRowExpression(list_get(transitions, ix));
    }
    return ("{Hsect {Index STTHsect} {Vsect {Index STTVsect} " + 
   	    expression + "}}");
}

string
StateTransitionTableRowExpression(transition_info info)
{
    string expression = "";

    if (info.cur_state == "")
    	return expression;

    expression = expression + "{Cell {Label \"" + escape_quotes(info.cur_state) + "\"}}";
    expression = expression + "{Cell {Label \"" + escape_quotes(info.event) + "\"}}";
    expression = expression + "{Cell {Label \"" + escape_quotes(info.action) + "\"}}";
    expression = expression + "{Cell {Label \"" + escape_quotes(info.next_state) + "\"}}";

    return ("{Row " + expression + "}");
}    
