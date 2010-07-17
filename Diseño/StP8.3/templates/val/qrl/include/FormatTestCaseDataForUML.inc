external string use_case_name;


string FILETEXT="";
set DataItemsDataTypes;
string CDFFILE;



void
FormatTestCaseData (string test_unit_id)
{

  list ScenarioList;
  int count1, i;
  int count2, j;
  string scenario_name;
  node scenario;
  int scenario_id;
  set DataTypes;
  string ActorIDs;

  DataItemsDataTypes = set_create ("string");

  set_clear(DataItemsDataTypes);

  message ("Generating Data for Use Case: " + use_case_name); 

  Create_CDF_Header();

  ActorIDs = GetActorIDList(use_case_name);

  ScenarioList = GetScenarioList(use_case_name);
  count2 = list_count (ScenarioList);
  for (j=0;j<count2;j=j+1)
    {
      scenario = list_get(ScenarioList,j);
      scenario_name = RemoveSpaces(scenario.name);
      scenario_id = scenario.id;
      message ("  Generating Data for Scenario: " + scenario_name );


      Build_Event_Section(use_case_name, scenario_name, scenario_id, ActorIDs);

      FILETEXT = FILETEXT + " }" + "\n";
    }

  Create_CDF_Footer();


  CDFFILE = OUTPUTPATH + test_unit_id + "/" + test_unit_id + ".cdf";
  WriteFileText (CDFFILE, FILETEXT);
  FILETEXT = "";

  message ("");
  message ("Formatting Test Case Data");

  FormatTestCases (OUTPUTDIR, CDFFILE, test_unit_id);
}

void
Create_CDF_Header()
{
  FILETEXT = FILETEXT + "BEGIN " + "\n";
  FILETEXT = FILETEXT + "  {" + "\n";
  //FILETEXT = FILETEXT + "    print HEADER ;" + "\n";
  FILETEXT = FILETEXT + "    open case  \"testcases.tcf\";" + "\n";
  FILETEXT = FILETEXT + "  }" + "\n";

  FILETEXT = FILETEXT + "CASE " + "\n";
  FILETEXT = FILETEXT + "  {" + "\n";
}

void
Create_CDF_Footer()
{

  FILETEXT = FILETEXT + "  resume   case;" + "\n";
  FILETEXT = FILETEXT + "}" + "\n";

  FILETEXT = FILETEXT + "END" + "\n";
  FILETEXT = FILETEXT + "  {" + "\n";
  FILETEXT = FILETEXT + "    close    case;" + "\n";
  FILETEXT = FILETEXT + "  }" + "\n";

}




void
Build_Event_Section( string use_case_name, string scenario_name, 
                     int scenario_id, string actorids)
{
  list Events;
  list EventList;
  int event_id, i,count;
  string event, eventname, fromclass, toclass, actionname;
  link_ref linkrefvar;
  int eventnumber = 0;
  string IO, PG;

  Events = list_create ("string", 0);

  list_clear(Events);

  Events = GetEvents( scenario_id);


  EventList = list_create("string",0);

  count = list_count(Events);

  PrintIsTestCase (use_case_name, scenario_name);

  PrintExercises();
  PrintProbes();
//  PrintEvaluation();

  for (i=0; i<count; i=i+1)
    {
       linkrefvar = list_get(Events, i);
       event = GetEventName(linkrefvar.link_id);
       eventname = StripData(event);
       event_id = linkrefvar.link_id;

       IO = DetermineIO(linkrefvar.link_id, scenario_id, actorids);

       if (IO == "INPUT")
         PG = "IP4";
       else if (IO == "OUTPUT")
         PG = "OP4";

       FILETEXT = FILETEXT + "print \"<" + PG + ">Step " + ++eventnumber + ". ";
       FILETEXT = FILETEXT + GetSourceName(event_id) + " <" + eventname 
                 + "> " + GetDestinationName(event_id)  + "\";\n";



       if (IO == "INPUT")
          PrintArguments( event);
       else if (IO == "OUTPUT")
          PrintExpectedOutputs( event_id);
    }

  FILETEXT = FILETEXT + "\n\n";

}


void
PrintArguments ( string event)
{
  int i,count;
  note notevar;
  list TNote, notelist;
  item itemvar,itemvar2,itemvar4;
  string returndata;
  string PRODUCES;

  int l1,l2;

  l1 = string_find(event, 0, "(");

  if (l1 == string_length(event) )
     return;

  string data;
  list datalist;


  l1 = string_find(event, 0, ":");

  if (l1 < string_length(event) )
     {
       if (PRODUCES == "")
          PRODUCES = string_extract( event, 0, l1 );
       else
          PRODUCES = PRODUCES + "," + string_extract( event, 0, l1 );
     }

  l2 = string_find(event, 0, "(");


  event = string_extract( event,l2+1,string_length(event)-(l2+1) );

  event = string_search_and_replace(event, ")", "");

  datalist = string_to_list(event, ",");

  count = list_count(datalist);

  for (i=0;i<count;i++)
    {
      data = list_get(datalist,i);

        FILETEXT = FILETEXT + "print \"<DI><" + RemoveSpaces(data)
          + "> ==> \" " + data + ";" + "\n";

    }

}




void
PrintExpectedOutputs ( int event_id)
{
  int i,count;
  note notevar, notevar0;
  list TNote, notelist;
  item itemvar1, itemvar2, itemvar3, itemvar4;

  string q1 = "item [HasExpectedOutput && obj_id == ${event_id}]";
  string q2 = "item [ExpectedOutputCommand && obj_id == ${event_id}]";
  string q3 = "item [OutputArgumentList && obj_id == ${event_id}]";
  string q4 = "item [OutputHasFixedValue && obj_id == ${event_id}]";


  itemvar1 = find_by_query (q1);
  itemvar2 = find_by_query (q2);
  itemvar3 = find_by_query (q3);
  itemvar4 = find_by_query (q4);

  string tmp;
  list tmplist;
  

  if (itemvar2 != NULL)
    {
      if (itemvar3 != NULL)
        {
          tmp = RemoveSpaces(itemvar3.value);
          tmp = "'\"" + string_search_and_replace(itemvar3.value, ",", "\"' '\"") + "\"'";


          FILETEXT = FILETEXT + "print \"<GR>" + itemvar2.value + " " + tmp +  "\";" + "\n";
          return;
        }
      else
        {
          FILETEXT = FILETEXT + "print \"<GR>" +
               itemvar2.value + "\";" + "\n";
          return;
        }
    }

  if (itemvar4 != NULL)
    {
      FILETEXT = FILETEXT + "print \"<ER>" + event_id + "\";" + "\n";
      return;
    }

  // No expected output
  FILETEXT = FILETEXT + "print \"<NO>No Expected Result Given.\";" + "\n";
}

void
Build_Dataitem_Set(int id)
{
  list dataitems;
  int i,count;
  note notevar;
  item itemvar1, itemvar3, itemvar4, itemvar5;
  string default="UNSPECIFIED" + SEPARATOR2 + "UNSPECIFIED";
  string typename= default;

  string q1 = "note [TestDataItem && obj_id == ${id}] sort by name";
  string q2 = "item [TDataItemName && note_id == ${notevar.id}]";
  string q3 = "item [TFixedValue && note_id == ${notevar.id}]";
  string q4 = "item [TDataType && note_id == ${notevar.id}]";
  string q5 = "item [TDataTypeName && note_id == ${notevar.id}]";

  dataitems = list_select(q1);

  count = list_count(dataitems);

  for (i=0;i<count;i=i+1)
    {
      notevar = list_get(dataitems,i);
      itemvar1 = find_by_query(q2);

      itemvar3 = find_by_query(q3);
      itemvar4 = find_by_query(q4);
      itemvar5 = find_by_query(q5);

      if (itemvar3 != NULL)
        {
           typename = "FIXED" + SEPARATOR2 + itemvar3.value;
        }

      else if (itemvar4 != NULL)
        {
           typename = "DEFINED" + SEPARATOR2 + itemvar4.value;
        }

      else if (itemvar5 != NULL)
        {
           typename = "DEFINED" + SEPARATOR2 + itemvar5.value;
        }


      set_add (DataItemsDataTypes, itemvar1.value + SEPARATOR + typename);


    }
}




void
WriteFileText (string OUTPUTDIR, string FILETEXT)
{
  string outputfile = OUTPUTDIR;
  write_file (outputfile, FILETEXT);

}

void 
FormatTestCases(string OUTPUTDIR, string CDFFILE, string test_unit_id )
{

  if (CheckForSDF(OUTPUTDIR, "TDDB"))
     tdriverPrepare (test_unit_id, CDFFILE, "UseCaseTestScripts");
  else
     message ("TDDB file for " + test_unit_id + " does not exist.");


}

string
GetEventName( int link_id)
{
  link linkvar;
  item itemvar;

  string query2 = "link [ id == ${link_id} ]";

  linkvar = find_by_query (query2);

  if (linkvar == NULL)
     return (NULL);
  else
    return (RemoveSpaces(linkvar.name));


}



string
GetSourceName( int link_id)
{
  link linkvar;
  string linkquery = "link [ id == ${link_id} ]";
  node nodevar;
  string nodequery = "node [  id == ${linkvar.from_node_id} ]";


  linkvar = find_by_query (linkquery);

  if (linkvar == NULL)
    return ("");
  else 
    {
      nodevar = find_by_query(nodequery);
      return (nodevar.name);
    }


}



string
GetDestinationName( int link_id)
{
  link linkvar;
  string linkquery = "link [ id == ${link_id} ]";
  node nodevar;
  string nodequery = "node [ id == ${linkvar.to_node_id} ]";


  linkvar = find_by_query (linkquery);

  if (linkvar == NULL)
    return ("");
  else 
    {
      nodevar = find_by_query(nodequery);
      return (nodevar.name);
    }

}


list
GetEvents(int scenario_id)
{
 
  string linkref_query, noderef_query;
  node_ref nr;

  noderef_query = "node_ref [node_id == ${scenario_id} ] ";

  nr = find_by_query (noderef_query);


  linkref_query = "link_ref [ file_id == ${nr.file_id} && 
           link [ UmlSimpleMessage ] ] sort by ycoord";

  return ( list_select (linkref_query) );

}


string
DetermineIO(int linkid, int scenario_id, string actorids)
{
 
  string linkref_query, noderef_query;
  node_ref nr;

  if (actorids == "")
      actorids = "0";

  noderef_query = "node_ref [node_id == ${scenario_id} ] ";

  nr = find_by_query (noderef_query);

  linkref_query = "link_ref [ file_id == ${nr.file_id} && 
           link [ UMLSimpleMessage   &&
           id == ${linkid} &&
           to_node[${actorids}] ] ]";

  if ( find_by_query (linkref_query) == NULL)
     return ("INPUT");
  else 
     return ("OUTPUT");

  
}





void
PrintIsTestCase(string use_case_name , string scenario_name)
{
  FILETEXT = FILETEXT + "if (ACTION_NAME == \"" + scenario_name + "\")" + "\n";
  FILETEXT = FILETEXT + " {" + "\n";
  FILETEXT = FILETEXT + "print  \"@\";"  + "\n";
}

void
PrintExercises()
{
  FILETEXT = FILETEXT + "if (IS_EXERCISED==\"y\")"  + "\n";
  FILETEXT = FILETEXT + "print ACTION_NAME \"#GOOD#\" CASE_NAME  ;" + "\n";
  FILETEXT = FILETEXT + "  else "  + "\n";
  FILETEXT = FILETEXT + "print ACTION_NAME \"#INVALID#\" CASE_NAME  ;" + "\n";
}



void
PrintProbes()
{

  FILETEXT = FILETEXT + "if (CASE_INDEX==1)   {"  + "\n";
  FILETEXT = FILETEXT + "   print \"<P3>Probes: All inputs at reference values\";"  + "\n";
  FILETEXT = FILETEXT + "   }"  + "\n";
//  FILETEXT = FILETEXT + "else if (CASE_INDEX==2)   {"  + "\n";
//  FILETEXT = FILETEXT + "   print \"<P3>Probes: All inputs at Low Bounds\";"  + "\n";
//  FILETEXT = FILETEXT + "   }"  + "\n";
//  FILETEXT = FILETEXT + "else if (CASE_INDEX==3)   {"  + "\n";
//  FILETEXT = FILETEXT + "   print \"<P3>Probes: All inputs at High Bounds\";"  + "\n";
//  FILETEXT = FILETEXT + "   }"  + "\n";
  FILETEXT = FILETEXT + "else"  + "\n";
  FILETEXT = FILETEXT + "   {"  + "\n";
  FILETEXT = FILETEXT + "   foreach USES_INDEX   {"  + "\n";
  FILETEXT = FILETEXT + "      if ( USES_SAMPLE != \"reference\" )   {"  + "\n";
  FILETEXT = FILETEXT + "         if ( USES_SAMPLE != \"fixed_value\" ) {"  + "\n";
  FILETEXT = FILETEXT + "            print \"<P3>Probes: \" USES_DOMAIN  \"  \" USES_NAME \" at \" USES_SAMPLE ;"  + "\n";
  FILETEXT = FILETEXT + "            }"  + "\n";
  FILETEXT = FILETEXT + "         }"  + "\n";
  FILETEXT = FILETEXT + "      }"  + "\n";
  FILETEXT = FILETEXT + "   }"  + "\n";


}

