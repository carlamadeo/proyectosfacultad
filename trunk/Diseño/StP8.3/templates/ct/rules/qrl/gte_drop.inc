
#include "rules/qrl/drag_drop.inc"

void
AcknowledgeDrop (string dragEditor, string dragEditorClass, 
		 string projdir, string system,
		 int dragStpmHandle, int xpos, int ypos)   
{
   // SPR 2306 - drag and drop projdir trailing backslash problem
   if (system == current_system() && projdir == string_strip(current_projdir(), "T", "\\"))
   {
      string thisEditor = current_editor() ;

      if       (dragEditorClass == "gte")
      {
	 if (thisEditor == "rqtbl")
	 {
	    RQTBL_AcknowledgeDrop (dragEditor, dragEditorClass,
		   projdir, system, dragStpmHandle, xpos, ypos) ;
	 }
	 else
	 {
	    SendCannotDropMsg (dragEditor, dragStpmHandle, current_editor(),
		editor_get_msgd_handle()) ;
	 }
      }
      else if  (dragEditorClass == "gde")
      {
	 if (thisEditor == "rqtbl")
	 {
	    RQTBL_AcknowledgeDrop (dragEditor, dragEditorClass,
		   projdir, system, dragStpmHandle, xpos, ypos) ;
	 }
	 else
	 {
	    SendCannotDropMsg (dragEditor, dragStpmHandle, current_editor(),
		editor_get_msgd_handle()) ;
	 }
      }
      else if (dragEditorClass == "stp")
      {
	 SendCannotDropMsg (dragEditor, dragStpmHandle, current_editor(),
		editor_get_msgd_handle()) ;
      }
      else
      {
	 SendCannotDropMsg (dragEditor, dragStpmHandle, current_editor(),
		editor_get_msgd_handle()) ;
      }
   }
}


void
drop_edfile(string pro,string sys,string edfile,string file,int x, int y)
{
    string thisEditor = current_editor() ;

    if (thisEditor == "ootbr")
    {
       ootbr_drop_edfile (pro, sys, edfile, file, x, y) ;
    }
    else if (thisEditor == "otbr")
    {
       otbr_drop_edfile (pro, sys, edfile, file, x, y) ;
    }
    else
    {
       editor_load_edfile(edfile,file);
    }
}
