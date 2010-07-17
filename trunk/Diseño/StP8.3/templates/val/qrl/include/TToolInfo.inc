//T_subactions=all
// Specifies which actions to prepare test cases for.

//T_substates=all
// Specifies which states to prepare test cases for.

//T_subindices=all
// Specifies which indices to prepare test cases for.

//T_extra_information=TRUE
// Set to TRUE, generates extra information.

//T_satisfy_preconditions=TRUE
// Set to TRUE, satisfies preconditions.

//T_case_desc_file=basic.cdf
// Specifies a case description file (CDF), if other  than
// the  default.   tdriver  searches  for  the CDF in this
// order of precedence:
// 1.   In the test unit.
// 2.   In the test_files directory.
// 3.   In the file specified by the -q option.
// 4.   In the default, which is $T4BASE/include/basic.cdf

//T_spacing=1
// Specifies line spacing.  
// (1)  single line spacing for the output.
// (2)  double line spacing for the output.

//T_message_level=3
// Level of messages

string tvariable;

string
Get_T_subactions()
{
  tvariable = (toolinfo_variable("T_subactions"));

  if (tvariable != NULL)
    return (tvariable);
  else
    return ("all");
}


string
Get_T_substates()
{
  tvariable = (toolinfo_variable("T_substates"));

  if (tvariable != NULL)
    return (tvariable);
  else
    return ("all");
}


string
Get_T_subindices()
{
  tvariable = (toolinfo_variable("T_subindices"));

  if (tvariable != NULL)
    return (tvariable);
  else
    return ("all");

}


string
Get_T_extra_information()
{
  tvariable = (toolinfo_variable("T_extra_information"));

  if (tvariable != NULL)
    return (tvariable);
  else
    return ("FALSE");

}


string
Get_T_satisfy_preconditions()
{
  tvariable = (toolinfo_variable("T_satisfy_preconditions"));

  if (tvariable != NULL)
    return (tvariable);
  else
    return ("FALSE");

}


string
Get_T_case_desc_file()
{
  tvariable = (toolinfo_variable("T_case_desc_file"));

  if (tvariable != NULL)
    return (tvariable);
  else
    return ("basic.cdf");

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
  tvariable = (toolinfo_variable("T_spacing"));

  if (tvariable != NULL)
    return (tvariable);
  else
    return ("1");

}


string
Get_T_message_level()
{
  tvariable = (toolinfo_variable("T_message_level"));

  if (tvariable != NULL)
    return (tvariable);
  else
    return ("3");

}


