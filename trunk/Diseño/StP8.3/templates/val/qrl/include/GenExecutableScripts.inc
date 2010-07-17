


string OUTPUTPATH = current_projdir() + current_system() + "/test_files/";
string OUTPUTDIR;
string FILETEXT="";

set DataItemsDataTypes;
string ActorIDs;


#include "qrl/tsdl_extract.inc"
#include "qrl/basic_textract_dt.inc"
#include "qrl/include/error.inc"

#include "val/qrl/include/TConfig.inc"
#include "val/qrl/include/TUseCase.inc"
#include "val/qrl/include/val_string_globals.inc"



void
CreateExecuteScript(string sdfpath, string scriptpath, string id, string scenarioname, string use_case_name, string tool)
{
  string outputfile =  scriptpath + "/CreateExecuteScript.cdf";
  OUTPUTDIR = scriptpath;

  FILETEXT="";
  Make_CDF_Header1(use_case_name, scenarioname, tool);
  Make_CDF_Case1(scenarioname, tool);
  Make_CDF_Footer1(use_case_name, scenarioname, tool);

  WriteFileText (outputfile, FILETEXT);

  FormatTestCases (sdfpath, outputfile, id, scriptpath);
}


void
Make_CDF_Header1(string use_case_name, string scenarioname, string tool)
{

  FILETEXT=FILETEXT + "\n";
  FILETEXT=FILETEXT + "/*  Case Description File: CreateExecuteScript.cdf\n";
  FILETEXT=FILETEXT + "** Software Under Test (SUT): " + current_system()  + "\n";
  FILETEXT=FILETEXT + "*/\n";

  FILETEXT=FILETEXT + "BEGIN {	open report   \"CreateTestScript.csh\";\n";
  FILETEXT=FILETEXT + "   print \"#!/bin/csh\";    \n";
  FILETEXT=FILETEXT + "   print \"# Software Description File: \" SDF_NAME \" (\" SDF_DATE \")\";\n";
  FILETEXT=FILETEXT + "	print \"# \" ;\n";

  if (tool == "validator")
    {
      PrintDiagScriptCommand("TInitScriptCommand", DiagID(use_case_name),  "OMTDiagram", "Initialization");
      PrintScriptCommand("TInitScriptCommand", use_case_name, "OMTUseCase", "Initialization");
      PrintScriptCommand("TInitScriptCommand", scenarioname, "OMTScenarioName", "Initialization");
    }

  FILETEXT=FILETEXT + "	print \"# \" ;  }\n";
  FILETEXT=FILETEXT + "\n";


}


void 
Make_CDF_Case1(string actionname, string tool)
{


  FILETEXT=FILETEXT + "\n";
  FILETEXT=FILETEXT + "CASE  {	\n";


  FILETEXT=FILETEXT + " if (ACTION_NAME == \"" + actionname + "\" )\n"; 
  FILETEXT=FILETEXT + "   {\n";

  FILETEXT=FILETEXT + " print \"# -------------- Test Case \"  CASE_NAME \" \" ;\n";
  FILETEXT=FILETEXT + "   print \"echo \"\"  \"\" \"; \n";
  FILETEXT=FILETEXT + "   print \"echo \"\"  \"\" \"; \n";
  FILETEXT=FILETEXT + "   print \"echo \"\"  \"\" \"; \n";
  FILETEXT=FILETEXT + 
      "   print \"echo \"\"#################################\"\" \";\n";
  FILETEXT=FILETEXT + 
      "   print \"echo \"\"#################################\"\" \";\n";
  FILETEXT=FILETEXT + "   print \"echo \"\"  \"\" \"; \n";
  FILETEXT=FILETEXT + "   print \"echo \"\"  \"\" \"; \n";
  FILETEXT=FILETEXT + " print \"echo   Test Case \"  CASE_NAME \" \" ;\n";
  FILETEXT=FILETEXT + " print \"setenv   TestCase \"  CASE_NAME \" \" ;\n";
  FILETEXT=FILETEXT + " print \"setenv   TestCaseNumber \"  CASE_INDEX \" \" ;\n";
  FILETEXT=FILETEXT + "   print \"echo \"\"  \"\" \"; \n";

  FILETEXT=FILETEXT + " if (IS_EXERCISED == \"y\" )\n"; 
  FILETEXT=FILETEXT + "   print \"echo   Exercises \"  ACTION_NAME \" \" ;\n";
  FILETEXT=FILETEXT + " else\n"; 
  FILETEXT=FILETEXT + "   print \"echo   Does Not Exercise \"  ACTION_NAME \" \" ;\n";

  FILETEXT=FILETEXT + "   print \"echo \"\"  \"\" \"; \n";
  FILETEXT=FILETEXT + "	print \"# \" ;\n";
  FILETEXT=FILETEXT + "          foreach USES_INDEX {\n";
  FILETEXT=FILETEXT + "           print \"echo   Probing \"  USES_NAME  \" with \"  USES_DOMAIN  \" value \"  USES_VALUE;\n";
  FILETEXT=FILETEXT + "           print  \"setenv \" USES_NAME \" \" USES_VALUE;\n";
  FILETEXT=FILETEXT + "          }\n";
  FILETEXT=FILETEXT + "	print \"# \" ;\n";
  FILETEXT=FILETEXT + "   print \"echo \"\"  \"\" \"; \n";
  FILETEXT=FILETEXT + "   print \"echo \"\"  \"\" \"; \n";

  if (tool == "validator")
   {
     PrintScriptCommand("TScriptCommand", actionname, "OMTScenarioName", "Script");
   }
  FILETEXT=FILETEXT + "	print \"source EventTrace_Script.csh\";\n";

  FILETEXT=FILETEXT + "   print \"echo \"\"  \"\" \"; \n";
  FILETEXT=FILETEXT + "   print \"echo \"\"  \"\" \"; \n";
  FILETEXT=FILETEXT + "	print \"# \" ;\n";
  FILETEXT=FILETEXT + "	print \"# \" ;\n";

  FILETEXT=FILETEXT + "   }\n";

  FILETEXT=FILETEXT + "}\n";

}


void
Make_CDF_Footer1(string use_case_name, string scenarioname, string tool)
{

  FILETEXT=FILETEXT + "\n";

  FILETEXT=FILETEXT + "END   {\n";


  if (tool == "validator")
   {
     PrintScriptCommand("TTermScriptCommand", scenarioname, "OMTScenarioName", "Termination");
     PrintScriptCommand("TTermScriptCommand", use_case_name, "OMTUseCase", "Termination");
     PrintDiagScriptCommand("TTermScriptCommand", DiagID(use_case_name), "OMTDiagram", "Termination");
   }


  FILETEXT=FILETEXT + "   	close report ; }\n";
  FILETEXT=FILETEXT + "\n";

}

void
PrintScriptCommand(string ScriptCommand, string node_name, string node_type, string caption)
{

  string query = "note[${ScriptCommand} && " + 
                "node[${node_type} && name == '${node_name}']]";
  note notevar;
  string outputfile = OUTPUTDIR + "/" + node_type + "_" + caption + ".csh";
  string tmp;

  notevar = find_by_query(query);

  if (notevar != NULL)
    {
      if (notevar.desc != "")
        {
          FILETEXT=FILETEXT + "	print \"# \" ;\n";
          FILETEXT=FILETEXT + "	print \"# " + node_type + " " + caption + "\"; \n";
          FILETEXT=FILETEXT + "	print \"# \" ;\n";
          FILETEXT=FILETEXT + "	print \"source " +  node_type + "_" + caption + ".csh\" ;\n";
          FILETEXT=FILETEXT + "	print \"# \" ;\n";
          FILETEXT=FILETEXT + "	print \"# \" ;\n";

          tmp = "#\n";
          tmp = tmp + "#\n";
          tmp = tmp + "# " + node_type + " " + caption + "\n";

          tmp = tmp + "#\n";
          tmp = tmp + "#\n";
          tmp = tmp + "\n" + notevar.desc + "\n";
          tmp = tmp + "#\n";
          tmp = tmp + "#\n";
          tmp = tmp + "#\n";

          WriteFileText ( outputfile,  tmp);
        }
    }


}


int
DiagID(string usecasename)
{
   string query = "file[OMTDiagram && node_refs[node[ OMTUseCase " +
               " && name=='${usecasename}']]]";

   file filevar;

   filevar = find_by_query(query);

   if (filevar != NULL)
      return (filevar.id);
   else
      return (0);
}


void
PrintDiagScriptCommand(string ScriptCommand, int diag_id, string file_type, string caption)
{

  string query = "note[${ScriptCommand} && file[${diag_id}]]";
  note notevar;
  string outputfile = OUTPUTDIR + "/" + file_type + "_" + caption + ".csh";
  string tmp;


  if (diag_id == 0)
     return;

  notevar = find_by_query(query);

  if (notevar != NULL)
    {
      if (notevar.desc != "")
        {
          FILETEXT=FILETEXT + "	print \"# \" ;\n";
          FILETEXT=FILETEXT + "	print \"# " + file_type + " " + caption + "\"; \n";
          FILETEXT=FILETEXT + "	print \"# \" ;\n";
          FILETEXT=FILETEXT + "	print \"source " +  file_type + "_" + caption + ".csh\" ;\n";
          FILETEXT=FILETEXT + "	print \"# \" ;\n";
          FILETEXT=FILETEXT + "	print \"# \" ;\n";

          tmp = "#\n";
          tmp = tmp + "#\n";
          tmp = tmp + "# " + file_type + " " + caption + "\n";
          tmp = tmp + "#\n";
          tmp = tmp + "#\n";
          tmp = tmp + "\n" + notevar.desc + "\n";
          tmp = tmp + "#\n";
          tmp = tmp + "#\n";
          tmp = tmp + "#\n";

          WriteFileText ( outputfile,  tmp);
        }
    }


}




void
WriteFileText (string outputfile, string FILETEXT)
{

  write_file (outputfile, FILETEXT);


}



void 
FormatTestCases(string outputdir, string CDFFILE, string id, string dest )
{

  string system_command;
  if (CheckForSDF(outputdir, "TDDB"))
     { 
       tdriverPrepare (id, CDFFILE, "prepare.tmp");
       system_command = "cp " + outputdir + "/prepare.tmp/* " +
                        dest  + "/.";

       system (system_command);
     }
  else
      message ("TDDB file for " + id + " does not exist.");


}


void
CreateActionScript( string scriptpath, string actionname, int id, string use_case_name, string tool )
{
  
  string actionscriptfile =  scriptpath +  "/EventTrace_Script.csh";

  FILETEXT="";

  AddScriptHeader(use_case_name, actionname);
  
  if (tool == "validator")
    {

      AddConstraint(id, "OMTEventConstraintBefore", "Initialization", "TInitScriptCommand");
      AddEventTraceScripts(id, use_case_name);
      AddConstraint(id, "OMTEventConstraintAfter", "Termination", "TTermScriptCommand");
    }

  FILETEXT=FILETEXT + "#\n";
  FILETEXT=FILETEXT + "#\n";
  FILETEXT=FILETEXT + "echo \"  \"\n";

  WriteFileText (actionscriptfile, FILETEXT);


}



void
AddScriptHeader(string testunitname, string actionname)
{

  FILETEXT=FILETEXT + "#!/bin/csh\n";
  FILETEXT=FILETEXT + "#\n";
  FILETEXT=FILETEXT + "############################################\n";
  FILETEXT=FILETEXT + "#\n";
  FILETEXT=FILETEXT + "#          Executable Test Script\n";
  FILETEXT=FILETEXT + "#\n";
  FILETEXT=FILETEXT + "#\n";
  FILETEXT=FILETEXT + "#    Test Unit: " + testunitname + "\n";
  FILETEXT=FILETEXT + "#\n";
  FILETEXT=FILETEXT + "#    Action: " + actionname + "\n";
  FILETEXT=FILETEXT + "#\n";
  FILETEXT=FILETEXT + "############################################\n";
  FILETEXT=FILETEXT + "#\n";
  FILETEXT=FILETEXT + "#\n";

}




void
AddEventTraceScripts(int scenarioid, string use_case_name)
{
  list inputevents;
  link_ref linkrefvar;
  int event_id, i,count;
  string eventname;
  string query = "note[TScriptCommand && obj_id == ${event_id}]";
  note notevar;


  inputevents = list_create ("string", 0);

  list_clear(inputevents);

  inputevents = GetInputEvents("Input", scenarioid, ActorIDs);

  count = list_count(inputevents);

  for (i=0; i<count; i=i+1)
    {
       linkrefvar = list_get(inputevents, i);
       eventname = GetEventName(linkrefvar.link_id);
       event_id = GetEventID(linkrefvar.link_id);



       notevar = find_by_query(query);

       if (notevar != NULL)
         {
           if (notevar.desc != "")
             GetNoteDescription( notevar.desc , "Event", eventname);

         }

    }

}


void
AddConstraint(int id, string notetype, string name, string commandtype)
{

  string query = "node_ref[node_id == ${id} && file[OMTDiagram] ]";

  string query1 = "cntx [${notetype} && name == '${name}' " +
                  " && cntx_refs[file_id == ${noderefvar.file_id} ] ]";

  string query2 = "note[${commandtype} && obj_id == ${cntxvar.id}]";


  node_ref noderefvar;
  cntx cntxvar;
  note notevar;



  if (name == "Termination")
    {
      FILETEXT=FILETEXT + "#\n";
      FILETEXT=FILETEXT + "#\n";
      FILETEXT=FILETEXT + "Termination:\n";
      FILETEXT=FILETEXT + "#\n";
      FILETEXT=FILETEXT + "#\n";
    }



  noderefvar = find_by_query(query);
  cntxvar = find_by_query(query1);
  if (cntxvar == NULL)
    return;

  notevar = find_by_query(query2);

  if (notevar != NULL)
    {
      if (notevar.desc != "")
        GetNoteDescription( notevar.desc, notetype, name);
    }
}



void
GetNoteDescription ( string desc, string typecommand, string name)
{
  int i,count;
  list desclist;
  string line;

  desclist = string_to_list (desc, "\n");
  count = list_count(desclist);

  FILETEXT=FILETEXT + "#\n";
  FILETEXT=FILETEXT + "# Shell command for " + typecommand  
      + "\n";
  FILETEXT=FILETEXT + "#    " + name  + "\n";
  FILETEXT=FILETEXT + "#\n";

      

  FILETEXT=FILETEXT + desc + "\n";

  FILETEXT=FILETEXT + "#\n";
  FILETEXT=FILETEXT + "#\n";
  FILETEXT=FILETEXT + "#\n";
  FILETEXT=FILETEXT + "#\n";
}

int
GetEventID( int link_id)
{
  link linkvar;
  string query = "link [ id == ${link_id} ]";

  linkvar = find_by_query (query);

  if (linkvar == NULL)
    return (0);
  else 
    return (linkvar.id);


}

string
GetEventName( int link_id)
{
  link linkvar;
  item itemvar;

  string query1 = "item [ TScriptVariable && obj_id ==  ${link_id} ]";
  string query2 = "link [ id == ${link_id} ]";

  itemvar = find_by_query (query1);
  linkvar = find_by_query (query2);

  if (itemvar == NULL && linkvar == NULL)
     return (NULL);

  if (itemvar != NULL && itemvar.value != "")
    return (RemoveSpaces(itemvar.value) );
  else if (string_length (linkvar.name) >20 )
    return ( RemoveSpaces(string_extract(linkvar.name, 0, 20)) );
  else
    return (RemoveSpaces(linkvar.name));

}



string
GetActorIDList(string usecase)
{
  string q1, q2, q3;
  string actorids="0";
  string SEP = " || ";

  node nodevar1, nodevar2, nodevar3;

  q1 = "node [ OMTUseCase && name == '${usecase}' ]";

  q2 = "node [OMTUseCaseActor && node_refs && " +
       "( out_links [ link_refs && OMTUseCaseInteraction && " +
       "to_node_id == ${nodevar1.id}] || in_links [ link_refs && " +
       "OMTUseCaseInteraction && from_node_id == ${nodevar1.id} ]  ) ]";

  q3 = "node [ OMTClass && name == '${nodevar2.name}' ]";

  nodevar1 = find_by_query(q1);

  for_each_in_select (q2, nodevar2)
    {
       nodevar3 = find_by_query(q3);
       if (nodevar3 != NULL)
         {
           if (actorids == "0")
             actorids = "" + nodevar3.id;
           else
             actorids = actorids + SEP + nodevar3.id;
         }
    }

  return (actorids);
}


list
GetInputEvents(string type, int scenario_id, string actorids)
{
 
  string linkref_query, noderef_query;
  node_ref nr;

  if (actorids == "")
      actorids = "0";

  noderef_query = "node_ref [node_id == ${scenario_id} ] ";

  nr = find_by_query (noderef_query);

  linkref_query = "link_ref [ file_id == ${nr.file_id} && 
           link [ OMTEventInstance   &&
           (from_node[${actorids}] || to_node[${actorids}]) ] ] sort by ycoord";

  return ( list_select (linkref_query) );

}




void
InitializeActionDir(string outputdir, string scenarioname)
{

  if (CheckForSDF(outputdir, scenarioname))
     return;
  else
  {
     //system ("mkdir " + outputdir + "/" + scenarioname ) ;
     CreateDirectory(outputdir + "/" + scenarioname);
  }

}


void
InitializeScriptsPath(string outputdir, string fname)
{

  if (CheckForSDF(outputdir, fname))
     return;
  else
  {
     //system ("mkdir " + outputdir + "/" + fname ) ;
     CreateDirectory(outputdir + "/" + fname);
  }
}


list
GetActionList(string sdfpath)
{
  string filetext;
  int i,count;
  list textlist;
  string textline, actionname;
  list tmplist;

  tmplist = list_create ("string", 0);

  if (CheckForSDF(sdfpath, "sdf"))
     {
        filetext = read_file (sdfpath + "/sdf");
        filetext = string_search_and_replace(filetext, "
", "");
        textlist = string_to_list (filetext, "\n");
        count = list_count (textlist);
        for (i=0; i<count; i++)
          {
            textline = list_get (textlist,i);
            if ( string_find ( textline, 0, "Action") != string_length (textline) )
              {
                 actionname = string_extract (textline, 7, string_length (textline)-7 );
                 list_append (tmplist, actionname );
              }
          }

     }
  else
     {
        message ("sdf does not exist");
     }

return (tmplist);

}
