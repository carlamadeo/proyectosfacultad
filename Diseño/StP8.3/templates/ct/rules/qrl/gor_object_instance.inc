//
//      sccsid[] = %W% %Y% %D%
//      StP/Booch
//      StP/OMT
//      StP/OO
//	StP Core
//      Confidential property of Interactive Development Environments, Inc.
//      Copyright (c) 1992-1995
//      All rights reserved
//

//
// Message label parsing
//
// Syntax:	[sequence ":" ] name ["(" args ")" ]
//

struct	BoochParsedMsg
{
	string	seqno;
	string	msg;
	string	attrs;
	boolean isMsg;
};

BoochParsedMsg	TheCachedParsedMessage = NULL;
string		TheLastParsedMessageLabel = NULL;
const	string	MSG_SNO_DLM = ":";
const	string	MSG_L_PAREN = "(";
const	string	MSG_R_PAREN = ")";

// Check if string is a valid sequence number for a message
const	string	valid_chars_in_seqno = "0123456789.*";
const	int	valid_chars_in_seqno_len = 12;

boolean
string_is_valid_seqno(string sno)
{
    int		l, i, ind;
    string	c;

    if (sno == NULL)
    {
	return(True);
    }
    sno = strip_whitespace(sno);
    l = string_length(sno);
    for(i = 0; i < l; i = i + 1)
    {
	c = string_extract(sno, i, 1);
	if (string_find(valid_chars_in_seqno, 0, c) >= valid_chars_in_seqno_len)
	{
	    return(False);
	}
    }
    return(True);

}

BoochParsedMsg
booch_parse_message_label(string lab)
{
    int		i, j, k, l, dbl_sep;
    BoochParsedMsg	pe;

    if (TheLastParsedMessageLabel != NULL && TheLastParsedMessageLabel == lab)
    {
	return(TheCachedParsedMessage);
    }

    i = 0;
    j = 0;
    k = 0;
    l = string_length(lab);

    if (l == 0)
    {
	pe.msg = lab;
	pe.attrs = NULL;
	pe.isMsg = True;
	pe.seqno = NULL;
	TheCachedParsedMessage = pe;
	return pe;
    }

    /* find "sno: msg(attrs) extract sno, but make sure that
       we don't accidentally parse the scope separator out
       */
    dbl_sep = string_find(lab, i, MSG_SNO_DLM+MSG_SNO_DLM);
    pe.seqno = NULL;
    j = string_find(lab, i, MSG_SNO_DLM);
    if (j < l && dbl_sep != j)
    {
	pe.seqno = string_extract(lab, 0, j);
	if (string_is_valid_seqno(pe.seqno) == True)
	{
	    if (j < (l-1))
	    {
		lab = string_extract(lab, j + 1, l - (j+1));
	    } else {
		lab = "";
	    }
	}
    }

    l = string_length(lab);
    if (l == 0)
    {
	pe.msg = lab;
	pe.attrs = NULL;
	pe.isMsg = True;
	TheCachedParsedMessage = pe;
	return pe;
    }

    /* find "(attrs)", extract "attrs" */
    j = string_find(lab, i, MSG_L_PAREN);
    if (j < l)
    {
	pe.isMsg = False;
	pe.msg = string_extract(lab, i, j - i);
	k = string_find(lab, j, MSG_R_PAREN);
	if (k < l)
	{
	    pe.attrs = string_extract(lab, j + 1, k - j - 1);
	} else {
	    pe.attrs = string_extract(lab, j + 1, l - j - 1);
	}
    } else {
	pe.msg = lab;
	pe.attrs = NULL;
	pe.isMsg = True;
    }

    if (pe.seqno != NULL)
    {
	pe.seqno = strip_whitespace(pe.seqno);
	if (pe.seqno == "")
	{
	    pe.seqno = NULL;
	}
    }
    if (pe.msg != NULL)
    {
	pe.msg = strip_whitespace(pe.msg);
    }
    if (pe.attrs != NULL)
    {
	pe.attrs = replace_newlines(pe.attrs, " ");
    }
    TheCachedParsedMessage = pe;
    return pe;
}

string
booch_make_link_name_from_parsed_msg(BoochParsedMsg r)
{
    string	lab = "";

    if (r.msg != NULL && r.msg != "")
    {
	lab = lab + r.msg;
    }
    if (r.isMsg == False)
    {
	lab = lab + MSG_L_PAREN;
	if (r.attrs != NULL && r.attrs != "")
	{
	    lab = lab + r.attrs;
	}
	lab = lab + MSG_R_PAREN;
    }
    return(lab);

}


//
// Simple minded parser to match signatures, currently only
// count number of args
//

const	string	ARG_SEPARATOR = ",";


int
oo_operation_count_args(string arglist, boolean signatureonly)
{
    int		argc = 0;
    int		ll, i;
    boolean	in_string = False;
    boolean	bs = False;
    int		nested_args = 0;
    string	ch, str_start;

    if (arglist == NULL || arglist == "")
    {
	return(argc);
    }
    arglist = string_strip(arglist, "B", " ");
    ll = string_length(arglist);
    // expecting name(params)
    if (signatureonly == False)
    {
	i = string_find(arglist, 0, MSG_L_PAREN);
	if (i < ll)
	{
	    arglist = string_extract(arglist, i, ll-i);
	    arglist = string_strip(arglist, "L", MSG_L_PAREN);
	    arglist = string_strip(arglist, "T", MSG_R_PAREN);
	} else { // no params ...
	    return(0);
	}
    }
    arglist = string_strip(arglist, "B", " ");

    ll = string_length(arglist);
    if (ll > 0)
    {
	argc = 1;
    }
    for(i = 0; i < ll; i = i + 1)
    {
	ch = string_extract(arglist, i, 1);
	if (bs == True)
	{
	    bs = False;
	    continue;
	}
	if (ch == "\\")
	{
	    bs = True;
	    continue;
	}
	if (in_string == True)
	{
	    if (ch == str_start)
	    {
		in_string = False;
	    }
	    continue;
	}
	if (ch == MSG_L_PAREN)
	{
	    nested_args = nested_args + 1;
	    continue;
	}
	if (ch == MSG_R_PAREN)
	{
	    if (nested_args > 0)
		nested_args = nested_args - 1;
	    continue;
	}
	if (ch == "\"" || ch == "'")
	{
	    str_start = ch;
	    in_string = True;
	    continue;
	}
	if (nested_args > 0)
	{
	    continue;
	}
	if (ch == ARG_SEPARATOR)
	{
	    argc = argc + 1;
	}
    }
    return(argc);
}

boolean
oo_operation_signature_matches(string formal, boolean fsig, string actual, boolean asig)
{
    int		formal_args;
    int		actual_args;

    formal_args = oo_operation_count_args(formal, fsig);
    actual_args = oo_operation_count_args(actual, asig);

    // message("oo_operation_signature_matches:\n\tformal("+formal_args+"): "
    //			+ formal + "\n\tactual("+actual_args+"):"+ actual);
    if (formal_args == actual_args)
	return(True);
    return(False);
}
/***************************************************
 * Simple generic parser to split strings into tokens
 ******************************************************/

list
string_to_tokens(string s, string tokenseps, string stringsep)
{
    int		ind, l, sl, ssl;
    string	ch, word, str_start;
    list	tokens;
    const	string	bschar = "\\";
    boolean	bs, in_string;

    tokens = list_create("string", 0);
    sl = string_length(tokenseps);
    if (stringsep == NULL)
	ssl = 0;
    else
	ssl = string_length(stringsep);
    l = string_length(s);
    word = "";
    bs = False;
    in_string = False;


    for(ind = 0; ind < l; ind++)
    {
	ch = string_extract(s, ind, 1);
	if (bs == True)
	{
	    bs = False;
	    word = word + ch;
	    continue;
	}
	if (ch == bschar)
	{
	    bs = True;
	    word = word + ch;
	    continue;
	}
	if (in_string == True)
	{
	    if (ch == str_start)
	    {
		in_string = False;
	    }
	    word = word + ch;
	    continue;
	}
	if (ssl > 0 && string_find(stringsep, 0, ch) < ssl)
	{
	    str_start = ch;
	    in_string = True;
	    word = word + ch;
	    continue;
	}
	if (sl > 0 && string_find(tokenseps, 0, ch) < sl)
	{
	    if (word != "")
		list_append(tokens, word);
	    list_append(tokens, ch);
	    word = "";
	} else {
	    word = word + ch;
	}
    }
    if (word != "")
	list_append(tokens, word);
    return(tokens);
}

boolean
token_replace(list tokens, string old, string new)
{
    int		ind;
    boolean	rc = False;

    for(ind = list_count(tokens)-1; ind >= 0; ind--)
    {
	if (list_get(tokens, ind) == old)
	{
	    list_set(tokens, ind, new);
	    rc = True;
	}
    }
    return(rc);
}

string
tokens_to_string(list tokens)
{
    return(list_to_string(tokens, ""));
}
