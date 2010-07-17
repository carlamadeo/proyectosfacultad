//
//      StP/UML 
//      Confidential property of Aonix
//      Copyright (c) 1997
//      All rights reserved

//
// The patterns for binary associations only.
//  
struct uml_binary_pattern_descriptor
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
struct uml_pattern_descriptor
{
    string                         name;
    uml_binary_pattern_descriptor  associations;
    uml_binary_pattern_descriptor  aggregations;
    uml_binary_pattern_descriptor  compositions;
    string                         nary_pattern;
};

//
// Some predefined binary patterns which can be used by more than
// one pattern family if desired.
// 
uml_binary_pattern_descriptor
pointer_template_patterns = initialize_pointer_template_patterns();

uml_binary_pattern_descriptor
instance_template_patterns = initialize_instance_template_patterns();

uml_binary_pattern_descriptor
pointer_patterns = initialize_pointer_patterns();

uml_binary_pattern_descriptor
instance_patterns = initialize_instance_patterns();

uml_binary_pattern_descriptor
rogue_wave_binary_patterns = initialize_rogue_wave_binary_patterns();

//
// These are the pattern families which are initialized and inserted
// into the global uml_pattern_families list.
//
uml_pattern_descriptor 
default_uml_pattern_family = initialize_default_uml_pattern_family();

uml_pattern_descriptor 
simple_uml_pattern_family = initialize_simple_uml_pattern_family();

uml_pattern_descriptor 
rogue_wave_uml_pattern_family = initialize_rogue_wave_uml_pattern_family();

//
// This is the global pattern families list that is referenced and examined
// outside this file.
list 
uml_pattern_families = initialize_uml_pattern_families();

///////////////////////////////////////////////////////////////////////////////

//
// Functions to initialize the predefined binary patterns.
//
uml_binary_pattern_descriptor
initialize_pointer_template_patterns()
{
    uml_binary_pattern_descriptor patterns;
    
    patterns.unqualified_single_pattern = 
	"${target_class.name}* ${identifier_name}";
    patterns.unqualified_unordered_pattern = 
	"Set<${target_class.name}*> ${identifier_name}";
    patterns.unqualified_ordered_pattern = 
	"List<${target_class.name}*> ${identifier_name}";
    patterns.qualified_single_pattern = 
	"Dict<${qualifier.type},${target_class.name}*> ${identifier_name}";
    patterns.qualified_unordered_pattern = 
	"Dict<${qualifier.type},Set<${target_class.name}*>> ${identifier_name}";
    patterns.qualified_ordered_pattern = 
	"Dict<${qualifier.type},List<${target_class.name}*>> ${identifier_name}";
    patterns.inverse_qualified_single_pattern = 
	"Tuple<${target_class.name}*,${inverse_qualifier.type}> ${identifier_name}";
    patterns.inverse_qualified_unordered_pattern = 
	"Set<Tuple<${target_class.name}*,${inverse_qualifier.type}>> ${identifier_name}";
    patterns.inverse_qualified_ordered_pattern = 
	"List<Tuple<${target_class.name}*,${inverse_qualifier.type}>> ${identifier_name}";
    patterns.double_qualified_single_pattern = 
	"Dict<${qualifier.type},Tuple<${target_class.name}*,${inverse_qualifier.type}>> ${identifier_name}";
    patterns.double_qualified_unordered_pattern = 
	"Dict<${qualifier.type},Set<Tuple<${target_class.name}*,${inverse_qualifier.type}>>> ${identifier_name}";
    patterns.double_qualified_ordered_pattern = 
	"Dict<${qualifier.type},List<Tuple<${target_class.name}*,${inverse_qualifier.type}>>> ${identifier_name}";

    return patterns;
}

uml_binary_pattern_descriptor
initialize_instance_template_patterns()
{
    uml_binary_pattern_descriptor patterns;
    
    patterns.unqualified_single_pattern = 
	"${target_class.name} ${identifier_name}";
    patterns.unqualified_unordered_pattern = 
	"Set<${target_class.name}> ${identifier_name}";
    patterns.unqualified_ordered_pattern = 
	"List<${target_class.name}> ${identifier_name}";
    patterns.qualified_single_pattern = 
	"Dict<${qualifier.type},${target_class.name}> ${identifier_name}";
    patterns.qualified_unordered_pattern = 
	"Dict<${qualifier.type},Set<${target_class.name}>> ${identifier_name}";
    patterns.qualified_ordered_pattern = 
	"Dict<${qualifier.type},List<${target_class.name}>> ${identifier_name}";
    patterns.inverse_qualified_single_pattern = 
	"Tuple<${target_class.name},${inverse_qualifier.type}> ${identifier_name}";
    patterns.inverse_qualified_unordered_pattern = 
	"Set<Tuple<${target_class.name},${inverse_qualifier.type}>> ${identifier_name}";
    patterns.inverse_qualified_ordered_pattern = 
	"List<Tuple<${target_class.name},${inverse_qualifier.type}>> ${identifier_name}";
    patterns.double_qualified_single_pattern = 
	"Dict<${qualifier.type},Tuple<${target_class.name},${inverse_qualifier.type}>> ${identifier_name}";
    patterns.double_qualified_unordered_pattern = 
	"Dict<${qualifier.type},Set<Tuple<${target_class.name},${inverse_qualifier.type}>>> ${identifier_name}";
    patterns.double_qualified_ordered_pattern = 
	"Dict<${qualifier.type},List<Tuple<${target_class.name},${inverse_qualifier.type}>>> ${identifier_name}";

    return patterns;
}


uml_binary_pattern_descriptor
initialize_pointer_patterns()
{
    uml_binary_pattern_descriptor patterns;

    patterns.unqualified_single_pattern = 
	"${target_class.name}* ${identifier_name}";
    patterns.unqualified_unordered_pattern = 
	"${target_class.name}* ${identifier_name}";
    patterns.unqualified_ordered_pattern = 
	"${target_class.name}* ${identifier_name}";
    patterns.qualified_single_pattern = 
	"${target_class.name}* ${identifier_name}";
    patterns.qualified_unordered_pattern = 
	"${target_class.name}* ${identifier_name}";
    patterns.qualified_ordered_pattern = 
	"${target_class.name}* ${identifier_name}";
    patterns.inverse_qualified_single_pattern = 
	"${target_class.name}* ${identifier_name}";
    patterns.inverse_qualified_unordered_pattern = 
	"${target_class.name}* ${identifier_name}";
    patterns.inverse_qualified_ordered_pattern = 
	"${target_class.name}* ${identifier_name}";
    patterns.double_qualified_single_pattern = 
	"${target_class.name}* ${identifier_name}";
    patterns.double_qualified_unordered_pattern = 
	"${target_class.name}* ${identifier_name}";
    patterns.double_qualified_ordered_pattern = 
	"${target_class.name}* ${identifier_name}";

    return patterns;
}

uml_binary_pattern_descriptor
initialize_instance_patterns()
{
    uml_binary_pattern_descriptor patterns;

    patterns.unqualified_single_pattern = 
	"${target_class.name} ${identifier_name}";
    patterns.unqualified_unordered_pattern = 
	"${target_class.name} ${identifier_name}${explicit_multiplicity}";
    patterns.unqualified_ordered_pattern = 
	"${target_class.name} ${identifier_name}${explicit_multiplicity}";
    patterns.qualified_single_pattern = 
	"${target_class.name} ${identifier_name}";
    patterns.qualified_unordered_pattern = 
	"${target_class.name} ${identifier_name}${explicit_multiplicity}";
    patterns.qualified_ordered_pattern = 
	"${target_class.name} ${identifier_name}${explicit_multiplicity}";
    patterns.inverse_qualified_single_pattern = 
	"${target_class.name} ${identifier_name}";
    patterns.inverse_qualified_unordered_pattern = 
	"${target_class.name} ${identifier_name}${explicit_multiplicity}";
    patterns.inverse_qualified_ordered_pattern = 
	"${target_class.name} ${identifier_name}${explicit_multiplicity}";
    patterns.double_qualified_single_pattern = 
	"${target_class.name} ${identifier_name}";
    patterns.double_qualified_unordered_pattern = 
	"${target_class.name} ${identifier_name}${explicit_multiplicity}";
    patterns.double_qualified_ordered_pattern = 
	"${target_class.name} ${identifier_name}${explicit_multiplicity}";

    return patterns;
}

uml_binary_pattern_descriptor
initialize_rogue_wave_binary_patterns()
{
    uml_binary_pattern_descriptor patterns;
    
    patterns.unqualified_single_pattern = 
	"Link<${target_class.name}>";
    patterns.unqualified_unordered_pattern = 
	"Set<Link<${target_class.name}>>";
    patterns.unqualified_ordered_pattern = 
	"List<Link<${target_class.name}>>";
    patterns.qualified_single_pattern = 
	"Dict<${qualifier.type},Link<${target_class.name}>>";
    patterns.qualified_unordered_pattern = 
	"Dict<${qualifier.type},Set<Link<${target_class.name}>>>";
    patterns.qualified_ordered_pattern = 
	"BList<${qualifier.type},Link<${target_class.name}>>";
    patterns.inverse_qualified_single_pattern = 
	"Link<${target_class.name}>";
    patterns.inverse_qualified_unordered_pattern = 
	"BAssn<Link<${target_class.name}>,${inverse_qualifier.type}>";
    patterns.inverse_qualified_ordered_pattern = 
	"BList<Link<${target_class.name}>,${inverse_qualifier.type}>";
    patterns.double_qualified_single_pattern = 
	"Dict<${qualifier.type},Tuple<Link<${target_class.name}>,${inverse_qualifier.type}>>";
    patterns.double_qualified_unordered_pattern = 
	"Dict<${qualifier.type},Set<Tuple<Link<${target_class.name}>,${inverse_qualifier.type}>>>";
    patterns.double_qualified_ordered_pattern = 
	"List<Tuple<${qualifier.type},Link<${target_class.name}>,${inverse_qualifier.type}>>";

    return patterns;
}

///////////////////////////////////////////////////////////////////////////////

//
// Functions to initialize the pattern families which will be inserted into
// the global uml_pattern_families list.
//
uml_pattern_descriptor
initialize_default_uml_pattern_family()
{
    uml_pattern_descriptor uml_pattern_family;

    // generic collection classes, originated in OMTool
    uml_pattern_family.name = "Default Template";

    uml_pattern_family.associations = pointer_template_patterns;
    uml_pattern_family.aggregations = pointer_template_patterns;
    uml_pattern_family.compositions = instance_template_patterns;
    
    uml_pattern_family.nary_pattern = 
	"Assn<${target_class.name}>* ${identifier_name}";
    
    return uml_pattern_family;
}

uml_pattern_descriptor
initialize_simple_uml_pattern_family()
{
    uml_pattern_descriptor uml_pattern_family;

    uml_pattern_family.name = "Pointer/Instance";

    uml_pattern_family.associations = pointer_patterns;
    uml_pattern_family.aggregations = pointer_patterns;
    uml_pattern_family.compositions = instance_patterns;
    
    uml_pattern_family.nary_pattern = 
	"${target_class.name}* ${identifier_name}";

    return uml_pattern_family;
}

uml_pattern_descriptor
initialize_rogue_wave_uml_pattern_family()
{
    uml_pattern_descriptor uml_pattern_family;

    // rogue wave's smalltalk-like collection classes
    uml_pattern_family.name = "Rogue Wave";

    uml_pattern_family.associations = rogue_wave_binary_patterns;
    uml_pattern_family.aggregations = rogue_wave_binary_patterns;
    uml_pattern_family.compositions = rogue_wave_binary_patterns;
    
    uml_pattern_family.nary_pattern = "Assn<${target_class.name}>*";

    return uml_pattern_family;
}

///////////////////////////////////////////////////////////////////////////////

//
// The function to initialize the global uml_pattern_families list.
//
list
initialize_uml_pattern_families()
{
    list uml_pattern_families = list_create("uml_pattern_descriptor", 0);

    list_append(uml_pattern_families, default_uml_pattern_family);
    list_append(uml_pattern_families, simple_uml_pattern_family);
//    list_append(uml_pattern_families, rogue_wave_uml_pattern_family);

    // STL
    // odmg's collection classes -- sets, bags, lists, and dynamic arrays 
    // nihcl's collection classes

    return uml_pattern_families;
}

//////////////////////////////////////////////////////////////////////////////

//
// The function to retrieve a particular pattern family from the global list
// by name.
//
uml_pattern_descriptor
get_family_uml_pattern_descriptor(string name)
{
    uml_pattern_descriptor uml_pattern_descriptor;
    int                    i, n;

    for (i = 0, n = list_count(uml_pattern_families); i < n; i = i + 1)
    {
	uml_pattern_descriptor = list_get(uml_pattern_families, i);
	if (name == uml_pattern_descriptor.name)
	    return uml_pattern_descriptor;
    }

    // if 'name' not found in list
    return default_uml_pattern_family;
}

