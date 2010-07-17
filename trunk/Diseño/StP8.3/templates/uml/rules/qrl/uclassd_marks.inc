// ECR 5184 created this file

// This method is used for returning display marks prefixed to
// attributes and operations in the class diagram editor.
string
uml_member_display_mark_str(list matched_items)
{
    item            one_item;
    int             i;
    string          derivedMark = "";
    string          visibilityMark = "";
	int             numItems = list_count(matched_items);

    for (i = 0; i < numItems; i = i + 1)
    {
		one_item = list_get(matched_items, i);
		if (one_item.type == "UmlAttributeIsDerived")
		{
			if (one_item.value == "True")
			{
				derivedMark = "/  ";
			}
		} else if (one_item.type == "UmlMemberVisibility")
		{
			if (one_item.value == "private")
			{
				visibilityMark = "-";
			} else if (one_item.value == "protected")
			{
				visibilityMark = "#";
			} else if (one_item.value == "public")
			{
				visibilityMark = "+";
			}
		}
    }
    return derivedMark + visibilityMark;
}


// this function determines which endmark (filled diamond or diamond)
// appears on the end of aggregation links.
string uml_agg_endmark(string agg_type)
{
	if ( NULL != agg_type ) {
		if ( "Aggregation" == agg_type ) {
			return "Diamond";
		} else if ( "Composition" == agg_type ) {
			return "FilledDiamond";
        }
	}
	return NULL;
}

// ECR 157 begin modify
string uml_navigability_mark(string isNavigableStr, int id )
{
    boolean show_bidirectional = False;
    string oppIsNavigable = "False";
        
    // empty means False !!!
    if( isNavigableStr == "" )
        isNavigableStr = "False";

    if( toolinfo_variable( "uclassd_show_navigability_on_both_ends" ) != NULL )
        show_bidirectional = True;
        
    // get the opposite Role
    link opp_role = find_by_query( "link[id != ${id} && " +
        "to_node[UmlAssociation && in_links[id==${id}]]]" );

    if( opp_role != NULL ) {
        item navi = find_by_query( "item[UmlRoleNavigability && link[id==${opp_role.id}]]" );
        if( navi != NULL ) {
            if( navi.value == "True" ) {
                oppIsNavigable = "True";
            } else {
                oppIsNavigable = "False";
            }                    
        } else {
            oppIsNavigable = "False";
        }
    }
    // print_message( "isNavigableStr: >" + isNavigableStr + "<" );
    // print_message( "oppIsNavigable: >" + oppIsNavigable + "<" );

    // state 1 and 3, both ends empty or set, bidirectional
	if( ( isNavigableStr == "False" && oppIsNavigable == "False" ) ||
	    ( isNavigableStr == "True" && oppIsNavigable == "True" ) ) 
	{
        // print_message( "bidirectional" );
        if( show_bidirectional )
            return "OpenArrow";
        else
		    return "None";
    }
    
    // state 2, one end set, unidirectional
    if( isNavigableStr == "False" && oppIsNavigable == "True" )
        return "None";
    if( isNavigableStr == "True" && oppIsNavigable == "False" )
        return "OpenArrow";
}
// ECR 157 end modify

// This function takes a list of items and creates a comma and \n
// separated list of them. open_str and close_str are used to enclose
// the string
string uml_class_property_dpymark_formatter(list matched_items, string open_str,
		string close_str, string sep_str)
{
    item            one_item;
    int             i;
	int             numItems = list_count(matched_items);
	list            strings = list_create("string",0);
	boolean         is_abs = False;
	boolean         decl_abs_in_props = False;

    for (i = 0; i < numItems; i = i + 1)
    {
		one_item = list_get(matched_items, i);

		if (NULL != one_item.value && "" != one_item.value)
		{
			if (one_item.type=="UmlClassIsAbstract") {
				if (one_item.value=="True") {
					is_abs = True;
				}
				continue;
			}
			if ("abstract" == one_item.value) {
				decl_abs_in_props = True;
			}

			list_append(strings,one_item.value);
		} 
    }
	if (is_abs && !decl_abs_in_props) {
		list_append(strings,"abstract");
	}

	return uml_dpymark_string_list_formatter(strings,open_str,close_str,sep_str);
}

// Begin modify ECR 6726
// relocated the next two functions to message_label.inc
// This function takes a list of items and creates a comma and \n
// separated list of them. open_str and close_str are used to enclose
// the string
//string uml_dpymark_list_formatter(list matched_items, string open_str,
//		string close_str, string sep_str)
//{
//    item            one_item;
//    int             i;
//	int             numItems = list_count(matched_items);
//	list            strings = list_create("string",0);
//
//    for (i = 0; i < numItems; i = i + 1)
//    {
//		one_item = list_get(matched_items, i);
//
//		if (NULL != one_item.value && "" != one_item.value)
//		{
//			list_append(strings,one_item.value);
//		} 
//    }
//	return uml_dpymark_string_list_formatter(strings,open_str,close_str,sep_str);
//}

// This function takes a list of items and creates a sep_str
// separated list of them. open_str and close_str are used to enclose
// the string
//string uml_dpymark_string_list_formatter(list strings, string open_str,
//		string close_str, string sep_str)
//{
//	if (list_count(strings) > 0 ) {
//		return open_str + list_to_string(strings, sep_str) + close_str;
//	} else {
//		return "";
//	}
//}
// End modify ECR 6726

string uml_dpymark_string_formatter(string mark_val, string open_str,
		string close_str)
{
	if (NULL != mark_val && "" != mark_val) {
		return open_str + mark_val + close_str;
	} else {
		return NULL;
	}
}
