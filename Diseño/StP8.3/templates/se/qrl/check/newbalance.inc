
//      StP/SE
//      Confidential property of Aonix
//      Copyright (c) 1999
//      All rights reserved
//

// This file contains all the balance routines that are shared between
// the dfe and cse balance checking.  In cse this is used to ensure
// that all control flows going into/out of a Cspec bar are actually used 
// within the Cspec.  In dfe it is used to balance Stores, SplitFlows
// and Process decompositions.



// check balance between the two lists of flows.  Leaves marks 
// everywhere there is balance, or flags to say what is wrong. 
// Then a second routine just walks the lists and 
// prints errors based on what is not visited.
//
// This routine needs to pay special care to balancing flows which have more then
// one possible definition, and need to have the proper parent infered from the
// context.  If the problem is that the context is not clear, set the ambiguious
// flag to true.
// 
// Note that for the two inout lists, sets would work better, except that there
// is no equiv. to list_set which preserves the for loop order necessary in many
// places.
//
// whichcheck = 1 for parent/decomp 
//            = 2 for store balance
//            = 3 otherwise

void
CheckFlowBalance(list l1, list l2, string in_or_out, int whichcheck)
{
    int i, j;
    inout_tp io;

    if (_SC_Debug)
    {
        dumpinoutlist(l1, "parent", in_or_out);
        dumpinoutlist(l2, "decomp", in_or_out);
    }

    // step 1: resolve ambiguities.  Will resolve both exact match
    // ambiguities and more difficult ones.

    ResolveAmbiguity(l1, l2);

    // Step 2: Look for an exact match in io lists

    for (i=0; i<list_count(l1); i++)
    {
        io = list_get(l1, i);
        if (io.errstatus != 0) 
            continue;
        j = FindIOByName(l2, io.name);
        if (j != -1)
        {
            io.errstatus = 1;
            list_set(l1, i, io);
            list_set(l2, j, io);
            if (_SC_Debug)
                print_error("Simple Match: " + io.name);
        }
    }

    for (i=0; i<list_count(l2); i++)
    {
        io = list_get(l2, i);
        if (io.errstatus != 0) 
            continue;
        j = FindIOByName(l1, io.name);
        if (j != -1)
        {
            io.errstatus = 1;
            list_set(l1, j, io);
            list_set(l2, i, io);
            if (_SC_Debug)
                print_error("Simple Match: " + io.name);
        }
    }

    // Step 3: No exact match.  Now, we need to search for children 
    // This is very much like the ResolveAmbiguity step, but goes from
    // parents to children, not the other way around.
    // Simplest case is that there are no definitions, just report that
    // and let step 3 check for Balance anyhow.

    // do not stop if the errstatus is anything but 2 (ambiguous), 
    // as SearchForChildren affects both lists.  

    for (i=0; i<list_count(l1); i++)
    {
        io = list_get(l1, i);

        if (list_count(io.possibilities) == 0 || io.errstatus == 2)
            continue;

        if (SearchForChildren(io, l2, whichcheck))
            list_set(l1, i, io);
    }

    for (i=0; i<list_count(l2); i++)
    {
        io = list_get(l2, i);

        if (list_count(io.possibilities) == 0 || io.errstatus == 2)
            continue;

        // for parent/decomp check, allow children flows to exist without 
        // corresponding flows
        if ((io.errstatus == 0) && (whichcheck == 1) && (io.store != NULL) && (io.store == io.name))
            continue;

        if (SearchForChildren(io, l1, whichcheck))
            list_set(l2, i, io);
    }

    // Allow introduction of new flows from stores.
    if (whichcheck == 1)
    {
        for (i=0; i<list_count(l2); i++)
        {
            io = list_get(l2, i);
            if ((io.errstatus != 0) && (io.store != NULL) && (io.store != io.name))
            {
                if (_SC_Debug)
                    print_error("Allowing new flow in child from store: " + io.name);
                io.errstatus = 1;
                list_set(l2, i, io);
            }
        }
    }

    return;
}



int 
CheckStoreMembership(inout_tp store, list l1, list l2)

{
    int i;
    inout_tp io;
    int errors = 0;
    ds_elem_tp store_e, child_e;

    store_e = FindBranchByFid(Forest, list_get(store.possibilities, 0));
    for (i=0; i<list_count(l1); i++)
    {
        io = list_get(l1, i);

        if (io.name == store.name)
            continue;
        if (list_count(io.possibilities) != 1)
            continue; 

        child_e = FindBranchByFid(Forest, list_get(io.possibilities, 0));
        if (! ChildBranch(child_e, store_e))
        {
            print_error("Flow '" + io.name + "' is not a component of Store '" + store.name + "'.");
            print_sc_error("Dfe", CurFile, "Flow '" + io.name + "' is not a component of Store '" + store.name + "'.");
            errors++;
        }
    }
    for (i=0; i<list_count(l2); i++)
    {
        io = list_get(l2, i);

        if (io.name == store.name)
            continue;
        if (list_count(io.possibilities) != 1)
            continue; 

        child_e = FindBranchByFid(Forest, list_get(io.possibilities, 0));
        if (! ChildBranch(child_e, store_e))
        {
            print_error("Flow '" + io.name + "' is not a component of Store '" + store.name + "'.");
            print_sc_error("Dfe", CurFile, "Flow '" + io.name + "' is not a component of Store '" + store.name + "'.");
            errors++;
        }
    }
    return errors;
}


int 
FindIOByName(list l, string name)

{ 
    int i;
    inout_tp io;

    for (i=0; i<list_count(l); i++)
    {
       io = list_get(l, i);
       if (io.name == name)
          return i;
    }
    return -1;
}




//list ParentStorePossibilities;
//list DecompStorePossibilities;

void
OLDInitializeStorePossibilities()

{
    ParentStorePossibilities = list_create("inout_tp", 0);
    DecompStorePossibilities = list_create("inout_tp", 0);
}




void 
OLDCreateParentStorePossibilities(list stores)

{
    int i;
    node store;
    inout_tp io;
    
    for (i = 0; i < list_count(stores); i++)
    {
	store = list_get(stores, i);
        io.name = store.name;
        io.kind = 0;
        io.errstatus = 0;
        io.possibilities = list_create("int", 0);
        GetIOPList(io.possibilities, store.name);
        list_append(ParentStorePossibilities, io);
    }
    return;
}


void 
OLDCreateDecompStorePossibilities(list stores)

{
    int i;
    cnode_tp store;
    inout_tp io;
    
    for (i = 0; i < list_count(stores); i++)
    {
	store = list_get(stores, i);
        io.name = store.node.name;
        io.kind = 0;
        io.errstatus = 0;
        io.possibilities = list_create("int", 0);
        GetIOPList(io.possibilities, store.node.name);
        list_append(DecompStorePossibilities, io);
    }
    return;
}




void 
ResolveAmbiguity(list l1, list l2)

{
    int i;
    inout_tp io;
    list catl;

    for (i=0; i<list_count(l1); i++)
    {
        io = list_get(l1, i);
        if (list_count(io.possibilities) > 1)
        {
            ResolveSingleAmbiguity(io, l2);
            list_set(l1, i, io);
        }
    }

    for (i=0; i<list_count(l2); i++)
    {
        io = list_get(l2, i);
        if (list_count(io.possibilities) > 1)
        {
            ResolveSingleAmbiguity(io, l1);
            list_set(l2, i, io);
        }
    }
}


struct potentialmatch_tp
{ 
    int io_index;
    int l_index;
    int l_io_index;
};



void 
ResolveSingleAmbiguity(inout_tp io, list l)

{
    int i, fid;
    inout_tp l_io;
    list parents;   // inout_tp list
    list potentialparents;
    potentialmatch_tp match;

    // Try for an exact match first.
    potentialparents = CreateNodePossibilityList(io, l);

    // now if there is one element on the list, we are done, otherwise, it is
    // error message time.

    if (list_count(potentialparents) == 1)
    {
        match = list_get(potentialparents, 0);
        fid = list_get(io.possibilities, match.io_index);
        list_clear(io.possibilities);
        list_append(io.possibilities, fid);
        //io.errstatus = 1;

        if (match.l_io_index != 0)
        {
            l_io = list_get(l, l_index);
            fid = list_get(l_io.possibilities, l_io_index);

            list_clear(l_io.possibilities);
            list_append(l_io.possibilities, fid);
            list_set(l, l_index, l_io);
        }
        return;
    }

    // try for an exact match.  This is done second, so that in cases
    // where parts of a store and the whole store are input at a parent
    // level, and there are ambiguities, they will be resolved, which
    // makes the final balance check come out right.

    for (i=0; i<list_count(l); i++)
    {
        l_io = list_get(l, i);
        if (l_io.name == io.name)
        {
            // This is the simple case of ambiguity where both use the 
            // same ambiguity, so we just let this slip on by.  If there 
            // really is a problem, it well get caught at the point of 
            // introduction.  Mark both as visited and OK (errstatus = 1)
            // to effectively be a simple match

            l_io.errstatus = 1;
            list_set(l, i, l_io);
            if (_SC_Debug)
                print_error("Ambiguity pass through match: " + l_io.name);
            io.errstatus = 1;
            return;
        }
    }

    // we failed, issue an error about the ambiguity.
    io.errstatus = 2;

    return;
}




// Go through and create a list of node possibilities

list 
CreateNodePossibilityList(inout_tp io, list l)

{
    int i, j, k, fid, l_fid;
    ds_elem_tp parent, leaf;
    inout_tp l_io;
    int cnt;
    list matches = list_create("potentialmatch_tp", 0);
    potentialmatch_tp pmatch;
    boolean done = False;


    if (io.store != NULL)
        return CreateStorePossibilityList(io, l);

    for (i=0; i<list_count(io.possibilities); i++)
    {
        done = False;
        cnt = 0;
        fid = list_get(io.possibilities, i);
        leaf = FindBranchByFid(Forest, fid);
        while((parent = FindBranchByFid(Forest, leaf.parent_fid)) != NULL)
        {
            cnt++;
            for (j=0; j<list_count(l); j++)
            {
                l_io = list_get(l, j);
                if (_SC_Debug) 
                    print_error("Parent: " + parent.n.name + ", List: " + l_io.name);
                if (parent.n.name == l_io.name)
                {
                    if (list_count(l_io.possibilities) > 1)
                    {
                        for (k=0; k<list_count(l_io.possibilities); k++)
                        {
                            l_fid = list_get(l_io.possibilities, k);
                            if (l_fid == parent.fid)
                            {
                                pmatch.io_index = i;
                                pmatch.l_index = j;
                                pmatch.l_io_index = k;
                                list_append(matches, pmatch);
                                done = True;
                                break;
                            }
                        }
                    }
                    else
                    {
                        pmatch.io_index = i;
                        pmatch.l_index = 0;
                        pmatch.l_io_index = 0;
                        list_append(matches, pmatch);
                        done = True;
                        break;
                    }
                }
            }
            if (done)
                break;

            leaf = parent;

            if (cnt>30)  // Just to keep from an infinite death, if somebody has a dse > 30 levels, well...
            {
                print_error("CreateNodePossibilityList: Loopbreak internal failure");
                break;
            }
        }
    }
    if (_SC_Debug)
        print_error("CreateNodePossibilityList found " + list_count(matches) + " matches.");
    return matches;
}




list 
CreateStorePossibilityList(inout_tp io, list l)

{
    int i, j, k, fid, l_fid;
    ds_elem_tp parent, leaf;
    inout_tp l_io;
    int cnt;
    list matches = list_create("potentialmatch_tp", 0);
    potentialmatch_tp pmatch;
    boolean done = False;

    for (i=0; i<list_count(io.possibilities); i++)
    {
        done = False;
        cnt = 0;
        fid = list_get(io.possibilities, i);
        leaf = FindBranchByFid(Forest, fid);
        while((parent = FindBranchByFid(Forest, leaf.parent_fid)) != NULL)
        {
            cnt++;

            // If parent is the store name, add it to the potential match 
            // and go on to the next io possibility
            if (parent.n.name == io.store)
            {
                // This possibility is a keeper
                pmatch.io_index = i;
                pmatch.l_index = 0;
                pmatch.l_io_index = 0;
                list_append(matches, pmatch);
                done = True;
                break;
            }

            leaf = parent;

            if (cnt>30)  // Just to keep from an infinite death, if somebody has a dse > 30 levels, well...
            {
                print_error("CreateStorePossibilityList: Loopbreak internal failure");
                break;
            }
        }
    }
    if (_SC_Debug)
        print_error("CreateStorePossibilityList found " + list_count(matches) + " matches.");
    return matches;
}





// Look at everything that still has visited = 0, and try to 
// match up a complete set of children.  Note that this is 
// a tad tricky, because a subtree counts for all it's children.
//
// To do it, a deep copy of the parent is created, and marked
// as children are found.

boolean
SearchForChildren(inout_tp io, list l, int whichcheck)

{
    int i, fid, l_fid;
    inout_tp l_io;
    ds_elem_tp single_tree;
    
    if (_SC_Debug)
        print_error("SearchForChildren: " + io.name);

    fid = list_get(io.possibilities, 0);

    single_tree = FindBranchByFid(Forest, fid);

    // if this is a leaf, just get out of here.
    if (list_count(single_tree.branches) == 0)
    {
        if (io.errstatus != 1) 
            io.errstatus = 4;
        return True;
    }

    MarkUnVisited(single_tree);
    // Mark the root visited
    //single_tree.visited = True;

    for (i=0; i<list_count(l); i++)
    {
        l_io = list_get(l, i);
        if (list_count(l_io.possibilities) == 0)
        {
            // no data definition
            continue;
        }
        l_fid = list_get(l_io.possibilities, 0);
        if (MarkVisitedIfExists(single_tree, l_fid))
        {
            if (l_io.errstatus == 0)
            {
                l_io.errstatus = 1;
                list_set(l, i, l_io);
            }
        }
    }

    // If we just called this to check on the other list, bail
    if (io.errstatus == 1)
        return True;

    single_tree.visited = MarkCompletedParentsVisited(single_tree.branches);
    // Now we need to evaluate the results of the single_tree
    // If the whole tree is marked visited, all balances, 
    // If we want to make the error messages better, this tree could
    // be tacked onto the io for printing purposes.

    if (_SC_Debug)
    {
        print_error("Before AllMarkedVisited:");
        dumptree(single_tree, 0);
    }

    if (AllMarkedVisited(single_tree))
        io.errstatus = 1;
    else 
        io.errstatus = 3;
    
    return True;
}


void
dumpinoutlist(list l, string which, string in_or_out)

{
    int i, j, fid;
    inout_tp io;

    print_error("Dumping " + which + " " + in_or_out + " list.");
  
    for (i=0; i<list_count(l); i++)
    {
        io = list_get(l, i);
        print_error("flow: " + io.name + ", errstatus = " + io.errstatus);
        for (j=0; j<list_count(io.possibilities); j++)
        {
            fid = list_get(io.possibilities, j);
            print_error("ds: " + fid);
        }
    }
}


