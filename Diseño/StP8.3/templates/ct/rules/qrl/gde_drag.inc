// routines to facillate drag/drop functionality
#include "rules/qrl/drag_drop.inc"

boolean GDE_DoingDropAck = False ;
boolean GDE_WaitingForUserResponse = False ;


list GDE_DraggedSymbols = list_create ("gde_symbol", 0) ;
int GDE_DraggedSymbolIndex = -1 ;

list GDE_SelectedOmsObjects = list_create ("all", 0) ;

string GDE_DoDragProc = NULL ;


// info obtained from the drop site
string GDE_dropEditorName = NULL ;
string GDE_dropEditorClass = NULL ;
int    GDE_dropEditorStpmHandle = -1 ;
string GDE_dropOmsName = NULL ;
string GDE_dropOmsType = NULL ;
string GDE_dropOmsClass = NULL ;
int    GDE_dropOid = -1 ;
int    GDE_dropXpos = -1 ;
int    GDE_dropYpos = -1 ;
string GDE_dropInfo = NULL ;




string
gde_get_builtin_drag_data()
{
   string Result = NULL ;

   if (!GDE_DoingDropAck || (GDE_DoingDropAck && GDE_WaitingForUserResponse))
   {
      string thisEditor = current_editor () ;

      list DragSelectedSymbolList = gde_selected_symbols () ;

      if (list_count (DragSelectedSymbolList) > 0)
      {
	 Result = CreateDragMessage ("gde") ;
      }
   }
   if (Result != NULL) print_message ("Type <ESC> to cancel drag.") ;
   return (Result) ;
}



void
ReceiveDropAcknowledge (
		string dropEditor, string dropEditorClass, int dropStpmHandle,
		string projdir, string system,
		string omsName, string omsType, string omsClass, int oid, 
		int xpos, int ypos,
		string info)
{
   // SPR 2306 - drag and drop projdir trailing backslash problem
   if ((!GDE_DoingDropAck || (GDE_DoingDropAck && GDE_WaitingForUserResponse))
      &&
      system == current_system() && projdir == string_strip(current_projdir(), "T", "\\"))
   {
      // save info for later use
      GDE_dropEditorName = dropEditor ;
      GDE_dropEditorClass = dropEditorClass ;
      GDE_dropEditorStpmHandle = dropStpmHandle;
      GDE_dropOmsName = omsName ;
      GDE_dropOmsType = omsType ;
      GDE_dropOmsClass = omsClass ;
      GDE_dropOid = oid ;
      GDE_dropXpos = xpos ;
      GDE_dropYpos = ypos ;
      GDE_dropInfo = info ;

      list DragSelectedSymbolList = gde_selected_symbols() ;
      int count = list_count (DragSelectedSymbolList) ;
      string thisEditor = current_editor () ;

      print_message ("") ;

      if (dropEditorClass == "gde")
      {
	  if ((thisEditor == "chen" || thisEditor == "bach" || 
               thisEditor == "ome"  || thisEditor == "uclassd" ) && 
	      (dropEditor == "chen" || dropEditor == "bach" || 
               dropEditor == "ome"  || dropEditor == "uclassd" ))
	  {
	     createObjectWithAttributes (
		dropEditor, dropEditorClass, dropStpmHandle,
		projdir, system,
		omsName, omsType, omsClass, oid, 
		xpos, ypos, info) ;
	  }
	  else
	  {
	     SendCannotDropMsg (current_editor(), editor_get_msgd_handle(), 
			       dropEditor, dropStpmHandle) ;
	  }
      }
      else if (dropEditorClass == "gte")
      {
	 if (dropEditor == "rqtbl")
	 {
	    if (count > 0)
	    {
	       GDE_DoingDropAck = True ;
	       GDE_WaitingForUserResponse = False ;
	       GDE_DraggedSymbols = list_copy (DragSelectedSymbolList) ;

	       GDE_DoDragProc = "GDE_RqtblSelectorCallbackFunc" ;

	       GDE_HandleNextSymbolInDragList (0) ;
	    }
	 }
	 else
	 {
	     SendCannotDropMsg (dragEditor, dragStpmHandle, current_editor(),
		editor_get_msgd_handle()) ;
	 }
      }
   }
}



void
GDE_DragObjectSelectorCallback ()
{

   if (GDE_WaitingForUserResponse && list_count (GDE_SelectedOmsObjects) > 0)
   {
      int oid = list_get (editor_selector_selected_objects(), 0) ;

      if (oid > 0)
      {
	 ObjectInfoType info = 
	     get_oms_info_from_oid (GDE_SelectedOmsObjects, oid) ;
	 
	 string cmd = GDE_DoDragProc +"(info);" ;

	 qrl_eval (cmd) ;
      }

      GDE_WaitingForUserResponse = False ;
      GDE_HandleNextSymbolInDragList (GDE_DraggedSymbolIndex+1) ;
   }
}




void
GDE_HandleNextSymbolInDragList (int symbolIndex)
{

   int count = list_count (GDE_DraggedSymbols) ;

   if (symbolIndex < count && symbolIndex >= 0)
   {
	 GDE_DraggedSymbolIndex = symbolIndex ;

	 gde_symbol symbol = list_get(GDE_DraggedSymbols, symbolIndex) ;

         GDE_SelectedOmsObjects = gde_symbol_to_oms (symbol) ;

         int sourceCount = list_count (GDE_SelectedOmsObjects) ;

         if (sourceCount > 1)
         {
	    GDE_WaitingForUserResponse = True ;

            SymbolInfoType symbolInfo = get_symbol_info (symbol) ;

	    string symbol_type = symbolInfo.type ;
	    string symbol_label = symbolInfo.label ;

	    string prompt = "From which object are you dragging (for " + 
			   symbolInfo.type +" \"" + 
			    symbolInfo.label + "\")?" ;


            int index = editor_item_selector (
	      GDE_SelectedOmsObjects,                // list of possibilities 
	      "GDE_DragObjectSelectorCallback ();",  // callback
              prompt,                                // label
	      True                                   // select only one object
            ) ;
         }
	 else if (sourceCount == 1)
	 {
	    ObjectInfoType info = 
	       get_oms_info_from_hetergeneous_oms_list 
		  (GDE_SelectedOmsObjects, 0) ;

	    string cmd = GDE_DoDragProc + "(info);" ;

	    qrl_eval (cmd) ;

	    GDE_WaitingForUserResponse = False ;
	    GDE_HandleNextSymbolInDragList (symbolIndex+1) ;
	 }
	 else
	 {
	    print_message ("gde drag error: Cannot map symbol to oms node.") ;

            GDE_DoingDropAck = False ;
            list_clear (GDE_DraggedSymbols) ;
	 }
   }
   else
   {
      GDE_DoingDropAck = False ;
      list_clear (GDE_DraggedSymbols) ;
   }
}





// rqtbl specific function
void
GDE_RqtblSelectorCallbackFunc (ObjectInfoType Info)
{
      if (Info.oid > 0)
      {
	 list oidList = list_create ("int", 0) ;
	 list_append (oidList, Info.oid) ;

	 string msg ;
	 string cmd = "REQ_SetCurrentObject (" +
		              "\"" + Info.omsClass + "\"," +
		              Info.oid  + ");" ;

	 // Set current object.
	 //
         SendToRemoteQrl 
	      (cmd, GDE_dropEditorName, GDE_dropEditorStpmHandle) ;


	 // Do allocations.
	 //
	 if (GDE_dropOmsName != NULL && GDE_dropOmsName != "")
	 {
	    REQ_AssignRqmtToOidList (oidList, GDE_dropOmsName, 
	       to_enum (GDE_dropInfo, "REQ_PhaseType")) ;

            msg =
                   "Allocating "+ GDE_dropInfo + " requirement \"" +
                   GDE_dropOmsName + "\" to the "+
                   Info.omsType + " \"" +Info.omsName+ "\" ("+Info.oid+").";

            SendToRemoteMessageLog (msg, GDE_dropEditorName, 
                                    GDE_dropEditorStpmHandle) ;
            print_message (msg) ;

	    // update rqtbl to notify it that rqmts have been added
	    cmd = "REQ_NotifyOfAllocations(" +
		       Info.oid + ",\"" + GDE_dropOmsName +"\");" ;

	    SendToRemoteQrl (cmd, GDE_dropEditorName, 
			   GDE_dropEditorStpmHandle) ;

	 }
      }
}
