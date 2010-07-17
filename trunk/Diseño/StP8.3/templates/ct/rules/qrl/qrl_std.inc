const int       ACTIVE_FUNC_IS_INACTIVE = 0;
const int       ACTIVE_FUNC_IS_ACTIVE = 1;

enum dir_enum {FROM, TO};


/* 
    Escapes any quotation chars in a string.  Used in generating table
    fill strings.
*/
string
escape_quotes(string source)
{
    return string_search_and_replace(source, "\"", "\\\"");
}

//
// The following routines (prefaced with "nc_") are used by the name completion
// function in gde and gte
//

string
nc_reference_relationship_clause(string extent)
{
    if (extent == "node")
	return (" && all_node_refs");
    else if (extent == "link")
	return (" && all_link_refs");
    else if (extent == "cntx")
	return (" && all_cntx_refs");
    else
	return ("");
}

string
nc_scope_clause(string extent, int scopeid, boolean ignore_scope)
{
    if (ignore_scope)
	return ("");
    
    if (extent == "node" || extent == "link" || extent == "cntx")
	return (" && scope_node_id == " + to_string(scopeid));
    else
	return ("");
}

string
nc_name_clause(string extent, string pattern)
{
    if (extent == "item")
	return (" && value $ '" + pattern + "'");
    else
	return (" && name $ '" + pattern + "'");
}

void
nc_get_names_from_repository(list names, string pattern, string extent,
			     string apptype, int scopeid, boolean ignore_scope)
{
    string query;
    
    query = extent + "[" + apptype
                   + nc_name_clause(extent, pattern)
		   + nc_scope_clause(extent, scopeid, ignore_scope)
                   + nc_reference_relationship_clause(extent)
		   + "]";

    
    if (extent == "node")
    {
	node n;

	for_each_in_select(query, n)
	{
	    list_append(names, n.name);
	}

    } else if (extent == "link")
    {
	link l;

	for_each_in_select(query, l)
	{
	    list_append(names, l.name);
	}
    } else if (extent == "cntx")
    {
	cntx c;

	for_each_in_select(query, c)
	{
	    list_append(names, c.name);
	}
    } else if (extent == "note")
    {
	note n;

	for_each_in_select(query, n)
	{
	    list_append(names, n.name);
	}
    } else if (extent == "item")
    {
	item i;

	for_each_in_select(query, i)
	{
	    list_append(names, i.value);
	}
    }
}

// JED - added here for DOORS integration since all products and all editos
// use it.
#include "ct/rules/qrl/doors.inc"
