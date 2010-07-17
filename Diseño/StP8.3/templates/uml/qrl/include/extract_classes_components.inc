//
//      StP/UML
//      Confidential property of Aonix
//      Copyright (c) 1997
//      All rights reserved

//
//  Language independent file for UML code generation
//

#include "ct/rules/qrl/ct.inc"
#include "ct/qrl/include/files.inc"
#include "qrl/include/error_message.inc"
#include "qrl/include/utilities.inc"

const string CLASSES = "node[ UmlClass ${name_restrictor} && node_refs ]";
const string CLASSES_BY_TABLE = "node[ UmlClass && node_refs[file[UmlClassTable ${name_restrictor}]]]";
const string CLASSES_BY_DIAGRAM = "node[ UmlClass && node_refs[file[UmlClassDiagram ${name_restrictor}]]]";

const string PACKAGES = "node[ UmlPackage ${name_restrictor} && node_refs ]";
const string PACKAGES_BY_DIAGRAM = "node[ UmlPackage && node_refs[file[UmlClassDiagram ${name_restrictor}]]]";

const string    CLASS_TABLE_FILE_TYPE = "(UmlClassTable)";
const string    CLASS_DIAGRAM_FILE_TYPE = "(UmlClassDiagram)";

int             global_errors = 0;

boolean
extract_classes_and_packages(set class_set, set package_set)
{
    int             i, j;
    string          query;
    list            file_list, class_nodes, package_nodes;
    file            file;
    string          name_restrictor = "";

    //
    // Check file histories
    //
    file_list = list_create("file", 0);

    if (class_diagrams != "")
	file_list = file_list(CLASS_DIAGRAM_FILE_TYPE, class_diagrams);
    else if (tables != "")
	file_list = file_list(CLASS_TABLE_FILE_TYPE, tables);

    for (i = 0; i < list_count(file_list); i = i + 1)
    {
	file = list_get(file_list, i);
	global_errors = global_errors + file_history(file);
    }

    //
    // Create an id_list with the results of the query
    //

    if (classes != "")
    {
        select_from_names(class_set, query = CLASSES, classes);
    }
    else if (tables != "")
    {
        select_from_names(class_set, query = CLASSES_BY_TABLE, tables);
    }
    else if (class_diagrams != "")
    {
        select_from_names(package_set, query = PACKAGES_BY_DIAGRAM, class_diagrams);
        select_from_names(class_set, query = CLASSES_BY_DIAGRAM, class_diagrams);
    }
    else if (packages != "")
    {
        select_from_names(package_set, query = PACKAGES, packages);
        for (i = 0; i < set_count(package_set); i = i + 1)
        {
	    class_nodes = list_create("node", 0);
	    package_nodes = list_create("node", 0);
            extract_package_contents(set_get_element(package_set, i), True,
				     class_nodes, package_nodes);
            for (j = 0; j < list_count(class_nodes); j = j + 1)
            {
                set_add (class_set, list_get(class_nodes,j));
            }
        }
    }
    else
    {
        package_set = set_select(query = "node[UmlPackage & node_refs]");
        class_set = set_select(query = "node[UmlClass & node_refs]");
    }

    // if no classes or packages extracted then report an error

    if (set_count(class_set) == 0 && set_count(package_set) == 0)
    {
	name_restrictor = "&& (" +
                          names_to_oms_query_disjunction(classes + tables + class_diagrams + packages,
                                                         " ,\n\t") + 
                          ")";
	print_error("No classes or packages found from query:  " +
                    string_search_and_replace(query, "${name_restrictor}", name_restrictor),
                    NULL);
	error_message_and_count(SCRIPT_DESCRIPTION, 1);
	return False;
    }        
    return True;
}

void
select_from_names(set selection, string query, string names)
{
    int             i, j;
    list            name_list;
    list            nodes;
    string          name, eqop, name_restrictor = "";

    name_list = string_to_list(names, " ,\n\t");

    for (i = 0; i < list_count(name_list); i = i + 1)
    {
        name = list_get(name_list, i);
        if (strpbrk(name, "*") != 0)
            eqop = "$";
        else
            eqop = "=";
        name_restrictor = "& name" + eqop + "'" + to_oms_string(name) + "'";
        nodes = list_select(query);
        for (j = 0; j < list_count(nodes); j = j + 1)
        {
            set_add(selection, list_get(nodes, j));
        }
    }
    return;
}
