//////////////////////////////////////////////////////////////////////////////
//
// 	sccsid[] = %W% %Y% %D%
// 	StP/Core
// 	Confidential property of Interactive Development Environments, Inc.
// 	Copyright (c) 1992-1995
// 	All rights reserved
//
//	File history checking.
//
//////////////////////////////////////////////////////////////////////////////


// Check that a file hasn't been modified since last repsync.

int 
file_history(file fileobj)
{
    return(file_history_query("id == " + fileobj.id, "", False));
}

// Check that no files in the id_list have been modified since the
// last repsync.

int 
file_id_list_history(string id_list)
{
    return(file_history_query(id_list, "", False));
}

// Check that files in file_restrictor have not been updated since the
// last repsync. file_restrictor is an oms query expression e.g.
// "(ChenDiagram && name == 'Earl')"

file_hist
file_hist_get(int id)
{
    list	l = list_select("file_hist[id = ${id}]");

    if (list_count(l) == 0)
	return(NULL);

    return(list_get(l, 0));
}

int 
file_history_query(string file_restrictor, string sort_clause, boolean ignore_annot_files)
{
    string query;
    file   file;
    int    errors = 0;
    file_hist	fhm, fhs;
    int		tmd, tsy;

    query = "file[" + file_restrictor + "]" + sort_clause;

    // Don't report files that can't be generated into the repository
    for_each_in_select(query, file)
    {
	if (file.type == "ObjectAnnotation")
            continue;

    	if (file_is_rep_source(file.type))
    	{
	    if (file.lsync_id == 0)
	    {
		file_error(file, "was saved with syntax errors.");
		errors = errors + 1;
		continue;
	    }
	    if (file.lmdfy_id > 0)
	    {
		fhm = file_hist_get(file.lmdfy_id);

		if (fhm == NULL)
		    continue;

		// this doesn't work under NT, so I'm using file_hist_raw_time
		// instead
		//tmd = string_to_time(fhm.time, NULL);
		tmd = file_hist_raw_time(fhm);
		fhs = file_hist_get(file.lsync_id);

		if (fhs == NULL)
		{
		    file_error(file, "was saved with syntax errors.");
		    errors = errors + 1;
		    continue;
		}
		//tsy = string_to_time(fhs.time, NULL);
		tsy = file_hist_raw_time(fhs);

		if (tsy < tmd)
		{
		    file_error(file, "was saved with syntax errors.");
		    errors = errors + 1;
		}
	    }
    	}
    }
    return(errors);
}

// If names is the empty string, then return all files of type 
// 'filetype_expression'; otherwise, return all files of that type whose
// names are in the string 'names'.  

list 
file_list(string filetype_expression, string filenames)
{
    list   names, files, result;
    int    i;
    string name, query, filetype_clause;

    if (filetype_expression == "" && filenames == "")
	return list_select("file[]");

    if (filenames == "")
	return list_select("file[${filetype_expression}]");

    names = string_to_list(filenames, ", \n");
    result = list_create("file", 0);

    for (i = 0; i < list_count(names); i = i + 1)
    {
    	name = list_get(names, i);

    	if (filetype_expression == "")
	{
    	    query = "file[name == '${name}']";
    	} else
	{
    	    query = "file[(${filetype_expression}) && name=='${name}']";
	}
	files = list_select(query);
    	list_concatenate(result, files);
    }
    return(result);
}

// Like file_list, only creates an ID list named 'id_list'

void 
file_id_list_create(string id_list, string type_expression, string filenames)
{
    string query, name_expression;

    if (filenames == "")
    {
    	query = "file[" + type_expression + "]";
    }
    else
    {
	name_expression = "(name == '" + 
			  list_to_string(string_to_list(filenames, ", \n"),
					 "' || name == '") +
			  "')";

    	query = "file[(${type_expression}) && ${name_expression}]";
    }
    id_list_create(query, id_list);
}

// Destroys the file_id_list named 'id_list'

void 
file_id_list_destroy(string id_list)
{
    id_list_free(id_list);
}

// Issue an error of the form ${file.type} '${file.name}' ${msg}
 
void
file_error(file file, string msg)
{
    print_error(app_type_print_string(file) + " '" + file.name + "' " + msg);
}

string
qt_file_name(file file)
{
    if (file.name == "")
	return(" ");
    return(" '" + file.name + "' ");
}

