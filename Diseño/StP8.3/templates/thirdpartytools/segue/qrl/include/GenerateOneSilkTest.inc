
string DATASEPARATOR = ";";

// Constants used by the data file

const string begincomment = "//  " ;
const string endcomment = "";
const string indent = "    ";
const string halfindent = "  ";


  list ScenarioList;

  string OUTPUTPATH = current_projdir() + current_system() + "/test_files/";

  string SCRIPTPATH = current_projdir() + current_system() + "/testscpt_files/segue/";

  string SCRIPTDIR;

  string SORACLESDIR = OUTPUTPATH + "SCRIPT_ORACLES/";
  string ORACLESDIR = OUTPUTPATH + "ORACLES/";

  string TESTDATA="";
  string OUTPUTTEXT="";
  string OUTPUTDIR;

  string DESC = "";
  string PARAMETER = "";
  string EVENTS = "";

  string DESIGNSTEP1 = "";
  string DESIGNSTEP2 = "";
  string DESIGNSTEP3 = "";
  string DESIGNSTEP4 = "";
  string DESIGNSTEP5 = "";



  set ACTORSET;

void 
GenerateOneSilkTest ()
{

  string scriptfile;
  string test_unit_id;

  string filetext;

  string testcases;
  string path;
  string inputfile;
  list testcaselist;

  ACTORSET = GetActors(use_case_name);

  test_unit_id = Get_T_test_unit_id(use_case_name, "");

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
     testcases = read_file (inputfile);
  else
    {
      message ("testcases.rpt for " + test_unit_id + " does not exist.");
      return;
    }


  MakeDir(SCRIPTPATH);

  MakeDir(SCRIPTPATH + "/" + use_case_name);

  testcaselist = string_to_list(testcases, "\n");

  message ("");
  message ("");
  message ("Generating scripts for Use Case: " + use_case_name);

  PrintScenarioInformation(testcaselist, use_case_name);


 
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


  string ScenarioPath = SCRIPTPATH + "/" + use_case_name;

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
           startstate = GetStateName("StartState", pastaction);
           endstate = GetStateName("EndState", pastaction);
           if (startstate == "NONE" || endstate == "NONE")
             {
               message ("ERROR - Scenario " + pastaction + " states are not defined");
               continue;
             }
           PrintScenario (pastaction, firsttest, startstate, endstate, ScenarioPath);
           GenerateStateFile("StartState", action, startstate);
           GenerateStateFile("EndState", action, endstate);
//           PrintCases(validlist, pastaction, startstate, endstate);

           list_clear(validlist);
           firsttest = testcase;
           firstaction = False;
        }


      list_append (validlist, testcase);

      pastaction = action;
    }

   startstate = GetStateName("StartState", action);
   endstate = GetStateName("EndState", action);
   if (startstate == "NONE" || endstate == "NONE")
     {
        message ("ERROR - Scenario " + action + " states are not defined");
        return;
     }

   PrintScenario(action, firsttest, startstate, endstate, ScenarioPath);
   GenerateStateFile("StartState", action, startstate);
   GenerateStateFile("EndState", action, endstate);
//   PrintCases(validlist, action, startstate, endstate);


}


void
PrintScenario(string scenario, string testcase, string startstate, string endstate, string scenariopath)
{
  string tmp;

  string filename, filepath;

  message ("  " + scenario);

  filename = scenariopath + "/" + scenario + ".t";

  GenerateScenarioFile(filename, scenario, testcase, startstate, endstate);


}



void
GenerateScenarioFile(string filename, string scenario, string testcase, string startstate, string endstate)
{

  string tmp = "";
  int i;
  int count;
  string statvar;

  string events =  GetScenarioEvents(testcase);
  list tmplist = string_to_list (PARAMETER, " " );

  list eventlist = string_to_list (EVENTS, " " );
  int eventcount = list_count(eventlist);

  tmp = tmp + "//" + GetScenarioDescription(scenario) + "\n";
  tmp = tmp + "//" + "\n";
  tmp = tmp + "//" + "\n";

  tmp = tmp + "// List of External Variables" + "\n";
  tmp = tmp + "//" + "\n";

  count = list_count(tmplist);

  for (i=0;i<count;i++)
    {
      tmp = tmp + "   extern " + list_get(tmplist, i) + ";\n";
    }


  tmp = tmp + "//" + "\n";
  tmp = tmp + "//" + "\n";


  tmp = tmp + "function " + scenario + "(out status)" + "\n";
  tmp = tmp + "{" + "\n";

  tmp = tmp + "//" + "\n";
  tmp = tmp + "// Define Local Variables" + "\n";

  for (i=0;i<eventcount;i++)
    {
        tmp = tmp + "   auto s" + to_string (i+1) + ";" + "\n";
    }

  tmp = tmp + "//" + "\n";
  tmp = tmp + "// Search Path" + "\n";
  tmp = tmp + "   setvar ( \"searchpath\", \"";
  tmp = tmp + SCRIPTPATH + "States;";
  tmp = tmp + SCRIPTPATH + "Events\" );" + "\n";

  tmp = tmp + "//" + "\n";
  tmp = tmp + "//" + "\n";

  tmp = tmp + "// Load Files" + "\n";
  tmp = tmp + "//" + "\n";

  tmp = tmp + "   load ( \"" + startstate + "\" );\n";
  tmp = tmp + "   load ( \"" + endstate + "\" );\n";


  for (i=0;i<eventcount;i++)
    {
      tmp = tmp + "   use ( \"" + list_get(eventlist, i) + ".inc\" );\n";
    }


  tmp = tmp + "//" + "\n";
  tmp = tmp + "//" + "\n";

  tmp = tmp + "// Start State" + "\n";
  tmp = tmp + "   " + startstate + "();\n";
  tmp = tmp + "//" + "\n";

  tmp = tmp + "//" + events + "\n";

  tmp = tmp + "//" + "\n";
  tmp = tmp + "//" + "\n";
  tmp = tmp + "// End State" + "\n";
  tmp = tmp + "   " + endstate + "();\n";
  tmp = tmp + "//" + "\n";
  tmp = tmp + "//" + "\n";


  tmp = tmp + "// Unload Files" + "\n";

  tmp = tmp + "   unload ( \"" + startstate + "\" );\n";
  tmp = tmp + "   unload ( \"" + endstate + "\" );\n";

  for (i=0;i<eventcount;i++)
    {
      tmp = tmp + "   unload ( \"" + list_get(eventlist, i) + "\" );\n";
    }


  tmp = tmp + "//" + "\n";
  tmp = tmp + "//" + "\n";

  for (i=0;i<eventcount;i++)
    {
      if (i == 0)
        statvar = "s" + to_string (i+1);
      else
        statvar = statvar + " + s" + to_string (i+1);
    }

  tmp = tmp + "   status = " + statvar + ";\n";
  tmp = tmp + "\n";
  tmp = tmp + "}" + "\n";


  message ("    script written to " + scenario + ".t");
  write_file(filename, tmp);



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

  desc = string_search_and_replace (desc, "\n", "\n//");

  return (desc);

}


string
GetScenarioEvents(string testcase)
{
  int count = 0;
  string st;
  boolean done = False;
  string description="";
  string event;
  list tmplist;

  int currentstep = 0;
  int nextstep = 1;
  string steptype;
  string command;
  string steptext = ""; 
  string dataitem, datavalue;
  list testcasedata = GetTestCaseData (testcase);

  PARAMETER = "test_status";

  list eventparameters;

  eventparameters = list_create("string", 0);

  if (count >= list_count(testcasedata))
    done = True;


  while (!done)
    {


       st = list_get(testcasedata,count);

       if ( st == "INPUTEVENT" )
         {
            if (currentstep == nextstep )
              {

                 PrintEventFile(event, eventparameters, nextstep);
                 description = description + EventDescription(nextstep, steptext, event, eventparameters);

                 list_clear(eventparameters);
                 ++nextstep;
               }

            ++currentstep;

            ++count;
            tmplist = string_to_list (list_get(testcasedata, count), " ");
            list_delete(tmplist, 3);
            steptext = list_to_string (tmplist, " " );
            event = list_get(tmplist,1);

            steptype = "input";

         }
       else if ( st == "DATAIN" )
         {

            ++count;
            dataitem = list_get(testcasedata, count);
            ++count;
            datavalue = list_get(testcasedata, count);

            PARAMETER = PARAMETER + " " + dataitem;
            list_append(eventparameters, dataitem);
         }

       else if ( st == "OUTPUTEVENT" )
         {
            if (currentstep == nextstep )
              {
                 PrintEventFile(event, eventparameters, nextstep);
                 description = description + EventDescription(nextstep, steptext, event, eventparameters);

                 list_clear(eventparameters);
                 ++nextstep;
              }

            ++currentstep;

            ++count;

            tmplist = string_to_list (list_get(testcasedata, count), " ");
            list_delete(tmplist, 3);
            steptext = list_to_string (tmplist, " " );
            event = list_get(tmplist,1);


            steptype = "output";
         }

       else if ( st == "NORESULTS" )
         {
//            description = description + "\n  No Expected Results Given";
         }

       else if ( st == "FIXEDRESULTS" )
         {
//            description = description + "\n  Has Fixed Output:";
//            description = description + "\n" + PrintFixedResults(steptext);
         }

       else if ( st == "GENERATEDRESULTS" )
         {
            ++count;
            command = list_get(testcasedata, count);
            tmplist = string_to_list(command, " " );
//           description = description +  "\n  Oracle Command: " + list_get(tmplist,0);
         }
         
       ++count;
       if (count >= list_count(testcasedata))
          done = True;
    }

  PrintEventFile(event, eventparameters, nextstep);
  description = description + EventDescription(nextstep, steptext, event, eventparameters);

  return (description);
}


string
EventDescription(int step, string eventtext, string event, list parameterlist)
{
  string functioncall = event + "( s" + step;
  string tmp = "";

  string parameter;
  int i, count;

  count = list_count(parameterlist);

  for (i=0;i<count;i++)
    {
      parameter = list_get(parameterlist, i);
      functioncall = functioncall + ", " + parameter;
    }

  functioncall = functioncall + " );";

  tmp = "\n\n// Step: " + step + ": " + eventtext + "\n";
  tmp = tmp + "   " + functioncall + "\n";

  if (step == 1)
    EVENTS = event;
  else
    EVENTS = EVENTS + " " + event;
 

  tmp = tmp + "   if ( s" + step + " == 0 )" + "\n";
  tmp = tmp + "     tl_step(\"" + event + "\", 0, \"Event " + event + " Passed\");" + "\n";
  tmp = tmp + "   else" + "\n";
  tmp = tmp + "     tl_step(\"" + event + "\", 1, \"Event " + event + " Failed\");" + "\n";

  return (tmp);
}


void
PrintEventFile(string event, list parameterlist, int step)
{
    string functiondef = event + "( out status";
    string tmp;
    string filepath = SCRIPTPATH + "/Events";

    string parameter;
    int i, count;


    MakeDir(filepath);

    count = list_count(parameterlist);

    for (i=0;i<count;i++)
      {
        parameter = list_get(parameterlist, i);
        functiondef = functiondef + ", " + parameter;
      }

  functiondef = functiondef + " )";

  tmp = "// Event " + event + "\n\n";

  tmp = tmp + "function " + functiondef + "\n";
  tmp = tmp + "{" + "\n\n";

  tmp = tmp + "  status = 0;" + "\n\n";

  tmp = tmp + "// Add event steps here" + "\n\n\n";

  tmp = tmp + "}" + "\n";

  message ("    Template file written to " + event + ".template");
  write_file(filepath + "/" + event + ".template" , tmp);



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



string
StripTheQuotes(string datavalue)
{
  string tmp;

  tmp = string_search_and_replace(datavalue, "\"", "");

  return (tmp);
}


void
GenerateStateFile(string state, string scenario, string statename)
{
  string tmp="";
  string filepath;
  list tmplist;
  int i;

  filepath = SCRIPTPATH + "/States";

  MakeDir(filepath);

  tmp = tmp + "#\n";
  tmp = tmp + "# Add Steps Here\n";
  tmp = tmp + "#\n";
  tmp = tmp + "\n";
  tmp = tmp + "\n";
  tmp = tmp + "function " + statename + "()" + "\n";
  tmp = tmp + "{" + "\n";
  tmp = tmp + "\n";
  tmp = tmp + "  return;" + "\n";
  tmp = tmp + "}" + "\n";


  message ("    Template file written to " + statename + ".template" );
  write_file(filepath + "/" + statename + ".template" , tmp);

}



string
GetStateName(string state, string scenario)
{

  item itemvar;

  string query = "item[${state} && node[UmlScenarioInstance && name == '${scenario}']]";



  itemvar = find_by_query(query);

  if (itemvar == NULL)
    {
      message ("ERROR - Scenario " + scenario + " needs a " + state );
      return ("NONE");
    }

  return ( itemvar.value );
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
      OUTPUTTEXT = OUTPUTTEXT + "\n";
      return;
    }
  else
    {
      OUTPUTTEXT = OUTPUTTEXT + "\n" + begincomment + "There are " + count + " allocated " + rqttype + " Requirements."  + endcomment;
      OUTPUTTEXT = OUTPUTTEXT + "\n";
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
        

      OUTPUTTEXT = OUTPUTTEXT + begincomment + "Requirement: " + cell1 + "; " + cell2  + endcomment;
      OUTPUTTEXT = OUTPUTTEXT + "\n" + begincomment + cell3  + endcomment;
      OUTPUTTEXT = OUTPUTTEXT + "\n\n";
 
    }


}

