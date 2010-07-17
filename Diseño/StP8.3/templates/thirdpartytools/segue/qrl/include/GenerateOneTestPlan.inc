
  const string ShowRequirements = "False";

  // Constants used by the data file

  const string begincomment = "\t[ ] // " ;
  const string endcomment = "";


  list ScenarioList;

  string OUTPUTPATH = current_projdir() + current_system() + "/test_files/";

  const string extension = ".pln";


  string SORACLESDIR = OUTPUTPATH + "SCRIPT_ORACLES/";

  string TESTDATA="";
  string OUTPUTTEXT="";
  string OUTPUTDIR;

//  const string SEPARATOR = "#";
//  const string SEPARATOR2 = "@";



  set ACTORSET;

void 
GenerateOneQAOrganizerIntegration()
{


  string scriptfile;
  string test_unit_id;

  string filetext;

  string path;
  string inputfile;
  list testcaselist;

  ACTORSET = GetActors(use_case_name);

  test_unit_id = Get_T_test_unit_id(use_case_name, "");

  SCRIPTDIR = current_projdir() + current_system() + "/testscpt_files/segue/" + test_unit_id;

  ScenarioList = GetScenarioList(use_case_name);



  OUTPUTDIR = OUTPUTPATH + test_unit_id;

  path = OUTPUTDIR;
  inputfile = path + "/testcases.rpt";

  if (!CheckForSDF(OUTPUTDIR, "TDDB"))
    {
      message ("TDDB for " + test_unit_id + " does not exist.");
      return;
    }

  FormatTestCaseData(test_unit_id);


  if (CheckForSDF(path, "testcases.rpt"))
     filetext = read_file (inputfile);
  else
    {
      message ("testcases.rpt for " + test_unit_id + " does not exist.");
      return;
    }


  testcaselist = string_to_list(filetext, "\n");

  message ("");
  message ("");
  message ("Generating scripts for Use Case: " + use_case_name);

  OUTPUTTEXT = begincomment + "Use Case: " + use_case_name + endcomment;

  PrintScenarioTestCases(testcaselist, use_case_name);

  scriptfile = SCRIPTDIR + "/" + use_case_name + extension;

  WriteFileText(scriptfile, OUTPUTTEXT);


}


void
PrintScenarioTestCases(list testcaselist, string use_case_name)
{


  int i,count;
  string testcase;
  list textlist;
  list actiondata;
  string  action, pastaction, exercises, testunitid;
  int testid=0;
  list validlist;

  int j,count2;
  boolean firstaction = False;

  string path, inputfile, filetext;
  string testcasefile;


  validlist = list_create("string",0);

  string testcasedata;
  list testcaseinfo;

  count = list_count(testcaselist);

  firstaction = True;

  for (i=0;i<count;i++)
    {

      testcasedata = list_get(testcaselist,i);

      testcaseinfo = string_to_list(testcasedata, " ");

      action = list_get(testcaseinfo, 0);
      testcase = list_get(testcaseinfo,1);


      if (i==0)
         pastaction = action;

      if (pastaction != action)
        {

           PrintCases(validlist, pastaction, use_case_name);

           list_clear(validlist);
           firstaction = False;
        }


      list_append (validlist, testcase);

      pastaction = action;
    }


   PrintCases(validlist, action, use_case_name);


}



void
PrintCases(list validlist, string action, string use_case_name)
{
   int count2,j;
   int testid; 
 
   string testunitid;
   string testcase;
   string datafile;
 
   message ("  Scenario: " + action);

   OUTPUTTEXT = OUTPUTTEXT + "\n[+] " + action;
   OUTPUTTEXT = OUTPUTTEXT + "\n\t[ ] script: " + action + ".t";
   OUTPUTTEXT = OUTPUTTEXT + "\n\t[ ] testcase: " + action;


   if (ShowRequirements == "True")
      PrintRequirements(use_case_name, action);

   count2 = list_count(validlist);


   testid=0;
   for (j=0;j<count2;j++)
     {
       testcase = list_get(validlist,j);

       message ("    " + testcase);

       FormatTestCase (testcase , ++testid, use_case_name);
     }


}

void 
FormatTestCase (string testcase, int testid, string use_case_name)
{

  string st;
  int i,count;
  set probeset;
  list actiondata;
  string  action, exercises, testunitid, domaintype;
  int start = 0;
  probeset = set_create ("string");
  list tmpprobelist;
  list testcasedata;
  boolean done=False;
  string exboolean;


  testcasedata = GetTestCaseData (testcase);

  action = list_get(testcasedata, 0);
  exercises = list_get(testcasedata, 1);
  testunitid = list_get(testcasedata, 2);



  TESTDATA="\n\t\t[ ] testdata: {";
  OUTPUTTEXT = OUTPUTTEXT + "\n\t[+] TC_" + testunitid;


  OUTPUTTEXT = OUTPUTTEXT + "\n\t" + begincomment;
  OUTPUTTEXT = OUTPUTTEXT + "\n\t" + begincomment + "Test Case: " + testid + endcomment;
  OUTPUTTEXT = OUTPUTTEXT + "\n\t" + begincomment + "Test Execution ID: " + testunitid + endcomment;

  if (exercises == "INVALID" )
    {
      OUTPUTTEXT = OUTPUTTEXT + "\n\t" + begincomment + "Evaluates exception for scenario " + action + endcomment;
      OUTPUTTEXT = OUTPUTTEXT + "\n\t" + begincomment + "Steps or entries after Exception may not be executable " + endcomment;
      exboolean = "False";
    }
  else
    {
     OUTPUTTEXT = OUTPUTTEXT + "\n\t" + begincomment + "Evaluates scenario " + action + endcomment;
     exboolean = "True";
    }

  TESTDATA = TESTDATA + " " + exboolean;

  count = 3;

  if ( list_get(testcasedata, count) != "PROBES")
    done = True;
//  else
//    description = description + "\\n\\nThis test case probes the following data items.\\n";


  while (!done )
    {
      ++count;
      st = list_get ( testcasedata, count);
      st = string_search_and_replace(st,"invalid", "Exception");
      st = string_search_and_replace(st,"valid ", "");
      
      OUTPUTTEXT = OUTPUTTEXT + "\n\t" + begincomment  + st + endcomment;


      ++count;

      if (list_get(testcasedata,count) != "PROBES" )
        done = True;
    }


  FormatTestData (testcasedata, count);


  OUTPUTTEXT = OUTPUTTEXT + TESTDATA + "}";

  TESTDATA = "";

}




list
GetTestCaseData (string testcase)
{

  list textlist;

  string path, inputfile, filetext;

  path = OUTPUTDIR + "/UseCaseTestScripts";
  inputfile = path + "/" + testcase;



  if (CheckForSDF(path, testcase))
     filetext = read_file (inputfile);
  else
     {
       message (testcase + " does not exist.");
       return;
     }

  textlist = string_to_list(filetext,"\n");

  return (textlist);

}


void
FormatTestData(list testcasedata, int count)
{
  string st;
  boolean done = False;

  int currentstep = 0;
  int nextstep = 1;
  string steptype;
  string command;
  string steptext = "";
  string description = "";
  string dataitem, datavalue;

  string tmp;
  list tmp_list;

  if (count >= list_count(testcasedata))
    done = True;

  while (!done)
    {
       st = list_get(testcasedata,count);

       if ( st == "INPUTEVENT" )
         {
            ++count;
            tmp = list_get(testcasedata, count);
            OUTPUTTEXT = OUTPUTTEXT + "\n\t" + begincomment + tmp + endcomment;
            tmp_list = string_to_list (tmp, " ");
            TESTDATA =  TESTDATA + ", " + StripTheQuotes (list_get (tmp_list, 3) );
 
         }
       else if ( st == "DATAIN" )
         {

            ++count;
            dataitem = list_get(testcasedata, count);
            ++count;
            datavalue = list_get(testcasedata, count);

            OUTPUTTEXT = OUTPUTTEXT + "\n\t" + begincomment + " Set " + dataitem + " to " + datavalue + endcomment;
            TESTDATA =  TESTDATA + ", " + datavalue;
         }

       else if ( st == "OUTPUTEVENT" )
         {
            ++count;
            OUTPUTTEXT = OUTPUTTEXT + "\n\t" + begincomment + list_get(testcasedata, count) + endcomment;
 
         }
       else if ( st == "NORESULTS" )
         {
            OUTPUTTEXT = OUTPUTTEXT + "\n\t" + begincomment + " No Expected Results Given" + endcomment;
         }
       else if ( st == "GENERATEDRESULTS" )
         {
            ++count;
            command = list_get(testcasedata, count);
            OUTPUTTEXT = OUTPUTTEXT + "\n\t" + begincomment +  PrintExpectedResults(command)  + endcomment;
         }
         
       ++count;
       if (count >= list_count(testcasedata))
          done = True;
    }


}



void
PrintExpectedResults(string command)
{
  string tmp;
  string dataitem, datavalue, datavalue1;
  string datatype, datatypename;
  list tmplist2;

  string sysdir = current_projdir() + current_system() + "/";

//
// Fix to remove ' from the expected result program - 5/11/98
// and to add /" in place of "
//
//
  tmp = string_search_and_replace (command, "\"", "@@@");
  command = string_search_and_replace (tmp, "@@@", "\\\"");
//
//

  system ("sh -c '" + SORACLESDIR + command + "'   > " + sysdir + "output");

  if(write_file_access(sysdir + "output"))
    {
      
      OUTPUTTEXT = OUTPUTTEXT + "\n\t" + begincomment + "Expected Result" + endcomment;
      
      datavalue = read_file(sysdir + "output");
      tmp = string_search_and_replace ( datavalue , "\n", endcomment+ "@@@" +begincomment ) + endcomment;
      tmp = string_search_and_replace ( tmp, "@@@", "\n\t");
      OUTPUTTEXT = OUTPUTTEXT + "\n\t" + begincomment + tmp;
 
      system ("rm " + sysdir + "output");

    }
  else
    {
      datavalue = "UNKNOWN";
    }


  datavalue = string_search_and_replace ( datavalue , "\n","");


}



void
PrintFixedResults(string event_id)
{

  string query = "note [expectedoutput && obj_id == ${event_id}]";
  note notevar;
  string tmp;

  notevar = find_by_query(query);

  if (notevar != NULL)
    {
      tmp = string_search_and_replace ( notevar.desc, "\n", "" );
  
      OUTPUTTEXT = OUTPUTTEXT + "\n\t" + begincomment + "Expected Fixed Result" + endcomment;

      OUTPUTTEXT = OUTPUTTEXT + "\n\t" + begincomment + tmp + endcomment;

    }

}



string
EventName(string st)
{
  list st1, st2;
  string data;

  st1 = string_to_list(st, "<");
  st2 = string_to_list(list_get(st1, 1), ">");
  data = list_get(st2,0);

  return (data);

}





void
PrintInputCorrelation(string st)
{
  string tmp, dataitem, datavalue, datavalue1;
  list tmplist, tmplist2;
  string datatype, datatypename;

  tmplist = string_to_list (st, ">");
  tmplist = string_to_list (list_get(tmplist,0), "<");

  dataitem = list_get(tmplist,0);

  tmplist = string_to_list (st, " ");

  list_delete(tmplist,0);
  list_delete(tmplist,0);

  datavalue = list_to_string(tmplist," ");

  TESTDATA =  TESTDATA + ", " +  datavalue;

}




void
PrintEventCorrelation(string st)
{
  string tmp, dataitem, datavalue;
  list tmplist;


tmplist = string_to_list (st, ">");
tmplist = string_to_list (list_get(tmplist,0), "<");

dataitem = list_get(tmplist,1);


//TESTDATA = TESTDATA + dataitem + ", ";

}

string
StripTheQuotes(string datavalue)
{
  string tmp;

  tmp = string_search_and_replace(datavalue, "\"", "");

  return (tmp);
}

string
DetermineOutputEvent(string st)
{
  string tmp, dataitem, datavalue;
  list tmplist;

  tmplist = string_to_list (st, ">");
  tmplist = string_to_list (list_get(tmplist,0), "<");

  return ( list_get(tmplist,1) );

}




void
PrintRequirements (string use_case_name, string action)
{
  string test_unit_id;
  int j, count2;
  node scenario;
  node nodevar;
  string query = "node[UmlUseCase && name=='${use_case_name}']";


  nodevar = find_by_query(query);


  OUTPUTTEXT = OUTPUTTEXT + "\n" + begincomment + endcomment;
  OUTPUTTEXT = OUTPUTTEXT + "\n" + begincomment + "REQUIREMENTS"  + endcomment;
  OUTPUTTEXT = OUTPUTTEXT + "\n" + begincomment + endcomment;
 

  ListRequirements( "Use Case", nodevar.id, use_case_name);



  count2 = list_count (ScenarioList);
  for (j=0;j<count2;j=j+1)
    {
      scenario = list_get(ScenarioList,j);

      if (action == scenario.name )
          ListRequirements( "Scenario", scenario.id, scenario.name);
    }

}


void
ListRequirements(string rqttype, int id, string name)
{

  string query;
  int i,count;
  item itemvar1, itemvar2;
  note notevar;
  list itemlist;
  string rqts="";
  string ReqType="";

  string cell1, cell2, cell3;

  string q1 = "item[REQAnalysisAssignmentItem && ${id} == obj_id]";

  string q2 = "note [REQDefinitionNote && node[REQRequirement && " +
               "name=='${itemvar1.value}'] ]";
  string q3 = "item [REQInformalNameItem && note_id==${notevar.id}]";

  itemlist = list_select(q1);

  count = list_count (itemlist);



  if (count == 0)
    {
      OUTPUTTEXT = OUTPUTTEXT + "\n" + begincomment + "There are no allocated " + rqttype + " Requirements."  + endcomment;
      OUTPUTTEXT = OUTPUTTEXT + "\n" + begincomment;
      return;
    }
  else
    {
      OUTPUTTEXT = OUTPUTTEXT + "\n" + begincomment + "There are " + count + " allocated " + rqttype + " Requirements."  + endcomment;
      OUTPUTTEXT = OUTPUTTEXT + "\n" + begincomment;

    }
  for (i=0;i<count;i++)
    {
      itemvar1 = list_get(itemlist, i);
      cell1 = itemvar1.value;

      notevar = find_by_query(q2);

      if (notevar != NULL)
         {
           itemvar2 = find_by_query(q3);
           cell2 = itemvar2.value;
           cell3 = notevar.desc;
         }
      else
         {
           cell2 = "";
           cell3 = "NO DESCRIPTION";
         }
        

      OUTPUTTEXT = OUTPUTTEXT + "\n" + begincomment + "Requirement: " + cell1 + "; " + cell2  + endcomment;
      OUTPUTTEXT = OUTPUTTEXT + "\n" + begincomment + cell3  + endcomment;
      OUTPUTTEXT = OUTPUTTEXT + "\n" + begincomment;
 
    }


}
