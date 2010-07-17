
//      StP/SE
//      Confidential property of Aonix
//      Copyright (c) 1999
//      All rights reserved
//

// Define two variables that are used by the printing
boolean UseFormat = False;
string CurFile = "";

// defined here because it is shared between Ste and Cse.
int cseVerbose;

void 
print_sc_title()

{
    string fmt;

    if (UseFormat)
    {
        fmt = paragraph();
        paragraph("Title");
        print_line("StP/SE Semantic Check Report");
        print_line("Generated at " + time_to_string(time_now(), NULL) + " on " + hostname() + " by " + user() + ".");
        paragraph(fmt);
    }
}
void
print_sc_para(string msg)

{
    if (UseFormat)
    {
        paragraph();
        print(msg);
        paragraph();
    }
    return;
}




void
print_sc_error(string ed, string file, string msg)
{
    if (UseFormat)
    {
        if (ed == "Cse")
            print_line("Table '" + file + "': Error: " + msg);
        else
            print_line(ed + " Diagram '" + file + "': Error: " + msg);
    }
    return;
}


