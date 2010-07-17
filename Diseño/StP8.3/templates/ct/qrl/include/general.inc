//
//      sccsid[] = %W% %Y% %D%
//      StP/whatever
//      Confidential property of Interactive Development Environments, Inc.
//      Copyright (c) 1992-1993
//      All rights reserved
//
 
//
// This is the maximum number of heading levels allowed by Microsoft Word,
// which thus limits the number of levels available for use in scripts.
// Some scripts use this value to warn users of an attempt to use too
// many levels.
//
const int MAX_HEADING_LEVELS = 9;

 
//// general purpose routines

boolean str_has_val(string try) 
{
    if (try == NULL || try == "")
        return False;
    return True;
}
 
boolean str_no_val(string try)
{
    if (try == NULL || try == "")
        return True;
    return False;
}

