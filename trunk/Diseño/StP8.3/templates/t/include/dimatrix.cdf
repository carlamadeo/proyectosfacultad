/* T3 Case Description File: dimatrix.cdf
** Copyright (C) 1992 Programming Environments, Inc.
*/

BEGIN {
   open report   "dimatrix.rpt";
   print "Software Description File: " SDF_NAME " (" SDF_DATE ")";
   print "";
   print "Test Case ?       1        2        3        4        5        6";
   print "------------------------------------------------------------------";
   typemark off;
}

CASE  {
/* rows: " nnnnnnnn X  ggggggg  ggggggg  ggggggg  ggggggg  ggggggg  ggggggg" */
   newline  off;
   print    " " CASE_NAME " " IS_EXERCISED ;
   foreach  USES_INDEX   {
      if (USES_INDEX==7) break;        /* only do first 6 input dataitems  */
      print "  " USES_VALUE %7,truncate=yes%; /* truncate more than 7 chars*/
      }
   newline  on;
   print    "";                  /* end the output line for this case*/
}

END   {
   print "------------------------------------------------------------------";
   print "         (^ if 'y', test case exercises action)";
   print "\nData Input Names for columns above\n";
   foreach USES_INDEX   print  USES_INDEX %3% " " USES_NAME ;
   close report;
}

