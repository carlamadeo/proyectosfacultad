//
//      sccsid[] = %W% %Y% %D%
//      StP/UML
//	StP Core
//      Confidential property of Aonix
//	and Aonix
//      Copyright (c) 1997
//      All rights reserved
//

//
// INHERITED OPS FUNCTIONS
//

// returns a set of all the inherited ops for class named classname
// ref_restriction is a restriction on the file type of references to operations
// Added implement_interface argument for ECR3000
set
uml_classname_inherited_ops(string classname, string ref_restriction, string ops_restriction,
    boolean implement_interface)
{
  node		cn;
  list classes;

  cn = node_from_name_type(classname, "UmlClass" );

  if (cn == NULL)
    return set_create("node"); // no such class -- return the empty set
  else
    // Added implement_interface argument for ECR3000
    return uml_class_inherited_ops(cn, ref_restriction, ops_restriction, set_create("node"),
       implement_interface);
}

// this returns all attributes and operations for the current class and all 
// those it inherits from or implements
set
uml_class_all_members(node class, string ref_restriction, string ops_restriction)
{
	set members = uml_class_local_members(class,ref_restriction,ops_restriction);
	set inh_members = uml_class_accessible_members(class,ref_restriction,
			ops_restriction,set_create("node"),NULL);

	// add in the inherited ones
	uml_members_union(members,uml_unshadowed_members(inh_members,members));

	return members;
}
//
// this returns only the operations for the current class and all 
// those it inherits from or implements
set
uml_class_all_operations(node class, string ref_restriction, string ops_restriction)
{
	set members = uml_class_local_ops(class,ref_restriction,ops_restriction);
	set inh_members = uml_class_accessible_ops(class,ref_restriction,
			ops_restriction, set_create("node"),NULL);

	// add in the inherited ones
	uml_members_union(members,uml_unshadowed_members(inh_members,members));

	return members;
}



// Added implement_interface argument for ECR3000
set
uml_class_inherited_ops(node class, string ref_restriction, string ops_restriction, set path,
    boolean implement_interface)
{
  // returns a set of all the inherited ops for a class
  // if a class has no superclasses, this set is null
  // otherwise, take the union over all direct superclasses of:
  // -- inherited ops which are not "shadowed" by locally defined ops
  // -- locally defined ops

  // ref_restriction is a restriction on the file type of references to operations
  // path is the set of classes visited so var to get to this one, we use it detect cycles
  
  set superclasses, ops;
  node superclass;
  int i;

  // initialize result set
  ops = set_create("node");

  if (set_is_member(path, class))
    // cycle detected in class-subclass graph; return
    return ops;
  
  // add class to path
  set_add(path, class);

  // get direct superclasses 
  superclasses = uml_class_direct_superclasses(class);
  for (i = 0; i < set_count(superclasses); i = i + 1) {

    // for each superclass
    superclass = set_get_element(superclasses, i);

    // Begin Change ECR3000
    // add *all* operations from interfaces, since an interface has *only* abstract operations
    if(implement_interface && 
        (selection_count("link[UmlImplements && link_refs && to_node_id == ${superclass.id} && from_node_id == ${class.id}]") > 0
       || selection_count("item[UmlStereotype && svalue == 'interface' && obj_id == ${superclass.id}]") > 0)) {
    	// collect inherited ops which are not redefined locally, and also local ops.
    	uml_members_union(ops,
    			    uml_unshadowed_members(uml_class_inherited_ops(superclass,
    				    ref_restriction, "", path, implement_interface),
    			    uml_class_local_ops(superclass, ref_restriction, "")));
    	uml_members_union(ops,
    			    uml_class_local_ops(superclass, ref_restriction, ""));
    }
    else {
    	// collect inherited ops which are not redefined locally, and also local ops.
    	uml_members_union(ops,
    			    uml_unshadowed_members(uml_class_inherited_ops(superclass,
    				    ref_restriction, ops_restriction, path, implement_interface),
    			    uml_class_local_ops(superclass, ref_restriction, "")));
    	uml_members_union(ops,
    			    uml_class_local_ops(superclass, ref_restriction, ops_restriction));
    }
    // End Change ECR3000
    
  }

  // remove class from path
  set_delete(path, class);

  return ops;
}


set
uml_class_inherited_members(node class, string ref_restriction, string ops_restriction, set path)
{
  // returns a set of all the inherited members for a class
  // if a class has no superclasses, this set is null
  // otherwise, take the union over all direct superclasses of:
  // -- inherited members which are not "shadowed" by locally defined members
  // -- locally defined members

  // ref_restriction is a restriction on the file type of references to operations
  // path is the set of classes visited so var to get to this one, we use it detect cycles
  
  set superclasses, members;
  node superclass;
  int i;

  // initialize result set
  members = set_create("node");

  if (set_is_member(path, class))
    // cycle detected in class-subclass graph; return
    return members;
  
  // add class to path
  set_add(path, class);

  // get direct superclasses 
  superclasses = uml_class_direct_superclasses(class);
  for (i = 0; i < set_count(superclasses); i = i + 1) {

    // for each superclass
    superclass = set_get_element(superclasses, i);

    // collect inherited members which are not redefined locally, and also local members.
    uml_members_union(members,
		uml_unshadowed_members(uml_class_inherited_members(superclass,
			ref_restriction, ops_restriction, path),
		uml_class_local_members(superclass, ref_restriction, "")));
    uml_members_union(members,
		uml_class_local_members(superclass, ref_restriction, ops_restriction));
    
  }

  // remove class from path
  set_delete(path, class);

  return members;
}


// set of all accessible operations from a class
// includes inherited and also instantiated parameterized classes

set
uml_class_accessible_ops(node class, string ref_restriction, string ops_restriction, set path, list lktypes)
{
  // returns a set of all the inherited ops for a class
  // if a class has no superclasses, this set is null
  // otherwise, take the union over all direct superclasses of:
  // -- inherited ops which are not "shadowed" by locally defined ops
  // -- locally defined ops

  // ref_restriction is a restriction on the file type of references to
  // operations
  // path is the set of classes visited so var to get to this one,
  // we use it detect cycles
  
  set superclasses, ops;
  node superclass;
  int i, j;
  string	lktp;

  if (lktypes == NULL)
  {
    lktypes = string_to_list(
	    "UmlImplements" + " " +
	    "UmlGeneralization",
		" ");

  }


  // initialize result set
  ops = set_create("node");

  if (set_is_member(path, class))
    // cycle detected in class-subclass graph; return
    return ops;
  
  // add class to path
  set_add(path, class);

  superclasses = set_create("node");
  for(j = 0; j < list_count(lktypes); j = j + 1)
  {
      set_union(superclasses,
		uml_class_direct_relatedclass(class, list_get(lktypes, j)));
  }

  for (i = 0; i < set_count(superclasses); i = i + 1) {

    // for each superclass
    superclass = set_get_element(superclasses, i);

    // collect accessible ops which are not redefined locally,
	// and also local ops.
    uml_members_union(ops,
		uml_unshadowed_members(uml_class_accessible_ops(superclass,
			ref_restriction, ops_restriction, path, lktypes),
		uml_class_local_ops(superclass, ref_restriction, "")));
    uml_members_union(ops,
		uml_class_local_ops(superclass, ref_restriction, ops_restriction));
    
  }

  // remove class from path
  set_delete(path, class);

  return ops;
}


set
uml_class_accessible_members(node class, string ref_restriction, string members_restriction, set path, list lktypes)
{
  // returns a set of all the inherited members for a class
  // if a class has no superclasses, this set is null
  // otherwise, take the union over all direct superclasses of:
  // -- inherited members which are not "shadowed" by locally defined members
  // -- locally defined members

  // ref_restriction is a restriction on the file type of references
  // to operations
  // path is the set of classes visited so var to get to this one,
  // we use it detect cycles
  
  set superclasses, members;
  node superclass;
  int i, j;
  string	lktp;

  if (lktypes == NULL)
  {
    lktypes = string_to_list(
	    "UmlImplements" + " " +
	    "UmlGeneralization",
		" ");

  }


  // initialize result set
  members = set_create("node");

  if (set_is_member(path, class))
    // cycle detected in class-subclass graph; return
    return members;
  
  // add class to path
  set_add(path, class);

  superclasses = set_create("node");
  for(j = 0; j < list_count(lktypes); j = j + 1)
  {
      set_union(superclasses,
		uml_class_direct_relatedclass(class, list_get(lktypes, j)));
  }

  for (i = 0; i < set_count(superclasses); i = i + 1) {

    // for each superclass
    superclass = set_get_element(superclasses, i);

    // collect accessible members which are not redefined locally, and 
	// also local members.
    uml_members_union(members,
		uml_unshadowed_members(uml_class_accessible_members(superclass,
			ref_restriction, members_restriction, path, lktypes),
			uml_class_local_members(superclass, ref_restriction, "")));
    uml_members_union(members,
		uml_class_local_members(superclass, ref_restriction, members_restriction));
    
  }

  // remove class from path
  set_delete(path, class);

  return members;
}


//
// LOCAL SEARCH FUNCTIONS
//

////////////////////////////////////////////////////////////////////////////////

const string PUBLIC_SUPERCLASS_LINKS_QUERY1 = "link[(UmlGeneralization|UmlImplements) && link_refs && from_node_id == ${class.id} ]";
const string CLASS_PUBLIC_SUPERCLASSES_QUERY1 = "node[UmlClass && in_links[PUBLIC_SUPERCLASS_LINKS_ID_LIST]]";

set
uml_class_direct_public_superclasses(node class)
{
  // returns a set of the direct public superclasses of a class
  // overly complicated because we needed to tune the queries to get performance
  // JED - simplified because for now all inheritance in UML is public
  id_list_create(PUBLIC_SUPERCLASS_LINKS_QUERY1, "PUBLIC_SUPERCLASS_LINKS_ID_LIST");
  set superclasses = set_select(CLASS_PUBLIC_SUPERCLASSES_QUERY1);
  id_list_free("PUBLIC_SUPERCLASS_LINKS_ID_LIST");
  return superclasses;
}


set
uml_class_direct_superclasses(node class)
{
    // returns a set of the direct superclasses of a class
 
    // removed check for node_refs, since link_refs are checked 2/9/97 
    return set_select("node[UmlClass && in_links[(UmlGeneralization|UmlImplements) && link_refs && from_node_id == ${class.id}]]");
}

const string CLASS_RELATEDCLASSES_QUERY = "node[UmlClass && node_refs && in_links[${lktp} && link_refs && from_node_id == ${class.id}]]";

set
uml_class_direct_relatedclass(node class, string lktp)
{
  return set_select(CLASS_RELATEDCLASSES_QUERY);
}

////////////////////////////////////////////////////////////////////////////////

const string CLASS_LOCAL_OPS_QUERY =
"node[UmlOperation && node_refs${ref_restriction} && scope_node_id == ${class.id} ${ops_restriction}]";

set
uml_class_local_ops(node class, string ref_restriction, string ops_restriction)
{
  // returns a set of the locally defined operations of a class
  
  return set_select(CLASS_LOCAL_OPS_QUERY);
}

const string CLASS_LOCAL_MEMBERS_QUERY =
"node[(UmlAttribute|UmlOperation) && node_refs${ref_restriction} && scope_node_id == ${class.id} ${attrs_restriction}]";

set
uml_class_local_members(node class, string ref_restriction, string attrs_restriction)
{
  // returns a set of the locally defined members of a class
  
  return set_select(CLASS_LOCAL_MEMBERS_QUERY);
}


//
// OPERATION SET OPERATIONS
//

set
uml_members_union(set ops1, set ops2)
{
  // destructive set operation
  // adds to ops1 distinct ops from ops2
  // op2 is equivalent to op1 iff (op2.name == op1.name && op2.sig == op1.sig)
  
  node op1, op2;
  int i, j;
  boolean distinct;

  // for each op2 in ops2
  for (i = 0; i < set_count(ops2); i = i + 1) {
    op2 = set_get_element(ops2, i);

    // see if op2 is distinct from each op1 in ops1
    for (j = 0, distinct = True;
	 distinct && j < set_count(ops1);
	 j = j + 1) {
      op1 = set_get_element(ops1, j);
      if (op1.name == op2.name && op1.sig == op2.sig)
	distinct = True;
    }

    // if distinct, then add op2 to ops1
    if (distinct)
      set_add(ops1, op2);
  }

  // return ops1
  return ops1;
}

set
uml_unshadowed_members(set ops1, set ops2)
{
  // returns a set of ops in ops1 which are not shadowed by ops in ops2
  // op2 shadows op1 iff (op2.name == op1.name && op2.sig == op1.sig)
  
  set ops;
  node op1;
  int i;
  // no shadowing ops
  if (set_count(ops2) == 0)
    return ops1;
  
  ops = set_create("node");
  
  for (i = 0; i < set_count(ops1); i = i + 1) {
    op1 = set_get_element(ops1, i);
    if (!uml_op_is_shadowed(op1, ops2))
      set_add(ops, op1);
  }
  return ops;
}

boolean
uml_op_is_shadowed(node op, set ops)
{
  // returns True iff op is shadowed by any of ops
  // op2 shadows op1 iff (op2.name == op1.name && op2.sig == op1.sig)
  
  node op2;
  int i;
  for (i = 0; i < set_count(ops); i = i + 1) {
    op2 = set_get_element(ops, i);
    if (op.name == op2.name && op.sig == op2.sig)
      return True;
  }
  return False;
}


  

// returns a set of all superclasses of this class

set
uml_class_super_classes(node class, string ref_restriction, set path)
{
  set	superclasses, classes;
  node	superclass;
  int	i;

  // initialize result set
  classes = set_create("node");

  // cycle detected in class-subclass graph; return
  if (set_is_member(path, class))
    return classes;
  
  // add class to path
  set_add(path, class);

  // get direct superclasses 
  superclasses = uml_class_direct_superclasses(class);
  set_union(classes, superclasses);
  for (i = 0; i < set_count(superclasses); i = i + 1)
  {
    // for each superclass
    superclass = set_get_element(superclasses, i);
    if (set_is_member(path, superclass) == False)
    {
	set_union(classes,
		uml_class_super_classes(superclass, ref_restriction, path));
    }
  }

  // remove class from path
  set_delete(path, class);

  return classes;
}
