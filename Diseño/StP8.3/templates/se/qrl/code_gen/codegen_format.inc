//
//      sccsid[] = %W% %Y% %D%
//      StP/SE
//      Confidential property of Interactive Development Environments, Inc.
//      Copyright (c) 1993
//      All rights reserved
//

// 	codegen_format.inc
//
//	This is a format file for SE code generation. 
//	Users should modify this file if they wish to change the
//	format of generated code. 
//


// generate_function
//
// The template for functions. Includes the function comment and 
// function definition.
//

module_def_tp
generate_function(prop_tp props, module_tp one_module)
{
	
    string      str;
    int         i;
    module_def_tp	module_def;
	     
module_def.id = get_unique_id_annot(props, one_module.module); 

module_def.comment = 
"\n" +
START_FUNCTION_HEADER_TAG +
"\n" +
" * (" + module_def.id + ")\n" +
" *\n" +
" *  Function: " + one_module.name + "\n" + 
" *\n" +
" *  Calls: \n" + 
	submodule_list(one_module, 0) + 
" *\n" +
" *  Description: \n" + 
" *" + TAB + "Module created by the SE Code Generator.\n" +
" *\n" + 
" *  Comment: \n" + 
" *" + TAB + one_module.comment + "\n" +
END_HEADER_TAG;

module_def.header =  
START_MODULE_TAG + module_def.id + " */\n" +
one_module.return_type + "\n" 
+ one_module.name; 

if (props.kandr_or_ansi == "ansi")
    module_def.header = module_def.header + "(" + one_module.params + ")\n" +  
"{";

else
    module_def.header = module_def.header + "(" + one_module.param_kandr_str + ")\n" + one_module.params + "\n{";
 
module_def.body = 
START_CODE_TAG + " \n" +
	    one_module.code_body + "\n" +
	    submodule_list(one_module, 1) + "\n" +
	    libcalls_list(props, one_module) + "\n" +
	    globals_list(one_module) + 
" \n" +
	END_CODE_TAG;

module_def.end = 
END_MODULE_TAG;

    return module_def;
}

string
generate_proto(prop_tp props, module_tp one_module)
{
    string	str;

    // the prototype
    if (props.kandr_or_ansi == "ansi")
	str = one_module.return_type + " " + one_module.name + "(" + one_module.param_proto_str + ");\n";
    else 
	str = one_module.return_type + " " + one_module.name + "();\n";

    return str;
}

// generate_adt
//
// The template for functions. Includes the function comment and
// function definition.
//
 
adt_def_tp
generate_adt(prop_tp props, adt_tp adt)
{
    string      str;
    int         i;
    adt_def_tp	adt_def;

    adt_def.id = get_unique_id_annot(props, adt.element);

adt_def.comment = 
START_DATA_HEADER_TAG +
"\n" +
" * (" + adt_def.id + ")\n" +
" *\n" +
" * Data Element: " + adt.name + "\n" +
" *\n" +
" * Description:\n" +
" *" + TAB + "Created by the SE Code Generator.\n" +
" *\n" +
" *  Comment: \n" +
" *" + TAB + generate_data_comment(props, adt.element.id) + "\n" +
" *\n" +
END_HEADER_TAG;

str = 
START_DATA_DEFINITION_TAG + adt_def.id + " */\n";
 
    if (adt.child_elements != NULL)
	str = str + print_structure(adt, props.gen_comment );
    else
	str = str + print_single_line_type(adt);

    str = str + END_DATA_DEFINITION_TAG + "\n";

    adt_def.def = str;

    return adt_def;
} 


// generate_c_impl_file_header
//
// This template is for the implementation file (c file) header.
//

string
generate_c_impl_file_header(string filename, set modules)
{

    string	str;
    int		i;

str =  
START_FILE_HEADER_TAG +
" \n" +
" * File: " + filename + "\n" +
" * \n" +
" *" + TAB + "created on " + time_to_string(time_now(), NULL) + " for \n" + 
" *" + TAB + user() + "@" + hostname() + " from system " + current_system() + "\n" +
" * \n" +
" * Functions:\n" +
	print_header_object_list(modules) +
" * \n" +
END_HEADER_TAG; 

    return str;
}

// generate_c_header_file_header
//
// This template is for the header file header.
//
 
string
generate_c_header_file_header(string filename, set adts)
{
 
    string      str;
     
    str =
START_FILE_HEADER_TAG +
" \n" +
" * File: "  + filename + "\n" +
" * \n" +
" *" + TAB + "created on " + time_to_string(time_now(), NULL) + " for \n" +
" *" + TAB + user() + "@" + hostname() + " from system " + current_system() + "\n" +" *\n" +
" * Components:\n" +
	print_header_object_list(adts) +
" *\n" +
END_HEADER_TAG;

    return str;
}
 

// generate_proto_file_header
//
// This template is for the prototype file header.
//
 
string
generate_proto_file_header(string filename)
{
 
    string      str;
 
    str =
START_FILE_HEADER_TAG +
" \n" +
" * File: " + filename + "\n" +
" * \n" +
" *" + TAB + "created on " + time_to_string(time_now(), NULL) + " for \n" +
" *" + TAB + user() + "@" + hostname() + " from system " + current_system() + "\n" +" *\n" +
" * Prototypes for all StP/SE functions defined in this directory\n" +
" *\n" +
" * Warning: This file is regenerated automatically by StP/SE.\n" +
" *          DO NOT EDIT.\n" +
" *\n" +
END_HEADER_TAG;
 
    return str;
}


// submodule_list
//
// Prints list of submodules, one per line.

string 
submodule_list(module_tp one_module, int get_args)
{ 
    int         i; 
    string	str = "";
    node        one_submodule;

    for ( i = 0; i < list_count(one_module.submodule_list); i++)
    {
	one_submodule = list_get(one_module.submodule_list, i);
	if (!get_args)
	    str = str + " *" + TAB + to_c_identifier(one_submodule.name) + "\n";
	else
	    str = str + TAB + "/*  " + to_c_identifier(one_submodule.name) 
		    + "(" + get_arguments(one_module.module, one_submodule) + ");  */\n";
    }  

    return str;

}


// get_arguments
// 	Prints arguments for module call 
//
 
string                     
get_arguments(node parent, node one_node)
{
    int         i;
    string      query, str = "";
    link	one_link;
    cntx_ref	one_cntx_ref = NULL;
    cntx	one_cntx;
    list	arg_list = list_create("cntx_ref", 0);

    query = "link[to_node_id=${one_node.id} && from_node_id=${parent.id} && link_refs]";

    for_each_in_select(query, one_link)
    {
        query = "cntx[link_id=${one_link.id} && cntx_refs]";

	for_each_in_select(query, one_cntx)
	{
	    // when there are multiple references to the same cntx it is 
	    // ambiguous, so just find the first reference to avoid duplicates
	    query = "cntx_ref[cntx_id==${one_cntx.id}]";
	    one_cntx_ref = find_by_query(query);
	    if (one_cntx_ref != NULL)
		list_append(arg_list, one_cntx_ref);
	}
    }

    if (one_node.type == "Module")
	check_actuals_match_formals(one_node, parent, list_count(arg_list));

    sort_params_by_x_y(arg_list);
 
    for ( i = 0; i < list_count(arg_list); i++)
    {
        one_cntx_ref = list_get(arg_list, i);

	query = "cntx[${one_cntx_ref.cntx_id}]";
	one_cntx = find_by_query(query);
 
        if (i > 0)
            str = str + ",";
        str = str + " " + to_c_identifier(one_cntx.name) ;
            
    }
 
    return str;
}

void
check_actuals_match_formals(node one_node, node parent, int arg_count)
{
    string	query, arg_str = "args";
    cntx	one_cntx;
    int		formal_count = 0;
    boolean	formals_found = False;

    if (checkForVarArgs(one_node.id))
	return;

    if (FC != NULL)
    {
	// get all formal parameter references
	query = "cntx[cntx_refs && link[to_node_id == ${one_node.id} && from_node_id == ${FC.id}]]";

	for_each_in_select(query, one_cntx)
	{
	    formals_found = True;
	    ++formal_count;
	}
    }

    if (arg_count != formal_count)
    {
	if (formal_count == 1) arg_str = "arg";

	++ERRORS;
	print_error("Module '" + one_node.name + "' is expecting " + formal_count + " " + arg_str + ", but is being passed " + arg_count + " in call from Module '" + parent.name + "'.");
    }
}


string
print_header_object_list(set objects)
{
    int         i;
    string      str = "";
    string      obj;

    for ( i = 0; i < set_count(objects); i++)
    {
        obj = set_get_element(objects, i);
        str = str + " *" + TAB + obj + "\n";
    }
 
    return str;
}

string 
module_list(set modules)
{ 
    int         i; 
    string	str = "";
    string	one_module;
	
    for ( i = 0; i < set_count(modules); i++)
    {
	one_module = set_get_element(modules, i);
	str = str + " *" + TAB + TAB + one_module + "\n";
    }  

    return str;

}

string
generate_adt_struct(list children, string tabs, int gen_comments)
     
{
    string str = "";
    adt_tp child;
    int i, num_children;
    note	comment_note;
    string	query, comment;
 
    if (children == NULL)
        return str;
 
    num_children = list_count(children);

    for (i = 0; i < num_children; i++)
    {
        child = list_get(children, i);

	if ( gen_comments != 0 )
	{
	    query = "note[DataStructureComment & obj_id=${child.element.id}]";
	    comment_note = find_by_query( query );
	    if ( comment_note != NULL )
		comment = "\t\t/* " + comment_note.desc + " */";
	    else
		comment = "";
	}
	else
	    comment = "";

        if (child.child_elements != NULL)
        {
            str = str + tabs + child.kind + " " +
		child.tag + comment + "\n" + tabs + 
		"{\n";
            str = str + generate_adt_struct(child.child_elements, tabs + TAB,
					    gen_comments);
            str = str + tabs + "} " + child.name ;
 
            if (child.array_size != "")
                str = str + child.array_size;

	    str = str + ";\n";

        }

        else if (child.kind != "enum")
        {
	    str =  str + tabs;

	    str =  str + child.type + " " + child.name;

            if (child.array_size != "")
                str =  str + child.array_size;

	    str =  str + ";" + comment + "\n";
        }

	// for enums
	else
	{
	    str = str + tabs + child.name;

            if ( num_children != 1 && i < (num_children - 1))
		str = str + "," + comment + "\n";
	    else
		str = str + comment + "\n";

	}

    }
    return str;
}    
 

// libcalls_list
//
// Prints list of library calls, one per line.  
 
string
libcalls_list(prop_tp props, module_tp the_module_struct)
{
    int		i;
    string	str = "";
    node	lib_module;
 
    for ( i = 0; i < list_count(the_module_struct.libcalls); i++)
    {            
        lib_module = list_get(the_module_struct.libcalls, i);
        str = str + TAB + "/*  " + to_c_identifier(lib_module.name) + "("; 

	str = str + get_arguments(the_module_struct.module, lib_module); 

	str = str + ");  *\/\n";
    }
     
    return str; 
 
}


// globals_list
//
// Prints list of globals, one per line.
 
string
globals_list(module_tp the_module_struct)
{
    int         i;
    string      str = "";
    string	glob;
 
    for ( i = 0; i < set_count(the_module_struct.globals); i++)
    {
        glob = set_get_element(the_module_struct.globals, i);
        str = str + TAB + "/*  " + glob + " *\/\n";
    }                    
     
    return str;
}

string
print_structure(adt_tp adt, int gen_comments)
{
    string	str = "";
    int		i;
    adt_tp	one_member;
    note	comment_note;
    string	query, comment;
 
    if (adt.kind == "enum")
    {
	for (i = 0; i < list_count (adt.child_elements) ; i++)
	{
	    one_member = list_get(adt.child_elements, i);

	    if ( gen_comments != 0 )
	    {
		query = "note[DataStructureComment & obj_id=${one_member.element.id}]";
		comment_note = find_by_query( query );
		if ( comment_note != NULL )
		    comment = "\t\t/* " + comment_note.desc + " */";
		else
		    comment = "";
	    }
	    else
		comment = "";

	    if (i > 0)
		str = str + ", " + comment + "\n";

	    str = str + TAB + one_member.name;

	}

	str = "enum " + adt.name + "\n{\n" + str + "\n};\n";
    }

    else
        str = adt.kind + " " + adt.name + "\n{\n" +
generate_adt_struct(adt.child_elements, TAB, gen_comments) + "};\n";
 
    return str;
 
}
 
string
print_single_line_type(adt_tp adt)
{    
    string str = "";

    if (adt.kind == "typedef")
	str = "typedef " + adt.type + " " + adt.name + adt.array_size + ";\n";

    else 
    {
	str = str + adt.kind + " " + adt.name + ";\n\n";
	print_message("\nWarning: Useless definition for '" + adt.kind + " " 
			+ adt.name + "' in file '" + adt.filename + "'.\n");
    }
 
    return str;
}
 
 
string
global_defs(set globals)
{
    string 	str = "";
    int 	i;
    string 	one_global;
 
    for (i = 0; i < set_count(globals); i++)
    {
        one_global = set_get_element(globals, i);
	str = str + one_global + ";\n";
    }

    if (str != "")
        str = START_GLOBALS_TAG + "\n" + str +
	    END_GLOBALS_TAG;
 
    return str;
}
 

// get include files for the file
 
string
include_list(prop_tp props, file_tp file)
{
    string one_include;
    string str = "";
    int i, j;
         
    if (file.type == "source" && set_count(file.file_components) !=0 )
	set_add(file.includes, remove_extension(file.name, props.file_extension)
		+ "_" + PROTOTYPE_FILENAME);
	
    for (i = 0; i < set_count(file.includes); i++)
    {
        one_include = set_get_element(file.includes, i);
        str = str + "#include " + handle_quotes(handle_extension(one_include, props.file_interface_extension)) + "\n";
    }
 
    if (str != "")
        str = START_INCLUDES_TAG + "\n" + str + 
	    END_INCLUDES_TAG;
 
    return str;
}


string
ifndef_statement(string filename)
{
    string	name = to_upper(to_c_identifier(filename));
    string	str;

    str = "#ifndef _" + name + "\n";

    str = str + "#define _" + name;

    return str;
}


string
endif_statement(string filename)
{
    string	name = to_upper(to_c_identifier(filename));
    string	str;

    str = "#endif        /* _" + name + " */\n";

    return str; 
}

// from here down is used for incremental printing of files

string
generate_c_file(prop_tp props, string file_contents, file_tp new_file)
{
    int	i;
    module_def_tp	one_module_def;
    adt_def_tp		one_adt_def;

    file_contents = generate_file_header(new_file.name, file_contents, new_file);

    file_contents = generate_protos(file_contents, new_file);

    file_contents = generate_include_list(props, file_contents, new_file);

    file_contents = generate_global_defs(file_contents, new_file);

    file_contents = generate_adt_defs(file_contents, new_file.data_elements);

    for (i = 0; i < list_count(new_file.module_elements); i++)
    {
        one_module_def = list_get(new_file.module_elements, i);
	file_contents = generate_module_def(file_contents, one_module_def);
    }

    return file_contents;
}

string
generate_file_header(string fname, string file_contents, file_tp new_file)
{
    int	pos;
    int	strlen = string_length(file_contents);
    string 	tag, contents = "";

    pos = string_find(file_contents, 0, START_FILE_HEADER_TAG);

    if (pos == strlen)
    {
	if (new_file.type == "source")
	    return "\n" + generate_c_impl_file_header(fname,
		    new_file.file_components) + "\n" + file_contents;

	else if (new_file.type == "header")
	    return "\n" + generate_c_header_file_header(fname,
		    new_file.file_components) + "\n" + file_contents;
    }

    if (new_file.type == "source")
	tag = " * Functions:\n";  

    else if (new_file.type == "header")
	tag = " * Components:\n";

    pos = string_find(file_contents, pos, tag);

    contents = string_extract(file_contents, 0, pos) +
" *" + TAB + "modified on " + time_to_string(time_now(), NULL) + " for \n" +
" *" + TAB + user() + "@" + hostname() + " from system " + current_system() + "\n" +
" * \n" +
tag + print_header_object_list(new_file.file_components) +
" * \n" +
END_HEADER_TAG;

    pos = string_find(file_contents, pos, END_HEADER_TAG);

    contents = contents + string_extract(file_contents, pos + END_HEADER_LENGTH, strlen - (pos + END_HEADER_LENGTH));

    return contents;
}

string
generate_include_list(prop_tp props, string file_contents, file_tp new_file)
{
    int	pos, pos2;
    int	strlen = string_length(file_contents);
    string	contents = "";

    pos = string_find(file_contents, 0, START_INCLUDES_TAG);

    if (pos == strlen)
    {
	pos = string_find(file_contents, 0, END_HEADER_TAG);

	contents = string_extract(file_contents, 0, pos + END_HEADER_LENGTH + 1)
		    + "\n" + include_list(props, new_file) + "\n\n" +
		    string_extract(file_contents, pos + END_HEADER_LENGTH + 1, strlen - (pos + END_HEADER_LENGTH + 1));

        return contents;
    }

    contents = string_extract(file_contents, 0, pos) +
		include_list(props, new_file);

    pos2 = string_find(file_contents, pos, END_INCLUDES_TAG);

    if (pos2 != strlen)
	contents = contents + string_extract(file_contents, pos2 + END_INCLUDES_LENGTH, strlen - (pos2 + END_INCLUDES_LENGTH));

    // in case the end tag is missing
    else contents = contents + "\n" + string_extract(file_contents, pos, strlen - pos);

    return contents;

}


string
generate_protos(string file_contents, file_tp new_file)
{
    int 	i, pos, pos2;
    int 	strlen = string_length(file_contents);
    int 	length = END_INCLUDES_LENGTH;
    string      one_proto, contents = "", pstr = "";
 
    for (i = 0; i < list_count(new_file.protos); i++)
    {
	one_proto = list_get(new_file.protos, i);
	pstr = pstr + one_proto;
    }

    if (pstr == "")
	return file_contents;

    pstr = START_PROTOS_TAG + "\n" + 
	    pstr + 
	    END_PROTOS_TAG + "\n";

    pos = string_find(file_contents, 0, START_PROTOS_TAG);
 
    if (pos == strlen)
    {
        pos = string_find(file_contents, 0, END_INCLUDES_TAG);
 
        if (pos == strlen)
        {
            pos = string_find(file_contents, 0, END_HEADER_TAG);
            length = END_HEADER_LENGTH;
        }

        contents = string_extract(file_contents, 0, pos + length + 1) 
		+ pstr + "\n"
		+ string_extract(file_contents, pos + length + 1, strlen - (pos + length + 1));

        return contents;
    }

    contents = string_extract(file_contents, 0, pos) + pstr;

    pos2 = string_find(file_contents, pos, END_PROTOS_TAG);
 
    if (pos2 != strlen)
        contents = contents + string_extract(file_contents, pos2 + END_PROTOS_LENGTH, strlen - (pos2 + END_PROTOS_LENGTH));
 
    // in case the end tag is missing
    else contents = contents + "\n" + string_extract(file_contents, pos, strlen - pos);
 
    return contents;
 
}


string
generate_global_defs(string file_contents, file_tp new_file)
{
    int	pos, pos2;
    int	strlen = string_length(file_contents);
    int length = END_PROTOS_LENGTH;
    string      contents = "";

    pos = string_find(file_contents, 0, START_GLOBALS_TAG);

    if (pos == strlen)
    {
        pos = string_find(file_contents, 0, END_PROTOS_TAG);

	if (pos == strlen)
	{
	    pos = string_find(file_contents, 0, END_INCLUDES_TAG);

	    if (pos == strlen)
	    {
		pos = string_find(file_contents, 0, END_HEADER_TAG);
		length = END_HEADER_LENGTH;
	    }
	    else
		length = END_INCLUDES_LENGTH;
	}

	contents = string_extract(file_contents, 0, pos + length + 1) +
		"\n" + global_defs(new_file.globals) +
		string_extract(file_contents, pos + length + 1, strlen - (pos + length + 1));
 
        return contents;
    }
 
    contents = string_extract(file_contents, 0, pos) +
		global_defs(new_file.globals);
 
    pos2 = string_find(file_contents, pos, END_GLOBALS_TAG);
 
    if (pos2 != strlen)
	contents = contents + string_extract(file_contents, pos2 + END_GLOBALS_LENGTH, strlen - (pos2 + END_GLOBALS_LENGTH));

    // in case the end tag is missing
    else contents = contents + "\n" + string_extract(file_contents, pos, strlen - pos);
 
    return contents;
 
}

string
generate_module_def(string file_contents, module_def_tp one_module_def)
{
    int		i, start, end, pos1, pos2;
    int		strlen = string_length(file_contents);
    string	contents = "";

    // check if the module with the same id already exists in the
    // current file
    start = string_find(file_contents, 0, " * (" + one_module_def.id + ")");

    // if the module already exists replace it with the new definition
    if (start != strlen)
    {
	end = string_find(file_contents, start, END_MODULE_TAG);

	contents = string_extract(file_contents, 0, start - START_FUNCTION_HEADER_LENGTH - 1);

	contents = contents + one_module_def.comment;

	// extract all data between the comment and function header
	pos1 = string_find(file_contents, start, END_HEADER_TAG);
	pos1 = pos1 + END_HEADER_LENGTH;
	pos2 = string_find(file_contents, pos1, START_MODULE_TAG);
	contents = contents + string_extract(file_contents, pos1, pos2 - pos1);

	contents = contents + one_module_def.header;

	// extract all data between the function header and body
	pos1 = string_find(file_contents, pos2, "{");
	pos1 = pos1 + 1;
	pos2 = string_find(file_contents, pos1, START_CODE_TAG);
	contents = contents + string_extract(file_contents, pos1, pos2 - pos1);

	contents = contents + one_module_def.body;

	// extract all data between the function body and end
	pos1 = string_find(file_contents, pos2, END_CODE_TAG);
	pos1 = pos1 + END_CODE_LENGTH;
	pos2 = string_find(file_contents, pos1, END_MODULE_TAG);
	contents = contents + string_extract(file_contents, pos1, pos2 - pos1);

	contents = contents + one_module_def.end;

	contents = contents + 
	string_extract(file_contents, end + END_MODULE_LENGTH, strlen - (end + END_MODULE_LENGTH));

    }
	// only 1 newline between the comment and the header lets RE pick
	// up the source code comments
    else contents = file_contents + one_module_def.comment + "\n" +
				    one_module_def.header + "\n\n" +
				    one_module_def.body + "\n\n" +
				    one_module_def.end + "\n";

    return contents;

}

string
generate_h_file(prop_tp props, string fname, string file_contents, file_tp new_file)
{
    file_contents = generate_file_header(fname, file_contents, new_file);
    file_contents = generate_include_list(props, file_contents, new_file);
    file_contents = generate_ifndef(fname, file_contents);

    file_contents = generate_adt_defs(file_contents, new_file.typedefs);
    file_contents = generate_adt_defs(file_contents, new_file.enums);
    file_contents = generate_adt_defs(file_contents, new_file.data_elements);

    file_contents = generate_endif(fname, file_contents);

    return file_contents;
}

string
generate_ifndef(string fname, string file_contents)
{
    int		pos;
    int         strlen = string_length(file_contents);
    string	str, contents = "";

    str = ifndef_statement(fname);
    pos = string_find(file_contents, 0, str);

    if (pos != strlen)
	return file_contents;

    pos = string_find(file_contents, 0, END_HEADER_TAG);
 
    contents = string_extract(file_contents, 0, pos + END_HEADER_LENGTH + 1)                    + "\n" + str + "\n" +
		string_extract(file_contents, pos + END_HEADER_LENGTH + 1, 
				strlen - (pos + END_HEADER_LENGTH + 1));

    return contents;
}

string   
generate_endif(string fname, string file_contents)
{
    int         pos;
    int         strlen = string_length(file_contents);
    string      str;
 
    str = endif_statement(fname);
    pos = string_find(file_contents, 0, str);
 
    if (pos != strlen)
        return file_contents;
 
    return file_contents + "\n" + str + "\n";
}

string
generate_adt_defs(string file_contents, list adt_def_list)
{
    adt_def_tp	one_adt_def;
    int		i;

    for (i = 0; i < list_count(adt_def_list); i++)
    {
        one_adt_def = list_get(adt_def_list, i);

	file_contents = generate_one_adt_def(file_contents, one_adt_def);
    }

    return file_contents;
}

string
generate_one_adt_def(string file_contents, adt_def_tp one_adt_def)
{
    int         start, end, pos1, pos2;
    int         strlen = string_length(file_contents);
    string      contents = "";

    // check if a data element with the same id already exists in the 
    // current file 
    start = string_find(file_contents, 0, " * (" + one_adt_def.id + ")"); 

    // if the data element already exists replace it with the new def
    if (start != strlen)
    {
	end = string_find(file_contents, start, END_DATA_DEFINITION_TAG); 
 
	contents = string_extract(file_contents, 0, start - START_DATA_HEADER_LENGTH - 1); 
  
	contents = contents + one_adt_def.comment;
 
	// extract all data between the comment and function header
	pos1 = string_find(file_contents, start, END_HEADER_TAG);
	pos1 = pos1 + END_HEADER_LENGTH;
	pos2 = string_find(file_contents, pos1, START_DATA_DEFINITION_TAG);
	contents = contents + string_extract(file_contents, pos1, pos2 - pos1);

	contents = contents + one_adt_def.def;

	contents = contents +
	string_extract(file_contents, end + END_DATA_LENGTH, strlen - (end + END_DATA_LENGTH));

    }
	// just 1 line between the comment and def, or RE cannot grab the
	// comments back again
    else contents = file_contents + "\n" +
			one_adt_def.comment + "\n" +
			one_adt_def.def + "\n";

    return contents;
}


string
generate_proto_file(string fname, string file_contents, file_tp new_file)
{
    int i;
    string       one_proto;

    file_contents = 
	"\n" + generate_proto_file_header(fname) + "\n\n";

    for (i = 0; i < list_count(new_file.module_elements); i++)
    {
        one_proto = list_get(new_file.module_elements, i);

	file_contents = file_contents + one_proto;
    }

    return file_contents;
}

