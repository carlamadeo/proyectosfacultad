
// Export to doors
// This function takes the currently selected symbols, gets their OMS
// objects, removes the onces that can't be exported to DOORS, and writes DXL
// commands to export the rest. It also tells NGOAE to add annotations with
// unique requirement identifiers for the ones that don't have these
// annotations


void
doors_export_selected()
{
	// get selected symbols
	list selSyms = gde_selected_symbols();
	list exportableSyms = gde_rules_typeset("DOORS");
	list exportableLinkTypes = doors_exportable_link_types();
	list exportableNodeTypes = doors_exportable_node_types();
	list exportableCntxTypes = doors_exportable_cntx_types();

	// get all mapped OMS objects that can have DOORS requirements but do not
	// already have them and split them into lists of cntx, link, and node
	set sel_nodes = set_create("node");
	set sel_links = set_create("link");
	set sel_cntxs = set_create("cntx");

	int i;
	int j;
	int k;
	gde_symbol sym;

	node n;
	link l;
	cntx c;
	list links;

	list oms_nodes;
	list oms_links;
	list oms_cntxs;

	for(i=0;i<list_count(selSyms);i++) {
		sym = list_get(selSyms,i);

		if (gde_symbol_is_node(sym)) {
			if(list_find(exportableSyms, 0,
			 gde_nt_name(gde_node_nt(to_gde_node(sym)))) ==
				list_count(exportableSyms))
				continue;

			oms_nodes = gde_node_oms_nodes(to_gde_node(sym));
			oms_links = gde_node_oms_links(to_gde_node(sym));
			oms_cntxs = gde_node_oms_cntxs(to_gde_node(sym));
		} else if (gde_symbol_is_arc(sym)) {
			if(list_find(exportableSyms, 0,
				gde_lt_name(gde_arc_lt(to_gde_arc(sym)))) ==
				list_count(exportableSyms))
					continue;
			links = gde_arc_links(to_gde_arc(sym));
	   		for(k = 0;k < list_count(links);k++)
			{
				oms_nodes = gde_link_oms_nodes(list_get(links,k));
				oms_links = gde_link_oms_links(list_get(links,k));
				oms_cntxs = gde_link_oms_cntxs(list_get(links,k));
			}
		} else {
			if(list_find(exportableSyms, 0,
			 gde_ct_name(gde_cntx_ct(to_gde_cntx(sym)))) ==
				list_count(exportableSyms))
				continue;
			oms_nodes = gde_cntx_oms_nodes(to_gde_cntx(sym));
			oms_links = gde_cntx_oms_links(to_gde_cntx(sym));
			oms_cntxs = gde_cntx_oms_cntxs(to_gde_cntx(sym));
		} 
		// for each OMS type, see if it is eligible for export and if it is,
		// add it to the sets
		for(j=0;j<list_count(oms_nodes);j++) {
			n = list_get(oms_nodes,j);
			if(list_find(exportableNodeTypes,0,n.type) != list_count(exportableNodeTypes))
				set_add(sel_nodes,n);
		}
		for(j=0;j<list_count(oms_links);j++) {
			l = list_get(oms_links,j);
				if(list_find(exportableLinkTypes,0,l.type) != list_count(exportableLinkTypes))
					set_add(sel_links,l);
		}
		for(j=0;j<list_count(oms_cntxs);j++) {
			c = list_get(oms_cntxs,j);
				if(list_find(exportableCntxTypes,0,c.type) != list_count(exportableCntxTypes))
					set_add(sel_cntxs,c);
		}
	}
	// Now the sets are full of all the things that can be exported to DOORS.
	// Now for each one, get the existing GUID it has, or create one if it
	// does not exist. Then export to DOORS. DOORS will make use of all of
	// them, even if they are already exported. This gets around the problem
	// of an error on the DOORS side after we have created the annotation
	
	string export_stmt =
        doors_make_export_stmt(sel_nodes,sel_links,sel_cntxs, False);
	// now send the statements to DOORS
    if ("" == export_stmt) // annot file locked
    {
        // JED - this code is not yet used. When it is, we'll have to
        // provide a good error message
        print_error("All selected symbols' annotations are locked." +
                " Unable to export to DOORS");
        return;
    }
	doors_send_string_to_doors(export_stmt);
}
