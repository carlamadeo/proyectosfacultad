//
//      sccsid[] = %W% %Y% %D%
//      StP/Booch
//      StP/OMT
//      StP/OO
//	StP Core
//      Confidential property of Interactive Development Environments, Inc.
//      Copyright (c) 1992-1995
//      All rights reserved
//

// Parse the object and interaction diagram syntax:
// For Booch: as defined in the booch book on page 209:
// 	A	Object Name only, class is anonynmous
//	:C	Object Class only, object name is anonymous and distinct
//	A:C	Object name and Class name
// For OMT: as defined in 
// 	A	Object Name only, follow instantiation link and find class
//	:C	Object Class only, object name is anonymous and distinct
//	A:C	Object name and Class name


#include "rules/qrl/gor_object_instance.inc"



struct	obj_name_cl_tp
{
    string	oname;
    string	cname;
    gde_node	n;
    boolean	instancelink;
};

void
debug_obj_name_cl_tp(string w, obj_name_cl_tp r)
{
    message("Obj Name: " + w);
    if (r.oname == NULL)
	message("\toname: NULL");
    else
	message("\toname: '" + r.oname + "'");
    if (r.cname == NULL)
	message("\tcname: NULL");
    else
	message("\tcname: '" + r.cname + "'");
    if (r.instancelink != NULL)
    {
	message("\ti-link: " + r.instancelink);
    }
}

const	string	OBJ_NAME_CLASS_DLM = ":";
const	string	NODE_CLASS_NAME_PROP = "ClassName";

obj_name_cl_tp
mapsym_to_obj_name_str(gde_mapsymbol s)
{
    gde_node	n = to_gde_node(s);

    return(gde_node_to_obj_name_str(n));
}

obj_name_cl_tp
gde_node_to_obj_name_str(gde_node n)
{
    return(string_to_obj_name_str(gde_node_label(n), n));
}

obj_name_cl_tp
string_to_obj_name_str(string s, gde_node n)
{
    string	l;
    list	nl;
    int		sl;
    obj_name_cl_tp	res;
    string	tp;

    res.n     = n;
    res.oname = NULL;
    res.cname = NULL;
    res.instancelink = False;

    l = " " + s + " ";

    nl = string_to_list(l, OBJ_NAME_CLASS_DLM);
    sl = list_count(nl);
    if (sl == 0)
	return(res);
    res.oname = strip_whitespace(list_get(nl, 0));
    if (res.oname == "")
	res.oname = NULL;
    if (sl == 1)
    {
	if (n != NULL)
	{
	    tp = gde_nt_name(gde_node_nt(n));
	    // meta classes and utilities have no object instances ....
	    if (tp == BOOCH_METACLASS_SYMBOL_TYPE ||
		tp == BOOCH_CLASSUTILITY_SYMBOL_TYPE)
	    {
		res.cname = res.oname;
		res.oname = NULL;
		return(res);
	    }
	}
    } else {
	res.cname = strip_whitespace(list_get(nl, 1));
    }
    if (res.cname == "" || res.cname == NULL) // OMT
    {
	if (n != NULL)
	{
	    res.cname = ome_object_instantiation_parent_name(n);
	    if (res.cname != "" && res.cname != NULL)
	    {
		res.instancelink = True;
	    }
	}
    }
    if (res.cname == "")
	res.cname = NULL;
    return(res);
}


//
// Booch: 209 para 1
// If several objects in the same diagram use the same unqualified object
// name then they all denote the same object.
// We therefore generate an anonymous class name based on the
// scope/diagram we are in.
//

const	string	ANONYMOUS_MAP_PROP = "AnonymousName";

void
booch_clear_anonymous_prop(gde_node n, string tp)
{
    string	ap, pn;

    if (tp != NULL)
    {
	pn = ANONYMOUS_MAP_PROP + tp;
    } else {
	pn = ANONYMOUS_MAP_PROP;
    }
    ap = gde_node_prop(n, pn);
    if (ap != NULL && ap != "")
    {
	gde_node_clear_prop(n, pn);
    }
}

// for backwards compatibility when migrating to core 2.0
//

string
booch_13_make_anonymous_name(string prefix, gde_node n)
{
    string	t;

    if (n != NULL)
	t = "__" + gde_node_psymid(n);
    else
	t = "";
    return(ANONYMOUS_PREFIX + prefix + t + "__" + gde_diagram_name());
}


string
booch_make_anonymous_name(string prefix, gde_node n, string tp)
{
    string	t, ap, pn;

    if (n == NULL)
    {
	return(ANONYMOUS_PREFIX + prefix + "__" +
			    create_default_diag_mapping_prop());
    }
    if (tp != NULL)
    {
	pn = ANONYMOUS_MAP_PROP + tp;
    } else {
	pn = ANONYMOUS_MAP_PROP;
    }
    ap = gde_node_prop(n, pn);
    if (ap == NULL)
    {
	t = "__" + gde_node_psymid(n);
	// new created ...
	if (gde_node_label(n) == "")
	{
	    ap = ANONYMOUS_PREFIX + prefix + t + "__" +
				create_default_diag_mapping_prop();
	} else {
	    ap = ANONYMOUS_PREFIX + prefix + t + "__" + gde_diagram_name();
	}
	    
	gde_node_set_prop(n, pn, ap);
    }
    return(ap);
}

string
booch_qualified_object_name(gde_mapsymbol s)
{
    obj_name_cl_tp	r = mapsym_to_obj_name_str(s);

    if (r.oname == NULL)
    {
	if (r.cname != NULL)
	    r.oname = booch_make_anonymous_name("Object" , r.n, "");
    } else {
	booch_clear_anonymous_prop(r.n, "");
    }
    return(r.oname);
}


string
booch_instance_owner_name(gde_mapsymbol s)
{
    obj_name_cl_tp	r = mapsym_to_obj_name_str(s);

    // debug_obj_name_cl_tp("mapsym_to_obj_name_str", r);
    if (r.cname == NULL)
    {
	r.cname = gde_node_prop(r.n, NODE_CLASS_NAME_PROP);
	if (r.cname == "")
	{
	    r.cname = NULL;
	}
	// debug_obj_name_cl_tp("gde_node_prop: " + NODE_CLASS_NAME_PROP, r);
    }
    // If class has no name we make them anonymous ...
    if (r.cname == NULL || r.cname == "")
    {
	// DW 06/13/95
	//
	// Peter, you used to be passing in NULL as the second argument to
	// this routine, but that caused a lot of problems: since the property
	// was never actually set on the node symbol, every time the mapping
	// was evaluated, a new name was created. This had the side effect
	// of generating references with the wrong node_id. Another side
	// effect is that doing a save marked the buffer as changed(!)
	// since you were doing a node_set_prop in the process of evaluating
	// the mappings. (the node_set_prop should have been done only once,
	// when the symbol was first mapped??).
	// Anyway, is it ok to pass in r.n instead of null?
	//
	// r.cname = booch_make_anonymous_name("Class", NULL, "Class");

	r.cname = booch_make_anonymous_name("Class", r.n, "Class");

	// debug_obj_name_cl_tp("booch_make_anonymous_class_name", r);
    } else {
	booch_clear_anonymous_prop(r.n, "Class");
    }

    return(r.cname);
}

string
booch_qualified_class_name(gde_mapsymbol s)
{
    obj_name_cl_tp	r = mapsym_to_obj_name_str(s);

    // debug_obj_name_cl_tp("mapsym_to_obj_name_str", r);
    if (r.cname == NULL)
    {
	r.cname = gde_node_prop(r.n, NODE_CLASS_NAME_PROP);
	if (r.cname == "")
	{
	    r.cname = NULL;
	}
	// debug_obj_name_cl_tp("gde_node_prop: " + NODE_CLASS_NAME_PROP, r);
    }
    // don't make these anonymous
    return(r.cname);
}

void
booch_set_class_type_prop(gde_node n, string c)
{
    gde_node_set_prop(n, NODE_CLASS_NAME_PROP, c);
}


string
booch_label_from_obj_name_cl_tp(obj_name_cl_tp r)
{
    string	l;

    if ((r.oname == "" || r.oname == NULL) &&
	(r.cname == "" || r.cname == NULL || r.instancelink))
	l = "";
    else if (r.oname == NULL || r.oname == "")
	l = OBJ_NAME_CLASS_DLM + r.cname;
    else if (r.cname == NULL || r.cname == "")
	l = r.oname;
    // don't change label if class name comes from instantiation link
    else if (r.instancelink)
	l = r.oname + OBJ_NAME_CLASS_DLM;
    else
	l = r.oname + OBJ_NAME_CLASS_DLM + r.cname;
    return(l);
}

string
ome_object_instantiation_parent_name(gde_node object)
{
    gde_node	cl = ome_object_instantiation_parent(object);
    string	res = NULL;

    if (cl != NULL)
    {
	res = strip_whitespace(gde_node_label(cl));
    }
    return(res);
}


gde_node
ome_object_instantiation_parent(gde_node object)
{
    list	ia = gde_node_outarcs(object);
    list	cnodes;
    int		i;
    gde_node	c;
    gde_arc	inst;

    ia = gde_arcs_find_by_type(gde_arc_list_to_sym_list(ia),
			OMT_OBJ_INSTANTIATION_LINK_SYMBOL);

    for(i = 0; i < list_count(ia); i = i + 1)
    {
	inst = list_get(ia, i);
	cnodes = list_create("gde_node", 0);
	gde_arc_get_real_tonode(inst, False, cnodes, NULL);
	if (list_count(cnodes) > 0)
	{
	    c = list_get(cnodes, 0);
	    return(c);
	}
    }
    return (NULL);
}

//
// Precondition on object symbol, make sure class is defined
//

boolean
booch_instance_has_owner(gde_mapsymbol ms)
{
    obj_name_cl_tp      r = mapsym_to_obj_name_str(ms);

    if (r.cname == NULL || r.cname == "")
	return(False);
    return(True);
}


//
// Name function for object instance attribute
//

string
object_instance_attr_name(gde_mapsymbol ms)
{
    return(object_instance_attr_name_val(ms,
					gde_node_label(to_gde_node(ms)), 0, 0));
}

//
// Signature function for object instance attribute
//

string
object_instance_attr_value(gde_mapsymbol ms)
{
    return(object_instance_attr_name_val(ms, NULL, 1, -1));
}

string
object_instance_attr_name_val(gde_mapsymbol ms, string def, int low, int high)
{
    string	lab = def;
    const	string	INST_ATTR_VAL_DLM = "=";
    list	l;
    int		ll, lc;

    l = string_to_list(gde_node_label(to_gde_node(ms)), INST_ATTR_VAL_DLM);
    lc = list_count(l);
    if (lc > low)
    {
	if (high < low || high >= lc)
	    high = lc-1;

	for(ll = low; ll <= high; ll = ll + 1)
	{
	    if (ll == low)
		lab = list_get(l, ll);
	    else
		lab = lab + INST_ATTR_VAL_DLM + list_get(l, ll);
	}
	lab = string_strip(lab, "B", " ");
    }
    return(lab);
}



void
ome_obj_instance_attr_label(gde_mapsymbol ms, node n, list rninfo,
			    gocha_tp gocha_type)
{
    string		name, value, label = NULL;
    rename_data		thisrename;
    gde_node		rnsym;
    int			i;

    if ( gocha_type != gochaRename )
    {
	print_error( "ome_obj_instance_attr_label() can only handle renames" );
	return; 
    }

    if (!gde_mapsymbol_is_node(ms))
    {
	return;
    }
    rnsym = to_gde_node(ms);
    value = object_instance_attr_value(ms);

    for(i = 0; i < list_count(rninfo); i = i + 1)
    {
	thisrename = list_get(rninfo, i);
	if ((thisrename.dependency_name == GDE_GOR_REN_DEP_NAME) ||
           (thisrename.dependency_name == GDE_GOR_REN_DEP_SELF))
	{
	    label = thisrename.label;
	    break;
	}
    }
    if (label == NULL && n != NULL)
    {
	label = n.name;
    }
    if (value == NULL && n != NULL)
    {
	value = n.sig;
    }
    gde_node_set_label(rnsym, obj_attr_name_val_to_node_label(label, value));
}


string
obj_attr_name_val_to_node_label(string name, string val)
{
    const	string	INST_ATTR_VAL_DLM = "=";
    string	label = "";


    if (name != NULL)
    {
	label = name;
    }
    label = label + " " + INST_ATTR_VAL_DLM + " ";
    if (val != NULL)
    {
	label = label + val;
    }
    return(make_multi_line_label_from_string(label));
}

void
oo_object_instance_label(gde_mapsymbol ms, node n, list rninfo,
			 gocha_tp gocha_type)
{
    string		label = NULL;
    gde_node		rnsym;
    obj_name_cl_tp	r;

    if ( gocha_type != gochaRename )
    {
	print_error( "oo_object_instance_label() can only handle renames" );
	return; 
    }

    if (!gde_mapsymbol_is_node(ms))
    {
	return;
    }
    rnsym = to_gde_node(ms);
    r = gde_node_to_obj_name_str(rnsym);
    if (r.oname != n.name)
    {
	// debug_obj_name_cl_tp("oo_object_instance_label\nOld", r);
	r.oname = n.name;
	// debug_obj_name_cl_tp("New", r);
	label = booch_label_from_obj_name_cl_tp(r);
	gde_node_set_label(rnsym, make_multi_line_label_from_string(label));
    }

}

// Inverse mapping function from class object to object instance symbol

void
oo_object_instance_class_label(gde_mapsymbol ms, node n, list rninfo,
			       gocha_tp gocha_type)
{
    string		label = NULL;
    gde_node		rnsym;
    obj_name_cl_tp	r;

    if ( gocha_type != gochaRename )
    {
	print_error( "oo_object_instance_class_label() can only handle renames" );
	return; 
    }

    if (!gde_mapsymbol_is_node(ms))
    {
	return;
    }
    rnsym = to_gde_node(ms);
    r = gde_node_to_obj_name_str(rnsym);
    // don't change label if class name comes from instantiation link
    if (r.instancelink == True)
    {
	return;
    }

    if (r.cname != n.name)
    {
	// debug_obj_name_cl_tp("oo_object_instance_class_label\nOld", r);
	r.cname = n.name;
	// debug_obj_name_cl_tp("New", r);
	label = booch_label_from_obj_name_cl_tp(r);
	gde_node_set_label(rnsym, make_multi_line_label_from_string(label));
    }
}
