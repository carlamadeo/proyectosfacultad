boolean
is_doors_available()
{
    string var = toolinfo_variable("use_doors_integration");
    if (var != NULL && (var=="True" || var == "Yes"))
        return True;
    return False;
}


void
node_navigate_to_doors_from_link (string NodeType)
{
   list SelectedList = gde_selected_symbols() ;
   int Count = list_count (SelectedList) ;

   if (Count > 0)
   {
      list GDELinkList = gde_arc_links (to_gde_arc(list_get(SelectedList, 0))) ;

      int LinkCount = list_count (GDELinkList) ;
      if (LinkCount > 0)
      {
         list NodeList = gde_link_oms_nodes (list_get(GDELinkList,0)) ;
	 int NodeCount = list_count (NodeList) ;
	 int i ;
	 node Node ;
	 for (i=0; i<NodeCount; i=i+1)
	 {
            Node = list_get (NodeList, i) ;

	    if (Node.type == NodeType)
	    {

               doors_navigate_to_doors (Node.id) ;
	       break ;
	    }
	 }
      }
   }
}
// Navigations to the DOORS objects that correspond to the StP object selected
void
node_navigate_to_doors(int oms_id)
{
	doors_navigate_to_doors(oms_id);
}

void
cntx_navigate_to_doors(int oms_id)
{
	doors_navigate_to_doors(oms_id);
}

void
link_navigate_to_doors(int oms_id)
{
	doors_navigate_to_doors(oms_id);
}

void
doors_navigate_to_doors(int oms_id)
{
	string guid = doors_get_obj_GUID(oms_id);
	string exp_stmt = "";

	if (NULL == guid) {
		if (False) {
			// JED - This way would be used if we separate
			// export and nav steps
			print_error("Object id " + oms_id + " has not been " +
					"exported to DOORS");
			return;
		} else {
			print_message("Object id " + oms_id + " has not been " +
					"exported to DOORS. Exporting it now....");
		}
	}
	// DOORS may have lost the item. The only way to be sure it has the
	// object is to re-export it. This has been combined into the navigation
	// in one step, mostly for speed reasons but also because DOORS may need
	// to prompt the user for information about where to add the new object.
	// This dialog must be responded to and the object added before the
	// navigation can proceed.
	exp_stmt = doors_make_export_nav_stmt(
			set_select("node[${oms_id}]"),
			set_select("link[${oms_id}]"),
			set_select("cntx[${oms_id}]"), True, False);
    if ("" == exp_stmt) // annot file locked
    {
        print_error("Annotation file for object id " + oms_id +
                " is locked. Unable to export to DOORS");
        return;
    }
	doors_send_string_to_doors(exp_stmt);
}

void
link_navigate_to_doors_from_link (string LinkType)
{
   list SelectedList = gde_selected_symbols() ;
   int Count = list_count (SelectedList) ;

   if (Count > 0)
   {
      list GDELinkList = gde_arc_links (to_gde_arc(list_get(SelectedList, 0))) ;

      int LinkCount = list_count (GDELinkList) ;
      if (LinkCount > 0)
      {
         list LinkList = gde_link_oms_links (list_get(GDELinkList,0)) ;
	 LinkCount = list_count (LinkList) ;
	 int i ;
	 link Link ;
	 for (i=0; i<LinkCount; i=i+1)
	 {
            Link = list_get (LinkList, i) ;

	    if (Link.type == LinkType)
	    {
               link_navigate_to_doors(Link.id) ;
	       break ;
	    }
	 }
      }
   }
}


#include "ct/rules/qrl/doors.inc"
