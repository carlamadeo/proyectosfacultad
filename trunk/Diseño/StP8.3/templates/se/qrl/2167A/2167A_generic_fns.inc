#include "qrl/include/ALL_Globals.inc"
#include "qrl/include/ALL_Report.inc"
#include "qrl/include/ALL_PrintDiagram.inc"

//////////////////////////////////////////////////////////////////////
//
//	2167A Generic Functions
//
//
//	Only functions that are common to all 2167A documents are included
//	in this file.
//
//////////////////////////////////////////////////////////////////////

//
//	Document Date
//
//	This function returns the system date as understood by
//	the Unix command "date".

string
document_date ()
{
    return time_to_string(time_now(),"%b %d, %Y");
}


//
//	ERRORS and WARNINGS
//
//	Beyond QRS and OMS errors, there are additional errors that
//	are specific to any given template.  The following functions
//	help display errors/warnings and tabulate the total errors 
//	encountered.
//
//	Errors is any situation where the document will probably have
//	to be regenerated whereas Warnings can be ignored.

int	DOD_error_count = 0;
int	DOD_warning_count = 0;

void
display_error (string msg)
{
    message ("");
    message ("DOD ERROR: " + msg);
    message ("");
    DOD_error_count = DOD_error_count + 1;
}

void
display_warning (string msg)
{
    message ("DOD WARNING: " + msg);
    DOD_warning_count = DOD_warning_count + 1;
}

void
display_fault_status ()
{
    if ((DOD_error_count > 0) || (DOD_warning_count > 0))
	    message ("");
    message ("");
    message ("DOD Fault Status");
    message ("  " + DOD_error_count + " Error(s) encountered");
    message ("  " + DOD_warning_count + " Warning(s) encountered");
    message ("");
}

//
// This function returns the project unique identifier associated with
// the "ident".  This object may be a link or a node of some sort.  The
// global variable "use_StP_generated_identifiers" will be tested to
// determine if the object identifier (i.e. just return the parameter)
// or the user supplied annotation is the the "project unique identifier".

string
find_unique_identifier (int ident)
{
    item one_note_item;

    if (use_StP_generated_identifiers)
	    return to_string(ident);
    else
    {
	one_note_item = find_by_query 
		("item[obj_id = ${ident} && ProjectUniqueIdentifier]");
	if (one_note_item != NULL)
		if (one_note_item.value != NULL)
			if (string_length(one_note_item.value) > 0)
				return one_note_item.value;
				
	// should only execute this if the object identifier is deficient
	// in some manner.
	display_warning ("No Identifier annot attached to the node/link " + 
		"with the identifier " + ident);
	return "??";
    }
}

// PRINT_NOTE_DESCRIPTION
//
// This function prints out the first note description that is found
// attached to an object with the "IDENT"ifier where the note is "NOTE_TYPE".  
// This description will be printed using the given "desc_style".
	
void
print_note_description (int ident, string desc_style, string note_type)
{
    note	desc_obj;
    int		cnt = 0;
    
    for_each_in_select ("note[obj_id = ${ident} && ${note_type}]", desc_obj)
    {
		paragraph (desc_style);
		print (strip_last_return(desc_obj.desc));
		cnt = cnt + 1;
    }
    
    // 01/21/94 JHSIA: Added warning.  Cnt instead of selection_count for speed
    if (cnt == 0)
		display_warning ("No " + note_type + " annotation note type found on"
			+ " link/node/cntx with id of " + ident);
    
}


// INCLUDE_EXTERNAL_INTERFACE_DIAGRAMS
//
// This function prints all the data flow diagrams where external
// interfaces are contained.  External interfaces should already be
// selected and should now be retrievable via the OMS id list called
// "external_interfaces".

void
include_external_interface_diagrams (string caption)
{
    file	one_fd;
    link	one_flow;
    link_ref	one_ref;
    list	ref_list;
    set		file_set=set_create("file");
    int		i;
    string	query, query2;
    
    // Include the top diagram
    one_fd = find_by_query ("file[DfeDiagram && name = 'top']");
    if (one_fd == NULL)
    {
		display_warning ("The data flow diagram 'top' is not defined");
    }
    else
    {
    	message ("        .. top.dfe");
        PrintDiagram( one_fd, True, "", "", "", "", "External Interface Diagram ", "" );
    }
	    
    // Include all flow diagrams where the external flows are found.
    //PEC mod 11/17/95 changing this query to look for DFD's where 
    //links have same name as external_interface names, otherwise
    //it will never work.  This will take a few queries.....
    //WRONG query = "file[DfeDiagram && link_refs[link[external_interfaces]] "
    //              + "&& name != 'top'] sort by name";

    query="link[external_interfaces]";
    for_each_in_select (query, one_flow)
    {
	query2="link_ref[link[name=='" +
		one_flow.name + "' &&(to_node[OffPageExternal||FlowAnchor]" +
		" || from_node[OffPageExternal||FlowAnchor])]]";
	ref_list=list_select(query2);
	for(i=0;i<list_count(ref_list);i++)
	{
	    one_ref=list_get(ref_list,i);
	    one_fd=find_by_query("file["+one_ref.file_id+"]");
	    //Make sure we get a unique set of files
	    set_add(file_set,one_fd);
	}
    }
    //Print the files
    for(i=0;i<set_count(file_set);i++)
    {
	one_fd=set_get_element(file_set,i);

    	message ("        .. " + one_fd.name + ".dfe");
        PrintDiagram( one_fd, True, "", "", "", "", "External Interface Diagram ", "" );
    }
}


// GET_NOTE_ITEM
//
// A simple function that returns the value of a note item when given the
// "NOTE_ID" of the note that this item is associated with and the "ITEM NAME".

string
get_note_item (int note_id, string item_name)
{
    string		query;
    item		note_item;
    
    query = "item[note_id=${note_id} && ${item_name}]";
    note_item = find_by_query (query);
    if (note_item != NULL)
    {
	return note_item.value;
    }
    else
    {
	return "";
    }
}

// GET_NOTE_DESCRIPTION
//
// This function returns the first note description that is found
// attached to an object with the "IDENT"ifier where the note is "NOTE_TYPE".  
	
string
get_note_description (int ident, string note_type)
{
    note    desc_obj;

    desc_obj = find_by_query ("note[obj_id = ${ident} && ${note_type}]");
    if (desc_obj == NULL)
	return "";
    else
	return strip_last_return(desc_obj.desc);
}


// STRIP_LAST_RETURN
//
// All descriptions seem to have an extra carriage return so let's strip
// it off.

string
strip_last_return (string desc)
{
    int    	pos;
    
    if (desc == NULL)
		return "";
    else if (string_length(desc) == 0)
		return "";
    else
    {
    	// 1/11/94 JHSIA: strip off all trailing returns & spaces
		pos = string_length(desc) - 1;
		while ((pos > 0) && 
			   ((string_extract(desc, pos, 1) == " ") ||
			    (string_extract(desc, pos, 1) == "\n")))
		{
			pos = pos - 1;
		}
		
	    return string_extract(desc, 0, pos + 1);
    }
}

// SET_GET_INDEX_OF_INT_ELEMENT
//
// Apparently, there is no built in function that searches a set for a
// value and then returns the index where it is found.  

int
set_get_index_of_int_element (set set1, int element)
{
    int    i;
    
    for (i=0; i<set_count(set1); i=i+1)
    {
	if (element == set_get_element(set1, i))
		return i;
    }
    return -1;
}


// SET_GET_INDEX_OF_STRING_ELEMENT
//
// Apparently, there is no built in function that searches a set for a
// value and then returns the index where it is found.  

int
set_get_index_of_string_element (set set1, string element)
{
    int		i;
    
    for (i=0; i<set_count(set1); i=i+1)
    {
	if (element == set_get_element(set1, i))
		return i;
    }
    return -1;
}


// SET_TO_STRING
//

string
set_to_string (set the_set, string separator)
{
    int		i;
    string	return_string;
    
    return_string = "";
    for (i=0; i<set_count(the_set); i=i+1)
    {
	if (i>0)
	    return_string = return_string + separator;
	return_string = return_string + set_get_element(the_set, i);
    }
    return return_string;
}




//
// Prints the names of the interfaces in a comma list.  
// Intended for use in the Identification Paragraph, where it inserts a
// list of the interfaces.
//
void
print_interface_name_list (string note_name)
{
    string query;
    boolean first_time = True; 
    item one_item;
    int num_items;
    int current_item = 0;


    query = "item[InterfaceName && note[" + note_name + " && " +
	    "link[external_interfaces]]]";

    num_items = selection_count(query);
    if (num_items > 0)
    {
	for_each_in_select(query, one_item)
	{
	    current_item = current_item + 1;
	    if (first_time == True)
	    {
		first_time = False;
		print(one_item.value);
	    }
	    else
	    {
		if (current_item == num_items)
		    print(", and " + one_item.value);
		else
		    print(", " + one_item.value);
	    }
	}
    }
}



//
// used in the IDD template to print the value of a note item
// preceded by the given label
//
void
print_item(int note_id,  string item_name, string label)      
{
    string query;
    string one_item = "";
    
    one_item = get_note_item(note_id, item_name);
    if (one_item != "")
    {
	print_line ();
	print(label + ": " + one_item);
    }
}

//
//  The 2167A docs use tables and diagrams, so insure that output target
//  supports them.
//
boolean
legal_target ()
{
	if (target() == Ascii)	{
	    display_error ("Target Format must support diagrams and tables.");
	    return False;
	}
	else
	    return True;
}
