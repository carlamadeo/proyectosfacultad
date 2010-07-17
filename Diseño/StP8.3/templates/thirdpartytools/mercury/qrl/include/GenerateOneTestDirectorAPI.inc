
void 
GenerateTestPlan ()
{


  string path;
  string inputfile;

  string testcases;

  ScenarioList = GetScenarioList(use_case_name);

  message ("Creating API  \"testplan.c\"");


  string SCRIPTPATH = TDDirectory + "/" + TDDataBase + "/Tests/" + test_unit_id;
  MakeDir(SCRIPTPATH);

  TDFILE = SCRIPTPATH + "/testplan.c";


  OUTPUTDIR = OUTPUTPATH + test_unit_id;

//  path = OUTPUTDIR + "/UseCaseTestScripts";
  path = OUTPUTDIR;
  inputfile = path + "/testcases.rpt";

  if (!CheckForSDF(OUTPUTDIR, "TDDB"))
    {
      print_error ("TDDB for " + test_unit_id + " does not exist.");
      return;
    }

  FormatTestCaseData(test_unit_id);

  if (CheckForSDF(path, "testcases.rpt"))
     testcases = read_file (inputfile);
  else
    {
      print_error ("testcases.rpt for " + test_unit_id + " does not exist.");
      return;
    }

  testcaselist = string_to_list(testcases, "\n");

  if (TDPassword == "NONE")
    TDPassword = "";


  PrintConstants(use_case_name);

  PrintHeader1();

  PrintUseCaseInformation(use_case_name);

  PrintScenarioInformation(testcaselist, use_case_name);

  PrintHeader2();

  PrintTemplate();

  print_message ("Output file written to " + TDFILE);
  write_file(TDFILE, OUTPUTTEXT);

}


void PrintTemplate()
{
  OUTPUTTEXT = OUTPUTTEXT +  GetAPIFunctions();  

}



void PrintConstants(string use_case_name)
{
  OUTPUTTEXT = OUTPUTTEXT + "const char *USECASENAME = \"" + use_case_name + "\";\n";

  OUTPUTTEXT = OUTPUTTEXT + "const char *DATABASENAME = \"" + TDDataBase + "\";\n";

  OUTPUTTEXT = OUTPUTTEXT + "const char *USERNAME = \"" + TDUserName + "\";\n";
  OUTPUTTEXT = OUTPUTTEXT + "const char *PASSWORD = \"" + TDPassword  + "\";\n";
  OUTPUTTEXT = OUTPUTTEXT + "const char *SERVER =  \"" + TDServer + "\";\n";

  OUTPUTTEXT = OUTPUTTEXT + "\n\n";
  OUTPUTTEXT = OUTPUTTEXT + "char *TEXT;\n";
  OUTPUTTEXT = OUTPUTTEXT + "char *DESCTEXT;\n";
}

void PrintHeader1()
{

  OUTPUTTEXT = OUTPUTTEXT + "int ROOTID, SUBJECTID, NODEID, TESTID, STEPID;\n\n";

  OUTPUTTEXT = OUTPUTTEXT + "char TESTPATH[500];\n\n";


  OUTPUTTEXT = OUTPUTTEXT + "char *USECASEDESC;\n";
  OUTPUTTEXT = OUTPUTTEXT + "char *SCENARIODESC;\n";

  if (DEBUG)
    OUTPUTTEXT = OUTPUTTEXT + "const int DEBUG = 0;\n";
  else
    OUTPUTTEXT = OUTPUTTEXT + "const int DEBUG = 1;\n";

  OUTPUTTEXT = OUTPUTTEXT + "int main ()\n\n";

  OUTPUTTEXT = OUTPUTTEXT + "{\n";

  OUTPUTTEXT = OUTPUTTEXT + "  int r0, s0, t0;\n\n";

  OUTPUTTEXT = OUTPUTTEXT + "  r0 = ConnectServer();\n\n";

  OUTPUTTEXT = OUTPUTTEXT + "  r0 = ConnectDB();\n\n";

  OUTPUTTEXT = OUTPUTTEXT + "  r0 = GetCategoryTreeRoot();\n\n";

  OUTPUTTEXT = OUTPUTTEXT + "  r0 = TreeCreateRoot();\n\n";

  OUTPUTTEXT = OUTPUTTEXT + "  r0 = TreeGetRoot();\n\n";

  OUTPUTTEXT = OUTPUTTEXT + "  r0 = TreeGetSubjectIdFromPath();\n\n";

}



void
PrintUseCaseInformation(string usecasename)
{
  string tmp;
  string desc = GetUseCaseDescription(use_case_name);

  tmp = string_search_and_replace (desc, "\"", "@@@");
  desc = string_search_and_replace (tmp, "@@@", "\\\"");

  tmp = string_search_and_replace (desc, "\n", "@@@");
  desc = string_search_and_replace (tmp, "@@@", "\\n");

  OUTPUTTEXT = OUTPUTTEXT + "  USECASEDESC = \"" + desc + "\";\n\n";

  OUTPUTTEXT = OUTPUTTEXT + "  r0 = TreeAddNode(USECASENAME, USECASEDESC);\n\n";

  OUTPUTTEXT = OUTPUTTEXT + "  r0 = CreateCycle (\"" + usecasename + "\");\n\n";

}


string
GetUseCaseDescription(string use_case_name)
{

  string desc = "";
  string q1 = "node[UMLUseCase && name == '${use_case_name}']";
  string q2 = "note[UmlUseCaseDefinition && obj_id == ${nodevar.id} ]";

  node nodevar;
  note notevar;

  nodevar = find_by_query (q1);

  notevar = find_by_query (q2);

  if (notevar != NULL)
    desc = notevar.desc;

  return (desc);

}


void
PrintHeader2()
{

  OUTPUTTEXT = OUTPUTTEXT + "  r0 = DisconnectDB();\n";

  OUTPUTTEXT = OUTPUTTEXT + "}\n\n";

}



void
PrintScenarioInformation(list testcaselist, string use_case_name)
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
  string firsttest;

  validlist = list_create("string",0);

  string testcasedata;
  list testcaseinfo;

  string startstate, endstate;



  OUTPUTTEXT = OUTPUTTEXT + "  ROOTID = NODEID;\n\n";

  count = list_count(testcaselist);

  firstaction = True;

  for (i=0;i<count;i++)
    {

      testcasedata = list_get(testcaselist,i);

      testcaseinfo = string_to_list(testcasedata, " ");

      action = list_get(testcaseinfo, 0);
      testcase = list_get(testcaseinfo, 1);

      if (i==0)
        {
          pastaction = action;
          firsttest = testcase;
        }

      if (pastaction != action)
        {
           PrintScenario (pastaction, firsttest, use_case_name);

           list_clear(validlist);
           firsttest = testcase;
           firstaction = False;
        }


      list_append (validlist, testcase);

      pastaction = action;
    }

   PrintScenario(action, firsttest, use_case_name);

}


void
PrintScenario(string scenario, string testcase, string use_case_name)
{
  string tmp;

  const string outputpath = SCRIPTPATH + "/" + scenario;

  print_message ("  " + scenario);

  MakeDir (outputpath);


  string desc = GetScenarioDescription(scenario);

  desc = desc + "\n\n" + GetScenarioSteps(testcase);

  tmp = string_search_and_replace (desc, "\"", "@@@");
  desc = string_search_and_replace (tmp, "@@@", "\\\"");

  tmp = string_search_and_replace (desc, "\n", "@@@");
  desc = string_search_and_replace (tmp, "@@@", "\\n");

  OUTPUTTEXT = OUTPUTTEXT + "  SCENARIODESC = \"" + desc + "\";\n\n";

  OUTPUTTEXT = OUTPUTTEXT + "  s0" + " = CreateTest(\"" + scenario + "\");\n\n";

  OUTPUTTEXT = OUTPUTTEXT + "  s0" + " = SetTestValue(\"TS_DESCRIPTION\", SCENARIODESC );\n\n";

  OUTPUTTEXT = OUTPUTTEXT + "  s0" + " = AddTestToCycle(\"" + use_case_name + "\", \"" + TDUserName + "\");\n\n";


}


string
GetScenarioDescription(string scenario)
{

  string desc = "";
  string q1 = "node[UMLScenarioInstance && name == '${scenario}']";
  string q2 = "note[UmlScenarioDefinition && obj_id == ${nodevar.id} ]";

  node nodevar;
  note notevar;

  nodevar = find_by_query (q1);

  notevar = find_by_query (q2);

  if (notevar != NULL)
    desc = notevar.desc;

  return (desc);

}


string
GetScenarioSteps(string testcase)
{
  int count = 0;
  string st;
  boolean done = False;
  string description="";

  list tmplist;

  int currentstep = 0;
  int nextstep = 1;
  string steptype;
  string command;
  string steptext = ""; 
  string dataitem, datavalue;
  list testcasedata = GetTestCaseData (testcase);

  if (count >= list_count(testcasedata))
    done = True;


  while (!done)
    {


       st = list_get(testcasedata,count);

       if ( st == "INPUTEVENT" )
         {
            if (currentstep == nextstep )
              {
                 ++nextstep;
               }

            ++currentstep;

            ++count;
            tmplist = string_to_list (list_get(testcasedata, count), " ");
            list_delete(tmplist, 3);
            steptext = list_to_string (tmplist, " " );
            description = description + "\n\nStep: " + nextstep + ": " + steptext;


            steptype = "input";

         }
       else if ( st == "DATAIN" )
         {

            ++count;
            dataitem = list_get(testcasedata, count);
            ++count;
            datavalue = list_get(testcasedata, count);

            description = description + "\n  Set " + dataitem;

//            PARAMETER = PARAMETER + ", " + dataitem;
         }

       else if ( st == "OUTPUTEVENT" )
         {
            if (currentstep == nextstep )
              {
                 ++nextstep;
              }

            ++currentstep;

            ++count;

            tmplist = string_to_list (list_get(testcasedata, count), " ");
            list_delete(tmplist, 3);
            steptext = list_to_string (tmplist, " " );
            description = description + "\n\nStep: " + nextstep + ": " + steptext;


            steptype = "output";
         }

       else if ( st == "NORESULTS" )
         {
            description = description + "\n  No Expected Results Given";
         }

       else if ( st == "FIXEDRESULTS" )
         {
            description = description + "\n  Has Fixed Output:";
            description = description + "\n" + PrintFixedResults(steptext);
         }

       else if ( st == "GENERATEDRESULTS" )
         {
            ++count;
            command = list_get(testcasedata, count);
            tmplist = string_to_list(command, " " );
            description = description +  "\n  Oracle Command: " + list_get(tmplist,0);
         }
         
       ++count;
       if (count >= list_count(testcasedata))
          done = True;
    }

  return (description);

}




string
PrintExpectedResults(string command)
{
  string tmp;
  string dataitem, datavalue, datavalue1;
  string datatype, datatypename;
  list tmplist2;

  string sysdir = current_projdir() + current_system() + "/";

  tmp = string_search_and_replace (command, "\"", "@@@");
  command = string_search_and_replace (tmp, "@@@", "\\\"");

  system ("sh -c '" + ORACLESDIR + command + "'   > " + sysdir + "output");

  if(write_file_access(sysdir + "output"))
    {
            
      datavalue = read_file(sysdir + "output");

      system ("rm " + sysdir + "output");

    }
  else
    {
      datavalue = "UNKNOWN";
    }

  return (datavalue);
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
       print_error (testcase + " does not exist.");
       return;
     }

  textlist = string_to_list(filetext,"\n");

  return (textlist);

}


string
StripTheQuotes(string datavalue)
{
  string tmp;

  tmp = string_search_and_replace(datavalue, "\"", "");

  return (tmp);
}
