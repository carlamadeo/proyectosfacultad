///////////////////////////////////////////////////////////
//
//
//      StP/UML
//      Confidential property of Aonix
//      Copyright (c) 1998
//      All rights reserved

//
//  Handling of library unit_names for Ada_95 code generation from UML
//  and ObjectAda navigations
//

node
outermost_package(node object)
{
  node    p;
  list    visited = list_create("int",0);
  list    containers;

  p = object;

  while ((containers = uml_get_containing_packages(p)) != NULL &&
	 list_count(containers) > 0)
    {
       if (list_count(containers)>1)
 	{
 	  print_error("Object '" + object.name + "' is contained by more than one package.");
 	  break;
 	}
      p = list_get(containers, 0);
       if (list_find(visited,0,p.id) != list_count(visited))
 	{
 	  // circularity detected
 	  print_error("Circular containment found for object '" +
 		      object.name + "'. Cannot determine fully qualified name.");
 	  break;
 	}
       else
 	list_append(visited,p.id);
    }

  if (p == object && p.type != "UmlPackage")
    {
      ide_warning("  Warning: Object '" + object.name + "' is referred to but it is not contained in any package.");
      return NULL;
    }
  else
    return p;
}

string
package_name (string name)
{
    return PACKAGE_NAME_PREFIX + name + PACKAGE_NAME_SUFFIX;
}

node
package_parent(node package)
{
  link  dependency;
  node  parent;

  dependency = find_by_query("link[UmlDependency && link_refs && items[UmlStereotype && (value = 'public child' || value = 'private child')] && from_node_id=${package.id}]");
  
  if (dependency != NULL &&
      (parent = find_by_query("node[${dependency.to_node_id}]")) != NULL)
    return parent;
  else 
    return NULL;
}

node
package_private_parent(node package)
{
  link  dependency;
  node  parent;

  dependency = find_by_query("link[UmlDependency && link_refs && items[UmlStereotype && value = 'private child'] && from_node_id=${package.id}]");

  if (dependency != NULL &&
      (parent = find_by_query("node[${dependency.to_node_id}]")) != NULL)
    return parent;
  else 
    return NULL;
}

boolean
library_unit_private(node package)
{
  package = outermost_package(package);
  return (package_private_parent(package) != NULL);
}

string
library_unit_name(node package)
{
  node parent;
  string package_name;

  if (package == NULL)
    return "";

  package = outermost_package(package);

  if (package == NULL)
    return "";

  package_name = package_name(package.name);

  if ((parent = package_parent(package)) != NULL)
    return (library_unit_name(parent) + "." + package_name);
  else
    return package_name;
}

string
library_unit_parent_name(node package)
{
  node  parent;

  if (package == NULL)
    return "";

  package = outermost_package(package);

  if ((parent = package_parent(package)) != NULL)
    return library_unit_name(parent);
  else
    return "";
}
