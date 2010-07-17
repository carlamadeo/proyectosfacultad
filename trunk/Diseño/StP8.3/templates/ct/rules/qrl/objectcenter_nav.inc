//
//      sccsid[] = %W% %Y% %D%
//      StP/OMT
//      StP/Booch
//      Confidential property of Interactive Development Environments, Inc.
//      Copyright (c) 1995
//      All rights reserved
//
 
 
//////////////////////////////////////////////////////////////////////////////
//
//
//    File: objectcenter_nav.inc
//
//    Description:
//
//    Routines to browse from programming environments to StP editors
//
//////////////////////////////////////////////////////////////////////////////
 


// Don't include is _loaded is False
// #include "rules/qrl/objectcenter_nav.impl"

boolean         objc_nav_loaded = False;
string          objc_nav_qrl_file = "rules/qrl/objectcenter_nav.impl";


int 
OMTFromProgrammingEnvironment(string editor, string class, string member)
{
    if (objc_nav_loaded == False)
    {
	objc_nav_loaded = qrl_addcode(objc_nav_qrl_file);
    }
    return Impl_OMTFromProgrammingEnvironment(editor, class, member);
}


void 
BoochFromProgrammingEnvironment(string editor, string class, string member)
{
    if (objc_nav_loaded == False)
    {
	objc_nav_loaded = qrl_addcode(objc_nav_qrl_file);
    }
    Impl_OMTFromProgrammingEnvironment(editor, class, member);
}

