//      StP/Uml
//      Confidential property of Aonix
//      Copyright (c) 1997
//      All rights reserved
//
#include "rules/qrl/uml_inherit_ops.inc"
#include "rules/qrl/uml_class_semantics.inc"

// retrieve the OMS node of type omsNodeType mapped by this node
node
node_from_symbol(gde_node theNode, string omsNodeType)
{
    node ret = NULL;
    node tmpNode;
    list mappedNodes;
    int i;

    // get the OMS UmlClass node for this symbol
    mappedNodes = gde_node_oms_nodes(theNode);
    for(i=0;i<list_count(mappedNodes);i++) {
        tmpNode = list_get(mappedNodes,i);
        if(omsNodeType == tmpNode.type) {
            ret = tmpNode;
            break;
        }
    }
    return ret;
}

// retrieve the OMS link of type omsLinkType mapped by this link
link
link_from_symbol(gde_arc theArc, string omsLinkType)
{
    link ret = NULL;
    link tmpLink;
    list mappedLinks;
    int i,j;
    list gdeArcLinks = gde_arc_links(theArc);

    // get the OMS UmlClass node for this symbol
    mappedLinks = gde_link_oms_links(list_get(gdeArcLinks,0));
    for(i=0;i<list_count(mappedLinks);i++) {
        tmpLink = list_get(mappedLinks,i);
        if(omsLinkType == tmpLink.type) {
            ret = tmpLink;
            break;
        }
    }
    return ret;
}

int uclassd_check_semantics()
{

    list classes = gde_nodes_find_by_type(gde_all_symbols(),
                    "Class");
    list classnames = gde_nodes_find_by_type(gde_all_symbols(),
                    "ClassName");
    if (NULL == classes) {
        classes = classnames;
    } else {
        list_concatenate(classes,classnames);
    }
    
    // ECR 6464
    list interfaces = gde_nodes_find_by_type(gde_all_symbols(),
                    "Interface");
    if( interfaces != NULL ) {
        list_concatenate(classes,interfaces);
    }
    // ECR 6464
                                    
    list inst_classes = gde_nodes_find_by_type(gde_all_symbols(),
                    "InstantiatedClass");
    list param_classes = gde_nodes_find_by_type(gde_all_symbols(),
                    "ParameterizedClass");
    list packages = gde_nodes_find_by_type(gde_all_symbols(),
                    "Package");
    list package_names = gde_nodes_find_by_type(gde_all_symbols(),
                    "PackageName");
    if (NULL == packages) {
        packages = package_names;
    } else {
        list_concatenate(packages,package_names);
    }
    list refines_links = gde_arcs_find_by_type(gde_all_symbols(),
                    "Refines");
    list impl_links = gde_arcs_find_by_type(gde_all_symbols(),
                    "Implements");
    list gen_links = gde_arcs_find_by_type(gde_all_symbols(),
                    "Generalization");
    list assoc_links = gde_arcs_find_by_type(gde_all_symbols(),
                    "Association");
    // ECR 3856 - begin
    list aggreg_links = gde_arcs_find_by_type(gde_all_symbols(),
                    "Aggregation");
    list_concatenate(assoc_links, aggreg_links);
    // ECR 3856 - end
    list nary_assoc_links = gde_arcs_find_by_type(gde_all_symbols(),
                    "NaryAssociationRole");

    int errs = 0;

    errs += uclassd_check_links(refines_links,impl_links,gen_links,
                assoc_links,nary_assoc_links);
    errs += uclassd_check_nodes(classes,inst_classes,param_classes,packages);

    return errs;
}

int
uclassd_check_class_semantics(gde_node cls)
{
    node oms_class = node_from_symbol(cls, "UmlClass");
    int j;
    list errlist;
    int errs = 0;
    int class_errs = 0;
    list packages;
        string current_msg;

    if (NULL != oms_class) {
        errlist = list_create("string",0);
        if ( !uml_check_class_semantics(oms_class.name,"", "",errlist) ) {
            for(j=0;j<list_count(errlist);j++) 
                        {
                            current_msg = list_get(errlist, j);
                            if (err_msg_first_char(current_msg) == "+")
                            {
                                current_msg = string_search_and_replace(current_msg, "+", " ");
                gde_print_message(current_msg);
                            }
                            else
                            {
                gde_print_error(current_msg, gde_node_psymid(cls));
                                class_errs++;
                            }
            }
            errs += class_errs;
        }
        packages = uml_get_containing_packages(oms_class);
        if(NULL != packages && list_count(packages) > 1) {
            gde_print_error("Class '" + oms_class.name + "' is contained by more than one package.\n"+
            "Packages: " + nodes_names(packages) + ".", gde_node_psymid(cls));
            errs++;
        }

    }
    return errs;
}

string
err_msg_first_char(string curr_string)
{
    string first_char = " ";

    if (string_length(curr_string) > 0)
        first_char = string_extract(curr_string, 0, 1);

    return first_char;
}


int
check_ed_op_int_param_names(gde_node gde_op)
{
    int x, y;
    int error_count = 0; 
    string sig_string;  
    string current_param;
    string current_param_name;   
    list sig_list;
    string msg;  

    list oms_nodes_list = gde_node_oms_nodes(gde_op);

    if (list_count(oms_nodes_list) == 0)
        return error_count;

    node current_operation = list_get(oms_nodes_list, 0);
 
    sig_string = current_operation.sig;
 
    sig_list = string_to_list(sig_string, ",");
 
    for (y=0; y<list_count(sig_list); y++)
    {
        current_param = list_get(sig_list, y);
        current_param_name = return_last_token_edsc(current_param, ":");
        current_param_name = string_strip(current_param_name, "B", " ");
 
        // Begin change ECR 6464
        if(selection_count("node[node_refs && name = '" + 
            current_param_name + "' && " + 
            "UmlClass && items[UmlStereotype && " + 
            "(value = 'interface' || value = 'Interface')]]") > 0 ||
           selection_count("node[node_refs && name = '" + 
            current_param_name + "' && " + 
            "UmlClass && in_links[UmlImplements]]") > 0 ||
           selection_count("node[node_refs && name = '" + 
            current_param_name + "' && " + 
            "UmlInterface]") > 0 )
        {
            gde_print_error("Operation '" + current_operation.name +
                "' uses a parameter type '" +
                current_param_name + "' that is an interface.",
            gde_node_psymid(gde_op));
            error_count++;
        }
        // End change ECR 6464
    }
    return error_count;
}


string
return_last_token_edsc(string in_string, string separator)
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


int
uclassd_check_nodes(list classes, list inst_classes, list param_classes, list packages)
{
    int errs  =0;
    gde_node c, ic, pc, member;
    int i,j,k;
    string lbl;
    string node_type;
    list outArcs;
    list cls_Members;
    list all_classes = list_create("gde_node",0);
    gde_arc arc;
    boolean isOK;
    list cls_Attrs;
    list cls_Ops;

        int x;
        list class_list;
        node current_class;

    if(NULL != classes) {
        list_concatenate(all_classes,classes);
    }
    if (NULL != param_classes) {
        list_concatenate(all_classes,param_classes);
    }

    // for each class, make sure it doesn't have duplicate attributes or
    // operations
    for(i=0;i<list_count(all_classes);i++) {
        c = list_get(all_classes,i);
        cls_Members = gde_node_contained_nodes(c,False); 
        if (NULL != cls_Members ) {
            cls_Attrs = list_create("string",0);
            cls_Ops = list_create("string",0);
            for(j=0;j<list_count(cls_Members);j++) {
                member = list_get(cls_Members,j);
                node_type = gde_nt_name(gde_node_nt(member));
                lbl = gde_node_label(member);
                isOK = True;
                if ("Attribute" == node_type) {
                    if (list_find(cls_Attrs,0,lbl) != list_count(cls_Attrs)) {
                        isOK = False;
                    } else {
                        list_append(cls_Attrs,lbl);
                    }
                }
                if ("Operation" == node_type) {

// ECR 7146 begin
                                        //check_ed_op_int_param_names(member);
                                        errs += check_ed_op_int_param_names(member);
// ECR 7146 end

                    if (list_find(cls_Ops,0,lbl) != list_count(cls_Ops)) {
                        isOK = False;
                    } else {
                        list_append(cls_Ops,lbl);
                    }
                }
                if (!isOK) {
                    gde_print_error(node_type + " name is not unique.",
                            gde_node_psymid(member));
                    errs++;
                }
            }
        }
        errs += uclassd_check_class_semantics(c);
    }


    
    node oms_node;
    node pc_node;
    list mapped_nodes;
    boolean isAnonymous;
    // for instantiated classes
    if (NULL != inst_classes) {
        for(i=0;i<list_count(inst_classes);i++) {
            ic = list_get(inst_classes,i);
            // make sure this instantiated class refines at least one
            // parameterized class. This can be either by a Refines link or 
            // by having parameters in the signature
            isOK = False;
            isAnonymous = False;
            mapped_nodes = gde_node_oms_nodes(ic);
            for(j=0;j<list_count(mapped_nodes);j++) {
                oms_node = list_get(mapped_nodes,j);
                if ("UmlInstantiatedClass" == oms_node.type) {
                    if ("" != oms_node.sig && NULL != oms_node.sig) {
                        // This means its name is the name of a parameterized
                        // class, thus it refines that class.
                        isOK = True;
                        isAnonymous = True;
                    }
                    break;
                }
            }

            outArcs = gde_node_outarcs(ic);
            if (NULL != outArcs) {
                for(k=0;k<list_count(outArcs);k++) {
                    arc = list_get(outArcs,k);
                    if (gde_lt_name(gde_arc_lt(arc)) == "Refines" ) {
                        // anonymous iclasses can't have refines links
                        if (isAnonymous)
                            isOK = False;
                        else
                            isOK = True;
                        break;
                    }
                }
            }
            if (!isOK) {
                if (isAnonymous) {
                    gde_print_error("Anonymous Instantiated Classes may not have Binds links.", gde_node_psymid(ic));
                } else {
                    // no Refines arcs
                    gde_print_error("Instantiated Classes must refine at least one ParameterizedClass.", gde_node_psymid(ic));
                }
                errs++;
            } else {
                // Check to make sure the pclass exists
                if (isAnonymous) {
                    pc_node = find_by_query("node[UmlParameterizedClass &&" +
                            " name='" + oms_node.name + "' && node_refs]");
                    if (NULL == pc_node) {
                        gde_print_error("Instantiated Class uses a Parameterized Class that does not exist in the repository.", gde_node_psymid(ic));
                        errs++;
                    }
                }
            }


            if (NULL != param_classes) {
                for(j=0;j<list_count(param_classes);j++) {
                    pc = list_get(param_classes,j);
                    lbl = gde_node_label(ic);
                    // make sure no ParameterizedClass is also represented by an
                    // InstantiatedClass symbol (and vice-versa)
                    if((NULL != lbl) &&
                        ("" != lbl) &&
                        (lbl == gde_node_label(pc))) {
                        gde_print_error("Instantiated Class also represented by a Parameterized Class symbol.", gde_node_psymid(ic));
                        errs++;
                    }
                }
            }
        }
    }

    // make sure all parameterized classes have parameters defined in their
    // annotations
    if (param_classes != NULL ) {
        item clsParams;
        node pcNode;
        for(j=0;j<list_count(param_classes);j++) {
            pc = list_get(param_classes,j);

            // get the OMS UmlClass node for this symbol
            pcNode = node_from_symbol(pc,"UmlClass");
            if(pcNode != NULL) {
                // make sure it has parameters
                clsParams = find_by_query("item[type=UmlClassParameters && obj_id=" + pcNode.id + "]");
                if (NULL == clsParams || NULL == clsParams.value || "" == clsParams.value) {
                    gde_print_error("Parameterized Class does not have any parameter annotations.", gde_node_psymid(pc));
                    errs++;
                }
            }
        }
    }
    // check packages to make sure they are only contained by one node
    if (packages != NULL ) 
        {
            node packageNode;
            list containers;

            for(j=0;j<list_count(packages);j++) 
            {
                pc = list_get(packages,j);

                // get the OMS UmlClass node for this symbol
                packageNode = node_from_symbol(pc,"UmlPackage");
                if(packageNode != NULL) 
                {
                    containers = uml_get_containing_packages(packageNode);
                    if(NULL != containers && list_count(containers) > 1) 
                    {
                        gde_print_error("Package '" + packageNode.name + "' is contained by more than one package. " +
						"Packages: " + nodes_names(containers) + ".", gde_node_psymid(pc));
                        errs++;
                    }

                   // Begin Change ECR 6466
                   if(selection_count("node[UmlClass&&node_refs&&name='${packageNode.name}']") > 0)
                   {
                       gde_print_error("Package '" + packageNode.name + 
                           "' is using a name already given to a class.", 
                           gde_node_psymid(pc));
                        errs++;
                   }
                   // End Change ECR 6466
                }
            }
        }
    
        return errs;
}

// For a given node ID, it gets its name and signature and finds the matching
// node of type elab_type
node
uml_find_elab_node(int id, string elab_type)
{
    node the_node = find_by_query("node[id=" + id + "]");
    string qry;
    if (NULL != the_node) {
        qry = "node[" + elab_type + " && node_refs " +
                " && name='" + the_node.name + "' " +
                " && sig='" + the_node.sig + "']";
        return find_by_query(qry);
    } else {
        return NULL;
    }
}

int
check_ed_for_assclass_link_label(node association_node, gde_arc theArc)
{
    int error_count = 0;
    string assoc_link_label;
    node association_class_node;

    if (string_find(association_node.name, 0, ":") ==
         string_length(association_node.name))
        return error_count;

    string query = "link[link_refs && UmlAssociationClassLink && from_node_id = " + 
                    association_node.id + "]";

    link assoc_class_link = find_by_query(query);

    if (assoc_class_link != NULL)
    {
        association_class_node = find_by_query("node[in_links[" + 
                                  assoc_class_link.id + "]]");

        assoc_link_label = return_first_token_classgsc(association_node.name, ":");

        if (association_class_node != NULL && 
           (assoc_link_label != association_class_node.name))
        {
            gde_print_error("Associations with Association Classes " + 
                            " should share the same label",
                            gde_arc_psymid(theArc));
            error_count++;
        }
    }
    return error_count;
}

string
return_first_token_classgsc(string in_string, string separator)
{
    if (string_length(in_string) > 0
        && string_find(in_string, 0, separator)
        < string_length(in_string))
        return string_extract(in_string, 0, string_find(in_string, 0, separator));
    else
        return in_string;
}


int
check_ed_for_assc_link_comp_bin(node association_node, gde_arc theArc)
{
    int error_count = 0;

    list role_list = list_select("link[UmlRole && to_node_id = " +
                                          association_node.id + 
                                          " && link_refs]");

    if (list_count(role_list) != 2)
        return error_count;

    link left_role = list_get(role_list, 0);
    link rite_role = list_get(role_list, 1);
 
    item this_agg_type_item = find_by_query("item[UmlAggregationType && obj_id = " +
                                   left_role.id +
                                  " && (value = 'Composition' || value = 'Aggregation')]");
    item other_agg_type_item = find_by_query("item[UmlAggregationType && obj_id = " +
                                   rite_role.id +
                                  " && (value = 'Composition' || value = 'Aggregation')]");
 
    if (this_agg_type_item != NULL && other_agg_type_item != NULL)
    {
        gde_print_error("Association may not have an aggregation " + 
                        " or composition on both ends",
                        gde_arc_psymid(theArc));
        error_count++;
    }
    return error_count;
}

int
check_ed_for_role_at_name_conflict(node association_node, gde_arc theArc)
{

    int x, y;
    int error_count = 0; 
    string msg;  
 
// Removed for ECR 7205
//    list end_nodes = list_select("node[node_refs && UmlClass && " +
//                                 "out_links[UmlRole && to_node_id = " +
//                                  association_node.id + "]]");

    list role_list = list_select("link[link_refs && UmlRole && " +
                                    "to_node_id = " + association_node.id + "]");

    if (list_count(role_list) != 2)
        return error_count;
 
    link one_role = list_get(role_list, 0);
    link other_role = list_get(role_list, 1);

    // Begin change ECR 7205
    boolean both_navi, other_navi, my_aggr;
    item the_item;
    
    list navigability = list_select( "item[UmlRoleNavigability && " +
        "link[link_refs && ( id = ${one_role.id} || id = ${other_role.id} )]]" );
    if( list_count( navigability ) == 0 || list_count( navigability ) == 2 )
        both_navi = True;
    else
        both_navi = False;
 
    node current_class;
    node current_matching_att;
    list att_list;
 
    for (x=0; x<list_count(role_list); x++)
    {
        one_role = list_get(role_list, x);
        // selects the class on the other(!) end of the association
        current_class = find_by_query("node[node_refs && UmlClass && " + 
                                "out_links[UmlRole && id!= ${one_role.id} && " +
                                "to_node_id = " + association_node.id + "]]");
        if( ! both_navi )
        {
            // Get navigability on opposite end of assoc
            the_item = find_by_query( "item[UmlRoleNavigability && link[link_refs && id = ${one_role.id}]]" );
            if( the_item != NULL )
                other_navi = True;
            else
                other_navi = False;
            
            // Get aggregation/composition on my side
            // we need to check this too for the case of an aggregation *and*
            // navigability set on one side of the assoc
            the_item = find_by_query( "item[UmlAggregationType && " +
                "( value = 'Aggregation' || value = 'Composition' ) && " +
                "link[UmlRole && link_refs && from_node_id = ${current_class.id}]]" );
            if( the_item != NULL )
                my_aggr = True;
            else
                my_aggr = False;
        }
        else
            other_navi = False;
        
        if( other_navi || both_navi || my_aggr )
        {
            att_list = list_select("node[node_refs && UmlAttribute && scope_node_id = " +
                 current_class.id + " && name = '" + one_role.name + "']");
            for (y=0; y<list_count(att_list); y++)
            {
                current_matching_att = list_get(att_list, y);
    
                gde_print_error( "Class \"" + current_class.name + "\" has an attribute and an " + 
                    "association role that share the same name \"" + 
                    current_matching_att.name + "\".",
                    gde_arc_psymid(theArc));
                error_count++;
            }
        }
    }
    // End change ECR 7205        
    return error_count;
}

int
check_ed_role_ends_for_dup_name(node association_node, gde_arc theArc)
{
    int x, y;
    int error_count = 0; 
    string msg;
 
    list role_list = list_select("link[link_refs && UmlRole && " +
                                    "to_node_id = " + association_node.id + "]");
 
    if (list_count(role_list) != 2)
        return error_count;
 
    link one_role = list_get(role_list, 0);
    link other_role = list_get(role_list, 1);
 
    if (one_role.name == "null" || other_role.name == "null" ||
            one_role.name == "" || other_role.name == "")
        return error_count;
 
    if (one_role.name == other_role.name)
    {
        gde_print_error("Association has identical role names on " + 
                        " both ends",
                        gde_arc_psymid(theArc));
        error_count++;
    }
    return error_count;
}



int
uclassd_check_links(list refines_links,list impl_links,list gen_links, list assoc_links, list nary_assoc_links)
{
    link theLink;
    string refineArgs;
    item theItem;
    int i,j;
    gde_arc theArc;
    int errs = 0;
    list arglist1, arglist2;
    int idx;
    list all_inh_links = list_create("gde_arc",0);

    graph sTypes = uml_get_full_stereotype_graph();

/*
This doesn't take into account interface symbols.

    // check the Implements links
    // make sure that the class on the to-end has stereotype "type"
    if (NULL != impl_links) {
        list_concatenate(all_inh_links,impl_links);
        for(i=0;i<list_count(impl_links);i++) {
            theArc = list_get(impl_links,i);
            theLink = link_from_symbol(theArc,"UmlImplements");
            if (theLink) {
                theItem = find_by_query("item[type=UmlStereotype && obj_id=" + theLink.to_node_id + "]");
                if (NULL==theItem || 
                    !(uml_is_stereotype_of(theItem.value,"interface",sTypes) ||
                     uml_is_stereotype_of(theItem.value,"type",sTypes)) ) {

                    gde_print_error("Only classes of stereotype 'interface' may receive Implements links.", gde_arc_psymid(theArc));
                    errs++;
                }
            }
        }
    }
*/

    // make sure each Refines link has the stereotype "bind" and parameters.
    // The parameters must match the number of parameters of the class on the
    // to-end
    if (NULL != refines_links) {
        list_concatenate(all_inh_links,refines_links);
        for(i=0;i<list_count(refines_links);i++) {
            theArc = list_get(refines_links,i);
            theLink = link_from_symbol(theArc,"UmlRefines");
            if (theLink) {
                theItem = find_by_query("item[type=UmlStereotype && obj_id=" + theLink.id + "]");
                if (NULL==theItem || !uml_is_stereotype_of(theItem.value,"bind",sTypes)) {

                    // ECR 4952
                    gde_print_error("Binds links must have the stereotype 'bind'.", gde_arc_psymid(theArc));
                    errs++;
                }
                theItem = find_by_query("item[type=UmlRefineArgs && obj_id=" + theLink.id + "]");
                if ((NULL == theItem) || (NULL == theItem.value) || ("" == theItem.value)) {
                    gde_print_error("Binds argument annotations are not present.",gde_arc_psymid(theArc));
                    errs++;
                } else {
                    // get the arg list form the class
                    refineArgs = theItem.value;
                    theItem = find_by_query("item[type=UmlClassParameters && obj_id=" + theLink.to_node_id + "]");
                    // don't report an error if there are none - this error is
                    // reported in the node checking stuff
                    if(NULL != theItem && NULL != theItem.value) {
                        arglist1 = list_create("all",0);
                        arglist2 = list_create("all",0);
                        if (!uml_split_arguments(refineArgs,arglist1)) {
                            gde_print_error("Error parsing actual parameters on link.",gde_arc_psymid(theArc));
                            errs++;
                        }
                        if (!uml_split_arguments(theItem.value,arglist2)) {
                            gde_print_error("Error parsing formal parameters on to-node.",gde_arc_psymid(theArc));
                            errs++;
                        }
                        if (list_count(arglist1) != list_count(arglist2)) {
                            gde_print_error("Number of actual parameters does not match formal parameters.",gde_arc_psymid(theArc));
                            errs++;
                        }
                    }
                }
            }
        }
    }

    // association/aggregation links
    // Make sure that classes with stereotypes type, utility, or metaclass are
    // not contained by value
    // Make sure that they are not navigable from an instantiated class
    node theAssoc;
    link theOtherRole;
    item aggType;
    boolean roleIsNavigable;
    boolean otherRoleIsNavigable;
    list mappedLinks;
    list gdeArcLinks;
    if (NULL != assoc_links) {
        for(i=0;i<list_count(assoc_links);i++) {
            theArc = list_get(assoc_links,i);

            gdeArcLinks = gde_arc_links(theArc);

            // get the OMS UmlRole links for this symbol
            mappedLinks = gde_link_oms_links(list_get(gdeArcLinks,0));
            for(j=0;j<list_count(mappedLinks);j++) {
                theLink = list_get(mappedLinks,j);
                theAssoc = find_by_query("node[" + theLink.scope_node_id + "]");

                                if (j == 0)
                                {
                                    errs += check_ed_for_assclass_link_label(theAssoc, theArc);
                                    errs += check_ed_for_assc_link_comp_bin(theAssoc, theArc);
                                    errs += check_ed_for_role_at_name_conflict(theAssoc, theArc);
                                    errs += check_ed_role_ends_for_dup_name(theAssoc, theArc);
                                }

                // This works because Association symbols map precisely 2 links
                theOtherRole = find_by_query("link[UmlRole && to_node_id=" +
                    theAssoc.id + " && id != " + theLink.id + " && link_refs]");

                // For each link, make sure if the AggregationDefinition says the 
                // aggregation type is Composition, that the class on the ToEnd does
                // not have the stereotype type, utility, or metaclass.
                if (NULL != theLink && NULL != theOtherRole) {
                    aggType = find_by_query("item[type=UmlAggregationType && obj_id=" + theLink.id +" && value='Composition']");
                    if (NULL != aggType) {
                        theItem = find_by_query("item[type=UmlStereotype && obj_id=" + theOtherRole.from_node_id + "]");
                        if (NULL==theItem) {
                        }
                        if (NULL!=theItem &&
                            (uml_is_stereotype_of(theItem.value,"type",sTypes) ||
                             uml_is_stereotype_of(theItem.value,"interface",sTypes) ||
                             uml_is_stereotype_of(theItem.value,"utility",sTypes) ||
                             uml_is_stereotype_of(theItem.value,"metaclass",sTypes))) {

                            gde_print_error("Classes with stereotype '" + 
                                    theItem.value + "' may not be targets" +
                                    " of aggregation by value.",
                                    gde_arc_psymid(theArc));
                            errs++;
                        }
                    }
                    // check navigability from InstantiatedClass
                    // check navigability to ParameterizedClass
                    if (NULL != find_by_query("item[UmlRoleNavigability && " +
                            "obj_id=" + theLink.id + " && value='True']")) {
                        roleIsNavigable = True;
                    } else {
                        roleIsNavigable = False;
                    }
                    if (NULL != find_by_query("item[UmlRoleNavigability && " +
                            "obj_id=" + theOtherRole.id + " && value='True']")) {
                        otherRoleIsNavigable = True;
                    } else {
                        otherRoleIsNavigable = False;
                    }
                    // Check instantiated class
                    if (otherRoleIsNavigable &&
                         NULL != uml_find_elab_node(theLink.from_node_id,
                                 "UmlInstantiatedClass")) {
                            gde_print_error("Association links may not be " +
                                    "navigable from Instantiated Classes.",
                                    gde_arc_psymid(theArc));
                            errs++;
                    }

                    // Check Parameterized Class
                    if (roleIsNavigable &&
                         NULL != uml_find_elab_node(theLink.from_node_id,
                                 "UmlParameterizedClass")) {
                            gde_print_error("Association links may not be " +
                                    "navigable to Parameterized Classes.",
                                    gde_arc_psymid(theArc));
                            errs++;
                    }
                }
            }
        }
    }

    // make sure the nary association UmlRole objects don't have aggregation
    // items
    // Also check navigability toward parameterized classes
    if (NULL != nary_assoc_links) {
        boolean naryRoleIsNavigable;
        for(i=0;i<list_count(nary_assoc_links);i++) {
            theArc = list_get(nary_assoc_links,i);
            theLink = link_from_symbol(theArc,"UmlRole");
            if (theLink) {
                theItem = find_by_query("item[type=UmlAggregationType && obj_id=" + theLink.id + "]");
                if (NULL!=theItem) {
                    gde_print_error("N-ary Association Roles may not have aggregation annotations.", gde_arc_psymid(theArc));
                    errs++;
                }
                if (NULL != find_by_query("item[UmlRoleNavigability && " +
                        "obj_id=" + theLink.id + " && value='True']")) {
                    naryRoleIsNavigable = True;
                } else {
                    naryRoleIsNavigable = False;
                }
                if (naryRoleIsNavigable &&
                     NULL != uml_find_elab_node(theLink.from_node_id,
                             "UmlParameterizedClass")) {
                        gde_print_error("N-ary Association Roles may not be" +
                                " navigable to Parameterized Classes.",
                                gde_arc_psymid(theArc));
                        errs++;
                }
            }
        }
    }

    // generalization links
    if (NULL != gen_links) {
        list_concatenate(all_inh_links,gen_links);
    }

    // check inheritance cycles
    errs += uml_check_inheritance_arcs(all_inh_links);

    return errs;
}

// check for generalization/implementes/refines cycles
int 
uml_check_inheritance_arcs(list inh_arcs)
{
    int errs = 0;
    int i;
    int idx;
        int x;
        list already_reported_nodes = list_create("gde_node", 0);
    gde_node toNode, fromNode;

    if (NULL != inh_arcs && list_count(inh_arcs) > 0) {
        graph inh_graph = graph_create();
        gde_arc theArc;
        gde_link theLink;
        string toName, fromName, tmp;
        list fromNames = list_create("string",0);
        set toNames = set_create("string");
        list fromNodes = list_create("gde_node",0);
        list subclasses;
        list superclasses;
        string fromNodeType;
        string toNodeType;

        for(i=0;i<list_count(inh_arcs);i++) {
            theArc = list_get(inh_arcs,i);
            theLink = list_get(gde_arc_links(theArc),0);
            toNode = gde_link_tonode(theLink);
            fromNode = gde_link_fromnode(theLink);

            toNodeType = gde_nt_name(gde_node_nt(toNode));

            if ("Package" == toNodeType || "PackageName" == toNodeType) {
                toNodeType = "Package";
            } else {
                toNodeType = "Class";
            }
            fromNodeType = gde_nt_name(gde_node_nt(fromNode));
            if ("Package" == fromNodeType || "PackageName" == fromNodeType) {
                fromNodeType = "Package";
            } else {
                fromNodeType = "Class";
            }

            toName = gde_node_label(toNode);
            fromName = gde_node_label(fromNode);

            // This will leave out unlabeled classes as well as vertices
            if (NULL == toName || "" == toName || NULL == fromName || "" == fromName) {
                continue;
            }
            toName = toNodeType + " " + 
                                 string_search_and_replace(toName, " ", "");
            fromName = fromNodeType + " " + 
                                 string_search_and_replace(fromName, " ", "");
            graph_add_arc(inh_graph,fromName,toName);
            list_append(fromNames,fromName);
            set_add(toNames,toName);
            list_append(fromNodes,fromNode);
                       
        }
        // so now we have lists of the from and to names, and we have a graph
        // of their connections. So, for each name in the toNames set, check
        // if it is part of a cycle. If it is part of a cycle, then by
        // definition it will also be present in the from* lists, so we can
        // use those lists to get teh node
        for(i=0;i<set_count(toNames);i++) {
            toName = set_get_element(toNames,i);
            // check to see if this node is involved in a cycle. If it is,
            // there will be a path from itself to itself
            if (graph_is_indirect_arc(inh_graph,toName,toName)) {
                // first, fetch the node

                idx = list_find(fromNames,0,toName);
                fromNode = list_get(fromNodes,idx);
                gde_print_error(toName + " is involved in a " +
                    "Generalization/Binds/Implementation cycle.",
                    gde_node_psymid(fromNode));
                                list_append(already_reported_nodes, fromNode);
                errs++;
            }
        }

            errs += check_inh_links_on_other_diags(fromNode, toNode, 
                             gde_diagram_name(), already_reported_nodes, inh_graph);
    }

    return errs;
}



list
sem_check_return_nodes()
{
int x;
gde_symbol current_symbol;
gde_node current_gde_node;
list tmp_symbol_list = list_create("gde_node", 0);
list symbol_list = gde_all_symbols();

for(x=0; x < list_count(symbol_list); x++)
{
    current_symbol = list_get(symbol_list, x);
    if (gde_symbol_is_node(current_symbol))
    {
        current_gde_node = to_gde_node(current_symbol);

        if (gde_nt_name(gde_node_nt(current_gde_node)) == "Class" ||
            gde_nt_name(gde_node_nt(current_gde_node)) == "Package")
        list_append(tmp_symbol_list, current_gde_node);
    }
}
return tmp_symbol_list;
}






int
check_inh_links_on_other_diags(gde_node from_node, gde_node to_node, 
              string diag_name, list already_reported_nodes, graph inh_graph)
{
    int x;
    int error_count = 0;
    string fromName = string_search_and_replace(gde_node_label(from_node), "\n", "");
    string toName = string_search_and_replace(gde_node_label(to_node), "\n", "");
    string fromNodeType = gde_nt_name(gde_node_nt(from_node));
    string toNodeType = gde_nt_name(gde_node_nt(to_node));
    string oms_from_node_type = NULL;
    string oms_to_node_type = NULL;
    link current_inh_link;
    node oms_from_node;
    node oms_to_node;
    string return_from_node_pair;
    string return_to_node_pair;
    string pretty_from_type;
    string pretty_to_type;
    list oms_from_node_list = list_create("node", 0);
    list oms_to_node_list = list_create("node", 0);
    list return_pair_from_node_list = list_create("string", 0);
    list return_pair_to_node_list = list_create("string", 0);
    list been_done_list = list_create("string", 0);
    string to_node_name;
    gde_node gde_from_node;
    gde_node gde_to_node;
    int index;


    fromName = string_search_and_replace(fromName, " ", "");
    toName = string_search_and_replace(toName, " ", "");

    if (fromNodeType == "Class")
        oms_from_node_type = "UmlClass";
    if (toNodeType == "Class")
        oms_to_node_type = "UmlClass";
    if (fromNodeType == "Package")
        oms_from_node_type = "UmlPackage";
    if (toNodeType == "Package")
        oms_to_node_type = "UmlPackage";

    if (oms_from_node_type == NULL || oms_to_node_type == NULL)
        return error_count;

//    string query = "link[UmlGeneralization && " + 
//            "link_refs[file[UmlClassDiagram && name != '" + diag_name + "']] && " + 
//            "((from_node[name = '" + toName + "' && type = " + oms_to_node_type + "] || " + 
//            "to_node[name = '" + toName + "' && type = " + oms_to_node_type + "]) || " + 
//            "(from_node[name = '" + fromName + "' && type = " + oms_from_node_type + "] || " + 
//            "to_node[name = '" + fromName + "' && type = " + oms_from_node_type + "]))]"; 

    // ECR4947
    // Since the previous query was *really* slow, now it is divided into 2 queries,
    // and after the two queries are executed, the result is merged. This works
    // *really* faster
    string query = "link[UmlGeneralization && " + 
            "link_refs[file[UmlClassDiagram && name != '" + diag_name + "']] && " + 
            "from_node[(name = '" + toName + "' || name = '" + fromName + "') && type = " + oms_to_node_type + "]] ";

    set other_links_list = set_select(query);

    query = "link[UmlGeneralization && " + 
            "link_refs[file[UmlClassDiagram && name != '" + diag_name + "']] && " + 
            "to_node[(name = '" + toName + "' || name = '" + fromName + "') && type = " + oms_to_node_type + "]]";

    set s1 = set_select(query);

    set_union(other_links_list,s1);

    // list other_links_list = list_select(query);

    
    for (x=0; x<set_count(other_links_list); x++)
    {

        current_inh_link = set_get_element(other_links_list, x);

        oms_from_node = find_by_query("node[node_refs && " + 
                                      current_inh_link.from_node_id + "]");
        oms_to_node = find_by_query("node[node_refs && " + 
                                     current_inh_link.to_node_id + "]");

        if (oms_from_node.type == "UmlPackage")
            pretty_from_type = "Package";
        if (oms_to_node.type == "UmlPackage")
            pretty_to_type = "Package";
        if (oms_from_node.type == "UmlClass")
            pretty_from_type = "Class";
        if (oms_to_node.type == "UmlClass")
            pretty_to_type = "Class";

        return_from_node_pair = pretty_from_type + " " + oms_from_node.name;
        return_to_node_pair = pretty_to_type + " " + oms_to_node.name;


        list_append(return_pair_from_node_list, return_from_node_pair);
        list_append(return_pair_to_node_list, return_to_node_pair);
        list_append(oms_from_node_list, oms_from_node);
        list_append(oms_to_node_list, oms_to_node);
        graph_add_arc(inh_graph, return_from_node_pair, return_to_node_pair);

    } 

    for (x=0; x<list_count(return_pair_to_node_list); x++)
    {

        to_node_name = list_get(return_pair_to_node_list, x);

        if (graph_is_indirect_arc(inh_graph, to_node_name, to_node_name)) 
        {
            oms_to_node = list_get(oms_to_node_list, x);
            gde_to_node = return_sem_check_gde_node_from_diag(sem_check_return_nodes(), 
                                                      oms_to_node);

            index = list_find(already_reported_nodes, 0, gde_to_node);

            if (gde_to_node != NULL && 
                index == list_count(already_reported_nodes) &&
                list_find(been_done_list, 0, to_node_name) == list_count(been_done_list))
            {
                list_append(been_done_list, to_node_name);
            gde_print_error(to_node_name + " is involved in a " +
                "Generalization/Binds/Implementation cycle.",
                gde_node_psymid(gde_to_node));
                error_count++;
            }
        }

    }

    return error_count;
}


gde_node
return_sem_check_gde_node_from_diag(list gde_node_list, node oms_from_node)
{
    int x;
    gde_node current_gde_node;
    string current_name;
    string current_type;

    for (x=0; x<list_count(gde_node_list); x++)
    {
        current_gde_node = list_get(gde_node_list, x);
        current_name = string_search_and_replace(gde_node_label(current_gde_node), " ", "");
        current_type = gde_nt_name(gde_node_nt(current_gde_node));

        if (current_type == "Package")
            current_type = "UmlPackage";
        if (current_type == "Class")
            current_type = "UmlClass";

        if (current_name == oms_from_node.name && 
            current_type == oms_from_node.type)
            return current_gde_node; 
    }
    return NULL;
}




