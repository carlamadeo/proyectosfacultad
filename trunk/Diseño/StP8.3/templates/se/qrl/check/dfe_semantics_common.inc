//
//      StP/SE
//      Confidential property of Aonix
//      Copyright (c) 1999
//      All rights reserved
//


// Misc types from balance.inc


// data_tp
//
// holds the definition of a flow or store plus a reference back to 
// the parent link.

struct data_tp
{
    node	node;		// the node
    link        parent;		// parent link (DataFlow | ControlFlow)
};


// tree_tp
//
// used to build balance trees during decomposition checking.  Ins and Outs
// are traversed to produce lists of balance trees which are then compared
// to establish equality.

struct tree_tp
{
    data_tp	data;		// the node and it's parent link
    list        children;	// child tree_tp's
    int	        matched;	// balaced or not?
    boolean	direct;		// is this match direct or a propagation?
    boolean	propagated;	// has the match already propagated further?
};

// cnode_tp
//
// basic check node structure.  Holds anything that we need more than
// once and hence prevents redundant queries.

struct cnode_tp
{
    node	node;		// oms node (iff type == node)
    link	link;		// oms link (iff type == link)
    data_tp	definition;	// the corresponding data definition
    string      pindex;		// pindex for processes
    string      decompname;	// decomp name for processes
    boolean	haspspec;	// does it have a pspec?
};



// the types of balance checks

enum balance_tp
{
    SplitFlow,
    StoreIn,
    StoreOut,
    DecompositionIn,
    DecompositionOut,
    CspecIn,
    CspecOut,
    TableIn,
    TableOut
};

// propagation_tp

enum propagation_tp
{
    Up,
    Down,
    Search
};

// use to run the semantics check from the driver.qrl program
boolean StandAloneMode = False;

// New routines to support speed enhancements

int __DbgSMytime;
int __DbgSStarttime;

int TIM()
{
    int t; 
    int delta;
    //return time_to_string(time_now(), NULL);
    
    t = time_now();
    if (__DbgSMytime == 0)
    {
        __DbgSStarttime = __DbgSMytime = t;
        return 0;
    }

    delta = t - __DbgSMytime;
    __DbgSMytime = t;
    return delta;
}


int TIMTOTAL()
{
    int t = time_now();
    return t - __DbgSStarttime;
}

// PUT ELSEWHERE
// True if MSJet, False otherwise

boolean __isjetcalled = False;
boolean __jetrep = False;

boolean
JetRepository()
{
    string filename;
    list file_contents;

    if (__isjetcalled)
        return __jetrep;

    __isjetcalled = True;
    filename = path_compose(current_projdir(), current_system(), ".repinfo");

    if ((!file_exists(filename)) || (!read_file_access(filename)))
	return False;
    file_contents = string_to_list(read_file(filename), "\n");
    if (list_find(file_contents, 0 ,"repositoryType=MS Jet") == list_count(file_contents))
        return False;

    __jetrep = True;
    return __jetrep;
}






// from check_dfe.impl


// check that it has a pspec or a decomposition but not both.

int
DfeCheckPspec(file file, cnode_tp cnode)
{
    file      decomp;
    note      pspec;
    
    decomp = find_by_query("file[DfeDiagram && name == '${cnode.decompname}']");
    pspec = find_by_query("note[Pspec && obj_id == ${cnode.node.id}]");

    if (pspec != NULL)
    {
	cnode.haspspec = True;
    } else
    {
	cnode.haspspec = False;
    }
    if (decomp != NULL && pspec != NULL )
    {
	print_error("Process '" + 
		    cnode.node.name + 
		    "' has both decompostion and Pspec.",
		    CnodeAppids(cnode, True, file));
	print_sc_error("Dfe", CurFile, "Process '" + 
		    cnode.node.name + 
		    "' has both decompostion and Pspec.");
	return(1);
    }
    if (decomp == NULL && pspec == NULL)
    {
	print_error("Process '" + cnode.node.name + 
		    "' has no decompostion or Pspec.",
	            CnodeAppids(cnode, True, file));
	print_sc_error("Dfe", CurFile, "Process '" + cnode.node.name + 
		    "' has no decompostion or Pspec.");
	return(1);
    }
    return(0);
}

// check that it has a table or state transition diagram

int
DfeCheckCspec(file file, cnode_tp cnode)
{
    int  i;
    list candidates = list_select("file[name == '${cnode.decompname}']");
    file candidate;

    for (i = 0; i < list_count(candidates); i++)
    {
	candidate = list_get(candidates, i);

	if (candidate.type == "SteDiagram" ||
	    candidate.type == "DecisionTable" ||
	    candidate.type == "ProcessActivationTable" ||
	    candidate.type == "StateTransitionTable" ||
	    candidate.type == "EventLogicTable" ||
	    candidate.type == "ActionLogicTable" ||
	    candidate.type == "ProcessActivationMatrix" ||
	    candidate.type == "StateEventMatrix")
	{
	    return(0);
	}
    }
    print_error("Cspec '" + 
		cnode.node.name +
		"' has no control spec table or state transition diagram.", 
		node_appids(cnode.node, file));
    print_sc_error("Dfe", CurFile, "Cspec '" + 
		cnode.node.name +
		"' has no control spec table or state transition diagram.");
    return(1);
}




// definition cache.  If there is more than one Flow/Store that
// maps to the same data structure then grab the structure from
// the cache rather than querying for it each time.

list structureCache = NULL;

// set this to False to disable the cache

const boolean UseCache = True;

void
cacheClear()
{
    if (UseCache)
    {
	structureCache = list_create("tree_tp", 0);
    }
}

void
cacheAdd(tree_tp t)
{
    int      i;
    tree_tp  tmp;

    if (!UseCache)
    {
	return;
    }
    for (i = 0; i < list_count(structureCache); i++)
    {
	tmp = list_get(structureCache, i);

	if (tmp.data.node.id == t.data.node.id)
	{
	    return;
	}
    }
    list_append(structureCache, t);
}

tree_tp
cacheFind(node n)
{
    int      i;
    tree_tp  tmp;

    if (!UseCache)
    {
	return(NULL);
    }
    for (i = 0; i < list_count(structureCache); i++)
    {
	tmp = list_get(structureCache, i);

	if (tmp.data.node.id == n.id)
	{
	    return(tmp);
	}
    }
    return(NULL);
}


list
CnodeAppids(cnode_tp cnode, boolean isNode, file file)
{
    if (StandAloneMode)
        return NULL;
 
    if (isNode)
	return(node_appids(cnode.node, file));

    return(link_appids(cnode.link, file));
}




// check processes with pspecs to make sure they don't have Control In's

int
DfeCheckControlIns(file file, cnode_tp cnode, list cflows)
{
    list      syms;
    int       i, errors = 0;
    cnode_tp  lcnode;

    for (i = 0; i < list_count(cflows); i++)
    {
	lcnode = list_get(cflows, i);

	if (lcnode.link.to_node_id == cnode.node.id)
	{
	    syms = node_appids(cnode.node, file);
	    list_concatenate(syms, link_appids(lcnode.link, file));

	    print_error("ControlFlow '" + 
			lcnode.link.name + 
			"' flows into Process '" +
			cnode.node.name +
			"' which has a Pspec.",
			syms);
	    print_sc_error("Dfe", CurFile, "ControlFlow '" + 
			lcnode.link.name + 
			"' flows into Process '" +
			cnode.node.name +
			"' which has a Pspec.");
	    errors += 1;
	}
    }
    return(errors);
}

