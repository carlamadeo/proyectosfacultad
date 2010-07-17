//
//      StP/Booch
//      StP/OMT
//      StP/OO
//	StP Core
//      Confidential property of Interactive Development Environments, Inc.
//      Copyright (c) 1992-1995
//      All rights reserved
//

const string TR_PARSE_LPAREN = "(";
const string TR_PARSE_RPAREN = ")";
const string TR_PARSE_LBRACKET = "[";
const string TR_PARSE_RBRACKET = "]";
const string TR_PARSE_FSLASH = "/";
const string TR_PARSE_SENDEV = "^"; 
const string TR_PARSE_SEDOT = "."; 

/* structure to hold the results of a parsed transition */
struct OMTParsedTransition
{
  string link_label;
  string event;
  string attribs;
  string condition;
  string action;
  string sendev_ev;
  string sendev_attr;
  string sendev_class;

  int unmatched_parens_error; 
  int unmatched_brackets_error; 
  int symid;
};


/* pseudo-static vars used by the parser,
   to avoid having to parse the same label twice in a row
*/

OMTParsedTransition	omt_pt;
boolean			omt_pt_first_time = True;

string
dbg_string_extract(string s, int st, int l)
{
    string res = string_extract(s, st, l);
    message("\nstr ex: <" + s +  "> start: " + st + " len: " + l);
    message("res: <" + res + ">\n");
    return(res);
}

OMTParsedTransition
omt_parse_transition_label(string link_label)
{
  /* A parser for labels on state transitions. These have the form:
   *
   *          event(attribs)[condition]/action
   * ! action can have args, but we don't parse these out
   * but we need to make sure that we don't accidentally use the
   * actions arges as event args if the event doesn't have any args
   *
   * Returns the elements in an OMTParsedTransition struct, along with any error conditions.
   * Replace by yacc grammar?. */

  /* cursor positions: 3 cursors, plus the end of string */
  int i, j, k, l, actst, guardst;
  string	flat_lab;

  // should that be empty ("") or space (" ")
  flat_lab = replace_newlines(link_label, " ");

  if (omt_pt_first_time)
    omt_pt_first_time = False;
  else if (flat_lab == omt_pt.link_label)
    /* memorization -- we just parsed this label, return the previous result */
    return omt_pt;

  /* initialize pt struct */
  omt_initialize_parsed_transition(omt_pt, flat_lab);
  
  /* no parsing needed */
  if (flat_lab == "" || flat_lab == NULL)
    return omt_pt;

  i = 0; j = 0; k = 0; l = string_length(flat_lab);

  // find "(attribs)", extract "attribs" 
  // make sure we don't mistake action args for event args
  // or a parenthesis in the guard for the arguments

  actst = string_find(flat_lab, i, TR_PARSE_FSLASH);
  guardst = string_find(flat_lab, i, TR_PARSE_LBRACKET);

  j = string_find(flat_lab, i, TR_PARSE_LPAREN);
  if (j < l)
  {
      // found action args or guard condition expression and not event args  ...
      if (actst < j || guardst < j)
      {
	j = l;
      }
  }

  if (j < l)
  {
    omt_pt.event = string_extract(flat_lab, i, j - i);
    k = string_find(flat_lab, j, TR_PARSE_RPAREN);
    if (k < l && k < actst && k < guardst)
    {
      omt_pt.attribs = string_extract(flat_lab, j + 1, k - j - 1);
      i = k + 1;
    }
    else {
      omt_pt.unmatched_parens_error = TRUE; 
      return omt_pt;
    }
  }
  if (i >= l)
    return omt_pt;

  /* find "[condition]", extract "condition" */
  j = string_find(flat_lab, i, TR_PARSE_LBRACKET);
  if (j < l) {
    if (omt_pt.event == "")
    {
      omt_pt.event = string_extract(flat_lab, i, j - i);
    }
    k = string_find(flat_lab, j, TR_PARSE_RBRACKET);
    if (k < l && k < actst) {
      omt_pt.condition = string_extract(flat_lab, j + 1, k - j - 1);
      i = k + 1;
    }
    else {
      omt_pt.unmatched_brackets_error = TRUE; 
      return omt_pt;
    }
  }
  if (i >= l)
    return omt_pt;
  
  /* find "/action", extract "action" */
  j = string_find(flat_lab, i, TR_PARSE_FSLASH);
  if (j < l) {
    if (omt_pt.event == "")
      omt_pt.event = string_extract(flat_lab, i, j - i);
    omt_pt.action = string_extract(flat_lab, j + 1, l - j - 1);
    return omt_pt;
  }
  
  omt_pt.event = flat_lab;
  return omt_pt;
}

string
string_from_parsed_transition(OMTParsedTransition pt)
{
    string	l;

    l = pt.event;
    if (pt.attribs != "")
	l = l + TR_PARSE_LPAREN + pt.attribs + TR_PARSE_RPAREN;
    if (pt.condition != "")
	l = l + TR_PARSE_LBRACKET + pt.condition + TR_PARSE_RBRACKET;
    if (pt.action != "")
	l = l + TR_PARSE_FSLASH + pt.action;
    return(l);

}

OMTParsedTransition
omt_parsed_transition_for_oms(OMTParsedTransition pt)
{
    OMTParsedTransition	res = pt;
    res.event = to_oms_string(pt.event);
    res.attribs = to_oms_string(pt.attribs);
    res.condition = to_oms_string(pt.condition);
    res.action = to_oms_string(pt.action);
    res.sendev_ev = to_oms_string(pt.sendev_ev);
    res.sendev_attr = to_oms_string(pt.sendev_attr);
    res.sendev_class = to_oms_string(pt.sendev_class);
    return(res);
}


void
omt_initialize_parsed_transition(OMTParsedTransition pt, string link_label)
{
  /* initialize a parsed transition struct */
  pt.link_label = link_label;
  pt.event = "";
  pt.attribs = "";
  pt.condition = "";
  pt.action = "";
  pt.sendev_ev = "";
  pt.sendev_attr = "";
  pt.sendev_class = "";
  pt.unmatched_parens_error = FALSE; 
  pt.unmatched_brackets_error = FALSE; 
  pt.symid = 0; 

  return;
}

