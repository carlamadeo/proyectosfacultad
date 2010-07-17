
void 
GenerateWinRunnerFiles (string use_case_name, string scriptpath)
{

//  ACTORSET = GetActors(use_case_name);

  list scenarios = GetScenarioList(use_case_name);
  node scenario;

  int i, count;
  string fname;
  string text;

  list dataitems;

  count = list_count(scenarios);

  for (i=0;i<count;i++)
    {
      scenario = list_get(scenarios,i);
       
      dataitems = GetDataItems(scriptpath, scenario.name);

      text = MakeText (scriptpath, scenario.name, dataitems);

      fname = scriptpath + "/" + scenario.name + "/script_template";
      print_message (fname + " written");
      write_file(fname, text);

      GenerateHeaderFile(scriptpath + "/" + scenario.name, "CM");
      GenerateDataBaseFiles (scriptpath + "/" + scenario.name);

    }
}


list
GetDataItems(string scriptpath, string scenario)
{

  string fname = scriptpath + "/" + scenario + "/" + scenario + ".txt";

  string tmp = read_file (fname);

  list tmplist = string_to_list (tmp, "\n");

  tmp = list_get(tmplist, 0);

  return (string_to_list(tmp,"\t") );
    

}


string
MakeText (string scriptpath, string scenario, list dataitems)
{

  string text;

  text = MakeHeader(scriptpath, scenario);
  text = text + MakeStatic(dataitems);
  text = text + MakeBody(dataitems);

  return (text);

}

string
MakeHeader(string scriptpath, string scenario)
{
  string path = string_search_and_replace(scriptpath, "\\", "\\\\");
  path = string_search_and_replace(scriptpath, "/", "\\\\");
  string tmp="";
  tmp = tmp + "################################################################" + "\n";
  tmp = tmp + "#" + "\n";
  tmp = tmp + "# Test Variables" + "\n";
  tmp = tmp + "#" + "\n";
  tmp = tmp + "#" + "\n";
  tmp = tmp + "################################################################" + "\n";
  tmp = tmp + "static table = \"" + path + "\\\\" + scenario + "\\\\" + scenario + ".txt\""  + ";" + "\n";
  tmp = tmp + "static rc"  + ";" + "\n";
  tmp = tmp + "#" + "\n";
  tmp = tmp + "#" + "\n";
  tmp = tmp + "################################################################" + "\n";
  tmp = tmp + "#" + "\n";
  tmp = tmp + "#" + "\n";

  return (tmp);

}


string
MakeStatic(list dataitems)
{

  string tmp="";
  int i;
  int count = list_count (dataitems);

  tmp = tmp + "################################################################" + "\n";
  tmp = tmp + "#" + "\n";
  tmp = tmp + "# Test Case Data Items" + "\n";
  tmp = tmp + "#" + "\n";
  tmp = tmp + "################################################################" + "\n";
  tmp = tmp + "#" + "\n";
  tmp = tmp + "#" + "\n";
  for (i=0;i<count;i++)
     tmp = tmp + "static " + list_get(dataitems, i) + ";" + "\n";

  tmp = tmp + "#" + "\n";
  tmp = tmp + "#" + "\n";
  tmp = tmp + "################################################################" + "\n";
  tmp = tmp + "#" + "\n";
  tmp = tmp + "#" + "\n";

  return (tmp);

}


string
MakeBody(list dataitems)
{

  string tmp="";
  string di;
  int i;
  int count = list_count (dataitems);

  tmp = tmp + "################################################################" + "\n";
  tmp = tmp + "#" + "\n";
  tmp = tmp + "# Test Script Body" + "\n";
  tmp = tmp + "#" + "\n";
  tmp = tmp + "################################################################" + "\n";
  tmp = tmp + "#" + "\n";
  tmp = tmp + "#" + "\n";


  tmp = tmp + "rc = ddt_open(table);" + "\n";
  tmp = tmp + "if (rc != E_OK)" + "\n";
  tmp = tmp + "  pause (\"ERROR\");" + "\n";
  tmp = tmp + "" + "\n";
  tmp = tmp + "" + "\n";
  tmp = tmp + "do" + "\n";
  tmp = tmp + "{" + "\n";
  tmp = tmp + "" + "\n";

  for (i=0;i<count;i++)
    {
      di = list_get(dataitems, i);
      tmp = tmp +  "  " + di + " = ddt_val(table, \"" + di + "\");" + "\n";
    }

  tmp = tmp + "#" + "\n";
  tmp = tmp + "#  ENTER SCRIPT HERE" + "\n";
  tmp = tmp + "#" + "\n";


  tmp = tmp + "" + "\n";
  tmp = tmp + "" + "\n";
  tmp = tmp + "" + "\n";
  tmp = tmp + "} while (ddt_next_row(table) == E_OK);" + "\n";
  tmp = tmp + "" + "\n";
  tmp = tmp + "" + "\n";
  tmp = tmp + "ddt_close(table);" + "\n";
  tmp = tmp + "" + "\n";

  return (tmp);

}



void
GenerateHeaderFile(string filepath, string headertype)
{

  string TEXT="";
  string functionname;
  string tmp;

  tmp = string_search_and_replace(filepath, "/", "@@@");
  functionname = string_search_and_replace(tmp, "@@@", "//");


  TEXT = TEXT + "#! asci, 100 , 100 , Test Header , Wed Aug 19 13:40:24 1998" + "\n";
  TEXT = TEXT + "\n";
  TEXT = TEXT + "TDDB_ID = 0" + "\n";
  TEXT = TEXT + "NAME = \"" + functionname + "\"" + "\n";
  TEXT = TEXT + "TYPE = \"" + headertype + "\"" + "\n";
  TEXT = TEXT + "AUTHOR = \"\"" + "\n";
  TEXT = TEXT + "\n";
  TEXT = TEXT + "DATE = 903559224" + "\n";
  TEXT = TEXT + "FUNCTION = \"\"" + "\n";
  TEXT = TEXT + "SPEC = \"\"" + "\n";
  TEXT = TEXT + "\n";
  TEXT = TEXT + "FSEARCH = \"\"" + "\n";
  TEXT = TEXT + "TSEARCH = \"\"" + "\n";
  TEXT = TEXT + "\n";
  TEXT = TEXT + "DESCRIPTION = \"\"" + "\n";
  TEXT = TEXT + "COMMENTS = \"\"" + "\n";
  TEXT = TEXT + "\n";
  TEXT = TEXT + "PARAMETERS = " + "\n";

  write_file(filepath + "/header", TEXT);

}

void
GenerateDataBaseFiles(string filepath)
{
  string dbpath, exppath;

  string tmp = "";

  tmp = tmp + "G track \"000000\" 0" + "\n";
  tmp = tmp + "G sync \"000000\" 0" + "\n";
  tmp = tmp + "G tlist \"000000\" 0" + "\n";
  tmp = tmp + "G tests \"000000\" 0" + "\n";
  tmp = tmp + "G x_product_name \"WinRunner\" 0" + "\n";
  tmp = tmp + "G x_product_versn \"WR10\" 0" + "\n";

  dbpath = filepath + "/DB";
  MakeDir(dbpath);

  write_file(dbpath + "/tm.asc", tmp);
  write_file(dbpath + "/crvx.asc", tmp);


  exppath = filepath + "/EXP";
  MakeDir(exppath);

}

