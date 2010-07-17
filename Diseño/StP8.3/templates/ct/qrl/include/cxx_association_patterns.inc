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

list
initialize_pattern_families()
{
    list            pattern_families = list_create("pattern_descriptor", 0);
    pattern_descriptor pattern_family;

    /* generic collection classes, originated in OMTool */
    pattern_family.name = "Generic";
    pattern_family.unqualified_single_pattern = "Link<${target_class.name}> ${identifier_name}";
    pattern_family.unqualified_unordered_pattern = "Set<Link<${target_class.name}> > ${identifier_name}";
    pattern_family.unqualified_ordered_pattern = "List<Link<${target_class.name}> > ${identifier_name}";
    pattern_family.qualified_single_pattern = "Dict<${qualifier.type},Link<${target_class.name}> > ${identifier_name}";
    pattern_family.qualified_unordered_pattern = "Dict<${qualifier.type},Set<Link<${target_class.name}> > > ${identifier_name}";
    pattern_family.qualified_ordered_pattern = "BList<${qualifier.type},Link<${target_class.name}> > ${identifier_name}";
    pattern_family.inverse_qualified_single_pattern = "Link<${target_class.name}> ${identifier_name}";
    pattern_family.inverse_qualified_unordered_pattern = "BAssn<Link<${target_class.name}>,${inverse_qualifier.type}> ${identifier_name}";
    pattern_family.inverse_qualified_ordered_pattern = "BList<Link<${target_class.name}>,${inverse_qualifier.type}> ${identifier_name}";
    pattern_family.double_qualified_single_pattern = "Dict<${qualifier.type},Tuple<Link<${target_class.name}>,${inverse_qualifier.type}> > ${identifier_name}";
    pattern_family.double_qualified_unordered_pattern = "Dict<${qualifier.type},Set<Tuple<Link<${target_class.name}>,${inverse_qualifier.type}> > > ${identifier_name}";
    pattern_family.double_qualified_ordered_pattern = "List<Tuple<${qualifier.type},Link<${target_class.name}>,${inverse_qualifier.type}> > ${identifier_name}";
    pattern_family.nary_pattern = "Assn<${target_class.name}>* ${identifier_name}";
    list_append(pattern_families, pattern_family);

    /* rogue wave's smalltalk-like collection classes */
//    pattern_family.name = "Rogue";
//    pattern_family.unqualified_single_pattern = "Link<${target_class.name}>";
//    pattern_family.unqualified_unordered_pattern = "Set<Link<${target_class.name}> >";
//    pattern_family.unqualified_ordered_pattern = "List<Link<${target_class.name}> >";
//    pattern_family.qualified_single_pattern = "Dict<${qualifier.type},Link<${target_class.name}> >";
//    pattern_family.qualified_unordered_pattern = "Dict<${qualifier.type},Set<Link<${target_class.name}> > >";
//    pattern_family.qualified_ordered_pattern = "BList<${qualifier.type},Link<${target_class.name}> >";
//    pattern_family.inverse_qualified_single_pattern = "Link<${target_class.name}>";
//    pattern_family.inverse_qualified_unordered_pattern = "BAssn<Link<${target_class.name}>,${inverse_qualifier.type}>";
//    pattern_family.inverse_qualified_ordered_pattern = "BList<Link<${target_class.name}>,${inverse_qualifier.type}>";
//    pattern_family.double_qualified_single_pattern = "Dict<${qualifier.type},Tuple<Link<${target_class.name}>,${inverse_qualifier.type}> >";
//    pattern_family.double_qualified_unordered_pattern = "Dict<${qualifier.type},Set<Tuple<Link<${target_class.name}>,${inverse_qualifier.type}> > >";
//    pattern_family.double_qualified_ordered_pattern = "List<Tuple<${qualifier.type},Link<${target_class.name}>,${inverse_qualifier.type}> >";
//    pattern_family.nary_pattern = "Assn<${target_class.name}>*";
//    list_append(pattern_families, pattern_family);

    /* odmg's collection classes -- sets, bags, lists, and dynamic arrays */
    /* nihcl's collection classes */

    return pattern_families;
}

list            pattern_families = initialize_pattern_families();

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

    pattern_descriptor.unqualified_single_pattern = "${target_class.name}* ${identifier_name}";
    pattern_descriptor.unqualified_unordered_pattern = "${target_class.name}* ${identifier_name}";
    pattern_descriptor.unqualified_ordered_pattern = "${target_class.name}* ${identifier_name}";
    pattern_descriptor.qualified_single_pattern = "${target_class.name}* ${identifier_name}";
    pattern_descriptor.qualified_unordered_pattern = "${target_class.name}* ${identifier_name}";
    pattern_descriptor.qualified_ordered_pattern = "${target_class.name}* ${identifier_name}";
    pattern_descriptor.inverse_qualified_single_pattern = "${target_class.name}* ${identifier_name}";
    pattern_descriptor.inverse_qualified_unordered_pattern = "${target_class.name}* ${identifier_name}";
    pattern_descriptor.inverse_qualified_ordered_pattern = "${target_class.name}* ${identifier_name}";
    pattern_descriptor.double_qualified_single_pattern = "${target_class.name}* ${identifier_name}";
    pattern_descriptor.double_qualified_unordered_pattern = "${target_class.name}* ${identifier_name}";
    pattern_descriptor.double_qualified_ordered_pattern = "${target_class.name}* ${identifier_name}";
    pattern_descriptor.nary_pattern = "${target_class.name}* ${identifier_name}";

    return pattern_descriptor;
}

pattern_descriptor
get_instance_pattern_descriptor()
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


