external string T_subactions="all";
// Specifies which actions to prepare test cases for.

external string T_substates="all";
// Specifies which states to prepare test cases for.

external string T_subindices="all";
// Specifies which indices to prepare test cases for.

external string T_extra_information="TRUE";
// Set to TRUE, generates extra information.

external string T_satisfy_preconditions="TRUE";
// Set to TRUE, satisfies preconditions.

external string T_case_desc_file="basic.cdf";
// Specifies a case description file (CDF), if other  than
// the  default.   tdriver  searches  for  the CDF in this
// order of precedence:
// 1.   In the test unit.
// 2.   In the test_files directory.
// 3.   In the file specified by the -q option.
// 4.   In the default, which is $T4BASE/include/basic.cdf

external string T_case_desc_dir="prepare.dir";
// Specifies a destination directory for the prepared test cases.
//

external string T_spacing="1";
// Specifies line spacing.  
// (1)  single line spacing for the output.
// (2)  double line spacing for the output.

external string T_message_level="3";
// Level of messages

string tvariable;

string
Get_T_subactions()
{
    return (T_subactions);
}


string
Get_T_substates()
{
    return (T_substates);
}


string
Get_T_subindices()
{
    return (T_subindices);

}


string
Get_T_extra_information()
{
    return (T_extra_information);

}


string
Get_T_satisfy_preconditions()
{
    return (T_satisfy_preconditions);

}


string
Get_T_case_desc_file()
{
    return (T_case_desc_file);

}



string
Get_T_tcdir(string tcdir)
{

  if (tcdir != "")
      return (tcdir);

  tvariable = (toolinfo_variable("T_tcdir"));

  if (tvariable != NULL)
    return (tvariable);
  else
    return ("testcases");

}


string
Get_T_spacing()
{
    return (T_spacing);

}


string
Get_T_message_level()
{
    return (T_message_level);

}


void
AddTime(string command, string path)
{
   string start, stop;
   list startlist, stoplist;
   list starttimelist, stoptimelist;
   int starttime, stoptime, elapsedtime;
   string syscommand;
   string sysdir = current_projdir() + current_system() + "/";

   start = read_file(sysdir + "start");
   start = string_search_and_replace(start, "", "");
   stop = read_file(sysdir + "stop");
   stop = string_search_and_replace(stop, "", "");

   start = string_search_and_replace (start, "  ", " ");
   stop = string_search_and_replace (stop, "  ", " ");

   startlist = string_to_list(start, " ");
   stoplist = string_to_list(stop, " ");


   starttimelist = string_to_list(list_get(startlist,3), ":");
   stoptimelist = string_to_list(list_get(stoplist,3), ":");

   starttime = 3600*to_int(list_get(starttimelist,0)) + 60*to_int(list_get(starttimelist,1)) + to_int(list_get(starttimelist,2));
   stoptime = 3600*to_int(list_get(stoptimelist,0)) + 60*to_int(list_get(stoptimelist,1)) + to_int(list_get(stoptimelist,2));

   elapsedtime = stoptime - starttime;

   syscommand = "echo '" + elapsedtime + "' > " + path + "/measures_" + command;
   system (syscommand);

}
