/* T3 Case Description File: summary.cdf
** Copyright (C) 1992 Programming Environments, Inc.
**
** this is a slightly modified form of the summary report
**    produced by T3 at the end of the design step
*/

BEGIN {
open  report   "summary.rpt"; /* name of output file in testcase directory */
print HEADER;                 /* standard herald */
print "Copyright (C) 1987-1992 Programming Environments, Inc.\n";
print "s_packet: " SDF_NAME ;
print "unitdate: " SDF_DATE ;
print "t_packet: " DRF_NAME ;
print "casedate: " DRF_DATE ;
print "";                     /* an extra blank line for readability       */
}

CASE  {
print "########################################################################";
print "CASENAME          " CASE_NAME ;
if (IS_EXERCISED=="y")   print "EXERCISES         " ACTION_NAME ;
else                     print "FAILS TO EXERCISE " ACTION_NAME ;
print "IN STATE          " STATE_NAME ;
if (CASE_INDEX==1)   {
   print "PURPOSE           all inputs at reference values";
   }
else
   {                          /* list all dataitems not at reference value */
   newline off;
   print "PURPOSE";
   foreach USES_INDEX   {
      if ( USES_SAMPLE != "reference" )   {
         if ( USES_SAMPLE != "fixed_value" ) {
            print "           to probe", USES_NAME, "at",
               USES_DOMAIN, USES_SUBDOMAIN, USES_SAMPLE "\n       ";
            }
         }
      }
   print "\n";                /* terminate last partial line here          */
   newline on;
   }
print "INPUT DATA\n"     "Name --- Value";
print "------------------------------------------------------------------------";
foreach USES_INDEX            /* list input dataitem names and values      */
   print USES_NAME "\n     --- " USES_VALUE ;
print "------------------------------------------------------------------------";
print "START BY          " START_BY ;
print "END BY            " END_BY ;
print "OUTPUT DATA\n"    "Name --- Value";
print "------------------------------------------------------------------------";
if (PRODUCES_COUNT==0)  {     /* special case when there are no outputs    */
   print "<none>" ;
   }
else  {
   foreach PRODUCES_INDEX  {  /* list output dataitem names and values     */
      print PRODUCES_NAME "\n     --- " PRODUCES_VALUE ;
      }
   }
print "------------------------------------------------------------------------";
if (TRANSITION_COUNT==0)   {  /* special case when there are no transitions*/
   print "TRANSITION        <none>\n";
   }
else  {
   newline off;
   print "TRANSITION";
   foreach TRANSITION_INDEX{  /* list transition names                     */
      print "        ", TRANSITION_NAME, "\n          ";
      }
   print "\n";                /* terminate last partial line here          */
   newline on;
   }
}

END   {
print "########################################################################";
print "END REPORT";
close report;
}
