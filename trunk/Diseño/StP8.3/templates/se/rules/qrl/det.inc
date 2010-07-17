#include "rules/qrl/cspec.inc"

/* for testing 
external int id = 0;

void
main()
{
    print(DecisionTableExpression(CspecControlIns(id), CspecControlOuts(id)));
}
*/

string
DecisionTableExpression (list control_ins, list control_outs)
{
    string expression;

    expression =  HsectExpression(DecisionTableVsectExpression(PrintRow(control_ins, "link")) + DecisionTableVsectExpression(PrintRow(control_outs, "link"))) + HsectExpression(VsectExpression(PrintControlRows(control_ins)));

    return FillTableExpression(DECISION_TABLE, expression);
}

/*  Prepend a blank row to the expression */
string
DecisionTableVsectExpression (string expression)
{
    expression = "{Row {Cell }} " + expression;
    return (VsectExpression(expression));
}

