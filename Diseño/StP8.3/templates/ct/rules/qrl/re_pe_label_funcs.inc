//      Confidential property of Interactive Development Environments, Inc.
//      Copyright (c) 1992-1997
//      All rights reserved
//
//
//      routines that build up the labels for Reverse Engineering and
//      other third party Programming Environment integrations.
//


string re_pe_PE = NULL;

// this function finds out the programming environment - it will only
// work it out once, since it cannot change within a StP session, so
// it returns the previously value on subsequent calls
//
string getPE()
{
    if ( re_pe_PE == NULL )
      {
	re_pe_PE = toolinfo_variable( "ProgrammingEnvironment" );
    // Begin Add ECR 6336
    if( re_pe_PE != NULL && re_pe_PE != "" ) 
      return re_pe_PE;
    // End Add ECR 6336
	if ( re_pe_PE == NULL )
	  re_pe_PE = "Reverse Engineering";

	if ( is_windows_platform() )
	  re_pe_PE = "Reverse Engineering";
	else if ( is_product_se() )
	  {
	    if ( re_pe_PE != "CodeCenter" )
	      re_pe_PE = "";
	  }
	else if ( is_product_booch() )
	  re_pe_PE = "";
      }
    return re_pe_PE;
}


// this is used as a parse condition to evaluate which navigation
// should appear
//
boolean
is_valid_programming_env()
{
    string localPE = getPE();
    if ( localPE == "" || localPE == "Reverse Engineering" )
      return False;
    else
      return True;
}


string
default_re_pe_nav_label()
{
    string localPE = getPE();
    if ( localPE == "Reverse Engineering" )
      return "Source Code";
    else
      return "Programming Environment";
}


// using the SMDB for navs means that we sometimes offer more than a
// single navigation. If we are going to do that, then we differentiate
// the titles with the words "declaration" and "definition"
//
string
alt1_re_pe_nav_label()
{
    string defaultLabel = default_re_pe_nav_label();
    if ( defaultLabel == "Source Code" && is_smdb_available() )
      return "Source Code Declaration";
    else
      return defaultLabel;
}


// and when we use thr routine above for the first name, we can use
// this to generate the second name
//
string
alt2_re_nav_label()
{
    return "Source Code Definition";
}


// TODO "Update Class using Reverse Engineering"
// Construct class from ParcPlace??
// Create from Reverse Engineering
// Construct from Reverse Engineering
