

string
CreateDragMessage (string editorClass)
{
      string Result ;

         // SPR 2306 - drag and drop projdir trailing backslash problem
	     Result = "EditorQrlEvalNoSave \"AcknowledgeDrop(" +
		     "\\\"" + current_editor() + "\\\"," +
		     "\\\"" + editorClass + "\\\"," +
		     "\\\"" + string_strip(string_escape(current_projdir(), "\\"),"T","\\") + "\\\"," +
		     "\\\"" + current_system() + "\\\"," +
		              editor_get_msgd_handle() + "," +
		     "${xpos}, ${ypos}" +
		     ");\"" ;

   return (Result) ;
}


struct ObjectInfoType
{
   string omsType ;
   string omsName ;
   string omsClass ;
   int oid ;
} ;

struct SymbolInfoType
{
   string class ;
   string type ;
   string label ;
} ;

void
SendDropAcknowledgment (string dragEditor, int dragStpmHandle,
			string projdir, string system,
		        string dropEditor, string dropEditorClass,
		        ObjectInfoType dropTarget,
		        int xpos, int ypos, string info)
{

      info = string_escape (info, "\"") ;
      info = string_escape (info, "\\") ;

	  // SPR 2306 - drag and drop projdir trailing backslash problem
	  projdir = string_escape (projdir, "\"") ;
	  projdir = string_escape (projdir, "\\") ;

      string stpemString = 
		     "EditorQrlEvalNoSave \"ReceiveDropAcknowledge(" +
		     "\\\"" + dropEditor + "\\\"," +
		     "\\\"" + dropEditorClass + "\\\"," +
			      editor_get_msgd_handle() + "," +
		     "\\\"" + projdir + "\\\"," +
		     "\\\"" + system + "\\\"," +
		     "\\\"" + dropTarget.omsName + "\\\"," +
		     "\\\"" + dropTarget.omsType + "\\\"," +
		     "\\\"" + dropTarget.omsClass + "\\\"," +
			      dropTarget.oid + "," +
			      xpos          + "," +
			      ypos          + "," +
		     "\\\"" + info + "\\\"" +
		       ");\"" ;

      int handle = editor_send_msg (dragEditor, stpemString, dragStpmHandle) ;
		    
}

void
SendToRemoteMessageLog (string msg, string editor_name, int stpmHandle)
{

   msg = string_escape (msg, "\"") ;
   msg = string_escape (msg, "\\") ;


   string stpemString = 
	   "EditorQrlEvalNoSave \"print_message(" +
		     "\\\"" + msg + "\\\"" +
		       ");\"" ;
   int handle = editor_send_msg (editor_name, stpemString, stpmHandle) ;
}


void
SendToRemoteQrl (string QrlCommand, string editor_name, int stpmHandle)
{
   QrlCommand = string_escape (QrlCommand, "\"") ;

   string stpemString = 
	   "EditorQrlEvalNoSave \"" + QrlCommand + "\"" ;

   int handle = editor_send_msg (editor_name, stpemString, stpmHandle) ;
}


void
SendCannotDropMsg 
   (string dragEditor, int dragHandle, string dropEditor, int dropHandle)
{
   string msg = "You currently cannot drag from the \"" + dragEditor +
		"\" editor and drop to the editor \"" + dropEditor + "\"." ;

   SendToRemoteMessageLog (msg, dragEditor, dragHandle) ;
   SendToRemoteMessageLog (msg, dropEditor, dropHandle) ;
}


int
get_index_from_hetergeneous_oms_list (list hetero, int oid)
{
      string LinkOrNodeOrCntx ; 
      int count = list_count (hetero) ;
      int i ;
      node Node ;
      link Link ;
      cntx Cntx ;
      int Result = -1 ;

      for (i=0; i< count; i=i+1)
      {  
	 LinkOrNodeOrCntx = list_get_type (hetero, i) ;

         if (LinkOrNodeOrCntx == "node")
         {
	    Node = list_get (hetero, i) ;
	    if (Node.id == oid)
	    {
	       Result = i ;
	       break ;
	    }
         }
         else if (LinkOrNodeOrCntx == "link")
         {
	    Link = list_get (hetero, i) ;
	    if (Link.id == oid)
	    {
	       Result = i ;
	       break ;
	    }
         }
         else if (LinkOrNodeOrCntx == "cntx")
         {
	    Cntx = list_get (hetero, i) ;
	    if (Cntx.id == oid)
	    {
	       Result = i ;
	       break ;
	    }
	 }
      }

      return (Result) ;
}




SymbolInfoType
get_symbol_info (gde_symbol symbol)
{
   SymbolInfoType Result ;
   Result.class = NULL ;
   Result.type = NULL ;
   Result.label = NULL ;

   if (gde_symbol_is_node (symbol))
   {
      gde_node gnode = to_gde_node (symbol) ;
      Result.class = "node" ;
      Result.label = gde_node_label (gnode) ;

      gde_nt gnt = gde_node_nt (gnode) ;
      Result.type = gde_nt_name (gnt) ;

   }
   else if (gde_symbol_is_arc (symbol))
   {
      gde_arc arc = to_gde_arc (symbol) ;
      list link_list = gde_arc_links (arc) ;
      Result.class = "link" ;
      gde_link glink = list_get(link_list, 0) ;

      Result.label = gde_link_label (glink) ;
      gde_lt glt = gde_link_lt (glink) ;
      Result.type = gde_lt_name (glt) ;
   }
   else if (gde_symbol_is_cntx (symbol))
   {
      gde_cntx gcntx = to_gde_cntx (symbol) ;
      Result.class = "cntx" ;
      Result.label = gde_cntx_label (gcntx) ;

      gde_ct gct = gde_cntx_ct (gcntx) ;
      Result.type = gde_ct_name (gct) ;
   }
   return (Result) ;
}




ObjectInfoType
get_oms_info_from_oid (list hetero, int oid)
{
   int index = get_index_from_hetergeneous_oms_list (hetero, oid) ;
   return (get_oms_info_from_hetergeneous_oms_list (hetero, index)) ;
}

ObjectInfoType
get_oms_info_from_hetergeneous_oms_list (list hetero, int index)
{
      string LinkOrNodeOrCntx ; 
      int count = list_count (hetero) ;
      node Node ;
      link Link ;
      cntx Cntx ;
      ObjectInfoType Result ;

      Result.omsName = NULL ;
      Result.omsType = NULL ;
      Result.oid = -1 ;

      if (index < count && index >= 0)
      {  
	 LinkOrNodeOrCntx = list_get_type (hetero, index) ;

         if (LinkOrNodeOrCntx == "node")
         {
	    Node = list_get (hetero, index) ;
	    Result.omsName = Node.name ;
	    Result.omsType = Node.type ;
	    Result.omsClass = "node" ;
	    Result.oid = Node.id ;
         }
         else if (LinkOrNodeOrCntx == "link")
         {
	    Link = list_get (hetero, index) ;
	    Result.omsName = Link.name ;
	    Result.omsType = Link.type ;
	    Result.omsClass = "link" ;
	    Result.oid = Link.id ;
         }
         else if (LinkOrNodeOrCntx == "cntx")
         {
	    Cntx = list_get (hetero, index) ;
	    Result.omsName = Cntx.name ;
	    Result.omsType = Cntx.type ;
	    Result.omsClass = "cntx" ;
	    Result.oid = Cntx.id ;
	 }
      }

      return (Result) ;
}

list // of node, cntx, link
gde_symbol_to_oms (gde_symbol symbol)
{
     list Result = list_create ("all", 0) ;

     if (symbol != NULL && gde_symbol_valid (symbol))
     {
	if (gde_symbol_is_node (symbol))
	{
	   gde_node node = to_gde_node (symbol) ;

	   list_concatenate (Result, gde_node_oms_nodes (node)) ;
	   list_concatenate (Result, gde_node_oms_links (node)) ;
	   list_concatenate (Result, gde_node_oms_cntxs (node)) ;
	}

	if (gde_symbol_is_arc (symbol))
	{
	   gde_arc arc = to_gde_arc (symbol) ;
	   list link_list = gde_arc_links (arc) ;
	   int i ;
	   int count = list_count (link_list) ;
	   gde_link link ;

	   for (i=0; i< count; i++)
	   {
	      link = list_get (link_list, i) ;

	      list_concatenate (Result, gde_link_oms_nodes (link)) ;
	      list_concatenate (Result, gde_link_oms_links (link)) ;
	      list_concatenate (Result, gde_link_oms_cntxs (link)) ;
	   }
	}

	if (gde_symbol_is_cntx (symbol))
	{
	   gde_cntx cntx = to_gde_cntx (symbol) ;

	   list_concatenate (Result, gde_cntx_oms_nodes (cntx)) ;
	   list_concatenate (Result, gde_cntx_oms_links (cntx)) ;
	   list_concatenate (Result, gde_cntx_oms_cntxs (cntx)) ;
	}
     }

     return (Result) ;
}
