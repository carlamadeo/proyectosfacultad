// This function pushes (or re-exports) all objects that have DOORS GUID
// annotations
//

void
doors_push_renames()
{
	// first, find all nodes, links, and cntx that have the appropriate
	// annotations (indicating they have been exported). Then, create export
	// statements for them all
	
	set all_nodes =
		set_select("node[items[${doors_annot_apptype}] && node_refs]");
	set all_links =
		set_select("link[items[${doors_annot_apptype}] && link_refs]");
	set all_cntxs =
		set_select("cntx[items[${doors_annot_apptype}] && cntx_refs]");

	string push_stmt = doors_make_export_stmt(all_nodes,all_links,all_cntxs, True);
	// now send the statements to DOORS
    // no need to check if annots locked because we only export those
    // objects that already have the annotation
	doors_send_string_to_doors(push_stmt);
}
