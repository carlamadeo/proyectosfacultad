////////////////////////////////////////////////////////////////////////////////
//
//      sccsid[] = %W% %Y% %D%
//      StP/Core
//      Confidential property of Aonix
//      Copyright (c) 1992-97
//      All rights reserved
//
//      Parser for StP rules/diagram/table/annotation files
//
//	Public functions:
//
//		RuleParseFile
//		RuleParseString
//		RuleString
//		RuleAttr
//		RuleAttrStringVal
//		RuleAttrIntVal
//		RuleAttrFloatVal
//		RuleId
//		AttrAttr
//
//
////////////////////////////////////////////////////////////////////////////////


struct Rule
{
    int        line;	// line number in file
    string     id;	// optional id
    list       attrs;	// list of rule attributes
    string     name;
};

enum RuleTokenId { 
    RuleTokenEof,
    RuleTokenLBracket,
    RuleTokenRBracket,
    RuleTokenLiteral,
    RuleTokenString,
    RuleTokenIntPair,
    RuleTokenInteger,
    RuleTokenFloat,
    RuleTokenFloatPair,
    RuleTokenAttrlist
};

struct RuleAttribute
{
    string       name;
    RuleTokenId  type;
    int          line;
    list         value;
};

////////////////////////////////////////////////////////////////////////////////
//
// take a filename, parse the file and return a list of parsed rules.
//
////////////////////////////////////////////////////////////////////////////////

list
RuleParseFile(string fname)
{
    if (!read_file_access(fname))
    {
	print_error("Cannot open '" + fname + "' for parsing.");
	return(NULL);
    }
    return(RuleParseString(read_file(fname), fname));
}

////////////////////////////////////////////////////////////////////////////////
//
// take a string of rules and return the parsed rule structures
//
////////////////////////////////////////////////////////////////////////////////

list
RuleParseString(string ip, string fname)
{
    if (ip == "")
    {
	return(NULL);
    }
    return(rpRules(ip, fname));
}

////////////////////////////////////////////////////////////////////////////////
//
// take a RuleAttribute and return it's string value
//
////////////////////////////////////////////////////////////////////////////////

string
RuleAttrStringVal(RuleAttribute a)
{
    if (a.value == NULL || list_count(a.value) == 0)
    {
	print_error(a.name + " Attribute has no value (line " + a.line + ").");
	return("");
    }
    return(list_get(a.value, 0));
}

////////////////////////////////////////////////////////////////////////////////
//
// take a RuleAttribute and return it's int value
//
////////////////////////////////////////////////////////////////////////////////

int
RuleAttrIntVal(RuleAttribute a)
{
    if (a.value == NULL || list_count(a.value) == 0)
    {
	print_error(a.name + " Attribute has no value (line " + a.line + ").");
	return(0);
    }
    return(list_get(a.value, 0));
}

// for IntPair

int
RuleAttrOtherIntVal(RuleAttribute a)
{
    if (a.type != RuleTokenIntPair || list_count(a.value) < 2)
    {
	print_error(a.name + " Attribute value missing (line " + a.line + ").");
	return(0);
    }
    return(list_get(a.value, 1));
}

////////////////////////////////////////////////////////////////////////////////
//
// take a RuleAttribute and return it's float value
//
////////////////////////////////////////////////////////////////////////////////

float
RuleAttrFloatVal(RuleAttribute a)
{
    if (a.value == NULL || list_count(a.value) == 0)
    {
	print_error(a.name + " Attribute has no value (line " + a.line + ").");
	return("");
    }
    return(list_get(a.value, 0));
}

// for FloatPair

float
RuleAttrOtherFloatVal(RuleAttribute a)
{
    if (a.type != RuleTokenFloatPair || list_count(a.value) < 2)
    {
	print_error(a.name + " Attribute value missing (line " + a.line + ").");
	return(0);
    }
    return(list_get(a.value, 1));
}

////////////////////////////////////////////////////////////////////////////////
//
// take a list of rule structures and convert them to a rule string.
// can be passed to write_file in order to write out a file of rules.
//
////////////////////////////////////////////////////////////////////////////////

string
RuleString(list rules)
{
    int    i;
    string res;

    for (i = 0, res = ""; i < list_count(rules); i++)
    {
	res += rpRuleString(list_get(rules, i));
    }
    return(res);
}

string
RuleId(Rule r)
{
    return(r.id);
}

// return a named string attribute

RuleAttribute
RuleAttr(Rule r, string attrname)
{
    int            i;
    RuleAttribute  attr;
    
    for (i = 0; i < list_count(r.attrs); i++)
    {
	attr = list_get(r.attrs, i);

	if (attr.name == attrname)
	{
	    return(attr);
	}
    }
    return(NULL);
}

RuleAttribute
AttrAttr(RuleAttribute a, string attrname)
{
    int            i;
    RuleAttribute  attr;
    
    for (i = 0; i < list_count(a.value); i++)
    {
	attr = list_get(a.value, i);

	if (attr.name == attrname)
	{
	    return(attr);
	}
    }
    return(NULL);
}

////////////////////////////////////////////////////////////////////////////////
//
// Everything below here is internal to the parser and should not be
// called from outside this file
//
////////////////////////////////////////////////////////////////////////////////

struct rpState
{
    int        line;	// current line number
    string     rem;	// remainder of input stream
    string     file;	// filename of parse input
};

struct rpToken {
    RuleTokenId    id;
    string	   value;
};

const int rpIndent = 4;

string
rpRuleString(Rule rule)
{
    string res = "";
    int    i;

    res += rule.name;

    if (rule.id != NULL)
    {
	res += " " + rule.id;
    }
    res += "\n{\n";

    for (i = 0; i < list_count(rule.attrs); i++)
    {
	res += rpAttrString(list_get(rule.attrs, i), rpIndent);
    }
    res += "}\n\n";
    return(res);
}

string
rpAttrString(RuleAttribute attr, int indent)
{
    string res = "";
    int    i;

    res += string_repeat(" ", indent) + "{ " + attr.name;

    if (attr.type == RuleTokenAttrlist)
    {
	res += "\n";

	for (i = 0; i < list_count(attr.value); i++)
	{
	    res += rpAttrString(list_get(attr.value, i), indent + rpIndent);
	}
	res += string_repeat(" ", indent) + "}\n";
    } else if (attr.type == RuleTokenString)
    {
	res += " \"" + list_get(attr.value, 0) + "\" }\n";
    } else if (attr.type == RuleTokenIntPair ||
	       attr.type == RuleTokenFloatPair)
    {
	res += " " + list_get(attr.value, 0) + 
	       " " + list_get(attr.value, 1) + " }\n";
    } else
    {
	res += " " + list_get(attr.value, 0) + " }\n";
    }
    return(res);
}

// return the list of rule structures

list
rpRules(string ip, string fname)
{
    Rule       rl;	
    rpState    s;
    rpToken    tk;
    list       rules = list_create("Rule", 0);

    s.line = 1;
    s.rem = ip;
    s.file = fname;

    for(tk = rpTokenNext(s); tk.id != RuleTokenEof; tk = rpTokenNext(s))
    {
	if (tk.id == RuleTokenLiteral)
	{
	    if (tk.value == "Include")
	    {
		// need to handle stp_file_path
		tk = rpTokenNext(s);
		list_concatenate(rules, RuleParseFile(tk.value));
		continue;
	    }
	    rl.line = s.line;
	    rl.name = tk.value;
	    tk = rpTokenNext(s);

	    if (tk.id == RuleTokenInteger || tk.id == RuleTokenLiteral)
	    {
		rl.id = tk.value;
		tk = rpTokenNext(s);
	    } else
	    {
		rl.id = NULL;
	    }
	    if (tk.id == RuleTokenLBracket)
	    {
		rl.attrs = rpRuleAttrs(s);
	    } else
	    {
		if (rl.id != NULL)
		{
		    rpSyntaxError(s, tk, "Expecting Left Bracket");
		} else
		{
		    rpSyntaxError(s, tk, "Expecting Rule Id or Left Bracket");
		}
		rl.attrs = NULL;
	    }
	    list_append(rules, rl);
	} else
	{
	    rpSyntaxError(s, tk, "Expecting Rule");
	}
    }
    return(rules); 
}

void
rpSyntaxError(rpState s, rpToken tk, string err)
{
    string    c, es = tk.value;
    int       i, l = string_length(s.rem);

    for (i = 0; i < l; i++)
    {
	c = string_extract(s.rem, i, 1);

	if (c == "\n")
	{
	    break;
	}
	es += c;
    }
    print_error("Syntax Error near line " +
		s.line +
		" in file '" + 
		s.file +
		"': " +
		es);
    print_error(err);
}

rpToken
rpTokenNext(rpState s)
{
    rpToken    tk;
    string     c;

    rpSkipSpace(s);
    tk.value = "";

    if (s.rem == "")
    {
	tk.id = RuleTokenEof;
	tk.value = "";
	return(tk);
    } 
    c = rpCharNext(s);

    if (c == "{")
    {
	rpPop(s);
	tk.id = RuleTokenLBracket;
	tk.value = c;
    } else if (c == "}")
    {
	rpPop(s);
	tk.id = RuleTokenRBracket;
	tk.value = c;
    } else if (c == "\n")
    {
	rpPop(s);
	++s.line;
	tk = rpTokenNext(s);
    } else if (c == "/")
    {
        rpPop(s);
	c = rpCharNext(s);

	if (c == "/")
	{
	    rpPop(s);

	    for (c = rpCharNext(s); c != "" && c != "\n"; c = rpCharNext(s))
	    {
		rpPop(s);
	    }
	    tk = rpTokenNext(s);
	} else
	{
	    tk.id = RuleTokenLiteral;
	    tk.value = "/";
	}
    } else if (c == "\"")
    {
	rpPop(s);

	for (c = rpCharNext(s); c != ""; c = rpCharNext(s))
	{
	    if (c == "\\")
	    {
		if (rpCharNext(s) == "\"")
		{
		    rpPop(s);
		}
	    } else if (c == "\"")
	    {
		rpPop(s);
		break;
	    }
	    tk.value += c;
	    rpPop(s);
	}
	tk.id = RuleTokenString;
    } else
    {
	tk.value += c;
	rpPop(s);

	for (c = rpCharNext(s); 
	     c != "\n" && c != " " && c != "{" && c != "}" && c != ""; 
	     c = rpCharNext(s))
	{
	    tk.value += c;
	    rpPop(s);
	}
	if (string_converts(tk.value, "int"))
	{
	    tk.id = RuleTokenInteger;
	} else if (string_converts(tk.value, "float"))
	{
	    tk.id = RuleTokenFloat;
	} else
	{
	    tk.id = RuleTokenLiteral;
	}
    }
    //message("line " + s.line + tk);
    return(tk);
}

void
rpSkipSpace(rpState s)
{
    if (s.rem == "")
    {
	return;
    }
    if (rpCharNext(s) == " ")
    {
	rpPop(s);
	rpSkipSpace(s);
    }
}

string
rpCharNext(rpState s)
{
    if (s.rem == "")
    {
	return("");
    }
    return(string_extract(s.rem, 0, 1));
}

void
rpPop(rpState s)
{
    int len = string_length(s.rem);

    if (len <= 1)
    {
	s.rem = "";
    } else
    {
	s.rem = string_extract(s.rem, 1, len - 1);
    }
}

void
rpPush(rpState s, string val)
{
    s.rem = val + s.rem;
}

list
rpRuleAttrs(rpState s)
{
    rpToken        tk;
    list           attrs = list_create("RuleAttribute", 0);
    RuleAttribute  attr;

    for (tk = rpTokenNext(s); tk.id != RuleTokenRBracket; tk = rpTokenNext(s))
    {
	if (tk.id == RuleTokenLBracket)
	{
	    attr = rpRuleAttr(s);

	    if (attr != NULL)
	    {
		list_append(attrs, attr);
	    } 
	} else
	{
	    rpSyntaxError(s, tk, "Expecting Rule Attribute");
	    break;
	}
    }
    return(attrs);
}

RuleAttribute
rpRuleAttr(rpState s)
{
    rpToken        tk;
    RuleAttribute  attr = NULL;

    tk = rpTokenNext(s);

    if (tk.id == RuleTokenLiteral)
    {
	attr.name = tk.value;
	attr.line = s.line;
	tk = rpTokenNext(s);

	if (tk.id == RuleTokenLiteral || tk.id == RuleTokenString)
	{
	    attr.type = tk.id;
	    attr.value = list_create("string", 0);
	    list_append(attr.value, tk.value);
	    rpAttrEnd(s);
	} else if (tk.id == RuleTokenInteger)
	{
	    attr.value = list_create("int", 0);
	    list_append(attr.value, to_int(tk.value));
	    tk = rpTokenNext(s);

	    if (tk.id == RuleTokenInteger)
	    {
		attr.type = RuleTokenIntPair;
		list_append(attr.value, to_int(tk.value));
		rpAttrEnd(s);
	    } else if (tk.id == RuleTokenRBracket)
	    {
		attr.type = RuleTokenInteger;
	    } else
	    {
		rpSyntaxError(s, tk, "Invalid Rule Attribute");
		rpAttrEnd(s);
	    }
	} else if (tk.id == RuleTokenFloat)
	{
	    attr.value = list_create("float", 0);
	    list_append(attr.value, to_float(tk.value));
	    tk = rpTokenNext(s);

	    if (tk.id == RuleTokenFloat)
	    {
		attr.type = RuleTokenFloatPair;
		list_append(attr.value, to_float(tk.value));
		rpAttrEnd(s);
	    } else if (tk.id == RuleTokenRBracket)
	    {
		attr.type = RuleTokenFloat;
	    } else
	    {
		rpSyntaxError(s, tk, "Invalid Rule Attribute");
		rpAttrEnd(s);
	    }
	} else if (tk.id == RuleTokenLBracket)
	{
	    attr.type = RuleTokenAttrlist;
	    rpPush(s, tk.value);
	    attr.value = rpRuleAttrs(s);
	} else
	{
	    rpSyntaxError(s, tk, "Expecting Attribute Value");
	    break;
	}
    } else if (tk.id != RuleTokenRBracket)
    {
	rpSyntaxError(s, tk, "Expecting Attribute Name");
    }
    return(attr);
}

void
rpAttrEnd(rpState s)
{
    rpToken  tk = rpTokenNext(s);

    if (tk.id != RuleTokenRBracket)
    {
	rpSyntaxError(s, tk, "Invalid Rule Attribute");

	while (tk.id != RuleTokenEof && tk.id != RuleTokenRBracket)
	{
	    tk = rpTokenNext(s);
	}
    }
}

