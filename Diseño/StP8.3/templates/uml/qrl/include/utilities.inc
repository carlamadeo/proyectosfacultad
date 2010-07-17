//
//      StP/UML
//      Confidential property of Aonix
//      Copyright (c) 1997
//      All rights reserved

//
//  this wouldn't be so necessary if it weren't for the
//  case of link attributes, which are classes with derived names in some
//  instances
//
string
class_base_name(node class)
{
    const string    LINK_CLASS_NAME = "node[id = ${class.scope_node_id}]";
    node            scope_node;
    list            list;

    if (class == NULL)
	return "";

    if (class.name != "")
	return class.name;

    if (list_count(list = list_select(LINK_CLASS_NAME)) == 0)
	return "";

    scope_node = list_get(list, 0);
    return scope_node.name;
}

string
class_qualified_name(node class)
{
    node name_class;
    name_class = class_surrogate_node(class);

    return language_build_qualified_name(
	language_make_scope_list(name_class), 
	class_base_name(name_class));
}


list
class_base_name_list_from_class_list(list classes)
{
    int             i;
    node            class;
    list            names = list_create("string", 0);

    for (i = 0; i < list_count(classes); i = i + 1)
    {
	class = list_get(classes, i);
	list_append(names, class_base_name(class));
    }
    return names;
}

list
class_base_name_list_from_class_set(set classes)
{
    int             i;
    node            class;
    list            names = list_create("string", 0);

    for (i = 0; i < set_count(classes); i = i + 1)
    {
	class = set_get_element(classes, i);
	list_append(names, class_base_name(class));
    }
    return names;
}

string
attribute_type(node member)
{
    const string    ATTRIBUTE_TYPE = "item[obj_id = ${member.id} && note[UmlAttributeDefinition] && UmlAttributeType]";
    list            list;
    item            item;

    if (member == NULL ||
	list_count(list = list_select(ATTRIBUTE_TYPE)) == 0)
	return "";

    item = list_get(list, 0);
    return item.value;
}

set
class_elaborated_types(node class)
{
    if (class == NULL)
	return NULL;

    return set_select("node[(UmlParameterizedClass|UmlInstantiatedClass) & name='" + to_oms_string(class.name) + "' & sig='" + to_oms_string(class.sig) + "' & scope_node_id=${class.scope_node_id} & node_refs]");
}

node
class_elaborated_type(node class)
{
    if (class == NULL)
	return NULL;

    return find_by_query("node[(UmlParameterizedClass|UmlInstantiatedClass) & name='" + to_oms_string(class.name) + "' & sig='" + to_oms_string(class.sig) + "' & scope_node_id=${class.scope_node_id} & node_refs]");
}

node
class_surrogate_node(node class)
{
    // This function is used when one class node should be substituted 
    // for another.
    
    node elaborated_node, surrogate_node, param_node;
    link refines_link;
    
    elaborated_node = class_elaborated_type(class);
    
    if (elaborated_node != NULL &&
	elaborated_node.type == "UmlInstantiatedClass") {
	refines_link = find_by_query("link[UmlRefines & from_node_id=${class.id} & items[UmlStereotype & value='bind']]");
	if (refines_link == NULL) {
	    // This means 'class' is an anonymous instantiation.
	    // The surrogate_node will be the corresponding 
	    // parameterized class node.
	    // Ideally, the fully qualified name of the corresponding 
	    // parameterized class would be available in the 
	    // instantiated class node, but it is not right now.  
	    // Since proper scoping is not yet implemented in uclassd,
	    // class names in StP are unique, and we can just look for
	    // a parameterized class with the same name as the 
	    // instantiation.
	    surrogate_node = find_by_query("node[UmlClass & name = '${class.name}' & sig = '']");
	    param_node = find_by_query("node[UmlParameterizedClass & name = '${class.name}']");
	    if (surrogate_node == NULL || param_node == NULL) {
		ide_warning("  Warning: Anonymous instantiated class '" + class.name + "' has no corresponding parameterized class.");
		return class;
	    }
	    else
		return surrogate_node;
	}
    }

    // No substitution necessary if this return is executed.
    return class;
}

void
extract_package_contents(node package, boolean explode, list classes, list packages)
{
    const string class_query = "node[UmlClass && node_refs && in_links[UmlContains && from_node_id = ${package.id} && link_refs]]";
    const string package_query = "node[UmlPackage && node_refs && in_links[UmlContains && from_node_id = ${package.id} && link_refs]]";
    node child;

    if (package == NULL)
      return;
    
    for_each_in_select(class_query, child)
    {
        if (child.name == "" || (list_find(classes, 0, child) < list_count(classes)))
            continue;
        list_append(classes, child);
    }
    for_each_in_select(package_query, child)
    {
      if (child.name == "" || (list_find(packages, 0, child) < list_count(packages)))
	continue;
      list_append(packages, child);
      if (explode)
	extract_package_contents(child, explode, classes, packages);
    }
    return;
}



