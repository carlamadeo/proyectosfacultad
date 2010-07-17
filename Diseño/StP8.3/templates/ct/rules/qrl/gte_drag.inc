
// routines to facillate drag/drop functionality
#include "rules/qrl/drag_drop.inc"

boolean GTE_DoingDropAck = False ;
boolean GTE_WaitingForUserResponse = False ;


//list GTE_DraggedSymbols = list_create ("gte_symbol", 0) ;
//int GTE_DraggedSymbolIndex = -1 ;

list GTE_SelectedOmsObjects = list_create ("all", 0) ;

string GTE_DoDragProc = NULL ;


// info obtained from the drop site
string GTE_dropEditorName = NULL ;
string GTE_dropEditorClass = NULL ;
int    GTE_dropEditorStpmHandle = -1 ;
string GTE_dropOmsName = NULL ;
string GTE_dropOmsType = NULL ;
string GTE_dropOmsClass = NULL ;
int    GTE_dropOid = -1 ;
int    GTE_dropXpos = -1 ;
int    GTE_dropYpos = -1 ;
string GTE_dropInfo = NULL ;




string
gte_get_builtin_drag_data()
{
   string Result = NULL ;

   if (!GTE_DoingDropAck || (GTE_DoingDropAck && GTE_WaitingForUserResponse))
   {
      string thisEditor = current_editor () ;

      if (thisEditor == "rqtbl")
      {
	 Result = rqtbl_get_builtin_drag_data() ;
      }
      else
      {
	 print_message ("Cannot yet drag from this editor.") ;
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
   if ((!GTE_DoingDropAck || (GTE_DoingDropAck && GTE_WaitingForUserResponse))
      &&
      system == current_system() && projdir == string_strip(current_projdir(), "T", "\\"))
   {
      // save info for later use
      GTE_dropEditorName = dropEditor ;
      GTE_dropEditorClass = dropEditorClass ;
      GTE_dropEditorStpmHandle = dropStpmHandle;
      GTE_dropOmsName = omsName ;
      GTE_dropOmsType = omsType ;
      GTE_dropOmsClass = omsClass ;
      GTE_dropOid = oid ;
      GTE_dropXpos = xpos ;
      GTE_dropYpos = ypos ;
      GTE_dropInfo = info ;

      print_message ("") ;

      string thisEditor = current_editor() ;

      //list DragSelectedSymbolList = GTE_selected_symbols() ;
      //int count = list_count (DragSelectedSymbolList) ;

      if (thisEditor == "rqtbl")
      {
	 RQTBL_ReceiveDropAcknowledge (
		dropEditor, dropEditorClass, dropStpmHandle,
        projdir, system,
		omsName, omsType, omsClass, oid, 
		xpos, ypos, info) ;
      }
      else if (dropEditorClass == "gde")
      {
      }
      else if (dropEditorClass == "gte")
      {
      }
   }
}

