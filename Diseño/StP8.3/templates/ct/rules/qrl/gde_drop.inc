#include "rules/qrl/requirement_allocation.inc"
#include "rules/qrl/drag_drop.inc"


// Data to save until object callback occurs.
int GDE_xposSave = -1 ;
int GDE_yposSave = -1 ;
int GDE_dragStpmSave = -1 ;
list GDE_possibleDropDestinationsSave = list_create ("all", 0) ;
string GDE_dragEditorNameSave = NULL ;


void
AcknowledgeDrop (string dragEditor, string dragEditorClass, 
		 string projdir, string system,
		 int dragStpmHandle, int xpos, int ypos)   
{
   // SPR 2306 - drag and drop projdir trailing backslash problem
   if (system == current_system() && projdir == string_strip(current_projdir(), "T", "\\"))
   {
      if       (dragEditorClass == "gte")
      {
	 if (dragEditor == "rqtbl")
	 {
	    standardGdeDrop (dragEditor, dragEditorClass, projdir,
		             system, dragStpmHandle, xpos, ypos) ;
	 }
	 else
	 {
	    SendCannotDropMsg (dragEditor, dragStpmHandle, current_editor(),
		editor_get_msgd_handle()) ;
	 }
      }
      else if  (dragEditorClass == "gde")
      {
	    standardGdeDrop (dragEditor, dragEditorClass, projdir,
		             system, dragStpmHandle, xpos, ypos) ;
      }
      else if (dragEditorClass == "stp")
      {
      }
      else
      {
	 SendCannotDropMsg (dragEditor, dragStpmHandle, current_editor(),
		editor_get_msgd_handle()) ;
      }
   }
}



void
standardGdeDrop (string dragEditor, string dragEditorClass, string projdir, 
	         string system, int dragStpmHandle, int xpos, int ypos)
{
         gde_symbol symbol = gde_get_symbol (xpos, ypos) ;
         list destinations = gde_symbol_to_oms (symbol) ;
         int count = list_count (destinations) ;

         if (count > 1)
         {
	    GDE_possibleDropDestinationsSave = list_copy (destinations) ;
	    GDE_dragEditorNameSave = dragEditor ;
	    GDE_xposSave = xpos ;
	    GDE_yposSave = ypos ;
	    GDE_dragStpmSave = dragStpmHandle ;

            SymbolInfoType symbolInfo = get_symbol_info (symbol) ;

	    string symbol_type = symbolInfo.type ;
	    string symbol_label = symbolInfo.label ;

	    string prompt = "To which object are you dropping (for " + 
			     symbolInfo.type +" \"" + 
			     symbolInfo.label + "\")?" ;

            int index = editor_item_selector (
	     destinations,                            // list of possibilities 
	     "gde_DropChooser_cb();",                 // callback
	     prompt,                                  // label
	     True
            ) ;
         }
         else if (count == 1)
         {
	    SendDropAcknowledgment (
		  dragEditor, dragStpmHandle,
		  projdir, system, current_editor(), "gde",
		  get_oms_info_from_hetergeneous_oms_list (destinations, 0),
		  xpos, ypos, "") ;
	    
	    list_clear (GDE_possibleDropDestinationsSave) ;
         }
	 else
	 {
	    ObjectInfoType Empty ;
	    Empty.omsName = "" ;
	    Empty.omsClass = "" ;
	    Empty.omsType = "" ;
	    Empty.oid = -1 ;

	    SendDropAcknowledgment (
		  dragEditor, dragStpmHandle,
		  projdir, system, current_editor(), "gde",
		  Empty,
		  xpos, ypos, "") ;
	    
	    list_clear (GDE_possibleDropDestinationsSave) ;
	 }
}



void
gde_DropChooser_cb()
{
   if (list_count (GDE_possibleDropDestinationsSave) > 0)
   {
      int oid = list_get (editor_selector_selected_objects(), 0) ;
      int index = 
	  get_index_from_hetergeneous_oms_list 
	       (GDE_possibleDropDestinationsSave, oid) ;

      if (index != -1)
      {
	    // SPR 2306 - drag and drop projdir trailing backslash problem
	    SendDropAcknowledgment (
		  GDE_dragEditorNameSave, GDE_dragStpmSave,
		  string_strip(current_projdir(),"T","\\"), 
		  current_system(), current_editor(), "gde",
		  get_oms_info_from_hetergeneous_oms_list 
		       (GDE_possibleDropDestinationsSave, index),
		  GDE_xposSave, GDE_yposSave, "") ;
	    
      }
   }
}




void
drop_edfile(string pro,string sys,string edfile,string file,int x, int y)
{
    editor_load_edfile(edfile,file);
}


list // of node, link, and cntx
get_all_oms_objects (int x, int y)
{
     gde_symbol symbol = gde_get_symbol (x, y) ;
     list Result = gde_symbol_to_oms (symbol) ;
     return (Result) ;
}
