 
//      sccsid[] = %W% %Y% %D%
//      StP/SE
//      Confidential property of Interactive Development Environments, Inc.
//      Copyright (c) 1995
//      All rights reserved
//
// decomp.inc: routines to help handle dfe decompositions
 

//
// Decomposition navigation
//
string 
FindDecompositionName(string filename, int pindex)
{
    if (filename == "top" || filename == "0")
    	return (to_string(pindex));
    else
    	return (filename + "." + pindex);
}        

file
FindDecomposition(string filename, int pindex)
{
    string newfilename = FindDecompositionName(filename, pindex);

    return find_by_query("file[DfeDiagram && name == '${newfilename}']");
} 

// parent node structure
struct parent_node
{
    string filename;	    // file which contains parent node
    string pindex;  	    // Relative Index of parent node
};

const string SEPARATOR = ".";
const int SEPLENGTH = string_length(SEPARATOR);

parent_node
FindParentInfo(string filename)
{
    int index = 0, fileindex = 0, length;
    string substring = filename, query;
    parent_node result = NULL;

    if (str_no_val(filename) || filename == "top")
	return result;	

    // Figure out the Parent filename, which is the string before
    // the final separator in the filename.
    while (True)
    {
    	length = string_length(substring);
    	index = string_find(substring, 0, SEPARATOR);

	// If there is no separator or nothing after the separator, stop.
    	if (length <= index + SEPLENGTH)
    	    break;
    	substring = string_extract(substring, index + SEPLENGTH, 
    	    	    	    	    	      length - (index + SEPLENGTH));
    	fileindex = fileindex + index + SEPLENGTH;
    }

    // No separator found ==> parent is 0 or top
    if (substring == filename)
    {
        // 0's parent is top
    	if (substring == "0")
    	    filename = "top";
    	else
    	    filename = "0";
    }
    else
    {
        // don't extract final separator
        filename = string_extract(filename, 0, fileindex - SEPLENGTH);
    }
    result.filename = filename;
    result.pindex = substring;
    return result;    
}

//
//  Find the parent Process node of the filename
//
node
FindParentNode (string filename)
{
    string query;
    parent_node parent = FindParentInfo(filename);

    query = "node[Process && " + 
    	    	"node_refs[file[DfeDiagram && name == '${parent.filename}']]
    	    	&& items[RelativeIndex && value == '${parent.pindex}']]";
    return find_by_query(query);
}

