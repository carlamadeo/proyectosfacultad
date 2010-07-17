//
//      StP/UML 
//      Confidential property of Aonix
//      Copyright (c) 1997
//      All rights reserved

#include "uml/qrl/code_gen/cxx/association_patterns.inc"
#include "uml/qrl/code_gen/java/association_patterns.inc"
#include "uml/qrl/code_gen/idl/association_patterns.inc"
// ECR 4052
#include "uml/rules/qrl/oper_parse.inc"
#include "uml/rules/qrl/uml_optional_dependencies.inc"

list
UmlCxxRolePatternFamilyValuesFunc(oae_item oae_item)
{
    list            uml_pattern_family_names = list_create("string", 0);
    uml_pattern_descriptor uml_pattern_descriptor;
    int             i, n;

    for (i = 0, n = list_count(uml_pattern_families); i < n; i = i + 1)
    {
	uml_pattern_descriptor = list_get(uml_pattern_families, i);
	list_append(uml_pattern_family_names, uml_pattern_descriptor.name);
    }
    return uml_pattern_family_names;
}

list
UmlJavaRolePatternFamilyValuesFunc(oae_item oae_item)
{
    list uml_pattern_family_names = list_create("string", 0);
    uml_java_pattern_descriptor uml_pattern_descriptor;
    int             i, n;

    for (i = 0, n = list_count(uml_java_pattern_families); i < n; i = i + 1)
    {
	uml_pattern_descriptor = list_get(uml_java_pattern_families, i);
	list_append(uml_pattern_family_names, uml_pattern_descriptor.name);
    }
    return uml_pattern_family_names;
}

list
UmlIDLRolePatternFamilyValuesFunc(oae_item oae_item)
{
    list            uml_pattern_family_names = list_create("string", 0);
    uml_idl_pattern_descriptor uml_pattern_descriptor;
    int             i, n;

    for (i = 0, n = list_count(uml_idl_pattern_families); i < n; i = i + 1)
    {
        uml_pattern_descriptor = list_get(uml_idl_pattern_families, i);
        list_append(uml_pattern_family_names, uml_pattern_descriptor.name);
    }
    return uml_pattern_family_names;
}








