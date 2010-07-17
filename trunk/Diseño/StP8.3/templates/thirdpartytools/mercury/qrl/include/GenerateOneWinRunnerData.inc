
string 
GenerateWinRunnerData (string use_case_name, string scriptpath)
{

  string scriptfile;

  string filetext;

  string testcases;
  string path;
  string inputfile;
  list testcaselist;
  string status;

  string OUTPUTDIR;


  OUTPUTDIR = OUTPUTPATH + use_case_name;


  if (!CheckForSDF(OUTPUTDIR, "TDDB"))
    {
      print_error ("TDDB for " + use_case_name + " does not exist.");
      return ("error");
    }

 

  print_message ("Generating Data for Use Case: " + use_case_name);


  string system_command; 


  system_command = 
         " tdriver.exe " +
         " -p " + current_projdir()  +
         " -s " + current_system() + 
         " -U " + use_case_name + 
         " -v " + "1"  +
         " -q " + "spreadsheet.cdf" +
         " -x " + "all" +
         " -y " + "all" + 
         " -z " + "all" +
         " -a " + scriptpath +
         " -P ";

  system ( system_command );

  status = MoveFiles(scriptpath, use_case_name);

  return (status);

}


string
MoveFiles(string scriptpath, string use_case_name)
{

  list scenarios = GetScenarioList(use_case_name);
  node scenario;
  boolean error=False;

  int i, count;
  string fname;
  string oldpath, newpath;

  count = list_count(scenarios);

  for (i=0;i<count;i++)
    {
       scenario = list_get(scenarios,i);
       
       fname = scenario.name + ".txt";
       oldpath = scriptpath + "/";
       newpath = scriptpath + "/" + scenario.name + "/";
       
       MakeDir(scriptpath + "/" + scenario.name);

       if (CheckForSDF(scriptpath, fname))
         MoveFile (oldpath, fname, newpath, fname);
      else
         {
            print_error (fname + " does not exist.");
            error = True;
         }
     }

  if (error)
    return ("error");
  else 
    return ("ok");
}



