
//      sccsid[] = %W% %Y% %D%
//      StP/SE
//      Confidential property of Interactive Development Environments, Inc.
//      Copyright (c) 1995
//      All rights reserved
//

// se_util.inc
//
//	Contains utility functions used by codegen and dse and sce 
//	semantic checks.
//	


//	IsBuiltInCType is used by codegen and dse and sce semantic 
//	checks to determine whether a type is a builtin C type. If 
//	not, it must be a dse defined type.

//	to_c_identifier and to_c_type_identifier are used by both
//	code generation and the PI integration to convert SE names
//	to valid C identifiers

#include "ct/qrl/include/general.inc"

string
SEArraySize(string array_size)
{
    if (array_size == NULL || array_size == "")
	return "";

    if (string_extract(array_size, 0, 1) == "[" &&
        string_extract(array_size, string_length(array_size) - 1, 1) == "]")
        return array_size;
 
    else return "[" + array_size + "]";
}


const string BuiltInCTypes = "void char int float double short long unsigned signed ";
list BuiltInTypesList = string_to_list(BuiltInCTypes, " ");
const int       BuiltInTypesListCount = 9;
const string    BAD_C_IDENTIFIERS_TRANSLATE_TABLE = " !@#$%^&*()+-=|\\{},./?~`;:'\"\n\t";
const string    GOOD_C_IDENTIFIERS_TRANSLATE_TABLE = string_repeat("_", string_length(BAD_C_IDENTIFIERS_TRANSLATE_TABLE));


boolean
IsBuiltInCType(string type)
{
    list type_list;
    int i, pos;
    string one_type;

    if (str_no_val(type))
	return False;

    pos = string_find(type, 0, "(");

    if (pos != string_length(type))
	return False;

    type_list = string_to_list(type, " ");

    for (i = 0; i < list_count(type_list); i++)
    {
        one_type = list_get(type_list, i);
        if (list_find(BuiltInTypesList, 0, one_type) != BuiltInTypesListCount)            return True;
    }

    return False;
}

//	checkForVarArgs is used by codegen and sce semantic checks
//	to determine whether a module is defined as having a variable
//	number of arguments.

boolean
checkForVarArgs(int id)
{
    string      query;
    item        item = NULL;

    query = "item[obj_id=${id} && VariableArguments]";

    item = find_by_query(query);

    if (item != NULL && item.value == "True")
        return True;

    return False;
}


//	The following are utility functions to handle type names
//	which are qualfied (i.e. struct, enum, etc.) or are pointers 
//	to types (i.e. foo *).

// strip_prefix
//
//      Strips the qualifier from a type name 
//	(i.e. struct, union, enum, or typedef). 
//
//	This is useful for finding the "real" 
//	types from a qualified type.
//	
//	e.g. a function might have a return type
//	of "struct foo", but the corresponding
//	structure in DSE might be "foo".

string
strip_prefix(string s)
{
    string      s_prefix = "";
    int         pos, s_length;

    if (s == "")
        return s;

    s_length = string_length(s);

    pos = string_find(s, 0, " ");

    if (pos != s_length)
    {
        s_prefix = string_extract(s, 0, pos + 1);
 
        if (s_prefix == ("struct ") ||
            s_prefix == ("enum ") ||
            s_prefix == ("union ") )
            s = string_extract(s, pos + 1, s_length - pos - 1);
    }

    return s;
}

// strip_suffix
//
//      Strips pointer character(s) from a type name.
//      Also handles multiple ptrs and extra spaces (i.e. foo * *)

string
strip_suffix(string s)
{
    string      s_suffix = "", temp = "";
    int         pos, s_length, i;

    if (s == "")
        return s;

    s_length = string_length(s);

    for (i = 0; i < s_length; i++)
    {
        temp = string_extract(s, string_length(s) - 1, 1);
        if(temp == " " || temp == "*")
            s = string_extract(s, 0, string_length(s) - 1);
        else break;
    }
 
    return s;
}
 
 
//  get_qualifier_type
//
//	Returns the pdm type for the type qualifier

string
get_qualifier_type(string s)
{
    int         s_length, pos;
    string      s_prefix = "";
 
    if (s == "")
        return s;
 
    s_length = string_length(s);
 
    pos = string_find(s, 0, " ");
 
    if (pos != s_length)
    {
        s_prefix = string_extract(s, 0, pos + 1);
 
        if (s_prefix == "struct ")                
            return "Sequence";
        if (s_prefix == "union ")
            return "Selection";
        if (s_prefix == "enum ")
            return "Enumeration";
    }
 
    return "";
}


// 
// to_reveng_name
//
// used to convert a name to one with spaces rather then underscores, as
// the reverse engineering tool does this.  This should not be used until
// the name is searched for without the change.
//

const string BAD_IDENT_CH = "_\n\t";
const string GOOD_IDENT_CH = string_repeat(" ", string_length(BAD_IDENT_CH));

string
to_reveng_name(string name)

{
    return string_translate(name, BAD_IDENT_CH, GOOD_IDENT_CH);
}


const string B_IDENT_CH = " \n\t";
const string G_IDENT_CH = string_repeat("_", string_length(BAD_IDENT_CH));

string
to_underscore_name(string name)

{
    return string_translate(name, B_IDENT_CH, G_IDENT_CH);
}

 
// get_type_definition
//
//	Uses the above utilties to return a list 
//	of candidate types.
//	
//	e.g. For a type "struct foo bar *" it would
//	first search for nodes corresponding to the name 
//	"struct foo bar", if none exist, it would look 
//	for nodes with the name	"foo bar".
//
//	This function returns a list which may be
//	used to populate a selection box during 
//	navigation.	

list
get_type_definition(string type_name)
{
    string      query, qtype = "", ntype;
    list        candidate_list = list_create("node", 0);
 
    type_name = strip_suffix(type_name);
    // get all definitions for type_name

    candidate_list = list_select("node[(Sequence || Selection || Enumeration || Typedef) && name = '" + to_oms_string(type_name) + "' && !in_links && node_refs]");

    if (list_count(candidate_list) != 0)
	return candidate_list;

    ntype = to_reveng_name(type_name);
    if (ntype != type_name)
	list_concatenate(candidate_list, list_select("node[(Sequence || Selection || Enumeration || Typedef) && name = '" + to_oms_string(ntype) + "' && !in_links && node_refs]"));
 
    if (list_count(candidate_list) == 0)
    {
        // check if the type is qualified
        qtype = get_qualifier_type(type_name);
        if (qtype != "")
        {
            query = "node[${qtype} && name = '"
                    + to_oms_string(strip_prefix(type_name))
                    + "' && !in_links && node_refs]";

            list_concatenate(candidate_list,list_select(query));

		// added 2/12/96 PN, as code gen is using this and getting
		// repeated entries in its list
	    if (list_count(candidate_list) > 0 )
		return candidate_list;
		// thats all I added - the rest is as is

	    // and the biz with the underscores
            ntype = to_reveng_name(strip_prefix(type_name));
            if (ntype != type_name)
	        list_concatenate(candidate_list, list_select("node[(Sequence || Selection || Enumeration || Typedef) && name = '" + to_oms_string(ntype) + "' && !in_links && node_refs]"));
        }
    }
    return candidate_list;
}



// Find the DSE definition of the data type of the given parameter. This
// is used by both DSE and SCE for navigating to types

list
FindTypeDefinition(int id, string item_type)
{
    item one_item;
    string type_name, ntype_name;
    list l = list_create("node_ref", 0);

    one_item = find_by_query("item[${item_type} && obj_id == ${id}]");
    if (one_item != NULL)
    {
	type_name = strip_suffix( one_item.value );
	l = list_select("node_ref[node[(Sequence | Selection | Enumeration | Typedef) && name == '" + to_oms_string(type_name) + "']]");

	if (list_count(l) == 0)
	{
	    // ok, so that is not there. How about the unembellished type
	    ntype_name = strip_prefix(type_name);
	    if (ntype_name != type_name)
	    {
	        l = list_select("node_ref[node[(Sequence | Selection | Enumeration | Typedef) && name == '" + to_oms_string(ntype_name) + "']]");
	    }
	    if (list_count(l) == 0)
	    {
	      // ok, so that is not there. How about without underscores 
		type_name = to_reveng_name(ntype_name);
	        if (ntype_name != type_name)
		    l = list_select("node_ref[node[(Sequence | Selection | Enumeration | Typedef) && name == '" + to_oms_string(type_name) + "']]");
		if (list_count(l) == 0)
		  {
		    type_name = to_underscore_name( type_name );
		    l = list_select("node_ref[node[(Sequence | Selection | Enumeration | Typedef) && name == '" + to_oms_string(type_name) + "']]");
		  }
   	    }
	}

    }

    return l;
}


string
replace_spaces_with_underscores(string s)
{
    return string_translate(s, "_", " ");
}

string   
to_c_identifier(string s)
{
    list l_string;
    string elem;

    if (s == "")
        return s;

    // if there is a dot followed by a number at the end, nix that part.

    l_string = string_to_list(s, ".");
    elem = list_get(l_string, list_count(l_string) - 1);
    if (string_converts(elem, "int"))
    {   
        list_delete(l_string, list_count(l_string) - 1);
        s = list_to_string(l_string, "_");
    }   

    return string_translate(s, BAD_C_IDENTIFIERS_TRANSLATE_TABLE, GOOD_C_IDENTIFIERS_TRANSLATE_TABLE);
}

string   
to_c_type_identifier(string s)
{
    string      s_suffix = "", s_prefix = "", s_scs = "";
    string temp = " ";
    int         i, pos, s_length, ll;
    list words;
 
    if (s == "")
        return s;
 
    s_length = string_length(s);
 
    pos = string_find(s, 0, " ");
 
    // handles types with struct, union, or enum qualifiers
    if (pos != s_length)
    {   
        s_prefix = string_extract(s, 0, pos + 1);
 
        if (s_prefix == ("struct ") ||
            s_prefix == ("enum ") ||
            s_prefix == ("union ") )
        {
            s = string_extract(s, pos + 1, s_length - pos - 1);
            s_length = string_length(s);
        }
    }

	// remove any storage class specifiers from the list first, 
	// then add them all back on again before we return
    ll = list_count( BuiltInTypesList );
    words = string_to_list( s, " " );
    for( i = 0; i < list_count( words ); i++ )
    {
	temp = list_get( words, i );
	if ( list_find( BuiltInTypesList, 0, temp ) < ll )
	{
	    s_scs = s_scs + temp;
	    if ( string_length( temp ) == string_length( s ))
	    {
		s = "";
		break;
	    }
	    else
	    {
		s_scs = s_scs + " ";
		s = string_extract( s, string_length( temp ) + 1,
			string_length( s ) - string_length( temp ) - 1 );
	    }
	}
	else
	    break;
    }
    list_clear( words );
 
    s_length = string_length(s);
    temp = "";

    // handles pointers to types including spaces and double ptrs
    if (s_length > 0)
    {   
        for (i = 0; s != "" && i < s_length; i++)
        {
            temp = string_extract(s, string_length(s) - 1, 1);
            if(temp == " " || temp == "*")
            {
                s = string_extract(s, 0, string_length(s) - 1);
                s_suffix = temp + s_suffix;
            }
            else break;
        }
    }

    if ( s == "" && s_scs != "" )
	return s_scs + s_suffix;
    else
	return s_scs + to_c_identifier(s) + s_suffix;
}

