//
//      sccsid[] = %W% %Y% %D%
//      StP/Core
//      Confidential property of Interactive Development Environments, Inc.
//      Copyright (c) 1992-1996
//      All rights reserved
//

#include "rules/qrl/ct.inc"

void
to_requirements_table_from_subsystem (string name)
{
      editor_send_msg("rqtbl", 
		      "EditorQrlEval \"REQ_SetSubsystem(\\\""+name+"\\\")\;\"",
		      CH_FIRST) ;
}

void
to_requirements_table_from_node (int node_id)
{
      editor_send_msg("rqtbl", 
		      "EditorQrlEval \"RequirementTableInitializeForNode(" +
			  node_id +
			  ")\;\"",
		      CH_FIRST) ;
}

void
to_requirements_table_from_cntx (int id)
{
      editor_send_msg("rqtbl", 
		      "EditorQrlEval \"RequirementTableInitializeForCntx(" +
			  id +
			  ")\;\"",
		      CH_FIRST) ;
}

void
to_requirements_table_from_link (int link_id)
{
      editor_send_msg("rqtbl", 
		      "EditorQrlEval \"RequirementTableInitializeForLink(" +
			  link_id +
			  ")\;\"",
		      CH_FIRST) ;
}

void
to_requirements_table_from_link_for_node (string NodeType)
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

               to_requirements_table_from_node (Node.id) ;
	       break ;
	    }
	 }
      }
   }
}

void
to_requirements_table_from_link_for_link (string LinkType)
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
               to_requirements_table_from_link (Link.id) ;
	       break ;
	    }
	 }
      }
   }
}

