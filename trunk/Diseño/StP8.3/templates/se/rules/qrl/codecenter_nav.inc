//
//      sccsid[] = %W% %Y% %D%
//      StP/SE
//      Confidential property of Interactive Development Environments, Inc.
//      Copyright (c) 1995
//      All rights reserved
//
//
//
//    File: codecenter_nav.inc
//
//    Description:
//
//    Routines to browse from CodeCenter to StP/SE editors
//    and from the editors to CodeCenter
//
/////////////////////////////////////////////////////////////////////////////
 


#include "qrl/include/se_utils.inc"
#include "rules/qrl/ct.inc"

void 
SEFromProgrammingEnvironment(string editor, string token)
{
    node	the_node = NULL;
    node_ref	nr;
    cntx	the_cntx = NULL;
    cntx_ref	cr;
    file	the_file;
    string	query;
    string	SCE_NODE_TYPES = "(Module|DataModule|LibraryModule)";
    string	SCE_CNTX_TYPES = "(DataIn|DataOut|DataInOut|ControlIn|ControlOut)";
    string	DSE_NODE_TYPES = "(Sequence|Selection|Typedef|Enumeration)";

    print_message("Browsing to object '" + token  + "' in " + to_upper(editor) + " ...");

    if (editor == "sce" )
    {
	if (token == "")
	{
	    print_error("Specify object to browse to in SCE...", NULL);
	    return;
	}

	the_node = node_from_name_type(token, SCE_NODE_TYPES);

	if (the_node == NULL)
	{
	    token = replace_spaces_with_underscores(token);
	    the_node = node_from_name_type(token, SCE_NODE_TYPES);
	}

	// if not a node, check if parameter
	if (the_node == NULL)
	{
	    the_cntx = cntx_from_name_type(token, SCE_CNTX_TYPES);
	    if (the_cntx == NULL)
	    {
		token = replace_spaces_with_underscores(token);
		the_cntx = cntx_from_name_type(token, SCE_CNTX_TYPES);
	    }
	}
	else	
	{
	    query = "node_ref[node_id == ${the_node.id}]";
	    for_each_in_select(query, nr)
	    {
		query = "file[id == ${nr.file_id}]";
		for_each_in_select(query, the_file)
		{
		    NavLoadAndFind(the_file.name, nr.appid);
		    return;
		}
	    }
	}

        if (the_cntx == NULL)
        {
            print_error("Object " + token + " not found.", NULL);
	    return;
        }
	else
	{
	    query = "cntx_ref[cntx_id == ${the_cntx.id}]";
	    for_each_in_select(query, cr)
	    {
		query = "file[id == ${cr.file_id}]";
		for_each_in_select(query, the_file)
		{
		    NavLoadAndFind(the_file.name, cr.appid);
		    return;
		}
	    }
	}
    } 

    else if (editor == "dse" )
    {
        if (token == "")
        {
            print_error("Specify object to browse to in DSE...", NULL);
            return;
        }
 
        the_node = node_from_name_type(token, DSE_NODE_TYPES);
 
	if (the_node == NULL)
	{
	    token = replace_spaces_with_underscores(token);
	    the_node = node_from_name_type(token, SCE_NODE_TYPES);
	}

	if (the_node != NULL)
        {
            query = "node_ref[node_id == ${the_node.id}]";
            for_each_in_select(query, nr)
            {
                query = "file[id == ${nr.file_id}]";
                for_each_in_select(query, the_file)
                {
                    NavLoadAndFind(the_file.name, nr.appid);
                    return;
                }
            }
        }
    }

    else
    {
	print_error("Object " + token + " not found.", NULL);
	return;
    }
}

const string ValidPETypes = "Module DataModule Sequence Selection Typedef Enumeration";
list ValidPETypesList = string_to_list(ValidPETypes, " ");
const int       ValidPETypesCount = 6;

void
NavigateToPE(int id)
{
    node	n = NULL;
    string	query;

    query = "node[id == ${id}]";
    n = find_by_query(query);

    if (n == NULL)
    {
	return;
    }

    if (list_find(ValidPETypesList, 0, n.type) != ValidPETypesCount)
    {
	NavigateToPeByName(n.name);
	return;
    }
}

const string	PROG_ENV = toolinfo_variable("ProgrammingEnvironment");
const string	CODECENTER_CLASS = "cc4d";

void
NavigateToPeByName(string node_name)
{
    if (node_name == NULL)
	return;

    editor_send_msg(CODECENTER_CLASS, "list \"" 
		    + to_c_identifier(node_name) + "\"", 0);
}

