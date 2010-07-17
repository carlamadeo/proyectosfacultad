//
//  data types
//

struct pattern_descriptor
{
    string          name;
    string          unqualified_single_pattern;
    string          unqualified_unordered_pattern;
    string          unqualified_ordered_pattern;
    string          qualified_single_pattern;
    string          qualified_unordered_pattern;
    string          qualified_ordered_pattern;
    string          inverse_qualified_single_pattern;
    string          inverse_qualified_unordered_pattern;
    string          inverse_qualified_ordered_pattern;
    string          double_qualified_single_pattern;
    string          double_qualified_unordered_pattern;
    string          double_qualified_ordered_pattern;
    string          nary_pattern;
};

//
//  globals
//

list pattern_families = initialize_pattern_families();


//
//  functions
//

list
initialize_pattern_families()
{
    list               pattern_families = list_create("pattern_descriptor", 0);
    pattern_descriptor pattern_family;

    pattern_family.name = "Default";

    string target                    = "${target_class.name}";
    string target_                   = target + "_";
    string target_package            = "${target_package}";
    string target_access_type        = "${target_access_type}";
    string qualifier_target_         = "${qualifier.type}_" + target_;
    string inverse_qualifier_target_ = "${inverse_qualifier.type}_" + target_;
    string double_qualifier_target_  = 
	"${qualifier.type}_${inverse_qualifier.type}_" + target_;


    pattern_family.unqualified_single_pattern = target_package + "." +
	target_access_type;
    
    pattern_family.unqualified_unordered_pattern = target_ + SET_TYPE_NAME + "." + SET_TYPE_NAME;

    pattern_family.unqualified_ordered_pattern = target_ + LIST_TYPE_NAME + "." + LIST_TYPE_NAME;


    pattern_family.qualified_single_pattern = qualifier_target_ + MAP_TYPE_NAME + "." + MAP_TYPE_NAME;

    pattern_family.qualified_unordered_pattern = qualifier_target_ + SET_TYPE_NAME + "_" + MAP_TYPE_NAME + "." + MAP_TYPE_NAME;

    pattern_family.qualified_ordered_pattern = qualifier_target_ + LIST_TYPE_NAME + "_" + MAP_TYPE_NAME + "." + MAP_TYPE_NAME;


    pattern_family.inverse_qualified_single_pattern = inverse_qualifier_target_ + TUPLE_TYPE_NAME + "." + TUPLE_TYPE_NAME;

    pattern_family.inverse_qualified_unordered_pattern = inverse_qualifier_target_ + TUPLE_TYPE_NAME + "_" + SET_TYPE_NAME + "." + SET_TYPE_NAME;

    pattern_family.inverse_qualified_ordered_pattern = inverse_qualifier_target_ + TUPLE_TYPE_NAME + "_" + LIST_TYPE_NAME + "." + LIST_TYPE_NAME;


    pattern_family.double_qualified_single_pattern = double_qualifier_target_ + TUPLE_TYPE_NAME + "_" + MAP_TYPE_NAME + "." + MAP_TYPE_NAME;

    pattern_family.double_qualified_unordered_pattern = double_qualifier_target_ + TUPLE_TYPE_NAME + "_" + SET_TYPE_NAME + "_" + MAP_TYPE_NAME + "." + MAP_TYPE_NAME;

    pattern_family.double_qualified_ordered_pattern = double_qualifier_target_ + TUPLE_TYPE_NAME + "_" + LIST_TYPE_NAME + "_" + MAP_TYPE_NAME + "." + MAP_TYPE_NAME;


    // same as unqualified_unordered_pattern
    pattern_family.nary_pattern = target_ + SET_TYPE_NAME + "." + SET_TYPE_NAME;


    list_append(pattern_families, pattern_family);


    return pattern_families;
}

pattern_descriptor
get_family_pattern_descriptor(string name)
{
    pattern_descriptor pattern_descriptor;
    int             i, n;

    for (i = 0, n = list_count(pattern_families); i < n; i = i + 1)
    {
	pattern_descriptor = list_get(pattern_families, i);
	if (name == pattern_descriptor.name)
	    return pattern_descriptor;
    }
    /* what should we do in the case we don't find the pattern family? */
    return get_pointer_pattern_descriptor();
}

pattern_descriptor
get_pointer_pattern_descriptor()
{
    pattern_descriptor pattern_descriptor;

    pattern_descriptor.unqualified_single_pattern = "${target_class.name}*";
    pattern_descriptor.unqualified_unordered_pattern = "${target_class.name}*";
    pattern_descriptor.unqualified_ordered_pattern = "${target_class.name}*";
    pattern_descriptor.qualified_single_pattern = "${target_class.name}*";
    pattern_descriptor.qualified_unordered_pattern = "${target_class.name}*";
    pattern_descriptor.qualified_ordered_pattern = "${target_class.name}*";
    pattern_descriptor.inverse_qualified_single_pattern = "${target_class.name}*";
    pattern_descriptor.inverse_qualified_unordered_pattern = "${target_class.name}*";
    pattern_descriptor.inverse_qualified_ordered_pattern = "${target_class.name}*";
    pattern_descriptor.double_qualified_single_pattern = "${target_class.name}*";
    pattern_descriptor.double_qualified_unordered_pattern = "${target_class.name}*";
    pattern_descriptor.double_qualified_ordered_pattern = "${target_class.name}*";
    pattern_descriptor.nary_pattern = "${target_class.name}*";

    return pattern_descriptor;
}

pattern_descriptor
get_instance_pattern_descriptor()
{
    pattern_descriptor pattern_descriptor;

    pattern_descriptor.unqualified_single_pattern = "${target_class.name}";
    pattern_descriptor.unqualified_unordered_pattern = "${target_class.name}";
    pattern_descriptor.unqualified_ordered_pattern = "${target_class.name}";
    pattern_descriptor.qualified_single_pattern = "${target_class.name}";
    pattern_descriptor.qualified_unordered_pattern = "${target_class.name}";
    pattern_descriptor.qualified_ordered_pattern = "${target_class.name}";
    pattern_descriptor.inverse_qualified_single_pattern = "${target_class.name}";
    pattern_descriptor.inverse_qualified_unordered_pattern = "${target_class.name}";
    pattern_descriptor.inverse_qualified_ordered_pattern = "${target_class.name}";
    pattern_descriptor.double_qualified_single_pattern = "${target_class.name}";
    pattern_descriptor.double_qualified_unordered_pattern = "${target_class.name}";
    pattern_descriptor.double_qualified_ordered_pattern = "${target_class.name}";
    pattern_descriptor.nary_pattern = "${target_class.name}";

    return pattern_descriptor;
}



