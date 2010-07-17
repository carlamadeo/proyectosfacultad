//
//      StP/UML 
//      Confidential property of Aonix
//      Copyright (c) 1997
//      All rights reserved

//
// JED - this file contains functions for parsing parameter lists
// 
const string UML_OPER_BAD_CH = "@#$()\\{}?`:'\";";
const string UML_OPER_GOOD_CH = string_repeat("_", string_length(UML_OPER_BAD_CH));
const string UML_OPER_BAD_CH_NO_COL = "@#$()\\{}?`'\";";
const string UML_OPER_GOOD_CH_NO_COL = string_repeat("_", string_length(UML_OPER_BAD_CH_NO_COL));
// ECR 4882 - begin
const string UML_OPER_BAD_CH_NO_COL_QUAT = "@#$()\\{}?`';";
const string UML_OPER_GOOD_CH_NO_COL_QUAT = string_repeat("_", string_length(UML_OPER_BAD_CH_NO_COL_QUAT));
// ECR 4882 - end

string uml_string_extract(string s, int startloc, int endloc)
// NOTE: startloc and endloc are 0-based and inclusive
{
	return string_extract(s,startloc,endloc-startloc+1);
}

enum format_enum {Canonical, Cxx, Ada95};

struct uml_argument {
	string argKind; // the kind of the arg
	string argName; // the name of the arg
	string argType; // the type
	string argVal;  // the default value
};

boolean delimitersMatch (string arg, string openDelim, string closeDelim)
// This function simply checks if the number of open and close delimiters
// is the same
{
	int delimCount = 0;
	int curLoc = 0;
	int length = string_length(arg);
	int nextOpen;
	int nextClose;

	// first search for open delims
	while (curLoc < length) {
		nextOpen = string_find(arg,curLoc,openDelim);
		if (nextOpen != length) {
			delimCount++;
		}
		curLoc = nextOpen + 1;
	}
	// now search for close delims
	curLoc = 0;
	while (curLoc < length) {
		nextClose = string_find(arg,curLoc,closeDelim);
		if (nextClose != length) {
			delimCount--;
		}
		curLoc = nextClose + 1;
	}
	// if delimCount is 0, the numbers matched
	return (0 == delimCount);

}


boolean parensMatch (string arg)
// This function simply checks if the number of open and close parens and
// brackets are the same 
// We check for brackets to allow for C++ instantiated types to appear in
// argument lists (bug13570)
{
    return (delimitersMatch(arg, "(", ")") &&
            delimitersMatch(arg, "<", ">"));
}

// this function takes an argument list (which may contain function
// pointers) in either the form:
//   arg1, arg2, ..., argN
// or the form (for Ada95)
//   arg1; arg2; ...; argN
// And returns it in the list with each argument separated.
// If any semicolons appear in the string, the latter form is assumed 
// and commas are insignificant

boolean
uml_split_arguments(string argument_str, list argList)
{
  int separatorLoc;
  boolean error = False;
  string curArg;
  string separator;

  /* no parsing needed */
  if (argument_str == "" || argument_str == NULL)
    return True;

  /* cursor initialization */
  int i = 0;
  int length = string_length(argument_str);

  /* decide on the separator */
  if (string_find(argument_str, 0, ";") == length)
      separator = ",";
  else
      separator = ";";

  // look for the first separator where the paren count is matched
  // this will get function pointer arguments as well as normal
  // arguments
	while (i < length) {

		// find next separator
		separatorLoc = string_find(argument_str, i, separator);
		// if no separator, done
		if (separatorLoc == length) {
			curArg = uml_string_extract(argument_str,i,separatorLoc-1);
			if (!parensMatch(curArg)) {
				error = True;
			} else {
				list_append(argList,curArg);
			}
			break;
		}

		curArg = uml_string_extract(argument_str,i,separatorLoc-1);

		// while parens aren't matched 
		while (!parensMatch(curArg)) {
			// find separator after that
			separatorLoc = string_find(argument_str, separatorLoc+1, separator);
			// if no separator and parens aren't matched, this is an error
			if ( separatorLoc == length) {
                // see if this is the last argument and if it has matching
                // parens. If so, it may be a default value constructor call
                // like this:
                //  object o = object(4,5)
                curArg = uml_string_extract(argument_str,i,separatorLoc-1);
                if (!parensMatch(curArg))
                    error = True;
				break;
			}
			// there was another separator
			// test to see if there are matched parens now
			curArg = uml_string_extract(argument_str,i,separatorLoc-1);
		}
		// if no error add current argument to list
		if (!error) {
			list_append(argList,curArg);
			i = separatorLoc + 1;
		} else {
			// otherwise, there is no reason to continue
			break;
		}
	}
	if (error) {
		// list_clear(argList);
		return False;
	}

	return True;
}

void uml_init_argument (uml_argument arg)
{
	arg.argKind = "";
	arg.argName = "";
	arg.argType = "";
	arg.argVal = "";
}

string uml_arg_struct_to_canonical_string (uml_argument arg)
{
	string ret = "";
	string tmpStr;

        if ("" != arg.argKind ) {
                ret += arg.argKind;
                ret += " ";
        }

        if ("" != arg.argName ) {
                ret += arg.argName;
        }

	if ("" != arg.argType ) {
		ret += ":" + arg.argType;
	}
	if ("" != arg.argVal ) {
		ret += "=" + arg.argVal;
	}
	
	// take the extra step to strip redundant whitespace (i.e. 
	// "   " becomes " "
	list argParts = string_to_list(ret," \n\r\t");
	int argLen = list_count(argParts);
	int i;
	ret = "";
	for(i=0;i<argLen;i++) {
		tmpStr = list_get(argParts,i);
		if ("" != tmpStr) {
			if ("" != ret) {
				ret += " ";
			}
			ret += tmpStr;
		}
	}
	list_clear(argParts);
	return ret;
}

string uml_arg_struct_to_cxx_string (uml_argument arg)
{
	string ret = "";

	if ("" != arg.argType ) 
		ret += arg.argType;

	if ("" != arg.argName )
		ret += " " + arg.argName;

	if ("" != arg.argVal )
		ret += " = " + arg.argVal;
	
	return ret;
}

string uml_arg_struct_to_ada95_string (uml_argument arg)
{
	string ret = "";
        string kind = "";

	if ("" != arg.argName )
            ret += arg.argName;

        if (arg.argKind == "inout")
            kind = "in out";
        else
            kind = arg.argKind;

        if (kind != "")
            kind += " ";

	if ("" != arg.argType ) 
            ret += ":" + kind + arg.argType;

	if ("" != arg.argVal )
            ret += " := " + arg.argVal;
	
	return ret;
}

// Tests to see if an argument is in UML format, and if not, convert
// it to UML format. If conversion is not possible, just return the
// argument as it was passed in (let the user deal with it).
// Any of the somethings can contain double colons and even
// whitespace, so only the general format will be verified. NOTE:
// paren matching will NOT be checked. It is assumed to have already
// been checked.
uml_argument uml_arg_string_to_struct(string argument)
{
	int singleColonLoc = -1;
	int equalsLoc = -1;
	int doubleColonLoc = -1;
	int argLen;
        string argKind = "";
	string argName = "";
	string argType = "";
	string defaultValue = "";
	string tmpStr;
	int i = 0;
	int tmpInt=0;
	uml_argument retVal;

	uml_init_argument(retVal);

	// pre-initialize to what we return on error

	// first, strip leading & trailing whitespace
	argument = list_to_string(string_to_list(argument, "\n"), "");
	argument = list_to_string(string_to_list(argument, "\r"), "");
        argument = string_strip( string_translate(argument,"\t"," "), "B"," ");
        // handle Ada95 default initializers
        argument = string_search_and_replace(argument, ":=", "=");

	retVal.argName = argument;

	argLen = string_length(argument);

	// JED - C++ users may use syntax like the following to provide a default
	// value for an object passed by reference or by value:
	// 		x:Foo = Foo(5,"Howdy")
	// which is valid UML syntax. So that the code that tests for function
	// pointer arguments is not confused by the parentheses, remove the
	// default value (and the equal sign) before checking.
	equalsLoc = string_find(argument,0,"=");
	if (argLen != equalsLoc) {
		// the optional assignment is there
		defaultValue = uml_string_extract(argument,equalsLoc+1,argLen-1);
		defaultValue = string_strip(defaultValue,"B"," \n\t\r");
		if (0 == equalsLoc)
		{
			// error - there is nothing to the left of the '='
			return retVal;
		}

		argument = uml_string_extract(argument,0,equalsLoc-1);
		argument = string_strip(argument,"B"," \n\t\r");
		if (defaultValue == "" || argument == "") {
			// missing default value or argument. This is an error
			return retVal;
		}
		argLen = string_length(argument);
	}
	// JED - special case for function pointers. It is possible for a
	// user to use UML format to specify the argument list for a
	// function pointer. This means we have to check to make sure that
	// the number of parens before the single colon we find matches,
	// otherwise we could have found a single colon in the argument
	// list, which is a case we don't handle (function pointer
	// arguments are not changed except for having leading & trailing
	// spaces stripped.
	// Since function pointers can never be in UML format, check for
	// them up front to save time.
	if (string_find(argument,0,"(") != argLen) {
		// found a paren, skip UML checks by simulating UML search
		// failure
		singleColonLoc = argLen;
	} else {
		// look for the UML form
		doubleColonLoc = string_find(argument,0,"::");
		singleColonLoc = string_find(argument,0,":");
		while (singleColonLoc == doubleColonLoc && singleColonLoc != argLen 
		    // Added 3rd check for ECR4268
		    && doubleColonLoc+2 != argLen) {
			// look 2 past singleColon since we know it is really a double
			// colon
			singleColonLoc = string_find(argument,singleColonLoc+2,":");
			doubleColonLoc = string_find(argument,doubleColonLoc+2,"::");
		}
		// ECR4170: Special case, global namespace qualifier. The UML delimiter is
		// the first of those three colons.
		if(singleColonLoc > 2 && string_extract(argument, singleColonLoc - 2, 2) == "::")
			singleColonLoc -= 2;
	}

	if (singleColonLoc != argLen) {
                list preColon = list_create("string",0);
		// single colon was found - must be UML format
		// argName = strip_whitespace(uml_string_extract(argument,0,singleColonLoc-1));
                argName = uml_string_extract(argument,0,singleColonLoc-1);
                preColon = string_to_list (argName, " \n\t\r");
                int numTokens = list_count(preColon);
                if (numTokens <= 1) // if (numTokens == 1)
                    argKind = "";
                else
                {
                    argName = list_get(preColon, numTokens - 1);
                    list_delete (preColon, numTokens - 1);
                    argKind = list_to_string(preColon, " ");
                }
                argKind = string_strip(argKind,"B"," \n\t\r");
		argName = string_strip(argName,"B"," \n\t\r");
		if (argName == "") {
			// we have an empty name - this is an error
			return retVal;
		}
		
		argType = uml_string_extract(argument,singleColonLoc+1,argLen-1);
		argType = string_strip(argType,"B"," \n\t\r");
		if (argType == "") {
			// missing type expression. This is an error
			return retVal;
		}
	} else {
		// if we get here, one of two things are true:
		//   - we have a normal C-style argument
		//   - we have a function pointer argument
		// I am assuming normal non-function pointer arguments don't have
		// parens, so if I find a paren, I return the whole argument as-is.

		argLen = string_length(argument);
		if (argLen != string_find(argument,0,"(")) {
			// found a paren - return this parameter unchanged
			return retVal;
		}

		// OK - normal C-style argument
		// then, the following is true
		//   - everything from the end of the string to the first space,
                //     *, or & found (searching backward) will be the type.
		//   - everything else is the name
		i = argLen-1;

		// JED - If any character can form the end of a C++ type declaration
		// then put it here. DO NOT put alphanumerics, as the alphanumeric
		// portion is assumed to be followed by a space or one of these
		// special characters
		const string CXX_TYPE_TERMCHARS = " >]*&";
		
		while ( i > 0 ) {
			tmpStr = string_extract(argument,i,1);

			if (string_length(CXX_TYPE_TERMCHARS) !=
					string_find(CXX_TYPE_TERMCHARS,0,tmpStr)) {
				// i is the position of the last space, *, or &
				break;
			}
			i--;
		}
		if ( 0 == i ) {
			// there is an error: the format should be
			//    something something
			// with at least 1 space
			return retVal;
		}
		if (i+1 == argLen) { // there is no name
			argName = "";
		} else {
			argName = strip_whitespace(uml_string_extract(argument,i+1,argLen-1));
		}
		// no need to strip argName - i is the last space
		argType = uml_string_extract(argument,0,i);
		argType = string_strip(argType,"T"," ");
		if (argType == "" || argName == "") {
			// missing stuff - shouldn't happen
			return retVal;
		}
	}
        retVal.argKind = argKind;
	retVal.argName = argName;
	retVal.argType = argType;
	retVal.argVal = defaultValue;
	return retVal;
}

// This function takes an argument list in the following format:
//    arg1, arg2, arg3, ..., argN
// arg1-N are in either of the two formats:
//    type-expr [ varname [ = default-value ]]    (C-style)
// or
//    [ varname : ] type-expr [ = default-value ] (UML style)
// 
// In the first case, every attempt to convert to the UML format will
// be made. In either case, if the expression can be parsed, all
// leading and trailing whitespace will be removed from varname,
// type-expr, and default-value. They will be combined into the
// canonical UML form as shown by example below:
// Example:
//    int i                      ---> i:int
//    int i, string s="Howdy"    ---> i:int, s:string="Howdy"
//    i : int = 3                ---> i:int=3
//    jjjjj ddddd                ---> ddddd:jjjjj
//    void (*f)(int), string s   ---> void (*f)(int), s:string
//    void (*c::f)(int)          ---> void (*c::f)(int)
//    (int i=3, string s="boo")  ---> (int i=3, string s="boo")
//    int:5 i                    ---> int:5 i
//
// Note that the last one was unchanged. This is not due to explicit
// support for bitfields in C/C++, but rather it failed to parse as a
// valid UML argument string. I will let the parsing code comments 
// explain what that means.
// Also note that if there are an unmatched number of parentheses in
// the argument list, it will fail to parse and will be returned
// unchanged.


string uml_args_to_format (string arguments, format_enum format)
{
    arguments = string_strip(arguments,"B"," \n\t\r");
    if (NULL == arguments || "" == arguments) {
	// nothing to do
	return arguments;
    }
    
    list unparsedArgs = list_create("string",0);
    
    // split the string into arguments
    if (!uml_split_arguments(arguments,unparsedArgs)) {
	// an error occurred parsing
	return arguments;
    }
    
    int i;
    string formattedArg;
    string separator;
    string returnVal = "";
    uml_argument arg;

    int numArgs = list_count(unparsedArgs);

    if (format == Ada95)
        separator = "; ";
    else
        separator = ", ";

    for(i=0;i<numArgs;i++) {
	if (i > 0) {
	    returnVal = returnVal + separator;
	}
        arg = uml_arg_string_to_struct(list_get(unparsedArgs,i));
        
	if (format == Cxx)
	    formattedArg = uml_arg_struct_to_cxx_string(arg);
	else if (format == Ada95)
            formattedArg = uml_arg_struct_to_ada95_string(arg);
        else // (format == Canonical)
	    formattedArg = uml_arg_struct_to_canonical_string(arg);

	returnVal = returnVal + formattedArg;
    }
    return returnVal;
}

// some utility functions
// operations can be input in either C-style:
//      ret_type opname (args)
// or UML style:
//      opname (args) : ret_type
//
struct uml_operation {
    string opName;
    string opArgs;
    string opRetType;
    boolean error;
};

void uml_init_operation(uml_operation op)
{
	op.opName = "";
	op.opArgs = "";
	op.opRetType = "";
	op.error = False;
}


// ECR 6560 begin
string uml_clarify_operation_name(string opStr)
{
    string result = "";
    
    opStr = string_strip( string_translate(opStr,"\r\t","  "), "B"," ");
    int len = string_length(opStr);
    if (len <= 0)
        return opStr;

    // first, get the name part - everything up to the parens
	string opNamePart;
    string opRest;
    int parenLoc = string_find(opStr, 0, "(");
    if (parenLoc != len)
    {
	    opNamePart = uml_string_extract(opStr, 0, parenLoc - 1);
	    opRest = uml_string_extract(opStr, parenLoc, len - 1);
    }
    else
    {
        opNamePart = opStr;
        opRest = "";
    }

    // search for last 'true' space character in name
    int spLoc = 0;
    int spLastLoc = 0;
    while (spLoc < parenLoc)
    {
        spLastLoc = spLoc;
        spLoc = string_find(opNamePart, spLoc + 1, " ");
    }

    // Now spLastLoc points to last 'true' space in string (if any)
    if (spLastLoc > 0)
    {
        // spaces in name; everything between last space and 
        // opening paren is name
        string tmp1 = uml_string_extract(opNamePart, 0, spLastLoc - 1);
        string tmp2 = uml_string_extract(opNamePart, spLastLoc, parenLoc - 1);
        opNamePart = tmp1 + " " + strip_whitespace(tmp2);
    }
    else
    {
        // no spaces in name
        opNamePart = strip_whitespace(opNamePart);
    }

    result = opNamePart + opRest;
    return result;
}
// ECR 6560 end

// this idea was copied from our OMT product
uml_operation opCache;
string lastOp = "";

// parse a UML or C-style operation declaration into a struct
uml_operation uml_parse_operation (string opStr)
{
    uml_operation retVal;
    int i;
    
// ECR 6560 begin
    opStr = uml_clarify_operation_name(opStr);
// ECR 6560 end

    // first, clean it up a bit
    opStr = string_strip( string_translate(opStr,"\n\r\t","   "), "B"," ");

    // next, check the cache
    if ( lastOp == opStr && !opCache.error ) 
    {
	    // been there, done that.  return the cached result
	    return opCache;
    }
    
    uml_init_operation(retVal);
    
    // first, get the name part - everything up to the parens
    int parenLoc = string_find(opStr,0,"(");
    
    if (0 != parenLoc) 
    {
	    // there is something like a name
	    string opNamePart;
	    string opArglistPart;
	    int len = string_length(opStr);
	    if (parenLoc != len) 
        {
	        // found a paren. Get everything up to it
	        opNamePart = uml_string_extract(opStr,0,parenLoc-1);
	        opArglistPart = uml_string_extract(opStr,parenLoc,len-1);
	    } 
        else 
        {
	        // everything is name part
	        opNamePart = opStr;
	        opArglistPart = "";
	    }
	
	    list word_list = string_to_list(opNamePart," ");
	    // in both formats:
	    //		ret_type opname (args)
	    // and
	    //		opname (args) : ret_type
	    // the operation name is the last thing before the argument list
	    // So, convert the string to a list and take the last item.
	    // JED - Not so simple. So handle C++ operator overloads, take
	    //  the last 2 things if the list contains the word operator
	    len = list_count(word_list);
	
	    if (len > 0) 
        {
	        // should always be true
	        i = list_find(word_list,0,"operator");
	        if (i == len) 
            { 
                // not found
		        retVal.opName = list_get(word_list,len-1);
	        } 
            else 
            { 
                // it's an operator
		        retVal.opName = list_get(word_list,i);
		        if ((i+1) < len) 
                { 
                    // make sure they provided the operator!!!
		            retVal.opName += " " + list_get(word_list,i+1);
		        }
	        }
	    } 
        else 
        {
	        print_error("uml_parse_operation: assertion failed");
	        retVal.error = True;
	        return retVal;
	    }
	    // JED - Bug# 11932
	    // Do some validity checking on the name. If it's an operator, it can
	    // be in the format "operator x", otherwise it may contain no space.
	    // In either case, it may not contain "illegal" characters.
	    word_list = string_to_list(retVal.opName," ");
	    if (list_count(word_list) > 1) 
        {
	        if ("operator" != list_get(word_list,0)) 
            {
		        print_error("Illegal operation name: " + retVal.opName);
		        retVal.error = True;
	        } 
            else if (list_count(word_list) > 2) 
            {
		        print_error("Illegal operator syntax: " + retVal.opName);
		        retVal.error = True;
	        }
	    
	    }
	    // now check for illegal characters

	    if (string_translate(retVal.opName, UML_OPER_BAD_CH, UML_OPER_GOOD_CH) != retVal.opName) 
        {
	        // illegal characters were found
	        print_error("Illegal characters in operation name: " + retVal.opName);
	        retVal.error = True;
	    }
	
	
	    // JED - TODO: Since we can't do anything with the return type
	    // in the GDE because the GTE owns that item, I have not
	    // implemented any parsing or checking for return types.
	    // However, they should be discarded in all cases.
	
	    // time to go after the argument list
	    if ("" != opArglistPart) 
        {
	        // first, we must remove the parentheses surrounding it
	        // we know opArglistPart[0] is the open, so we must look
	        // from the end to find the close paren. This can be
	        // confused if the user input a C-style function return
	        // type in UML format after the arg list, but if they do
	        // that they are smart enough to figure out why this
	        // function ate what they typed.
	        len = string_length(opArglistPart);
	    
	        for (i=len-1;i>0;i--) 
            {
		        if ( string_find(opArglistPart,i,")") != len ) 
                {
		            // found at i
		            break;
		        }
	        }
	        if (i == 0) 
            {
		        // didn't find it. Remove the leading paren and
		        // proceed normally. We could give up here, but
		        // because the code later will put parens around the
		        // argument list, it won't hurt anything to try and
		        // parse the arguments
		        opArglistPart = string_extract(opArglistPart,1,len-1);
	        } 
            else 
            {
		        // strip off the parens
		        opArglistPart = string_extract(opArglistPart,1,i-1);
	        }
	    
	        // parse this baby
	        retVal.opArgs = uml_args_to_format(opArglistPart, Canonical);
	    }
    }
    
    // update the cache
    lastOp = opStr;
    opCache = retVal;
    
    return retVal;
}

// JED - get the next token in the input string, as terminated by terminators.
// termRequired is a flag that inidates if the end of the string is a
// sufficient token terminator.
string
uml_get_token(string input, string terminators, boolean termRequired,
		string openChar, string closeChar)
{
	int i;
	int len = string_length(input);
	int term_len = string_length(terminators);
	string arg = "";
	string tmp = NULL;
	boolean inString = False;
	boolean gotTerm = False;
	int nestingLevel = 0;

	if (0 == len || 0 == term_len) {
		return NULL;
	}

	for(i=0;i<len;i++) {
		tmp = string_extract(input,i,1);
		if ("\"" == tmp) {
			inString = !inString;
		}
		if (inString) {
			if ("\\" == tmp) {
				// this is an escaped character. Get the next one
				if (i+1<len) { // if there's a character to get
					arg = arg + tmp;
					i++;
					tmp = string_extract(input,i,1);
				} else {
					// error - premature end of string
					break;
				}
			}
		} else {
			if (0==nestingLevel && term_len!=string_find(terminators,0,tmp)) {
				// got a terminator
				gotTerm = True;
				break;
			}
			if (tmp == openChar) {
				nestingLevel++;
			}
			if (tmp == closeChar) {
				if (0 == nestingLevel) {
					return NULL;
				}
				nestingLevel--;
			}
		}
		arg = arg + tmp;
	}
	if (inString || (termRequired && !gotTerm) || nestingLevel != 0) {
		// error
		return NULL;
	}
	return arg;
}


// Instantiated classes can have a variety of formats:
// 	I1	- using refines link for arguments
// 	P1<2> - Anonymous with constant argument
// 	P1<cls1,4> - Anonymous with mixed arguments
// 	P1<P2<3>,cls1> - Anonymous with anonymous arguments and normal args
// 	P1<" < > \""> - Anonymous with a nighmare string
// To handle all these cases, we need a semi-sophisticated parser.
// The return value will be a list where the 0th item is the class name, and
// the arguments 1-n will be the arguments. The arguments will each be a list
// of the same format: 0th element is a string, 1-nth are lists.
// 
list
uml_parse_instantiated_class(string iclass_label)
{
	list ret = list_create("all",0);
	list arg;
	int i;
	string working_copy = string_strip(iclass_label,"B"," \n\t\r");
	int len, len_tok;
	string tok;
	string term_char;


	// first thing - go after the two parts - the class name and the argument
	// list
	tok = uml_get_token(working_copy,"<", False,"<",">");
	if (NULL == tok) {
		// error parsing
		return NULL;
	}

	// tok now has the class name
	list_append(ret,tok);
	len = string_length(working_copy);
	len_tok = string_length(tok);

	if (len == len_tok) {
		// no arguments
		return ret;
	}

	// we have an argument list
	// first, remove the leading "<"
	if (len_tok+1 == len) {
		// nothing after <
		return NULL;
	}
	working_copy = string_extract(working_copy,len_tok+1,len-len_tok-1);
	working_copy = string_strip(working_copy,"L"," ");
	len = string_length(working_copy);

	while (len > 0) {
		// now, get the arguments. It can be terminated by a number of
		// characters
		tok = uml_get_token(working_copy,",>",True,"<",">");
		if (NULL == tok) {
			// invalid argument
			return NULL;
		}
		len_tok = string_length(tok);
		arg = uml_parse_instantiated_class(tok);
		if (NULL == arg) {
			return NULL;
		}
		// remove the token and the terminating character
		if (len_tok+1 == len) {
			working_copy = "";
			len = 0;
		} else {
			working_copy = string_extract(working_copy,
					len_tok+1,len-len_tok-1);
			working_copy = string_strip(working_copy,"L"," ");
			len = string_length(working_copy);
		}

		list_append(ret,arg);
	}

	return ret;
}

string
uml_format_instantiated_class_arguments(list parsed_class)
{
	int i;
	list arg;
	string ret = "";
	int len = list_count(parsed_class);

	for(i=1;i<len;i++) {
		arg = list_get(parsed_class,i);
		ret += uml_format_instantiated_class(arg);
		if (i+1<len) {
			ret += ", ";
		}
	}
	return ret;
}

string
uml_format_instantiated_class(list parsed_class)
{
	string ret = "";
	list arg;
	int i;
	int len;

	if (NULL == parsed_class) {
		return NULL;
	}

	ret = string_translate(list_get(parsed_class,0),
                               UML_BAD_CH,
                               UML_GOOD_CH);
	len = list_count(parsed_class);

	if (len > 1) {
		ret += "<" + uml_format_instantiated_class_arguments(parsed_class) + ">";
	}
	return ret;
}

boolean
uml_instantiated_class_uses_type(list parsed_class, string type)
{
	int i;
	list arg;
	int len = list_count(parsed_class);
	string name;

	name = list_get(parsed_class,0);
	if (type == name) {
		return True;
	}
	for(i=1;i<len;i++) {
		arg = list_get(parsed_class,i);
		if(uml_instantiated_class_uses_type(arg,type)) {
			return True;
		}
	}
	return False;
}


list
uml_instantiated_class_replace_type(list parsed_class, string oldType, string newType)
{
	int i;
	list arg;
	int len = list_count(parsed_class);
	list new_class = list_create("all",0);
	string name;

	name = list_get(parsed_class,0);
	if (oldType == name) {
		name = newType;
	}
	list_append(new_class,name);

	for(i=1;i<len;i++) {
		arg = list_get(parsed_class,i);
		list_append(new_class,
				uml_instantiated_class_replace_type(arg,oldType,newType));
	}
	return new_class;
}

string
uml_instantiated_class_node_to_string(node n)
{
	string s = n.name;

	if (NULL != n.sig && "" != n.sig) {
		s += "<" + n.sig + ">";
	}
	return s;
}
