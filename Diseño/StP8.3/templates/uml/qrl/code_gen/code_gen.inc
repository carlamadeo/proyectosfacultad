//
//      StP/UML
//      Confidential property of Aonix
//      Copyright (c) 1997
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

external string categories = "";
external_help = "Names of class categories to use as input (separated by spaces).";

external string omtsubsystems = "";
external_help = "Names of OMT subsystems to use as input (separated by spaces).";

external string modules = "";
external_help = "Names of modules to use as input (separated by spaces).";

external string module_diagrams = "";
external_help = "Names of module diagrams to use as input (separated by spaces).";
external string subsystems = "";
external_help = "Names of module subsystems to use as input (separated by spaces).";

external string packages = "";
external_help = "Names of packages to use as input  (separated by spaces).";

external string file_directory = "";
external_help = "File directory used for source code output (uses projdir/system/src_files if blank.)";

external string file_name = "";
external_help = "File name used for source code output (uses one file per class if blank.)";

external boolean file_per_class = False;
external_help = "True:  Generate one file per class (overrides file_name.)";

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

external boolean keep_old_classes = False;
external_help = "True:  Keep all classes that appear in the target file, even if they're not being updated (otherwise, they're removed.)";

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

external boolean use_module = False;
external_help = "True:  Use component information, if applicable, to determine output file names and directories.";

external string exclusion_predicate = "(UmlClassIsImported & value='True')";
external_help = "Exclude if class annotation predicate is true.";

///////////////////////////////////////////////////////////////////////////////
// Misc Constants
///////////////////////////////////////////////////////////////////////////////

const string  NL              = "\n";    // newline
const string  CODE_GEN_INDENT = "    ";  // 4 spaces

///////////////////////////////////////////////////////////////////////////////
// ClassCodeDefinition
///////////////////////////////////////////////////////////////////////////////

struct ClassCodeDefinition
{
    //
    // The class's language name and fully qualified name, definition header, 
    // and definition represented as strings using the syntax of the 
    // implementation language.  
    // The class's enclosing scope represented as a list of whatever is 
    // deemed appropriate for that language.
    //
    string  language_base_name;
    list    language_scope_list;   // list of enclosing scopes, outermost first
    string  language_qualified_name;
    string  definition_header;
    string  definition;
};

ClassCodeDefinition
ClassCodeDefinition(node class)
{
    ClassCodeDefinition self;

    self.language_base_name = language_make_base_name(class);

    // The enclosing scopes are separated into a list and then put back
    // together into a string to allow each language to process the
    // individual scope names as needed.
    self.language_scope_list = language_make_scope_list(class);
    self.language_qualified_name = 
	language_build_qualified_name(self.language_scope_list, 
				     self.language_base_name);
    self.definition_header = "";
    self.definition = "";

    return self;
}

void
ClassCodeDefinition__Compile(ClassCodeDefinition self, ClassCodeDescriptor c)
{
    ClassCodeDefinition__SetIdentifier(self, c);
    ClassCodeDefinition__SetDefinitionHeader(self, c);
    ClassCodeDefinition__SetDefinition(self, c);

    return;
}

///////////////////////////////////////////////////////////////////////////////
// ClassCodeOperations
///////////////////////////////////////////////////////////////////////////////

struct ClassCodeOperations
{
    list            headers;    
    list            bodies;     
    list            footers;    
    list            stubs;      
    list            file_extensions;  
    list            ids;               // code gen id 
    list            written;           // operation was updated
    list            stubs_written;     // operation stub was updated 
};

ClassCodeOperations
ClassCodeOperations(node class)
{
    ClassCodeOperations self;

    // initialize parallel lists 
    self.headers = list_create("string", 0);
    self.bodies = list_create("string", 0);
    self.footers = list_create("string", 0);
    self.stubs = list_create("string", 0);
    self.file_extensions = list_create("string", 0);
    self.ids = list_create("int", 0);
    self.written = list_create("boolean", 0);
    self.stubs_written = list_create("boolean", 0);

    return self;
}

void
ClassCodeOperations__Compile(ClassCodeOperations self, ClassCodeDescriptor c)
{
    // just for completeness -- nothing to do

    return;
}

void
ClassCodeOperations__Append(ClassCodeOperations self, int id, string header, string body, string footer, string stub, string file_extension)
{
    list_append(self.ids, id);
    list_append(self.headers, header);
    list_append(self.bodies, body);
    list_append(self.footers, footer);
    list_append(self.stubs, stub);
    list_append(self.file_extensions, file_extension);
    list_append(self.written, False);
    list_append(self.stubs_written, False);

    return;
}

///////////////////////////////////////////////////////////////////////////////
// ClassCodeFile
///////////////////////////////////////////////////////////////////////////////

struct ClassCodeFile
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

ClassCodeFile
ClassCodeFile(node class, ClassCodeDefinition def)
{
    ClassCodeFile   self;
    string          class_file_base_name;

    self.name = NULL;

    if (use_module)
    {
	ClassCodeFile__SetFileNameDirectoryFromModuleSubsystem(self, class);
	if (self.name != NULL)
	    return self;
    }

    self.name = file_name;
    self.dir = file_directory;
    if (self.name == "")
    {
	class_file_base_name = def.language_qualified_name;

	self.name = generic_generate_file_name(
	    language_file_name(class_file_base_name));
	
	self.name = file_name_case(self.name);
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

graph           ClassCodeFile__SS = NULL;

void
ClassCodeFile__SetFileNameDirectoryFromModuleSubsystem(ClassCodeFile self, 
						       node class)
{
    item            moduleitem;
    string          modulename, subsystem_dirs;
    node            subsystem, module, modulespec, modulebody;

    //!! This routine constructs the file name and directory for a class if 
    //!! it has a module assigned to it, otherwise it just returns.
    //!! This has not been converted to UML yet, so for now just return.

    return;

    if (ClassCodeFile__SS == NULL) {
	ClassCodeFile__SS = UmlClassGraph(NULL, list_select("node[Subsystem & node_refs]"));
	ClassCodeFile__SS = UmlClassGraph(ClassCodeFile__SS, list_select("link[ComponentOf & from_node[Subsystem] & to_node[Subsystem] & link_refs]"));
    }

    moduleitem = find_by_query("item[ClassModule & obj_id=${class.id}]");
    if (moduleitem == NULL || (modulename = moduleitem.value) == "")
	return;

    modulename = to_oms_string(modulename);
    module = find_by_query("node[ModuleSpecBody & name='${modulename}' & node_refs]");
    if (module == NULL)
    {
	modulespec = find_by_query("node[ModuleSpec & name='${modulename}' & node_refs]");
	modulebody = find_by_query("node[ModuleBody & name='${modulename}' & node_refs]");
    } else
    {
	modulespec = module;
	modulebody = module;
    }

    if (modulespec == NULL && modulebody == NULL)
	return;

    self.name = generic_generate_file_name(modulename);
    self.name = file_name_case(self.name);

    self.interface_extension = interface_extension;
    self.implementation_extension = implementation_extension;
    self.operation_extension = operation_extension;

    if (modulespec == NULL)
    {
	self.interface_extension = implementation_extension;
	modulespec = modulebody;
    } else if (modulebody == NULL)
    {
	self.implementation_extension = interface_extension;
	self.operation_extension = interface_extension;
	modulebody = modulespec;
    }

    self.full_path_interface_name = file_directory;
    subsystem = find_by_query("node[Subsystem & in_links[ComponentOf & from_node_id=${modulespec.id} & link_refs] & node_refs]");
    if (subsystem != NULL) {
	subsystem_dirs = path_compose(graph_node_indirect_predecessors(
	    ClassCodeFile__SS, subsystem.name));
	self.full_path_interface_name = 
	path_compose(self.full_path_interface_name,
		     subsystem_dirs, subsystem.name);
    }
    self.full_path_interface_name = 
	path_compose(self.full_path_interface_name, self.name);

    self.full_path_implementation_name = file_directory;
    subsystem = find_by_query("node[Subsystem & in_links[ComponentOf & from_node_id=${modulebody.id} & link_refs] & node_refs]");
    if (subsystem != NULL) {
	subsystem_dirs = path_compose(graph_node_indirect_predecessors(
	    ClassCodeFile__SS, subsystem.name));
	self.full_path_implementation_name = 
	    path_compose(self.full_path_implementation_name,
			 subsystem_dirs, subsystem.name);
    }
    self.full_path_implementation_name = 
	path_compose(self.full_path_implementation_name, self.name);

    if (self.operation_extension == self.implementation_extension)
	self.full_path_operation_name = self.full_path_implementation_name;
    else 
	self.full_path_operation_name = self.full_path_interface_name;

    return;
}

boolean
ClassCodeFile__IsAccessible(ClassCodeFile self)
{
    if ((!test_file_access(path_part(self.full_path_interface_name), self.full_path_interface_name + self.interface_extension, True)) ||
        (!test_file_access(path_part(self.full_path_implementation_name), self.full_path_implementation_name + self.implementation_extension, True)) ||
        (!test_file_access(path_part(self.full_path_operation_name), self.full_path_operation_name + self.operation_extension, True)))
        return False;

    return True;
}

void
ClassCodeFile__Compile(ClassCodeFile self, ClassCodeDescriptor c)
{
    if (!use_module && file_per_class)
	ClassCodeFile__AdjustName(self, c);
    
    return;
}

//////////////////////////////////////////////////////////////////////////////
// ClassCodeVariant
///////////////////////////////////////////////////////////////////////////////

struct ClassCodeVariant
{
    //
    //  used for more than 1 language
    //
    list     generalizations;
    list     associations;
    set      dependencies;

    //
    //  used for C++
    //
    boolean  is_template;
    string   template_prefix;
    string   template_parameters;
 
    string   constructor_initialization_list;  // attribute default values
    string   static_attribute_initializers;    // class attribute default values
    list     instantiated_class_typedefs;
 
    //
    //  used for Ada_95
    //
    string   class_type_privacy;
    string   private_declarations;
    string   implementation_declarations;
    boolean  generate_access_type;
 
    boolean  need_set_instantiation;
    boolean  need_list_instantiation;
    list     map_instantiations;
    list     map_to_set_instantiations;
    list     map_to_list_instantiations;
 
    list     tuple_instantiations;
    list     set_of_tuple_instantiations;
    list     list_of_tuple_instantiations;
    list     map_to_tuple_instantiations;
    list     map_to_set_of_tuple_instantiations;
    list     map_to_list_of_tuple_instantiations;
 
    //  The 'written' lists are parallel to the 'instantiations' lists 
    boolean  set_written;
    boolean  list_written;
    list     map_written;
    list     map_to_set_written;
    list     map_to_list_written;
 	   
    list     tuple_written;
    list     set_of_tuple_written;
    list     list_of_tuple_written;
    list     map_to_tuple_written;
    list     map_to_set_of_tuple_written;
    list     map_to_list_of_tuple_written;
    
    string   package_name;
    string   package_kind;
    string   private_operations;
    string   private_class_operations;
    string   implementation_operations;
    string   implementation_class_operations;

     //  Used for TOOL (forte)
    boolean  is_userwindow;
    boolean  is_interface;
};

//////////////////////////////////////////////////////////////////////////////
// ClassCodeDescriptor
///////////////////////////////////////////////////////////////////////////////

struct ClassCodeDescriptor
{
    node            oms_class;	// oms representation of class 
    UmlClass        uml_class;	// uml representation of class 

    ClassCodeDefinition def;
    ClassCodeOperations ops;
    ClassCodeFile   file;
    ClassCodeVariant var;

    boolean         written;
    boolean         implementation_written;
    boolean         attribute_implementation_written;
    boolean         derivatives_written;

    int             id;		// code gen id for class
};

ClassCodeDescriptor
ClassCodeDescriptor(node class)
{
    ClassCodeDescriptor self;

    self.oms_class = class;
    self.written = False;
    self.implementation_written = False;
    self.attribute_implementation_written = False;
    self.derivatives_written = False;
    self.id = 0;

    // propogate constructors  (order is significant) 
    self.var  = ClassCodeVariant(class);
    self.def  = ClassCodeDefinition(class);
    self.ops  = ClassCodeOperations(class);
    self.file = ClassCodeFile(class, self.def);

    //
    // initialize uml_class separately so that we're sure the rest of the
    // descriptor was constructed because we don't want to do all the
    // querying in UmlClass() for nothing 
    // (e.g. if the output files are not accessible, we abort)
    //
    self.uml_class = NULL;

    return self;
}

void
ClassCodeDescriptor__FillFromRepository(ClassCodeDescriptor self)
{
    self.uml_class = UmlClass(self.oms_class);
}

boolean
ClassCodeDescriptor__IsOk(ClassCodeDescriptor self)
{
    // preliminary checks before we start the real work

    // are output files ok?
    if (ClassCodeFile__IsAccessible(self.file) != True)
        return False;

    return True;
}

void
ClassCodeDescriptor__Compile(ClassCodeDescriptor self)
{
    // propogate compile (order is signifigant)

    string id = language_code_gen_id(self.uml_class);
    if (id != "")
	self.id = to_int(id);

    ClassCodeVariant__Compile(self.var, self);
    ClassCodeFile__Compile(self.file, self);
    ClassCodeDefinition__Compile(self.def, self);
    ClassCodeOperations__Compile(self.ops, self);

    return;
}

void
ClassCodeDescriptors__Write(list classes, string language)
{
    // incrementally write the class descriptors
    int             i, n;
    string          file;
    list            interface_files, implementation_files;

    // initialize "files we wrote" lists
    interface_files = list_create("string", 0);
    implementation_files = list_create("string", 0);

    //
    // write interfaces incrementally...
    //
    if (generate_interface_files)
	write_interfaces(classes, interface_files);

    //
    // write implementations incrementally...
    //
    // the flag 'generate_implementation_files' cannot be used here,
    // it must be used inside 'write_implementations', because of the
    // additional flag 'generate_operation_files'
    //
    write_implementations(classes, interface_files, implementation_files);

    // write interface file footer 
    for (i = 0, n = list_count(interface_files); i < n; i = i + 1)
    {
        file = list_get(interface_files, i);
        file_append(interface_files, file, 
            generate_tagged_interface_file_footer(file));
    }

    //EH -- A little confusing for java
    if (language != "Java")
    {

    // show the names of all the files we wrote 
    ide_warning("");  // blank line
    for (i = 0, n = list_count(interface_files); i < n; i = i + 1)
	ide_warning("Wrote interfaces to file:" + "\n" + "    " +
		      list_get(interface_files, i));
    for (i = 0, n = list_count(implementation_files); i < n; i = i + 1)
	ide_warning("Wrote implementations to file:" + "\n" + "    " +
		      list_get(implementation_files, i));
    }

    return;
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
    
    // compile classes
    global_errors = global_errors + compile_classes(language, class_set);

    // return
    error_message_and_count(SCRIPT_DESCRIPTION, global_errors);
    return global_errors;
}

int
compile_classes(string language, set class_set)
{
    int             i, n;
    node            class;
    list            ClassCodeDescriptors;
    ClassCodeDescriptor ClassCodeDescriptor;

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

    // if there is no file specification, then generate one file per class
    if (file_name == "")
        file_per_class = True;

    // one file per class overrides everything else
    if (file_per_class == True)
        file_name = "";

    language_check_options();

    // initialization 

    ClassCodeDescriptors = list_create("ClassCodeDescriptor", 0);

    ide_warning("Preparing for " + language + " code generation ..." + "\n");

    // compilation

    // preprocessing (e.g., filtering, sorting)
    class_set = language_compile_preprocessing(class_set);
    
    // preprocessed input classes
    for (i = 0, n = set_count(class_set); i < n; i = i + 1)
    {
        class = set_get_element(class_set, i);
        ClassCodeDescriptor = ClassCodeDescriptor(class);
        ide_warning("Processing class '" + ClassCodeDescriptor.def.language_qualified_name + "'...");
        ClassCodeDescriptor__FillFromRepository (ClassCodeDescriptor);
        if (ClassCodeDescriptor__IsOk(ClassCodeDescriptor) == False)
            return 1;
        ClassCodeDescriptor__Compile(ClassCodeDescriptor);
        list_append(ClassCodeDescriptors, ClassCodeDescriptor);
    }

    // postprocessing (e.g., filtering, sorting) 
    ClassCodeDescriptors = language_compile_postprocessing(ClassCodeDescriptors);

    //EH
    if (language == "Java")
    {
        save_java_backups(ClassCodeDescriptors, True);  
        arrange_java_inner_classes(ClassCodeDescriptors, True);
        arrange_java_method_bodies(ClassCodeDescriptors, True);
    }

    // write (incrementally)
    ClassCodeDescriptors__Write(ClassCodeDescriptors, language);

    //EH
    if (language == "Java")
    {
        arrange_java_method_bodies(ClassCodeDescriptors, False);
        arrange_java_inner_classes(ClassCodeDescriptors, False);
        save_java_backups(ClassCodeDescriptors, False);  
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////

boolean
is_class_excluded(node class)
{
    const string    CLASS_EXCLUDED = "item[obj_id = ${class.id} && (${exclusion_predicate})]";

    if (class == NULL ||
	exclusion_predicate == "" ||
	selection_count(CLASS_EXCLUDED) == 0)
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

            generated = generated +
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

                generated = generated +
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


const string    BAD_FILENAME_TRANSLATE_TABLE = " !@#$%^&*()+-=|\\{}[],./<>?~`;:'\"\n\t";
const string    GOOD_FILENAME_TRANSLATE_TABLE = string_repeat("_", string_length(BAD_FILENAME_TRANSLATE_TABLE));

string
generic_generate_file_name(string s)
{
    return string_translate(s, BAD_FILENAME_TRANSLATE_TABLE, GOOD_FILENAME_TRANSLATE_TABLE);
}


///////////////////////////////////////////////////////////////////////////////

//
// "interface" file description   
//
//
//  header                  }   (1)
//
//  INCLUDES                \
//  include statements       }  (1 -- not used for all languages)
//  INCLUDES END            /
//
//  DECLARATIONS            \
//  forward declarations     }  (1 -- not used for all languages)
//  DECLARATIONS END        /
//
//  DEFINITION              \
//  class header            /   (1 per class)
//
//      BODY                \
//      class features       }  (1 per class)
//      END BODY            /
//    
//      SECOND BODY         \
//      more class features  }  (0 or 1 per class -- not used for all languages)
//      END SECOND BODY     /
//
//  class footer            \   (1 per class)
//  DEFINITION END          /
//
//  OPERATION               \
//  operation                }  (0 to many per class -- not used for all lang's)
//  OPERATION END           /
//
//  DERIVATIVES              }  (0 or 1 per class -- not used for all languages)
//
//      DERIVATIVE          \
//      derivative           }  (0 to many per class -- not used for all lang's)
//      END DERIVATIVE      /
//
//  END DERIVATIVES          }  (0 or 1 per class -- not used for all languages)
//
//  FOOTER                  \
//  footer                   }  (1 -- not used for all languages)
//  FOOTER END              /
//

void
write_interfaces(list class_list, list files)
{
    // write the "interfaces" file, incrementally

    int             i, n;
    int             start;
    // Remedy 2540 (C.L.) Added generated_incl to hold intermidiary string
    string          line, includes, generated, generated_decl, generated_incl;
    string          old_file, interface_file, content;
    list            old_files = list_create("string", 0);
    list            contents  = list_create("string", 0);
    ClassCodeDescriptor c;
    boolean         need_includes = True;
    int             tag, id, cix;
            

    generated_decl = "";

    //
    // 1.  read each referenced existing file and store its name/content in
    //     parallel lists
    //
    for (i = 0, n = list_count(class_list); i < n; i = i + 1)
    {
        c = list_get(class_list, i);
        interface_file = c.file.full_path_interface_name + c.file.interface_extension;
        read_store_backup_file (old_files, interface_file, contents, True);
    }


    if (incremental == True)
    {

        // 2.  incrementally generate each file (parse and update)
        for (i = 0, n = list_count(contents); i < n; i = i + 1)
        {
            start = 0;
            generated = "";
            old_file = list_get(old_files, i);
            content = list_get(contents, i);
            need_includes = True;
            
            while ((line = get_line(content, start)) != NULL)
            {
                if (event_found_tag(line, START_OPERATION_TAG))
                {
                    generated = generated + action_update_operation
                        (old_file, content, line, start, class_list, False);
                    start = skip_tag(content, start, END_OPERATION_TAG);

                } else if (event_found_tag(line, START_CLASS_DEFINITION_TAG))
                {
                    if (need_includes) 
                    {
                        // get the class descriptor using the id of the class
                        tag = string_find(line, 0, START_CLASS_DEFINITION_TAG);
                        id = get_int(line, tag + string_length(START_CLASS_DEFINITION_TAG));
                        if ( id != 0) 
                        {
                            if ((cix =
                                 find_index_of_class_in_list_by_id(class_list,id)) < 
                                                                   list_count(class_list))
                            {
                                
			      c = list_get(class_list, cix);
// Remedy Bug 2540 (C.L) check if generated_tagged_includes returns empty string before
// adding new line.                  
//                            generated  = generated + generate_tagged_includes(c) + "\n";
//
			      generated_incl = generate_tagged_includes(c);
			      if (generated_incl != "")
				generated = generated + generated_incl + "\n";
// End Remedy Bug 2540.
                            }
                        }
                        
                    }

                    generated = generated + generated_decl + "\n" + action_update_class_definition
                        (old_file, content, line, start, class_list, False, 
			START_CLASS_DEFINITION_TAG, END_CLASS_DEFINITION_TAG);
                    start = skip_tag(content, start, END_CLASS_DEFINITION_TAG);
                    generated_decl = "";
                } else if (event_found_tag(line, 
		    START_EXTRA_CLASS_DEFINITION_TAG))
                {
                    generated = generated + action_update_class_definition
                        (old_file, content, line, start, class_list, True,
			START_EXTRA_CLASS_DEFINITION_TAG, 
			END_EXTRA_CLASS_DEFINITION_TAG);
                    start = skip_tag(content, start, 
			END_EXTRA_CLASS_DEFINITION_TAG);

                } else if (event_found_tag(line, START_INCLUDES_TAG))
                {
                    generated = generated + action_update_includes
                        (old_file, content, line, start, class_list);
                    start = skip_tag(content, start, END_INCLUDES_TAG);
                    need_includes = False;

                } else if (event_found_tag(line, START_CLASS_DECLARATION_TAG))
                {
                    
                    generated_decl = action_update_class_declaration
                        (old_file, content, line, start, class_list);
                    start = skip_tag(content, start, END_CLASS_DECLARATION_TAG);

                } else if (event_found_tag(line, START_CLASS_DERIVATIVES_TAG))
                {
                    generated = generated + action_update_class_derivatives
                        (old_file, content, line, start, class_list);
                    start = skip_tag(content, start, END_CLASS_DERIVATIVES_TAG);

                } else if (event_found_tag(line, START_FOOTER_TAG))
                {
                    // no action -- we just remove it and rewrite it later
                    start = skip_tag(content, start, END_FOOTER_TAG);

                } else
                {
		  
		  // Remedy Bug 2540.  If generated_decl is emtpty string then don't worry
		  // if current line is only a newline. Otherwise do not add current read
		  // line to the generated sum if it is only a newline character.

		  if (generated_decl == "" || line != "\n")
		 
		  // End Remedy Bug 2540
		      generated = generated + line;
                    start = start + string_length(line);
		    
		}
            }
            write_file(old_file, generated);
            list_append(files, old_file);
        }
    }

    //
    // for each interface file referenced by the remaining classes, if the
    // class has not been visited during incremental code generation above,
    // then there exist two possibilities: (1) the class is going to be
    // written to a new file, or (2) the class is going to be appended to an
    // existing file.  if the class is written to a new file, we simply dump
    // everything to that file all at once and mark those classes as visited.
    // if the class is written to an existing file, we simply append the
    // class's definition to that file.
    //

    // 3.  write remaining classes
    for (i = 0, n = list_count(class_list); i < n; i = i + 1)
    {
        c = list_get(class_list, i);

        if (!c.written)
        {
            interface_file =c.file.full_path_interface_name + c.file.interface_extension;

            if (list_find(files, 0, interface_file) == list_count(files))
            {
                if (file_per_class == True)
                {
                    includes = generate_tagged_includes(c);
                    if (includes != "")
                        includes = includes + "\n";
                } else
                    includes = "";

                //  Declarations must be written for all classes in this 
                //  file at once, but definitions could be written for only 
                //  this class OR for all classes in the file.  
                //  The second approach is taken here.
                //
                write_file (interface_file,
                    language_generate_file_header (c) +
                    includes + 
                    generate_tagged_declarations (class_list, interface_file) + 
                    "\n" + 
                    generate_tagged_definitions (class_list, interface_file));

                list_append (files, interface_file);

            } else
            {
                file_append (files, interface_file, generate_tagged_class(c));
            }
        }

        // Write the remaining class's derivatives. Not all languages have them.
        //
        if (!c.derivatives_written)
        {
            generated = generate_tagged_class_derivatives (c);
            if (generated != "")
                file_append(files, interface_file, generated);
        }

    }

    return;
}


//
// "implementation" file description 
//
//
//  include statement (1 -- not used for all languages)
//
//  CLASS IMPLEMENTATION (1 per class -- not used for all languages)
//
//  IMPLEMENTATION DECLARATIONS                \
//  implementation declarations                 }  (0 or 1 per class)
//  IMPLEMENTATION DECLARATIONS END            /
//
//  OPERATION        \
//  operation         }   (0 to many per class)
//  OPERATION END    /
//
//  CLASS IMPLEMENTATION END (1 per class -- not used for all languages)
//


//
// "operation" file description
//
//
//  OPERATION        \
//  operation         }   (0 to many per class)
//  OPERATION END    /
//


void
write_implementations(list class_list, list ix_files, list files)
{
    // write the "implementations" file, incrementally

    int         i, j, m, n;
    ClassCodeDescriptor c;
    string      old_file, content, impl_file, op_file, op_file_extension;
    string      operation_stub;
    list        old_files = list_create("string", 0);
    list        contents  = list_create("string", 0);
    int         start, end, tag, id, cix, oix;
    string      line, generated, generated_minus_whitespace;
    int         old_file_length, extension_length;
    string      old_file_impl_extension = "";
    string      old_file_op_extension = "";

    //
    // 1.  read each referenced existing file and store its name/content in
    //     parallel lists
    //
    for (i = 0, n = list_count(class_list); i < n; i = i + 1)
    {
        c = list_get(class_list, i);

        impl_file = c.file.full_path_implementation_name + c.file.implementation_extension;
        op_file   = c.file.full_path_operation_name + c.file.operation_extension;

        // 
        //  If the implementation file for this class is one of the 
        //  interface files, read and store it, but don't back it up 
	//  because that has already been done by 'write_interfaces'.
        //
        if ( list_find(ix_files, 0, impl_file) != list_count(ix_files))
        {
	    if (generate_interface_files)
                read_store_backup_file(old_files, impl_file, contents, False);
        }
        else
        {
	    if (generate_implementation_files)
                read_store_backup_file(old_files, impl_file, contents, True);
        }

        if (c.file.operation_extension != c.file.implementation_extension && 
            c.file.operation_extension != c.file.interface_extension)
        {
	    if (generate_operation_files)
                read_store_backup_file(old_files, op_file, contents, True);
        }
    }


    if (incremental == True)
    {
        // 2.  incrementally generate each file (parse and update)
        for (i = 0, n = list_count(contents); i < n; i = i + 1)
        {
            start = 0;
            generated = "";
            old_file = list_get(old_files, i);
            content = list_get(contents, i);

            while ((line = get_line(content, start)) != NULL)
            {
                //
                //  Languages with class implementations (Ada) handle 
                //  updates of attributes, operations, and operation stubs 
                //  within action_update_class_implementation.
		//
                //  Languages without them (C++) handle updates of attributes
                //  and operations here.
		//
                //  Languages with class implementations but also with
                //  separate operation files (Ada) handle the updates 
		//  of those operations here also.
		//
		//  For future reference:
		//  Right now the 'extra' class will never have any operations
		//  defined in its separate operation file.  If it becomes
		//  possible for that to happen, new tags for the 'extra'
		//  class operations would need to be defined so that they
		//  could be recognized as belonging to the 'extra' class;
		//  and the 'action_update_operation' function would need to be
		//  changed to take the tags as arguments, just like
		//  'action_update_class_definition' and 
		//  'action_update_class_implementation'.
                //
                if (event_found_tag(line, START_CLASS_IMPLEMENTATION_TAG))
                {
                    generated = generated + action_update_class_implementation
                        (old_file, content, line, start, class_list, False,
			START_CLASS_IMPLEMENTATION_TAG, 
			END_CLASS_IMPLEMENTATION_TAG);
                    start = skip_tag(content, start, 
                        END_CLASS_IMPLEMENTATION_TAG);

                } else if (event_found_tag(line, 
		    START_EXTRA_CLASS_IMPLEMENTATION_TAG))
                {
                    generated = generated + action_update_class_implementation
                        (old_file, content, line, start, class_list, True,
			START_EXTRA_CLASS_IMPLEMENTATION_TAG, 
			END_EXTRA_CLASS_IMPLEMENTATION_TAG);
                    start = skip_tag(content, start, 
                        END_EXTRA_CLASS_IMPLEMENTATION_TAG);

                } else if (event_found_tag(line, START_OPERATION_TAG))
                {
                    generated = generated + action_update_operation
                        (old_file, content, line, start, class_list, False);
                    start = skip_tag(content, start, END_OPERATION_TAG);

                } else if (event_found_tag(line, START_IMPLEMENTATION_DECLS_TAG))
                {
                    generated = generated + action_update_attribute
                        (old_file, content, line, start, class_list, False);
                    start = skip_tag(content, start, END_IMPLEMENTATION_DECLS_TAG);

                } else
                {
		    generated = generated + line;
		    start = start + string_length(line);
                }
            }

	    //
	    // The tests of the old_file extension vs. implementation_extension
	    // and operation_extenstion are needed because, in the incremental
	    // update of the existing files above, 'action_update_operation'
	    // can be invoked for either an implementation file or an 
	    // operation file.
	    //
	    old_file_length = string_length (old_file);
	    extension_length = string_length (implementation_extension);
	    if (extension_length > 0)
		old_file_impl_extension = string_extract(old_file, 
		    old_file_length-extension_length, extension_length);
	    extension_length = string_length (operation_extension);
	    if (extension_length > 0)
		old_file_op_extension = string_extract(old_file, 
		    old_file_length-extension_length, extension_length);


 	    if ((generate_implementation_files && 
		old_file_impl_extension == implementation_extension) ||
		(generate_operation_files &&
		 old_file_op_extension == operation_extension)) {
 		//
		// Implementation file may be empty for some languages, in 
 		// which case, don't write it.
 		//
		generated_minus_whitespace = 
		    string_strip(generated, "L", " \t\n");
		if (generated_minus_whitespace != "") {
		    write_file(old_file, generated);
		    list_append(files, old_file);
		}
	    }
        }
    }


    // 3.  write remaining classes
    for (i = 0, n = list_count(class_list); i < n; i = i + 1)
    {
        c = list_get(class_list, i);

        impl_file= c.file.full_path_implementation_name + c.file.implementation_extension;

	if (generate_implementation_files)
	{
	    //
	    //  If it's a new implementation file, write the include directive
	    //
	    if (list_find(old_files, 0, impl_file) == list_count(old_files))
	    {
		list_append(old_files, impl_file);
		generated = language_generate_include(
		    c.file.name + c.file.interface_extension);
		if (generated != "") {
		    file_append(files, impl_file, generated);
		}
	    }

	    //  Write the remaining class's implementation.
	    //  Not all languages have one.
	    //
	    if (!c.implementation_written)
	    {
		generated = generate_tagged_class_implementation(c);
		if (generated != "") {
		    file_append(files, impl_file, generated);
		}
	    }

	    //  Write the remaining class's attributes.
	    //  This is not executed for languages with class implementations.
	    //
	    if(!c.implementation_written && !c.attribute_implementation_written)
	    {
		generated = generate_tagged_attribute(c);
		if (generated != "") {
		    file_append(files, impl_file, generated + "\n");
		}
	    }
	}

        //  Write the remaining class's operations.
        //  For languages with class implementations, this is only needed
        //  for the separate operation file, if there is one.
        //
        for (j = 0, m = list_count(c.ops.ids); j < m; j = j + 1)
        {
            if (!list_get(c.ops.written, j))
            {
                op_file_extension = list_get(c.ops.file_extensions, j); 
                op_file = c.file.full_path_operation_name + op_file_extension;

                if (op_file_extension == c.file.interface_extension &&
		    generate_interface_files)
                {
		    file_append(ix_files, op_file, 
				generate_tagged_operation(c, j));
                } else if ((op_file_extension == 
			   c.file.implementation_extension &&
			   generate_implementation_files) ||
			   (op_file_extension == c.file.operation_extension &&
			   generate_operation_files)) {
		    //
		    // if it's a new implementation file, 
		    // write the include directive
		    //
		    if (list_find(old_files, 0, op_file) == 
			list_count(old_files) &&
			list_get(c.ops.file_extensions, j) != 
			c.file.interface_extension) {
			list_append(old_files, op_file);
			generated = language_generate_include(
			    c.file.name + c.file.interface_extension);
			if (generated != "") {
			    file_append(files, op_file, generated);
			}
		    }

		    file_append(files, op_file, 
				generate_tagged_operation(c, j));
                }
            }
        }
    }

    return;
}

string
action_update_class_implementation (string file, string stream, string line, 
    int start, list class_list, boolean class_is_extra, string start_tag, 
    string end_tag)
{
    ClassCodeDescriptor c;
    int             tag, id, cix, end, j, m;
    string          old_code;
    string          generated = "";
    boolean         attributes_updated;


    // get the id of the class
    tag = string_find(line, 0, start_tag);
    id = get_int(line, tag + string_length(start_tag));

    if (class_is_extra)
        cix = find_index_of_extra_class_in_list_by_id (class_list, id);
    else
        cix = find_index_of_class_in_list_by_id (class_list, id);

    // is it an input class?  if so, update it, else should we keep it?
    if (cix < list_count(class_list))
    {
        c = list_get(class_list, cix);

        // does it belong in this file?
        if (file == c.file.full_path_implementation_name + c.file.implementation_extension)
        {

	    // write the new class implementation header
	    generated = generated + 
		generate_tagged_class_implementation_header (c);

	    // skip everything up to end of class implementation header
	    for (start = start + string_length(line);
		 (line = get_line(stream, start)) != NULL &&
		 (end = language_find_class_implementation_header_end(line)) == 
		 string_length(line);
		 start = start + string_length(line))
	    {
	    }

	    if (line == NULL)
	    {
		ide_warning ("Warning: file " + file + " was corrupted.");
        dfglhdfjh
		return "";
	    }

	    //  This flag used because attributes are optional and may not
	    //  be there from a previous code generation.
	    //  This makes sure they get written in proper location.
	    //
	    attributes_updated = False;

	    //  After each call to an 'action_update_...' function, we need to
	    //  re-get 'c' from the class list because its copy therein
	    //  may have been updated.
	    //
	    start = start + string_length(line);
	    while ((line = get_line(stream, start)) != NULL &&
		string_find(line, 0, end_tag) == 
		string_length(line))
	    {
		if (event_found_tag(line, START_IMPLEMENTATION_DECLS_TAG))
		{
		    generated = generated + action_update_attribute
			(file, stream, line, start, class_list, class_is_extra);
		    start = skip_tag(stream, start, END_IMPLEMENTATION_DECLS_TAG);

		    attributes_updated = True;

		    c = list_get (class_list, cix);

		} else if (event_found_tag(line, START_OPERATION_TAG))
		{
		    if (!attributes_updated)
			generated = generated + 
			    generate_tagged_attribute_newline (c);
		    attributes_updated = True;

		    generated = generated + action_update_operation
			(file, stream, line, start, class_list, class_is_extra);
		    start = skip_tag(stream, start, END_OPERATION_TAG);

		    c = list_get (class_list, cix);

		} else if (event_found_tag(line, START_OPERATION_STUB_TAG))
		{
		    if (!attributes_updated)
			generated = generated + 
			    generate_tagged_attribute_newline (c);
		    attributes_updated = True;

		    generated = generated + action_update_operation_stub
			(file, stream, line, start, class_list, class_is_extra);
		    start = skip_tag(stream, start, END_OPERATION_STUB_TAG);

		    c = list_get (class_list, cix);

		} else
		{
		    generated = generated + line;
		    start = start + string_length(line);
		}
	    }

	    if (!attributes_updated)
		generated = generated + generate_tagged_attribute_newline (c);

	    //  Write the remaining operations.
	    generated = generated + generate_tagged_operations (c);

	    //
	    // write the class implementation footer 
	    // and mark the class implementation as being updated
	    //
	    generated = generated + 
		generate_tagged_class_implementation_footer (c);

	    c.implementation_written = True;
	    c.attribute_implementation_written = True;
	    list_set (class_list, cix, c);
        }
    } 
    else
    {
        if (keep_old_classes == True)
        {
            end = string_find (stream, start, end_tag) + 
                string_length(end_tag) + 1;

            old_code = string_extract(stream, start, end - start);
            generated = generated + old_code;
        }
    }

    return generated;
}

string
action_update_operation (string file, string stream, string line, int start, 
    list class_list, boolean class_is_extra)
{
    ClassCodeDescriptor c;
    int             tag, id, cix, oix, end;
    string          old_code;
    string          generated = "";
    int  delimiter_index, id_index;


    // get the id of the class 
    tag = string_find(line, 0, START_OPERATION_TAG);
    id = get_int(line, tag + string_length(START_OPERATION_TAG));

    if (class_is_extra)
        cix = find_index_of_extra_class_in_list_by_id (class_list, id);
    else
        cix = find_index_of_class_in_list_by_id (class_list, id);

    // is it an input class?  if so, update it, else should we keep it?
    if (cix < list_count(class_list))
    {
        c = list_get(class_list, cix);

        // get the id of the operation
        delimiter_index = 
            string_find(line, tag + string_length(START_OPERATION_TAG), SD);

        id_index = delimiter_index + string_length (SD);

        id = get_int (line, id_index);

        oix = list_find(c.ops.ids, 0, id);

        // is it an input operation and does it belong in this file?
        if (oix != list_count(c.ops.ids) &&
            file == c.file.full_path_operation_name + list_get(c.ops.file_extensions, oix))
        {

            // write the new operation header
            generated = generated + generate_tagged_operation_header(c, oix);

            // skip everything up to end of operation header
            for (start = start + string_length(line);
                 (line = get_line(stream, start)) != NULL &&
                 (end = language_find_operation_header_end(line)) == 
                 string_length(line);
                 start = start + string_length(line))
            {
            }

	    if (line == NULL)
	    {
		ide_warning ("Warning: file " + file + " was corrupted.");
		return "";
	    }

            // keep everything up to beginning of operation body
            start = start + string_length(line);
            while ((line = get_line(stream, start)) != NULL &&
                language_find_operation_body_begin(line) == string_length(line))
            {
                generated = generated + line;
                start = start + string_length(line);
            }

            if (line != NULL)
            {
                generated = generated + line;
                start = start + string_length(line);
            }

            // replace the stp part of the body
            while ((line = get_line(stream, start)) != NULL &&
             string_find(line, 0, END_OPERATION_TAG) == string_length(line))
            {
                if (string_find(line, 0, START_OP_IMPLEMENTATION_TAG) != 
                    string_length(line))
                {
                    generated = generated + 
                        generate_tagged_operation_body(c, oix);
                    start = 
                        string_find (stream, start, END_OP_IMPLEMENTATION_TAG) + 
                        string_length(END_OP_IMPLEMENTATION_TAG);
                } else
                {
                    generated = generated + line;
                    start = start + string_length(line);
                }
            }

            //
            // write the tagged comment and mark the operation as being
            // updated
            //
            generated = generated + generate_tagged_operation_footer(c, oix);

            list_set(c.ops.written, oix, True);
            list_set(class_list, cix, c);
        }
    } else
    {
        if (keep_old_classes == True)
        {
            end = string_find (stream, start, END_OPERATION_TAG) + 
                string_length(END_OPERATION_TAG) + 1;

            old_code = string_extract(stream, start, end - start);
            generated = generated + old_code;
        }
    }

    return generated;
}

string
action_update_class_definition (string file, string stream, string line, 
    int start, list class_list, boolean class_is_extra, string start_tag, 
    string end_tag)
{
    ClassCodeDescriptor c;
    int             tag, id, cix, oix, end;
    string          old_code;
    string          generated = "";
    boolean         second_body_updated;

    // get the id of the class
    tag = string_find(line, 0, start_tag);
    id = get_int(line, tag + string_length(start_tag));

    if (class_is_extra)
	cix = find_index_of_extra_class_in_list_by_id (class_list, id);
    else
	cix = find_index_of_class_in_list_by_id (class_list, id);

    // is it an input class?  if so, update it, else should we keep it?
    if (cix < list_count(class_list))
    {
        c = list_get(class_list, cix);

        // does it belong in this file?
        if (file == c.file.full_path_interface_name + c.file.interface_extension)
        {
            // write the new definition header
            generated = generated + generate_tagged_class_header(c);

            // skip everything up to beginning of class definition
            for (start = start + string_length(line);
                 (line = get_line(stream, start)) != NULL &&
                 (end = language_find_class_definition_start(line)) == 
                 string_length(line);
                 start = start + string_length(line))
            {
            }

	    if (line == NULL)
	    {
		ide_warning ("Warning: file " + file + " was corrupted.");
		return "";
	    }

	    //  This flag used because second body is optional and may not
	    //  be there from a previous code generation.
	    //  This makes sure it gets written before in proper location.
	    //
            second_body_updated = False;

            // replace the stp part of the body
            start = start + string_length(line);
            while ((line = get_line(stream, start)) != NULL &&
                   string_find(line, 0, end_tag) == 
                   string_length(line))
            {
                if (string_find(line, 0, START_CLASS_BODY_TAG) 
                    != string_length(line))
                {
                    generated = generated + generate_tagged_class_body(c);
                    start = skip_tag (stream, start, END_CLASS_BODY_TAG);

                } else if (string_find(line, 0, START_CLASS_SECOND_BODY_TAG) 
                    != string_length(line))
                {
                    generated =generated + generate_tagged_class_second_body(c);
                        start=skip_tag(stream,start, END_CLASS_SECOND_BODY_TAG);

		    second_body_updated = True;

                } else
                {
                    generated = generated + line;
                    start = start + string_length(line);
                }
            }

            if (!second_body_updated)
                generated = generated + generate_tagged_class_second_body (c);

            //
            // write the tagged comment and mark the class as being
            // updated
            //
            generated = generated + generate_tagged_class_footer(c);
            c.written = True;
            list_set(class_list, cix, c);
        }
    } else
    {
        if (keep_old_classes == True)
        {
            end = string_find(stream, start, end_tag) + 
                string_length(end_tag) + 1;
            old_code = string_extract(stream, start, end - start);
            generated = generated + old_code;
        }
    }

    return generated;
}

string
action_update_includes(string file, string stream, string line, int start, list class_list)
{
    ClassCodeDescriptor c;
    int             tag, id, cix, oix, end;
    string          generated = "";

    // get the id of the class
    tag = string_find(line, 0, START_INCLUDES_TAG);
    id = get_int(line, tag + string_length(START_INCLUDES_TAG));

    // is it an input class?  if so, update it, else discard it
    if ((cix = find_index_of_class_in_list_by_id(class_list, id)) < list_count(class_list))
    {
        c = list_get(class_list, cix);
        generated = generated + generate_tagged_includes(c);
    }

    return generated;
}
 
string
action_update_class_declaration(string file, string stream, string line, int start, list class_list)
{
    ClassCodeDescriptor c;
    int             tag, id, cix, oix, end;
    string          old_code;
    list            old_code_list;
    set             declarations;
    string          generated = "";

    if (keep_old_classes == True)
    {
        end = string_find (stream, start, END_CLASS_DECLARATION_TAG) + 
            string_length(END_CLASS_DECLARATION_TAG) + 1;

        old_code_list = string_to_list (
            string_extract (stream, start, end - start), "\n");

        // the start comment
        list_delete(old_code_list, 0);

        // the end comment
        list_delete(old_code_list, list_count(old_code_list) - 1);

        old_code = list_to_string(old_code_list, "\n") + "\n";

        declarations = list_to_set (string_to_list (old_code + 
            language_generate_declaration_list(class_list, file) + "\n", "\n"));

        generated = generated + tag_declarations (
            list_to_string (set_to_list(declarations), "\n") + "\n");
    } else
    {
        generated = generated + generate_tagged_declarations(class_list, file);
    }

    return generated;
}

string
action_update_attribute (string file, string stream, string line, int start, 
    list class_list, boolean class_is_extra)
{
    ClassCodeDescriptor c;
    int             tag, id, cix, oix, end;
    string            old_class_related_code;
    string          generated = "";

    // get the id of the class
    tag = string_find(line, 0, START_IMPLEMENTATION_DECLS_TAG);
    id = get_int(line, tag + string_length(START_IMPLEMENTATION_DECLS_TAG));

    if (class_is_extra)
        cix = find_index_of_extra_class_in_list_by_id (class_list, id);
    else
        cix = find_index_of_class_in_list_by_id (class_list, id);

    // is it an input class?  if so, update it, else should we keep it?
    if (cix < list_count(class_list))
    {
        c = list_get(class_list, cix);

        // does it belong in this file?
        if (file == c.file.full_path_implementation_name + c.file.implementation_extension)
        {
            //
            // write the tagged comment and mark the attributes as updated
            //
            generated = generated + generate_tagged_attribute (c);

            c.attribute_implementation_written = True;
            list_set (class_list, cix, c);
        }
    } else
    {
        if (keep_old_classes == True)
        {
            end = string_find (stream, start, END_IMPLEMENTATION_DECLS_TAG) + 
                string_length(END_IMPLEMENTATION_DECLS_TAG) + 1;
            old_class_related_code = string_extract(stream, start, end-start);
            generated = generated + old_class_related_code;
        }
    }

    return generated;
}

string
action_update_operation_stub (string file, string stream, string line, 
    int start, list class_list, boolean class_is_extra)
{
    ClassCodeDescriptor c;
    int             tag, id, cix, oix, end;
    string            old_class_related_code;
    string          generated = "";
    int             delimiter_index, id_index;


    // get the id of the class
    tag = string_find (line, 0, START_OPERATION_STUB_TAG);
    id = get_int (line, tag + string_length(START_OPERATION_STUB_TAG));

    if (class_is_extra)
        cix = find_index_of_extra_class_in_list_by_id (class_list, id);
    else
        cix = find_index_of_class_in_list_by_id (class_list, id);

    // is it an input class?  if so, update it, else should we keep it?
    if (cix < list_count(class_list))
    {

        c = list_get(class_list, cix);

        // get the id of the operation
        delimiter_index =
           string_find(line, tag + string_length(START_OPERATION_STUB_TAG), SD);
 
        id_index = delimiter_index + string_length (SD);
 
        id = get_int (line, id_index);
 
        oix = list_find(c.ops.ids, 0, id);
 
        // is it an input operation and does it belong in this file?
        if (oix != list_count(c.ops.ids) &&
            file == c.file.full_path_implementation_name + implementation_extension)
        {
            //  write the new operation stub 
            generated = generated + 
                generate_tagged_operation_stub (c, oix);

            //  mark stubs as being written
            list_set(c.ops.stubs_written, oix, True);
            list_set(class_list, cix, c);
        }
    } else
    {
        if (keep_old_classes == True)
        {
            end = string_find (stream, start, END_OPERATION_STUB_TAG) + 
                string_length(END_OPERATION_STUB_TAG) + 1;
            old_class_related_code = string_extract(stream, start, end-start);
            generated = generated + old_class_related_code;
        }
    }

    return generated;
}

boolean
event_found_tag(string line, string tag)
{
    // is the tag found on the line?
    if (string_find(line, 0, tag) != string_length(line))
        return True;

    return False;
}

int 
skip_tag(string stream, int start, string tag)
{
    // return the character position in stream after tag
    return string_find(stream, start, tag) + string_length(tag) + 1;
}

string
get_line(string stream, int start)
{
    // get a newline-delimited line from stream
    int             newline;

    if (start >= string_length(stream))
        return NULL;

    newline = string_find(stream, start, "\n");

    if (newline == string_length(stream))
        return string_extract(stream, start, newline - start);

    return string_extract(stream, start, (newline - start) + 1);
}

string
next_token (string stream, int from)
{
    // read the next token in stream

    int space, tab, newline, end;
    string substring;

    if (stream == NULL || stream == "")
        return "";

    substring = string_extract (stream, from, string_length(stream)-from);
    substring = string_strip (substring, "L", " \t\n");

    space   = string_find (substring, 0, " ");
    newline = string_find (substring, 0, "\n");
    tab     = string_find (substring, 0, "\t");
 
    if (space <= newline)
        if (space <= tab)
            end = space;
        else
            end = tab;
    else
        if (newline <= tab)
            end = newline;
        else
            end = tab;

    return string_extract (substring, 0, end);
}

int
get_int(string stream, int from)
{
    // read the next integer in stream
    string          id = "0", tid;

    if (stream == NULL || stream == "")
        return 0;

    while (from < string_length(stream) && 
        (id = string_extract(stream, from, 1)) == " ")
    {
        from = from + 1;
    }

    for (from = from + 1;
        from < string_length(stream) && 
        (tid = string_extract(stream, from, 1)) != " " &&
        (string_converts(tid, "int") || tid == "-" || tid == "+");
        from = from + 1)
    {
        id = id + tid;
    }

    return to_int(id);
}

int
find_index_of_class_in_list_by_id(list class_list, int id)
{
    // find index of class in list by id
    int i, n;
    ClassCodeDescriptor c;

    for (i = 0, n = list_count(class_list); i < n; i = i + 1)
    {
        c = list_get(class_list, i);
        if (c.id == id)
            break;
    }
    return i;
}

void
read_store_backup_file(list files, string file, list files_contents, boolean backup)
{
    // read the file, store its contents, and back it up
    string          contents = "";

    // if we already processed the file or we can't read it, then return
    if (list_find(files, 0, file) != list_count(files) || 
        !read_file_access(file))
        return;

    // read it, store it, and back it up
    contents = read_file(file);

    if (contents == NULL)
        contents = "";

    list_append(files, file);
    list_append(files_contents, contents);

    if (backup == True)
    {
        delete_file(file + ".BAK");
        if (0 == copy_file(file,file + ".BAK"))
            delete_file(file);
    }

    return;
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
uml_name_type_syntax_2_type_space_name_syntax(string rumbaugh_names_types, string option)
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
	if (i == 0)
	    types_names = pair;
	else
	    types_names = types_names + ", " + pair;
    }
    return string_strip(types_names, "B", " ");
}


string
parse_label_from_association(node association)
{
    int             d;
    string          label = "";

    if (association == NULL || association.name == "")
	return "";

    if ((d = string_find(association.name, 0, UML_ASSOCIATION_NAME_DELIMITER1)) != string_length(association.name) && d > 0)
	label = string_extract(association.name, 0, d);

    return string_strip(label, "B", " ");
}

string
parse_label_from_aggregation(node aggregation)
{
    if (aggregation == NULL || aggregation.name == "") 
	return "";

    return string_strip(aggregation.name, "B", " ");
}

string
indent(int level)
{
    int i;
    string indent_string = "";

    for (i = 0; i < level; i = i + 1) 
	indent_string = indent_string + CODE_GEN_INDENT;

    return indent_string;
}

