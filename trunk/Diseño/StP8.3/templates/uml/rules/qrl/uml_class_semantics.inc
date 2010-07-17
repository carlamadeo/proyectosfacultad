//
//      sccsid[] = %W% %Y% %D%
//      StP/UML
//      Confidential property of Aonix
//      Copyright (c) 1997
//      All rights reserved
//
// Class semantics checks

#include "rules/qrl/uml_stereotypes.inc"


string member_format_error(node class, node member, string err)
{
	node owner;
	string owner_name;
	string member_str;
	string inherited_str;

	owner = find_by_query("node[type == UmlClass && id == " + member.scope_node_id + "]");
	if (NULL == owner) {
		// shouldn't happen
		owner_name = "<Anonymous>::";
		inherited_str = "";
	} else {
		owner_name = uml_class_qualified_name(owner) + "::";
		if (owner.id == class.id) {
			// a local memeber
			inherited_str = "Local member ";
		} else {
			inherited_str = "Inherited member ";
		}
	}

	if (NULL == member.sig || "" == member.sig) {
		member_str = member.name;
	} else {
		member_str = member.name + "(" + member.sig + ")";
	}


	return (inherited_str + owner_name + member_str + " : " + err);
}


// 4.3.1
// If a class has the stereotype "type" or "interface", then the
// following must be true:
//	2 - Any operations must be abstract
//	3 - Any operations must have public visibility
//	4 - Any classes inherited from or implemented must meet the above
//	    criteria.
// This is implemented by getting all operations/attributes (including
// those inherited) and then checking the whole bunch all at once.
// NOTE: errlist is assumed to be a pre-initialized list of strings.
//


boolean 
uml_check_interface_semantics(node class, string ref_restriction, 
                             string member_ref_restriction, list errlist)
{
    int i;
    node member;
    item the_item;
    int errors = 0;

    //list_append(errlist,"Checking <<interface>> semantics for '" + class.name + "'...");
    print_message("Checking <<interface>> semantics for '" + class.name + "'...");

    set members = uml_class_all_members(class, ref_restriction, member_ref_restriction);
    int count = set_count(members);

    for (i = 0; i < count; i++)
    {
        // for each member
        member = set_get_element(members, i);

        if ("UmlOperation" == member.type)
        {
            // make sure it's abstract
            // It must have an item UmlOperationIsAbstract with the value
            // 'True'
            the_item = find_by_query("item[type == UmlOperationIsAbstract && obj_id == " +
                                      member.id + "]");
            if (NULL != the_item)
            {
                if (the_item.value != "True")
                {
                    // this operation is not abstract
                    list_append(errlist,
                            member_format_error(class, member,
                            "Operation is not abstract"));
                    errors++;
                }
            }
            the_item = find_by_query("item[type == UmlMemberVisibility && obj_id == "
                                      + member.id + "]");
            if (the_item != NULL)
            {
                if (the_item.value != "public")
                {
                    // this operation is not abstract
                   list_append(errlist,
                           member_format_error(class, member,
                           "Operation does not have 'public' visibility"));
                   errors++;
                }
            }
        }
        else
        {
            list_append(errlist,
                        member_format_error(class, member,
                        "Attribute cannot be included in Interface"));
            errors++;

/*
            // to be a class member, the UmlMemeberIsClass item must be
            // present and it must have the value of 'True'
            the_item = find_by_query("item[type == UmlMemberIsClass && obj_id == " +
                                      member.id + " && value == 'True']");
            if (NULL == the_item)
            {
                // this attribute is not a class attribute
                list_append(errlist,
                            member_format_error(class, member,
                            "Attribute is not class-scoped"));
                errors++;
            }
*/
        }
    }
    if (errors > 0)
    {
// put an explanation of the failures in the errlist
//list_append(errlist,"Classes with the stereotype <<interface>> must meet the following criteria:");
//list_append(errlist,"1 - Interface cannot have attributes");
//list_append(errlist,"2 - Any operations must be abstract");
//list_append(errlist,"3 - Any operations must have public visibility");
//list_append(errlist,"4 - Any classes inherited from or implemented must meet the above criteria.");
    return False;
    }
    return True;
}

boolean
uml_check_type_semantics(node class, string ref_restriction,
                             string member_ref_restriction, list errlist)
{
    int i;
    node member;
    item the_item;
    int errors = 0;

    //list_append(errlist,"Checking <<type>> semantics for '" + class.name + "'...");
    print_message("Checking <<type>> semantics for '" + class.name + "'...");

    set members = uml_class_all_members(class, ref_restriction, member_ref_restriction);
    int count = set_count(members);

    for (i = 0; i < count; i++)
    {
        // for each member
        member = set_get_element(members, i);

        if ("UmlOperation" == member.type)
        {
            // make sure it's abstract
            // It must have an item UmlOperationIsAbstract with the value
            // 'True'
            the_item = find_by_query("item[type == UmlOperationIsAbstract && obj_id == " +
                                      member.id + "]");
            if (NULL != the_item)
            {
                if (the_item.value != "True")
                {
                    // this operation is not abstract
                    list_append(errlist,
                            member_format_error(class, member,
                            "Operation is not abstract"));
                    errors++;
                }
            }
            the_item = find_by_query("item[type == UmlMemberVisibility && obj_id == "
                                      + member.id + "]");
            if (the_item != NULL)
            {
                if (the_item.value != "public")
                {
                    // this operation is not abstract
                   list_append(errlist,
                           member_format_error(class, member,
                           "Operation does not have 'public' visibility"));
                   errors++;
                }
            }
        }
    }
    if (errors > 0)
    {
// put an explanation of the failures in the errlist
//list_append(errlist,"Classes with the stereotype <<interface>> must meet the following criteria:");
//list_append(errlist,"1 - Interface cannot have attributes");
//list_append(errlist,"2 - Any operations must be abstract");
//list_append(errlist,"3 - Any operations must have public visibility");
//list_append(errlist,"4 - Any classes inherited from or implemented must meet the above criteria.");
    return False;
    }
    return True;
}


// 4.3.2
// If a class has the stereotype "utility", then the following must be
// true:
//	1 - Any attributes must be class attributes
//	2 - Any operations must be class operations
// NOTE: errlist is assumed to be a pre-initialized list of strings.
//

boolean uml_check_utility_semantics(node class, string ref_restriction, string member_ref_restriction, list errlist)
{
	int i;
	node member;
	item the_item;
	int errors = 0;

	list_append(errlist,"Checking <<utility>> semantics for '" + class.name + "'...");


	set members = uml_class_local_members(class, ref_restriction, member_ref_restriction);
	int count = set_count(members);

	for (i = 0; i < count; i++) {
		// for each member
		member = set_get_element(members, i);

		the_item = find_by_query("item[type == UmlMemberIsClass && obj_id == " + member.id + " && value == 'True']");
		if (NULL == the_item) {
			list_append(errlist,member_format_error(class, member,"Member is not class-scoped"));
			errors++;
		}
	}
	if (errors > 0) {
		list_append(errlist,"Classes with the stereotype <<utility>> must meet the following criteria:");
		list_append(errlist,"1 - Any attributes must be class attributes");
		list_append(errlist,"2 - Any operations must be class operations");
		return False;
	}
	return True;
}

// 4.3.3
// For normal classes, verify that one of the following is true:
// 	1 - It is an abstract class, or 
// 	2 - It provides operations to override all the abstract operations it
// 	inherits or implements.
//
// 	The first test is trivial. The second test is made easier by the routines
// 	in uml_inherit_ops.inc, which do not include inherited operations that are
// 	"shadowed by local operations. This means that if we get all operations,
// 	we can simply check if any are abstract. If so, they are either declared
// 	abstract locally or are inherited as abstract and not overriddden.
// NOTE: errlist is assumed to be a pre-initialized list of strings.
//
boolean
uml_check_concrete_class_semantics(node class, string ref_restriction, string member_ref_restriction, list errlist)
{
	int i;
	node member;
	item the_item;
	int errors = 0;

	//list_append(errlist,"Checking concrete semantics for '" + class.name + "'...");
	print_message("Checking concrete semantics for '" + class.name + "'...");

	// verify no operations are abstract
	set members = uml_class_all_operations(class, ref_restriction, member_ref_restriction);
	int count = set_count(members);

	for (i = 0; i < count; i++) {
		// for each member
		member = set_get_element(members, i);

		// make sure it's not abstract
		// It must not have an item UmlOperationIsAbstract with the value
		// 'True'
		the_item = find_by_query("item[type == UmlOperationIsAbstract && obj_id == " + member.id + " && value == 'True']");
		if (NULL != the_item) {
			// this operation is abstract
			if (member.scope_node_id == class.id) {
				// this is a locally defined operation that is abstract
				list_append(errlist,member_format_error(class, member,"Operation is abstract"));
			} else {
				// an inherited operation not overridden
				list_append(errlist,member_format_error(class, member,"No local operation provided for inherited abstract operation."));
			}

			errors++;
		}
	}
	if (errors > 0) {
		list_append(errlist,"+Concrete (i.e. non-abstract) classes must meet one of the following criteria:");
		list_append(errlist,"+1 - It is an abstract class, or");
		list_append(errlist,"+2 - It provides operations to override all the abstract operations it inherits");
		return False;
	}
	return True;
}

boolean uml_check_common_semantics(node class, string ref_restriction, string member_ref_restriction, list errlist)
{
	int i;
	node member;
	note attr_def;
	item attr_type;
	int errors = 0;
	string qry;

	//list_append(errlist,"Checking common semantics for '" + class.name + "'...");
	print_message("Checking common semantics for '" + class.name + "'...");

        errors = check_ed_duplicated_operation_param_names(class, errlist);

	set members = uml_class_local_members(class, ref_restriction, member_ref_restriction);
	int count = set_count(members);

	for (i = 0; i < count; i++) {
		member = set_get_element(members, i);
		// for each attribute
		if ("UmlAttribute" != member.type) {
			continue;
		}
		// find all the UmlAttributeDefinition notes 
		// find all the UmlAttributeTypes belonging to these notes
		// report an error if exactly one is not found

                qry = "item[UmlAttributeType && obj_id=" + member.id + "]";
                attr_type = find_by_query(qry);
                if (NULL == attr_type || "" == strip_whitespace(attr_type.value)) {
                list_append(errlist,member_format_error(class, member,"Attribute has no type defined"));
                errors++;
                }

	}
	return (errors == 0);
}


int
check_ed_duplicated_operation_param_names(node current_class, list errlist)
{
    int x, y;
    int error_count = 0;
    string sig_string;
    string current_param;
    string current_param_name;
    string previous_name;
    string current_sorted_name;
    list sig_list;
    list param_name_list = list_create("string", 0);
    node current_operation;
    string query = "node[node_refs && UmlOperation && scope_node_id = " +
                    current_class.id + "]";
    string msg;

    list operation_list = list_select(query);

    for (x=0; x<list_count(operation_list); x++)
    {
        current_operation = list_get(operation_list, x);
        sig_string = current_operation.sig;

        sig_list = string_to_list(sig_string, ",");

        if (list_count(sig_list) < 2)
            continue;

        for (y=0; y<list_count(sig_list); y++)
        {
            current_param = list_get(sig_list, y);
            current_param_name = return_first_token_edgsc(current_param, ":");
            current_param_name = string_strip(current_param_name, "B", " ");
            list_append(param_name_list, current_param_name);
        }

        param_name_list = list_sort(param_name_list);
        previous_name = NULL;

        for (y=0; y<list_count(param_name_list); y++)
        {
            current_sorted_name = list_get(param_name_list, y);

            if (current_sorted_name == previous_name)
            {
                    msg = "Operation \"" + current_class.name + "::" +
                            current_operation.name + "\" has duplicate parameter names \"" +
                            current_sorted_name + "\".";

                   list_append(errlist,msg);

                   error_count++;
            }
            previous_name = current_sorted_name;
        }
        list_clear(param_name_list);
    }
    return error_count;
}


string
return_first_token_edgsc(string in_string, string separator)
{
    if (string_length(in_string) > 0
        && string_find(in_string, 0, separator)
        < string_length(in_string))
        return string_extract(in_string, 0, string_find(in_string, 0, separator));
    else
        return in_string;
}


string
return_last_token_edgsc(string in_string, string separator)
{
    if (string_length(in_string) > 0
        && string_find(in_string, 0, separator) < string_length(in_string))
    {
        return string_extract(in_string,
               string_find(in_string, 0, separator) + string_length(separator),
               string_length(in_string) - string_find(in_string, 0, separator) -
               string_length(separator));
    }
    else
        return in_string;
}






// ref_restriction - a string restricting the query to find classes referenced
// in a certain editor file
// member_ref_restriction - same for operations and attributes
//
boolean uml_check_class_semantics(string class_name, string ref_restriction,
		string member_ref_restriction, list errlist)
{
	boolean ret = True;
	boolean isConcrete = True;
	node class = find_by_query("node[type==UmlClass && name == '" +
			to_oms_string(class_name) + "' && node_refs${ref_restriction}]");

	errlist = list_create("string",0);
	// pre-fetch the stereotype hierarchy to save time
	graph sg = uml_get_full_stereotype_graph();

	//list_append(errlist,"Checking semantics for '" + class_name + "'...");
	print_message("Checking semantics for '" + class_name + "'...");

	if (NULL == class) {
		// no such class
		list_append(errlist,"Class '" + class_name + "' does not exist in the repository.");
		return False;
	}

	ret = uml_check_common_semantics(class,ref_restriction, member_ref_restriction, errlist);


	// get the stereotype. If it's one we know, we do special checks
	item stereoType = find_by_query("item[type==UmlStereotype && " +
				" obj_id == " + class.id + "]");
	if (NULL != stereoType ) 
        {
		if ( uml_is_stereotype_of(stereoType.value,"type",sg))
                {
			// check stereotypes of "type" specially
			ret = uml_check_type_semantics(class,ref_restriction,
					member_ref_restriction, errlist);
			isConcrete = False;
		}
		if ( uml_is_stereotype_of(stereoType.value,"interface",sg))
                {
			// check stereotypes of "interface" specially
			ret = uml_check_interface_semantics(class,ref_restriction,
					member_ref_restriction, errlist);
			isConcrete = False;
		}
		if ( uml_is_stereotype_of(stereoType.value,"utility",sg) ) 
                {
			// check stereotypes of "utility" specially
			ret = uml_check_utility_semantics(class,ref_restriction,
					member_ref_restriction, errlist);
			isConcrete = False;
		}
	}

    // ECR 6464
    // This is not too effective ... But due to the way uml_check_class_semantics
    // get's called we don't know if we're currently working on a UmlClass or
    // a UmlInterface
    node interface = find_by_query( "node[UmlInterface && name == '${class.name}']" );
    if( interface != NULL )
    {
		ret = uml_check_interface_semantics(class,ref_restriction,
				member_ref_restriction, errlist);
		isConcrete = False;
    }
    // ECR 6464

	// fall through to the normal checks
	//
	// first, see if the class is abstract
	
	item isAbs = find_by_query("item[type==UmlClassIsAbstract && " +
				" obj_id == " + class.id + " && value == 'True']");
	if (NULL != isAbs) {
		isConcrete = False;
	}
	if (isConcrete) {
	  ret = ret && uml_check_concrete_class_semantics(class,ref_restriction,
				member_ref_restriction, errlist);
	}
	return ret;

}

