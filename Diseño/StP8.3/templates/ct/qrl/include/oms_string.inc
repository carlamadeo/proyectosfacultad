
/*
    converts a list of strings to a string containing the following:
    to_oms_string(string_in_list) + separator + to_oms_string(string_in_list)
    + separator + ... + to_oms_string(string_in_list)
*/
string
list_to_oms_string(list the_list, string separator)
{
    string element, result = "";
    int ix, count = list_count(the_list);

    for (ix = 0; ix < count; ix = ix + 1)
    {
    	result = result + to_oms_string(list_get(the_list, ix));
    	if (ix < count - 1)
    	    result = result + separator;
    }
    return result;
}
