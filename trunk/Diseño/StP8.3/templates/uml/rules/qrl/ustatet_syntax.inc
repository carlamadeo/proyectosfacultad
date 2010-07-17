//      StP/UML
//      Confidential property of Aonix
//      Copyright (c) 1997
//      All rights reserved


// SYNTAX CHECKING FOR USTATET

boolean
UStateTCheckSyntax()
{
    int errors = 0;

// ECR 5580 begin
    int ie_hsect, event_col, action_col;
    int row1, row2, k;
    string event_name, action_name;

    ie_hsect = gte_lookup_symbol("UmlInternalActionHsect");
    event_col = gte_lookup_symbol("UmlInternalEventCol");
    action_col = gte_lookup_symbol("UmlInternalActionCol");

    row1 = gte_get_first_row(ie_hsect);
    row2 = gte_get_last_row(ie_hsect);
    for (k = row1; k <= row2; k++)
    {
        event_name = gte_get_cell_label(k, event_col);
        action_name = gte_get_cell_label(k, action_col);

        if ("" == action_name && "" != event_name)
        {
            print_error("Internal Event '" + event_name + "' without Internal Action is not mapped into the repository");
            errors++;
        }
    }
// ECR 5580 end

    return (errors == 0);
}
