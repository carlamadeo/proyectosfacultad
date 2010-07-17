//
//      StP/UML
//      Confidential property of Aonix
//      Copyright (c) 1997
//      All rights reserved


#include "ct/rules/qrl/ct.inc"
#include "qrl/include/utilities.inc"

struct UmlAttribute
{
    // oms
    node            attribute;
    // annotations
    note            UmlAttributeAnnotation;
    list            UmlAttributeAnnotationItems;	// list<item>
};

UmlAttribute
UmlAttribute(node attribute)
{
    UmlAttribute    self;

    self.attribute = attribute;

    self.UmlAttributeAnnotation = find_by_query("note[UmlAttributeDefinition & obj_id=${attribute.id}]");

    // Language-independent items
    if (self.UmlAttributeAnnotation != NULL)
	self.UmlAttributeAnnotationItems = list_select("item[note_id=${self.UmlAttributeAnnotation.id}]");
    else
	self.UmlAttributeAnnotationItems = list_create("item", 0);

    // Language-dependent items
    list_concatenate(self.UmlAttributeAnnotationItems, 
		     UmlAttributeItems(attribute));

    return self;
}

item
UmlAttribute__GetItem(UmlAttribute self, string item_type)
{
    item            item;
    int             i, n;

    if (self.UmlAttributeAnnotationItems == NULL)
	return NULL;

    for (i = 0, n = list_count(self.UmlAttributeAnnotationItems); i < n; i = i + 1)
    {
	item = list_get(self.UmlAttributeAnnotationItems, i);
	if (item.type == item_type)
	    return item;
    }

    return NULL;
}

string
UmlAttribute__GetItemValue(UmlAttribute self, string item_type)
{
    item            item = UmlAttribute__GetItem(self, item_type);

    if (item == NULL)
	return "";

    return item.value;
}

struct UmlOperation
{
    // oms
    node            operation;
    // annotations
    note            UmlOperationAnnotation;
    list            UmlOperationAnnotationItems;	// list<item>
    // code
    note            UmlOperationCode;
};

UmlOperation
UmlOperation(node operation)
{
    UmlOperation    self;

    self.operation = operation;

    self.UmlOperationAnnotation = find_by_query("note[UmlOperationDefinition & obj_id=${operation.id}]");

    // Language-independent items
    if (self.UmlOperationAnnotation != NULL)
	self.UmlOperationAnnotationItems = list_select("item[note_id=${self.UmlOperationAnnotation.id}]");
    else
	self.UmlOperationAnnotationItems = list_create("item", 0);

    // Language-dependent items
    list_concatenate(self.UmlOperationAnnotationItems, UmlOperationItems(operation));

    self.UmlOperationCode = UmlOperationCode(operation);

    return self;
}

item
UmlOperation__GetItem(UmlOperation self, string item_type)
{
    item            item;
    int             i, n;

    if (self.UmlOperationAnnotationItems == NULL)
	return NULL;

    for (i = 0, n = list_count(self.UmlOperationAnnotationItems); i < n; i = i + 1)
    {
	item = list_get(self.UmlOperationAnnotationItems, i);
	if (item.type == item_type)
	    return item;
    }

    return NULL;
}

string
UmlOperation__GetItemValue(UmlOperation self, string item_type)
{
    item            item = UmlOperation__GetItem(self, item_type);

    if (item == NULL)
	return "";

    return item.value;
}

struct UmlRole
{
    // oms
    link            role;
    // annotations
    note            UmlRoleAnnotation;
    list            UmlRoleAnnotationItems;	// list<item>
};

UmlRole
UmlRole(link role)
{
    UmlRole         self;

    self.role = role;

    self.UmlRoleAnnotation = find_by_query("note[UmlRoleDefinition & obj_id=${role.id}]");

    // Language-independent items
    if (self.UmlRoleAnnotation != NULL)
	self.UmlRoleAnnotationItems = list_select("item[note_id=${self.UmlRoleAnnotation.id}]");
    else
	self.UmlRoleAnnotationItems = list_create("item", 0);

    // Language-dependent items
    list_concatenate(self.UmlRoleAnnotationItems, UmlRoleItems(role));

    return self;
}

item
UmlRole__GetItem(UmlRole self, string item_type)
{
    item            item;
    int             i, n;

    if (self.UmlRoleAnnotationItems == NULL)
	return NULL;

    for (i = 0, n = list_count(self.UmlRoleAnnotationItems); i < n; i = i + 1)
    {
	item = list_get(self.UmlRoleAnnotationItems, i);
	if (item.type == item_type)
	    return item;
    }

    return NULL;
}

string
UmlRole__GetItemValue(UmlRole self, string item_type)
{
    item            item = UmlRole__GetItem(self, item_type);

    if (item == NULL)
	return "";

    return item.value;
}

struct UmlAssociation
{
    // oms
    node            association;
    // parts
    UmlRole         UmlSourceRole;
    list            UmlTargetRoles;	// list<UmlRole>
    link            link_class;
    node            class;
    // annotations
    note            UmlAssociationAnnotation;
    list            UmlAssociationAnnotationItems;	// list<item>
};

UmlAssociation
UmlAssociation(node association, node class)
{
    UmlAssociation  self;
    int             i, n;
    link            role;

    self.association = association;
    self.UmlSourceRole = NULL;
    self.UmlTargetRoles = list_create("UmlRole", 0);

    list roles = list_select("link[UmlRole & to_node_id=${association.id} & link_refs]");

    for (i = 0, n = list_count(roles); i < n; i = i + 1)
    {
	role = list_get(roles, i);

	// reflexive association will have two UmlRole links to the same
	// UmlAssociation node, so one will become a TargetRole
	if (role.from_node_id == class.id && self.UmlSourceRole == NULL)
	    self.UmlSourceRole = UmlRole(role);
	else
	    list_append(self.UmlTargetRoles, UmlRole(role));
    }

    self.link_class = find_by_query("link[UmlAssociationClassLink & from_node[id=${association.id}] & link_refs]");

    if (self.link_class != NULL)
	self.class = find_by_query("node[id=${self.link_class.to_node_id} & node_refs]");
    else
	self.class = NULL;

    //  This note is not needed for now.
    //
    //        self.UmlAssociationAnnotation = find_by_query("note[UmlAssociationDefinition & obj_id=${association.id}]");

    // Language-independent items
    // (none for now)
    //
    //    if (self.UmlAssociationAnnotation != NULL)
    // 	self.UmlAssociationAnnotationItems = list_select("item[note_id=${self.UmlAssociationAnnotation.id}]");
    //     else
	self.UmlAssociationAnnotationItems = list_create("item", 0);

    // Language-dependent items
    list_concatenate(self.UmlAssociationAnnotationItems, 
		     UmlAssociationItems(association));

    return self;
}

item
UmlAssociation__GetItem(UmlAssociation self, string item_type)
{
    item            item;
    int             i, n;

    if (self.UmlAssociationAnnotationItems == NULL)
	return NULL;

    for (i = 0, n = list_count(self.UmlAssociationAnnotationItems); i < n; i = i + 1)
    {
	item = list_get(self.UmlAssociationAnnotationItems, i);
	if (item.type == item_type)
	    return item;
    }

    return NULL;
}

string
UmlAssociation__GetItemValue(UmlAssociation self, string item_type)
{
    item            item = UmlAssociation__GetItem(self, item_type);

    if (item == NULL)
	return "";

    return item.value;
}

struct UmlGeneralization
{
    // oms
    link            generalization;
    node            class;
    node            elaborated_type;
    // annotations
    note            UmlGeneralizationAnnotation;
    list            UmlGeneralizationAnnotationItems;	// list<item>
};

UmlGeneralization
UmlGeneralization(link generalization)
{
    UmlGeneralization   self;

    self.generalization = generalization;
    self.class = find_by_query("node[id=${generalization.to_node_id}]");
    self.elaborated_type = class_elaborated_type(self.class);

    // No language-independent note
    self.UmlGeneralizationAnnotation = NULL;

    // Language-independent items
     if (self.UmlGeneralizationAnnotation != NULL)
 	self.UmlGeneralizationAnnotationItems = list_select("item[note_id=${self.UmlGeneralizationAnnotation.id}]");
     else
 	self.UmlGeneralizationAnnotationItems = list_create("item", 0);

    // Language-dependent items
     list_concatenate(self.UmlGeneralizationAnnotationItems, UmlGeneralizationItems(generalization));

    return self;
}

item
UmlGeneralization__GetItem(UmlGeneralization self, string item_type)
{
    item            item;
    int             i, n;

    if (self.UmlGeneralizationAnnotationItems == NULL)
	return NULL;

    for (i = 0, n = list_count(self.UmlGeneralizationAnnotationItems); i < n; i = i + 1)
    {
	item = list_get(self.UmlGeneralizationAnnotationItems, i);
	if (item.type == item_type)
	    return item;
    }

    return NULL;
}

string
UmlGeneralization__GetItemValue(UmlGeneralization self, string item_type)
{
    item            item = UmlGeneralization__GetItem(self, item_type);

    if (item == NULL)
	return "";

    return item.value;
}

struct UmlClass
{
    // oms
    node            class;
    node            elaborated_type;
    // parts
    list            UmlAttributes;	// list<UmlAttribute>
    list            UmlOperations;	// list<UmlOperation>
    list            UmlAssociations;	// list<UmlAssociation>
    list            UmlGeneralizations;	// list<UmlGeneralization>
    // annotations
    note            UmlClassAnnotation;
    list            UmlClassAnnotationItems;	// list<item>
    list            UmlClassAnnotationNotes;	// list<note>
};

UmlClass
UmlClass(node class)
{
    UmlClass        self;
    int             i, n;
    set             elaborated_types = set_create("node");

    // the class and its elaborated type
    self.class = class;

    elaborated_types = class_elaborated_types(class);
    if (set_count(elaborated_types) > 0)
	self.elaborated_type = set_get_element(elaborated_types, 0);
    else
	self.elaborated_type = NULL;

    // its attributes
    self.UmlAttributes = list_create("UmlAttribute", 0);
    list attributes = list_select_node_sort_by_table_row("node[UmlAttribute & scope_node_id=" + class.id + " & node_refs[file[UmlClassTable]]]");
    for (i = 0, n = list_count(attributes); i < n; i = i + 1)
	list_append(self.UmlAttributes, UmlAttribute(list_get(attributes, i)));

    // its operations
    self.UmlOperations = list_create("UmlOperation", 0);
    list operations = list_select_node_sort_by_table_row("node[UmlOperation & scope_node_id=" + class.id + " & node_refs[file[UmlClassTable]]]");
    for (i = 0, n = list_count(operations); i < n; i = i + 1)
	list_append(self.UmlOperations, UmlOperation(list_get(operations, i)));

    // its associations
    self.UmlAssociations = list_create("UmlAssociation", 0);
    list associations = list_select("node[UmlAssociation & in_links[from_node_id=${class.id} & link_refs] & node_refs]");
    for (i = 0, n = list_count(associations); i < n; i = i + 1)
	list_append(self.UmlAssociations, UmlAssociation(list_get(associations, i), class));

    // its generalizations
    self.UmlGeneralizations = list_create("UmlGeneralization", 0);
//!! need further tests on 'order_generalizations'
//    list generalizations = order_generalizations(list_select("link[UmlGeneralization & from_node_id=${class.id} & link_refs]"));
    list generalizations = list_select("link[UmlGeneralization & from_node_id=${class.id} & link_refs]");
    // A Refines link originating from a class which is not an 
    // instantiated class is interpreted as being inheritance
    if (self.elaborated_type == NULL ||
	self.elaborated_type.type != "UmlInstantiatedClass")
	list_concatenate(generalizations, list_select("link[UmlRefines & from_node_id=${class.id} & link_refs]"));
    // Allow each language to add to the generalizations as appropriate
    // e.g. C++ will use UmlImplements link as a generalization
    LanguageUmlGeneralizations(generalizations, class);
    for (i = 0, n = list_count(generalizations); i < n; i = i + 1)
	list_append(self.UmlGeneralizations, UmlGeneralization(list_get(generalizations, i)));

    // its annotations
    self.UmlClassAnnotation = find_by_query("note[UmlClassDefinition & obj_id=${class.id}]");
    if (self.UmlClassAnnotation != NULL)
	self.UmlClassAnnotationItems = list_select("item[note_id=${self.UmlClassAnnotation.id}]");
    else
	self.UmlClassAnnotationItems = list_create("item", 0);
    list_concatenate(self.UmlClassAnnotationItems, list_select("item[obj_id=${class.id} & note[UmlExtensibilityDefinition]]"));
    list_concatenate(self.UmlClassAnnotationItems, UmlClassItems(class));
    self.UmlClassAnnotationNotes = list_select("note[obj_id=${class.id}]");

    return self;
}

note
UmlClass__GetNote(UmlClass self, string note_type)
{
    note            note;
    int             i, n;

    if (self.UmlClassAnnotationNotes == NULL)
	return NULL;

    for (i = 0, n = list_count(self.UmlClassAnnotationNotes); i < n; i = i + 1)
    {
	note = list_get(self.UmlClassAnnotationNotes, i);
	if (note.type == note_type)
	    return note;
    }

    return NULL;
}

string
UmlClass__GetNoteDescription(UmlClass self, string note_type)
{
    note            note = UmlClass__GetNote(self, note_type);

    if (note == NULL)
	return "";

    return note.desc;
}

item
UmlClass__GetItem(UmlClass self, string item_type)
{
    item            item;
    int             i, n;

    if (self.UmlClassAnnotationItems == NULL)
	return NULL;

    for (i = 0, n = list_count(self.UmlClassAnnotationItems); i < n; i = i + 1)
    {
	item = list_get(self.UmlClassAnnotationItems, i);
	if (item.type == item_type)
	    return item;
    }

    return NULL;
}

string
UmlClass__GetItemValue(UmlClass self, string item_type)
{
    item            item = UmlClass__GetItem(self, item_type);

    if (item == NULL)
	return "";

    return item.value;
}

struct UmlPackage
{
    // oms
    node            package;
    // contents
    list            UmlContentClasses;         // list<UmlClass>
    list            UmlContentPackages;        // list<UmlPackage>

    // annotations
    note            UmlPackageAnnotation;
    list            UmlPackageAnnotationItems;	// list<item>
    list            UmlPackageAnnotationNotes;	// list<note>
};

UmlPackage
UmlPackage(node package)
{
    UmlPackage      self;
    int             i, n;
    list	    class_nodes, package_nodes;

    self.package = package;
    self.UmlContentClasses = list_create("UmlClass", 0);
    self.UmlContentPackages = list_create("UmlPackage", 0);

    // its contents
    class_nodes = list_create("node", 0);
    package_nodes = list_create("node", 0);
    extract_package_contents(package, False, class_nodes, package_nodes);
    for (i = 0, n = list_count(class_nodes); i < n; i = i + 1)
    {
      list_append(self.UmlContentClasses, UmlClass(list_get(class_nodes, i)));
    }
    for (i = 0, n = list_count(package_nodes); i < n; i = i + 1)
    {
      list_append(self.UmlContentPackages, UmlPackage(list_get(package_nodes, i)));
    }

    // its annotations
    self.UmlPackageAnnotation = find_by_query("note[UmlPackageDefinition & obj_id=${package.id}]");
    if (self.UmlPackageAnnotation != NULL)
	self.UmlPackageAnnotationItems = list_select("item[note_id=${self.UmlPackageAnnotation.id}]");
    else
	self.UmlPackageAnnotationItems = list_create("item", 0);
    list_concatenate(self.UmlPackageAnnotationItems, list_select("item[obj_id=${package.id} & note[UmlExtensibilityDefinition]]"));
    list_concatenate(self.UmlPackageAnnotationItems, UmlPackageItems(package));
    self.UmlPackageAnnotationNotes = list_select("note[obj_id=${package.id}]");

    return self;
}
	
note
UmlPackage__GetNote(UmlPackage self, string note_type)
{
    note            note;
    int             i, n;

    if (self.UmlPackageAnnotationNotes == NULL)
	return NULL;

    for (i = 0, n = list_count(self.UmlPackageAnnotationNotes); i < n; i = i + 1)
    {
	note = list_get(self.UmlPackageAnnotationNotes, i);
	if (note.type == note_type)
	    return note;
    }

    return NULL;
}

string
UmlPackage__GetNoteDescription(UmlPackage self, string note_type)
{
    note            note = UmlPackage__GetNote(self, note_type);

    if (note == NULL)
	return "";

    return note.desc;
}

item
UmlPackage__GetItem(UmlPackage self, string item_type)
{
    item            item;
    int             i, n;

    if (self.UmlPackageAnnotationItems == NULL)
	return NULL;

    for (i = 0, n = list_count(self.UmlPackageAnnotationItems); i < n; i = i + 1)
    {
	item = list_get(self.UmlPackageAnnotationItems, i);
	if (item.type == item_type)
	    return item;
    }

    return NULL;
}

string
UmlPackage__GetItemValue(UmlPackage self, string item_type)
{
    item            item = UmlPackage__GetItem(self, item_type);

    if (item == NULL)
	return "";

    return item.value;
}



//
// the oo data model of a class represented in-memory using qrl structures
// built by using a small number of large queries to build a client "view,"
// rather than a large number of small queries on demand
//

list
order_generalizations(list supers)
{
    //
    // if this is a join class and if there exists a diagram where the join
    // class and all of its generalizations appear on a single diagram, then use
    // the layout on that diagram to determine the ordering of generalizations
    // (this is a work-around for the current lack of representation of such
    // information)
    //
    link            super;
    set             ids;
    list            diagrams;
    file            diagram;
    string          link_ref_restrictor;
    link_ref        reference;
    int             i, n;

    if (list_count(supers) <= 1)
	return supers;

    link_ref_restrictor = "link_refs[link_id=" + list_to_string(project_id_list_from_link_list(supers), "] & link_refs[link_id=") + "]";
    diagrams = list_select("file[UmlClassDiagram & ${link_ref_restrictor}]");
    if (list_count(diagrams) > 0)
    {
	diagram = list_get(diagrams, 0);	// select an arbitrary one --
						// can we make this more
						// restrictive?  maybe we
						// could restrict it to the
						// generalization viewpoint? 

	//
	// ok, so we found a diagram with all the required classes.  now,
	// simply find the first (random) reference of each and sort them by
	// their x coords
	//
	ids = set_create("int");
	for (i = 0, n = list_count(supers); i < n; i = i + 1)
	{
	    super = list_get(supers, i);
	    reference = find_by_query("link_ref[file_id=${diagram.id} & link_id=${super.id}]");
	    set_add(ids, reference.id);
	}
	// this is a trick to instantiate an idlist w/o a query and sort
	id_list_update("SUPER_REFS", "link_ref", ids, NULL);
	supers = list_select("link[link_refs[SUPER_REFS]] sort by link_refs[SUPER_REFS].to_node_ref.xcoord");
	id_list_free("SUPER_REFS");
    }
    return supers;
}

/**************************************** 
          TEST
list
UmlAttributeItems(node attribute)
{
    return list_create("item", 0); 
}

list
UmlOperationItems(node operation)
{
    return list_create("item", 0); 
}

note
UmlOperationCode(node operation)
{
    return NULL; 
}

list
UmlRoleItems(link role)
{
    return list_create("item", 0); 
}

list
UmlAssociationItems(node association)
{
    return list_create("item", 0); 
}

list
UmlGeneralizationItems(link generalization)
{
    return list_create("item", 0);
}

list
UmlClassItems(node class)
{
    return list_create("item", 0); 
}

list
UmlPackageItems(node package)
{
    return list_create("item", 0); 
}

int
main()
{
	int t1, t2;

	t1 = time_now();
	UmlClass c = UmlClass(find_by_query("node[UmlClass&name='test'&node_refs]"));
	t2 = time_now();
	print_line(c);
	print_line((t2 - t1) + " seconds");

        return 0;
}
    END TEST
*************************************************************/
