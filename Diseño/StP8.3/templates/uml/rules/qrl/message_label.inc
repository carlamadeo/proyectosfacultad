
//      StP/UML
//      Confidential property of Aonix
//      Copyright (c) 1997
//      All rights reserved
//

#include "rules/qrl/interaction_dg.inc"
#include "rules/qrl/message_rename.inc"
//
// Functions for parsing message labels and [to be added] GOR funcs
//

const int PredecessorGuardComponent = 0;
const int SequenceValComponent = 1;
const int RecurrenceComponent = 2;
const int ReturnTypeComponent = 3;
const int MessageNameComponent = 4;
const int MessageArgsComponent = 5;

// Parse the label into component parts. Since this will be invoked several
//consecutive times for the same label, do some caching
string lastLabel = "";
list lastCompList = NULL;

list uml_get_label_components(gde_mapsymbol sym)
{
    string aLabel;

    // allow nodes because of ExtensionPoint elements
    if (gde_mapsymbol_is_cntx(sym) || gde_mapsymbol_is_node(sym))
    {
	aLabel = gde_node_label(to_gde_node(sym));
    } else if (gde_mapsymbol_is_link(sym))
    {
	aLabel = gde_link_label(to_gde_link(sym));
    } else {
 	message("is NOT link NOR cntx!!");
 	return NULL;
    }

    return(uml_parse_message_label(aLabel));
}

string uml_compose_message_label(list aLabelList)
{
    string message_name = list_get(aLabelList,MessageNameComponent);
    string message_args = list_get(aLabelList,MessageArgsComponent);
    string result;
    
    if(message_args != NULL && message_args != "")
	return message_name + "(" + message_args + ")";
    else
	return message_name;
}

// ECR 6634 begin
// Function returns position of the last occurence of to_find in the_string
// returns -1 if the_string does not contain to_find
//
int string_find_last(string the_string, int start, string to_find)
{
    int last_pos = -1;
    int pos = start;
    int len = string_length(the_string);
    
    while (1)
    {
        pos = string_find(the_string, pos, to_find);
        if (pos == len)
            break;

        last_pos = pos;
        pos++;
    }

    return last_pos;
}

void uml_parse_seqretsig_expr(string string_to_parse, list result)
{
    // string_to_parse is in format
    //     sequence-expr ':' ret-value ':=' msg-name '(' arg-list ')'
    // this function parses these 4 parts and stores them in result list
    // if any of these parts is missing, this functions places empty string "" in list

    // Since sequence-expr and arg-list can contain colons ':', the only reliable method
    // to parse label is:
    //  1) extract the arg-list - by counting opening and closing parenthesises
    //  2) extract the msg-name - string after assignment ':=' (or after colon ':', if there is no assignment)
    //  3) if there is assignment ':=', extract ret-value - string after colon ':'
    //  4) the rest (if any) is sequence-expr

    int has_ret_value;
    int has_sequence_expr;
    int pos, idx, old_idx;
    int paren_count;

    string message_name = "";
    string arguments = "";
    string ret_value = "";
    string sequence_expr = "";
    string curr_char;
    string str;
     
	str = string_strip(string_to_parse, "B", " ");
    if (0 == string_length(str))
    {
        // label is empty

        list_append(result, "");
        list_append(result, "");
        list_append(result, "");
        list_append(result, "");
        return;
    }

    // Parse arg-list

    paren_count = 0;
    idx = string_length(str) - 1;
    old_idx = idx;
    curr_char = string_extract(str, idx, 1);
    if (curr_char == ")")
    {
        while (idx >= 0)
        {
            curr_char = string_extract(str, idx, 1);
            if (curr_char == "(")
            {
                paren_count--;
                if (paren_count == 0)
                    break;
            }
            else if (curr_char == ")")
                paren_count++;

            idx--;
        }
    }
    if (idx != old_idx)
    {
        // there is arguments

        arguments = string_extract(str, idx + 1, old_idx - idx - 1);
        str = string_extract(str, 0, idx);
    }
	arguments = string_strip(arguments, "B", " ");

    // Now parse msg-name

    has_ret_value = 1;
    has_sequence_expr = 1;
    pos = string_find_last(str, 0, ":=");
    if (pos != -1)
    {
        message_name = string_extract(str, pos + 2, string_length(str) - pos - 2);
        str = string_extract(str, 0, pos);
    }
    else
    {
        has_ret_value = 0;
        pos = string_find_last(str, 0, ":");
        if (pos != -1)
        {
            message_name = string_extract(str, pos + 1, string_length(str) - pos - 1);
            str = string_extract(str, 0, pos);
        }
        else
        {
            has_sequence_expr = 0;
            message_name = str;
            str = "";
        }
    }
	message_name = string_strip(message_name, "B", " ");

    // Now parse ret-value

    if (has_ret_value)
    {
        pos = string_find_last(str, 0, ":");
        if (pos != -1)
        {
            ret_value = string_extract(str, pos + 1, string_length(str) - pos - 1);
            str = string_extract(str, 0, pos);
        }
        else
        {
            has_sequence_expr = 0;
            ret_value = str;
            str = "";
        }
    }
	ret_value = string_strip(ret_value, "B", " ");

    // The rest is sequence-expr

    if (has_sequence_expr)
	    sequence_expr = string_strip(str, "B", " ");
    else
        sequence_expr = "";

    list_append(result, sequence_expr);
    list_append(result, ret_value);
    list_append(result, message_name);
    list_append(result, arguments);
}
// ECR 6634 end


list uml_parse_message_label(string aLabel)
{
    if (aLabel == lastLabel && lastCompList != NULL) 
    {
        return lastCompList;
    } 
    else 
    {
	    lastLabel = aLabel;
	    lastCompList = list_create("string", 0);
    }

    string predecessorGuard = "";
    string sequenceVal = ""; 
    string recurrenceVal = "";
    string returnVal = "";
    string messageName = "";
    string argumentList = "";

    if (aLabel != "" && aLabel != NULL) 
    {
	    list tempSplit = uml_split_string(aLabel, "/", False, False);
	    predecessorGuard = list_get(tempSplit, 0);
	    string remaining = list_get(tempSplit, 1);
	    string seqExpression; 

	    // the separator for sequence expression is ":", which also ocurrs in the
	    // separator for return values.  Check the latter one first.  If ":=" not
	    // found, there is no return value and proceed with checking for ":".
	    if (remaining != "" && remaining != NULL) 
        {
// ECR 6634 begin
            list temp_list = list_create("string", 0);
            uml_parse_seqretsig_expr(remaining, temp_list);

            seqExpression = list_get(temp_list, 0);
            returnVal     = list_get(temp_list, 1);
            messageName   = list_get(temp_list, 2);
            argumentList  = list_get(temp_list, 3);

            list_clear(temp_list);
// ECR 6634 end

	        // extract recurrence, if it is specified
	        // check for iteration first
	        // to handle conditions involving '*', check if '*' comes before '['
	        //if(seqExpression!="" && seqExpression!=NULL && string_find(seqExpression,0,"*") < string_length(seqExpression))
	        if (seqExpression != "" && seqExpression != NULL) 
            {
		        int splitPoint = string_find(seqExpression, 0, "*");
		        int split2 = string_find(seqExpression, 0, "[");
		        if (splitPoint == split2) 
                { 
                    // if equal, there is no recurrence
		            sequenceVal = seqExpression;
		            recurrenceVal = "";
		        } 
                else 
                {
		            if (splitPoint > split2)
			            splitPoint = split2;
		            sequenceVal = string_extract(seqExpression,0,splitPoint);
		            recurrenceVal = string_extract(seqExpression,splitPoint,string_length(seqExpression)-splitPoint);
		        }
	        }
	    }
    }

    // assemble result
    list_append(lastCompList,predecessorGuard);
    list_append(lastCompList,sequenceVal);
    list_append(lastCompList,recurrenceVal);
    list_append(lastCompList,returnVal);
    list_append(lastCompList,messageName);
    list_append(lastCompList,argumentList);

    return(lastCompList);
}

string uml_get_label_component(gde_mapsymbol sym,int pos)
{
    //message("DEBUG uml_get_label_component @ pos="+pos);
    list allComps = uml_get_label_components(sym);
    if(allComps==NULL)
	return NULL;
    int count = list_count(allComps);
    if(count<=pos)
	return NULL;
    else
    {
        if (pos == MessageArgsComponent)
	    return(list_get(allComps,pos));
        else
	    return(strip_whitespace(list_get(allComps,pos)));
    }
}

    
string uml_fix_message_chars(string str, string componentName)
{
    string result = "";
    if(str!=NULL) {
	// --- START ECR 6635
	result=string_translate(str,UML_BAD_CH_NO_COL_WITH_BRACES, UML_GOOD_CH_NO_COL_WITH_BRACES);
	// --- END ECR 6635
	if(result!=str)
	{
	    // NOTE: will not change external name, i.e., label.  Do not have
	    // the full name.  That should be done separately, at a place
	    // where the whole name is known.
	    string component = "Name";
	    if(componentName!="")
		component = componentName;
	    print_error(component+" component `"+str+"' has invalid characters. Fixing INTERNAL name ...");
	    //message("DEBUG uml_fix_message_chars='"+result+"'");
	}
    }
    return result;
}

string uml_message_PredecessorGuard(gde_mapsymbol sym)
{
    string result = uml_get_label_component(sym,PredecessorGuardComponent);
    //message("DEBUG uml_message_PredecessorGuard='"+result+"'");
    return result;
}

string uml_message_SequenceVal(gde_mapsymbol sym)
{
    string result = uml_get_label_component(sym,SequenceValComponent);
    //message("DEBUG uml_message_SequenceVal='"+result+"'");
    return result;
}

string uml_message_Recurrence(gde_mapsymbol sym)
{
    string result = uml_get_label_component(sym,RecurrenceComponent);
    //message("DEBUG uml_message_Recurrence='"+result+"'");
    return result;
}

string uml_message_ReturnType(gde_mapsymbol sym)
{
    string result = uml_get_label_component(sym,ReturnTypeComponent);
    //message("DEBUG uml_message_ReturnType='"+result+"'");
    return (uml_fix_message_chars(result,"Return Value"));
}

string uml_message_MessageName(gde_mapsymbol sym)
{
    string result = uml_get_label_component(sym,MessageNameComponent);
    //message("DEBUG uml_message_MessageName='"+result+"'");
    return (uml_fix_message_chars(result,"Message Name"));
}

string uml_message_MessageArgs(gde_mapsymbol sym)
{
    string result = uml_get_label_component(sym,MessageArgsComponent);
    //message("DEBUG uml_message_MessageArgs='"+result+"'");
    return result;
}


list uml_gde_link_name_func(gde_mapsymbol sym)
{
    list	resultList = NULL;
    string aLabel="";
    string rvStr = uml_message_ReturnType(sym);
    if(rvStr!="" && rvStr!=NULL)
	aLabel = rvStr + ":=";
    //message("DEBUG uml_gde_link_name_func::aLabel='"+aLabel+"'");
    string nameStr = uml_message_MessageName(sym);
    if(nameStr!="" && nameStr!=NULL)
	aLabel += nameStr;
    //message("DEBUG uml_gde_link_name_func::aLabel='"+aLabel+"'");
    string argsStr = uml_message_MessageArgs(sym);

    // 12802 fix. arguments without message name
    if (aLabel != "")
        if (argsStr!=NULL)    //if(argsStr!="" && argsStr!=NULL)
	    aLabel += "(" + argsStr + ")";

    string val = strip_whitespace(aLabel);
    //message("DEBUG uml_gde_link_name_func::val='"+val+"'");
    if (val != NULL) { // && val != "") {
	resultList = list_create("string", 0);
	// ECR 6677 begin (currently commented out)
	list_append(resultList, val);   // <- comment this and
	//list_append(resultList, aLabel); <- uncomment this to empower ECR 6677
	// ECR 6677 end
    }
    //message("DEBUG uml_gde_link_name_func::val='"+val+"' result="+resultList);
    return(resultList);
}

    
string uml_gde_noop_node_name_func(gde_mapsymbol sym)
{
    //trace("all");
    
    list	resultList = NULL;
    string aLabel;

    if (gde_mapsymbol_is_node(sym))// gde_mapsymbol_is_cntx(sym)
    {
	aLabel = gde_node_label(to_gde_node(sym));
    } // else if (gde_mapsymbol_is_link(sym))
//     {
// 	aLabel = gde_link_label(to_gde_link(sym));
//     } else {
//  	message("is NOT link NOR cntx!!");
//  	return NULL;
//     }

    string val = strip_whitespace(aLabel);
    return(val);
}

// recycled from "object.qrl & object.mapping"
gde_node to_node_from_arc(gde_arc gArc)
{
    list	lks;

    lks = gde_arc_links(gArc);
    if (lks == NULL || list_count(lks) == 0)
	return(NULL);
    return(gde_link_tonode(list_get(lks, 0)));
}

gde_node from_node_from_arc(gde_arc gArc)
{
    list	lks;

    lks = gde_arc_links(gArc);
    if (lks == NULL || list_count(lks) == 0)
	return(NULL);
    return(gde_link_fromnode(list_get(lks, 0)));
}

gde_mapsymbol uml_message_to_end(gde_mapsymbol msg)
{
    gde_cntx	gCntx;
    gde_node	gNode;
    gde_symbol	gSym;
    gde_arc	gArc;

    gCntx = to_gde_cntx(msg);
    gNode = to_gde_node(to_gde_symbol(gCntx));
    gSym = gde_node_parent(gNode);
    if (gSym == NULL)
    {
	return(NULL);
    }
    if (gde_symbol_valid(gSym) == False)
    {
	return(NULL);
    }
    gArc = to_gde_arc(gSym);
    gNode = to_node_from_arc(gArc);
    if (gNode == NULL)
	return(NULL);
    return(to_gde_mapsymbol(gNode));
}

gde_mapsymbol uml_message_from_end(gde_mapsymbol msg)
{
    gde_cntx	gCntx;
    gde_node	gNode;
    gde_symbol	gSym;
    gde_arc	gArc;

    gCntx = to_gde_cntx(msg);
    gNode = to_gde_node(to_gde_symbol(gCntx));
    gSym = gde_node_parent(gNode);
    if (gSym == NULL)
    {
	return(NULL);
    }
    if (gde_symbol_valid(gSym) == False)
    {
	return(NULL);
    }
    gArc = to_gde_arc(gSym);
    gNode = from_node_from_arc(gArc);
    if (gNode == NULL)
	return(NULL);
    return(to_gde_mapsymbol(gNode));
}

///////////////////////////////////////////////////////

string uml_get_ext_point_label_from_node(node theNode, node_ref theNodeRef, string notePrefix)
{
    item anItem;
    string thePredecessorGuard  = "";
    string theSequenceExpression  = "";
    string theMessageRecurrence = "";
    string result = NULL;

    string noteName = "";//notePrefix + uml_get_node_psymid(theNodeRef);

    note theNote = find_by_query("note[obj_id=${theNode.id} & name='${noteName}']");
    int noteId = 0;
    if(theNote!=NULL)
	noteId = theNote.id;
    else {
	print_warning("uml_get_message_label_from_node::Cannot find a note object '"+noteName+"' - you may need to remap interaction diagrams");
	// use any note
	theNote = find_by_query("note[obj_id=${theNode.id}]");
	if(theNote!=NULL)
	    noteId = theNote.id;
    }

    list matched_items = list_select("item[obj_id=${theNode.id} & note_id=${noteId}]");
	    
    int ii;
    int howMany = list_count(matched_items);
    for (ii = 0; ii < howMany; ii++) {
	anItem = list_get(matched_items, ii);
	if(anItem.value != "" && anItem.value !=NULL) {
	    if(anItem.type=="UmlPredecessorGuard")
		thePredecessorGuard = anItem.value + "/";
	    else if(anItem.type=="UmlSequenceExpression")
		theSequenceExpression = anItem.value;
	    else if(anItem.type=="UmlMessageRecurrence")
		theMessageRecurrence = anItem.value;
	}
    }
    theSequenceExpression += theMessageRecurrence;
    if(theSequenceExpression != "") 
	theSequenceExpression += ":";
    result = thePredecessorGuard + theSequenceExpression + theNode.name;
    //print_//message("DEBUG uml_get_message_label_from_node::'"+result+"'"+theNode);
    return(result);
}

///////////////////////////////////////////////////////

string uml_get_message_label_from_link(link theLink, link_ref theLinkRef, string notePrefix)
{
    item anItem;
    string thePredecessorGuard  = "";
    string theSequenceExpression  = "";
    string theMessageRecurrence = "";
    string result = NULL;
//     string theReturnType = "";
//     string theObjectName  = "";
//     string theMessageArgs  = "";
    //string newLabel = NULL;
    //anItem = find_by_query("item[UmlPredecessorGuard && obj_id=${theLink.id}]");

    string noteName = notePrefix + uml_get_link_arc_psymid(theLinkRef);

    note theNote = find_by_query("note[obj_id=${theLink.id} & name='${noteName}']");
    int noteId = 0;
    if(theNote!=NULL)
	noteId = theNote.id;
    else {
	print_warning("uml_get_message_label_from_link::Cannot find a note object '"+noteName+"' - you may need to remap interaction diagrams");
	// use any note
	theNote = find_by_query("note[obj_id=${theLink.id}]");
	if(theNote!=NULL)
	    noteId = theNote.id;
    }

    list matched_items = list_select("item[obj_id=${theLink.id} & note_id=${noteId}]");
	    
    int ii;
    int howMany = list_count(matched_items);
    for (ii = 0; ii < howMany; ii++) {
	anItem = list_get(matched_items, ii);
	if(anItem.value != "" && anItem.value !=NULL) {
	    if(anItem.type=="UmlPredecessorGuard")
		thePredecessorGuard = anItem.value + "/";
	    else if(anItem.type=="UmlSequenceExpression")
		theSequenceExpression = anItem.value;
	    else if(anItem.type=="UmlMessageRecurrence")
		theMessageRecurrence = anItem.value;
// 	    else if(anItem.type=="UmlReturnType")
// 		theReturnType = anItem.value + ":=";
// 	    else if(anItem.type=="UmlObjectName")
// 		theObjectName = anItem.value;
// 	    else if(anItem.type=="theMessageArgs")
// 		theMessageArgs = "("+anItem.value + ")";
	}
    }
    theSequenceExpression += theMessageRecurrence;
    if(theSequenceExpression != "") 
	theSequenceExpression += ":";
    result = thePredecessorGuard + theSequenceExpression + theLink.name;
    //print_//message("DEBUG uml_get_message_label_from_link::'"+result+"'"+theLink);
    return(result);
}

///////////////////////////////////////////////////////
item uml_get_message_seq_item_from_link_ref(link_ref theLinkRef, string notePrefix)
{
    item anItem = NULL;
    string noteName = notePrefix + uml_get_link_arc_psymid(theLinkRef);
    note theNote = find_by_query("note[obj_id=${theLinkRef.link_id} & name='${noteName}']");
    if(theNote!=NULL)
        anItem = find_by_query("item[UmlSequenceExpression & obj_id=${theLinkRef.link_id} & note_id=${theNote.id}]");
    else {
        print_warning("uml_get_message_seq_item_from_link_ref::Cannot find a note object '"+noteName+"' - you may need to remap interaction diagrams");
	    // use any note
	    anItem = find_by_query("item[UmlSequenceExpression & obj_id=${theLinkRef.link_id}]");
    }
    return(anItem);
}

///////////////////////////////////////////////////////

string uml_get_link_arc_psymid(link_ref theLinkRef)
{
    string linkAppid = theLinkRef.appid;
    list arcsList = string_to_list(linkAppid,",");
    //message("DEBUG uml_get_link_arc_psymid:: linkAppid='"+linkAppid+"' arcsList="+arcsList);
    string arcPsymid = list_get(arcsList,0);
    //message("DEBUG uml_get_link_arc_psymid:: arcPsymid='"+arcPsymid+"'");
    
    return(arcPsymid);
}

///////////////////////////////////////////////////////
void uml_set_message_label(gde_mapsymbol sym,
			   string thePredecessorGuard,
			   string theSequenceValue,
			   string theMessageRecurrence,
			   string theReturnType,
			   string theMessageName,
			   string theMessageArgs)
{
    //message("DEBUG uml_set_message_label::thePredecessorGuard'"+thePredecessorGuard+"' theSequenceValue'"+theSequenceValue+"' theMessageRecurrence'"+theMessageRecurrence+"' theReturnType'"+theReturnType+"' theMessageName'"+theMessageName+"' theMessageArgs'"+theMessageArgs+"'");

    string newLabel = "";
    if(thePredecessorGuard!="" && thePredecessorGuard!=NULL)
	newLabel = thePredecessorGuard + "/";
    string theSequenceExpression = "";

    if(theSequenceValue!="" && theSequenceValue!=NULL)
    theSequenceExpression = theSequenceValue;
    if(theMessageRecurrence!="" && theMessageRecurrence!=NULL)
	theSequenceExpression += theMessageRecurrence;
    if(theSequenceExpression!="" && theSequenceExpression!=NULL)
 	newLabel += theSequenceExpression + ":";
    if(theReturnType!="" && theReturnType!=NULL)
    newLabel += theReturnType + ":=";
    newLabel += theMessageName + "(" + theMessageArgs + ")";
    gde_link gLink = NULL;
    gde_cntx gCntx = NULL;
    //gde_arc gArc = NULL;
    node aNode = NULL;
    link aLink = NULL;
    int theTransaction;

    if (newLabel == "()")
        newLabel = "";

    // save current state for editor
    gde_save_state();
    theTransaction = gde_start_edit_transaction();

    if (gde_mapsymbol_is_cntx(sym) || gde_mapsymbol_is_node(sym)) {
	gde_node gNode = to_gde_node(sym);
	string existingLabel = gde_node_label(gNode);
	if(existingLabel!=newLabel)
	    gde_node_set_label(gNode, newLabel);
    } else if (gde_mapsymbol_is_link(sym)) {
	gLink = to_gde_link(sym);
	string existingLabel = gde_link_label(gLink);
	if(existingLabel!=newLabel)
	    gde_link_set_label(gLink, newLabel);
    }
    // end transaction
    gde_end_edit_transaction(GDE_CMD_INSERT, theTransaction, False);
}
    

string uml_get_message_fullLabel_display_mark(list matched_items, string label)
{
    item anItem;
    int howMany = list_count(matched_items);
    string thePredecessorGuard  = "";
    string theSequenceExpression  = "";
    string theMessageRecurrence = "";
    string result = NULL;
//     string theReturnType = "";
//     string theObjectName  = "";
//     string theMessageArgs  = "";
//     string newLabel = NULL;
    string dm;
    int ii;
    for (ii = 0; ii < howMany; ii++) {
	anItem = list_get(matched_items, ii);
	if(anItem.value != "" && anItem.value !=NULL) {
	    if(anItem.type=="UmlPredecessorGuard")
		thePredecessorGuard = anItem.value + "/";
	    else if(anItem.type=="UmlSequenceExpression")
		theSequenceExpression = anItem.value;
	    else if(anItem.type=="UmlMessageRecurrence")
		theMessageRecurrence = anItem.value;
// 	    else if(anItem.type=="UmlReturnType")
// 		theReturnType = anItem.value + ":=";
// 	    else if(anItem.type=="UmlObjectName")
// 		theObjectName = anItem.value;
// 	    else if(anItem.type=="theMessageArgs")
// 		theMessageArgs = "("+anItem.value + ")";
	}
    }
    theSequenceExpression += theMessageRecurrence;
    if(theSequenceExpression != "") 
	theSequenceExpression += ":";
//     newLabel = theReturnType + theObjectName + theMessageArgs;
//     if(newLabel != label) {
// 	if(howMany==0) {
// 	    print_error("Internal error?? No items matched for a message symbol");
// 	} else {
// 	    int linkId = anItem.obj_id;
// 	    uml_update_message_symbol_for_link(linkId,newLabel,label);
// 	}
//     }
    result = thePredecessorGuard + theSequenceExpression;
    return(result);
}


string
uml_get_message_display_mark_from(list matched_items)
{
    item anItem;
    int howMany = list_count(matched_items);
    string result = "";
    string dm = "";
    int ii;
    for (ii = 0; ii < howMany; ii++) {
	anItem = list_get(matched_items, ii);
	if(anItem.type=="UmlMessageIsDestruction" && anItem.value=="True") {
	    dm = "X";
	    break;
	} else if(anItem.type=="UmlMessageDirection" && anItem.value=="Return")
	    dm = "Ea";
    }
    result = dm;
    //print_//message("DEBUG uml_get_message_display_mark_from::'"+result+"'");
    return(result);
}

string
uml_get_message_display_mark_to(list matched_items)
{
    item anItem;
    int howMany = list_count(matched_items);
    string result = "";
    string crDm = "";
    string dirDm = "";
    int ii;
    for (ii = 0; ii < howMany; ii++) {
	anItem = list_get(matched_items, ii);
	if(anItem.type=="UmlMessageIsCreation" && anItem.value=="True")
	    crDm = "C";
// 	else if(anItem.type=="UmlMessageDirection" && anItem.value=="Activation")
// 	    dirDm = "A";
    }
//     if ( crDm != "" && dirDm != "")
// 	result = crDm + "\n" + dirDm;
//     else
// 	result = crDm + dirDm;
//     //print_//message("DEBUG uml_get_message_display_mark_to::'"+result+"'");
//     return(result);
    return(crDm);
}


// This function takes a list of items and creates a comma and \n
// separated list of them. open_str and close_str are used to enclose
// the string
// string uml_dpymark_list_formatter(list matched_items, string open_str,
// 		string close_str)
// {
//     item            one_item;
//     int             i;
//     string          props = open_str;
// 	int             numItems = list_count(matched_items);
   
//     for (i = 0; i < numItems; i = i + 1)
//     {
// 		one_item = list_get(matched_items, i);
   
// 		if (NULL != one_item.value && "" != one_item.value)
// 		{
// 			if (i > 0) {
// 				props += ",\n";
// 			}
// 			props += one_item.value;
// 		} 
//     }
// 	props += close_str;
//     return props;
// }

// ECR 5861 begin
string
uml_dpymark_list_formatter_constrained(list matched_items, int linkid, string open_str,
		string close_str, string sep_str)
{
    item    one_item;
    int     i;
    int     numItems = list_count(matched_items);
    list    strings = list_create("string", 0);

    for (i = 0; i < numItems; i = i + 1)
    {
        one_item = list_get(matched_items, i);

	if (NULL != one_item.value && "" != one_item.value)
	{
	    if (one_item.obj_id == linkid)
                list_append(strings, one_item.value);
	} 
    }

    return uml_dpymark_string_list_formatter(strings, open_str, close_str, sep_str);
}
// ECR 5861 end

// Begin add ECR 6726
// This function takes a list of items and creates a comma and \n
// separated list of them. open_str and close_str are used to enclose
// the string
string uml_dpymark_list_formatter(list matched_items, string open_str,
		string close_str, string sep_str)
{
    item            one_item;
    int             i;
	int             numItems = list_count(matched_items);
	list            strings = list_create("string",0);

    for (i = 0; i < numItems; i = i + 1)
    {
		one_item = list_get(matched_items, i);

		if (NULL != one_item.value && "" != one_item.value)
		{
			list_append(strings,one_item.value);
		} 
    }
	return uml_dpymark_string_list_formatter(strings,open_str,close_str,sep_str);
}

// This function takes a list of items and creates a sep_str
// separated list of them. open_str and close_str are used to enclose
// the string
string uml_dpymark_string_list_formatter(list strings, string open_str,
		string close_str, string sep_str)
{
	if (list_count(strings) > 0 ) {
		return open_str + list_to_string(strings, sep_str) + close_str;
	} else {
		return "";
	}
}
// End add ECR 6726

const string DG_PROP_HIDE_SEQUENCE_VAL = "HideSequenceValues";

string uml_get_diag_prop_hide_message_seqVal()
{
    string propVal = gde_diagram_prop(DG_PROP_HIDE_SEQUENCE_VAL);
    if (propVal == "")
	propVal = NULL;
    return(propVal);
    //gde_diagram_set_prop(DIAG_MAPPING_PROP, prop);
}

void uml_hide_message_seqVal()
{
    string propVal = uml_get_diag_prop_hide_message_seqVal();
    if (propVal != NULL) {
	if(propVal=="True")
	    return;
	else message("uml_hide_mesage_seqVal::value '"+propVal+"'??");
    }
    gde_diagram_set_prop(DG_PROP_HIDE_SEQUENCE_VAL, "True");
    uml_update_all_message_labels();
}

void uml_show_message_seqVal()
{
    string propVal = uml_get_diag_prop_hide_message_seqVal();
    if (propVal != NULL)
	gde_diagram_set_prop(DG_PROP_HIDE_SEQUENCE_VAL, "");
    uml_update_all_message_labels();   
}

void uml_update_all_message_labels()
{
    string dg_type = current_editor_apptype();
    if(dg_type=="UmlSequenceDiagram")
	uml_update_all_message_labels_link();
    else
	uml_update_all_message_labels_cntx();
}

list allMessageLinkTypes = NULL;
void uml_update_all_message_labels_link()
{
    const string messageNames = "SimpleMessage SynchronousMessage BalkingMessage TimeoutMessage AsynchronousMessage";
    if(allMessageLinkTypes==NULL)
	allMessageLinkTypes = string_to_list(messageNames," ");
    int ii;
    string symTypeName;
    list allMessages = gde_arcs_find_by_type(gde_all_symbols(),
					     list_get(allMessageLinkTypes,0));
    for(ii=1; ii<list_count(allMessageLinkTypes); ii++) {
	symTypeName = list_get(allMessageLinkTypes,ii);
	list_concatenate(allMessages,
			 gde_arcs_find_by_type(gde_all_symbols(),symTypeName));
    }
    gde_arc aMessage;
    for(ii=0; ii<list_count(allMessages); ii++) {
	aMessage = list_get(allMessages,ii);
	uml_update_message_label_link(aMessage);
    }
}

list allMessageCntxTypes = NULL;
void uml_update_all_message_labels_cntx()
{
// ECR 6577 begin
    //const string messageNames = "SimpleMessage SynchronousMessage BalkingMessage TimeoutMessage AsynchronousMessage RevSimpleMessage RevSynchronousMessage RevBalkingMessage RevTimeoutMessage RevAsynchronousMessage ReturnMessage";
    const string messageNames = "SimpleMessage SynchronousMessage BalkingMessage TimeoutMessage AsynchronousMessage RevSimpleMessage RevSynchronousMessage RevBalkingMessage RevTimeoutMessage RevAsynchronousMessage ReturnMessage RevReturnMessage";
// ECR 6577 end
    if(allMessageCntxTypes==NULL)
	allMessageCntxTypes = string_to_list(messageNames," ");
    int ii;
    string symTypeName;
    list allMessages = gde_arcs_find_by_type(gde_all_symbols(),
					     list_get(allMessageCntxTypes,0));
    for(ii=1; ii<list_count(allMessageCntxTypes); ii++) {
	symTypeName = list_get(allMessageCntxTypes,ii);
	list_concatenate(allMessages,
			 gde_arcs_find_by_type(gde_all_symbols(),symTypeName));
    }
    gde_arc aMessage;
    for(ii=0; ii<list_count(allMessages); ii++) {
	aMessage = list_get(allMessages,ii);
	uml_update_message_label_link(aMessage);
    }
}

void uml_update_message_label_link(gde_arc gArc)
{
}

void uml_update_message_label_cntx(gde_cntx gCntx)
{
}

node
usequenced_get_current_message_class()
{
	list selected_syms;
	gde_node theNode;
	gde_symbol theSym;
    gde_arc theArc;

	// get currently selected link
	selected_syms = gde_selected_symbols();
	if (NULL == selected_syms || list_count(selected_syms) != 1)
		return NULL;

	theSym = list_get(selected_syms,0);
    // JED - fix for ucollaborationd, which uses this code but uses cntx
    // symbols attached to links instead of the links themselves.
    if (gde_symbol_is_cntx(theSym))
        theArc = gde_cntx_parent(to_gde_cntx(theSym));
    else
        if (gde_symbol_is_arc(theSym))
            theArc = to_gde_arc(theSym);
        else
    		return NULL;

    // get the to_node
    // Begin Change ECR 4110
    if( string_extract( gde_get_symbol_type( theSym ), 0, 3 ) == "Rev" && gde_symbol_is_cntx( theSym ) ) 
        theNode = gde_arc_fromnode( theArc );
    else
        theNode = gde_arc_tonode(theArc);
    // End Change ECR 4110
	if (NULL == theNode)
		return NULL;

	// get the mapped OMS nodes
	list mapped_nodes = gde_node_oms_nodes(theNode);

	// get the one for the class scope
	node class_scope = NULL;
	int i;

	for (i=0;i<list_count(mapped_nodes);i++) {
		class_scope = list_get(mapped_nodes,i);
		if ("UmlObjectClassScope" == class_scope.type)
			break;
		else
			class_scope = NULL;
	}
	if (NULL == class_scope) 
	{
        // ECR 4110, this function doesn't work for reflexive messages, so I reuse
        // some code from message_prop.inc:MessageOperationsBrowserPopulate
        link mssgLink = list_get(gde_link_oms_links(list_get(gde_arc_links(theArc),0)),0);
        node omsToNode = find_by_query("node[id=${mssgLink.to_node_id}]");
    	node objScopeClass = find_by_query("node[id=${omsToNode.scope_node_id}]");
    	node serverClass = find_by_query("node[UmlClass & name='${objScopeClass.name}']");
        return( serverClass );
        // End ECR 4110
    }

	node class = find_by_query("node[UmlClass && name='"+class_scope.name+"']");

	return class;
}

list
usequenced_operations_name_completion_func (node class, string label, list names)
{
	string pattern;
	// JED - first, get the names for the link type like the default would do
	// One difference: The Pattern is going to run the name func, which
	// will append () to the name typed so far, then name completion will
	// append a "*". This will search for name()*, which will never find
	// anything. So, we have to use the label and fix the pattern.
	if (label != NULL) {
		pattern = label + "*";
	} else {
		pattern = "*";
	}

	if (NULL == class)
		return names;

	if (NULL == names)
		names = list_create("string",0);

	// do name completion for the operations
	list allops = uml_all_operations_for_class(pattern,class,NULL);

	node op;
	string opname;
	list args;
	uml_argument arg;
	int i;
	int j;

	for(i=0;i<list_count(allops);i++) {
		op = list_get(allops,i);
		opname = op.name;
		if (op.sig != NULL && op.sig != "") {
			// JED - just put the types in the string returned
			//   (as a clue to what arguments to pass in the message)
			args= list_create("string",0);
			opname += "(";

			if(uml_split_arguments(op.sig,args)) {
				for(j=0;j<list_count(args);j++) {
					arg = uml_arg_string_to_struct(list_get(args,j));
					opname += arg.argType;
					if (j+1 < list_count(args))
						opname += ",";
				}
			}
			opname += ")";

		}
		else
			opname += "()";

		// don't duplicate anything from first search
		if (list_count(names) == list_find(names,0,opname))
			list_append(names,opname);
	}
	
	// JED debug
	// print_message("JED - returning: " + list_to_string(names,","));
	return names;
}

list allMessageOmsTypes = NULL;
list uml_message_name_completion_func(string pattern, string label, 
				 string extent, string apptype, int scopeid)
{
    // Get names of all existing messages.  A list of operations
    list	names;
    list	compound_names;
    string	fullName;
    compound_names = list_create("string", 0);

    //message("DEBUG uml_message_name_completion_func::pattern='"+pattern+"' label='"+label+"' extent='"+extent+"' apptype='"+apptype+"' scopeid="+scopeid);
    //print_message("DEBUG uml_message_name_completion_func::pattern='"+pattern+"' label='"+label+"' extent='"+extent+"' apptype='"+apptype+"' scopeid="+scopeid);
    string srcStr = label;
    // use pattern instead of label, because it contains the filter string if
    // provided, otherwise it contains ther label as a base.
    //if(srcStr=="" || srcStr == NULL)
    srcStr = pattern;

    list aLabelList = uml_parse_message_label(srcStr);
    string message_name = list_get(aLabelList,MessageNameComponent);
    string message_args = list_get(aLabelList,MessageArgsComponent);
    string retVal = list_get(aLabelList,ReturnTypeComponent);
    string predecessor = list_get(aLabelList,PredecessorGuardComponent);
    string sequenceVal = list_get(aLabelList,SequenceValComponent);
    string recurrence = list_get(aLabelList,RecurrenceComponent);
    string prefix = "";
    if(predecessor!="")
	prefix = predecessor + "/";
    string seqExpression = sequenceVal + recurrence; 
    if(seqExpression!="")
	prefix += seqExpression + ":";
    string linkName = "";
    if(retVal!="" && retVal!=NULL)
	linkName = retVal + ":=";
    if(message_name==NULL) {
	message_name = "";
    }
    linkName += message_name;
    if(message_args==NULL || message_args == "")
		linkName += "*";
	else {
	linkName += "(" + message_args; // + ")"; want to end with "*", not ")*" !!
    linkName += "*";
	}
       
    //
    // call the standard routines to get all the message names that match
    // the pattern.
    //
    names = list_create("string", 0);
    int scope = 0;		// class objects have scopeId==0
	 
	// JED - this has been cloned to uclassd.qrl for operation name
	// completion. If you modify this string, modify that one as well
    const string linkTypes = "UmlSimpleMessage UmlSynchronousMessage UmlBalkingMessage UmlTimeoutMessage UmlAsynchronousMessage";

    int ii;
	// JED - I don't like this, but it gets message names that are otherwise
	// unavailable
    if(allMessageOmsTypes==NULL)
		allMessageOmsTypes = string_to_list(linkTypes," ");
    string linkType;
    for(ii=0;ii<list_count(allMessageOmsTypes); ii++) {
		linkType = list_get(allMessageOmsTypes,ii);
		nc_get_names_from_repository(names,linkName,extent,linkType,scope,False);
		nc_get_names_from_buffer(names,linkName,extent,linkType,scope,False);
    }

	// JED - add on messages inherited from super classes
	node class = usequenced_get_current_message_class();
	if (NULL != class) {
            string fixed_pattern = message_name; //label + "*";
            list_concatenate(names, uml_messages_for_superclasses(fixed_pattern,class,NULL));
	}
	

    // then prepend each class name with the prefix containing
    // predecessor guard and seq expr, if any.
    //
    for (ii = 0; ii < list_count(names); ii++) {
	fullName = prefix + list_get(names, ii);
	list_append(compound_names, fullName);
	//message("Found:fullName='"+fullName+"'");
    }

	// JED - add on operations
    //compound_names = usequenced_operations_name_completion_func (class, label, compound_names);
    compound_names = usequenced_operations_name_completion_func (class, message_name, compound_names);
    return (compound_names);
}


void
uml_message_inverse_mapping(gde_mapsymbol ms, link ln, list rninfo,
			    gocha_tp gocha_type)
{
    string		classname, objname, label = NULL;
    rename_data		thisrename;
    //gde_link		rnsym;
    int			i;

    if ( gocha_type != gochaRename ) {
	print_error( "uml_message_inverse_mapping() can only handle renames" );
	return; 
    }

    int count = list_count(rninfo);
//     for(i = 0; i < list_count(rninfo); i = i + 1)
//     {
// 	thisrename = list_get(rninfo, i);
// 	if ((thisrename.dependency_name == GDE_GOR_REN_DEP_NAME) ||
//            (thisrename.dependency_name == GDE_GOR_REN_DEP_SELF))
// 	{
// 	    label = thisrename.label;
// 	    break;
// 	}
//     }
    thisrename = list_get(rninfo, count-1);
    label = thisrename.label;
    //message("DEBUG uml_message_inverse_mapping thisrename='"+thisrename+"' label='"+label+"' ln='"+ln+"'");

    string newLabel = "?";
    string existingLabel = "?";

    if(thisrename.dependency_name=="MessageItemValueChange" ||
       thisrename.dependency_name=="MessageSignatureChange") {
	// the source of change is operation rename, and its label is
	// in a different format.  Instead of messing with the label,
	// use the link name, which has already been updated to
	// contain all relevant info
	label=ln.name;
    } // else {
// 	message("DEBUG uml_message_inverse_mapping dependency_name='"+thisrename.dependency_name+"' is NEITHER 'MessageItemValueChange' NOR 'MessageSignatureChange'");
//     }
//     message("DEBUG uml_message_inverse_mapping label='"+label+"' ln='"+ln+"'");

    if (gde_mapsymbol_is_link(ms)) {
	gde_link rnsym = to_gde_link(ms);
	existingLabel = gde_link_label(rnsym);
	newLabel = uml_update_label_using_new_signature(existingLabel,label);
	gde_link_set_label(rnsym, newLabel);
    } else if (gde_mapsymbol_is_cntx(ms)) {
	gde_cntx rnsym = to_gde_cntx(ms);
	existingLabel = gde_cntx_label(rnsym);
	newLabel = uml_update_label_using_new_signature(existingLabel,label);
	gde_cntx_set_label(rnsym, newLabel);
    }
    //message("DEBUG uml_message_inverse_mapping dependency_name='"+thisrename.dependency_name+"' label='"+label+"' existingLabel='"+existingLabel+"' newLabel='"+newLabel+"' thisrename="+thisrename);
}


///////////////////////////////////////////////////////

void
uml_message_inverse_mapping_items(gde_mapsymbol ms, item ln, list rninfo,
			    gocha_tp gocha_type)
{
    string		classname, objname, label = NULL;
    rename_data		thisrename;
    //gde_link		rnsym;
    int			i;

    if ( gocha_type != gochaRename ) {
	print_error( "uml_message_inverse_mapping_items() can only handle renames" );
	return; 
    }

    int count = list_count(rninfo);
//     for(i = 0; i < list_count(rninfo); i = i + 1)
//     {
// 	thisrename = list_get(rninfo, i);
// 	if ((thisrename.dependency_name == GDE_GOR_REN_DEP_NAME) ||
//            (thisrename.dependency_name == GDE_GOR_REN_DEP_SELF))
// 	{
// 	    label = thisrename.label;
// 	    break;
// 	}
//     }
    thisrename = list_get(rninfo, count-1);
    label = thisrename.label;
    //message("DEBUG uml_message_inverse_mapping_items thisrename='"+thisrename+"' label='"+label+"' ln='"+ln+"'");

    string newLabel = label;
    //string existingLabel = "?";

    if (gde_mapsymbol_is_link(ms)) {
	gde_link rnsym = to_gde_link(ms);
	gde_link_set_label(rnsym, newLabel);
    } else if (gde_mapsymbol_is_cntx(ms)) {
	gde_cntx rnsym = to_gde_cntx(ms);
	gde_cntx_set_label(rnsym, newLabel);
    }
    //message("DEBUG uml_message_inverse_mapping_items dependency_name='"+thisrename.dependency_name+"' label='"+label+"' newLabel='"+newLabel+"' thisrename="+thisrename);
}


///////////////////////////////////////////////////////

string uml_update_label_using_new_signature(string oldLabel,string newName)
{
    list oldLabelComponents = uml_parse_message_label(oldLabel);
    list newNameComponents = uml_parse_message_label(newName);
    //message("DEBUG uml_update_label_using_new_signature:: oldLabel='"+oldLabel+"' oldLabelComponents='"+oldLabelComponents+"'");
    //message("DEBUG uml_update_label_using_new_signature:: newName='"+newName+"' newNameComponents='"+newNameComponents+"'");
    string newVal = list_get(newNameComponents,MessageNameComponent);
    //message("DEBUG uml_update_label_using_new_signature:: newVal='"+newVal+"'");
    list_set(oldLabelComponents,MessageNameComponent,newVal);
    newVal = list_get(newNameComponents,ReturnTypeComponent);
    list_set(oldLabelComponents,ReturnTypeComponent,newVal);
    newVal = list_get(newNameComponents,MessageArgsComponent);
    list_set(oldLabelComponents,MessageArgsComponent,newVal);

    string newLabel = uml_build_new_label(oldLabelComponents,False);
    //message("DEBUG uml_update_label_using_new_signature:: oldLabel='"+oldLabel+"' newName='"+newName+"' oldLabelComponents='"+oldLabelComponents+"' newNameComponents='"+newNameComponents+"' newLabel='"+newLabel+"'");
    return newLabel;    
}

///////////////////////////////////////////////////////

string uml_get_extensionPoint_label_from_node(node theNode)
{
    item anItem;
    //string thePredecessorGuard  = "";
    string theSequenceExpression  = "";
    string theMessageRecurrence = "";
    string result = NULL;
//     string theReturnType = "";
//     string theObjectName  = "";
//     string theMessageArgs  = "";
    //string newLabel = NULL;
    //anItem = find_by_query("item[UmlPredecessorGuard && obj_id=${theLink.id}]");
    list matched_items = list_select("item[obj_id=${theNode.id}]");
	    
    int ii;
    int howMany = list_count(matched_items);
    for (ii = 0; ii < howMany; ii++) {
	anItem = list_get(matched_items, ii);
	if(anItem.value != "" && anItem.value !=NULL) {
	    if(anItem.type=="UmlSequenceExpression")
		theSequenceExpression = anItem.value;
	    else if(anItem.type=="UmlMessageRecurrence")
		theMessageRecurrence = anItem.value;
// 	    else if(anItem.type=="UmlPredecessorGuard")
// 		thePredecessorGuard = anItem.value + "/";
// 	    else if(anItem.type=="UmlReturnType")
// 		theReturnType = anItem.value + ":=";
// 	    else if(anItem.type=="UmlObjectName")
// 		theObjectName = anItem.value;
// 	    else if(anItem.type=="theMessageArgs")
// 		theMessageArgs = "("+anItem.value + ")";
	}
    }
    theSequenceExpression += theMessageRecurrence;
    if(theSequenceExpression != "") 
	theSequenceExpression += ":";
    result = theSequenceExpression + theNode.name;
    return(result);
}


void
uml_extensionPoint_inverse_mapping(gde_mapsymbol ms,node theNode,list rninfo,
			    gocha_tp gocha_type)
{
    string		classname, objname, label = NULL;
//     rename_data		thisrename;
    int			i;

    if ( gocha_type != gochaRename ) {
	print_error( "uml_extensionPoint_inverse_mapping() can only handle renames" );
	return; 
    }

//     int count = list_count(rninfo);
//     for(i = 0; i < list_count(rninfo); i = i + 1)
//     {
// 	thisrename = list_get(rninfo, i);
// 	if ((thisrename.dependency_name == GDE_GOR_REN_DEP_NAME) ||
//            (thisrename.dependency_name == GDE_GOR_REN_DEP_SELF))
// 	{
// 	    label = thisrename.label;
// 	    break;
// 	}
//     }
//     thisrename = list_get(rninfo, count-1);
//     label = thisrename.label;

    if (gde_mapsymbol_is_node(ms)) {
	gde_node rnsym = to_gde_node(ms);
	label = uml_get_extensionPoint_label_from_node(theNode);
	gde_node_set_label(rnsym, label);
	return;
    } else {
	print_error( "uml_message_inverse_mapping() can only handle nodes" );
	return; 
    }

}

string uml_extensionPoint_name(gde_mapsymbol sym)
{
    string result = uml_message_MessageName(sym);
    ////message("DEBUG uml_extensionPoint_name='"+result+"'");
    if(result=="")
	return NULL;
    else
	return result;
}

////////////////////////////////////////////////////////////////////////////////
//  This routine should go in message_label.inc
////////////////////////////////////////////////////////////////////////////////
string uml_build_new_label(list labelComponents, boolean noEmptyParenthesis)
{
    // noEmptyParenthesis: if True then include parenthesis only when 
    // there are args, so there are no empty parens "()"
    string newLabel = "";
    string thePredecessorGuard = list_get(labelComponents,0);
    string theSequenceExpression = "";
//ECR 6814 begin
    string theSequenceValue = list_get(labelComponents,SequenceValComponent);
    string theMessageRecurrence = list_get(labelComponents,RecurrenceComponent);
    string theReturnType = list_get(labelComponents,ReturnTypeComponent);
    string theMessageName = list_get(labelComponents,MessageNameComponent);
    string theMessageArgs = list_get(labelComponents,MessageArgsComponent);
//ECR 6814 end
    if(thePredecessorGuard!="" && thePredecessorGuard!=NULL)
	newLabel = thePredecessorGuard + "/";
    if(theSequenceValue!="" && theSequenceValue!=NULL)
    theSequenceExpression = theSequenceValue;
    if(theMessageRecurrence!="" && theMessageRecurrence!=NULL)
	theSequenceExpression += theMessageRecurrence;
    if(theSequenceExpression!="" && theSequenceExpression!=NULL)
	newLabel += theSequenceExpression + ":";
    if(theReturnType!="" && theReturnType!=NULL)
        newLabel += theReturnType + ":=";
    if(noEmptyParenthesis && (theMessageArgs=="" || theMessageArgs==NULL))
        newLabel += theMessageName;
    else
        newLabel += theMessageName + "(" + theMessageArgs + ")";
    return(newLabel);
}


string uml_change_label_using_new_name(string oldLabel,string newName)
{
    list labelComponents = uml_parse_message_label(oldLabel);
    // new name can have return value as well
    list newNameComponents = uml_parse_message_label(newName);
    string newLabel = newName;
    //message("DEBUG uml_change_label_using_new_name:: oldLabel='"+oldLabel+"' labelComponents='"+labelComponents+"'");
    //message("DEBUG uml_change_label_using_new_name:: newName='"+newName+"' newNameComponents='"+newNameComponents+"'");
    if(labelComponents!=NULL) {
	string newVal = list_get(newNameComponents,MessageNameComponent);
	//message("DEBUG uml_change_label_using_new_name:: newVal='"+newVal+"'");
	list_set(labelComponents,MessageNameComponent,newVal);
	newVal = list_get(newNameComponents,ReturnTypeComponent);
	//message("DEBUG uml_change_label_using_new_name:: newVal='"+newVal+"'");
	//if(newVal!=NULL && newVal!="") 
	list_set(labelComponents,ReturnTypeComponent,newVal);
	newLabel = uml_build_new_label(labelComponents,False);
    }
    //message("DEBUG uml_change_label_using_new_name:: oldLabel='"+oldLabel+"' newName='"+newName+"' labelComponents='"+labelComponents+"' newNameComponents='"+newNameComponents+"' newLabel='"+newLabel+"'");
    return newLabel;    
}

string uml_MessageNote_name(gde_mapsymbol mSym)
{
    // make the note unique, to avoid conflicts if there are 2 or more messages that have the same signature, but different guards or seq expressions
    string dg_name = gde_diagram_name();
    string dg_type = current_editor_apptype();

    // get psymid, to make note name symbol-specific
    gde_arc gArc = NULL;
    gde_link gLink = NULL;
    gde_cntx gCntx = NULL;
    gde_node gNode = NULL;
    gde_symbol gSym = NULL;
    int symId = 0;
    //message("DEBUG uml_MessageNote_name=*0");
    if (gde_mapsymbol_is_cntx(mSym) || gde_mapsymbol_is_node(mSym)) {
	gNode = to_gde_node(mSym);
	symId = gde_node_psymid(gNode);
    } else if (gde_mapsymbol_is_link(mSym)) {
	//message("DEBUG uml_MessageNote_name=*1");
	gLink = to_gde_link(mSym);
	//message("DEBUG uml_MessageNote_name=*2");
	string existingLabel = gde_link_label(gLink);
	//message("DEBUG uml_MessageNote_name label='"+existingLabel+"'");
	// get an arc, so we can have its psymid
	list gLinkArcs = gde_link_arcs(gLink);
	if(list_count(gLinkArcs)>0) {
	    //message("DEBUG uml_MessageNote_name=*3");
	    gArc = list_get(gLinkArcs,0);
	    //message("DEBUG uml_MessageNote_name=*4");
	    symId = gde_arc_psymid(gArc);
	    //message("DEBUG uml_MessageNote_name=*5 symId="+symId);
	} else {
	    //print_warning("uml_MessageNote_name could not find link's arcs");
	    //message("DEBUG uml_MessageNote_name:: Link does not have any arcs??");
	}
	//message("DEBUG uml_MessageNote_name=*6");
    }
    string symIdStr = to_string(symId);
    string result = dg_type+"%"+dg_name+"%"+symIdStr;
    //message("DEBUG uml_MessageNote_name='"+result+"'");
    return result;
}

const string _super_qry = "node[UmlClass && in_links[(UmlGeneralization|UmlImplements) && link_refs && from_node_id == ${class.id} ]]";
// get messages for all superclasses
list
uml_messages_for_superclasses(string pattern, node class, set path)
{
	node superCls;
	int i;
	list superClasses;
	list messages = list_create("string",0);

	if (NULL == path) {
		path = set_create("string");
	} else {
		// check for a cycle of inheritance
		if (set_is_member(path,class.name)) 
			return messages;
	}
	
	// add this class to the path
	set_add(path,class.name);
	
	// next, append messages for its superclasses
	superClasses = list_select(_super_qry);
	for(i=0;i<list_count(superClasses);i++) {
		superCls = list_get(superClasses,i);
		list_concatenate(messages,
				uml_messages_for_class(pattern,superCls));
		list_concatenate(messages,
				uml_messages_for_superclasses(pattern,superCls,path));
	}
	set_delete(path,class.name);
	return messages;
}

// NOTE: This function does not filter out duplicates due to override. It is
// meant solely to be used in name completion
list
uml_all_operations_for_class(string pattern, node class, set path)
{
	node superCls;
	int i;
	list superClasses;
	list operations = list_create("node",0);

	if (NULL == path) {
		path = set_create("string");
	} else {
		// check for a cycle of inheritance
		if (set_is_member(path,class.name)) 
			return operations;
	}
	
	// add this class to the path
	set_add(path,class.name);
	
	// first, get operations for this class (excluding ctor/dtor)
	operations = list_select("node[UmlOperation && scope_node_id=${class.id}"+
			" && name != '${class.name}' && name != '~${class.name}' " +
			" && name $ '${pattern}' && node_refs]");

	// next, append operations for its superclasses
	superClasses = list_select(_super_qry);
	for(i=0;i<list_count(superClasses);i++) {
		superCls = list_get(superClasses,i);
		list_concatenate(operations,
				uml_all_operations_for_class(pattern,superCls,path));
	}
	set_delete(path,class.name);
	return operations;
}



