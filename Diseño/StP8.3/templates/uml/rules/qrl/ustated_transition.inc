//      StP/UML
//      Confidential property of Aonix
//      Copyright (c) 1997
//      All rights reserved

const string UML_TR_PARSE_LPAREN = "(";
const string UML_TR_PARSE_RPAREN = ")";
const string UML_TR_PARSE_LBRACKET = "[";
const string UML_TR_PARSE_RBRACKET = "]";
const string UML_TR_PARSE_FSLASH = "/";
const string UML_TR_PARSE_SENDEV = "^"; 
const string UML_TR_PARSE_SEDOT = "."; 

/* structure to hold the results of a parsed transition */
struct UMLParsedTransition
{
  string link_label;
  string event;
  string guard;
  string action;
  string sendev;
  boolean unmatched_parens_error; 
  boolean unmatched_brackets_error; 
  int symid;
};


/* pseudo-static vars used by the parser,
   to avoid having to parse the same label twice in a row
*/

UMLParsedTransition	uml_pt;
boolean			uml_pt_first_time = True;

// ECR 6776 begin

boolean check_parens_matching(string actions)
{
    string curr_char;
    int pos = 0;
    int length = string_length(actions);
    int parse_state = 0;

    while (pos < length)
    {
        curr_char = string_extract(actions, pos, 1);
        if (curr_char == "(")
        {
            parse_state++;
        } 
        else if (curr_char == ")") 
        {
            parse_state--;
            if (parse_state < 0)
                return False;
        }

        pos++;
    }

    return parse_state == 0;
}

//
// Implemented same algorithm as in uml2interface
//
UMLParsedTransition
uml_parse_transition_label(string label)
{
    // form of label is:
    // event-signature '[' guard-condition ']' '/' action-expression '^' target-list
    //

    int len, pos;
    string currstr;
    boolean flag;

    label = replace_newlines(label, " ");
	len = string_length(label);

    /* initialize pt struct */
    uml_initialize_parsed_transition(uml_pt, label);
  
    /* no parsing needed */
    if (label == "" || label == NULL)
        return uml_pt;

    // first, process target-list

    pos = string_find(label, 0, "^");
	if (pos < len)
	{
        // found '^' character
        currstr = string_extract(label, pos + 1, len - pos - 1);
        label = string_extract(label, 0, pos);
        len = string_length(label);

		currstr = string_strip(currstr, "B", " ");
        if (currstr != "")
        {
            uml_pt.sendev = currstr;
            flag = check_parens_matching(currstr);
            if (!flag)
            {
                uml_pt.unmatched_parens_error = True;
                return uml_pt;
            }
        }
	}

    // then, process action-expression

    pos = string_find(label, 0, "/");
	if (pos < len)
	{
        // found '/' character
        currstr = string_extract(label, pos + 1, len - pos - 1);
        label = string_extract(label, 0, pos);
        len = string_length(label);

		currstr = string_strip(currstr, "B", " ");
        if (currstr != "")
        {
            uml_pt.action = currstr;
            flag = check_parens_matching(currstr);
            if (!flag)
            {
                uml_pt.unmatched_parens_error = True;
                return uml_pt;
            }
        }
	}

    // then, process guard-condition

    pos = string_find(label, 0, "[");
	if (pos < len)
	{
        // found '[' character
        currstr = string_extract(label, pos + 1, len - pos - 1);
        label = string_extract(label, 0, pos);
        len = string_length(label);

        pos = string_find(currstr, 0, "]");
        if (pos < string_length(currstr))
            currstr = string_extract(currstr, 0, pos);
        else
        {
            uml_pt.unmatched_brackets_error = True;
            return uml_pt;
        }

		currstr = string_strip(currstr, "B", " ");
        if (currstr != "")
            uml_pt.guard =  currstr;
	}

    // the rest is event-signature

	currstr = string_strip(label, "B", " ");
    if (currstr != "")
    {
        uml_pt.event = currstr;
        flag = check_parens_matching(currstr);
        if (!flag)
        {
            uml_pt.unmatched_parens_error = True;
            return uml_pt;
        }
    }

    return uml_pt;
}

//UMLParsedTransition
//uml_parse_transition_label(string link_label)
//{
//    string attribs = "";
//  /* A parser for labels on state transitions. These have the form:
//   *
//   *          event(attribs)[guard]/action(attribs)^sendev(attribs)
//   * ! action can have args, but we don't parse these out
//   * but we need to make sure that we don't accidentally use the
//   * actions arges as event args if the event doesn't have any args
//   *
//   * Returns the elements in an UMLParsedTransition struct, along with any error conditions.
//   * Replace by yacc grammar?. */
//
//  /* cursor positions: 4 cursors, plus the end of string */
//  int i, j, k, l, m, actst, guardst,sendst;
//  string	flat_lab;
//
//  // should that be empty ("") or space (" ")
//  flat_lab = replace_newlines(link_label, " ");
//
//  if (uml_pt_first_time)
//    uml_pt_first_time = False;
//  else if (flat_lab == uml_pt.link_label)
//    /* memorization -- we just parsed this label, return the previous result */
//    return uml_pt;
//
//  /* initialize pt struct */
//  uml_initialize_parsed_transition(uml_pt, flat_lab);
//  
//  /* no parsing needed */
//  if (flat_lab == "" || flat_lab == NULL)
//    return uml_pt;
//
//  i = 0; j = 0; k = 0; l = string_length(flat_lab);
//
//  // find "(attribs)", extract "attribs" 
//  // make sure we don't mistake action args for event args
//  // or a parenthesis in the guard for the arguments
//
//  actst = string_find(flat_lab, i, UML_TR_PARSE_FSLASH);
//  guardst = string_find(flat_lab, i, UML_TR_PARSE_LBRACKET);
//  sendst = string_find(flat_lab, i, UML_TR_PARSE_SENDEV);
//  j = string_find(flat_lab, i, UML_TR_PARSE_LPAREN);
//  if (j < l)
//  {
//      // found action args or guard condition expression and not event args  ...
//      if (actst < j || guardst < j || sendst < j)
//      {
//	j = l;
//      }
//  }
//
//  if (j < l)
//  {
//    uml_pt.event = string_extract(flat_lab, i, j - i);
//    k = string_find(flat_lab, j, UML_TR_PARSE_RPAREN);
//    if (k < l && k < actst && k < guardst && k < sendst)
//    {
//      attribs = string_extract(flat_lab, j + 1, k - j - 1);
//      i = k + 1;
//      uml_pt.event = uml_pt.event + UML_TR_PARSE_LPAREN + attribs + UML_TR_PARSE_RPAREN;
//    }
//    else {
//      uml_pt.unmatched_parens_error = True; 
//      return uml_pt;
//    }
//  }
//  
//  if (i >= l)
//    return uml_pt;
//
//  /* find "[guard]", extract "guard" */
//  j = string_find(flat_lab, i, UML_TR_PARSE_LBRACKET);
//  if (j < l) {
//    if (uml_pt.event == "")
//    {
//      uml_pt.event = string_extract(flat_lab, i, j - i);
//    }
//    k = string_find(flat_lab, j, UML_TR_PARSE_RBRACKET);
//    if (k < l && k < actst && k < sendst) {
//      uml_pt.guard = string_extract(flat_lab, j + 1, k - j - 1);
//      i = k + 1;
//    }
//    else {
//      uml_pt.unmatched_brackets_error = True; 
//      return uml_pt;
//    }
//  }
//  if (i >= l)
//    return uml_pt;
//  
//  /* find "/action", extract "action" */
//  j = string_find(flat_lab, i, UML_TR_PARSE_FSLASH);
//  if (j < l) {
//    if (uml_pt.event == "")
//      uml_pt.event = string_extract(flat_lab, i, j - i);
//    /**** BEGIN: ECR4041 Bug Fix: missing action after slash */
//// ECR 3786, doesn't work !!! outcommented fix
////	if (j + 1 <= l) {
////	   return uml_pt;
//    /**** END:   ECR4041 Bug Fix */
//    uml_pt.action = string_extract(flat_lab, j + 1, sendst - j - 1);
//    if(sendst < l)
//	uml_pt.sendev = string_extract(flat_lab,sendst + 1,l - sendst  - 1);
//    return uml_pt;
//  }
//  
//  uml_pt.event = flat_lab;
//  return uml_pt;
//}

// ECR 6776 end

string
uml_string_from_parsed_transition(UMLParsedTransition pt)
{
    string	l;

    l = pt.event;
    if (pt.guard != "")
	l = l + UML_TR_PARSE_LBRACKET + pt.guard + UML_TR_PARSE_RBRACKET;
    if (pt.action != "")
	l = l + UML_TR_PARSE_FSLASH + pt.action;
    if(pt.sendev != "")
	l = l + UML_TR_PARSE_SENDEV + pt.sendev;
    return(l);

}

UMLParsedTransition
uml_parsed_transition_for_oms(UMLParsedTransition pt)
{
    UMLParsedTransition	res = pt;
    res.event = to_oms_string(pt.event);
    res.guard = to_oms_string(pt.guard);
    res.action = to_oms_string(pt.action);
    res.sendev = to_oms_string(pt.sendev);
    return(res);
}


void
uml_initialize_parsed_transition(UMLParsedTransition pt, string link_label)
{
  /* initialize a parsed transition struct */
  pt.link_label = link_label;
  pt.event = "";
  pt.guard = "";
  pt.action = "";
  pt.sendev = "";
  pt.unmatched_parens_error = False; 
  pt.unmatched_brackets_error = False; 
  pt.symid = 0; 

  return;
}

string
uml_transition_event(gde_mapsymbol transition_sym)
{

  /* return the event for a transition */

  UMLParsedTransition parsed_transition;

  if (!gde_mapsymbol_is_link(transition_sym))
    return (NULL);

  parsed_transition = uml_parse_transition(to_gde_link(transition_sym));
  if (parsed_transition == NULL || uml_transition_syntax_error(parsed_transition))
    return NULL;

  return parsed_transition.event;
}

string
uml_transition_send_event(gde_mapsymbol transition_sym)
{

  /* return the event for a transition */

  UMLParsedTransition parsed_transition;

  if (!gde_mapsymbol_is_link(transition_sym))
    return (NULL);

  parsed_transition = uml_parse_transition(to_gde_link(transition_sym));
  if (parsed_transition == NULL || uml_transition_syntax_error(parsed_transition))
    return NULL;

  return parsed_transition.sendev;
}

string
uml_transition_action(gde_mapsymbol transition_sym)
{
  /* return the action for a transition */

  UMLParsedTransition parsed_transition;

  if (!gde_mapsymbol_is_link(transition_sym))
    return (NULL);

  parsed_transition = uml_parse_transition(to_gde_link(transition_sym));
  if (parsed_transition == NULL || uml_transition_syntax_error(parsed_transition))
    return NULL;

  return parsed_transition.action;
}


UMLParsedTransition
uml_parse_transition(gde_link transition)
{
  UMLParsedTransition	pt;
  gde_arc arc = NULL;
  string label = "";
  int i;

  list arcs = gde_link_arcs(transition);
  if (list_count(arcs) == 0)
    return NULL;

  /* find the label on the first labeled arc segment */
  for (i = 0; i < list_count(arcs); i = i + 1)
  {
    arc = list_get(arcs, i);
    label = gde_arc_label(arc);
    if (label != NULL && label != "")
      break;
  }

  /* parse the label and return the result */
  pt =  uml_parse_transition_label(label);
  if (arc != NULL)
  {
    pt.symid = gde_arc_psymid(arc);
  }
  return(pt);
}

boolean
uml_transition_syntax_error(UMLParsedTransition parsed_transition)
{
  if (parsed_transition.unmatched_parens_error || parsed_transition.unmatched_brackets_error)
    return True;
  
  return False;
}


list
uml_transition_name(gde_mapsymbol transition_sym)
{
  /* return the name for a transition */

  gde_link transition;
  list arcs, result;
  string label;
  int i;

  if (!(gde_mapsymbol_is_link(transition_sym)))
    return (NULL);

  transition = to_gde_link(transition_sym);

  /* the linkname is the name of the first labeled arc segment, with newlines replaced by blanks */
  label = "";
  arcs = gde_link_arcs(transition);
  for (i = 0; i < list_count(arcs); i = i + 1) {
    label = gde_arc_label(list_get(arcs, i));
    if (label != "")
      break;
  }

  /* the name has to be returned in a list */
  result = list_create("string", 0);
  list_append(result, replace_newlines(label, " "));
  return result;
}


//
// Change transition label if the link changes
//

void
uml_transition_inverse_map_func(gde_mapsymbol ms, link l, list rninfo,
				gocha_tp gocha_type)
{
    int		i;
    string	oldlabel = NULL;
    string	label = NULL;
    gde_link	gl;
    rename_data the_rename_data;

    if ( gocha_type != gochaRename )
    {
	print_error( "uml_transition_inverse_map_func() can only handle renames" );
	return; 
    }


    gl = to_gde_link(ms);
    oldlabel = gde_link_label(gl);

    for(i = 0; i < list_count(rninfo); i = i + 1)
    {
	the_rename_data = list_get(rninfo, i);
	if (l.id == the_rename_data.obj_id && 
		       the_rename_data.updated_attr == "name")
	{
	    if(the_rename_data.dependency_name == "Self")
		label = the_rename_data.label;
	    else
		label = l.name;
	    if (label != oldlabel)
	    {
		label = make_multi_line_label_from_string(label);
		gde_link_set_label(gl, label);
		gde_print_rename_notification_message(ms,
		    gde_mapsymbol_symbol_type_printname(ms) +
		    " renamed from '" + oldlabel + "' to '" + label + "'");
	    }
	    break;
	}
    }
}

string
uml_transition_guard(gde_mapsymbol transition_sym)
{
  /* return the guard for a transition */

  UMLParsedTransition parsed_transition;

  if (!gde_mapsymbol_is_link(transition_sym))
    return (NULL);

  parsed_transition = uml_parse_transition(to_gde_link(transition_sym));
  if (parsed_transition == NULL || uml_transition_syntax_error(parsed_transition))
    return NULL;

  return parsed_transition.guard;
}

// ECR 6369 begin
int
count_comma_separated_expressions(string label)
{
    string action = "", char;
    int pos = 0;
    int length = string_length(label);
    int parse_state = 0;
    int count = 0;

    while (pos < length)
    {
        char = string_extract(label, pos, 1);
        if (parse_state == 0)
        {
            if (char == ",")
            {
                count++;
                action = "";
            }
            else if (char == "(")
            {
                action += char;
                parse_state++;
            }
            else
                action += char;
        }
        else if (parse_state == 1)
        {
            action += char;
            if (char == ")")
                parse_state--;
        }

        pos++;
    }

    if (action != "")
        count++;
        
    return count;
}
// ECR 6369 end
