
#include "ct/qrl/include/cxx_association_patterns.inc"
#include "ct/qrl/include/java_association_patterns.inc"

list
CxxRoleTemplateFamilyValuesFunc(oae_item oae_item)
{
    list            pattern_family_names = list_create("string", 0);
    pattern_descriptor pattern_descriptor;
    int             i, n;

    for (i = 0, n = list_count(pattern_families); i < n; i = i + 1)
    {
	pattern_descriptor = list_get(pattern_families, i);
	list_append(pattern_family_names, pattern_descriptor.name);
    }
    return pattern_family_names;
}

list
JavaRoleTemplateFamilyValuesFunc(oae_item oae_item)
{
    list            pattern_family_names = list_create("string", 0);
    pattern_descriptor pattern_descriptor;
    int             i, n;

    for (i = 0, n = list_count(java__pattern_families); i < n; i = i + 1)
    {
	pattern_descriptor = list_get(java__pattern_families, i);
	list_append(pattern_family_names, pattern_descriptor.name);
    }
    return pattern_family_names;
}
