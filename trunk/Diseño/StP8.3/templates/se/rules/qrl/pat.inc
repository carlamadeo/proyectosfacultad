#include "rules/qrl/cspec.inc"

string
ProcessActivationTableExpression(list processes, list control_ins)
{
    string expression;

    expression = HsectExpression(ProcessActivationTableVsectExpression(PrintRow(control_ins, "link")) + ProcessActivationTableVsectExpression(PrintRow(processes, "node"))) + HsectExpression(VsectExpression(PrintControlRows(control_ins)));

    return FillTableExpression(PROCESS_ACTIVATION_TABLE, expression);
}

/*  Prepend a blank row to the expression */
string
ProcessActivationTableVsectExpression(string expression)
{
    expression = "{Row {Cell }} " + expression;
    return (VsectExpression(expression));
}

