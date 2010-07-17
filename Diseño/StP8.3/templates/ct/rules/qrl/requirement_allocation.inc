// has functions to communicate with non graphical annotation editor
#include "ct/qrl/include/opt_displaymark.inc"

enum REQ_PhaseType {Analysis, Design, Implementation, Test} ;

const string REQ_ASSIGNMENT_NOTE = "REQAssignmentNote" ;
const string REQ_ANALYSIS_ASSIGNMENT_ITEM = "REQAnalysisAssignmentItem" ;
const string REQ_DESIGN_ASSIGNMENT_ITEM = "REQDesignAssignmentItem" ;
const string REQ_IMPLEMENTATION_ASSIGNMENT_ITEM = 
					  "REQImplementationAssignmentItem" ;
const string REQ_TEST_ASSIGNMENT_ITEM = "REQTestAssignmentItem" ;


// Determines whether or not allocations are cascaded, i.e., whether
// all successive phases are implied when an earlier phase is allocated.
// rts 8010
boolean REQ_CascadeRqmtAllocations = False ;
boolean REQ_CascadeFlagHasBeenSet = False ;
const string REQ_DoRqmtCascadeTIVariable = "rte_CascadeRequirementAllocation" ;



void
REQ_CheckOutAnnotationFileAndExec (int oid, string QrlFunc)
{
   file File = find_by_query ("file[annot_node[${oid}]]") ;

   if (File == NULL)
   {
      File = find_by_query ("file[annot_link[${oid}]]") ;
   }

   if (File == NULL)
   {
      File = find_by_query ("file[annot_cntx[${oid}]]") ;
   }

   annot_checkout_then_exec (File, QrlFunc) ;
}


// variables used to store value until the annotation callback occurs
int              REQ_ANNOT_OBJID ;
string           REQ_ANNOT_UNIQUE_ID ;
REQ_PhaseType    REQ_ANNOT_PHASE ;
list             REQ_ANNOT_RqmtList ;





void
REQ_AnnotAssignCallback ()
{

	 int Id = REQ_ANNOT_OBJID ;
	 string unique_id = REQ_ANNOT_UNIQUE_ID ;
	 REQ_PhaseType ThisPhase = REQ_ANNOT_PHASE ;


         int handle = start_oae (Id) ;

 	 //
	 // Always send the ngoae messages to the first instance rather than
	 // to the handle returned by start_oae. This is necessitated by a
	 // bug in the cix library that manifests itself as RTS 9454.
	 // When the underlying problem in the cix is fixed, we can remove
	 // the following line:
	 handle = CH_FIRST;

         string cmd = OAE_CMD_NOTE_SEL + REQ_ASSIGNMENT_NOTE ;
         REQ_PhaseType Phase ;

         editor_send_msg(NGOAE_CLASS, cmd, handle);
 
         if (REQ_CascadeRqmtAllocations)
         {
            for (Phase = ThisPhase; Phase != NULL; Phase = enum_next (Phase))
            {
               REQ_SelectAssignmentItem (unique_id, handle, Phase) ;
            }

         }
         else
         {
            REQ_SelectAssignmentItem (unique_id, handle, ThisPhase) ;
         }
 
         send_oae_save_update_unload_note(handle);
}


void
REQ_AssignRqmtToOidList (list OidList, string unique_id, 
					     REQ_PhaseType ThisPhase)
{


      int count = list_count (OidList) ;
      int i ;
 
      for (i=0 ; i<count; i=i+1)
      {  
         REQ_ANNOT_OBJID = list_get (OidList, i) ;
         REQ_ANNOT_UNIQUE_ID = unique_id ;
         REQ_ANNOT_PHASE  = ThisPhase ;
	 REQ_CheckOutAnnotationFileAndExec (REQ_ANNOT_OBJID, 
				    "REQ_AnnotAssignCallback();") ;
      }  
}
 






void
REQ_AnnotUnassignCallback ()
{
   int Id = REQ_ANNOT_OBJID ;
   string unique_id = REQ_ANNOT_UNIQUE_ID ;
   REQ_PhaseType ThisPhase = REQ_ANNOT_PHASE ;


   int handle = start_oae (Id) ;

   //
   // Always send the ngoae messages to the first instance rather than
   // to the handle returned by start_oae. This is necessitated by a
   // bug in the cix library that manifests itself as RTS 9454.
   // When the underlying problem in the cix is fixed, we can remove
   // the following line:
   handle = CH_FIRST;

   string cmd = OAE_CMD_NOTE_SEL + REQ_ASSIGNMENT_NOTE ;

   REQ_PhaseType Phase ;
   editor_send_msg(NGOAE_CLASS, cmd, handle);
 
   if (REQ_CascadeRqmtAllocations)
   {
      for (Phase = ThisPhase; Phase != NULL; Phase = enum_next (Phase))
      {
         REQ_SelectAssignmentItem (unique_id, handle, Phase) ;
         cmd = OAE_CMD_ITEM_DEL ;
         editor_send_msg(NGOAE_CLASS, cmd, handle);            
      }
   }
   else
   {
      REQ_SelectAssignmentItem (unique_id, handle, ThisPhase) ;
      cmd = OAE_CMD_ITEM_DEL ;
      editor_send_msg(NGOAE_CLASS, cmd, handle);
   }
   send_oae_save_update_unload_note(handle);
}

void
REQ_UnassignRqmtToOidList (list OidList, string unique_id,
					    REQ_PhaseType ThisPhase)
{

      int count = list_count (OidList) ;
      int i ;
 
      for (i=0 ; i<count; i=i+1)
      {  
         REQ_ANNOT_OBJID = list_get (OidList, i) ;
         REQ_ANNOT_UNIQUE_ID = unique_id ;
         REQ_ANNOT_PHASE  = ThisPhase ;
	 REQ_CheckOutAnnotationFileAndExec 
		 (REQ_ANNOT_OBJID, "REQ_AnnotUnassignCallback();") ;
      }  
}




void
REQ_AssignRqmtListToOIdCallback ()
{

      REQ_PhaseType ThisPhase = REQ_ANNOT_PHASE ;
      list ReqList = REQ_ANNOT_RqmtList ;

      int handle = start_oae (REQ_ANNOT_OBJID) ;

      //
      // Always send the ngoae messages to the first instance rather than
      // to the handle returned by start_oae. This is necessitated by a
      // bug in the cix library that manifests itself as RTS 9454.
      // When the underlying problem in the cix is fixed, we can remove
      // the following line:
      handle = CH_FIRST;

      string cmd = OAE_CMD_NOTE_SEL + REQ_ASSIGNMENT_NOTE ;
      editor_send_msg(NGOAE_CLASS, cmd, handle);


      string req_id ;
      int index ;
      REQ_PhaseType Phase ;

      int ReqCount = list_count (ReqList) ;

      for (index=0; index < ReqCount; index=index+1) 
      {
	    req_id = list_get (ReqList, index) ;

            if (REQ_CascadeRqmtAllocations)
	    {
	       for (Phase = ThisPhase; Phase != NULL; Phase = enum_next (Phase))
               {
	          REQ_SelectAssignmentItem (req_id, handle, Phase) ;
	       }
	    }
	    else
	    {
	       REQ_SelectAssignmentItem (req_id, handle, ThisPhase) ;
	    }
      }

      send_oae_save_update_unload_note(handle);
}


void
REQ_AssignRqmtListToOid (list ReqList, int oid, REQ_PhaseType ThisPhase)
{

   int ReqCount = list_count (ReqList) ;

   if (oid != 0 && ReqCount > 0)
   {
       REQ_ANNOT_OBJID = oid ;
       REQ_ANNOT_PHASE  = ThisPhase ;
       REQ_ANNOT_RqmtList  = list_copy (ReqList) ;
       REQ_CheckOutAnnotationFileAndExec 
		 (oid, "REQ_AssignRqmtListToOIdCallback();") ;
   }
}


void
REQ_UnassignRqmtListToOidCallback ()
{
      REQ_PhaseType ThisPhase = REQ_ANNOT_PHASE ;
      list ReqList = REQ_ANNOT_RqmtList ;

      int handle = start_oae (REQ_ANNOT_OBJID) ;

      //
      // Always send the ngoae messages to the first instance rather than
      // to the handle returned by start_oae. This is necessitated by a
      // bug in the cix library that manifests itself as RTS 9454.
      // When the underlying problem in the cix is fixed, we can remove
      // the following line:
      handle = CH_FIRST;

      string cmd = OAE_CMD_NOTE_SEL + REQ_ASSIGNMENT_NOTE ;
      editor_send_msg(NGOAE_CLASS, cmd, handle);

      string req_id ;
      int index ;
      REQ_PhaseType Phase ;

      int ReqCount = list_count (ReqList) ;

      for (index=0; index < ReqCount; index=index+1) 
      {
	    req_id = list_get (ReqList, index) ;

            if (REQ_CascadeRqmtAllocations)
	    {
	       for (Phase = ThisPhase; Phase != NULL; Phase = enum_next (Phase))
               {
	          REQ_SelectAssignmentItem (req_id, handle, Phase) ;
                  cmd = OAE_CMD_ITEM_DEL ;
                  editor_send_msg(NGOAE_CLASS, cmd, handle);
	       }
            }
	    else
	    {
	       REQ_SelectAssignmentItem (req_id, handle, ThisPhase) ;
               cmd = OAE_CMD_ITEM_DEL ;
               editor_send_msg(NGOAE_CLASS, cmd, handle);
	    }
      }

      send_oae_save_update_unload_note(handle);
}

void
REQ_UnassignRqmtListToOid (list ReqList, int oid, REQ_PhaseType ThisPhase)
{

   int ReqCount = list_count (ReqList) ;

   if (oid != 0 && ReqCount > 0)
   {
       REQ_ANNOT_OBJID = oid ;
       REQ_ANNOT_PHASE  = ThisPhase ;
       REQ_ANNOT_RqmtList  = list_copy (ReqList) ;
       REQ_CheckOutAnnotationFileAndExec 
		 (oid, "REQ_UnassignRqmtListToOidCallback();") ;
   }
}
 

void
REQ_SetCascadeRqmtFlag ()
{
   if (!REQ_CascadeFlagHasBeenSet)
   {
      string value = toolinfo_variable (REQ_DoRqmtCascadeTIVariable) ;
 
      if (value != NULL)
      {
         value = to_lower (value) ;
 
         if (value == "t" || value == "true" || value == "y" || value == "yes"
            || value == "1")
         {
            REQ_CascadeRqmtAllocations = True ;
         }
         else
         {
            REQ_CascadeRqmtAllocations = False ;
         }
      }     

      REQ_CascadeFlagHasBeenSet = True ;
   }
}     



int
start_oae (int id)
{
    string cmd = OAE_CMD_ANNOT_EDIT + id;
    int    handle;

    editor_set_is_busy (True);
    REQ_SetCascadeRqmtFlag() ;

    handle = editor_send_msg(NGOAE_CLASS, cmd, CH_FIRST);
    editor_set_is_busy (False);
    return (handle);
}


string
REQ_ItemTypeFromEnum (REQ_PhaseType PhaseEnum)
{
   string Result ;

   if (PhaseEnum == Analysis)
   {
      Result = REQ_ANALYSIS_ASSIGNMENT_ITEM ;
   }
   else if (PhaseEnum == Design)
   {
      Result = REQ_DESIGN_ASSIGNMENT_ITEM ;
   }
   else if (PhaseEnum == Implementation)
   {
      Result = REQ_IMPLEMENTATION_ASSIGNMENT_ITEM ;
   }
   else if (PhaseEnum == Test)
   {
      Result = REQ_TEST_ASSIGNMENT_ITEM ;
   }

   return (Result) ;
}



string
REQ_PhaseStringFromItem (item Item)
{
	       string phase ;

	       if (Item.type == REQ_ANALYSIS_ASSIGNMENT_ITEM)
	       {
	          phase = "Analysis" ;
	       }
	       else if (Item.type == REQ_DESIGN_ASSIGNMENT_ITEM)
	       {
	          phase = "Design" ;
	       }
	       else if (Item.type == REQ_IMPLEMENTATION_ASSIGNMENT_ITEM)
	       {
	          phase = "Implementation" ;
	       }
	       else if (Item.type == REQ_TEST_ASSIGNMENT_ITEM)
	       {
	          phase = "Test" ;
	       }
	       else
	       {
	          phase = Item.type ;
	       }

               return (phase) ;
}



boolean
IsBlank (string word)
{
   return (word == NULL || word == "") ;
}



void
REQ_SelectAssignmentItem (string unique_id, int handle, REQ_PhaseType Phase)

{
         string cmd  = NULL ;
         string ItemType = REQ_ItemTypeFromEnum (Phase) ;

         if (!IsBlank (unique_id))
         {
            cmd = OAE_CMD_ITEM_SEL +
                    stpem_string_val(ItemType + ":" +
                    unique_id);

            editor_send_msg(NGOAE_CLASS, cmd, handle);
         }
}


string
REQ_AllocationMsgString (REQ_PhaseType Phase, string reqName, 
		   string objName, string objType, int oid)
{
   return (
   "Allocating "+ to_string (Phase) + " requirement \"" +
    reqName + "\" to the "+
    objType + " \"" +objName+ "\" ("+oid+").");
}


string
REQ_DeAllocationMsgString (REQ_PhaseType Phase, string reqName, 
		   string objName, string objType, int oid)
{
   return (
   "Deallocating "+ to_string (Phase) + " requirement \"" +
    reqName + "\" from the "+
    objType + " \"" +objName+ "\" ("+oid+").");
}
