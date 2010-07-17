//*****************************************************************************
// DESCRIPTION: DesignTestCases.inc
//              ----------------------
//              (c) 1998 by AONIX
//*****************************************************************************
// Include-file for contains aoxiliary functions used in DesignTestCases.qrl  
//*****************************************************************************
// Functions in module:
// ============================================================================
// string      formatTraceData (string sTestUnitId, string sOutputPath)
// void        formatTestCases (string sOutputDir, string sCdfFile,
//				string sTestUnitId)
// void        writeFileText   (string sOutputDir, string sFileText)
// string      createCDFHeader ()
// string      createCDFFooter ()
//*****************************************************************************

string
formatTraceData(string sTestUnitId, string sOutputPath)
{
  
  string sCdfFile, sFileText;
  
  sFileText = createCDFHeader();
  sFileText = sFileText + createCDFFooter();

  sCdfFile = sOutputPath + sTestUnitId + "/" + sTestUnitId + "1.cdf";
  writeFileText(sCdfFile, sFileText);

  // message("Formatting Test Case Data");
  // formatTestCases(sOutputDir, sCdfFile, sTestUnitId);
  return sCdfFile;
}


void 
formatTestCases(string sOutputDir, string sCdfFile, string sTestUnitId )
{
  if (CheckForSDF(sOutputDir, "TDDB"))
     tdriverPrepare (sTestUnitId, sCdfFile, "UseCaseTestScripts");
  else
     message ("TDDB file for " + sTestUnitId + " does not exist.");
}

void
writeFileText (string sOutputDir, string sFileText)
{
  string sOutputFile = sOutputDir;
  write_file (sOutputFile, sFileText);
}


string 
createCDFHeader()
{
  return (
    "BEGIN \n"+
    "  {\n"+            
    "    open  report \"UseCaseTestScripts/scenario.rpt\";    /* file name for summary */\n"+
    // "    print HEADER ;" + "\n"+
    "  }\n"+
    "CASE \n"+
    "  {\n"
            );
}

string 
createCDFFooter()
{
  return (
  "  print    ACTION_NAME \":\" CASE_NAME ;\n"+
  "}\n"+

  "END\n"+
  "  {\n"+
  "    close report;\n"+
  "  }\n"
         );
}


