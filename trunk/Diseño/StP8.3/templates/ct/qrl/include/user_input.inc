//////////////////////////////////////////////////////////////////////////////
//
//	StP Core
//      Confidential property of Interactive Development Environments, Inc.
//      Copyright (c) 1994
//      All rights reserved
//
//////////////////////////////////////////////////////////////////////////////

// 
// Handle user input

void
user_input_show_prompt(string prompt)
{
    message(prompt, False);
}

string
user_input(string prompt)
{
    return read_line_from_user();
}

boolean
get_user_input_yesorno(string p, boolean def)
{
    string 	v;

    v = user_input(p);
    if (def == True)
	v = v + "y";
    else
	v = v + "n";
    v = string_extract(to_lower(v), 0, 1);
    if (v == "y" || v == "j" || v == "1" || v == "t")
	return(True);
    return(False);
}

int
get_user_input_enum(string p, string valid, int def)
{
    string 	v;
    int		rc, i;
    string	inv = "";

    for(i = 0;  i < 10; i = i + 1)
    {
	v = user_input(p);
	if (v == "")
	    return(def);
	valid = to_lower(valid);
	v = to_lower(v);
	rc = string_find(valid, 0, string_extract(v, 0, 1));
	if (rc < string_length(valid))
	    return(rc);
	message("Invalid user input -- valid values are: " + valid);
    }
    return(-1);
}

void
show_meter(int c, int incr)
{
    int		l;
    string	s;

    if (c != 0)
    {
	l = 1 + c/incr;
	s = string_repeat("-", l);
	message("\t" + s, True);
	message("\t", False);
    }
}

void
update_meter()
{
    message("+", False);
}


void
finish_meter()
{
    message(" ", True);
}

