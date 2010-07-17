//
//      sccsid[] = %W% %Y% %D%
//      StP/T
//      Confidential property of Interactive Development Environments, Inc.
//      Copyright (c) 1994
//      All rights reserved
//
//      version_utils.inc -- get version string from product_version file
//

string 
find_value_in_rule(string rule, string keyword, string version_special)

{
    int index, end_index, i;
    string result_str;

    keyword = keyword + " ";
    index = string_find(rule, 0, keyword);

    if (index == string_length(rule))
	return "";

    end_index = string_find(rule, index, "}");
    
    result_str = string_extract(rule, index + string_length(keyword), 
                   end_index - (index + string_length(keyword))); 

    index = string_find(result_str, 0, "\"");
    end_index = string_find(result_str, index + 1, "\"");

    result_str = string_extract(result_str, index + 1, 
		    end_index - 1 - index);

    if (version_special != NULL)
    {
	if (version_special == "Major")
	{
	    index = -1;		
            for (i = 0; i <= 2; i = i + 1)
	        index = string_find(result_str, index + 1, ".");
	    result_str = string_extract(result_str, 0, 
                             //string_length(result_str) - index);
                             index);
	}
	else
	{
	    index = -1;		
            for (i = 0; i <= 2; i = i + 1)
	        index = string_find(result_str, index + 1, ".");
	    result_str = string_extract(result_str, index, 
                             string_length(result_str) - index + 1);
	}    
    }

    return result_str;

} // end of find_value_in_rule()





// version_special is either NULL: no special case, or "Major" to return
// the major part of the version number, or "Minor" to return the minor part
// of he version number

string
rule_value(string filename, string keyword, string version_special)

{
    string str;

    filename = file_name_expand(filename);

    if (filename == NULL || filename == "")
	return "";

    str = read_file(filename);

    return find_value_in_rule(str, keyword, version_special);
}
