/* T3 Case Description File: cmdline.cdf
** Copyright (C) 1992 Programming Environments, Inc.
*/

BEGIN {
   open report   "cmdline.rpt";
   print "Software Description File: " SDF_NAME " (" SDF_DATE ")";
   print "";
   print "Test Case  Command line text";
   print "------------------------------------------------------------------";
   typemark off;
   escape off;
}

CASE  {
   newline  off;
   print    " " CASE_NAME ": " ;
   foreach  USES_INDEX   {
      print USES_VALUE;
      }
   newline  on;
   print    "";                  /* end the output line for this case*/
}

END   {
   print "------------------------------------------------------------------";
   close report;
}

