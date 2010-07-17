#include "rules/qrl/cspec.inc"

string
EventLogicTableExpression(list control_ins, list events)
{
    string expression;

    expression = HsectExpression(EventLogicTableVsectExpression(PrintRow(control_ins, "link")) + EventLogicTableVsectExpression(PrintRow(events, "node"))) + HsectExpression(VsectExpression(PrintControlRows(control_ins)));

    return FillTableExpression(EVENT_LOGIC_TABLE, expression);
}

/*  Prepend a blank row to the expression */
string
EventLogicTableVsectExpression(string expression)
{
    expression = "{Row {Cell }} " + expression;
    return (VsectExpression(expression));
}

