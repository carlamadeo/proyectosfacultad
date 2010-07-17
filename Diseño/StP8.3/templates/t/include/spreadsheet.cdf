/* T3 Case Description File: spreadsheet.cdf
** Copyright (C) 1992 Programming Environments, Inc.
*/

BEGIN {
}

CASE  {


   if (CASE_INDEX == 1)
     {
       open case ACTION_NAME ".txt";

       newline  off;
       print "CaseName\tExercised" ;
       foreach  USES_INDEX
         {
            print "\t" USES_NAME "\t" USES_NAME "_PROBE";
         }

       foreach  PRODUCES_INDEX
         {
            print "\t" PRODUCES_NAME;
         }
       newline  on;
       print    "";
     }

   newline  off;
   print "TC_" CASE_NAME "\t" IS_EXERCISED ;
   foreach  USES_INDEX
     {
       print "\t" USES_VALUE "\t" USES_DOMAIN;
     }


   foreach  PRODUCES_INDEX
     {
       print "\t" PRODUCES_VALUE;
     }

   newline  on;
   print    "";

/*
   if (CASE_INDEX == CASE_COUNT)
     {
       close case;
     }
*/
}

END   {

       close case;
}

