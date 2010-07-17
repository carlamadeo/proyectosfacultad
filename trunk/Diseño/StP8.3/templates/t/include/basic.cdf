/* 
 * Default  Case Description File: basic.cdf
 * Copyright (C) 1992 Programming Environments, Inc.
 * Copyright (C) 1993-1994 Interactive Development Environments, Inc.
 *
 * This file should be modified for integration with Test Case Execution
 * Tools
*/

BEGIN {
open     report "summary.rpt";    /* file name for summary */
print    HEADER ;
print    "sdf_name: " SDF_NAME "\nsdf_date: " SDF_DATE "\n";
print    "\nPrepared cases:";
}

CASE  {
open     case CASE_NAME ".tcf";
print    "cn " CASE_NAME ;
print    "ex " IS_EXERCISED ;
print    "is " STATE_NAME ;
foreach  USES_INDEX   {
   print "di " USES_NAME ;
   print "vi " USES_VALUE ;
   }
print    "bn " START_BY ;
print    "tn " END_BY ;
foreach  PRODUCES_INDEX   {
   print "do " PRODUCES_NAME ;
   print "vo " PRODUCES_VALUE ;
   }
close    case;
resume   report;           /* now add to output report the 8-char case */
print    CASE_NAME %12% ;  /* name, right-justified in a field of 12   */
}

END      close report;
