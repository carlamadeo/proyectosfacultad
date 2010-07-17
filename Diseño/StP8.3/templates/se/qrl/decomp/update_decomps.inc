int
update_decomps (list names, boolean verbose)
{
    string restrictor = "DfeDiagram", name_restrictor = "";
    string cur_name, comparator;
    boolean top_found = False;
    int ix, count;

    count = list_count(names);
    for (ix = 0; ix < count; ix = ix + 1)
    {
    	cur_name = list_get(names, ix);
    	    /* Want all diagrams */
	if (cur_name == "top.*")
	{
	    top_found = True;
	    break;
	}
    	if (string_find(cur_name, 0, "*") != string_length(cur_name))
    	{
    	    /* Want all diagrams but top */
    	    if (string_extract(cur_name, 0, 1) == "0")
    	    {
    	    	name_restrictor = "name != 'top'";
    	    	break;
    	    }
    	    else
    	    	comparator = " $ ";
    	}
    	else
    	    comparator = " == ";
    	name_restrictor = name_restrictor + "name" + comparator + "'" 
    	    	    	  + cur_name + "'";
    	if (ix < count - 1)
    	    name_restrictor =  name_restrictor + " || ";
    }    	    
    if (count != 0)    
    	name_restrictor = "(" + name_restrictor + ")";
    if (!top_found)
    	restrictor = restrictor + " && " + name_restrictor;
    return file_update(restrictor, verbose);
}
