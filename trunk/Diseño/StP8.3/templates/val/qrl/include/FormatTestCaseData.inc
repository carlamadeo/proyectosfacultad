//*****************************************************************************
// DESCRIPTION: FormatTestCasesData.inc
//              ----------------------
//              (c) 1998 by AONIX
//*****************************************************************************
// This file is part of validator package.
// Include-file for GenerateTestSpecification.qrl, 
//*****************************************************************************
// Functions in module:
//=============================================================================
// string  formatTestCaseData     (string sTestUnitId)
// string  createCDFHeader        ()
// string  createCDFFooter        ()
// string  buildEventSection      (string sUseCaseName, string sScenarioName, 
//                                 int scenario_id, string actorids)
// string  printArguments         (string event)
// string  printExpectedOutputs   (int event_id)
// set     buildDataitemSet       (int id)
// void    writeFileText          (string sOutputDir, string sFileText)
// 
// string  getEventName           (int link_id)
// string  getSourceName          (int link_id)
// string  getDestinationName     (int link_id)
// 
// list    getEvents              (int scenario_id)
// string  determineIO            (int linkid, int scenario_id, 
//                                 string actorids)
// string  printIsTestCase        (string sScenarioName)
// string  printExercises         ()
// 
// string  printProbes            ()
// 
// added for OMT:
// string  getData                (string IOtype, int event_id)
// boolean checkExpectedOutput    (int id)
// string  getCommand             (int id)
//*****************************************************************************
void
formatTestCaseData(string sTestUnitId, string sUseCaseName)
{
  list  lstScenarious;
  int   count1, i;
  int   count2, j;
  string sScenarioName;
  node  ndScenario;
  int   nScenarioId;
  set   setDataTypes;
  string sActorIDs;
  
  string sFileText;
  string sCdfFile;

  // DataItemsDataTypes = set_create ("string"); 
  // set_clear(DataItemsDataTypes);

  validatorUserMessage(" " + sUseCaseName); 
 
  sFileText = createCDFHeader();
  sActorIDs = GetActorIDList(sUseCaseName);
  lstScenarious = GetScenarioList(sUseCaseName);
  count2 = list_count(lstScenarious);
  validatorUserMessage("  Scenarious:");
  for (j=0;j<count2;j=j+1)
    {
      ndScenario = list_get(lstScenarious,j);
      sScenarioName = RemoveSpaces(ndScenario.name);
      nScenarioId = ndScenario.id;
      validatorUserMessage("   " + sScenarioName);
      sFileText = sFileText + buildEventSection(sUseCaseName, sScenarioName, nScenarioId, sActorIDs);
      sFileText = sFileText + " }" + "\n";
    }
  validatorUserMessage(" ");

  sFileText = sFileText + createCDFFooter();
  sCdfFile = current_projdir() + current_system() + "/test_files/" +
             sTestUnitId + "/" + sTestUnitId + ".cdf";
  writeFileText(sCdfFile, sFileText);
}


string 
createCDFHeader()
{
  return (
        "BEGIN \n" +
        "  {\n" +
      //"    print HEADER ;\n" +
        "    open case  \"testcases.tcf\";" + "\n"+
        "  }\n"+
        "CASE \n"+
        "  {\n"
            );  
}

string
createCDFFooter()
{
    return (
        "  resume   case;\n"+
        "}\n"+

        "END\n"+
        "  {\n"+
        "    close    case;\n"+
        "  }\n"
            );
}

string
buildEventSection(string sUseCaseName, string sScenarioName,
                  int nScenarioId, string sActorIds)
{
  list   lstEvents;
  int    nEventId, i,count;
  string sEvent, sEventName, sFromClass, sToClass, sActionName;
  link_ref lrefTemp;
  int    nEventNumber = 0;
  string sIO, sPG;
  string sFileText = "";

  lstEvents = list_create ("string", 0);

  // list_clear(lstEvents);

  lstEvents = getEvents(nScenarioId);
  count = list_count(lstEvents);

  sFileText = sFileText + printIsTestCase(sScenarioName);

  sFileText = sFileText + printExercises();
  sFileText = sFileText + printProbes();
  // PrintEvaluation();

  for (i=0; i<count; i=i+1)
    {
       lrefTemp = list_get(lstEvents, i);
       sEvent = getEventName(lrefTemp.link_id);

       if (VALIDATOR_BASE_PRODUCT == "UML")
            sEventName = StripData(sEvent);
       else
            sEventName = sEvent;

       nEventId = lrefTemp.link_id;

       sIO = determineIO(lrefTemp.link_id, nScenarioId, sActorIds);


       if (VALIDATOR_BASE_PRODUCT == "UML")
       {

            if (sIO == "INPUT")
                sPG = "IP4";
            else 
                if (sIO == "OUTPUT")
                    sPG = "OP4";

            sFileText = sFileText + "print \"<" + sPG + ">Step " + ++nEventNumber + ". ";
            sFileText = sFileText + getSourceName(nEventId) + " <" + sEventName 
                        + "> " + getDestinationName(nEventId)  + "\";\n";

            if (sIO == "INPUT")
                sFileText = sFileText + printArguments(sEvent);
            else 
                if (sIO == "OUTPUT")
                    sFileText = sFileText + printExpectedOutputs(nEventId);

       }
       else
       {
            sPG = "P4";
            sFileText = sFileText + "print \"<" + sPG + ">Step " + ++nEventNumber + ". ";
            sFileText = sFileText + getSourceName(nEventId) + " <" + sEventName 
                        + "> " + getDestinationName(nEventId)  + "\";\n";

            sFileText = sFileText + getData(sIO, nEventId);
       }
    }

  return (sFileText + "\n\n");
}

string
printArguments(string sEvent)
{
  int i,count;
  int l1,l2;
  string sProduces;

  l1 = string_find(sEvent, 0, "(");

  if (l1 == string_length(sEvent) )
     return "";

  string sData;
  string sFileText = "";
  list lstData;


  l1 = string_find(sEvent, 0, ":");

  if (l1 < string_length(sEvent) )
     {
       if (sProduces == "")
          sProduces = string_extract( sEvent, 0, l1 );
       else
          sProduces = sProduces + "," + string_extract( sEvent, 0, l1 );
     }

  l2 = string_find(sEvent, 0, "(");


  sEvent = string_extract( sEvent,l2+1,string_length(sEvent)-(l2+1) );

  sEvent = string_search_and_replace(sEvent, ")", "");

  lstData = string_to_list(sEvent, ",");

  count = list_count(lstData);

  for (i=0;i<count;i++)
    {
      sData = list_get(lstData,i);
      sFileText = sFileText + "print \"<DI><" + RemoveSpaces(sData)
          + "> ==> \" " + sData + ";" + "\n";
    }
  
  return sFileText;

}

string
printExpectedOutputs(int nEventId)
{
  int i,count;
  item itmHasExpectedOutput;
  item itmExpectedOutputCommand;
  item itmOutputArgumentList;
  item itmOutputHasFixedValue;

  string sRepQuery1 = "item [HasExpectedOutput && obj_id == ${nEventId}]";
  string sRepQuery2 = "item [ExpectedOutputCommand && obj_id == ${nEventId}]";
  string sRepQuery3 = "item [OutputArgumentList && obj_id == ${nEventId}]";
  string sRepQuery4 = "item [OutputHasFixedValue && obj_id == ${nEventId}]";

  string sTemp;
  string sFileText = "";

  itmHasExpectedOutput = find_by_query (sRepQuery1);
  itmExpectedOutputCommand = find_by_query (sRepQuery2);
  itmOutputArgumentList  = find_by_query (sRepQuery3);
  itmOutputHasFixedValue = find_by_query (sRepQuery4);

  if (itmExpectedOutputCommand != NULL)
    {
      if (itmOutputArgumentList != NULL)
        {
          sTemp = RemoveSpaces(itmOutputArgumentList.value);
          sTemp = "'\"" + string_search_and_replace(itmOutputArgumentList.value, ",", "\"' '\"") + "\"'";


          sFileText = sFileText + "print \"<GR>" + itmExpectedOutputCommand.value + " " + sTemp +  "\";" + "\n";
          return sFileText;
        }
      else
        {
          sFileText = sFileText + "print \"<GR>" +
               itmExpectedOutputCommand.value + "\";" + "\n";
          return sFileText;
        }
    }

  if (itmOutputHasFixedValue != NULL)
    {
      sFileText = sFileText + "print \"<ER>" + nEventId + "\";" + "\n";
      return sFileText;
    }

  // No expected output
  return (sFileText + "print \"<NO>No Expected Result Given.\";\n");
}

set
buildDataitemSet(int nId)
{
  list lstDataItems;
  int  i,count;
  note   ntTestDataItem;
  item   itmTDataItemName, itmTFixedValue, itmTDataType, itmTDataTypeName;
  string sDefaultTypeName="UNSPECIFIED" + SEPARATOR2 + "UNSPECIFIED";
  string sTypeName= sDefaultTypeName;

  string sRepQuery1 = "note [TestDataItem && obj_id == ${nId}] sort by name";
  string sRepQuery2 = "item [TDataItemName && note_id == ${ntTestDataItem.nId}]";
  string sRepQuery3 = "item [TFixedValue && note_id == ${ntTestDataItem.nId}]";
  string sRepQuery4 = "item [TDataType && note_id == ${ntTestDataItem.nId}]";
  string sRepQuery5 = "item [TDataTypeName && note_id == ${ntTestDataItem.nId}]";

  set setDataItemsDataTypes;

  
  lstDataItems = list_select(sRepQuery1);

  count = list_count(lstDataItems);

  for (i=0;i<count;i=i+1)
    {
      ntTestDataItem = list_get(lstDataItems,i);
      itmTDataItemName = find_by_query(sRepQuery2);

      itmTFixedValue = find_by_query(sRepQuery3);
      itmTDataType = find_by_query(sRepQuery4);
      itmTDataTypeName = find_by_query(sRepQuery5);

      if (itmTFixedValue != NULL)
        {
           sTypeName = "FIXED" + SEPARATOR2 + itmTFixedValue.value;
        }

      else if (itmTDataType != NULL)
        {
           sTypeName = "DEFINED" + SEPARATOR2 + itmTDataType.value;
        }

      else if (itmTDataTypeName != NULL)
        {
           sTypeName = "DEFINED" + SEPARATOR2 + itmTDataTypeName.value;
        }
      set_add (setDataItemsDataTypes, itmTDataItemName.value + SEPARATOR + sTypeName);
    }
  return setDataItemsDataTypes;
}

void
writeFileText (string sOutputDir, string sFileText)
{
  write_file (sOutputDir, sFileText);
}


string
getEventName(int nLinkId)
{
  link lnkObject;
  item itmObject;

  string sRepQuery = "link [ id == ${nLinkId} ]";

  lnkObject = find_by_query(sRepQuery);

  if (lnkObject == NULL)
     return (NULL);
  else
    return (RemoveSpaces(lnkObject.name));
}


string
getSourceName(int nLinkId)
{
  link lnkObject;
  node ndObject;
  string sRepQuery1 = "link [ id == ${nLinkId} ]";
  string sRepQuery2 = "node [  id == ${lnkObject.from_node_id} ]";

  lnkObject = find_by_query (sRepQuery1);

  if (lnkObject == NULL)
    return ("");
  else 
    {
      ndObject = find_by_query(sRepQuery2);
      return (ndObject.name);
    }
}


string
getDestinationName(int nLinkId)
{
  link lnkObject;
  node ndObject;
  string sRepQuery1 = "link [ id == ${nLinkId} ]";
  string sRepQuery2 = "node [ id == ${lnkObject.to_node_id} ]";

  lnkObject = find_by_query (sRepQuery1);

  if (lnkObject == NULL)
    return ("");
  else 
    {
      ndObject = find_by_query(sRepQuery2);
      return (ndObject.name);
    }
}


list
getEvents(int nScenarioId)
{
 
  string sLinkrefQuery, sNoderefQuery;
  node_ref nrefObject;

  sNoderefQuery = "node_ref [node_id == ${nScenarioId} ] ";
  nrefObject = find_by_query (sNoderefQuery);

  sLinkrefQuery = "link_ref [ file_id == ${nrefObject.file_id} && 
           link [ UmlSimpleMessage ] ] sort by ycoord";

  return (list_select (sLinkrefQuery) );
}


string
determineIO(int nLinkId, int nScenarioId, string nActorIds)
{
 
  string sLinkrefQuery, sNoderefQuery;
  node_ref nrefObject;

  if (nActorIds == "")
      nActorIds = "0";

  sNoderefQuery = "node_ref [node_id == ${nScenarioId} ] ";
  nrefObject = find_by_query (sNoderefQuery);

  sLinkrefQuery = "link_ref [ file_id == ${nrefObject.file_id} && 
           link [ UMLSimpleMessage   &&
           id == ${nLinkId} &&
           to_node[${nActorIds}] ] ]";

  if ( find_by_query (sLinkrefQuery) == NULL)
     return ("INPUT");
  else 
     return ("OUTPUT");
}


string
printIsTestCase(string sScenarioName)
{
  return (
    "if (ACTION_NAME == \"" + sScenarioName + "\")\n" +
    " {\n" +
    "print  \"@\";\n"
          );
}

string
printExercises()
{
  return (
    "if (IS_EXERCISED==\"y\")\n" +
    "print ACTION_NAME \"#GOOD#\" CASE_NAME  ;\n" +
    "  else \n" +
    "print ACTION_NAME \"#INVALID#\" CASE_NAME  ;\n"
          );
}

string
printProbes()
{
  return (
       "if (CASE_INDEX==1)   {\n" +
       "   print \"<P3>Probes: All inputs at reference values\";\n" +
       "   }\n" +
    // "else if (CASE_INDEX==2)   {\n" +
    // "   print \"<P3>Probes: All inputs at Low Bounds\";\n" +
    // "   }\n" +
    // "else if (CASE_INDEX==3)   {\n" +
    // "   print \"<P3>Probes: All inputs at High Bounds\";\n" +
    // "   }\n" +
       "else\n" +
       "   {\n" +
       "   foreach USES_INDEX   {\n" +
       "      if ( USES_SAMPLE != \"reference\" )   {\n" +
       "         if ( USES_SAMPLE != \"fixed_value\" ) {\n" +
       "            print \"<P3>Probes: \" USES_DOMAIN  \"  \" USES_NAME \" at \" USES_SAMPLE ;\n" +
       "            }\n" +
       "         }\n" +
       "      }\n" +
       "   }\n"
         );
}

//++++++

string // this function is applicable for OMT model only
getData(string sIOType, int nEventId)
{
  int i,count;
  note ntTestDataItem;
  list lstTNotes, lstNotes;
  item itmTDataItemName;

  string sRepQuery1 = "note [TestDataItem && obj_id == ${nEventId}]";
  string sRepQuery2 = "item [TDataItemName && note_id == ${ntTestDataItem.id}]";

  string sFileText = "";
  
  lstNotes = list_select(sRepQuery1);
  lstTNotes = SortBySequenceNumber(lstNotes, nEventId);
  string PG;

  count = list_count(lstTNotes);

  if (sIOType == "INPUT" && count > 0)
    {
      sFileText = sFileText + "print \"<P5>Data Inputs:\";" + "\n";
          PG = "DI";
    }
  else if (sIOType == "OUTPUT" && checkExpectedOutput(nEventId))
    {
      sFileText = sFileText + "print \"<GR>" +
           getCommand(nEventId) + "\";" + "\n";
      PG = "PP";
    }
  else if (sIOType == "OUTPUT" && count == 0)
    {
      sFileText = sFileText + "print \"<NO>No Expected Result Given.\";" + "\n";
      PG = "PP";
    }
  else if  (sIOType == "OUTPUT" && count > 0)
    {
      sFileText = sFileText + "print \"<ER>Expected Results:\";" + "\n";
      PG = "DO";
    }

  for (i=0; i<count; i=i+1)
    {

      ntTestDataItem = list_get(lstTNotes, i);
      itmTDataItemName = find_by_query(sRepQuery2);

      if (PG == "PP")
        sFileText = sFileText + "print \"<CP>" + RemoveSpaces(itmTDataItemName.value)
          + "\";" + "\n";
      else if (PG == "DI")
        sFileText = sFileText + "print \"<DI><" + RemoveSpaces(itmTDataItemName.value)
          + "> ==> \" " + itmTDataItemName.value + ";" + "\n";
      else if (PG == "DO")
        sFileText = sFileText + "print \"<DO><" + RemoveSpaces(itmTDataItemName.value)
          + "> ==> \" " + itmTDataItemName.value + ";" + "\n";

    }

  return sFileText;

}

boolean
checkExpectedOutput(int nId)
{
  string sRepQuery = "note [ExpectedOutput && obj_id == ${nId}]";
  note ntExpectedOutput = find_by_query(sRepQuery);

  if (ntExpectedOutput != NULL)
     return (True);
  else
     return (False);
}

string
getCommand(int nId)
{
  string sRepQuery = "item [ExpectedOutputCommand && obj_id == ${nId}]";
  item itmExpectedOutputCommand = find_by_query(sRepQuery);

  if (itmExpectedOutputCommand != NULL)
     return (itmExpectedOutputCommand.value);
  else
     return ("");
}



