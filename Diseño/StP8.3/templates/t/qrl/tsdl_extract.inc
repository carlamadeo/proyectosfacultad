//
//      sccsid[] = %W% %Y% %D%
//      StP/T
//      Confidential property of Interactive Development Environments, Inc.
//      Copyright (c) 1994
//      All rights reserved
//
//      tsdl_extract.inc -- basic source for extraction from IDE tools to 
//      tsdl.
/*
 * Public Functions:
 *	Tid
 *	action_annots_and_end
 *	actiontype
 *	acts_only_if
 *	add_constant
 *	add_dataitem
 *	add_u_p
 *	build_input_list
 *	check_and_add_constant
 *	clean_testunit_files
 *	datatypeclass_and_specifiers
 *	end_sentence
 *	find_constant
 *	fix_invalid
 *	fix_type_name
 *	get_an_item_from_obj_notetype_itemtype
 *	get_dataitem_type
 *	get_description_from_id
 *	get_items_from_name
 *	get_items_from_note
 *	get_items_from_obj
 *	get_note_from_obj
 *	get_notes_from_obj
 *	get_particular_item
 *	get_particular_items
 *	get_procs_fname
 *	get_specific_bool_item
 *	has_bool_value
 *	has_values
 *	is_concluded_on
 *	is_selected_by
 *	items_from_note
 *	last_description
 *	name_and_copyright
 *      predefined_constant   
 *	predefined_type
 *	primitive_type
 *	print_S_Packet
 *	print_cond_clause
 *	print_constants
 *	print_dataitems_and_datatypes
 *	print_ending
 *	print_header
 *	print_placeholder
 *	print_remaining_stuff
 *	print_st_sentence
 *	print_state_sentence
 *	print_std_include
 *	print_tested_exhaustively
 *	print_u_p
 *	print_user_include_files
 *	range_definitions
 *	satisfies
 *	start_action_sentence
 *	start_condition_sentence
 *	string_length
 *	test_exhaustive_annots
 *	type_from_link
 *	which_op
*/ 

const string BAD_CHARS = " !@#$%^&*()+-=|\\{},./?~`:'\"\n\t";
const string GOOD_CHARS = string_repeat("_", string_length(BAD_CHARS));

// The following lists are for things already defined in standard include files
// tsdl.std and stp.std.  These list can be added to to suppress default 
// creation of certain types and constants.

const string PreDefinedDataTypes = " TInteger ReturnType int short_int long_int unsigned_int TReal float double TString TChar char unsigned_char PrintableASCII Digit Bit Bit8string LocalPhone USPhone ZipCode5 ZipCode9 State_Abbreviations Date Time ";

const string PreDefinedConstants = " TRUE FALSE UNKNOWN as_specified below_bounds above_bounds out_of_bounds out_of_type not_in_list abnormal TRUE_string FALSE_string TRUE_integer FALSE_integer ";
 
const string PrimitiveTypes = " integer string character real boolean unspecified boolean ";

#include "qrl/version_utils.inc"

void
name_and_copyright()
{
    // This has to be changed since there is no product_version file any more
    
    //print_warning("\nStP/T Version " + rule_value("t/product_version", "Version", "Major"));   
    print_warning("\nStP/T Version 5.5");   

    //if (Tversion == "True")
    //   print_warning("." + rule_value("t/product_version", "Version", "Minor"));

    print_warning("Copyright 1994-1998 Aonix, Inc\n");

    return;
}



string 
Tid(string s)
{
    if (string_find(s, 0, "_") == 0)
        s = "T" + s;

    return string_translate(s, BAD_CHARS, GOOD_CHARS);

} // end Tid()




 
string
which_op(string name)
 
{
    string op;
 
    if (string_find(name, 0, "*") != string_length(name))
        op = "$";
    else
        op = "==";
 
    return op;
 
} // end of which_op()
 
 
 
string
get_procs_fname(node proc)
 
{
    string query = "file[node_refs[node_id = ${proc.id}]]";
    file the_file;
 
    the_file = find_by_query(query);
    if (the_file == NULL)
        return NULL;
    else
        return the_file.name;
 
} // end of get_procs_fname()
 
 


// NOTES and ITEMS ///////////////////////////////////////////////////////////
//
// This section includes:
//
// Data structures for notes and items
//
// Routines to fetch lists of items given an object id and notename.
//    get_note_from_obj()
//    get_notes_from_obj()
//    get_items_from_name()
//    get_items_from_note()
//    get_items_from_obj()
//
// Routines to print out lines for appropriate testablility annotations.
//  TestAction:  is_concluded_on()
//               acts_only_if()
//               is_selected_by()
//               satisfies()
//
// oddballs:  period()   
// prints the description for the most recent call to get_items_from_note,
// or get_items_from_obj().
//               
//  TestDataType: datatypeclass_and_specifiers()
//
//////////////////////////////////////////////////////////////////////////////

struct datatype_tp
{
    string name;
    string scope;
    node parent;
    int node_type;  
    int annot_id;
};

struct dataitem_tp
{
    string name;
    string simple_name;
    int type_index;
    node parent;
    int node_type;  
};
    

const string BuiltInTypes = " TInteger ReturnType int short_int long_int unsigned_int TReal float double TString TCharacter char unsigned_char boolean_string boolean_integer PrintableASCII Digit Bit Bit8string LocalPhone USPhone ZipCode5 ZipCode9 State_Abbreviations Date Time ";


list 
items_from_note(note the_note)

{
    list ilist;

    string query = "item[note_id = ${the_note.id}]";

    ilist = list_select(query);

    if (ilist == NULL || (list_count(ilist) == 0))
	return NULL;

    return ilist;

} // end of items_from_note;



item 
get_particular_item(list itml, string particular)

{
    int i;
    item itm;

    if (itml == NULL || particular == NULL || particular == "")
	return NULL;

    for (i = 0; i < list_count(itml); i++)
    {
	itm = list_get(itml, i);
	if (itm.type == particular)
	    return itm;
    }
    return NULL;

} // end get_particular_item()




list 
get_particular_items(list itml, string particular)

{
    int i;
    list particular_list;
    item itm;

    particular_list = list_create("item", 0);

    if (itml == NULL || particular == NULL || particular == "")
	return NULL;

    for (i = 0; i < list_count(itml); i++)
    {
	itm = list_get(itml, i);
	if (itm.type == particular)
	    list_append(particular_list, itm);
    }

    if (list_count(particular_list) < 1)
        return NULL;
    else
	return particular_list;

} // end get_particular_items()





list 
get_items_from_name(string name, string notetype)

{
    note nte;
    string query = "note[${notetype} && name = '${name}']";

    nte = find_by_query(query);

    if (nte == NULL)
	return NULL;

    return items_from_note(nte);
}



note 
get_note_from_obj(int id, string notetype)

{
    note the_note;
    string query;

    query = "note[${notetype} && obj_id = ${id}]";
    the_note = find_by_query(query);

    if (the_note == NULL)
	return NULL;

    last_desc = the_note.desc;
    return the_note;

} // end of get_note_from_obj();




// for more then one note of the same type

list 
get_notes_from_obj(int id, string notetype)

{
    list notelist;
    string query;

    query = "note[${notetype} && obj_id = ${id}]";
    notelist = list_select(query);

    if (list_count(notelist) < 1)
	return NULL;

    return notelist;
}




list 
get_items_from_note(note the_note)

{
    last_desc = "";

    last_desc = the_note.desc;    

    return items_from_note(the_note);
}


item get_an_item_from_obj_notetype_itemtype(int id, string n_t, string i_t)

{
    list itml;
   
    itml = get_items_from_obj(id, n_t);

    if (itml != NULL)
	return get_particular_item(itml, i_t);
    else
	return NULL;
} // end of get_an_item_from_obj_and_notetype_itemtype()




list 
get_items_from_obj(int id, string notetype)

{
    list notelist;
    note the_note;
    string query = "note[${notetype} && obj_id = ${id}]";

    if (id < 2)
	return NULL;

    last_desc = "";
    notelist = list_select(query);

    if (list_count(notelist) != 1)
    	return NULL;

    the_note = list_get(notelist, 0);

    return get_items_from_note(the_note);

} // end of get_items_from_obj()



string
end_sentence()

{
    return ".\n\n\n";
}

string 
is_concluded_on(list itml)

{
    string out;
    item itm;

    itm = get_particular_item(itml, "TIsConcludedOn");
    if (itm != NULL)
	out = "\tis concluded on \"" + itm.value + "\";\n";
    else 
	out = "";
	
    return out; 

} // end is_concluded_on() 


string 
is_selected_by(list itml)

{
    string out;
    item itm;

    itm = get_particular_item(itml, "TIsSelectedBy");
    if (itm != NULL)
	out = "\tis selected by \"" + itm.value + "\";\n";
    else 
	out = "";
	
    return out; 

} // end is_selected_by() 




// MORE CHECKING TO DETERMINE VALIDITY?
// HOW ABOUT GENERAL CONDITIONS?? ANOTHER NOTE?

string 
acts_only_if(list itml)

{
    string out;
    item itm;

    itm = get_particular_item(itml, "TActsOnlyIf");
    if (itm != NULL)
	out = "\tacts only if " + itm.value + ";\n";
    else 
	out = "";
	
    return out; 

} // end acts_only_if()




string 
satisfies(list itml)

{
    string out;
    item itm;

    itm = get_particular_item(itml, "TSatisfies");
    if (itm != NULL)
	out = "\tsatisfies " + itm.value + ";\n";
    else 
	out = "";
	
    return out; 

} // end satisfies()





// return values:
//     0 -- no item present
//     1 -- item == True
//     2 -- item == False

int 
get_specific_bool_item(list itml, string itemname)

{
    item itm;

    itm = get_particular_item(itml, itemname);
    if (itm != NULL) 
    {
	if (itm.value == "True")
	{
	    return 1;
	}
	else
	    return 2;
    }
    return 0;

} // end get_specific_bool_item()




// Print heading and closing /////////////////////////////////////////////////
// 
//  For the heading: print_header()
//                   print_std_include()
//                   print_S_Packet();
//
//  For the closing: print_remaining_stuff()
//                   print_ending()
//
//////////////////////////////////////////////////////////////////////////////



string
print_header(string product)

{
    string out = "\n\n";
    
    out += "/*\n\t" + product + " -- StP/T Software Description File \n";
    out += "\tGenerated at " + time_to_string(time_now(), NULL) + " by " + user() + " on host " + hostname() + "\n";
    out += "*/\n\n";

    return out;
}




string
print_std_include()

{
    string out = "";

    out += "#include <tsdl.std>\n";
    out += "#include <stp.std>\n";
    out += "\n\n";

    return out;
}




// For adding user include files. These files must either be relative 
// to the include directory, or full pathnames.

string 
print_user_include_files(list item_l) 

{
    int i;
    item itm;
    string out;

    out = "";

    for (i = 0; i < list_count(item_l); i++)
    {
	itm = list_get(item_l, i);
	if (itm.type == "TIncludeFile")
	{
	    if (out == "")
		out = "\n/* User include file(s) */\n\n";

	    out += "#include \"" + itm.value + "\"\n";
	}
    }
    if (out != "")
	out += "\n\n";

    return(out);

} // end of print_user_include_files() 




string 
print_remaining_stuff(int placeholder)
 
{
    string out;

    out = print_placeholder(placeholder);
    print_warning("Completing DataItems and DataTypes ...");
    out += print_dataitems_and_datatypes();
    out += print_constants(Constants);
    out += print_ending();

    return out;
}

string
print_ending()

{
    string out = "\n";
    
    out += "pe_mark\n";

    return out;
}


string
print_S_Packet(list itml)

{
    string out;
    item itm;
    string name, subject, desc;

    // name
    itm = get_particular_item(itml,  "TS_PacketName");
    if ((itm != NULL))
        name = itm.value;
    else
	name = "UnNamed_S_Packet";

    // subject
    itm = get_particular_item(itml,  "TS_PacketSubject");
    if ((itm != NULL))
        subject = itm.value;
    else
	subject = "UnSpecified S_Packet Subject";

    // name
    itm = get_particular_item(itml,  "TS_PacketDesc");
    if ((itm != NULL))
        desc = itm.value;
    else
	desc = "UnSpecified S_Packet Description";
 
    out = "S_Packet " + Tid(name) + "\n";
    out += "\thas subject \"" + subject + "\";\n";
    out += "\thas description \"" + desc + "\";\n";
    out += end_sentence();

    return out;
}



string 
fix_dataitem_name(string name, string scope)

{
    if (scope != NULL && scope != "")
	return Tid(name) + "_" + Tid(scope);
    else
	return Tid(name);
}


string 
fix_type_name(string name, string scope)

{
    if (name == "integer")
	return "TInteger";
    if (name == "real")
	return "TReal";
    if (name == "character")
	return "TCharacter";
    if (name == "string")
	return "TString";
    //if (name == "unspecified")
    //	return "TUnspecified";

    if (scope != NULL && scope != "")
	return Tid(name) + "_" + Tid(scope);
    else
	return Tid(name);
}



// print_dataitems_and_datatypes() ///////////////////////////////////////////
//
// There are three globals that accumulate DataTypes, DataItems and Constants
//  to be included in a tsdl file. They are:

list DataTypes;
list DataItems;
list Constants;
    
// Then you can add dataitems with:
//
// add_dataitem()    
//
// And add constants with
//
// add_constant()
//
// For actions, fill up Uses and
// Produces lists, then just print out the proper uses and produces clause
// at the appropriate time.
// 
// add_u_p()
// print_u_p()
// 
// If you have properly accumulated all the dataitems and data types, one call
// to is all you need to get them all into TSDL:
//
// print_dataitems_and_datatypes()
//
// and print constants with
//
// print_constants()
//
//////////////////////////////////////////////////////////////////////////////


boolean 
good_constant(string str, string tname)

{
    if (string_converts(str, "int") ||
        string_converts(str, "float") ||
        (string_extract(str, 0, 1) == "_"))
    {
	print_warning("[T-??] Error: DataType `" + tname + "', boolean attribute `" + str + "' not a constant. Using default value.");
	return False;
    }

    return True;
}




string 
has_bool_value(string bv, string noteval, list itml, string defbv, string tname)

{
    string out;
    item itm;
    out = "";

    itm = get_particular_item(itml, noteval);

    if (itm != NULL && itm.value != "" && good_constant(itm.value, tname))    
    {
	out += "\thas value " + bv + " " + Tid(itm.value) + ";\n";
        add_constant(Constants, itm.value, itm.value, itm.obj_id);
    }
    else
        out += "\thas value " + bv + " " + defbv + ";\n";

    return out;

} // end has_bool_value()



string 
fix_invalid(string old)

{
    string out, invalid_type;

    out = "";
    invalid_type = "";

    if (old == "OutOfBounds")
	invalid_type = "out_of_bounds";

    if (old == "BelowBounds")
	invalid_type = "below_bounds";

    if (old == "AboveBounds")
	invalid_type = "above_bounds";

    if (old == "OutOfType")
	invalid_type = "out_of_type";

    if (old == "NotInList")
	invalid_type = "not_in_list";

    if (old == "OutOfType")
	invalid_type = "out_of_type";

    if (old == "Abnormal")
	invalid_type = "abnormal";

    if (invalid_type != "")
	out = "\thas invalid subdomain " + invalid_type + ";\n";

    return out;

} // end of fix_invalid






string 
datatypeclass_and_specifiers(list itml, int att_id, string tname)

{
    string out, str;
    item itm;

    itm = get_particular_item(itml, "TDataTypeClass");
    if (itm == NULL)
        itm = get_particular_item(itml, "DataType");
	
    if (itm != NULL && itm.value == "boolean")
    {
	out = "\tis datatypeclass " + Tid(itm.value) + ";\n";
	out += has_bool_value("false", "TFalseValue", itml, "FALSE", tname);
	out += has_bool_value("true", "TTrueValue", itml, "TRUE", tname);
	out += has_bool_value("unknown", "TUnknownValue", itml, "UNKNOWN", tname);
    }
    else if ((itm != NULL) &&
             (itm.value == "integer" || itm.value == "real"))
    {
	out = "\tis datatypeclass " + Tid(itm.value) + ";\n";
	str = has_values(itml, "values", True, att_id);
	if (str != NULL)
	    out += str;
	else
	    out += range_definitions(tname, itml);
    }
    else if ((itm != NULL) &&
             (itm.value == "character" || itm.value == "string"))
    {
	out = "\tis datatypeclass " + Tid(itm.value) + ";\n";
	str = has_values(itml, "values", False, att_id);
	if (str == NULL) 
	{
	    print_warning("[T-18] Error: DataType `" + tname + 
                          "': incomplete definition.");
	}
	else
	    out += str;
    }
    else if ((itm != NULL) &&
             (itm.value == "unspecified"))
    {
	out = "\tis datatypeclass unspecified;\n";
	str = has_values(itml, "placeholder value", True, att_id);
	if (str == NULL) 
	{
	    print_warning("[T-18] Error: DataType `" + tname + 
                          "': incomplete definition.");
	}
	else
	    out += str;
    }
    else 
    {
	// the datatype is not primitive, just return NULL and do not create
        // this type.  Warnings have already been given.
	return NULL;
    }

    // now get the remainder of the annotations: format, testunits,
    // invalid subdomain, valid subdomain

    itm = get_particular_item(itml, "TFormat");
    if (itm != NULL)
	out += "\thas format \"" + itm.value + "\";\n";

    itm = get_particular_item(itml, "TTestUnits");
    if (itm != NULL)
	out += "\thas units \"" + itm.value + "\";\n";

    itm = get_particular_item(itml, "TInvalidSubdomain");
    if (itm != NULL)
	out += fix_invalid(itm.value);

    out += "\thas valid subdomain as_specified;\n";
    return out; 

} // end datatypeclass_and_specifiers()





string 
range_definitions(string tname, list itml)

{
    string out = "";
    item itm;
    int i, n;
    boolean warn = False;

    itm = get_particular_item(itml, "TMinimumValue");
    if (itm != NULL)
    {
        out += "\thas value range minimum " + itm.value + ";\n";
	// a constant ??
	check_and_add_constant(Constants, itm.value);
    }
    else
	warn = True;

    itm = get_particular_item(itml, "TMaximumValue");
    if (itm != NULL)
    {
        out += "\thas value range maximum " + itm.value + ";\n";
	// a constant ??
	check_and_add_constant(Constants, itm.value);
    }
    else
	warn = True;

    itm = get_particular_item(itml, "TResolution");
    if (itm != NULL)
    {
        out += "\thas value range resolution " + itm.value + ";\n";
	// a constant ??
	check_and_add_constant(Constants, itm.value);
    }
    else
	warn = True;

    if (warn)
	print_warning("[T-18] Error: DataType `" + tname + 
            "': incomplete definition.");

    return out;

} // end of range_definitions()
 

boolean 
is_T_regex(string str)

{
    string nstr;
    // just see if the string starts and ends with two slashes

    nstr = string_strip(str, "B", "//");
    if (string_length(str) == (string_length(nstr) + 4))
	return True;

    return False;
}



string 
has_values(list itml, string words, boolean no_quotes, int att_id) 

{
    string out;
    item itm;
    list itml2;
    int i, n;
    out = "";
    string query;

    if (OmtMode)
    {
	itml2 = get_particular_items(itml, "TTestValue");
	// and the default value
	itm = get_an_item_from_obj_notetype_itemtype(att_id, 
                 "OMTAttributeDefinition", "OMTAttributeDefaultValue");
	if (itm != NULL)
	{
	    if (itml2 != NULL)
	        list_insert(itml2, 0, itm);
	    else
	    {
		itml2 = list_create("item", 0);
		list_append(itml2, itm);
	    }
	}
    }
    else // SE
	itml2 = get_particular_items(itml, "AllowedValue");

    if (itml2 == NULL)
	return NULL;

    n = list_count(itml2);
    if (n > 0)
	out += "\thas " + words + " ";

    for (i = 0; i < n; i++)
    {
	itm = list_get(itml2, i);
	if (no_quotes || is_T_regex(itm.value))
	{
	    // Do not use Tid here, as that will remove all sorts of special
            // characters.
	    out += "\n\t\t" + itm.value;
	    if (! OmtMode)
		check_and_add_constant(Constants, itm.value);
	}
	else
	    out += "\n\t\t\"" + itm.value + "\"";


	if (i + 1 < n )
	    out += ",";
	else
	    out += ";\n";
    }
	
    if (out == "")
	return NULL;

    return out; 

} // end has_values()





string
print_dataitems_and_datatypes()
{
    int i, n;
    string out = "";
    string searchtype, typename, tname, tstr;
    dataitem_tp di; 
    datatype_tp dy;
    list itml;

    for(i = 0; i < list_count(DataItems); i++)
    {
	di = list_get(DataItems, i);
	dy = list_get(DataTypes, di.type_index);

	out += "Dataitem " + fix_dataitem_name(di.name, dy.scope) + "\n";
	out += "\tis an instance of datatype ";
	out += fix_type_name(dy.name, dy.scope) + ";\n";
	out += end_sentence();
    }

    for(i = 0; i< list_count(DataTypes); i++)
    {
	dy = list_get(DataTypes, i);
 
	// do not redo built in typeclasses defined in stp.std 

	typename = fix_type_name(dy.name, dy.scope);
	searchtype = " " + typename + " ";
	if (string_find(BuiltInTypes, 0, searchtype) == 
	    string_length(BuiltInTypes))
	{
	    // Find an annotation for typename 

	    if (OmtMode)
		itml = get_omt_type_info(dy);
	    else // SE
	    {
		if (dy.annot_id != -1)
		    itml = get_items_from_obj(dy.annot_id, "DataDefinition");
		else
		    itml = get_items_from_name(dy.name, "DataDefinition");
	    }

	    tname = Tid(typename);
	    tstr = datatypeclass_and_specifiers(itml, dy.annot_id, tname);

	    if (tstr != NULL)
	        out += "Datatype " + tname + "\n" + tstr + 
                      get_description_from_id(dy.annot_id) + end_sentence();
	}
    }
     
    return out;

} // end print_dataitems_and_datatypes()





// This function is used by omt for functional models. Currently, we need
// to find the name of the flow in the class's attribute table, and use 
// that type.  Kind of messy, as flows are not separately annotatable with
// dataitem and type info.

string
type_from_link(type_detail_tp type_detail)

{
    return "integer";

} // end of type_from_link() 




string 
get_dataitem_type(type_detail_tp type_detail)

{
    item itm;

    if (type_detail.name != NULL)
	return type_detail.name;

    if (type_detail.from_link != NULL)
	return type_from_link(type_detail);

    if (type_detail.from_node != NULL)
	return attribute_type(type_detail.from_node);

    if (type_detail.from_cntx != NULL)
    {
	// use annotation info for the type name
        itm = get_an_item_from_obj_notetype_itemtype(type_detail.annot_id,
				     "DataDefinition", "DataType");
	if (itm != NULL)
	    return itm.value;
    }


    return "integer";

} // end of  get_dataitem_type()






/// routines to deal with constants
//  check_and_add_constant() -- adds one if string given in DSE, or is 
//    neither int or float
//  print_constants() prints all the constants into a string
//  add_constant() adds one to the list
//  find constant() finds a constant, and returns the const structure.


// Note that we do not support this is OMT.

constant_tp
check_and_add_constant(list cl, string con)

{
    constant_tp c;
    node nde;
    list itml;
    item itm;
    string query;

    if (OmtMode)
	return NULL;

    if (con == "*")
	return NULL;

    // now we want to see if we have a constant by this cell's name
    if ((c = find_constant(cl, con)) == NULL)
    {
	// look to see if there is a constant in the DB.  This is 
	// a box in dse with type = integer or constant and an allowed value

	query = "node[Sequence && node_refs && name = '${con}']";
	nde = find_by_query(query);
	if (nde != NULL)
	{
	    itml = get_items_from_obj(nde.id, "DataDefinition");
	    itm = get_particular_item(itml, "DataType");
	    if (itm != NULL && 
		(itm.value == "constant" || itm.value == "integer" ||
		 itm.value == "real" || itm.value == "float"))
		itm = get_particular_item(itml, "AllowedValue");
	    if (itm != NULL)
		c = add_constant(cl, con, itm.value, nde.id);
	    else
	    {
		print_warning("[T-22] Warning: Constant `" + 
		    con + "': incomplete definition in DSE.");
		add_constant(cl, con, con, nde.id);
	    }
	}
	else
	{
	    // issue a warning unless this is an integer or float.

	    if (! (string_converts(con, "int") || 
		   string_converts(con, "float")))
	    {
		print_warning("[T-23] Warning: Constant `" + 
		    con + "': no definition found in DSE.");
		add_constant(cl, con, con, -1);
	    }
	}
    }
    return c;

} // end of check_and_add_constant()




string print_constants(list cl)

{
    int i;
    string str, out;
    out = "";
    constant_tp c;

    for (i = 0; i < list_count(cl); i++)
    {
        c = list_get(cl, i);
	out += "Constant " + Tid(c.name) + "\n" +
	      "\thas internal value ";

	// no quotes for integers, quotes for everything else.
	if (string_converts(c.value, "int") ||
	    string_converts(c.value, "float"))
	    out += c.value;
	else
	    out += "\"" + c.value + "\"";

	out += ";\n" +  get_description_from_id(c.id) + 
              end_sentence();
    }
    return out;

} // end of print_constants()



constant_tp
find_constant(list cl, string name)

{
    int i;
    constant_tp c;

    if (name == "*")
	return NULL;

    for (i = 0; i < list_count(cl); i++)
    {
	c = list_get(cl, i);
	if (c.name == name)
	    return c;
    }
    return NULL;

} // end of find_constant()




void add_constant(list cl, string name, string value, int id)

{
    constant_tp c;

    if (name == "*")
	return;

    if (predefined_constant(name))
	return;

    if (find_constant(cl, name) == NULL)
    {
	c.id = id;
	c.name = name; 
	c.value = value;
	list_append(cl, c);
    }
    
    return;

} // end of add_constant()




int 
find_simple_dataitem(string sname)

{
    dataitem_tp di;
    int i, indx = -1;

    for (i = 0; i < list_count(DataItems); i++)
    {
	di = list_get(DataItems, i);
	if (di.simple_name == sname)
	{
	    if (indx != -1)
	    {
		print_warning("[T-XX] Warning: More then one type found for `" + sname + "'.");
		return indx;
	    }
	    indx = i;
	}
    }
    return indx;

} // end of find_simple_dataitem()



int 
find_dataitem(string name)

{
    dataitem_tp di;
    int i;

    for (i = 0; i < list_count(DataItems); i++)
    {
	di = list_get(DataItems, i);
	if (di.name == name)
	{
	    return i;
	}
    }
    return -1;

} // end of find_dataitem()




int 
add_dataitem(string name, type_detail_tp the_type_detail, string simple_name)

{
    int i, n, index;
    dataitem_tp new_di;
    datatype_tp dy, new_dy;
    string type_name;

    index = find_dataitem(name);
    if (index != -1)
	return index;

    new_di.name = name;
    new_di.simple_name = simple_name;

    // print_error("Adding dataitem: " + name + " and: " + simple_name);

    // now check to see if we have the type there 

    if ((type_name = get_dataitem_type(the_type_detail)) != "")
    {
	// issue warning if the type name is the same as the item name
	if (type_name == new_di.name)
	{
	    print_warning("[T-21] Error: DataInstance `" + new_di.name + 
"': Same instance and type name.\n\tGenerated SDF will contain errors.");

	}

        for (i = 0, n = list_count(DataTypes); i < n; i++)
	{
	    dy = list_get(DataTypes, i);
	    if (dy.name == type_name && dy.scope == the_type_detail.scope)
	    {
		new_di.type_index = i;
		list_append(DataItems, new_di);
		return list_count(DataItems) - 1;
	    }
	}

	new_dy.name = type_name;	
	new_dy.scope = the_type_detail.scope;
	new_dy.annot_id = the_type_detail.annot_id;
        list_append(DataTypes, new_dy);
	new_di.type_index = list_count(DataTypes) - 1;
    }
    else
    {
	// zero for "int"
	new_di.type_index = 0;
    }

    list_append(DataItems, new_di);
	
    return list_count(DataItems) - 1;

} // end add_dataitem()



// uses and produces lists  are just lists of pointers into the DataItems and 
// DataTypes data structures.  
// This should work, even if the DataItems list is not complete, as 
// that list is only appended to.

list
add_u_p(list uplist, int index)

{
    if (uplist == NULL)
	uplist = list_create("int", 0);

    list_append(uplist, index);
    return uplist;
}



string
print_u_p(list uplist, int u_or_p)  // 1 for used, 2 for produced
{
    int i, n;
    string out;
    out = "";
    dataitem_tp di;
    datatype_tp dy;

    n = list_count(uplist);

    if (n > 0) 
    {
	if (u_or_p == 1)
	    out += "\tuses dataitem";
	else
	    out += "\tproduces dataitem";
    }
    else
	return out;

    for(i = 0; i < n; i++)
    {
	if (i > 0)
	    out += ",\n\t\t";
	else
	    out += "\n\t\t";

	di = list_get(DataItems, list_get(uplist, i));
	dy = list_get(DataTypes, di.type_index);

	out += fix_dataitem_name(di.name, dy.scope);
        //out += Tid(di.name);
    }
    out += ";\n";

    return out;
}

// end dataitems and datatypes
//
//////////////////////////////////////////////////////////////////////////////



// actions ///////////////////////////////////////////////////////////////////
//
// routines to complete actions sentences
//

string
action_annots_and_end(int id)
 
{
    list itml;
    string out;

    out = "";
 
    // now the misc. annotation information
    itml = get_items_from_obj(id, "TestAction");

    if (itml != NULL)
    {
	out += is_selected_by(itml);
	out += is_concluded_on(itml);
	out += acts_only_if(itml);
	out += satisfies(itml);
	out += test_exhaustive_annots(itml);
    }

    out += get_description_from_id(id);
    out += end_sentence();

    return out;

} // end of action_annots_and_end()
 




string 
start_action_sentence(string name)

{
    string out;

    out = "Action " + Tid(name) + "\n";
    return out + "\tis actiontype internal;\n";

} // end of start_action_sentence()





string
actiontype()

{
    return "\tis actiontype internal;\n";
}





// Exhaustive testing is only specified in a test action, and it is potentially
// a list of items, all which need to be added to the exhaustive list.
// The very similar funtion below if for printing a tested exhaustively clause
// for process activiation tables in SE.

string 
test_exhaustive_annots(list item_l)

{
    int i, indx;
    string out, din;
    item itm;
    dataitem_tp di;
    datatype_tp dy;

    out = "";

    for (i = 0; i < list_count(item_l); i++)
    {
        itm = list_get(item_l, i);
        if (itm.type == "TExhaustive")
        {
	    // lookup name
	    indx = find_simple_dataitem(itm.value);
	    if (indx != -1)
	    {
		di = list_get(DataItems, indx);
                dy = list_get(DataTypes, di.type_index);
		din = fix_dataitem_name(di.name, dy.scope);
	    }
	    else
		din = Tid(itm.value);

            if (out == "")
                out = "\tis tested exhaustively on dataitem\n\t\t" + din;
	    else
		out += ",\n\t\t" + din;
        }
    }
    
    if (out != "")
	out += ";\n";
	
    return out; 

} // end test_exhaustive_annots()


 
string
print_tested_exhaustively(list cio_l)
 
{
    string din, out, query;
    list uplist;
    node cio;
    int i, indx;
    dataitem_tp di;
    datatype_tp dy;
 
    out = "";
 
    for (i = 0; cio_l != NULL && i < list_count(cio_l); i++)
    {
        cio = list_get(cio_l, i);
 
        if (cio.name != "")
        {
	    // lookup 
	    indx = find_simple_dataitem(cio.name);
	    if (indx != -1)
	    {
		di = list_get(DataItems, indx);
                dy = list_get(DataTypes, di.type_index);
		din = fix_dataitem_name(di.name, dy.scope);
	    }
	    else
		din = Tid(cio.name);

            if (out == "")
                out += "\tis tested exhaustively on dataitem\n\t\t" + din;
            else
                out += ",\n\t\t" + din; 
        }
    }
    if (out != "")
        out += ";\n";
 
    return out;
 
} // end of print_tested_exhaustively()



int PrintPlaceholder = 0;
 
string print_placeholder(int ph)
 
{
    string out;
    out = "";
    type_detail_tp td;

 
    if (ph)
    {
        out += start_action_sentence("placeholder_action");
        out = out +
              "\thas description \"placeholder for non-existent action\";\n";
        out += "\tuses dataitem T_Nothing;\n";
        out += "\tproduces dataitem T_Nothing;\n";
        out += end_sentence();

	td = clear_td();
	td.name = "integer";
	td.scope = "";
 
        add_dataitem("T_Nothing", td, "T_Nothing" );
    }
 
    return out;
 
} // end of print_placeholder()


 

// conditions //////////////////////////////////////////////////////////////////

string 
start_condition_sentence(string name)

{
    return "Condition " + Tid(name) + "\n";

} // end of start_condition_sentence() 




// NOTE: the part of this for constants is SE specific!! 

string
print_cond_clause(list cio_l, string clause, 
                  int dec_id, int fileid)
 
{
    string out, query, cvalue, din;
    list uplist, itml;
    item itm;
    node cio;
    link cell;
    int i, indx;
    constant_tp c;
    node nde;
    dataitem_tp di;
    datatype_tp dy;
 
    out = "";
 
    for (i = 0; cio_l != NULL && i < list_count(cio_l); i++)
    {
        cio = list_get(cio_l, i);
 
        query = "link[ControlValue && ${dec_id} = to_node_id &&
                  ${cio.id} = from_node_id && link_refs[file_id = ${fileid}]]";
        cell = find_by_query(query);

        if (cell != NULL && cell.name != "")
        {
	    cvalue = cell.name;

	    if (cell.name == "*")
		continue; 

	    // now we want to see if we have a constant by this cell's name
	    c = check_and_add_constant(Constants, cell.name);

	    if (c != NULL) 
		cvalue = c.name;
 
	    // lookup cio.name
	    indx = find_simple_dataitem(cio.name);
	    if (indx != -1)
	    {
		di = list_get(DataItems, indx);
                dy = list_get(DataTypes, di.type_index);
		din = fix_dataitem_name(di.name, dy.scope);
	    }
	    else
		din = Tid(cio.name);

            if (out == "")
                out = "\t" + clause + "\n\t\t((" + din + 
                      " == " + cvalue + ") ";
            else
                out += " && \n\t\t(" + din + " == " + 
		      cvalue + ")";
        }
    }
    if (out != "")
        out += ");\n";
 
    return out;
 
} // end of print_cond_clause()
 



// printing routines for TSDL states /////////////////////////////// 

string
print_state_sentence(string qualifier, string name, int desc_id)

{
    string out;
    note the_note;

    out = "";

    if (qualifier != NULL)
        out += "State " + Tid(qualifier) + "_" + Tid(name) + "\n";
    else
        out += "State " + Tid(name) + "\n";

    out += "\tis statetype action;\n";

    if (desc_id >= 0)
        out += get_description_from_id(desc_id);

    return out;

} // end of print_state_sentence()


 
string
print_st_sentence(string qualifier, link transition)
 
{
    node tnode, fnode;
    string out, query, tname, fname;
 
    out = "";
 
    query = "node[id = ${transition.from_node_id}]";
    fnode = find_by_query(query);
 
    query = "node[id = ${transition.to_node_id}]";
    tnode = find_by_query(query);

    // there are a few assumptions here about these names, namely if they 
    // are null, consider them start and stop states.  These are valid for
    // OMT, and harmless for SE.

    if (fnode.name == NULL || fnode.name == "")
	    fname = "Start"; 
    else
	fname = Tid(fnode.name);
    if (tnode.name == NULL || tnode.name == "")
	    tname = "Final"; 
    else
	tname = Tid(tnode.name);
 
    out += "Statetransition " + fname + "_" + tname +
          "\n\tis statetransitiontype action;\n";
    out += get_description_from_id(transition.id);
    if (qualifier != NULL)
	out += "\tgoes from first state " + qualifier + "_" + fname +
	      ";\n\tgoes to second state " + qualifier + "_" + tname + ";\n";
    else
	out += "\tgoes from first state " + fname +
	      ";\n\tgoes to second state " + tname + ";\n";
 
    return out + end_sentence();
 
} // end of print_st_sentence()
 




// Descriptions ///////////////////////////////////////////////////////////////
//

string last_desc = "";

string
last_description()

{
    string out; 

    out = "\thas description\n\t\t\"";
    if (last_desc != NULL && last_desc != "")
	out += string_search_and_replace(last_desc, "\n", "\"\n\t\t\"");
    else
	out += "Generated by StP/T";
    
    return out + "\";\n";

} // end last_description()




 
string
get_description_from_id(int id)
 
{
    note the_note = NULL;

    if (id != -1)
        the_note = get_note_from_obj(id, "TestDescription");

    if (the_note != NULL)
        last_desc = the_note.desc;
    else
	last_desc = "";

    return last_description();

} // end of get_description_from_id()


// For parsing args  

list 
build_input_list(string itemname, list itml)

{
    int i;
    item itm;
    list new_list;

    new_list = NULL;

    for (i = 0; i < list_count(itml); i++)
    {
	itm = list_get(itml, i);
	if (itm.type == itemname)
	{
	    if (new_list == NULL)
		new_list = list_create("string", 0);

	    list_append(new_list, itm.value);
	}
    }
    return new_list;
}


// clean out the testunit

int
clean_testunit_files(string tu)

{
    string shHead="";
    // Just call tdriver to do the job
    if (!is_unix_platform())
	shHead = "sh ";
    return system(shHead+"tdriver -quiet -p " + current_projdir() + " -s " + current_system() + " -U " + tu + " -Y");
}


int
primitive_type(string str)
 
{
    str = " " + str + " ";
    if (string_find(PrimitiveTypes, 0, str) != string_length(PrimitiveTypes))
        return 1;
 
    return 0;
}



int
predefined_type(string str)
 
{
    str = " " + str + " ";
    if (string_find(PreDefinedDataTypes, 0, str) != string_length(PreDefinedDataTypes))
        return 1;
 
    return 0;
}


int
predefined_constant(string str)
 
{
    str = " " + str + " ";
    if (string_find(PreDefinedConstants, 0, str) != string_length(PreDefinedConstants))
        return 1;
 
    return 0;
}


// This is like do_qualify_name() in se/rules/qrl/qualify.inc, but uses different separators.

string 
get_dse_object_scope(node kid) 

{
    node parent;
    string file_dir = "", fname = "", dname = "", rootname = "", scope;

    if (kid == NULL)
	return "";
    else
    {
	scope = kid.name;
	rootname = scope;
    }

    if (TDebug)
	print_error("...Finding scope chain for :" + kid.name);
  
    while ((parent = find_by_query("node[id = ${kid.scope_node_id}]")) != NULL)
    {
	if (parent.type == "SEFile")
	    fname = parent.name;
	else if (parent.type == "SEDirectory")
            dname = parent.name;
	else
	{
	    scope = parent.name + "_" + scope;
	    rootname = parent.name;
	}
	kid = parent;
    } 

    if (TDebug)
	print_error("...Found: " + scope);

    // file and directory

    if (fname != "" && (rootname + ".h" != fname))
    {
        if (dname == "")
	    file_dir = fname;
	else
	    file_dir = dname + "_" + fname;
	   
	scope += "_" + file_dir;
    }

    if (TDebug)
	print_error("...Found full scope: " + scope);

    return scope;
}
