BEGIN 
  {
    open  report "types.rpt";    /* file name for summary */
  }
CASE 
  {
   if (CASE_INDEX == 1)
     foreach  USES_INDEX
      {
          print USES_NAME " " USES_TYPE;
      }
  }
END
  {
    close report;
  }
