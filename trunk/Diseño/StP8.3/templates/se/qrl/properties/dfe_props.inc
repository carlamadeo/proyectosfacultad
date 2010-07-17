//
//      StP/SE
//      Confidential property of Aonix
//      Copyright (c) 1995 - 1999
//      All rights reserved
//

int
OkDfeSemantics(ui_event_tp event, ui_prop prop, int id, string value)
{
    ApplyDfeSemantics(event, prop, id, value);
    return(1);
}

const string FILETYPE = "DfeDiagram";

int
ApplyDfeSemantics(ui_event_tp event, ui_prop prop, int id, string value)
{
    int	    balance, decomp, pspec, cspec, verbose;
    list    nodes = gde_all_mapped_oms_nodes();
    list    links = gde_all_mapped_oms_links();
    string  filename = gde_substitute_string("${file}");

    if (filename == NULL || filename == "")
    {
        print_error("Save the diagram before running Check Semantics.");
	return(0);
    }
    balance = ui_value(to_ui_prop("DfeSemantics", "CheckBalance"));
    decomp = ui_value(to_ui_prop("DfeSemantics", "CheckDecomp"));
    pspec = ui_value(to_ui_prop("DfeSemantics", "CheckPspec"));
    cspec = ui_value(to_ui_prop("DfeSemantics", "CheckCspec"));
    verbose = ui_value(to_ui_prop("DfeSemantics", "Verbose"));

    DfeCheckSemanticsOp(file_list(FILETYPE, filename), 
			nodes, 
			links, 
			balance, 
			decomp, 
			pspec, 
			cspec, 
			verbose);
    return(0);
}


// New Semantics checks
int
NewOkDfeSemantics(ui_event_tp event, ui_prop prop, int id, string value)
{
    NewApplyDfeSemantics(event, prop, id, value);
    return(1);
}


int
NewApplyDfeSemantics(ui_event_tp event, ui_prop prop, int id, string value)
{
    int	    store, decomp, process, split, pspec, cspec, verbose;
    list    nodes = gde_all_mapped_oms_nodes();
    list    links = gde_all_mapped_oms_links();
    string  filename = gde_substitute_string("${file}");

    if (filename == NULL || filename == "")
    {
        print_error("Save the diagram before running Check Semantics.");
	return(0);
    }
    decomp = ui_value(to_ui_prop("NewDfeSemantics", "DecompBalance"));
    store = ui_value(to_ui_prop("NewDfeSemantics", "StoreBalance"));
    process = ui_value(to_ui_prop("NewDfeSemantics", "ProcessBalance"));
    split = ui_value(to_ui_prop("NewDfeSemantics", "SplitFlowBalance"));
    pspec = ui_value(to_ui_prop("NewDfeSemantics", "CheckPspec"));
    cspec = ui_value(to_ui_prop("NewDfeSemantics", "CheckCspec"));
    verbose = ui_value(to_ui_prop("NewDfeSemantics", "Verbose"));

    DfeCheckSemanticsNow(file_list(FILETYPE, filename), store, decomp, pspec, cspec, process, split, verbose);
    return(0);
}

