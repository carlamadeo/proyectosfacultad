#include "rules/qrl/cspec.inc"

string
ProcessActivationMatrixExpression(list processes, list control_ins)
{
    string expression;

    expression = HsectExpression(ProcessActivationMatrixVsectExpression(PrintRow(processes, "node")))
	 + ProcessActivationMatrixHsectExpression(PrintColumn(control_ins, "link", NULL));

    return FillTableExpression(PROCESS_ACTIVATION_MATRIX, expression);
}

string
ProcessActivationMatrixHsectExpression(string expression)
{
    return("{Hsect {Index ControlHsect}" + VsectExpression(expression) + "}");
}

/*  Prepend a blank row to the expression */
string
ProcessActivationMatrixVsectExpression(string expression)
{
    expression = "{Row {Cell }} " + expression;
    return ("{Vsect {Index ProcessVsect}" + expression + "}");
}

