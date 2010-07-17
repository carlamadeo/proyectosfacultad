/* T3 Case Description File: truth.cdf
** Copyright (C) 1992 Programming Environments, Inc.
** Copyright (C) 1992-1996 Interactive Development Environments, Inc.
*/

BEGIN {
   open  report   "truth.rpt";
   print "Software Description File: " SDF_NAME " (" SDF_DATE ")";
   print "";      /* .--- col 12 */
   print "Test Case  1  2  3  4  5  6  7  8  9 10";
   print "----------------------------------------------------------------";
   typemark off;           /* no quote marks around T, F                */
   open  case  "sort.tmp"; /* second file, to catch CASE section output */
}

CASE  {
/* rows: " nnnnnnnn  b  b  b  b  b  b  b  b  b  b" */
   newline  off;
   print    " " CASE_NAME ;
   foreach  USES_INDEX   {
      if (USES_INDEX==11) break; /* only do first 10 input dataitems */
      print "  " USES_VALUE %1,  /* keep only 1st char: T, F, or U   */
                  align=left,truncate=yes% ;
   }
   if ( IS_EXERCISED == "y" )    print "    => " ACTION_NAME;
   newline  on;
   print    "";                  /* end the output line for this case*/
}

END   {
   close case;

   resume report;
   print "IN TESTCASE ORDER"; /* list the output from the CASE section  */
   run   "cat", TESTCASE_DIR "sort.tmp";  /* TESTCASE_DIR has end slash */

   print "";
   print "IN STANDARD DECISION TABLE ORDER";
            /* sort starting at col 12; puts the f,t,u   */
            /* columns in the usual decision table order */
   run   "sort","+1", TESTCASE_DIR "sort.tmp" ;

   print "------------------------------------------------------------------";
   print "\nData Input Names for columns above\n";
   foreach  USES_INDEX   print  USES_INDEX %3% " " USES_NAME ;
   close report;
}

