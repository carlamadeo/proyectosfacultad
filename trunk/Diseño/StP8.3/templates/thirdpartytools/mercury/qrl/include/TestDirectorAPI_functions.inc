
string GetAPIFunctions()
{


string APIFunctions = "

int SetTestValue(const char *ValueType, const char *TestValue)
{


  int res = TDAPI_SetTestValue(TESTID, ValueType, TestValue);

  if (DEBUG == 0)
    {
      if (res == 0)
        td_message(\"   SetTestValue  Successful\");
      else
        td_message(\"   SetTestValue  Unsuccessful\");

      debugid (\"SetTestValue\", TESTID);
    }
  return (res);

}
 

int CreateDesStep(const char *StepName, const char *steptext)
{

  int res = TDAPI_CreateDesStep( TESTID, StepName, steptext, &STEPID);
 
  if (DEBUG == 0)
    {
      if (res == 0)
        td_message(\"   CreateDesStep  Successful\");
      else
        td_message(\"   CreateDesStep  Unsuccessful\");

      debugid (StepName, STEPID);
    }

  return (res);

}


 
int SetDesStepValue(const char *StepName, const char *description)
{

  int res = TDAPI_SetDesStepValue(TESTID, STEPID, \"Expected Result\", description);
 
  if (DEBUG == 0)
    {
      if (res == 0)
        td_message(\"   SetDesStepValue  Successful\");
      else
        td_message(\"   SetDesStepValue  Unsuccessful\");

      debugid (StepName, STEPID);
    }

  return (res);

}


int AddDesExpectedOutput(const char *StepName, const char *steptext, const char *description)
{
  int res;

  res = CreateDesStep(StepName, steptext);
  res = SetDesStepValue(StepName, description);

  return (res);

}


 

int CreateTest(const char *TestName)
{

  int res = TDAPI_CreateTest(0, TestName, \"WR-AUTOMATED\", \"Exported\", NODEID, TESTPATH, &TESTID);

  if (DEBUG == 0)
    {
      if (res == 0)
        td_message(\"   CreateTest  Successful\");
      else
        td_message(\"   CreateTest  Unsuccessful\");

      debugid (TestName, TESTID);
    }

  return (res);

}




int AddTestToCycle(const char *CycleName, const char *UserName)
{

  int res = TDAPI_AddTestToCycle(CycleName, TESTID, UserName);

  if (DEBUG == 0)
    {
      if (res == 0)
        td_message(\"   AddTestToCycle  Successful\");
      else
        td_message(\"   AddTestToCycle  Unsuccessful\");

      debugid (CycleName, NODEID);
    }

  return (res);

}




int TreeAddNode(char *NodeName,char *NodeDesc)
{
  int res = TDAPI_TreeAddNode(ROOTID, NodeName, NodeDesc, &NODEID);

  if (DEBUG == 0)
    {
      if (res == 0)
        td_message(\"   TreeAddNode  Successful\");
      else
        td_message(\"   TreeAddNode  Unsuccessful\");

      debugid (NodeName, NODEID);
    }

  return (res);

}



int CreateCycle(char *NodeName)
{

  int res = TDAPI_CreateCycle (NodeName);

  if (DEBUG == 0)
    {
      if (res == 0)
        td_message(\"   CreateCycle Successful\");
      else
        td_message(\"   CreateCycle Unsuccessful\");

      debugid (NodeName, NODEID);
    }

  return (res);

}


int TreeCreateRoot()
{

  int id;

  int res =  TDAPI_TreeCreateRoot(\"Subject\", &SUBJECTID);

  if (DEBUG == 0)
    {
      if (res == 0)
        td_message(\"   TreeCreateRoot Successful\");
      else  
        td_message(\"   TreeCreateRoot Unsuccessful\");

      debugid (\"Subject\", SUBJECTID);
    }

  return (res);

}



int TreeGetSubjectIdFromPath()
{

  int id;

  int res =  TDAPI_TreeGetSubjectIdFromPath(\"Subject\", &SUBJECTID);
         
  if (DEBUG == 0)
    {
      if (res == 0)
        td_message(\"   TreeGetSubjectIdFromPath Successful\");
      else  
        td_message(\"   TreeGetSubjectIdFromPath Unsuccessful\");

      debugid (\"Subject\", SUBJECTID);
    }

  return (res);

}


int TreeGetRoot()
{
  int id;
  int res =  TDAPI_TreeGetRoot(&SUBJECTID);
         
  if (DEBUG == 0)
    {
      if (res == 0)
        td_message(\"   TreeGetRoot Successful\");
      else  
        td_message(\"   TreeGetRoot Unsuccessful\");

      debugid (\"Subject\", SUBJECTID);
    }

  return(res);
}


int GetCategoryTreeRoot()
{
  int  res;

  res = TDAPI_GetCategoryTreeRoot(\"TS_SUBJECT\", &ROOTID);
          
  if (DEBUG == 0)
    {
      if (res == 0)
        td_message(\"   GetCategoryTreeRoot Successful\");
      else  
        td_message(\"   GetCategoryTreeRoot Unsuccessful\");

      debugid(\"ROOTID\", ROOTID);
    }

  return (res);
}


int ConnectDB()
{
  char tmp[40];

  int res=TDAPI_Connect(DATABASENAME,\"\",USERNAME, PASSWORD);

  if (DEBUG == 0)
    {
      if (res == 0) 
        sprintf(tmp, \"Connect to %s Successful\", DATABASENAME);
      else
        sprintf(tmp, \"Connect to %s Unsuccessful\", DATABASENAME);

      td_message (tmp);
    }

  return (res);
}


int
ConnectServer()
{

  int res =  TdServerInitInstance(\"apartment\",SERVER);

  if (DEBUG == 0)
    {
      if (res == 0)
        td_message(\"   Connected to the Server\");
      else
        td_message(\"   Cannot connect to the server\");
    }

  return (res);

}


int
DisconnectDB()
{

  int res = TDAPI_Disconnect();

  if (DEBUG == 0)
    {
      if (res == 0)
        td_message(\"   Disconnected from the database\");
      else
        td_message(\"   Cannot disconnect from the database\");

    }

  return (res);
}


int debugid  (char *name, int id)
{

  char tmp[25];

  sprintf(tmp, \"      %s has id  %d\", name,  id);

  td_message(tmp);

}


";

return (APIFunctions);

}


