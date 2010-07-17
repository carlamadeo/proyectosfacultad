//
//      StP/UML
//      Confidential property of Aonix
//      Copyright (c) 1998
//      All rights reserved

//
//  Language independent file for UML code generation
//

#include "qrl/include/extract_classes_components.inc"
#include "qrl/include/error_message.inc"
#include "qrl/include/uml_data_model.inc"
#include "qrl/include/graph.inc"
#include "qrl/include/utilities.inc"


///////////////////////////////////////////////////////////////////////////////
// External Interface to Script
///////////////////////////////////////////////////////////////////////////////

const string    SCRIPT_DESCRIPTION = "StP/UML code generation";
script_help = "This script generates source code from the model.";

external string classes = "";
external_help = "Names of classes to use as input (separated by spaces).";

external string tables = "";
external_help = "Names of class tables to use as input (separated by spaces).";

external string class_diagrams = "";
external_help = "Names of class diagrams to use as input (separated by spaces).";

external string packages = "";
external_help = "Names of packages to use as input (separated by spaces).";

external string file_directory = "";
external_help = "File directory used for source code output (uses projdir/system/src_files if blank.)";

external string file_name = "";
external_help = "File name used for source code output (uses one file per class if blank.)";

external boolean file_per_package = False;
external_help = "True:  Generate one file per package (overrides file_name.)";

external boolean generate_interface_files = True;
external_help = "Specifies whether or not to generate interface files.";

external boolean generate_implementation_files = True;
external_help = "Specifies whether or not to generate implementation files.";

external boolean generate_operation_files = True;
external_help = "Specifies whether or not to generate operation files.";

external string interface_extension = ".";
external_help = "Source code interface file extension. (For C++: class declarations/definitions. For Ada: class package specification.)";

external string implementation_extension = ".";
external_help = "Source code implementation file extension. (For C++: member functions. For Ada: class package body.)";

external string operation_extension = ".";
external_help = "Source code operation file extension. (For C++: inline member functions. For Ada: subunits.)";

external string generated_file_name_case = "As Is";
external_help = "Determines the case of the automatically generated part of the output file names.  Possible values are AsIs, Lower, and Upper";

external boolean keep_old_packages = False;
external_help = "True:  Keep all packages that appear in the target file, even if they're not being updated (otherwise, they're removed.)";

external boolean generate_types_and_interfaces = True;
external_help = "True:  Generate code as appropriate for UML types and interfaces in the model.  You may not want to generate code for types or interfaces if other classes implement them on their own, or if they are used purely for analysis.";

external boolean generate_type_attributes = True;
external_help = "True:  If generating code for types and interfaces, this boolean determines whether to generate the attributes defined for types.  You may not want to generate the attributes defined for types if they are merely an abstract specification for the classes which actually implement the types.";

external boolean comment_classes = False;
external_help = "True:  Generate a comment from the class's Object and Requirement notes.";

external boolean comment_attributes = False;
external_help = "True:  Generate a comment from the attribute's Object and Requirement notes.";

external boolean comment_operations = False;
external_help = "True:  Generate a comment from the operation's Object and Requirement notes.";

external boolean format_comments = False;
external_help = "False: Format comments generated from a class's, attribute's or operation's Object and Requirement notes.";

external int format_comments_line_length = 80;
external_help = "80: Line length for formatting comments generated from a class's, attribute's or operation's Object and Requirement notes.";

external boolean incremental = True;
external_help = "True:  Incrementally update output files.";

external boolean use_component = False;
external_help = "True:  Use component information, if applicable, to determine output file names and directories.";

external string exclusion_predicate = "(UmlClassIsImported & value='True')";
external_help = "Exclude if class annotation predicate is true.";

///////////////////////////////////////////////////////////////////////////////
// Misc Constants
///////////////////////////////////////////////////////////////////////////////

const string  NL              = "\n";    // newline
const string  CODE_GEN_INDENT = "    ";  // 4 spaces

///////////////////////////////////////////////////////////////////////////////
// PackageCodeFile
///////////////////////////////////////////////////////////////////////////////

struct PackageCodeFile
{
    string name;	// file name portion of file stores 
    
    string interface_extension;	
						
    string implementation_extension;	
						
    string operation_extension;      // C++: for inline operations, e.g., .i
                                     // Ada: for subunits
    string full_path_interface_name;	
						
    string full_path_implementation_name; 
						   
    string full_path_operation_name;	
						

    string dir;         // file dir portion of file stores 
                        // CHANGE TO ALLOW FOR MULTIPLE TARGET DIRECTORIES 

    string full_path;	// full path file_name 
                        // CHANGE TO ALLOW FOR MULTIPLE TARGET DIRECTORIES
};

PackageCodeFile
PackageCodeFile(node package, PackageCodeDescriptor p)
{
    PackageCodeFile   self;

    self.name = NULL;
    self.dir = file_directory;

    if (use_component)
	PackageCodeFile__SetFileNameFromComponent(self, package);
    else 
      self.name = file_name;

    if (self.name == NULL || self.name == "")
    {
      self.name = language_file_name(package);
      self.name = file_name_case(generic_generate_file_name(self.name));
    }

    self.interface_extension = interface_extension;
    self.implementation_extension = implementation_extension;
    self.operation_extension = operation_extension;

    self.full_path = path_compose(self.dir, self.name);

    self.full_path_interface_name = self.full_path;
    self.full_path_implementation_name = self.full_path;
    self.full_path_operation_name = self.full_path;

    return self;
}

void
PackageCodeFile__SetFileNameFromComponent(PackageCodeFile self, node package)
{
    item            component_item;
    string          component_name;
    node            component;

    component_item =
      find_by_query("item[UmlPackageComponent & obj_id=${package.id}]");
    if (component_item == NULL || 
	(component_name = component_item.value) == "")
      return;

    component_name = to_oms_string(component_name);
    component = find_by_query
      ("node[UmlComponentSource & name='${component_name}' & node_refs]");

    if (component == NULL)
      return;

    self.name = generic_generate_file_name(component_name);
    self.name = file_name_case(self.name);

    self.interface_extension = interface_extension;
    self.implementation_extension = implementation_extension;
    self.operation_extension = operation_extension;
}

boolean
PackageCodeFile__IsAccessible(PackageCodeFile self)
{
    if ((!test_file_access(path_part(self.full_path_interface_name),
			   self.full_path_interface_name + self.interface_extension,
			   True)) ||
        (!test_file_access(path_part(self.full_path_implementation_name),
			   self.full_path_implementation_name + self.implementation_extension,
			   True)) ||
        (!test_file_access(path_part(self.full_path_operation_name), 
			   self.full_path_operation_name + self.operation_extension,
			   True)))
      return False;
    else
      return True;
}

//////////////////////////////////////////////////////////////////////////////
// PackageCodeDescriptor
///////////////////////////////////////////////////////////////////////////////

struct PackageCodeDescriptor
{
    node            oms_package;	// oms representation of package 
    UmlPackage      uml_package;	// uml representation of package 
	
    string          language_unit_name; // the name of the compilation unit

    PackageCodeFile file;

    list            interface_code;       // list<string>
    list            implementation_code;  // list<string>
    list            operation_code;       // list<string>
};

PackageCodeDescriptor
PackageCodeDescriptor(node package)
{
    PackageCodeDescriptor self;

    self.oms_package = package;

    // initialize uml_package separately so that we're sure the rest of the
    // descriptor was constructed because we don't want to do all the
    // querying in UmlPackage() for nothing 
    // (e.g. if the output files are not accessible, we abort)
    //
    self.uml_package = NULL;

    self.language_unit_name = language_make_unit_name(package);

    self.file = PackageCodeFile(package, self);

    self.interface_code = list_create("string",0);
    self.implementation_code = list_create("string",0);
    self.operation_code = list_create("string",0);

    return self;
}

void
PackageCodeDescriptor__FillFromRepository(PackageCodeDescriptor self)
{
    self.uml_package = UmlPackage(self.oms_package);
}

boolean
PackageCodeDescriptor__IsOk(PackageCodeDescriptor self)
{
    // preliminary checks before we start the real work

    // are output files ok?
    if (PackageCodeFile__IsAccessible(self.file) != True)
        return False;

    return True;
}

void
PackageCodeDescriptor__Compile(PackageCodeDescriptor self)
{
    language_compile(self);
}

void
PackageCodeDescriptors__Write(list packages)
{
    // incrementally write the package descriptors
    int             i, j, m, n;
    string          file, content_string;
    list            content, old_content;
    list            generated_files, generated_contents;

    PackageCodeDescriptor p;

    generated_files = list_create("string", 0);
    generated_contents = list_create("list", 0);

    if (generate_interface_files)
    {
	for (i = 0, n = list_count(packages); i < n; i = i + 1)
	{
	    p = list_get(packages, i);
	    file = p.file.full_path_interface_name + p.file.interface_extension;
	    append_contents(generated_files, generated_contents, file, p.interface_code);
	}
    }

    if (generate_implementation_files)
    {
	for (i = 0, n = list_count(packages); i < n; i = i + 1)
	{
	    p = list_get(packages, i);
	    file = p.file.full_path_implementation_name + p.file.implementation_extension;
	    append_contents(generated_files, generated_contents, file, p.implementation_code);
	}
    }

    if (generate_operation_files)
    {
	for (i = 0, n = list_count(packages); i < n; i = i + 1)
	{
	    p = list_get(packages, i);
	    file = p.file.full_path_operation_name + p.file.operation_extension;
	    append_contents(generated_files, generated_contents, file, p.operation_code);
	}
    }

    for (i = 0, n = list_count(generated_files); i < n; i = i + 1)
    {
	file = list_get(generated_files, i);
	content = list_get(generated_contents, i);



	if (list_count(content) != 0)
	{		
	  old_content = NULL;
	  if (read_file_access(file))
	    {
	      if (incremental)
		old_content = string_to_list(read_file(file), NL);
// C.L. 4/22/99 Removed MKS dependency
//	      system("mv -f " + file + " " + file + ".BAK");
              copy_file(file,file + ".BAK");
              delete_file(file);
	    }
	  
	  if (old_content == NULL)
	    {
	      ide_warning("Writing code to file:" + "\n" + "    " + file);

	      // only generate file header first time we create a file
	      language_generate_file_header(content);

	      // parsing and flattening does the blank line handling
	      content = flatten_parsed_code(parse_code(content));
	    }
	  else
	    {
	      ide_warning("Merging code to file:" + "\n" + "    " + file);
	      content = incremental_merge(content, old_content);
	    }
	  content_string = "";
	  for (j = 0, m = list_count(content); j < m; j = j + 1)
	    content_string = content_string + list_get(content, j) + NL;
	  write_file(file, content_string);
	}
    }
    
}

//////////////////////////////////////////////////////////////////////////////
// Main
///////////////////////////////////////////////////////////////////////////////

int
code_gen_main(string language)
{
    set class_set = set_create("node");
    set package_set = set_create("node");

    init_codegen(language);
    
    // extract classes from database
    if (!extract_classes_and_packages(class_set, package_set)) {
	return global_errors;
    }

    // compile packages
    global_errors = global_errors + compile_packages(language, package_set);

    // return
    error_message_and_count(SCRIPT_DESCRIPTION, global_errors);
    return global_errors;
}

int
compile_packages(string language, set package_set)
{
    int             i, n;
    node            package;
    list            PackageCodeDescriptors;
    PackageCodeDescriptor PackageCodeDescriptor;

    // set up and check options

    file_directory = string_strip(file_directory, "B", " ");
    file_name = string_strip(file_name, "B", " ");
    interface_extension = string_strip(interface_extension, "B", " ");
    implementation_extension = string_strip(implementation_extension, "B", " ");
    operation_extension = string_strip(operation_extension, "B", " ");
    exclusion_predicate = string_strip(exclusion_predicate, "B", " ");
    exclusion_predicate = string_strip(exclusion_predicate, "L", "||");

    // if there is no directory specification, use projdir/system/src_files
    if (file_directory == "")
        file_directory = path_compose(current_projdir(), current_system(),
				      "src_files");

    // if there is no file specification, then generate one file per package
    if (file_name == "")
        file_per_package = True;

    // one file per package overrides everything else
    if (file_per_package == True)
	file_name = "";
    
    language_check_options();

    // initialization 

    PackageCodeDescriptors = list_create("PackageCodeDescriptor", 0);

    ide_warning("Preparing for " + language + " code generation ..." + "\n");

    // compilation

    // preprocessing (e.g., filtering, sorting)
    package_set = language_compile_preprocessing(package_set);
    
    // preprocessed input packages
    for (i = 0, n = set_count(package_set); i < n; i = i + 1)
    {
        package = set_get_element(package_set, i);
        PackageCodeDescriptor = PackageCodeDescriptor(package);
        ide_warning(NL + "Processing compilation unit '" + 
		    PackageCodeDescriptor.language_unit_name + "'...");
        PackageCodeDescriptor__FillFromRepository (PackageCodeDescriptor);
        if (PackageCodeDescriptor__IsOk(PackageCodeDescriptor) == False)
            return 1;
        PackageCodeDescriptor__Compile(PackageCodeDescriptor);
        list_append(PackageCodeDescriptors, PackageCodeDescriptor);
    }

    // postprocessing (e.g., filtering, sorting) 
    PackageCodeDescriptors = language_compile_postprocessing(PackageCodeDescriptors);

    // write (incrementally)
    PackageCodeDescriptors__Write(PackageCodeDescriptors);

    return 0;
}

////////////////////////////////////////////////////////////////////////////////


// object can meaningfully be a class or package
boolean
is_object_excluded(node object)
{
    const string    OBJECT_EXCLUDED = "item[obj_id = ${object.id} && (${exclusion_predicate})]";

    if (object == NULL ||
	exclusion_predicate == "" ||
	selection_count(OBJECT_EXCLUDED) == 0)
	return False;

    return True;
}

boolean
is_object_imported(node object)
{
    const string    OBJECT_IMPORTED = "item[obj_id = ${object.id} && (UmlClassIsImported || UmlPackageIsImported) && value = 'True']";

    if (object == NULL ||
        selection_count(OBJECT_IMPORTED) == 0)
        return False;

    return True;
}


boolean
is_class_table_defined(node class)
{
    const string    CLASS_TABLE = "file[UmlClassTable && node_refs[node_id = ${class.id}]]";

    if (class == NULL ||
        selection_count(CLASS_TABLE) == 0)
        return False;

    return True;
}

node
get_attribute_by_name(node class, string name)
{
    const string    ATTRIBUTE_BY_NAME = "node[UmlAttribute && name = '${name}' && scope_node_id = ${class.id} && node_refs[file[UmlClassTable]]]";
    list            list;

    name = to_oms_string(name);
    if (class == NULL ||
        list_count((list = list_select(ATTRIBUTE_BY_NAME))) == 0)
        return NULL;

    return list_get(list, 0);
}

////////////////////////////////////////////////////////////////////////////////

string
break_up_comment_lines(string line, string prefix, string suffix)
{
    int i;
    int l = string_length(line);
    int max = format_comments_line_length-string_length(prefix)-string_length(suffix);

    if (max < 10) max = 10;
    
    i = string_find(line, 0, "\n");
    if (i != l)
        return (break_up_comment_lines(string_extract(line, 0, i),
                                       prefix, suffix) +
                break_up_comment_lines(string_extract(line, i+1, l-i-1),
                                       prefix, suffix));
    if (format_comments && (l > max))
    {
        i = max;
        while (i > 0 && string_extract(line, i, 1) != " ")
        {
            i = i-1;
        }

        if (i == 0)
        {
            return (prefix + string_extract(line, 0, max) + suffix + "\n" +
                    break_up_comment_lines(string_extract(line, max, l-max),
                                           prefix, suffix));
        }
        else
        {
            return (prefix + string_extract(line, 0, i) + suffix + "\n" +
                    break_up_comment_lines(string_extract(line, i+1, l-i-1),
                                           prefix, suffix));
        }   
    }
    else
        return (prefix + line + suffix + "\n");
}

string
generic_generate_object_comment(node object, string object_type, 
    string start_comment, string start_comment_line, string end_comment, 
    string end_comment_line)
{
    string          generated = "";
    note            note;
    item            item;

    // start_comment
    // start_comment_line  object_type : object.name end_comment_line
    // start_comment_line    generic annotations end_comment_line
    // end_comment

    // the generic annotation 
    note = find_by_query("note[GenericObject & obj_id=${object.id}]");

    if (note != NULL)
    {
        // description
        if (note.desc != "")
        {
            generated = generated + start_comment_line + "    Description:" + 
                end_comment_line + "\n";
            
            generated = generated + 
                break_up_comment_lines(note.desc,
                                       start_comment_line + "    ",
                                       end_comment_line);
        }

        // the generic object items (ignore StP ones)
        for_each_in_select("item[note_id=${note.id}]", item)
        {
            if (item.type == "ObjectName")
                continue;

            generate = generated +
                break_up_comment_lines(item.type + ": " + item.value,
                                       start_comment_line + "    ",
                                       end_comment_line);
        }

        // the requirements
        for_each_in_select("note[Requirement & obj_id=${object.id}]", note)
        {
            generated = generated + start_comment_line + "    Requirement " + 
                note.name + end_comment_line + "\n";

            if (note.desc != "")
            {
                generated = generated + start_comment_line + 
                    "      Description:" + end_comment_line + "\n";

                generated = generated +
                    break_up_comment_lines(note.desc,
                                           start_comment_line + "      ",
                                           end_comment_line);
            }

            for_each_in_select("item[note_id=${note.id}]", item)
            {
                if (item.type == "NoteKey")
                    continue;

                generate = generated +
                    break_up_comment_lines(item.type + ": " + item.value,
                                           start_comment_line + "      ",
                                           end_comment_line);
            }
        }
    }

    if (generated != "")
        return start_comment + start_comment_line + "  " + object_type + 
            ": " + object.name + end_comment_line + "\n" + generated + 
            end_comment;

    return "";
}


const string    BAD_FILENAME_TRANSLATE_TABLE = " !@#$%^&*()+=|\\{}[],./<>?~`;:'\"\n\t";
const string    GOOD_FILENAME_TRANSLATE_TABLE = string_repeat("_", string_length(BAD_FILENAME_TRANSLATE_TABLE));

string
generic_generate_file_name(string s)
{
    return string_translate(s, BAD_FILENAME_TRANSLATE_TABLE, GOOD_FILENAME_TRANSLATE_TABLE);
}

void
append_contents(list files, list contents, string file, list content)
{
    int  i;

    i = list_find(files, 0, file);
    
    if (i == list_count(files))
    {
	list_append(files, file);
	list_append(contents, list_create("string",0));
    }
    
    list_concatenate(list_get(contents, i), content);
}

string
file_name_case(string file_name)
{
    if (generated_file_name_case == "Lower")
	return to_lower(file_name);
    else if (generated_file_name_case == "Upper")
	return to_upper(file_name);
    else
	return file_name;
}

// BS 12/27/96 -- for parsing of typed qualifiers
// shared by smalltalk and c++ code generation

string
uml_name_type_syntax_2_type_space_name_syntax(string rumbaugh_names_types,
                                              string option, string default)
{
    //
    // parse a comma-separated list of "name:type" pairs into a
    // comma-separated list of "type name" pairs
    //
    string          types_names = "";
    string          pair;
    int             i, j, n;
    list            pairs, type_name, tokens;
    boolean         types = (option == "T" || option == "t" || option == "B" || option == "b");
    boolean         names = (option == "N" || option == "n" || option == "B" || option == "b");

    pairs = string_to_list(rumbaugh_names_types, ",");
    for (i = 0, n = list_count(pairs); i < n; i = i + 1)
    {
	pair = string_strip(list_get(pairs, i), "B", " ");
	type_name = string_to_list(pair, ":");
	pair = "";
	if (list_count(type_name) > 0)
	{
	    if (list_count(type_name) == 2)
	    {
		if (types)
		    pair = string_strip(list_get(type_name, 1), "B", " ");
		if (names)
		    pair = pair + " " + string_strip(list_get(type_name, 0), "B", " ");
	    } else if (list_count(type_name) == 1)
	    {
		tokens = string_to_list(list_get(type_name, 0), " ");
		for (j = 0; j < list_count(tokens); j = j + 1)
		{
		    if (j == list_count(tokens) - 1)
		    {
			if (names)
			    pair = pair + " " + string_strip(list_get(tokens, j), "B", " ");
		    } else
		    {
			if (types)
			    pair = pair + " " + string_strip(list_get(tokens, j), "B", " ");
		    }
		}
	    } else
		pair = string_strip(list_get(pairs, i), "B", " ");
	}
	pair = string_strip(pair, "B", " ");

        if (pair == "")
            pair = default;

	if (i == 0)
	    types_names = pair;
	else
	    types_names = types_names + ", " + pair;
    }
    return string_strip(types_names, "B", " ");
}

void
add_multiline(list l, string s)
{
  list_concatenate(l, string_to_list(s, NL));
}

void
add_multiline_with_indent(list l, string s)
{
  list_concatenate_with_indent(l, string_to_list(s, NL));
}

void
list_concatenate_with_indent (list l1, list l2)
{
  int i,n;
  for (i = 0, n = list_count(l2); i < n; i = i + 1)
    list_append(l1, INDENT + list_get(l2, i));
}

string
trim (string s)
{
  return string_strip(s, "B", " \t");
}

/// incremental stuff

struct tagged_block
{
  string begin_tag;
  string end_tag;
  list   lines;  // list<string>  
};

struct scoped_block
{
  string begin_tag;
  string end_tag;
  list   lines;  // list<all>
};

list
parse_code(list code)
{
  list   parsed_code;
  string line;
  int    i,n;

  i = 0;
  n = list_count(code);
  parsed_code = list_create("all", 0);

  while (i < n)
    {
      line = list_get(code, i);
      
      if (is_begin_block(line))
	{
	  i = parse_tagged_block(i, code, parsed_code);
	}
      else if (is_begin_scope(line))
	{
	  i = parse_scoped_block(i, code, parsed_code);
	}
      else
	{
	  list_append(parsed_code, line);
	  i = i + 1;
	}
    }
  
  return parsed_code;
}

boolean
is_begin_block(string line)
{
  return (string_find(line, 0, COMMENT_PREFIX + "  begin block") != string_length(line));
}

boolean
is_end_block(string line)
{
  return (string_find(line, 0, COMMENT_PREFIX + "  end block") != string_length(line));
}


int
parse_tagged_block(int i, list code, list parsed_code)
{
  tagged_block b;
  string line;

  b.begin_tag = list_get(code, i);
  b.lines = list_create("string",0);

  i = i + 1;
  while (!is_end_block(line = list_get(code, i)))
    {
      list_append(b.lines, line);
      i = i + 1;
    }
  b.end_tag = line;
  list_append(parsed_code, b);
  return (i + 1);
}

boolean
is_begin_scope(string line)
{
  return (string_find(line, 0, COMMENT_PREFIX + "  begin scope") != string_length(line));
}

boolean
is_end_scope(string line)
{
  return (string_find(line, 0, COMMENT_PREFIX + "  end scope") != string_length(line));
}

int
parse_scoped_block(int i, list code, list parsed_code)
{
  scoped_block b;
  string line;

  b.begin_tag = list_get(code, i);
  b.lines = list_create("all", 0);

  i = i + 1;
  while (!is_end_scope(line = list_get(code, i)))
    {
      if (is_begin_block(line))
	{
	  i = parse_tagged_block(i, code, b.lines);
	}
      else if (is_begin_scope(line))
	{
	  i = parse_scoped_block(i, code, b.lines);
	}
      else
	{
	  list_append(b.lines, line);
	  i = i + 1;
	}
    }
  b.end_tag = line;
  list_append(parsed_code, b);
  return (i + 1);
}

void
pre_blank_line(list flattened_code)
{
  int i = list_count(flattened_code);
  if (i == 0 ||
      trim(list_get(flattened_code, i-1)) != "")
    list_append(flattened_code, "");
}

void
post_blank_line(list flattened_code, list parsed_code, int i)
{
  i = i + 1;

  if (i == list_count(parsed_code) ||
      !(list_get_type(parsed_code, i) == "string" &&
	trim(list_get(parsed_code, i)) == ""))
    list_append(flattened_code, "");
}

list
flatten_parsed_code(list code)
{
  list flattened_code;
  string type;
  int  i,n;
  tagged_block tb;
  scoped_block sb;
  
  flattened_code = list_create("string",0);

  for (i = 0, n = list_count(code); i < n; i = i + 1)
    {
      type = list_get_type(code, i);
      if (type == "tagged_block")
	{
	  tb = list_get(code, i);
	  pre_blank_line(flattened_code);
	  list_append(flattened_code, tb.begin_tag);
	  list_concatenate(flattened_code, tb.lines);
	  list_append(flattened_code, tb.end_tag);
	  post_blank_line(flattened_code, code, i);
	}
      else if (type == "scoped_block")
	{
	  sb = list_get(code, i);
	  pre_blank_line(flattened_code);
	  list_append(flattened_code, sb.begin_tag);
	  list_concatenate(flattened_code,
			   flatten_parsed_code(sb.lines));
	  list_append(flattened_code, sb.end_tag);
	  post_blank_line(flattened_code, code, i);
	}
      else
	{
	  list_append(flattened_code, list_get(code, i));
	}
    }
  return flattened_code;
}
	
void
merge_parsed_code(list merged, list new, list old)
{
  string type;

  while (list_count(old) > 0)
    {
      type = list_get_type(old, 0);
      if (type == "scoped_block")
	merge_scoped_block (merged, new, old);
      else if (type == "tagged_block")
	merge_tagged_block (merged, new, old);
      else if (type == "string")
	merge_simple_string (merged, new, old);
      else
	ide_error ("Inappropriate element type in parsed_code.");
    }

  // now add any new code that wasn't caught in the above merge.
  add_new_code(merged, new, old, list_count(new));
}

int
find_simple_string(list code, string line)
{
  int   i, n;
  string trimmed_line;

  trimmed_line = trim(line);

  for (i = 0, n = list_count(code); i < n; i = i + 1)
    {
      if (list_get_type(code, i) == "string" &&
	  trim(list_get(code, i)) == trimmed_line)
	  break;
    }
  return i;
}

int
find_tagged_block(list code, tagged_block tb)
{
  int   i, n;
  tagged_block b;
  string trimmed_tag;

  trimmed_tag = trim(tb.begin_tag);

  for (i = 0, n = list_count(code); i < n; i = i + 1)
    {
      if (list_get_type(code, i) == "tagged_block")
	{
	  b = list_get(code, i);
	  if (trim(b.begin_tag) == trimmed_tag)
	    break;
	}
    }
  return i;
}

int
find_scoped_block(list code, scoped_block sb)
{
  int   i, n;
  scoped_block b;
  string trimmed_tag;

  trimmed_tag = trim(sb.begin_tag);

  for (i = 0, n = list_count(code); i < n; i = i + 1)
    {
      if (list_get_type(code, i) == "scoped_block")
	{
	  b = list_get(code, i);
	  if (trim(b.begin_tag) == trimmed_tag)
	    break;
	}
    }
  return i;
}

void
merge_simple_string(list merged, list new, list old)
{
  string old_line;
  int    found;

  old_line = list_get(old, 0);
  list_delete(old, 0);

  found = find_simple_string(new, old_line);

  if (found != list_count(new))
    add_new_code(merged, new, old, found + 1);
  else
    list_append(merged, old_line);
}

void
merge_tagged_block(list merged, list new, list old)
{
  tagged_block  old_block;
  int    found;

  old_block = list_get(old, 0);
  list_delete(old, 0);

  found = find_tagged_block(new, old_block);

  if (found != list_count(new))
    add_new_code(merged, new, old, found + 1);
}

void
merge_scoped_block(list merged, list new, list old)
{
  scoped_block  old_block, new_block, merged_block;
  int    found;

  old_block = list_get(old, 0);
  list_delete(old, 0);

  found = find_scoped_block(new, old_block);

  if (found != list_count(new))
    {
      new_block = list_get(new, found);
      list_delete(new, found);

      add_new_code(merged, new, old, found);

      merged_block.begin_tag = new_block.begin_tag;
      merged_block.end_tag = new_block.end_tag;
      merged_block.lines = list_create("all", 0);

      merge_parsed_code(merged_block.lines,
			new_block.lines,
			old_block.lines);

      list_append(merged, merged_block);
    }       
  else if (keep_old_packages)
    list_append(merged, old_block);
}

void
add_new_code(list merged, list new, list old, int n)
{
  int i = 0;
  int old_count = list_count(old);
  string type;
  
  while (i < n)
    {
      type = list_get_type(new, i);
      if (
	  (type == "scoped_block"
	   && (find_scoped_block(old, list_get(new, i)) == old_count)) ||

	  (type == "tagged_block"
	   && (find_tagged_block(old, list_get(new, i)) == old_count)) ||
 
	  (type == "string"
	   && (find_simple_string(old, list_get(new, i)) == old_count))
	  )
	{
	  list_append(merged, list_get(new, i));
	  list_delete(new, i);
	  n = n - 1;
	}
      else
	i = i + 1;
    }
}
  
list
incremental_merge(list content, list old_content)
{
  list  flattened_code;

  list  old_code, new_code, merged_code;

  new_code = parse_code(content);
  old_code = parse_code(old_content);
  merged_code = list_create("all", 0);

  //ide_warning("new_code is..." + NL + new_code);  

  merge_parsed_code(merged_code, new_code, old_code);
  //ide_warning("merged_code is..." + NL + merged_code);  

  flattened_code = flatten_parsed_code(merged_code);
  //  ide_warning("flattened_code is..." + NL + flattened_code);  

  return flattened_code;
}

