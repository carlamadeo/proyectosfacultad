//////////////////////////////////////////////////////////////////////////////
//
// 	sccsid[] = @(#)namespace.inc	4.1  94/08/03
// 	StP/IM
// 	Confidential property of Interactive Development Environments, Inc.
// 	Copyright (c) 1992-1993
// 	All rights reserved
//
//	Check for namespace conflicts
//
//////////////////////////////////////////////////////////////////////////////


// Check for the existence of a node of type 'type' with name 'name'.

boolean
node_namespace(string type, string name)
{
    if (selection_count("node[${type} && node_refs && name == '${name}']"))
	return(True);

    return(False);
}

