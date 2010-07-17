//*****************************************************************************
//DESCRIPTION: progress.inc
//             ==============================
//             (c) 1998 by Aonix GmbH
//*****************************************************************************
//
// The qrls which are called trough tdrv.exe can use the progress bar
// indication suplied by tdrv. 
// To force progress indication control print messages with the following format
// to stderror (use message() function from qrl source):
// "##<level>#<current progress>#<progress at the next control point>#<Descriptive message>", 
// <level> : an integer 0 .. 9, indicates include level of progress control messages
// <current progress>:integer 0..99, indicates current progress in <level>
// <progress at the next control point>:an integer 1..100, indocates what will be progress at <level> when next progress control message occures
//
//           example: 
//                  main(){  // Level 0 - the level of main function
//                      ...
//                      message("#0#0#20#Starting...");  // 10%
//                      ...
//                      message("#0#20#90#20% completed...");  // 10%
//                      func(); // switch to level 1
//                      message("#0#90#100#90% completed...");  // 10%
//                      ...
//                  } // when exit, a 100% are completed
//
//                  void func() // func() has independent progerss control points
//                  {
//                     ...
//                     message("#1#0#50#Starting...");  
//                     ...
//                     message("#1#50#100#Starting...");  // 10%
//                     ...
//                  } // a 100% of level 1 are completed
//
//*****************************************************************************
// functions: 

//*****************************************************************************

//*****************************************************************************
// Global variables 
//*****************************************************************************

list glstProgressLevelSteps;
list glstProgressLevels;
list glstProgressExpectedLevels;

int  gnCurrentProgressLevel;
int  gnProgressStepTime;
int  gnProgressStartTime;
int  gnProgressTotalTime;
const int gcnMaxProgressLevels = 10;
boolean gbProgressDebugMode;
boolean gbProgressMessagesDiasabled;
boolean gbProgressDiasableAutoDecrmentLevel;
const string ERR_PREFIX = "$";

void initProgress(int nStep)
{

    glstProgressLevels = list_create("int", gcnMaxProgressLevels);
    glstProgressExpectedLevels = list_create("int", gcnMaxProgressLevels);
    glstProgressLevelSteps = list_create("int", gcnMaxProgressLevels);
    gnCurrentProgressLevel = 0;
    list_set(glstProgressLevels, 0, 0);
    list_set(glstProgressExpectedLevels, 0, nStep);
    list_set(glstProgressLevelSteps, 0, nStep);
    gbProgressDebugMode = False;
    gbProgressMessagesDiasabled = False;
    gbProgressDiasableAutoDecrmentLevel = True;
    gnProgressStepTime = time_now();
    gnProgressTotalTime = 0;
    if (gbProgressDebugMode == True)
        gnProgressStartTime = time_now();
    showProgress("");
    
}

void progress(string sMessage, int nExpected)
{
   list_set(glstProgressLevels, gnCurrentProgressLevel, 
            list_get(glstProgressExpectedLevels, gnCurrentProgressLevel));
   list_set(glstProgressExpectedLevels, gnCurrentProgressLevel, nExpected);
   showProgress(sMessage);
}

void debugProgress()
{
    int nTimeNow = time_now();
    gnProgressStepTime = nTimeNow;
    if (gnProgressTotalTime != 0)
    {
        int nElapsedTime = nTimeNow - gnProgressStartTime;
        int nPercentCompleted;

        if (nElapsedTime == 0)
            nPercentCompleted = 0;
        else
            nPercentCompleted = (nElapsedTime*100)/gnProgressTotalTime;
        message("[" + time_to_string(nTimeNow, NULL)+"]" + 
                ", Completed:" + nPercentCompleted +
                "%, TimeElapsed:" + nElapsedTime);
    }
    else
    {
        message("[" + time_to_string(nTimeNow, NULL)+"]" + 
                ", StepTime:" + (nTimeNow - gnProgressStepTime) +
                ", TimeElapsed:" + (nTimeNow - gnProgressStartTime));
    }

}

void doneProgress()
{
    message("##0#100#100#Done");
    if (gbProgressDebugMode)
        debugProgress();
}

int calcProgress(int nProgress, int nProgressStep)
{
    nProgress = nProgress + nProgressStep;
    if(nProgress > 100)
        return 100;
    return nProgress;
}

int calcProgressStep(int nFromProgress, int nToProgress, int nNumSteps)
{
    return ((nToProgress - nFromProgress)/(nNumSteps + 1));
}

void showProgress(string sMessage)
{
    sMessage =  "##"+ gnCurrentProgressLevel +
                "#"+list_get(glstProgressLevels, gnCurrentProgressLevel)+
                "#"+list_get(glstProgressExpectedLevels, gnCurrentProgressLevel)+
                "#"+sMessage;

    if (!gbProgressMessagesDiasabled)
        message(sMessage);

    if (gbProgressDebugMode == True)
        debugProgress();
    
}

void updateProgress(string sMessage)
{

   int nProgress = list_get(glstProgressLevels, gnCurrentProgressLevel);
   int nProgressExpected = list_get(glstProgressExpectedLevels, gnCurrentProgressLevel);
   int nProgressStep = list_get(glstProgressLevelSteps, gnCurrentProgressLevel);

   nProgress = nProgressExpected;
   nProgressExpected = nProgress + nProgressStep;
   if (gbProgressDiasableAutoDecrmentLevel != True &&
       nProgress >= 100)
   {
        decProgressLevel();
        return;
   }

   list_set(glstProgressLevels, gnCurrentProgressLevel, nProgress);
   list_set(glstProgressExpectedLevels, gnCurrentProgressLevel, nProgressExpected);
   showProgress(sMessage);

}


// === simple 'get' functions
int getProgressLevel() {return gnCurrentProgressLevel;}

int getProgress(){return list_get(glstProgressLevels, gnCurrentProgressLevel);}
int getProgressAtLevel(int nLevel) { return list_get(glstProgressLevels, nLevel);}

int getProgressStep(){ return list_get(glstProgressLevelSteps, gnCurrentProgressLevel);}
int getProgressStepAtLevel(int nLevel){ return list_get(glstProgressLevelSteps, nLevel);}

int getProgressExpected(){return list_get(glstProgressExpectedLevels, gnCurrentProgressLevel);}
int getProgressExpectedAtLevel(int nLevel){return list_get(glstProgressExpectedLevels, nLevel);}

// === simple 'set' functions
void setProgressStep(int nStep){list_set(glstProgressLevelSteps, gnCurrentProgressLevel, nStep);}
void setProgressStepAtLevel(int nStep, int nLevel){list_set(glstProgressLevelSteps, nLevel, nStep);}

void setProgress(int nProgress){ list_set(glstProgressLevel, gnCurrentProgressLevel, nProgress);}
void setProgressAtLevel(int nProgress, int nLevel){ list_set(glstProgressLevel, nLevel, nProgress);}

void setProgressExpected(int nProgress){ list_set(glstProgressExpectedLevel, gnCurrentProgressLevel, nProgress);}
void setProgressExpectedAtLevel(int nProgress, int nLevel){ list_set(glstProgressExpectedLevel, nLevel, nProgress);}


int incProgressLevel(int nStep, string sMessage)
{
    gnCurrentProgressLevel = gnCurrentProgressLevel + 1;
    if (gnCurrentProgressLevel == gcnMaxProgressLevels)
    {
        message("ERROR:ProgressLevelStack overflow.");
        return gcnMaxProgressLevels;
    }

    if (nStep != 0)
        setProgressStep(nStep);

    list_set(glstProgressLevels, gnCurrentProgressLevel, 0);
    list_set(glstProgressExpectedLevels, gnCurrentProgressLevel, nStep);
    list_set(glstProgressLevelSteps, gnCurrentProgressLevel, nStep);

    showProgress(sMessage);
    return gnCurrentProgressLevel;

}

int decProgressLevel()
{

    list_set(glstProgressLevels, gnCurrentProgressLevel, 100);
    list_set(glstProgressExpectedLevels, gnCurrentProgressLevel, 100);
    
    showProgress("");
    
    if (gnCurrentProgressLevel != 0)
    {
        gnCurrentProgressLevel = gnCurrentProgressLevel - 1;
    }
    else
    {
        message("ERROR: ProgressLevel Stack rollover.");
        return gcnMaxProgressLevels;
    }
    return gnCurrentProgressLevel;

}


