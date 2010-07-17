
list
java__initialize_pattern_families()
{
    list            pattern_families = list_create("pattern_descriptor", 0);
    pattern_descriptor pattern_family;

    pattern_family.name = "Generic";
    pattern_family.unqualified_single_pattern = "Link ${identifier_name}";
    pattern_family.unqualified_unordered_pattern = "Set ${identifier_name}";
    pattern_family.unqualified_ordered_pattern = "List ${identifier_name}";
    pattern_family.qualified_single_pattern = "Link ${identifier_name}";
    pattern_family.qualified_unordered_pattern = "Set ${identifier_name}";
    pattern_family.qualified_ordered_pattern = "List ${identifier_name}";
    pattern_family.inverse_qualified_single_pattern = "Link ${identifier_name}";
    pattern_family.inverse_qualified_unordered_pattern = "Set ${identifier_name}";
    pattern_family.inverse_qualified_ordered_pattern = "List ${identifier_name}";
    pattern_family.double_qualified_single_pattern = "Link ${identifier_name}";
    pattern_family.double_qualified_unordered_pattern = "Set ${identifier_name}";
    pattern_family.double_qualified_ordered_pattern = "List ${identifier_name}";
    pattern_family.nary_pattern = "Link ${identifier_name}";
    list_append(pattern_families, pattern_family);

    return pattern_families;
}

list            java__pattern_families = java__initialize_pattern_families();

pattern_descriptor
java__get_family_pattern_descriptor(string name)
{
    pattern_descriptor pattern_descriptor;
    int             i, n;

    for (i = 0, n = list_count(java__pattern_families); i < n; i = i + 1)
    {
	pattern_descriptor = list_get(java__pattern_families, i);
	if (name == pattern_descriptor.name)
	    return pattern_descriptor;
    }
    return java__get_instance_pattern_descriptor();
}

pattern_descriptor
java__get_instance_pattern_descriptor()
{
    pattern_descriptor pattern_descriptor;

    pattern_descriptor.unqualified_single_pattern = "${target_class.name} ${identifier_name}";
    pattern_descriptor.unqualified_unordered_pattern = "${target_class.name} ${identifier_name}${explicit_multiplicity}";
    pattern_descriptor.unqualified_ordered_pattern = "${target_class.name} ${identifier_name}${explicit_multiplicity}";
    pattern_descriptor.qualified_single_pattern = "${target_class.name} ${identifier_name}";
    pattern_descriptor.qualified_unordered_pattern = "${target_class.name} ${identifier_name}${explicit_multiplicity}";
    pattern_descriptor.qualified_ordered_pattern = "${target_class.name} ${identifier_name}${explicit_multiplicity}";
    pattern_descriptor.inverse_qualified_single_pattern = "${target_class.name} ${identifier_name}";
    pattern_descriptor.inverse_qualified_unordered_pattern = "${target_class.name} ${identifier_name}${explicit_multiplicity}";
    pattern_descriptor.inverse_qualified_ordered_pattern = "${target_class.name} ${identifier_name}${explicit_multiplicity}";
    pattern_descriptor.double_qualified_single_pattern = "${target_class.name} ${identifier_name}";
    pattern_descriptor.double_qualified_unordered_pattern = "${target_class.name} ${identifier_name}${explicit_multiplicity}";
    pattern_descriptor.double_qualified_ordered_pattern = "${target_class.name} ${identifier_name}${explicit_multiplicity}";
    pattern_descriptor.nary_pattern = "${target_class.name} ${identifier_name}";

    return pattern_descriptor;
}


