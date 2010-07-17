
void
create_external_interfaces(string name, boolean selective, string value)
{
    list    	externals;
    set	    	idset;
    link    	the_link;
    int		ix;

    idset = set_create("int");
    if (selective)
    {
        id_list_create("link[link_refs && (DataFlow || ControlFlow) " + 
		   "&& from_node[External] " + 
    	    	   "&& items[DocumentName && value = '${value}']]", name);
        externals = list_select("link[link_refs && (DataFlow || ControlFlow) " +
    	    	    	    	" && to_node[External] && items[DocumentName && value = '${value}']]");

    }
    else
    {
        id_list_create("link[link_refs && (DataFlow || ControlFlow) " + 
		   "&& from_node[External]]", name);
        externals = list_select("link[link_refs && (DataFlow || ControlFlow) && " +
			    "to_node[External]]");
    }
    for (ix = 0; ix < list_count(externals); ix = ix + 1)
    {
    	the_link = list_get(externals, ix);
    	set_add(idset, the_link.id); 
    }
    id_list_update(name, "link", idset, NULL);
}
