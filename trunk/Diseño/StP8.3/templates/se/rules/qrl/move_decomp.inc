#include "rules/qrl/dfe_stputil.inc"

//
// Executes move decomposition.  Does not confirm if force is True.
//
void 
MoveDecomp(string from, string to, boolean force)
{
    string force_cmd = "";
    string output = "", final = NULL;
    string editor;

    if (force)
    {
    	force_cmd = " -C 'force on'";
        SetStputil();
    	output = "> " + GetStputil();
    	final = "FinishUp";
    }

    editor = "dydfe";  // gsdfe would be OK also

    editor_system("stputil -p " + current_projdir() + 
    	    	  " -s " + current_system() + 
    	          " -C 'e " + editor + "'" + force_cmd + 
    	    	  " -C 'move " + from + " " + to + "'" + output,
    	          final);
}

int
FinishUp(int process_id, int status)
{
    PrintStputil();
    if (status != 0)
    {
    	gde_print_error("Move decomps failed.");
    	gde_print_message("Change Process Index failed.");
        RemoveStputil();
    	return 1;
    }
    // remove the temporary stputil file 
    RemoveStputil();
    return 0;
}
