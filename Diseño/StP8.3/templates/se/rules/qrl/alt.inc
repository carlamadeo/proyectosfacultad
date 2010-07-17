#include "rules/qrl/cspec.inc"

string
ActionLogicTableExpression(list processes, list control_outs, list actions)
{
    string expression;

    expression = HsectExpression(ActionLogicTableVsectExpression(PrintRow(processes, "node"), "ProcessVsect")
    	    	    	    	+ ActionLogicTableVsectExpression(PrintRow(control_outs, "link"), "ALTControlOutVsect"));

    expression = expression 
    	    	 + HsectExpression(VsectExpression(PrintColumn(actions, "node", NULL)));

    return FillTableExpression(ACTION_LOGIC_TABLE, expression);
}

/*  Prepend a blank row to the expression */
string
ActionLogicTableVsectExpression(string expression, string vsect)
{
    expression = "{Row {Cell }} " + expression;
    return (VsectExpression("{Index " + vsect + "}" + expression));
}

