//////////////////////////////////////////////////////////////////////////////
//
//      sccsid[] = %W% %Y% %D%
//      StP/SE
//      Confidential property of Interactive Development Environments, Inc.
//      Copyright (c) 1992 - 1995
//      All rights reserved
//
//////////////////////////////////////////////////////////////////////////////

// This file contains all the balance routines that are shared between
// the dfe and cse balance checking.  In cse this is used to ensure
// that all control flows going into/out of a Cspec bar are actually used 
// within the Cspec.  In dfe it is used to balance Stores, SplitFlows
// and Process decompositions.


// options_tp
//
// used as a convenient way to pass all the options around.  Saves
// changing a whole bunch of function signatures when a new option is added.

struct options_tp
{
    file	file;		// the file being checked
    int		decomp;		// check in/outflows against parent diag
    int 	balance;	// check in/out flows for stores & splitflows
    int		pspec;		// check for pspec/decomposition
    int		cspec;		// check for cspec table/std
    int 	verbose;
};



// afterbalance_tp
//
// returned from balance function containing any unmatched trees
// or sub-trees.  The caller can then produce errors specific to
// the type of balance check rather than having the generic balance
// function try to do it.

struct afterbalance_tp
{
    list	ins;		// list of un or partially matched in trees
    list	outs;		// list of un or partially matched out trees
};


// check balance between list l and list m.  Used for all balance checks.

afterbalance_tp
CheckBalance(file file, cnode_tp cnode, list l, list m)
{
    int             i;
    list            a, b;
    afterbalance_tp result;

    a = list_copy(l);
    b = list_copy(m);

    result.ins = BuildTrees(a);
    result.outs = BuildTrees(b);

    //message("ins (before):" + result.ins);
    //message("outs (before):" + result.outs);

    PruneForest(result.ins, result.outs);

    //message("ins (after):" + result.ins);
    //message("outs (after):" + result.outs);

    return(result);
}

// take a list of data elements and for each build it's structure tree

list
BuildTrees(list defs)
{
    list     result = list_create("tree_tp", 0);
    int      i;

    if (!list_count(defs))
    {
	return(NULL);
    }
    for (i = 0; i < list_count(defs); i++)
    {
	list_append(result, BuildOneTree(list_get(defs, i)));
    }
    return(result);
}

// take one cnode_tp and build it's structure tree

tree_tp
BuildOneTree(data_tp def)
{
    tree_tp   element;
    string    query, sub;
    node      tmp;
    data_tp   child;
    item      dtype;

    element = cacheFind(def.node);
    
    if (element != NULL)
    {
	if (def.parent != NULL)
	{
	    element.data.parent = def.parent;
	}
	return(element);
    }
    element.data = def;
    element.children = NULL;
    element.matched = 0;
    element.direct = False;
    element.propagated = False;

    child.node = NULL;
    child.parent = NULL;

    query = "node[in_links[link_refs && from_node_id == ${def.node.id}]]";

    for_each_in_select(query, tmp)
    {
        if (tmp.type == "Attribute")
            continue;

	if (element.children == NULL)
	{
	    element.children = list_create("tree_tp", 0);
	}
	child.node = tmp;
	list_append(element.children, BuildOneTree(child));
    }
    if (child.node == NULL)
    {
        /*
         * no outlinks from def... check for type annotation
         */

        dtype = find_by_query("item[obj_id == ${def.node.id} && DataType]");

	if (dtype == NULL)
	{
	    sub = def.node.name;
	} else
	{
	    sub = dtype.value;
	}

	// find the sub-type and graft it on to the tree here

	query = "node[name == '" + to_oms_string(sub) + "' && scope_node[SEFile] && node_refs]";

	tmp = find_by_query(query);

	// a single node with no type decomposes to itself by this
	// scheme so ignore self if returned

	if (tmp != NULL && def.node.id != tmp.id)
	{
	    if (element.children == NULL)
	    {
		element.children = list_create("tree_tp", 0);
	    }
	    child.node = tmp;
	    list_append(element.children, BuildOneTree(child));
	}
    }
    cacheAdd(element);
    return(element);
}

// compare definition trees (in vs. out)

void
PruneForest(list ins, list outs)
{
    int      i, count;

    if (ins == NULL || outs == NULL)
    {
	return;
    }
    for (count = list_count(ins), i = count - 1; i >= 0; i--)
    {
	if (Match(list_get(ins, i), outs) != NULL)
	{
	    list_delete(ins, i);
	    PropagateMatch(outs, Search, False);
	}
    }
    for (count = list_count(outs), i = count - 1; i >= 0; i--)
    {
	if (Match(list_get(outs, i), ins) != NULL)
	{
	    list_delete(outs, i);
	    PropagateMatch(ins, Search, False);
	}
    }
    PropagateMatch(ins, Up, False);
    PropagateMatch(outs, Up, False);
}

// look for the sub-tree (element) in a list of trees
//
// mark the node that it matched at (if it did)
// return the list that we were called with (modified to indicate
// matches) so that it can be reset in parent.  This is required
// since the reference to tree.children causes a copy.

list
Match(tree_tp element, list trees)
{
    int       i;
    tree_tp   tree;
    list      result;

    if (trees == NULL)
	return(NULL);

    // breadth first 

    for (i = 0; i < list_count(trees); i++)
    {
	tree = list_get(trees, i);
	
	if (element.data.node.id == tree.data.node.id)
	{
	    tree.matched = 1;
	    tree.direct = True;
	    tree.propagated = False;

	    // message("*** Match");
	    // message(element.data);
	    // message(tree.data);
	    // message("---");

	    if (element.data.parent != NULL)
	    {
		tree.data.parent = element.data.parent;
	    }
	    list_set(trees, i, tree);
	    return(trees);
	}
    }
    for (i = 0; i < list_count(trees); i++)
    {
	tree = list_get(trees, i);

	result = Match(element, tree.children);

	if (result != NULL)
	{
	    tree.children = result;
	    list_set(trees, i, tree);
	    return(trees);
	}
    }
    return(NULL);
}

// Find where the most recent Match occurred in a list of trees
// and propagate it appropriately.
//
//   Matches in the child of a selection propagate up.
//   Matches in a sequence propagate down.
//   When all sequence children have matched the match propagates up.
//   Matches anywhere in a selection sub-tree propagate up.
// 
// Downward propagations occurs every after new match but upward
// ones should only be performed once (at the end).

list
PropagateMatch(list trees, propagation_tp pt, boolean force)
{
    int       i, n;
    tree_tp   tree;

    if (trees == NULL)
    {
	return(trees);
    }
    for (i = 0; i < list_count(trees); i++)
    {
	tree = list_get(trees, i);

	if (pt == Down)
	{
	    tree.matched = 1;
	    tree.propagated = True;

	    if (tree.data.node.type != "Selection" &&
		tree.data.node.type != "Enumeration")
	    {
		tree.children = PropagateMatch(tree.children, Down, False);
	    }
	} else if (pt == Up)
	{
	    if (tree.data.node.type == "Selection" ||
		tree.data.node.type == "Enumeration")
	    {
		tree.children = PropagateMatch(tree.children, Up, True);

		n = Matched(tree.children, True);

		if ((n > tree.matched) || (force && n > 0))
		{
		    tree.matched = n;
		    tree.propagated = True;
		}
	    } else
	    {
		tree.children = PropagateMatch(tree.children, Up, force);

		if (!tree.matched && tree.children != NULL)
		{
		    n = Matched(tree.children, False);

		    if ((list_count(tree.children) == n) || (force && n > 0))
		    {
			tree.matched = 1;
			tree.propagated = True;
		    }
		}
	    }
	} else // Search
	{
	    if (tree.matched && !tree.propagated)
	    {
		if (tree.data.node.type != "Selection" &&
		    tree.data.node.type != "Enumeration")
		{
		    tree.children = PropagateMatch(tree.children, Down, False);
		}
		tree.propagated = True;
	    } else
	    {
		tree.children = PropagateMatch(tree.children, Search, False);
	    }
	}
	list_set(trees, i, tree);
    }
    return(trees);
}

// return the number of elements in the list that have been matched

int 
Matched(list trees, boolean ReportDuplicates)
{
    int       i, result;
    tree_tp   tree;

    if (trees == NULL)
	return 0;

    for (result = 0, i = 0; i < list_count(trees); i++)
    {
	tree = list_get(trees, i);

	if (ReportDuplicates)
	{
	    result += tree.matched;
	} else
	{
	    if (tree.matched != 0)
	    {
		result += 1;
	    }
	}
    }
    return(result);
}

// find out if a data_tp is already in the list
// can't use list_find since the parent links may differ but I'm
// just interested in whether or not the definition exists

boolean
ListContains(list list, data_tp data)
{
    int      i;
    data_tp  one;

    for (i = 0; i < list_count(list); i++)
    {
	one = list_get(list, i);

	if (one.node.id == data.node.id)
	{
	    return(True);
	}
    }
    return(False);
}

// get data definition of a single node/link
//
// silent mode is for gathering data for objects not referenced on this
// diagram.  We don't want to report errors on them.

int
CnodeGetDataDef(options_tp options, 
		cnode_tp cnode, 
		boolean isNode, 
		boolean silent)
{
    string    name, type, sig, query;
    list      data, appids;
    node      node;
    int       count;
    data_tp   def;

    cnode.definition = NULL;

    if (isNode)
    {
	name = cnode.node.name;
	type = cnode.node.type;
	sig = cnode.node.sig;
	def.parent = NULL;
    } else
    {
	name = cnode.link.name;
	type = cnode.link.type;
	sig = cnode.link.sig;
	def.parent = cnode.link;
    }

//if (FastSemantics)
//    data = GetDataDefsByName(to_oms_string(name));
//else
    data = list_select("node[name == '" + to_oms_string(name) + "' && node_refs[file[DseDiagram]]]");

    count = list_count(data);

    if (count == 1)
    {
	node = list_get(data, 0);
	def.node = node;
	cnode.definition = def;
    }
    //else if (count == 0 && ! FastSemantics)
    else if (count == 0)
    {
	query = "node[(Entity || EntityGroup) && name == '" + 
		to_oms_string(name) + "']";

	node = find_by_query(query);

	if (node == NULL)
	{
	    if (!silent)
	    {
		appids = CnodeAppids(cnode, isNode, options.file);
		
		print_error(type + " '" + name + 
                            "' has no data definition.", 
                            appids);
		print_sc_error("Cse", CurFile, type + " '" + name + 
                            "' has no data definition.");
		return(1);
	    }
	} else
	{
	    def.node = node;
	    cnode.definition = def;
	}
    } 
    //else if (count == 0 && FastSemantics)
    //{
        // Need a proper mechanism for this
        //print_error("Skipping IM query ...");
        //return 1;
    //} 
    else if (count > 1)
    {
	if (sig != NULL && sig != "")
	{
	    node = CnodeQualify(data, name, sig);

	    if (node == NULL)
	    {
		if (!silent)
		{
		    appids = CnodeAppids(cnode, isNode, options.file);

		    print_error(type + " '" + name + " (" + sig + 
				")' has no data definition.", 
				appids);
		    print_sc_error("Cse", CurFile, type + " '" + name + " (" + sig + 
				")' has no data definition.");
		    return(1);
		}
	    } else
	    {
		def.node = node;
		cnode.definition = def;
	    }
	} else
	{
	    if (!silent)
	    {
		appids = CnodeAppids(cnode, isNode, options.file);

		print_error("Ambiguous " + type + " '" + 
			    name + "' has " + count +
			    " potential definitions.", 
			    appids);
		print_sc_error("Cse", CurFile, "Ambiguous " + type + " '" + 
			    name + "' has " + count +
			    " potential definitions.");
		return(1);
	    }
	}
    }
    return(0);
}

node
CnodeQualify(list data, string name, string sig)
{
    list    parts = string_to_list(sig, "::");
    node    dir, file, scope;
    string  query, parent;
    int     i;

    query = "node[SEDirectory && name == '" + 
	    to_oms_string(list_get(parts, 2)) + "']";

    dir = find_by_query(query);

    if (dir == NULL)
	return(NULL);

    query = "node[SEFile && name == '" + 
	    to_oms_string(list_get(parts, 3)) + 
	    "' && scope_node_id == ${dir.id}]";
	    
    file = find_by_query(query);
    
    if (file == NULL)
	return(NULL);

    parent = list_get(parts, 4);

    if (parent == "--")
    {
	query = "node[" + list_get(parts, 1) + " && name == '" +
		to_oms_string(name) + "' && scope_node_id == ${file.id}]";

	return(find_by_query(query));
    } else
    {
	query = "node[name == '" + to_oms_string(parent) + 
		"' && scope_node_id == ${file.id}]";

	scope = find_by_query(query);

	if (scope == NULL)
	    return(NULL);

	for (i = 5; i < list_count(parts); i++)
	{
	    parent = list_get(parts, i);

	    query = "node[name == '" + to_oms_string(parent) +
		    "' && scope_node_id == ${scope.id}]";

	    scope = find_by_query(query);

	    if (scope == NULL)
		return(NULL);
	}
	query = "node[" + list_get(parts, 1) + " && name == '" +
		to_oms_string(name) + "' && scope_node_id == ${scope.id}]";

	return(find_by_query(query));
    }
    return(NULL);
}

