//
//      sccsid[] = %W% %Y% %D%
//      StP/UML
//      Confidential property of Aonix
//      Copyright (c) 1997
//      All rights reserved
//
// JED - this file is mostly cloned from omt_booch
////////////////////////////////////////////////////////////////////////////////
// Begin ECR947, ECR 4494
#include "rules/qrl/note_link.inc"
// End ECR947, ECR 4494

const string classname_attr_from_table_query = "node[UmlAttribute && scope_node[name='${classname}'] && node_refs[file[UmlClassTable]]]";

const string classname_op_from_table_query = "node[UmlOperation && scope_node[name='${classname}'] && node_refs[file[UmlClassTable]]]";

const string classname_attr_from_all_query = "node[UmlAttribute && scope_node[name='${classname}'] && node_refs]";
const string classname_op_from_all_query = "node[UmlOperation && scope_node[name='${classname}'] && node_refs]";

gde_node
create_new_node_at_current(string type)
{
  /* shared function */
    
    int    x, y;
    gde_node    nn;



    x = gde_currx();
    y = gde_curry();

    nn = gde_node_create(type, x, y);
    return(nn);
}

void
create_new_operation()
{
    gde_node    nn;

    nn = create_new_node_at_current("Operation");
    gde_select_node(nn);
}

void
create_new_attribute()
{
    gde_node    nn;

    nn = create_new_node_at_current("Attribute");
    gde_select_node(nn);
}

// return a list of current nodes of specified type 
// which is scoped to the given gde_node
list
get_node_sibling(string apptype, string sep, gde_node gn )
{
    node n, cn;
    int i;
    list nodes;
    list t = string_to_list(apptype,sep);

    list r = list_create("node", 0);
    if (gn == NULL)
        return r;

    list oms_nodes = gde_node_oms_nodes(gn);
    if (oms_nodes == NULL || list_count(oms_nodes) == 0)
        return r;

    // we just need the first one
    cn = list_get(oms_nodes, 0);

    nodes = gde_all_mapped_oms_nodes();
    for (i = 0; i < list_count(nodes); i = i + 1)
    {
        n = list_get(nodes, i);
        if ((list_find(t, 0, n.type) != list_count(t)) &&
            n.scope_node_id == cn.id)
            list_append(r, n);
    }
    return r;
}

void
DecorateSelectedBoochClassSymbolsFromSelectedAll()
{
    list            selset;
    list            r;
    gde_node        class;
    string          classname;
    int             cnt;

    ClassEditorAction__begin();

    selset = booch_selected_class_symbols();
    if (list_count(selset) == 1)
    {
    class = list_get(selset, 0);
    DecorateSelectedBoochClassSymbolsFromSelectedAll__class_sym = class;
    classname = to_oms_string(strip_whitespace(gde_node_label(class)));
    r = list_create("all", 0);
    list_concatenate(r, list_select(classname_attr_from_all_query));
    list_concatenate(r, list_select(classname_op_from_all_query));
    if (list_count(r) < 1)
    {
        print_error("Class '" + classname + "' has no components.", NULL);
        DecorateSelectedBoochClassSymbolsFromSelectedAll__class_sym = NULL;
    } else
    {
        cnt = editor_item_selector(r, "DecorateSelectedBoochClassSymbolsFromSelectedAll__callback();", "Add Selected Attributes And Operations", False);
        if (cnt <= 0)
        DecorateSelectedBoochClassSymbolsFromSelectedAll__class_sym = NULL;
    }
    }

    ClassEditorAction__end();

    return;
}

void
DecorateSelectedBoochClassSymbolsFromTable()
{
    list            class_symbols;
    int             i;
    gde_node        gnode;

    ClassEditorAction__begin();

    class_symbols = booch_selected_class_symbols();
    for (i = 0; i < list_count(class_symbols); i++)
    {
    gnode = list_get(class_symbols, i);
    DecorateBoochClassSymbol1(gnode, classname_attr_from_table_query, classname_op_from_table_query, "list_select_node_sort_by_table_row", False);
    }

    ClassEditorAction__end();

    return;
}

void
DecorateSelectedBoochClassSymbolsFromAll()
{
    list    class_symbols;
    int        i;
    gde_node    gnode;
  
    ClassEditorAction__begin();

    class_symbols = booch_selected_class_symbols();
    for (i = 0; i < list_count(class_symbols); i++)
    {
    gnode = list_get(class_symbols, i);
    DecorateBoochClassSymbol1(gnode, classname_attr_from_all_query, classname_op_from_all_query, "list_select", True);
    }

    ClassEditorAction__end();

    return;
}

void
ScaleToFitSelectedBoochClassSymbols()
{
    list            class_symbols;
    int             i;
    gde_node        gnode, float_sym;

    ClassEditorAction__begin();

    class_symbols = booch_selected_class_symbols();
    for (i = 0; i < list_count(class_symbols); i++)
    {
    gnode = list_get(class_symbols, i);
    float_sym = find_floating_container_in_class(gnode);
    if (float_sym == NULL)
        continue;
    ScaleToFitFloatBoochClassSymbol1(gnode, float_sym, True);
    }

    ClassEditorAction__end();

    return;
}

/*************************************************************************
 * Return a list of categories which are part of this category.
 *    if cat == NULL returns all global categories
 *     explode == True descend hierarchy recursively
 *************************************************************************/

list
categories_in_category(node cat, boolean explode)
{
    list    res;
    list    l;
    node    n, comp;
    // ECR 5591 sort by name
    const string cq = "node[UmlPackage && node_refs && in_links[UmlContains && from_node_id = ${cat.id} && link_refs] ] sort by name";
    const string gq = "node[UmlPackage && node_refs && count[in_links[UmlContains && link_refs && from_node[UmlPackage]]]=0] sort by name";
                
    string    q;

    if (cat == NULL)
    {
        q = gq;
    } else {
        q = cq;
    }

    res = list_create("node", 0);
    for_each_in_select(q, comp)
    {
        /* cycle check */
        if (list_find(res, 0, comp) != list_count(res))
            continue;
        list_append(res, comp);
        if (explode)
        {
            l = categories_in_category(comp, explode);
            list_concatenate(res, l);
        }
    }
    return(res);
}


void
DecorateSelectedBoochCatSymbolsFromSelectedAll()
{
    list            selset;
    list            r;
    node            cat;
    int             cnt;

    ClassEditorAction__begin();

    selset = booch_selected_cat_symbols();
    if (list_count(selset) == 1)
    {
    DecorateSelectedBoochCatSymbolsFromSelectedAll__cat_sym = list_get(selset, 0);
    cat = node_from_oid(symbol_object_id(to_gde_symbol(DecorateSelectedBoochCatSymbolsFromSelectedAll__cat_sym), "ClassMapping"));
    if (cat != NULL)
    {
        r = list_create("node", 0);
        list_concatenate(r, categories_in_category(cat, False));
        list_concatenate(r, classes_in_category(cat, False));
        if (list_count(r) < 1)
        {
        print_error("Package '" + cat.name + "' has no components.", NULL);
        DecorateSelectedBoochCatSymbolsFromSelectedAll__cat_sym = NULL;
        } else
        {
        cnt = editor_item_selector(r, "DecorateSelectedBoochCatSymbolsFromSelected__callback(DecorateSelectedBoochCatSymbolsFromSelectedAll__cat_sym);", "Add Selected Packages and Classes", False);
        if (cnt <= 0)
            DecorateSelectedBoochCatSymbolsFromSelectedAll__cat_sym = NULL;
        }
    }
    }

    ClassEditorAction__end();

    return;
}

/*************************************************************************
 * Return a list of classes which are part of this category or class.
 *    if cat == NULL returns all global classes
 *     explode == True descend hierarchy (nested classes) recursively
 *************************************************************************/


list
classes_in_category(node cat, boolean explode)
{
    list    res;
    list    l;
    node    comp;
    // ECR 5591 sort by name
    const string cq = "node[UmlClass && node_refs && in_links[UmlContains && from_node_id = ${cat.id} && link_refs]] sort by name";
    const string gq = "node[UmlClass && node_refs && count[in_links[UmlContains && link_refs && from_node[UmlPackage]]]=0] sort by name";
    string    q;

    if (cat == NULL)
    {
        q = gq;
    } else {
        q = cq;
    }
    res = list_create("node", 0);

//
// input type is actually either a class or cat -- explode will
// result in any nested classes being added, but is this correct?
// i don't think a nested class is inherently a member of the same
// category as the parent class
//
// if we have this check here, then we don't need the cycle check
// below and we effectively disregard the explode option
//
//    if (cat != NULL && cat.type != "Category")
//    return res;
//

    for_each_in_select(q, comp)
    {
        if (comp.name == "")
            continue;
        /* cycle check */
        if (list_find(res, 0, comp) != list_count(res))
            continue;
        list_append(res, comp);
        if (explode)
        {
            l = classes_in_category(comp, explode);
            list_concatenate(res, l);
        }
    }
    return(res);
}


void
DecorateSelectedBoochCatSymbolsFromSelectedGlobal()
{
    list            selset;
    list            r;
    node            cat;
    int             cnt;
    // ECR 3680, index of selected package in package list
    int selfIdx;

    ClassEditorAction__begin();

    selset = booch_selected_cat_symbols();
    if (list_count(selset) == 1)
    {
    DecorateSelectedBoochCatSymbolsFromSelectedGlobal__cat_sym = list_get(selset, 0);
    cat = node_from_oid(symbol_object_id(to_gde_symbol(DecorateSelectedBoochCatSymbolsFromSelectedGlobal__cat_sym), "ClassMapping"));
    if (cat != NULL)
    {
        r = list_create("node", 0);
        list_concatenate(r, categories_in_category(NULL, False));
        // begin ECR 3680, try to find the selected package, if successful delete it from the list
        selfIdx = list_find(r, 0, cat);
        if(selfIdx != list_count(r))
	    list_delete(r, selfIdx);
        // end ECR 3680
        list_concatenate(r, classes_in_category(NULL, False));
        if (list_count(r) < 1)
        {
        print_error("There are no uncategorized categories/classes.", NULL);
        DecorateSelectedBoochCatSymbolsFromSelectedGlobal__cat_sym = NULL;
        } else
        {
        cnt = editor_item_selector(r, "DecorateSelectedBoochCatSymbolsFromSelected__callback(DecorateSelectedBoochCatSymbolsFromSelectedGlobal__cat_sym);", "Add Selected Global Packages and Classes", False);
        if (cnt <= 0)
            DecorateSelectedBoochCatSymbolsFromSelectedGlobal__cat_sym = NULL;
        }
    }
    }

    ClassEditorAction__end();

    return;
}

void
DecorateSelectedBoochCatSymbolsFromAll()
{
    list    cat_symbols;
    int        i;
    gde_node    gnode;
  
    ClassEditorAction__begin();

    cat_symbols = booch_selected_cat_symbols();
    for (i = 0; i < list_count(cat_symbols); i++)
    {
    gnode = list_get(cat_symbols, i);
    DecorateBoochCatSymbol1(gnode, "categories_in_category", "classes_in_category");
    }

    ClassEditorAction__end();

    return;
}


void
DeleteMembersOfSelectedBoochCats()
{
    list    cat_symbols;
    int        i;
    gde_node    gnode;
    int        dodefer;
  
    ClassEditorAction__begin();

    cat_symbols = booch_selected_cat_symbols();    
    dodefer = gde_start_edit_transaction();
    for (i = 0; i < list_count(cat_symbols); i++)
    {
    gnode = list_get(cat_symbols, i);
    ClearBoochSymbolComponents1(gnode, "Contains");
    }
    gde_builtin(SYM_SCALE_TO_LABEL);
    gde_end_edit_transaction(GDE_CMD_DELETE, dodefer, False);
    gde_select_symbols_in_list(gde_node_list_to_sym_list(cat_symbols));
    gde_builtin(SYM_UPDATE_DMARK_MSG);

    ClassEditorAction__end();

    return;
}

gde_node    DecorateSelectedBoochClassSymbolsFromSelectedAll__class_sym = NULL;
void
DecorateSelectedBoochClassSymbolsFromSelectedAll__callback()
{
    int        id;
    list    ids, attrs, ops,rm;
    int        i,j;
    node    attrop,n;

    if (DecorateSelectedBoochClassSymbolsFromSelectedAll__class_sym == NULL)
    return;

    // 13053 fix. Apply , Undo and Apply caused a gde crash
    list selset = booch_selected_class_symbols();
    if (list_count(selset) == 1)
    {
    gde_node class = list_get(selset, 0);
        if (class != DecorateSelectedBoochClassSymbolsFromSelectedAll__class_sym)
        {
            print_error("Current selection changed. Cancel the command and try again.");
            return;
        }
    }
    

    if (!gde_symbol_valid(to_gde_symbol(DecorateSelectedBoochClassSymbolsFromSelectedAll__class_sym)))
    {
    print_error("Internal error; command aborted.", NULL);
    return;
    }

    // SPR 3388 - disable graphics drawing
    gde_disable_gr_update();

    ClassEditorAction__begin();

    ids = editor_selector_selected_objects();
    attrs = list_create("node", 0);
    ops = list_create("node", 0);

    // get a list of current attributes and operations
    rm = get_node_sibling("UmlAttribute||UmlOperation", "||",
        DecorateSelectedBoochClassSymbolsFromSelectedAll__class_sym);

    for(i = 0; i < list_count(ids); i++)
    {
    id = list_get(ids, i);
    if (id == 0)
        continue;
    attrop = node_from_oid(id);
    if (attrop == NULL)
        continue;

        // check if duplicates exist
        for (j=0; j< list_count(rm); j++)
        {
             n = list_get(rm,j);
             if (n.id == attrop.id)
             {
                 //print_error("Duplicates ignored.", NULL);
                 attrop = NULL;
                 break;
             }
        }
        if (attrop == NULL)
            continue;


    if (attrop.type == "UmlAttribute")
        list_append(attrs, attrop);
    else if (attrop.type == "UmlOperation")
        list_append(ops, attrop);
    }
    decorate_map_attr_op_list(DecorateSelectedBoochClassSymbolsFromSelectedAll__class_sym, attrs, ops, "AttributeLabel", "OperationLabel", False);

    ClassEditorAction__end();

    // SPR 3388 - make sure we reenable drawing and update the diagram
    gde_enable_gr_update();
    gde_redraw_diagram();

    return;
}

gde_node    DecorateSelectedBoochCatSymbolsFromSelectedAll__cat_sym = NULL;
gde_node    DecorateSelectedBoochCatSymbolsFromSelectedGlobal__cat_sym = NULL;
void
DecorateSelectedBoochCatSymbolsFromSelected__callback(gde_node cat_sym)
{
    int        id;
    list    ids, cats, classes;
    int        i;
    node    cc;

    if (cat_sym == NULL)
    return;

    if (!gde_symbol_valid(to_gde_symbol(cat_sym)))
    {
    print_error("Internal error; command aborted.", NULL);
    return;
    }

    ClassEditorAction__begin();

    ids = editor_selector_selected_objects();
    cats = list_create("node", 0);
    classes = list_create("node", 0);
    for(i = 0; i < list_count(ids); i++)
    {
    id = list_get(ids, i);
    if (id == 0)
        continue;
    cc = node_from_oid(id);
    if (cc == NULL)
        continue;
    if (cc.type == "UmlPackage")
        list_append(cats, cc);
    else if (cc.type == "UmlClass")
        list_append(classes, cc);
    }
    decorate_map_cat_cl_list(cat_sym, cats, classes, "node_name", "node_name", False);

    ClassEditorAction__end();

    return;
}


////////////////////////////////////////////////////////////////////////////////

void
DecorateBoochClassSymbol1(gde_node class_sym, string attr_query, string op_query, string query_list_select_fn, boolean addMessages)
{
    string          classname;
    list            attrs, ops;
    int i;

    classname = to_oms_string(strip_whitespace(gde_node_label(class_sym)));

    if (query_list_select_fn == NULL || query_list_select_fn == "")
    query_list_select_fn = "list_select";
    qrl_eval("attrs = " + query_list_select_fn + "(string_search_and_replace(attr_query, \"${classname}\", classname));");
    qrl_eval("ops = " + query_list_select_fn + "(string_search_and_replace(op_query, \"${classname}\", classname));");

    // SPR 2894 - disable graphics drawing
    gde_disable_gr_update();

    decorate_map_attr_op_list(class_sym, attrs, ops, "AttributeLabel", "OperationLabel", True);
    if (addMessages) {
        // get messages
        // get inherited ops, add to ops
        // remove messages masked by ops
        node the_class = find_by_query("node[UmlClass && name='${classname}']");
        if (NULL == the_class)
        {
            // SPR 2894 - make sure we reenable drawing and update the diagram
            gde_enable_gr_update();
            gde_redraw_diagram();
            return;
        }

        list messages = uml_messages_for_class("*",the_class);

        // Messages can be sent to an object of a class that inherits operations
        // to implement them. So, it is not right to add operations for the
        // messages because they are already there via inheritance. So, get all
        // the inherited operations and remove messages from the list that have
        // the same name as inherited operations
        if (list_count(messages) <= 0)
        {
            // SPR 2894 - Make sure we reenable drawing and update the diagram
            gde_enable_gr_update();
            gde_redraw_diagram();
            return;
        }

        // Added implement_interface parameter for ECR3000
        set inh_ops = uml_classname_inherited_ops(classname,"","",False);
        // append local ops
        for(i=0;i<list_count(ops);i++) {
            set_add(inh_ops,list_get(ops,i));
        }

        node op;
        int loc;
        for(i=0;i<set_count(inh_ops);i++) {
            op = set_get_element(inh_ops,i);
            loc = list_find(messages,0,op.name);
            if (loc != list_count(messages)) {
                // found a message already covered by an inherited operation.
                // Remove it from the list before we build the table.
                list_delete(messages,loc);
            }
        }
        
        decorate_map_attr_op_list(class_sym, list_create("node",0),
                messages, "AttributeLabel", "", False);
    }

    // SPR 2894 - make sure we reenable drawing and update the diagram
    gde_enable_gr_update();
    gde_redraw_diagram();

    return;
}

void
DecorateBoochCatSymbol1(gde_node cat_sym, string cats_fn, string classes_fn)
{
    list            cats, classes;

    node            cat = node_from_oid(symbol_object_id(to_gde_symbol(cat_sym), "ClassMapping"));

    qrl_eval("cats = " + cats_fn + "(cat, False);");
    qrl_eval("classes = " + classes_fn + "(cat, False);");

    decorate_map_cat_cl_list(cat_sym, cats, classes, "node_name", "node_name", True);
    return;
}

void
ScaleToFitFloatBoochClassSymbol1(gde_node class_sym, gde_node float_sym, boolean tight)
{
    /* obsolete */
    return;
}

void
ClearBoochSymbolComponents1(gde_node class_sym, string component_link_type)
{
//return;
    list outarcs;
    list comp_nodes;
    gde_arc outarc;
    gde_node comp_node;
    int i;

    // iterate through the list of outgoing arcs
    outarcs = gde_node_outarcs(class_sym);
    comp_nodes = list_create("gde_node", 0);

    // First find all of the component nodes and store them in a list
    for (i = 0; i < list_count(outarcs); i++) 
    {
    outarc = list_get(outarcs, i);

    // if the arc is a class component arc, delete the tonode
    if (gde_lt_name(gde_arc_lt(outarc)) == component_link_type)
    {
        comp_node = gde_arc_tonode(outarc);
        list_append(comp_nodes, comp_node);
    }
    }

    // Now delete all the component nodes that we found
    for (i = 0; i < list_count(comp_nodes); i++) 
    {
    comp_node = list_get(comp_nodes, i);
    gde_node_delete(comp_node);
    }
}

void
DecorateBoochClassSymbolFromStrings1(gde_node class_sym, list attrs, list ops)
{
    // SPR 3388 - disable graphics drawing
    gde_disable_gr_update();

    decorate_map_attr_op_list(class_sym, attrs, ops, "make_multi_line_label_from_string", "make_multi_line_label_from_string", True);
    
	// SPR 3388 - make sure we reenable drawing and update the diagram
    gde_enable_gr_update();
    gde_redraw_diagram();

	return;
}


// don't change the name of this unless you also change the logic
// and the name in ct/rules/qrl/cc_update.inc
//
void
DecorateBoochCatSymbolFromStrings1(gde_node cat_sym, list cats, list classes)
{
    decorate_map_cat_cl_list(cat_sym, cats, classes, "string_identity", "string_identity", True);
    return;
}

void
decorate_map_attr_op_list(gde_node class_sym, list attrs, list ops, string attr_mapping_fn, string op_mapping_fn, boolean clear_first)
{
    int             dodefer;
    int             i, x, y;
    gde_node        attr_sym, op_sym, float_sym;

    gde_save_state();
    dodefer = gde_start_edit_transaction();

    // ECR 6432 start
    // if (clear_first && (list_count(attrs) + list_count(ops)) > 0)
        // ClearBoochSymbolComponents1(class_sym, "Contains");
    list arcs;
    list comp_node_labels, comp_gde_nodes;
    gde_arc arc;
    gde_node comp_node;
    int found_idx;
    string new_label;
    int j;

    // iterate through the list of outgoing arcs
    arcs = gde_node_outarcs(class_sym);
    comp_node_labels = list_create("string", 0);
    comp_gde_nodes = list_create("gde_node", 0);

    // First find all of the component nodes and store them in a list
    for (i = 0; i < list_count(arcs); ++ i) 
    {
        arc = list_get(arcs, i);

        // if the arc is a class component arc, add it to list
        if (gde_lt_name(gde_arc_lt(arc)) == "Contains")
        {
            comp_node = gde_arc_tonode(arc);
            list_append(comp_node_labels, gde_node_label(comp_node));
            list_append(comp_gde_nodes, comp_node);
        }
    }
    list used_nodes = list_create("boolean", list_count(comp_gde_nodes));
    // ECR 6432 end

    /*
     * hack to make sure the class is of at least the default size, otherwise
     * the attrs/ops seem to miss if it's too small
     */
     // SPR 2894 - This hack was a bad thing to do, found a better solution
     //  gde_node        dummy = gde_node_create("Class", -10000, -10000);
     //  gde_node_scale(class_sym, 5, max(gde_node_width(class_sym), gde_node_width(dummy)), max(gde_node_height(class_sym), gde_node_height(dummy)));
     //  gde_node_delete(dummy);
 
     x = gde_node_x(class_sym);
//     y = gde_node_y(class_sym); // ECR 7570
     y = gde_node_y(class_sym) - gde_node_height(class_sym) / 2 + 1; // ECR 7570

    for (i = 0; i < list_count(attrs); i++)
    {
        attr_sym = gde_node_create("Attribute", x - 10, y);
        qrl_eval("gde_node_set_label(attr_sym, " + attr_mapping_fn + "(list_get(attrs, i)));");
        
        // ECR 6432 start
        new_label = gde_node_label(attr_sym);
        // message("new_label: " + new_label);
        found_idx = list_find(comp_node_labels, 0, new_label);
        if(found_idx != list_count(comp_node_labels)) {
            gde_node_delete(attr_sym);
        // Get new x,y and move attribute into class
// ECR 7570 begin
//        x = gde_node_x(class_sym);
//        y = gde_node_y(class_sym);
// ECR 7570 end
            gde_node_move_to(list_get(comp_gde_nodes, found_idx), x, y + 1);	// ECR 7673 - changed to +1
            list_set(used_nodes, found_idx, True);
// ECR 7570 begin
            x = gde_node_x(list_get(comp_gde_nodes, found_idx)); 
            y = gde_node_y(list_get(comp_gde_nodes, found_idx)); 
// ECR 7570 end
            continue;
        }
        // ECR 6432 end

        // Get new x,y and move attribute into class
// ECR 7570 begin
//        x = gde_node_x(class_sym);
//        y = gde_node_y(class_sym);
// ECR 7570 end
        gde_node_move_to(attr_sym, x, y);
// ECR 7570 begin
        x = gde_node_x(attr_sym); 
        y = gde_node_y(attr_sym); 
// ECR 7570 end
    }

    for (i = 0; i < list_count(ops); i++)
    {
        op_sym = gde_node_create("Operation", x - 10, y);
        qrl_eval("gde_node_set_label(op_sym, " + op_mapping_fn + "(list_get(ops, i)));");

        // ECR 6432 start
        new_label = gde_node_label(op_sym);
        // message("new_label: " + new_label);
        found_idx = list_find(comp_node_labels, 0, new_label);
        if(found_idx != list_count(comp_node_labels)) {
            gde_node_delete(op_sym);
        // Get new x,y and move operation into class
// ECR 7570 begin
//        x = gde_node_x(class_sym);
//        y = gde_node_y(class_sym);
// ECR 7570 end
            gde_node_move_to(list_get(comp_gde_nodes, found_idx), x, y + 1);	// ECR 7673 - changed to +1
            list_set(used_nodes, found_idx, True);
// ECR 7570 begin
            x = gde_node_x(list_get(comp_gde_nodes, found_idx)); 
            y = gde_node_y(list_get(comp_gde_nodes, found_idx)); 
// ECR 7570 end
            continue;
        }
        // ECR 6432 end

        // Get new x,y and move operation into class
// ECR 7570 begin
//        x = gde_node_x(class_sym);
//        y = gde_node_y(class_sym);
// ECR 7570 end
        gde_node_move_to(op_sym, x, y + 1);	// ECR 5873 - added +16	, ECR 7673 - changed to +1
// ECR 7570 begin
        x = gde_node_x(op_sym); 
        y = gde_node_y(op_sym); 
// ECR 7570 end
    }


    // ECR 6432 start
    // now delete all unused (same behaviour as before)
    if (clear_first && (list_count(attrs) + list_count(ops)) > 0) {
        for(i = 0; i < list_count(used_nodes); ++ i) {
            if(list_get(used_nodes, i) == NULL) {
                gde_node_delete(list_get(comp_gde_nodes, i));
            }
        }
    }

    // finally fix the endpoints of all arcs
    gde_enable_gr_update();
    gde_redraw_diagram();
    gde_disable_gr_update();

    // now the diagram is drawn again
    // the attributes and operations have found their final position
    // start fixing the links
    int dx, dy;
    for(i = 0; i < list_count(comp_gde_nodes); ++ i) {
        comp_node = list_get(comp_gde_nodes, i);
        arcs = gde_node_outarcs(comp_node);
        for(j = 0; j < list_count(arcs); ++ j) {
            arc = list_get(arcs, j);
            if (gde_lt_name(gde_arc_lt(arc)) != "Contains")
                gde_arc_set_dx_dy(arc, 0, 0, 0, 0);
        }

        arcs = gde_node_inarcs(comp_node);
        for(j = 0; j < list_count(arcs); ++ j) {
            arc = list_get(arcs, j);
            if (gde_lt_name(gde_arc_lt(arc)) != "Contains")
                gde_arc_set_dx_dy(arc, 0, 0, 0, 0);
        }
    }
    // ECR 6432 end

    gde_end_edit_transaction(GDE_CMD_INSERT, dodefer, False);


    // SPR 2894 - select the class
    gde_node_select(class_sym);
    
    return;
}

// JED - new layout code
#include "rules/qrl/uml_node_layout.inc"

void
decorate_map_cat_cl_list(gde_node cat_sym, list cats, list classes, string cat_mapping_fn, string cl_mapping_fn, boolean clear_first)
{
    int             dodefer;
    int             i, x, y;
    gde_node        cat, class;

    dodefer = gde_start_edit_transaction();

    list contents;
    if (clear_first && (list_count(cats) + list_count(classes)) > 0) {
        ClearBoochSymbolComponents1(cat_sym, "Contains");
        contents = NULL;
    } else {
    // JED - get existing contents
        contents = gde_node_contained_nodes(cat_sym,False); 
    }
    if (NULL == contents) {
        contents = list_create("gde_node",0);
    }

    

    x = gde_node_x(cat_sym);
    y = gde_node_y(cat_sym);

    for (i = 0; i < list_count(cats); i++)
    {
        cat = gde_node_create("Package", x, y);
        qrl_eval("gde_node_set_label(cat, " + cat_mapping_fn + "(list_get(cats, i)));");
        list_append(contents,cat);
    }

    //ECR 4986
    string class_type; 
    node class_node; 
    //ECR 4986
    for (i = 0; i < list_count(classes); i++)
    {
        //ECR 4986 begin
        //class = gde_node_create("Class", x, y);
        class_node = list_get(classes, i);
        if (NULL != find_by_query("node[UmlParameterizedClass && name='${class_node.name}']"))
            class_type = "ParameterizedClass";
        else if (NULL != find_by_query("node[UmlInterface && name='${class_node.name}']"))
            class_type = "Interface";
        else
            class_type = "Class";
        class = gde_node_create(class_type, x, y);
        //ECR 4986 end
        qrl_eval("gde_node_set_label(class, " + cl_mapping_fn + "(list_get(classes, i)));");
        list_append(contents,class);
    }
    // JED - layout the nodes
    uml_node_layout(cat_sym,contents);

    // Added for ECR4193
    gde_map_all_symbols();
    gde_end_edit_transaction(GDE_CMD_INSERT, dodefer, False);

    return;
}
// JED - merge in the booch class.inc

////////////////////////////////////////////////////////////////////////////////
//
// class menu functions (modelled after, i.e., stolen from) ome.inc

const    int    ACTIVE_FUNC_IS_UNDEFINED = -1; // SAME AS OMT

list    the_last_selection = NULL; // SAME AS OMT

int    idehook_class_selected_active_func_lv = ACTIVE_FUNC_IS_UNDEFINED;
int    idehook_multiple_class_selected_active_func_lv = ACTIVE_FUNC_IS_UNDEFINED;
int    idehook_cat_selected_active_func_lv = ACTIVE_FUNC_IS_UNDEFINED;
int    idehook_single_class_selected_active_func_lv = ACTIVE_FUNC_IS_UNDEFINED;
int    idehook_single_cat_selected_active_func_lv = ACTIVE_FUNC_IS_UNDEFINED;
int    idehook_attributed_association_active_func_lv = ACTIVE_FUNC_IS_UNDEFINED;
int    idehook_or_association_active_func_lv = ACTIVE_FUNC_IS_UNDEFINED;
int    idehook_comment_link_active_func_lv = ACTIVE_FUNC_IS_UNDEFINED;
int    idehook_relationship_selected_active_func_lv = ACTIVE_FUNC_IS_UNDEFINED;
int    idehook_mult_permitted_active_func_lv = ACTIVE_FUNC_IS_UNDEFINED;
int    idehook_add_remove_from_package_active_func_lv = ACTIVE_FUNC_IS_UNDEFINED;
// Begin ECR947, ECR 4494
int    idehook_note_link_active_func_lv = ACTIVE_FUNC_IS_UNDEFINED;
// End ECR947, ECR 4494

boolean
idehook_invalidate_all()
{
    idehook_class_selected_active_func_lv = ACTIVE_FUNC_IS_UNDEFINED; 
    idehook_multiple_class_selected_active_func_lv = ACTIVE_FUNC_IS_UNDEFINED;
    idehook_cat_selected_active_func_lv = ACTIVE_FUNC_IS_UNDEFINED; 
    idehook_single_class_selected_active_func_lv = ACTIVE_FUNC_IS_UNDEFINED; 
    idehook_single_cat_selected_active_func_lv = ACTIVE_FUNC_IS_UNDEFINED; 
    idehook_attributed_association_active_func_lv = ACTIVE_FUNC_IS_UNDEFINED; 
    idehook_or_association_active_func_lv = ACTIVE_FUNC_IS_UNDEFINED;
    idehook_comment_link_active_func_lv = ACTIVE_FUNC_IS_UNDEFINED; 
    idehook_relationship_selected_active_func_lv = ACTIVE_FUNC_IS_UNDEFINED; 
    idehook_mult_permitted_active_func_lv = ACTIVE_FUNC_IS_UNDEFINED; 
    idehook_add_remove_from_package_active_func_lv = ACTIVE_FUNC_IS_UNDEFINED;
// Begin ECR947, ECR 4494
    idehook_note_link_active_func_lv = ACTIVE_FUNC_IS_UNDEFINED;
// End ECR947, ECR 4494
    return(True);
}

// SAME AS OMT
void
idehook_force_new_selection()
{
    the_last_selection = NULL;
    idehook_invalidate_all();
}

// SAME AS OMT
boolean
idehook_did_selection_change()
{
    list    symbols;

    symbols = gde_selected_symbols();

    if (the_last_selection == NULL)
    {
    the_last_selection = symbols;
    return(idehook_invalidate_all());
    }
    if (list_count(symbols) != list_count(the_last_selection))
    {
    the_last_selection = symbols;
    return(idehook_invalidate_all());
    }
    if (lists_equal(symbols, the_last_selection) == False)
    {
    the_last_selection = symbols;
    return(idehook_invalidate_all());
    }
    return(False);
}

gde_node add_remove_node;

// If this function returns ACTIVE_FUNC_IS_ACTIVE, then add_remove_node
// is set to the one selected node
int
idehook_add_remove_from_package_active_func()
{
    list classes;
    list packages;

    idehook_did_selection_change();

    if (idehook_add_remove_from_package_active_func_lv ==
            ACTIVE_FUNC_IS_UNDEFINED) {
        classes = uml_containable_class_symbols(gde_selected_symbols());
        packages = booch_selected_cat_symbols();
        gde_node the_one;
        list arcs;
        int i;
        gde_arc arc;

        // will only be active if one class or one package is chosen,
        // and that symbol must be graphically contained by a package
        if (list_concatenate(classes,packages) == 1) {
            // classes list will contain both lists
            the_one = list_get(classes,0);

            // for the menu string
            add_remove_node = the_one;

            // check for an incoming Contains arc
            arcs = gde_node_inarcs(the_one);
            if (NULL != arcs) {
                for(i=0;i<list_count(arcs);i++) {
                    arc = list_get(arcs,i);
                    if ("Contains" == gde_lt_name(gde_arc_lt(arc))) {
                        idehook_add_remove_from_package_active_func_lv =
                            ACTIVE_FUNC_IS_ACTIVE;
                        break;
                    }
                }
            }
            if (idehook_add_remove_from_package_active_func_lv ==
                    ACTIVE_FUNC_IS_UNDEFINED) {
                    idehook_add_remove_from_package_active_func_lv =
                        ACTIVE_FUNC_IS_INACTIVE;
            }
        } else {
            idehook_add_remove_from_package_active_func_lv =
                ACTIVE_FUNC_IS_INACTIVE;
        }
    }
    return idehook_add_remove_from_package_active_func_lv;
}


int
UmlGeneralizationActive()
{
    if (is_smdb_available()) 
        return (idehook_single_class_selected_active_func());
    return ACTIVE_FUNC_IS_INACTIVE;
}

int
idehook_single_class_selected_active_func()
{

    idehook_did_selection_change();

    if (idehook_single_class_selected_active_func_lv == ACTIVE_FUNC_IS_UNDEFINED)
    {
    if (list_count(booch_selected_class_symbols()) == 1 && 1 == list_count(gde_selected_symbols()))
    {
        idehook_single_class_selected_active_func_lv = ACTIVE_FUNC_IS_ACTIVE;
            // 12610 fix.
            if (list_count(gde_nodes_find_by_type(gde_selected_symbols(), "Interface")) != 0)
            {
            idehook_single_class_selected_active_func_lv = ACTIVE_FUNC_IS_INACTIVE;
            }
    } else
    {
        idehook_single_class_selected_active_func_lv = ACTIVE_FUNC_IS_INACTIVE;
    }
    }
    return (idehook_single_class_selected_active_func_lv);
}

int
idehook_single_cat_selected_active_func()
{
    idehook_did_selection_change();

    if (idehook_single_cat_selected_active_func_lv == ACTIVE_FUNC_IS_UNDEFINED)
    {
    if (list_count(booch_selected_cat_symbols()) == 1)
    {
        idehook_single_cat_selected_active_func_lv = ACTIVE_FUNC_IS_ACTIVE;
    } else
    {
        idehook_single_cat_selected_active_func_lv = ACTIVE_FUNC_IS_INACTIVE;
    }
    }
    return (idehook_single_cat_selected_active_func_lv);
}

// ECR 7610 begin
int
gde_count_nodes_by_types(list nodes, list types)
{
    int result = 0, i;
    gde_node gn;
    string node_type;

    for (i = 0; i < list_count(nodes); i++)
    {
        gn = list_get(nodes, i);
        node_type = gde_nt_name(gde_node_nt(gn));
        if (list_find(types, 0, node_type) < list_count(types))
            result++;
    }

    return result;
}

void
gde_get_node_adjacent_nodes(gde_node node, gde_arc exclude_arc, list nodes)
{
    int i;
    list arcs;
    gde_arc arc;
    gde_node from_node, to_node;

    arcs = gde_node_outarcs(node);
    for (i = 0; i < list_count(arcs); i++)
    {
        arc = list_get(arcs, i);
        if (arc == exclude_arc)
            continue;

        to_node = gde_arc_tonode(arc);
        if (to_node != NULL && list_find(nodes, 0, to_node) == list_count(nodes))
	        list_append(nodes, to_node);
        if (gde_nt_name(gde_node_nt(to_node)) == VERTEX_SYMBOL_TYPE)
            //gde_get_node_adjacent_nodes(node, arc, nodes);
            gde_get_node_adjacent_nodes(to_node, arc, nodes);
    }
    list_clear(arcs);

    arcs = gde_node_inarcs(node);
    for (i = 0; i < list_count(arcs); i++)
    {
        arc = list_get(arcs, i);
        if (arc == exclude_arc)
            continue;

        from_node = gde_arc_fromnode(arc);
        if (from_node != NULL && list_find(nodes, 0, from_node) == list_count(nodes))
	        list_append(nodes, from_node);
        if (gde_nt_name(gde_node_nt(from_node)) == VERTEX_SYMBOL_TYPE)
            //gde_get_node_adjacent_nodes(node, arc, nodes);
            gde_get_node_adjacent_nodes(from_node, arc, nodes);
    }
    list_clear(arcs);
}

void
gde_get_arc_tonodes(gde_arc assoc_arc, list nodes)
{
    gde_node gn = gde_arc_tonode(assoc_arc);

    if (gde_nt_name(gde_node_nt(gn)) == VERTEX_SYMBOL_TYPE)
        gde_get_node_adjacent_nodes(gn, assoc_arc, nodes);
    else
    {
	    if (gn != NULL && list_find(nodes, 0, gn) == list_count(nodes))
            list_append(nodes, gn);
    }
}

void
gde_get_arc_fromnodes(gde_arc assoc_arc, list nodes)
{
    gde_node gn = gde_arc_fromnode(assoc_arc);

    if (gde_nt_name(gde_node_nt(gn)) == VERTEX_SYMBOL_TYPE)
        gde_get_node_adjacent_nodes(gn, assoc_arc, nodes);
    else
    {
	    if (gn != NULL && list_find(nodes, 0, gn) == list_count(nodes))
            list_append(nodes, gn);
    }
}

int
check_for_free_assoc_class(gde_symbol assoc_sym)
{
    list class_types = string_to_list("Class", " ");
    int total_classes = list_count(gde_symbols_find_with_types(gde_all_symbols(), class_types));
    int classes_in_assoc;

    if (gde_symbol_is_arc(assoc_sym))
    {
        // Binary association
        gde_arc assoc_arc = to_gde_arc(assoc_sym);

        list assoc_from_nodes = list_create("gde_node", 0);
        list assoc_to_nodes = list_create("gde_node", 0);
        
        gde_get_arc_fromnodes(assoc_arc, assoc_from_nodes);
        gde_get_arc_tonodes(assoc_arc, assoc_to_nodes);

        classes_in_assoc = gde_count_nodes_by_types(assoc_from_nodes, class_types)
                         + gde_count_nodes_by_types(assoc_to_nodes, class_types);

        list_clear(assoc_from_nodes);
        list_clear(assoc_to_nodes);
        list_clear(class_types);

        if (classes_in_assoc < total_classes)
            return ACTIVE_FUNC_IS_ACTIVE;
        else
            return ACTIVE_FUNC_IS_INACTIVE;
    }

    if (gde_symbol_is_node(assoc_sym))
    {
        // Nary association
        if (gde_nt_name(gde_node_nt(to_gde_node(assoc_sym))) == "NaryAssociationHub")
        {
            list nodes = list_create("gde_node", 0);
            gde_get_node_adjacent_nodes(to_gde_node(assoc_sym), NULL, nodes);
        
            classes_in_assoc = gde_count_nodes_by_types(nodes, class_types);

            list_clear(nodes);
            list_clear(class_types);

            if (classes_in_assoc < total_classes)
                return ACTIVE_FUNC_IS_ACTIVE;
            else
                return ACTIVE_FUNC_IS_INACTIVE;
        }
    }

    return ACTIVE_FUNC_IS_INACTIVE;
}
// ECR 7610 end

int
idehook_attributed_association_active_func()
{
    list            symbols;

    idehook_did_selection_change();

    if (idehook_attributed_association_active_func_lv != ACTIVE_FUNC_IS_UNDEFINED)
    return (idehook_attributed_association_active_func_lv);

    idehook_attributed_association_active_func_lv = ACTIVE_FUNC_IS_INACTIVE;
    symbols = the_last_selection;

    if (list_count(symbols) == 1)
    {
    if (list_count(gde_symbols_find_with_types(symbols, booch_class_assoc_types())) == 1)
// ECR 7610 begin
//        idehook_attributed_association_active_func_lv = ACTIVE_FUNC_IS_ACTIVE;
        idehook_attributed_association_active_func_lv = 
            check_for_free_assoc_class(list_get(gde_symbols_find_with_types(symbols, booch_class_assoc_types()), 0));
// ECR 7610 end
    } else if (list_count(symbols) == 2)
    {
    if (list_count(gde_symbols_find_with_types(symbols, booch_class_assoc_types())) == 1 &&
        list_count(gde_symbols_find_with_types(symbols, booch_class_types())) == 1)
// ECR 7610 begin
//        idehook_attributed_association_active_func_lv = ACTIVE_FUNC_IS_ACTIVE;
        idehook_attributed_association_active_func_lv = 
            check_for_free_assoc_class(list_get(gde_symbols_find_with_types(symbols, booch_class_assoc_types()), 0));
// ECR 7610 end
    }
    return (idehook_attributed_association_active_func_lv);
}

int
idehook_class_selected_active_func()
{
    idehook_did_selection_change();

    if (idehook_class_selected_active_func_lv == ACTIVE_FUNC_IS_UNDEFINED)
    {
    if (list_count(booch_selected_class_symbols()) != 0)
    {
        idehook_class_selected_active_func_lv = ACTIVE_FUNC_IS_ACTIVE;
            // 12610 fix.
            if (list_count(gde_nodes_find_by_type(gde_selected_symbols(), "Interface")) != 0)
            {
            idehook_class_selected_active_func_lv = ACTIVE_FUNC_IS_INACTIVE;
            }
    } else
    {
        idehook_class_selected_active_func_lv = ACTIVE_FUNC_IS_INACTIVE;
    }
    }
    return (idehook_class_selected_active_func_lv);
}

int
idehook_multiple_class_selected_active_func()
{
    idehook_did_selection_change();

    if (idehook_multiple_class_selected_active_func_lv == ACTIVE_FUNC_IS_UNDEFINED)
    {
    if (list_count(booch_selected_class_symbols()) >= 2)
    {
        idehook_multiple_class_selected_active_func_lv = ACTIVE_FUNC_IS_ACTIVE;
    } else
    {
        idehook_multiple_class_selected_active_func_lv = ACTIVE_FUNC_IS_INACTIVE;
    }
    }
    return (idehook_multiple_class_selected_active_func_lv);
}

int
idehook_cat_capture_active_func()
{
    if (idehook_PE() != "ParcPlace" )
    return ACTIVE_FUNC_IS_INACTIVE;
    return idehook_cat_selected_active_func();
}

int
idehook_cat_selected_active_func()
{
    idehook_did_selection_change();

    if (idehook_cat_selected_active_func_lv == ACTIVE_FUNC_IS_UNDEFINED)
    {
    if (list_count(booch_selected_cat_symbols()) != 0)
    {
        idehook_cat_selected_active_func_lv = ACTIVE_FUNC_IS_ACTIVE;
    } else
    {
        idehook_cat_selected_active_func_lv = ACTIVE_FUNC_IS_INACTIVE;
    }
    }
    return (idehook_cat_selected_active_func_lv);
}

int
idehook_relationship_selected_active_func()
{
    /*
     * will succeed with any relationship between classes (ass, has, use,
     * inh, ins, met)
     */

    list            symbols;

    idehook_did_selection_change();

    if (idehook_relationship_selected_active_func_lv != ACTIVE_FUNC_IS_UNDEFINED)
    return (idehook_relationship_selected_active_func_lv);

    idehook_relationship_selected_active_func_lv = ACTIVE_FUNC_IS_INACTIVE;
    symbols = the_last_selection;

    if (booch_symbols_is_class_relationship(symbols))
    idehook_relationship_selected_active_func_lv = ACTIVE_FUNC_IS_ACTIVE;
    return idehook_relationship_selected_active_func_lv;
}

// JED - this will succeed if 2 or more association links are selected
int
idehook_or_association_active_func()
{
    list            symbols;

    idehook_did_selection_change();

    if (idehook_or_association_active_func_lv != ACTIVE_FUNC_IS_UNDEFINED)
    return (idehook_attributed_association_active_func_lv);

    idehook_or_association_active_func_lv = ACTIVE_FUNC_IS_INACTIVE;
    symbols = the_last_selection;

    if (list_count(gde_symbols_find_with_types(symbols, booch_assoc_types())) > 1) {
        idehook_or_association_active_func_lv = ACTIVE_FUNC_IS_ACTIVE;
    } 
    return (idehook_or_association_active_func_lv);
}

list 
omt_selected_class_symbols()
{
  return booch_selected_class_symbols();
}

list 
booch_selected_class_symbols()
{
  return booch_class_symbols(gde_selected_symbols());
}

list 
booch_selected_cat_symbols()
{
  return booch_cat_symbols(gde_selected_symbols());
}

list 
booch_class_symbols(list symbols)
{
  /* returns a list of all the class nodes in the list symbols */

  list classes;
  gde_node class;
  int i;

  classes = list_create("gde_node", 0);
  for (i = 0; i < list_count(symbols); i++) {
    class = booch_symbol_to_class(list_get(symbols, i));
    if (class != NULL)
      list_append(classes, class);
  }

  return classes;
}

list 
uml_containable_class_symbols(list symbols)
{
  /* returns a list of all the class nodes in the list symbols */

  list classes;
  gde_node class;
  gde_symbol sym;
  int i;

  classes = list_create("gde_node", 0);
  for (i = 0; i < list_count(symbols); i++) {
      sym = list_get(symbols,i);
      if (!gde_symbol_is_node(sym))
          continue;
      class = to_gde_node(sym);
      // append if either InstantiatedClass or has application tag
      // for ClassSymbol.
      if ( ("InstantiatedClass" == gde_nt_name(gde_node_nt(class))) ||
              (NULL != (class = booch_symbol_to_class(sym)))) {
          list_append(classes, class);
      }
  }

  return classes;
}

list 
booch_cat_symbols(list symbols)
{
  /* returns a list of all the cat nodes in the list symbols */

  list cats;
  gde_node cat;
  int i;

  cats = list_create("gde_node", 0);
  for (i = 0; i < list_count(symbols); i++) {
    cat = booch_symbol_to_cat(list_get(symbols, i));
    if (cat != NULL)
      list_append(cats, cat);
  }

  return cats;
}

gde_node
booch_symbol_to_class(gde_symbol symbol)
{
  /* if the symbol is a class, return it as a gde_node, otherwise return NULL */

  gde_node node;

  /* classes are nodes */
  if (gde_symbol_is_node(symbol)) {
    node = to_gde_node(symbol);
    if (gde_nt_application_tag(gde_node_nt(node)) == "ClassSymbol" )  
      return (node);
  }
  return (NULL);
}

gde_node
booch_symbol_to_cat(gde_symbol symbol)
{
  /* if the symbol is a cat, return it as a gde_node, otherwise return NULL */

  gde_node node;

  /* cats are nodes */
  if (gde_symbol_is_node(symbol)) {
    node = to_gde_node(symbol);
    if (gde_nt_name(gde_node_nt(node)) == "Package" )  
      return (node);
  }
  return (NULL);
}

// SAME AS OMT
gde_node
omt_symbol_to_operation(gde_symbol symbol)
{
  /* if the symbol is an operation, return it as a gde_node, otherwise return NULL */

  gde_node node;

  /* classes can be either nodes or cntxs */
  if (gde_symbol_is_node(symbol)) {
    node = to_gde_node(symbol);
    if (gde_nt_name(gde_node_nt(node)) == "Operation")
      return (node);    
  }
  return (NULL);    
}

// SAME AS OMT
gde_cntx
omt_symbol_to_role(gde_symbol symbol)
{
  /* if the symbol is a role, return it as a gde_cntx, otherwise return NULL */

  gde_cntx cntx;
  string roletype;

  if (gde_symbol_is_cntx(symbol)) {
    cntx = to_gde_cntx(symbol);
    roletype = gde_ct_name(gde_cntx_ct(cntx));
    if (roletype == FROM_ROLE_CNTX_TYPE || roletype == TO_ROLE_CNTX_TYPE)
      return (cntx);
  }
  return (NULL); 
}

list            The_booch_assoc_types = NULL;
list
booch_assoc_types()
{
    if (The_booch_assoc_types == NULL)
    The_booch_assoc_types = gde_rules_typeset("AssociationLinks");
    return (The_booch_assoc_types);
}

list            The_booch_class_assoc_types = NULL;
list
booch_class_assoc_types()
{
    if (The_booch_class_assoc_types == NULL)
    The_booch_class_assoc_types = gde_rules_typeset("CanHaveAssocClass");
    return (The_booch_class_assoc_types);
}

list            The_booch_class_types = NULL;
list
booch_class_types()
{
    if (The_booch_class_types == NULL)
    The_booch_class_types = gde_rules_typeset("AllClassTypes");
    return (The_booch_class_types);
}

boolean
booch_is_class_symbol(gde_node class)
{
    return (gde_nt_application_tag(gde_node_nt(class)) == "ClassSymbol");
}

boolean
booch_is_class_symbol_nested(gde_node class)
{
    list            arcs = gde_node_inarcs(class);
    return !(list_count(arcs) == 0 || list_count(gde_arcs_find_by_type(gde_arc_list_to_sym_list(arcs), "Contains")) == 0);
}

list            The_booch_relationship_types = NULL;
boolean
booch_symbols_is_class_relationship(list symbols)
{
    if (The_booch_relationship_types == NULL)
    The_booch_relationship_types = gde_rules_typeset("LinkReplaceTypeset");

    /*
     * accept a set of arcs that are part of a single link that connect two
     * classes
     */

    if (symbols == NULL || list_count(symbols) == 0)
    return False;

    int             i;
    gde_symbol      symbol;
    list            links = list_create("gde_link", 0);
    for (i = 0; i < list_count(symbols); i++)
    {
    symbol = list_get(symbols, i);
    if (!gde_symbol_is_arc(symbol) ||
        list_find(The_booch_relationship_types, 0, gde_lt_name(gde_arc_lt(to_gde_arc(symbol)))) == list_count(The_booch_relationship_types) ||
        list_count(gde_arc_links(to_gde_arc(symbol))) != 1)
        return False;
    list_concatenate(links, gde_arc_links(to_gde_arc(symbol)));
    }

    int             j;
    gde_link        link, l;
    for (i = 0; i < list_count(links); i++)
    {
    link = list_get(links, i);
        for (j = i + 1; j < list_count(links); j++)
    {
        l = list_get(links, j);
        if (gde_link_arcs(link) != gde_link_arcs(l))
        return False;
    }
    }

    link = list_get(links, 0);

    string          from = gde_nt_name(gde_node_nt(gde_link_fromnode(link)));
    string          to = gde_nt_name(gde_node_nt(gde_link_tonode(link)));
    if (list_find(booch_class_types(), 0, from) == list_count(booch_class_types()) ||
    list_find(booch_class_types(), 0, to) == list_count(booch_class_types()))
    return False;
    return True;
}

gde_node
find_floating_container_in_class(gde_node class)
{
    int             i, j;
    gde_node        floater;
    list            arcs, links;

    arcs = gde_node_outarcs(class);
    for (i = 0; arcs != NULL && i < list_count(arcs); i++)
    {
    links = gde_arc_links(list_get(arcs, i));
    for (j = 0; links != NULL && j < list_count(links); j++)
    {
        floater = gde_link_tonode(list_get(links, j));
        if (gde_nt_name(gde_node_nt(floater)) == BOOCH_ATTROP_CONTAINER_SYMBOL_TYPE)
        return floater;
    }
    }
    return NULL;
}

gde_node
find_class_from_attrop(gde_node attrop)
{
    int             i, j;
    gde_node        class;
    list            arcs, links;

    arcs = gde_node_inarcs(attrop);
    for (i = 0; arcs != NULL && i < list_count(arcs); i++)
    {
    links = gde_arc_links(list_get(arcs, i));
    for (j = 0; links != NULL && j < list_count(links); j++)
    {
        class = gde_link_fromnode(list_get(links, j));
        if (booch_is_class_symbol(class))
        return class;
    }
    }
    return NULL;
}

gde_node
find_floater_from_attrop(gde_node attrop)
{
    int             i, j;
    gde_node        floater;
    list            arcs, links;

    arcs = gde_node_inarcs(attrop);
    for (i = 0; arcs != NULL && i < list_count(arcs); i++)
    {
    links = gde_arc_links(list_get(arcs, i));
    for (j = 0; links != NULL && j < list_count(links); j++)
    {
        floater = gde_link_fromnode(list_get(links, j));
        if (gde_nt_name(gde_node_nt(floater)) == BOOCH_ATTROP_CONTAINER_SYMBOL_TYPE)
        return floater;
    }
    }
    return NULL;
}

////////////////////////////////////////////////////////////////////////////////
//
// nav and context

void
BoochCategoryClassDiagramInit(int oid)
{
    EditorShowOnNavReceive();
    list scope_chain = scope_list_from_object(find_by_query("node[id=${oid}]"));
    if (scope_chain == NULL || list_count(scope_chain) == 0)
    return;
    string filename = make_filename_from_scope_chain(scope_chain, 1);
    if (list_count(scope_chain) == 1)
    filename = create_new_file_in_sequence(filename,
                        current_editor_apptype());
    gde_builtin("FileLoad " + filename);
    if (gde_diagram_name() != filename)
    return;
    booch_set_class_diag_context(scope_chain);
    return;
}

////////////////////////////////////////////////////////////////////////////////
//
// ui hooks

boolean         ClassEditorActionDoneXXX__entered = False;

void
ClassEditorAction__begin()
{
    ClassEditorActionDoneXXX__entered = True;
    return;
}

void
ClassEditorAction__end()
{
    ClassEditorActionDoneXXX__entered = False;
    return;
}

void
ClassEditorActionDoneBefore()
{
    int             action;

    /*
     * do any work before or, more importantly, post any work to be done at
     * action-completed time (on a command-type basis)
     */

    if (ClassEditorActionDoneXXX__entered)    /* serialized and non-reentrant */
    return;

    ClassEditorActionDoneXXX__entered = True;

    action = gde_node_sym_current_action();
    if (action == GDE_CMD_INSERT)
    ClassEditorActionDoneBefore__Insert(gde_handle_node_sym_symbols());
    /* any others? */

    ClassEditorActionDoneXXX__entered = False;

    return;
}

void
ClassEditorActionDone()
{

    /*
     * do any work after or, more importantly, process any work posted from
     * action-before time (on a command-type basis)
     */

    if (ClassEditorActionDoneXXX__entered)    /* serialized and non-reentrant */
    return;

    ClassEditorActionDoneXXX__entered = True;

    ClassEditorActionDone__Insert();
    /* any others? */

    ClassEditorActionDoneXXX__entered = False;

    return;
}

void
ClassEditorActionDoneBefore__Insert(list symbols)
{
    int             i, j;
    gde_symbol      symbol;
    list            cats, classes;
    gde_node        inserting, cat, class;
    string          inserting_typename;

    if (symbols == NULL)
    return;

    /*
     * if the user drops a cat/class on a cat, then transform it into a
     * component of the cat (e.g., turn the class into a class name)
     */
    for (i = 0; i < list_count(symbols); i++)
    {
    symbol = list_get(symbols, i);
    if (gde_symbol_is_node(symbol))
    {
        inserting = to_gde_node(symbol);
        inserting_typename = gde_nt_name(gde_node_nt(inserting));

        /* dropped a cat/class on a cat */
        if (booch_is_class_symbol(inserting) || inserting_typename == "Package")
        {
        cats = gde_find_nodes_by_location(gde_node_x(inserting), gde_node_y(inserting));
        for (j = 0; cats != NULL && j < list_count(cats); j++)
        {
            cat = list_get(cats, j);
            if (gde_nt_name(gde_node_nt(cat)) == "Package" )
            {
            if (inserting_typename == "Package"  && cat == inserting)
                continue;  /* exclude the cat if it's the one we're inserting! */
            if (WorkQueue__DropTheCat == NULL)
                WorkQueue__DropTheCat = list_create("gde_symbol", 0);
            list_append(WorkQueue__DropTheCat, symbol);
            break;
            }
        }
        continue;
        }

        /* drop an attr/op on a class */
        /*******************************
        if (inserting_typename == OMT_ATTRIBUTE_SYMBOL_TYPE || inserting_typename == OMT_OPERATION_SYMBOL_TYPE)
        {
        classes = gde_find_nodes_by_location(gde_node_x(inserting), gde_node_y(inserting));
        for (j = 0; classes != NULL && j < list_count(classes); j++)
        {
            class = list_get(classes, j);
            if (booch_is_class_symbol(class))
            {
            if (WorkQueue__DropTheAttrOp == NULL)
                WorkQueue__DropTheAttrOp = list_create("gde_symbol", 0);
            list_append(WorkQueue__DropTheAttrOp, symbol);
            break;
            }
        }
        continue;
        }
        *******************************/

    }
    }

    /* any others? */

    return;
}

boolean         ClassEditorActionDone__Insert = False;
void
ClassEditorActionDone__Insert()
{
    int             i, dodefer;
    gde_symbol      symbol;
    gde_node        inserting, class;
    string          inserting_typename;

    if (WorkQueue__DropTheCat != NULL)
    {
        dodefer = gde_start_edit_transaction();
    for (i = 0; i < list_count(WorkQueue__DropTheCat); i++)
    {
        symbol = list_get(WorkQueue__DropTheCat, i);
        if (!gde_symbol_valid(symbol))
        continue;
        inserting = to_gde_node(symbol);
        inserting_typename = gde_nt_name(gde_node_nt(inserting));
        if (inserting_typename == "Package" )
        gde_node_replace_nt(inserting, "PackageName" );
        else
        gde_node_replace_nt(inserting, "ClassName" );
    }
    WorkQueue__DropTheCat = NULL;
    gde_end_edit_transaction(GDE_CMD_INSERT, dodefer, False);
    }
    if (WorkQueue__DropTheAttrOp != NULL)
    {
        dodefer = gde_start_edit_transaction();
    for (i = 0; i < list_count(WorkQueue__DropTheAttrOp); i++)
    {
        symbol = list_get(WorkQueue__DropTheAttrOp, i);
        if (!gde_symbol_valid(symbol))
        continue;
        inserting = to_gde_node(symbol);
        class = find_class_from_attrop(inserting);
        if (class == NULL)
        continue;
        gde_node        floater = find_floating_container_in_class(class);
        if (floater == NULL)
        floater = gde_node_create(BOOCH_ATTROP_CONTAINER_SYMBOL_TYPE, gde_node_x(class), gde_node_y(class));
        if (find_floater_from_attrop(inserting) == NULL)
        {
        gde_select_node(inserting);
        gde_builtin("SymbolMove " + gde_node_x(floater) + " " + gde_node_y(floater));
        }

    }
    WorkQueue__DropTheAttrOp = NULL;
    gde_end_edit_transaction(GDE_CMD_INSERT, dodefer, False);
    }
    /* any others? */

    return;
}

list            WorkQueue__DropTheCat = NULL;    
list            WorkQueue__DropTheAttrOp = NULL;    

////////////////////////////////////////////////////////////////////////////////
void
CreateAttributedAssociation()
{
    list            symbols, assocs, targets;
    gde_symbol      sym;
    gde_arc         assoc;
    gde_node        target;
    gde_node        n_anchor = NULL;
    gde_cntx        c_anchor = NULL;
 
    symbols = gde_selected_symbols();
    assocs = gde_symbols_find_with_types(symbols, booch_class_assoc_types());
    targets = gde_symbols_find_with_types(symbols, booch_class_types());
 
    if (list_count(assocs) != 1 || list_count(targets) > 1)
    {
    gde_print_error("Incorrect symbol(s) selected for Create Association Class.");
    return;
    }
    sym = list_get(assocs, 0);
    if (gde_symbol_is_node(sym)) {
        n_anchor = to_gde_node(sym);
    } else {
        assoc = to_gde_arc(sym);
        c_anchor = create_anchor_on_arc(assoc, "AssociationAnchor");
        sym = to_gde_symbol(c_anchor);
    }
 
    if (list_count(targets) == 1)
    {
        target = to_gde_node(list_get(targets, 0));
        if (NULL != n_anchor)
            gde_arc_create("AssociationClassLink", n_anchor, target);
        else
            gde_arc_create("AssociationClassLink", c_anchor, target);
    } else
    {
    start_an_arc_from(sym,
              "AssociationClassLink",
     "Please select a class to represent the association class...",
              False);
    }
    return;
}

// ECR947, ECR 4494
// moved create_anchor_on_arc to note_link.inc


gde_mapsymbol
booch_attributed_association_source_symbol(gde_mapsymbol aa_sym)
{
    gde_link        aa;
    gde_arc         anchor_arc;
    gde_cntx        anchor;
    gde_node        hub;

    aa = to_gde_link(aa_sym);
    hub = gde_link_fromnode(aa);

    if (NULL != hub && "NaryAssociationHub" == gde_nt_name(gde_node_nt(hub))) {
        // this is only true if we are associating to an association hub
        return to_gde_mapsymbol(hub);
    } else {
        // must be to an anchor
        anchor = gde_link_fromcntx(aa);
        anchor_arc = gde_cntx_parent(anchor);
        return to_gde_mapsymbol(list_get(gde_arc_links(anchor_arc), 0));
    }
}


void
CreateOrAssociation()
{
    CreateAnyConstraint("AssociationOr",
        "AssociationAnchor",
        booch_assoc_types(), "{Or}");
}

const string SEL_CONSTRAINT_TARGET_PROMPT = "Please Select the target for the Constraint";

void
CreateAnyConstraint(string link_tp, string anchor_tp, list types, string lab)
{
    int        arc_id;
    list    symbols, sends, cl_cons;
    gde_arc    a;
    gde_cntx    ca;
    gde_node    n;
    gde_node    c1, c2;
    gde_symbol    s, s1, s2;
    int        cnt;
    int        x, y;
    string    clink_type = link_tp;
    string    tp;
    string    t1, t2;
    int        i, csyms;


    symbols = gde_selected_symbols();

    cnt = list_count(symbols);
    if (cnt < 1 || cnt > 2)
    {
        return;
    }

    sends = gde_symbols_find_with_types(symbols, types);

    cnt = list_count(sends);
    if (cnt < 1 || cnt > 2)
    {
        return;
    }

    // Don't change the sequence of the list get
    // I know this is really implementation dependent, but the selection stuff
    // allocates things backwards, so the first selected thing becomes
    // the last in the list .....

    if (cnt == 2)
    {
        s = list_get(sends, 1);

        if (gde_symbol_is_arc(s))
        {
            a = to_gde_arc(s);
            ca = create_anchor_on_arc(a, anchor_tp);
            s1 = to_gde_symbol(ca);
        } else if (gde_symbol_is_node(s) || gde_symbol_is_cntx(s))
        {
            s1 = s;
        }
    }

    s = list_get(sends, 0);

    if (gde_symbol_is_arc(s))
    {
        a = to_gde_arc(s);
        ca = create_anchor_on_arc(a, anchor_tp);
        s2 = to_gde_symbol(ca);
        clink_type = link_tp;
    } else if (gde_symbol_is_node(s) || gde_symbol_is_cntx(s))
    {
        s2 = s;
        t2 = gde_get_symbol_type(s2);
    }

    if (cnt == 1)
    {
        if (s2 == NULL)
        {
            return;
        }
        start_an_arc_from(s2, clink_type, SEL_CONSTRAINT_TARGET_PROMPT, False);
        gde_builtin("EditorActionDone " + start_an_arc_from_id + " CreateConstraintArcCompleted();");
        return;
    }

    if (s1 == NULL || s2 == NULL)
    {
        return;
    }

    clink_type = link_tp;

    // print_warning("JED - creating arc of type " + clink_type);
    a = gde_arc_create(clink_type, s1, s2);
    if (a != NULL)
    {
        arc_id = gde_arc_psymid(a);
        gde_builtin(SYM_SEL_APPID_MSG + arc_id);
        if (lab != NULL)
        {
            gde_arc_set_label(a, lab);
        }
    } else {
        print_warning("JED - falied to create arc");
    }
}

void
CreateConstraintArcCompleted()
{
    list    selected_symbols, ctypes;
    int        cnt, i;
    gde_symbol    symbol;
    gde_arc    a = NULL;
    string    tp;
    gde_node    fn;


    selected_symbols = gde_selected_symbols();

    cnt = 0;
    if (start_arc_from_target_link_type != NULL)
    {
    for (i = 0; i < list_count(selected_symbols); i++)
    {
        symbol = list_get(selected_symbols, i);

        if (gde_symbol_is_arc(symbol))
        {
        a = to_gde_arc(symbol);
        fn = gde_arc_fromnode(a);
        if (fn == NULL)
            continue;
        // This is the only way to find out if the
        // operation was successful or if it was terminated
        // with an undo ....
        if (symbol_arc_started_from == to_gde_symbol(fn))
            cnt = cnt + 1;
        }
    }
    }

    CreateArcFromCompleted();

    if (cnt > 0 && a != NULL)
    {
        ctypes = oo_constraint_link_types();
        tp = gde_lt_name(gde_arc_lt(a));
        if (list_find(ctypes, 0, tp) >= list_count(ctypes))
        {
            fn = gde_arc_get_real_tonode(a, True, NULL, NULL);
            if (fn != NULL)
            {
            print_error("Cannot create Constraint to a " +
                gde_nt_name(gde_node_nt(fn)) + " node", NULL);
            }
            gde_arc_delete(a);
        } else 
        {
            gde_arc_set_label(a, "Or");
        }
    }
}

void
uml_create_nary_association()
{
    gde_symbol sym;
    int i;
    gde_node hub;
    gde_node cls;
    
    // get selected symbols
    list selected_symbols = gde_selected_symbols();
    list selected_classes = list_create("gde_node",0);

    // make sure all have application tag for classes
    for(i=0;i<list_count(selected_symbols);i++) {
        sym = list_get(selected_symbols,i);
        cls = booch_symbol_to_class(sym);
        if (NULL == cls) {
            gde_print_error("Non-class symbol selected for N-ary association",
                    sym_get_psymid(sym));
        } else {
            list_append(selected_classes,cls);
        }
    }
    if (list_count(selected_symbols) != list_count(selected_classes)) {
        return; // do nothing, error has already been reported
    }

    if (list_count(selected_classes) < 2) {
        gde_print_error("You must select at least two class symbols for N-ary association",NULL);
    }

    // find center x,y for symbols
    gde_node curnode;

    int x=0;
    int y=0;

    for(i=0;i<list_count(selected_classes);i++) {
        curnode = list_get(selected_classes,i);
        x += gde_node_x(curnode);
        y += gde_node_y(curnode);
    }
    x = x / list_count(selected_classes);
    y = y / list_count(selected_classes);


    // create NaryAssociationHub there
    hub = gde_node_create("NaryAssociationHub",x,y);

    // create link from hub to each class
    for(i=0;i<list_count(selected_classes);i++) {
        gde_arc_create("NaryAssociationRole",hub,list_get(selected_classes,i));
    }
}
    
string
uml_class_id_scope_name(int id)
{
    node n = find_by_query("node[" + id + "]");
    if (NULL == n) {
        return UML_PATH_SEPARATOR;
    }
    list packs = uml_get_containing_packages(n);
    if(NULL == packs || list_count(packs)!=1) {
        return "Many";
    }
    n = list_get(packs,0);
    
    return uml_class_qualified_name(n);
}

// Begin ECR947, ECR 4494
list
uml_note_link_symbols()
{
    // ECR 7480 added  NavigableAssociation
    string link_types = "Generalization Association Aggregation NavigableAssociation Dependency Refines Implements ObjectInstantiation ObjectAssociation AssociationOr Contains AssociationClassLink NaryAssociationRole";
    return string_to_list(link_types, " ");
}
// End ECR947, ECR 4494
