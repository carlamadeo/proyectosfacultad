//
//      StP/UML
//      Confidential property of Aonix
//      Copyright (c) 1997
//      All rights reserved


//
// print a final error count  and message
//
void
error_message_and_count(string script_description, int errors)
{
    string m = "\n" + script_description + " completed with " + errors + 
	       " execution error";

    if (errors != 1)
	m = m + "s.";
    else
	m = m + ".";

    ide_warning(m);

    return;
}
