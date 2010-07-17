//
//      StP/UML 
//      Confidential property of Aonix
//      Copyright (c) 1997
//      All rights reserved

//
// The patterns for binary associations only.
//  
struct uml_java_binary_pattern_descriptor
{
    string  unqualified_single_pattern;
    string  unqualified_unordered_pattern;
    string  unqualified_ordered_pattern;
    string  qualified_single_pattern;
    string  qualified_unordered_pattern;
    string  qualified_ordered_pattern;
    string  inverse_qualified_single_pattern;
    string  inverse_qualified_unordered_pattern;
    string  inverse_qualified_ordered_pattern;
    string  double_qualified_single_pattern;
    string  double_qualified_unordered_pattern;
    string  double_qualified_ordered_pattern;
};

//
// All the association patterns for a given family.
//
struct uml_java_pattern_descriptor
{
    string                         name;
    uml_java_binary_pattern_descriptor  associations;
    uml_java_binary_pattern_descriptor  aggregations;
    uml_java_binary_pattern_descriptor  compositions;
    string                         nary_pattern;
};

//
// Some predefined binary patterns which can be used by more than
// one pattern family if desired.
// 

uml_java_binary_pattern_descriptor
java_generic_patterns = initialize_java_generic_patterns();

uml_java_binary_pattern_descriptor
java_instance_patterns = initialize_java_instance_patterns();

//
// These are the pattern families which are initialized and inserted
// into the global uml_java_pattern_families list.
//
uml_java_pattern_descriptor 
default_uml_java_pattern_family = initialize_default_uml_java_pattern_family();

uml_java_pattern_descriptor 
simple_uml_java_pattern_family = initialize_simple_uml_java_pattern_family();

//
// This is the global pattern families list that is referenced and examined
// outside this file.
list 
uml_java_pattern_families = initialize_uml_java_pattern_families();

///////////////////////////////////////////////////////////////////////////////

//
// Functions to initialize the predefined binary patterns.
//

uml_java_binary_pattern_descriptor
initialize_java_generic_patterns()
{
    uml_java_binary_pattern_descriptor patterns;

    patterns.unqualified_single_pattern = 
	"${target_class.name} ${identifier_name}";
    patterns.unqualified_unordered_pattern = 
	//"${target_class.name} ${identifier_name}";
	"Vector ${identifier_name} = new Vector();";
    patterns.unqualified_ordered_pattern = 
	//"${target_class.name} ${identifier_name}";
	"Vector ${identifier_name} = new Vector();";
    patterns.qualified_single_pattern = 
	"${target_class.name} ${identifier_name}";
    patterns.qualified_unordered_pattern = 
	//"${target_class.name} ${identifier_name}";
	"Vector ${identifier_name} = new Vector();";
    patterns.qualified_ordered_pattern = 
	//"${target_class.name} ${identifier_name}";
	"Vector ${identifier_name} = new Vector();";
    patterns.inverse_qualified_single_pattern = 
	"${target_class.name} ${identifier_name}";
    patterns.inverse_qualified_unordered_pattern = 
	//"${target_class.name} ${identifier_name}";
	"Vector ${identifier_name} = new Vector();";
    patterns.inverse_qualified_ordered_pattern = 
	//"${target_class.name} ${identifier_name}";
	"Vector ${identifier_name} = new Vector();";
    patterns.double_qualified_single_pattern = 
	"${target_class.name} ${identifier_name}";
    patterns.double_qualified_unordered_pattern = 
	//"${target_class.name} ${identifier_name}";
	"Vector ${identifier_name} = new Vector();";
    patterns.double_qualified_ordered_pattern = 
	//"${target_class.name} ${identifier_name}";
	"Vector ${identifier_name} = new Vector();";

    return patterns;
}

uml_java_binary_pattern_descriptor
initialize_java_instance_patterns()
{
    uml_java_binary_pattern_descriptor patterns;

    patterns.unqualified_single_pattern = 
        "${target_class.name} ${identifier_name}";
    patterns.unqualified_unordered_pattern =     
        "${target_class.name}[] ${identifier_name} = new ${target_class.name}${explicit_multiplicity}";
    patterns.unqualified_ordered_pattern = 
        "${target_class.name}[] ${identifier_name} = new ${target_class.name}${explicit_multiplicity}";
    patterns.qualified_single_pattern = 
        "${target_class.name} ${identifier_name}";
    patterns.qualified_unordered_pattern = 
        "${target_class.name}[] ${identifier_name} = new ${target_class.name}${explicit_multiplicity}";
    patterns.qualified_ordered_pattern = 
        "${target_class.name}[] ${identifier_name} = new ${target_class.name}${explicit_multiplicity}";
    patterns.inverse_qualified_single_pattern = 
        "${target_class.name} ${identifier_name}";
    patterns.inverse_qualified_unordered_pattern = 
        "${target_class.name}[] ${identifier_name} = new ${target_class.name}${explicit_multiplicity}";
    patterns.inverse_qualified_ordered_pattern = 
        "${target_class.name}[] ${identifier_name} = new ${target_class.name}${explicit_multiplicity}";
    patterns.double_qualified_single_pattern =  
        "${target_class.name} ${identifier_name}";
    patterns.double_qualified_unordered_pattern = 
        "${target_class.name}[] ${identifier_name} = new ${target_class.name}${explicit_multiplicity}";
    patterns.double_qualified_ordered_pattern = 
        "${target_class.name}[] ${identifier_name} = new ${target_class.name}${explicit_multiplicity}";

    return patterns;
}


///////////////////////////////////////////////////////////////////////////////

//
// Functions to initialize the pattern families which will be inserted into
// the global uml_java_pattern_families list.
//
uml_java_pattern_descriptor
initialize_default_uml_java_pattern_family()
{
    uml_java_pattern_descriptor uml_java_pattern_family;

    uml_java_pattern_family.name = "Generic";

    uml_java_pattern_family.associations = java_generic_patterns;
    uml_java_pattern_family.aggregations = java_generic_patterns;
    uml_java_pattern_family.compositions = java_generic_patterns;
    
    uml_java_pattern_family.nary_pattern = 
	"Assn_${target_class.name} ${identifier_name}";
    
    return uml_java_pattern_family;
}

uml_java_pattern_descriptor
initialize_simple_uml_java_pattern_family()
{
    uml_java_pattern_descriptor uml_java_pattern_family;

    uml_java_pattern_family.name = "Instance";

    uml_java_pattern_family.associations = java_instance_patterns;
    uml_java_pattern_family.aggregations = java_instance_patterns;
    uml_java_pattern_family.compositions = java_instance_patterns;
    
    uml_java_pattern_family.nary_pattern = 
	"${target_class.name} ${identifier_name}";

    return uml_java_pattern_family;
}


///////////////////////////////////////////////////////////////////////////////

//
// The function to initialize the global uml_java_pattern_families list.
//
list
initialize_uml_java_pattern_families()
{
    list uml_java_pattern_families = list_create("uml_java_pattern_descriptor", 0);

    list_append(uml_java_pattern_families, default_uml_java_pattern_family);
    list_append(uml_java_pattern_families, simple_uml_java_pattern_family);

    return uml_java_pattern_families;
}

//////////////////////////////////////////////////////////////////////////////

//
// The function to retrieve a particular pattern family from the global list
// by name.
//
uml_java_pattern_descriptor
get_family_uml_java_pattern_descriptor(string name)
{
    uml_java_pattern_descriptor uml_java_pattern_descriptor;
    int                    i, n;

    for (i = 0, n = list_count(uml_java_pattern_families); i < n; i = i + 1)
    {
	uml_java_pattern_descriptor = list_get(uml_java_pattern_families, i);
	if (name == uml_java_pattern_descriptor.name)
	    return uml_java_pattern_descriptor;
    }

    // if 'name' not found in list
    return default_uml_java_pattern_family;
}

