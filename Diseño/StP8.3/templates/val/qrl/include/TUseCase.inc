
#include "val/qrl/include/get_product.inc"
#include "val/qrl/include/val_string_globals.inc"
#include "val/qrl/include/CreateDir.inc"

boolean gbDebugEnable = False;

string
Get_Include_File_Reference()
{
  string text = "";

  text = text +  ("#include <tsdl.std>\n");
  text = text +  ("#include <stp.std>\n\n");
  text = text +  ("#include <usecase.std>\n\n");

  return (text);
}

string
RemoveSpaces(string phrase)
{
    return (string_search_and_replace(phrase, " ", "_"));
}

string
StripData(string event)
{
  int l1, l2, l3;

  l1 = string_find(event, 0, "=");

  if (l1 < string_length(event) )
     event = string_extract( event,l1+1,string_length(event)-(l1+1) );

  l2 = string_find(event,0, "(");

  if (l2 < string_length(event) )
     event = string_extract( event,0, l2);
return (event);
}

boolean
CheckExistance(string name)
{

  string query;

  if (VALIDATOR_BASE_PRODUCT == "UML")
      query = "node [ UmlUseCase && name == '${name}' ]";
  else 
      query = "node [ OMTUseCase && name == '${name}' ]";

  if( find_by_query (query) == NULL)
     return (False);
  else 
     return (True);
}

string
Get_T_test_unit_id(string usecase, string scenario)
{
  string q1, q2;
  string usecasename, scenarioname;

  if (VALIDATOR_BASE_PRODUCT == "UML")
  {
      q1 = "item [ TTestUnitID &&
                   node [ UmlUseCase && name == '${usecase}' ]]";
      q2 = "item [ TTestUnitID &&
                   node [ UmlScenarioInstance && name == '${scenario}' ]]";
  }
  else 
  {
      q1 = "item [ TTestUnitID &&
                   node [ OMTUseCase && name == '${usecase}' ]]";
      q2 = "item [ TTestUnitID &&
                   node [ OMTScenarioName && name == '${scenario}' ]]";
  }

  item itemvar1, itemvar2;

  itemvar1 = find_by_query (q1);

  if (itemvar1 == NULL)
     usecasename = usecase;
  else
     usecasename = itemvar1.value;

  if (scenario == "")
     scenarioname = "";
  else
    {
      itemvar2 = find_by_query (q2);
      if (itemvar2 == NULL)
        scenarioname =  "__" + scenario;
      else
        scenarioname =  "__" +itemvar2.value;
    }

  return (usecasename + scenarioname);
}

string
Get_T_Include_Files (string name, string type)
{
  string q = "item [ TIncludeFiles && node [ ${type} && name == '${name}' ]]";
  string text = "";
  item itemvar;

  for_each_in_select (q, itemvar)
    {
       text = text +  ("#include \"" + itemvar.value + "\"\n");
    }

  return (text);
}

string
Get_T_SPacketSubject(string name, string type)
{
  string q = "item [ TSubject && node [ ${type} && name == '${name}' ]]";

  item itemvar;

  itemvar = find_by_query (q);

 
  if (itemvar == NULL)
     return ("NO SUBJECT");
  else
     return (itemvar.value);
}

string
Get_T_SPacketDescription(string name, string type)
{
  string q;

  q = "note [ TTestUnitDefinition && node [  ${type} && name == '${name}' ]]";

  note notevar;
  notevar = find_by_query (q);
  if (notevar == NULL)
     return ("NO DESCRIPTION");
  else
     return (notevar.desc);
}


boolean
CheckForSDF(string dir , string file)
{
  string fname = dir + "/" + file;

  if (write_file_access(fname))
     return (True);
  else
     return (False);
}


void
tdriverVerify (string unit)
{
  string system_command = 
             " tdriver.exe" +
             " -p " + current_projdir()  +
             " -s " + current_system() + 
             " -U " + unit  +
             " -e " + Get_T_extra_information() +
             " -v " + Get_T_message_level()  +
             " -C ";
  if(gbDebugEnable)
    message ( system_command );
  system ( system_command );
}


void
tdriverDesign (string unit)
{

  string system_command = 
             " tdriver.exe " +
             " -p " + current_projdir()  +
             " -s " + current_system() + 
             " -U " + unit +

             " -c " + Get_T_satisfy_preconditions() +
             " -v " + Get_T_message_level()  +
             " -d " + Get_T_spacing() +

             " -D ";

  if(gbDebugEnable)
    message ( system_command );
  system ( system_command );

}

void
tdriverPrepare (string unit, string cdf, string tcdir)
{

  string system_command; 

  if (cdf == "")
     cdf = Get_T_case_desc_file();

  system_command = 
             " tdriver.exe " +
             " -p " + current_projdir()  +
             " -s " + current_system() + 
             " -U " + unit + 
             " -v " + Get_T_message_level()  +
             " -q " + cdf +
             " -x " + Get_T_subactions() +
             " -y " + Get_T_substates() + 
             " -z " + Get_T_subindices() +
             " -a " + Get_T_tcdir(tcdir) +
             " -P ";
  if(gbDebugEnable)
    message ( system_command );
  system ( system_command );

}

void // Verify, Design, Prepare
tdriverVDP (string sUnits, string cdf, string tcdir)
{

  string system_command; 

  if (cdf == "" )
     cdf = Get_T_case_desc_file();

  system_command = 
             " tdriver.exe " +
             " -p " + current_projdir()  +
             " -s " + current_system() + 
             " -U " + sUnits + 
             " -v " + Get_T_message_level()  + // OK 
             " -q " + cdf +  // basic.cdf
             " -x " + Get_T_subactions() +
             " -y " + Get_T_substates() + 
             " -z " + Get_T_subindices() +
             " -c " + Get_T_satisfy_preconditions() +
             " -d " + Get_T_spacing() +
             " -e " + Get_T_extra_information() +
             " -a " + Get_T_tcdir(tcdir) +
             " -A";
  if(gbDebugEnable)
    message ( system_command );
  system ( system_command );

}


void
tdriverDP (string sUnits, string cdf, string tcdir)
{

  string system_command; 

  if (cdf == "" )
     cdf = Get_T_case_desc_file();

    system_command = 
             " tdriver.exe " +
             " -p " + current_projdir()  +
             " -s " + current_system() + 
             " -U " + sUnits +
             " -v " + Get_T_message_level()  +
             " -q " + cdf +
             " -x " + Get_T_subactions() +
             " -y " + Get_T_substates() + 
             " -z " + Get_T_subindices() +
             " -c " + Get_T_satisfy_preconditions() +
             " -d " + Get_T_spacing() +
             " -a " + Get_T_tcdir(tcdir) +
             " -DP";

  if(gbDebugEnable)
    message ( system_command );
  system ( system_command );
}

list GetTestCaseList(string OUTPUTDIR)
{

  string test_unit_id;

  string filetext;

  string path;
  string inputfile;
  list testcaselist;

  path = OUTPUTDIR + "/UseCaseTestScripts";
  inputfile = path + "/testcases.tcf";

  if (CheckForSDF(path, "testcases.tcf"))
  {
     filetext = read_file (inputfile);
     filetext = string_search_and_replace(filetext, "\r", "");
  }
  else
    {
      message ("testcases.tcf for " + test_unit_id + " does not exist.");
      return;
    }

  testcaselist = list_create("string",0);
  testcaselist = string_to_list(filetext, SEPARATOR2);

  return(testcaselist);
}


string
GetActorIDList(string usecase)
{
  //ECR 6820 begin - q2 is splitted into 2 queries - q21, q22
  //string q1, q2, q3;
  string q1, q21, q22, q3;
  //ECR 6820 end
  string actorids = "0";
  string SEP = " || ";

  list listvar;
  int i,count;

  node nodevar1, nodevar2, nodevar3;

  if (VALIDATOR_BASE_PRODUCT == "UML")
  {
    q1 = "node [ UmlUseCase && name == '${usecase}' ]";

    //ECR 6820 begin - q2 is splitted
    //q2 = "node [UmlActor && node_refs && " +
    //     "( out_links [ link_refs && UmlUseCaseInteraction && " +
    //     "to_node_id == ${nodevar1.id}] || in_links [ link_refs && " +
    //     "UmlUseCaseInteraction && from_node_id == ${nodevar1.id} ]  ) ] " +
    //     "sort by name";    
    q21 = "node [UmlActor && node_refs && " + 
          "out_links[link_refs && UmlUseCaseInteraction && to_node_id == ${nodevar1.id}]]";
    q22 = "node [UmlActor && node_refs && " +
          "in_links[link_refs && UmlUseCaseInteraction && from_node_id == ${nodevar1.id}]]";
    //ECR 6820 end

    nodevar1 = find_by_query(q1);

    // ECR 6820 begin - concatenate q21 and q22
    //listvar = list_select (q2);
    listvar = list_select(q21);
    list_concatenate(listvar, list_select(q22));
    list_sort(listvar);
    // ECR 6820 end

    count = list_count(listvar);

    for (i=0;i<count;i++)
    {
       nodevar2 = list_get(listvar,i);
       {
          if (actorids == "0")
              actorids = "" + nodevar2.id;
          else
              actorids = actorids + SEP + nodevar2.id;
       }
    }
  }
  else
  {
    q1 = "node [ OMTUseCase && name == '${usecase}' ]";
    
    //ECR 6820 begin - split q2
    //q2 = "node [OMTUseCaseActor && node_refs && " +
    //     "( out_links [ link_refs && OMTUseCaseInteraction && " +
    //     "to_node_id == ${nodevar1.id}] || in_links [ link_refs && " +
    //     "OMTUseCaseInteraction && from_node_id == ${nodevar1.id} ]  ) ] " +
    //     "sort by name";
    q21 = "node[OMTUseCaseActor && node_refs && " +
          "(out_links[link_refs && OMTUseCaseInteraction && to_node_id == ${nodevar1.id}]]";
    q22 = "node[OMTUseCaseActor && node_refs && " +
          "(in_links[link_refs && OMTUseCaseInteraction && from_node_id == ${nodevar1.id}]]";
    //ECR 6820 end
     
    q3 = "node [ OMTClass && name == '${nodevar2.name}' ]";
 
    nodevar1 = find_by_query(q1);
 
    //ECR 6820 begin - concatenate q21 and q22
    //listvar = list_select (q2);
    listvar = list_select(q21);
    list_concatenate(listvar, list_select(q22));
    list_sort(list_var);
    //ECR 6820 end
 
    count = list_count(listvar);
 
    for (i=0;i<count;i++)
    {
        nodevar2 = list_get(listvar,i);
        nodevar3 = find_by_query(q3);
        if (nodevar3 != NULL)
        {
           if (actorids == "0")
             actorids = "" + nodevar3.id;
           else
             actorids = actorids + SEP + nodevar3.id;
        }
    }
  }
  return (actorids);
}

list
GetScenarioList( string use_case_name)
{
  list nodelist;


  int i,j,count, count2;
  node nodevar;
  item itemvar;
  string sequence;
  string SEP = "#";
  list tmp1, tmp2;
  string stvar;
  list stlist;
  int id;
  string q1;

  int node_id;

  if (VALIDATOR_BASE_PRODUCT == "UML")
    q1 = "node[UmlScenarioInstance && node_refs && " + 
         "items [ UmlParentNameItem && value == '${use_case_name}'] && " + 
         "items [ UmlParentTypeItem && value == 'UmlUseCase' ]] " +  
         "sort by name";
  else 
    q1 = "node[OMTScenarioName && node_refs && " + 
         "scope_node [ OMTUseCaseParent && name == '${use_case_name}']] " + 
         "sort by name";


  nodelist = list_select (q1);
  count = list_count (nodelist);

  tmp1 = list_create ("string", 0);

  for (i=0; i<count ; i++)
    {
      nodevar = list_get (nodelist,i);

      sequence = GetScenarioSequenceNumber(nodevar.id);

      list_append (tmp1, sequence + SEP + to_string (nodevar.id) );

    }

  tmp1 = list_sort(tmp1);

  count2 = list_count (tmp1);

  tmp2 = list_create ("node", 0);

  for (j=0; j<count2 ; j++)
    {
      stvar = list_get (tmp1,j);
      stlist = string_to_list (stvar, SEP);
      id = to_int (list_get(stlist,1) );
      for (i=0; i<count ; i++)
        {
          nodevar = list_get (nodelist,i);
          if (nodevar.id == id )
            {
               list_append (tmp2, nodevar);
               continue;
            }
        }

    }

   return(tmp2);
}



int
GetScenarioID( string use_case_name, string scenarioname)
{
  node nodevar;
  string query;

  if (VALIDATOR_BASE_PRODUCT == "UML")
    query = "node[UmlScenarioInstance && node_refs && name == '${scenarioname}' && " + 
            "items [ UmlParentNameItem && value == '${use_case_name}'] && " + 
            "items [ UmlParentTypeItem && value == 'UmlUseCase' ] ] ";
  else
    query = "node[OMTScenarioName && node_refs && name == '${scenarioname}' " + 
            "&& scope_node [ OMTUseCaseParent && name == '${use_case_name}']]";
 

  nodevar = find_by_query(query);

  if (nodevar != NULL)
     return (nodevar.id);
  else
     return (0);

}



int
GetUmlActorID( string actorname)
{
  node nodevar;

  string query = "node[UmlActor && node_refs && name == '${actorname}'] ";



  nodevar = find_by_query(query);

  if (nodevar != NULL)
     return (nodevar.id);
  else
     return (0);

}



void
MakeBackup(string fname)
{

  if (write_file_access(fname))
    {
       system ("cp " + fname  + " " + fname + ".bak" );

    }

}


void
Remove_T_Databases(string OUTPUTDIR)
{



   if (write_file_access(OUTPUTDIR + "/SDDB"))
      system ("rm " + OUTPUTDIR + "/SDDB");


   if (write_file_access(OUTPUTDIR + "/TDDB"))
      system ("rm " + OUTPUTDIR + "/TDDB");


   if (write_file_access(OUTPUTDIR + "/STAR"))
      system ("rm " + OUTPUTDIR + "/STAR");

}

set
GetActors(string usecase)
{
  //ECR 6820 begin - split q2
  //string q1, q2, q3;
  string q1, q21, q22, q3;
  //ECR 6820 -end
  string actorids="0";
  string SEP = " || ";
  set actorset;

  list listvar;
  int i,count;

  node nodevar1, nodevar2, nodevar3;

  if (VALIDATOR_BASE_PRODUCT == "UML")
  {

  q1 = "node [ UmlUseCase && name == '${usecase}' ]";

  //ECR 6820 begin - split q2
  //q2 = "node [UmlActor && node_refs && " +
  //     "( out_links [ link_refs && UmlUseCaseInteraction && " +
  //     "to_node_id == ${nodevar1.id}] || in_links [ link_refs && " +
  //     "UmlUseCaseInteraction && from_node_id == ${nodevar1.id} ]  ) ] " +
  //     "sort by name";
  q21 = "node [UmlActor && node_refs && " +
        "out_links[link_refs && UmlUseCaseInteraction && to_node_id == ${nodevar1.id}]]";
  q22 = "node [UmlActor && node_refs && " +
        "in_links[link_refs && UmlUseCaseInteraction && from_node_id == ${nodevar1.id}]]";
  //ECR 6820 end
  }
  else
  {

  q1 = "node [ OMTUseCase && name == '${usecase}' ]";
 
  //ECR 6820 begin - split q2
  //q2 = "node [OMTUseCaseActor && node_refs && " +
  //     "( out_links [ link_refs && OMTUseCaseInteraction && " +
  //     "to_node_id == ${nodevar1.id}] || in_links [ link_refs && " +
  //     "OMTUseCaseInteraction && from_node_id == ${nodevar1.id} ]  ) ] " +
  //     "sort by name";
  q21 = "node[OMTUseCaseActor && node_refs && " +
        "out_links[link_refs && OMTUseCaseInteraction && to_node_id == ${nodevar1.id}]]";
  q22 = "node[OMTUseCaseActor && node_refs && " +
        "in_links[link_refs && OMTUseCaseInteraction && from_node_id == ${nodevar1.id}]]";
  //ECR 6820 - end    
  }

  nodevar1 = find_by_query(q1);
  //ECR 6820 - concatenate q2
  //listvar = list_select (q2);
  listvar = list_select(q21);
  list_concatenate(listvar, list_select(q22));
  list_sort(listvar);
  //ECR 6820 end
  count = list_count(listvar);
  actorset = set_create("string");

  for (i=0;i<count;i++)

    {

       nodevar2 = list_get(listvar,i);
       if (nodevar2 != NULL)
         {
            set_add(actorset, nodevar2.name);
         }
    }

  return (actorset);
}



list
GetActorList(string usecase)
{
  //ECR 6820 begin - split q2
  //string q1, q2, q3;
  string q1, q21, q22, q3;
  //ECR 6820 end
  string actorids="0";
  string SEP = " || ";
  list actorlist;

  list listvar;
  int i,count;

  node nodevar1, nodevar2, nodevar3;

  if (VALIDATOR_BASE_PRODUCT == "UML")
  {
  q1 = "node [ UmlUseCase && name == '${usecase}' ]";

  //ECR 6820 begin - split q2
  //q2 = "node [UmlActor && node_refs && " +
  //     "( out_links [ link_refs && UmlUseCaseInteraction && " +
  //     "to_node_id == ${nodevar1.id}] || in_links [ link_refs && " +
  //     "UmlUseCaseInteraction && from_node_id == ${nodevar1.id} ]  ) ] " +
  //     "sort by name";
  q21 = "node[UmlActor && node_refs && " + 
        "out_links[link_refs && UmlUseCaseInteraction && to_node_id == ${nodevar1.id}]]";
  q22 = "node[UmlActor && node_refs && " +
        "in_links[link_refs && UmlUseCaseInteraction && from_node_id == ${nodevar1.id}]]";
  //ECR 6820 end
  }
  else
  {
  q1 = "node [ OMTUseCase && name == '${usecase}' ]";
  //ECR 6820 begin - split q2   
  //q2 = "node [OMTUseCaseActor && node_refs && " +
  //     "( out_links [ link_refs && OMTUseCaseInteraction && " +
  //     "to_node_id == ${nodevar1.id}] || in_links [ link_refs && " +
  //     "OMTUseCaseInteraction && from_node_id == ${nodevar1.id} ]  ) ] " +
  //     "sort by name";
  q21 = "node[OMTUseCaseActor && node_refs && " +
        "out_links[link_refs && OMTUseCaseInteraction && to_node_id == ${nodevar1.id}]]";
  q22 = "node[OMTUseCaseActor && node_refs && " +
        "in_links[link_refs && OMTUseCaseInteraction && from_node_id == ${nodevar1.id}]]";
  //ECR 6820 end
  }

  nodevar1 = find_by_query(q1);

  //ECR 6820 - concatenate q21 and q22
  //listvar = list_select (q2);
  listvar = list_select(q21);
  list_concatenate(listvar, list_select(q22));
  list_sort(listvar);
  //ECR 6820 end
  count = list_count(listvar);

  actorlist = list_create("string",0);

  for (i=0;i<count;i++)

    {
       nodevar2 = list_get(listvar,i);
       if (nodevar2 != NULL)
            list_append(actorlist, nodevar2.name);
    }

  return (actorlist);
}



string
GetActorName(int id)
{
  string query = "node[id == ${id}";
  node nodevar;

  nodevar = find_by_query (query);

  return (nodevar.name);

}

string
GetScenarioSequenceNumber(int id)
{


  string query = "item [ScenarioSequence && obj_id == ${id}]";

  item itemvar;

  itemvar = find_by_query(query);

  if (itemvar == NULL)
    return ("999");
  else if (itemvar.value == "")
    return ("999");
  else if (to_int(itemvar.value) < 10)
    return ("00" + to_string(itemvar.value) );
  else if (to_int(itemvar.value) < 100)
    return ("0" + to_string(itemvar.value) );
  else 
    return (to_string(itemvar.value));
}


list
SortBySequenceNumber (list notelist, int event_id)
{
  list itemlist, tmp;
  int i,j,count1, count2;
  note notevar;
  item itemvar;

  tmp = list_create("note",0);

  string query = "item [TDataSequence && obj_id == ${event_id}] sort by value";

  itemlist = list_select(query);
  count1 = list_count(itemlist);

  for (i=0; i<count1; i++)
    {
       itemvar = list_get(itemlist,i);
       count2 = list_count(notelist);
       for (j=0; j<count2; j++)
         {
           notevar = list_get (notelist,j);
           if ( itemvar.note_id == notevar.id)
             {
               list_append (tmp, notevar);
               list_delete (notelist, j);
               break;
             }
          }
     }

   count2 = list_count(notelist);

   for (j=0; j<count2; j++)
     list_append (tmp, list_get (notelist,j));

  return (tmp);
}







void
MakeDir(string dir)
{

  if (!write_file_access(dir))
    {
      // system ("mkdir '" + dir + "'");
      CreateDirectory(dir);
      system ("chmod  777 " + dir );
    }  
}



