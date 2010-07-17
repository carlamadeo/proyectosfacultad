// things still to to :
// new coordinates should be aligned on the grid
// if a child node has children, and a tag note, rename it with
// the tag name - perhaps, as it may spoil subsequent searches made
// by the synchronizer

const int XDIFF = 320;
const int YDIFF = 192;
const int DSE_XDIFF = 160;
const int FLOW_GAP = 32;

void
SEREReply( int reply_handle, boolean ok )
{
    if ( ok )
      editor_send_msg( "SESynchronize", "OK", reply_handle );
    else
      editor_send_msg( "SESynchronize", "FAILED", reply_handle );
}

// this routine now extended so that it gets all the nore_refs itself for
// the given node-id, and performs all the necessary diagram loads too.
// The kid depth should normally be 1 - this removes a single layer of
// children too
//
int
DeleteNodes(int oms_id, int kid_depth, int reply_handle)
{
    node_ref ref;
    file file_chap;
    string filename;
    int ok = 0, count = 0;

    for_each_in_select( "node_ref[ node_id = " + oms_id + "]", ref )
    {
	count++;
	file_chap = find_by_query( "file[ id = " + ref.file_id + "]" );
	// send a message to load the appropriate file - we do not
	// care what diagram type it is, because it must be accessible to
	// this editor - we should really check the file type matches
	// the type of editor we are running
	LoadDiagramIfNeeded( file_chap.name );
	ok = ok + delete_node( to_int( ref.appid ), kid_depth);
	// now save the diagram
	gde_builtin("FileSave");
	// finally, see if there are any nodes at all on this diagram now
	// if there are not, then delete this file
	if ( list_count( gde_all_symbols()) == 0 )
	{
	    editor_system( "stputil -p " + current_projdir() + 
    	    	  " -s " + current_system() + 
    	          " -Force -Quiet -C \"lock delete -t A -e " +
		  current_editor() +
		  " " + file_chap.name + "\"", NULL );

	    editor_system( "stputil -p " + current_projdir() + 
    	    	  " -s " + current_system() + 
    	          " -Force -Quiet -C \"delete -e " + current_editor() +
		  " " + file_chap.name + "\"", NULL );
	}
    }

    if ( count > 0 && ok == 0 )
      SEREReply( reply_handle, True );
    else
      SEREReply( reply_handle, False );
    return 0;
}


// this is used to delete calls and global accesses - it is used to locate
// instances of the call on a particular diagram
//
int
DeleteLinkObject(int parent_oms_id, int child_oms_id, int reply_handle)
{
    node_ref ref;
    file file_chap;
    int file_id, ok = -1;
    node_ref r = sce_defn_file_id( parent_oms_id );

    if ( r )
    {
	file_id = r.file_id;

	file_chap = find_by_query( "file[ id = " + file_id + "]" );
	if ( file_chap != NULL )
	{
	    LoadDiagramIfNeeded( file_chap.name );
	    for_each_in_select( "node_ref[ node_id = " + child_oms_id +
				" && file_id = " + file_id +
				" && in_link_refs[ from_node_ref[ node_id = " +
				 parent_oms_id + " ]]]", ref )
	    {
		ok = delete_node( to_int( ref.appid ), -1);
	    }
	    gde_builtin("FileSave");
	}
    }

    if ( ok == 0 )
      SEREReply( reply_handle, True );
    else
      SEREReply( reply_handle, False );
    return 0;
}


int
DeleteNode(int psymid, int depth, int reply_handle )
{
    if ( delete_node( psymid, depth ) == 0 )
      SEREReply( reply_handle, True );
    else
      SEREReply( reply_handle, False );
    return 0;
}

// this guy just takes the psymid and deletes them all from the current
// diagram - it will also delete children up to a depth of depth. Set depth
// to -1 if you want no children deleting, 1 for just a single layer, set
// it big to really wipe them all out
//
int
delete_node(int psymid, int depth)
{
    int i, gap, whitespace, deleted_node_width, deleted_x, loop;
    int tmp_x, tmp_y, txn;
    list outarcs, endnodes, all_arc_syms, kids_to_go, arcs = NULL;
    gde_symbol symbol;
    gde_node thisnode, parent = NULL, left_node, right_node, dummy;

    thisnode = gde_node_find_by_psymid( psymid );

    // only perform the deletion if we have found the right node.
    // if depth == 0, we are on the last iteration of a descending delete,
    // if, at this depth, a node has children, then leave it alone
    if ( thisnode != NULL && 
	     !(depth == 0 && list_count(gde_node_outarcs_no_comments( thisnode )) != 0 ))
    {
	gde_save_state();
	txn = gde_start_edit_transaction();
	arcs = gde_node_inarcs_no_comments( thisnode );
	if ( list_count( arcs ) > 0 )
	    parent = gde_arc_get_real_fromnode( list_get( arcs, 0 ), True,
						NULL, NULL );
	else
	    parent = NULL;

	arcs = gde_node_outarcs_no_comments( thisnode );
	kids_to_go = list_create( "gde_node", 0 );
	for( loop = 0; loop < list_count( arcs ); loop++ )
	    dummy = gde_arc_get_real_tonode( list_get( arcs, loop ), False, kids_to_go, NULL );

	    // store the dimensions of the node that is going
	deleted_node_width = gde_node_width( thisnode );
	deleted_x = gde_node_x( thisnode );

	gde_node_delete( thisnode );
	if ( parent != NULL && gde_nt_name(gde_node_nt( parent )) == "Anchor" )
	{
	    gde_node_delete( parent );
	    parent = NULL;
	}

	    // so, there may be some kids that need sorting out
	if ( parent != NULL )
	{
	    // get all the endnodes
	    outarcs = gde_node_outarcs_no_comments( parent );
	    endnodes = list_create( "gde_node", 0 );
	    all_arc_syms = list_create( "gde_symbol", 0 );
	    for( loop = 0; loop < list_count( outarcs ); loop++ )
	    {
		dummy = gde_arc_get_real_tonode( list_get( outarcs, loop ),
					 False, endnodes, all_arc_syms );
	    }

	    // if this node is to one side or the other, then get all
	    // symbols below the parent on that side, and shift them
	    // all up by the width of this node, plus half the gap.
	    // must leave nodes on grid if poss.
	    // get the average distance between nodes
	    left_node = NULL;
	    right_node = NULL;
	    for( loop = 0; loop < list_count( endnodes ); loop++ )
	    {
		tmp_x = gde_node_x( list_get( endnodes, loop ));
		if ( tmp_x <= deleted_x )
		    if ( left_node == NULL ||
			 tmp_x > gde_node_x( left_node ))
			left_node = list_get( endnodes, loop );

		// we do not use >= here to prevent the same node ever being
		// selected as both the left and the right node
		if ( tmp_x > deleted_x )
		    if ( right_node == NULL ||
			 tmp_x < gde_node_x( right_node ))
			right_node = list_get( endnodes, loop );
	    }

		// if this is at one extreme or the other, then leave
		// it alone - I do not care about that case
	    if ( left_node != NULL && right_node != NULL )
	    {
		// see how much space we have created
		gap = gde_node_x( right_node ) - gde_node_x( left_node );
		gap = gap - (( gde_node_width( right_node ) +
			       gde_node_width( left_node ) ) / 2);
		whitespace = (gap - deleted_node_width) / 2;

		    // if there is room to move nodes over
		if ( gap >= deleted_node_width + whitespace )
		{
		    if ( deleted_x > gde_node_x( parent ))
			move_over_nodes( all_arc_syms, deleted_x,
				       -(deleted_node_width + whitespace),1);
		    else
			move_over_nodes( all_arc_syms, deleted_x,
					deleted_node_width + whitespace, -1 );
		}
	    }
	    list_clear( all_arc_syms );
	    list_clear( endnodes );
	}

	gde_end_edit_transaction( GDE_CMD_DELETE, txn, False );

	    // now we can remove the critter's children too
	if ( depth > 0 )
	    for( loop = 0; loop < list_count( kids_to_go ); loop++ )
		delete_node( gde_node_psymid( list_get( kids_to_go, loop )),
			    depth - 1);
	list_clear( kids_to_go );
    }
    else
	return 1;

    return 0;
}


struct coord_struct
{
    int tot_x;			// the total x positions, and y positions
    int tot_y;
    int points;			// the number of samples in the set
};

// find all the symbols beneath a node - take care that recursive links are
// not followed
//
list
all_symbols_beneath( gde_node root, list symbols_found,
		     coord_struct re_shift_pos )
{
    gde_symbol this_sym = to_gde_symbol( root );
    int sym_count = list_count( symbols_found );
    list children = gde_node_outarcs_no_comments( root );
    gde_arc thisarc;
    gde_node thisnode;
    int loop;

    if ( this_sym != NULL && 
	 list_find( symbols_found, 0, this_sym ) == sym_count )
    {
	list_append( symbols_found, this_sym );
	re_shift_pos.points++;
	re_shift_pos.tot_x += gde_node_x( root );
	re_shift_pos.tot_y += gde_node_y( root );
	for( loop = 0; loop < list_count( children ); loop++ )
	{
	    thisarc = list_get( children, loop );
	    thisnode = gde_arc_tonode( thisarc );
	    symbols_found = all_symbols_beneath( thisnode, symbols_found,
						 re_shift_pos );
	}
    }
    return symbols_found;
}


// this whole list is all arcs, find any arc with an endnode/context which
// has locations to one side or the other of the midpoint, and move them
// all over by the shuffle amount.
void
move_over_nodes( list arcs, int midpoint, int shuffle_amount, int direction )
{
    // for the first pass, lets hope that we can just move nodes and
    // contexts
    int loop, loop2, xpos;
    list cxs;
    gde_arc thisarc = NULL;
    gde_node thisnode = NULL;
    gde_symbol thissym;
    list done = list_create( "gde_arc", 0 );
    list syms_below;
    coord_struct re_shift_pos;
    int mid_x, mid_y;

    for( loop = 0; loop < list_count( arcs ); loop++ )
    {
	thissym = list_get( arcs, loop );
	if ( gde_symbol_is_arc( thissym ) )
	{
	    thisarc = to_gde_arc( thissym );

// we can have duplicates in the list, so this should sort out that problem
//
	    if ( list_find( done, 0, thisarc ) != list_count( done ))
		continue;

	    list_append( done, thisarc );

	    if ( (direction < 0 && gde_arc_tox( thisarc ) <= midpoint) ||
		 (direction > 0 && gde_arc_tox( thisarc ) >= midpoint) )
	    {
		thisnode = gde_arc_tonode( thisarc );
		xpos = gde_node_x( thisnode ) + shuffle_amount;

		if ( gde_nt_name( gde_node_nt( thisnode )) ==
		     VERTEX_SYMBOL_TYPE )
		{
		    gde_select_node( thisnode );
		    gde_builtin("SymbolMove " + xpos + " " +
			 	 gde_node_y( thisnode ));
		}
		else
		{
		// if this is a bottom level chap and it has kids, then we want to add it
		// to the list below to traverse more fully - either add it
		// as the first or the last

		// now move over any lower levels of nodes
		    syms_below = list_create( "gde_symbol", 0 );
		    re_shift_pos.points = 0;
		    re_shift_pos.tot_x = 0;
		    re_shift_pos.tot_y = 0;
		    gde_select_symbols_in_list(
			    all_symbols_beneath( thisnode, syms_below,
						 re_shift_pos ));
		    list_clear( syms_below );
		    if ( re_shift_pos.points )	// no div zero please
		    {
			mid_x = re_shift_pos.tot_x / re_shift_pos.points;
			mid_y = re_shift_pos.tot_y / re_shift_pos.points;
			gde_builtin("SymbolMove " + (mid_x + shuffle_amount) +
				" " + mid_y );
		    }
		}
	    }
	}
    }
    list_clear(done);
    return;
}


// could be on TYPEDEFS diagram, a brand new diagram, or on that of a
// corresponding structure
//
void
add_typedef( string filename, string name, string fname, string dname,
	     string type_item, string array_item, int reply_handle )
{
    LoadDiagramIfNeeded( filename );

    gde_node top_left = NULL, top_right = NULL;
    gde_node lwr_left = NULL, lwr_right = NULL;
    gde_node newbie;
    int txn;
    list links = gde_all_mapped_oms_links();
    int x = 0, y = 0;

    get_boundary_nodes(top_left, top_right, lwr_left, lwr_right);

    if ( top_left != NULL )
    {
	    // if we are on the TYPEDEFS diagram, then the new node
	    // goes on the bottom line - if it overflows, start a
	    // new line with it
	if ( list_count(links) == 0 )
	{
	    x = gde_node_x( lwr_right ) + XDIFF;
	    y = gde_node_y( lwr_right );
	    if ( x > gde_node_x( top_right ))
	    {
		x = gde_node_x( lwr_left );
		y = gde_node_y( lwr_left ) + YDIFF;
	    }
	}
	    // if we are on a normal DS diagram, then put it at the top
	    // to one side
	else
	{
	    x = gde_node_x( top_right ) + XDIFF;
	    y = gde_node_y( top_right );
	}
    }

	// lets create a new node
    gde_save_state();
    txn = gde_start_edit_transaction();
    newbie = gde_node_create( "Typedef", x, y );
    gde_node_set_label(newbie, name);
    gde_node_scale( newbie, 5, 172, 64 );
    gde_node_set_prop(newbie, "SEDirectory", dname);
    gde_node_set_prop(newbie, "SEFile", fname);

    gde_end_edit_transaction( GDE_CMD_INSERT, txn, False );

    node node;
    int handle, ok = 0;
    node = get_proper_oms_node(newbie);
    if (node != NULL)
    {
	handle = send_start_edit_annot(node.id);

	editor_send_msg(NGOAE_CLASS,
			    OAE_CMD_NOTE_SEL + "TypedefDefinition",
			    handle);

	editor_send_msg(NGOAE_CLASS,
			    OAE_CMD_ITEM_SEL + "DataType",
			    handle);

	editor_send_msg(NGOAE_CLASS,
			    OAE_CMD_ITEM_SETVAL + stpem_string_val(type_item),
			    handle);

	if ( array_item != "" )
	{
	    editor_send_msg(NGOAE_CLASS,
			    OAE_CMD_ITEM_SEL + "ArraySize",
			    handle);
	    editor_send_msg(NGOAE_CLASS,
			    OAE_CMD_ITEM_SETVAL + stpem_string_val(array_item),
			    handle);
	}
	send_oae_save_update_unload(handle);
	ok = 1;
    }
    gde_builtin( "FileSave" );

    if ( ok )
      SEREReply( reply_handle, True );
    else
      SEREReply( reply_handle, False );
}


// find the nodes that geographically bound the current diagram, return
// the four corner nodes, or set them all to NULL if the diagram is empty
void
get_boundary_nodes( gde_node top_left, gde_node top_right,
		    gde_node lwr_left, gde_node lwr_right )
{
    list symbols;
    gde_node n;
    int i, ydiff;
    symbols = gde_all_symbols();
    gde_symbol s;

    top_left = NULL;
    top_right = NULL;
    lwr_left = NULL;
    lwr_right = NULL;

    for(i = 0; i < list_count(symbols); i = i + 1)
    {
	s = list_get(symbols, i);
	if (gde_symbol_is_node(s))
	{
	    n = to_gde_node(s);
	    if ( top_left == NULL )
		top_left = n;
	    else
	    {
		ydiff = gde_node_y( top_left ) - gde_node_y( n );
		if ( ydiff == 0 )
		{
		    if ( gde_node_x( n ) < gde_node_x( top_left ))
			top_left = n;
		}
		else if ( ydiff > 0 )
		    top_left = n;
	    }

	    if ( top_right == NULL )
		top_right = n;
	    else
	    {
		ydiff = gde_node_y( top_right ) - gde_node_y( n );
		if ( ydiff == 0 )
		{
		    if ( gde_node_x( n ) > gde_node_x( top_right ))
			top_right = n;
		}
		else if ( ydiff > 0 )
		    top_right = n;
	    }

	    if ( lwr_left == NULL )
		lwr_left = n;
	    else
	    {
		ydiff = gde_node_y( n ) - gde_node_y( lwr_left );
		if ( ydiff == 0 )
		{
		    if ( gde_node_x( n ) < gde_node_x( lwr_left ))
			lwr_left = n;
		}
		else if ( ydiff > 0 )
		    lwr_left = n;
	    }

	    if ( lwr_right == NULL )
		lwr_right = n;
	    else
	    {
		ydiff = gde_node_y( n ) - gde_node_y( lwr_right );
		if ( ydiff == 0 )
		{
		    if ( gde_node_x( n ) > gde_node_x( lwr_right ))
			lwr_right = n;
		}
		else if ( ydiff > 0 )
		    lwr_right = n;
	    }
	}
    }
}




int AddField( int parent_oms_id, string name, string type, string array,
	      int reply_handle )
{
    file file_chap;
    int file_id, psymid = 0, ok = -1;
    node_ref r = dse_defn_file_id( parent_oms_id );

    if ( r )
    {
	file_id = r.file_id;
	psymid = to_int( r.appid );

	file_chap = find_by_query( "file[ id = " + file_id + "]" );
	if ( file_chap != NULL )
	{
	    LoadDiagramIfNeeded( file_chap.name );
	    ok = add_field( psymid, name, type, array);
	}
    }

    if ( ok == 0 )
      SEREReply( reply_handle, True );
    else
      SEREReply( reply_handle, False );
    return 0;
}


int
add_field( int parent_psymid, string name,
	   string type_item, string array_item )
{
    gde_node pnode;
    int txn, ok = -1;

    pnode = gde_node_find_by_psymid( parent_psymid );
    if ( pnode != NULL )
    {
	list arcs, kids, kid_arcs;
	gde_node dummy, newbie;
	int loop, biggest_x = -99999, suitable_y;
	gde_arc newarc;

	// put the new node to the left of the existing children;
	arcs = gde_node_outarcs_no_comments( pnode );
	kids = list_create( "gde_node", 0 );
	kid_arcs = list_create( "gde_symbol", 0 );
	for( loop = 0; loop < list_count( arcs ); loop++ )
	    dummy = gde_arc_get_real_tonode( list_get( arcs, loop ), False, kids, kid_arcs );

	for( loop = 0; loop < list_count( kids ); loop++ )
	{
	    dummy = list_get( kids, loop );
	    if ( gde_node_x( dummy ) > biggest_x )
	    {
		biggest_x = gde_node_x( dummy );
		suitable_y = gde_node_y( dummy );
	    }
	    biggest_x += DSE_XDIFF / 2;
	}
	if (loop == 0 )
	{
	    biggest_x = gde_node_x( pnode );
	    suitable_y = gde_node_y( pnode ) + YDIFF;
	}
	else
	{
	    gde_save_state();
	    txn = gde_start_edit_transaction();
	    move_over_nodes( kid_arcs, -99999, -DSE_XDIFF / 2, 1 );
	    gde_end_edit_transaction( GDE_CMD_INSERT, txn, False );
	}

	list_clear( kids );
	list_clear( kid_arcs );
	gde_save_state();
	txn = gde_start_edit_transaction();
	newbie = gde_node_create( "Sequence", biggest_x, suitable_y );
	gde_node_set_label(newbie, name);
	gde_node_scale( newbie, 5, 172, 64 );
	gde_arc_create( "Component", pnode, newbie);

	gde_end_edit_transaction( GDE_CMD_INSERT, txn, False );
    
	node node;
	int handle;
	node = get_proper_oms_node(newbie);
	if (node != NULL)
	{
	    handle = send_start_edit_annot(node.id);
    
	    editor_send_msg(NGOAE_CLASS,
				OAE_CMD_NOTE_SEL + "DataDefinition",
				handle);
    
	    editor_send_msg(NGOAE_CLASS,
				OAE_CMD_ITEM_SEL + "DataType",
				handle);
    
	    editor_send_msg(NGOAE_CLASS,
				OAE_CMD_ITEM_SETVAL + stpem_string_val(type_item),
				handle);
    
	    if ( array_item != "" )
	    {
		editor_send_msg(NGOAE_CLASS,
				OAE_CMD_ITEM_SEL + "ArraySize",
				handle);
		editor_send_msg(NGOAE_CLASS,
				OAE_CMD_ITEM_SETVAL + stpem_string_val(array_item),
				handle);
	    }
	    send_oae_save_update_unload(handle);
	    ok = 0;
	}
	gde_builtin( "FileSave" );
    }

    return ok;
}


int
AddGlobal( int parent_oms_id, string name, string access, string file,
	   string dir, int reply_handle )
{
    file file_chap;
    int file_id, psymid = 0, ok = -1;
    node_ref r = sce_defacto_file_id( parent_oms_id );

    if ( r )
    {
	file_id = r.file_id;
	psymid = to_int( r.appid );

	file_chap = find_by_query( "file[ id = " + file_id + "]" );
	if ( file_chap != NULL )
	{
	    LoadDiagramIfNeeded( file_chap.name );
	    ok = add_global_access( psymid, name, access, file, dir);
	    gde_builtin("FileSave");
	}
    }

    if ( ok == 0 )
      SEREReply( reply_handle, True );
    else
      SEREReply( reply_handle, False );
    return 0;
}





// adds a new call to the left of all other nodes, does not take into account
// the present spacing, nor whether the diagram is sawtooth or not
//
int
add_global_access( int parent_psymid, string name, string access, string file,
		   string dir )
{
    gde_node pnode;
    int txn;

    pnode = gde_node_find_by_psymid( parent_psymid );
    if ( pnode != NULL )
    {
	list arcs, kids, kid_arcs;
	gde_node dummy, newbie, newbie2;
	int loop, biggest_x = -99999, suitable_y;
	gde_arc new_arc;

	// put the new node to the left of the existing children;
	arcs = gde_node_outarcs_no_comments( pnode );
	kids = list_create( "gde_node", 0 );
	kid_arcs = list_create( "gde_symbol", 0 );
	for( loop = 0; loop < list_count( arcs ); loop++ )
	    dummy = gde_arc_get_real_tonode( list_get( arcs, loop ), False, kids, kid_arcs );

	for( loop = 0; loop < list_count( kids ); loop++ )
	{
	    dummy = list_get( kids, loop );
	    if ( gde_node_x( dummy ) > biggest_x )
	    {
		biggest_x = gde_node_x( dummy );
		suitable_y = gde_node_y( dummy );
	    }
	}
	if (loop == 0 )
	{
	    biggest_x = gde_node_x( pnode );
	    suitable_y = gde_node_y( pnode ) + YDIFF;
	}
	else
	{
	    biggest_x += XDIFF / 2;
	    gde_save_state();
	    txn = gde_start_edit_transaction();
	    move_over_nodes( kid_arcs, -99999, -XDIFF / 2, 1 );
	    gde_end_edit_transaction( GDE_CMD_INSERT, txn, False );
	}

	list_clear( kids );
	list_clear( kid_arcs );
	gde_save_state();
	txn = gde_start_edit_transaction();
	newbie = gde_node_create( "GlobalData", biggest_x, suitable_y );
	gde_node_set_label(newbie, name);
	gde_node_set_prop(newbie, "SEDirectory", dir);
	gde_node_set_prop(newbie, "SEFile", file);
	newbie2 = gde_node_create( VERTEX_SYMBOL_TYPE, biggest_x,
				   gde_node_y( pnode ) +
				   (suitable_y - gde_node_y( pnode ))/ 3);
	gde_arc_create( "ModuleCall", pnode, newbie2);
	new_arc = gde_arc_create( "ModuleCall", newbie2, newbie);
	gde_end_edit_transaction( GDE_CMD_INSERT, txn, False );

	list links, int_links;
	int handle, ok = 0;
	int_links = gde_arc_links( new_arc );
	if ( list_count( int_links ) > 0 )
	    links = gde_link_oms_links( list_get( int_links, 0 ));
	else
	    links = NULL;
	if ( list_count( links ) >= 1 && access != "" )
	{
	    link link = list_get( links, 0 );
	    handle = send_start_edit_annot(link.id);
    
	    editor_send_msg(NGOAE_CLASS,
				OAE_CMD_NOTE_SEL + "GlobalUsage",
				handle);
    
	    editor_send_msg(NGOAE_CLASS,
				OAE_CMD_ITEM_SEL + "AccessMode",
				handle);
    
	    editor_send_msg(NGOAE_CLASS,
				OAE_CMD_ITEM_SETVAL + stpem_string_val(access),
				handle);

	    send_oae_save_update_unload(handle);
	    return 0;
	}
    }
    return -1;
}

int
AddCall( int parent_oms_id, string name, string file, string dir, string type,
	 int reply_handle )
{
    file file_chap;
    int file_id, psymid = 0, ok = -1;
    node_ref r = sce_defacto_file_id( parent_oms_id );

    if ( r )
    {
	file_id = r.file_id;
	psymid = to_int( r.appid );

	file_chap = find_by_query( "file[ id = " + file_id + "]" );
	if ( file_chap != NULL )
	{
	    LoadDiagramIfNeeded( file_chap.name );
	    ok = add_fn_call( psymid, name, file, dir, type);
	    gde_builtin("FileSave");
	}
    }

    if ( ok == 0 )
      SEREReply( reply_handle, True );
    else
      SEREReply( reply_handle, False );
    return 0;
}


// its definition is where it is linked to a formal caller - if there is no
// such link, then we'll have the place where it has no in links at all,
node_ref
sce_defn_file_id( int oms_id )
{
    node_ref ref;

    ref = find_by_query( "node_ref[ node_id = " + oms_id + " && in_link_refs[from_node_ref[node[FormalCaller]]]]" );
    if ( ref == NULL )
	ref = find_by_query( "node_ref[ node_id = " + oms_id + " && !in_link_refs]" );

    return ref;
}


// its defacto definition is where is has all its out links
node_ref
sce_defacto_file_id( int oms_id )
{
    node_ref ref;

    ref = find_by_query( "node_ref[ node_id = " + oms_id + " && out_link_refs]" );
    if ( ref == NULL )
    {
	int x = selection_count( "node_ref[ node_id = " + oms_id + "]" );
	if ( x == 1 )		// return the node
	    ref = find_by_query( "node_ref[ node_id = " + oms_id + "]" );
	else if ( x == 2 )	// find the one not coming from formal
	    ref = find_by_query( "node_ref[ node_id = " + oms_id + " && in_link_refs[ from_node_ref[ node[ Module ]]]]" );
	else
	    ref = sce_defn_file_id( oms_id );
    }
    return ref;
}


node_ref
dse_defn_file_id( int oms_id )
{
    node_ref ref;

    ref = find_by_query( "node_ref[ node_id = " + oms_id + " && !in_link_refs]" );
    if ( ref == NULL )
	ref = find_by_query( "node_ref[ node_id = " + oms_id + " && out_link_refs]" );

    if ( ref == NULL )
	ref = find_by_query( "node_ref[ node_id = " + oms_id + "]" );

    return ref;
}




// the module tyep tells us if this is a library or a module, 
// it goes to the left of the other calls for the time being
//
int
add_fn_call( int parent_psymid, string name, string file,
	     string dir, string module_type )
{
    gde_node pnode;
    int txn;

    pnode = gde_node_find_by_psymid( parent_psymid );
    if ( pnode != NULL )
    {
	list arcs, kids, kid_arcs;
	gde_node dummy, newbie, newbie2;
	int loop, biggest_x = -99999, suitable_y, smallest_x = 99999;
	gde_arc newarc;

	// put the new node in the right place;
	arcs = gde_node_outarcs_no_comments( pnode );
	kids = list_create( "gde_node", 0 );
	kid_arcs = list_create( "gde_symbol", 0 );
	for( loop = 0; loop < list_count( arcs ); loop++ )
	    dummy = gde_arc_get_real_tonode( list_get( arcs, loop ), False, kids, kid_arcs );

	for( loop = 0; loop < list_count( kids ); loop++ )
	{
	    dummy = list_get( kids, loop );
	    if ( gde_nt_name(gde_node_nt(dummy)) == "Module" ||
		 gde_nt_name(gde_node_nt(dummy)) == "LibraryModule" )
	    {
		if ( gde_node_x( dummy ) > biggest_x )
		{
		    biggest_x = gde_node_x( dummy );
		    suitable_y = gde_node_y( dummy );
		}
	    }
	    else
		if ( gde_node_x( dummy ) < smallest_x )
		    smallest_x = gde_node_x( dummy );
	}
	if (biggest_x == -99999 )
	{
	    if ( smallest_x == 99999 )
		biggest_x = gde_node_x( pnode );
	    else
		biggest_x = smallest_x - XDIFF;
	    suitable_y = gde_node_y( pnode ) + YDIFF;
	}
	else
	{
	    gde_save_state();
	    txn = gde_start_edit_transaction();
	    move_over_nodes( kid_arcs, biggest_x, -XDIFF, -1 );
	    gde_end_edit_transaction( GDE_CMD_INSERT, txn, False );
	}

	list_clear( kids );
	list_clear( kid_arcs );
	gde_save_state();
	txn = gde_start_edit_transaction();
	newbie = gde_node_create( module_type, biggest_x, suitable_y );
	gde_node_set_label(newbie, name);
	newbie2 = gde_node_create( VERTEX_SYMBOL_TYPE, biggest_x,
				   gde_node_y( pnode ) +
				   (suitable_y - gde_node_y( pnode ))/ 3);
	gde_arc_create( "ModuleCall", pnode, newbie2);
	gde_arc_create( "ModuleCall", newbie2, newbie);
	gde_node_set_prop(newbie, "SEDirectory", dir);
	gde_node_set_prop(newbie, "SEFile", file);
	gde_end_edit_transaction( GDE_CMD_INSERT, txn, False );

	return 0;
    }
    return -1;
}



// delete a context. so far this just removes the formal parameter, and
// shuffles up the positions of the others. It may need extending so that
// all the corresponding actual parameters are also removed on all the
// other diagrams.
//
void
delete_parameter( int oms_id, int reply_handle )
{
    cntx_ref ctx;
    file file_chap;
    int ok = -1;

    for_each_in_select( "cntx_ref[ cntx_id = " + oms_id + "]", ctx )
    {
	file_chap = find_by_query( "file[ id = " + ctx.file_id + "]" );
	LoadDiagramIfNeeded( file_chap.name );
	ok = delete_context( to_int( ctx.appid ));
	gde_builtin( "FileSave" );
    }

    if ( ok == 0 )
      SEREReply( reply_handle, True );
    else
      SEREReply( reply_handle, False );
}


// the right diagram is already loaded
//
int
delete_context( int psymid )
{
    gde_cntx this_ctx;
    list all_arcs, parms;
    int loop, position;
    gde_arc thisarc;
    gde_node fromnode;
    int ok = -1, txn;

    this_ctx = gde_cntx_find_by_psymid( psymid );

    if ( this_ctx )
    {
	all_arcs = gde_arcs_find_by_type( gde_all_symbols(), "ModuleCall" );
	for( loop = 0; loop < list_count( all_arcs ); loop++ )
	{
	    thisarc = list_get( all_arcs, loop);
	    fromnode = gde_arc_fromnode( thisarc );
	    if ( gde_nt_name(gde_node_nt(fromnode)) != "Anchor" )
		continue;

	    parms = find_cntxs_on_parent(thisarc, NULL);
	    if ( target_cntx_in_list( this_ctx, parms ) >= 0 )
	    {
		sort_cntx_list_by_x_y( parms );
		position = target_cntx_in_list( this_ctx, parms );
		gde_save_state();
		txn = gde_start_edit_transaction();
		shuffle_cntx_positions_down( parms, position );
		gde_cntx_delete( this_ctx );
		gde_end_edit_transaction( GDE_CMD_DELETE, txn, False );
		ok = 0;
	    }
	    list_clear( parms );
	}
	list_clear( all_arcs );
    }
    return ok;
}


int
target_cntx_in_list( gde_cntx this_ctx, list parms )
{
    int loop;
    for( loop = 0; loop < list_count( parms ); loop++ )
	if ( list_get( parms, loop ) == this_ctx )
	    return loop;
    return -1;
}


// sorts a list of contexts by their coordinates, I had to turn the contexts
// into nodes first so that I can get their screen locations
void
sort_cntx_list_by_x_y(list the_list)
{
    gde_cntx	temp, temp2;
    gde_node	n1, n2;
    int 	i, j;
 
    if (the_list == NULL || (list_count(the_list) == 0))
        return;
 
    for (i = 0; i < list_count(the_list) - 1 ; ++i)
    {
	for (j = i + 1; j < list_count(the_list); ++j)
	{
	    temp = list_get(the_list, i);
	    temp2 = list_get(the_list, j);
	    n1 = to_gde_node( to_gde_symbol( temp ));
	    n2 = to_gde_node( to_gde_symbol( temp2 ));

	    if (gde_node_y(n2) < gde_node_y(n1))
	    {
		list_insert(the_list, i, temp2); 
		list_delete(the_list, j + 1);
	    }

	    else if (gde_node_y(n2) == gde_node_y(n1))
	    {
		if (gde_node_x(n2) <  gde_node_x(n1))
		{
		    list_insert(the_list, i, temp2); 
		    list_delete(the_list, j + 1);
		}
	    }
	}
    }
}    


// they take the position of the one before it - all the ones after 
// the position marker in the list are affected
//
void
shuffle_cntx_positions_down( list parms, int position )
{
    int loop, x, y;
    gde_node tmp;

    for( loop = list_count( parms ) - 1; loop > position; loop-- )
    {
	// this contexts receives the location of the previous one;
	tmp = to_gde_node( to_gde_symbol( list_get( parms, loop - 1)));
	gde_select_cntx( list_get( parms, loop ));
	gde_builtin("SymbolMove " + gde_node_x( tmp ) + " " + 
		     gde_node_y( tmp ));
    }
}



int
AddParameter( int parent_oms_id, int position, string name, string type,
	      string storage_class, string symbol_type, int reply_handle )
{
    file file_chap;
    int file_id, psymid = 0, ok = -1;
    node_ref r = sce_defn_file_id( parent_oms_id );

    if ( r )
    {

	file_id = r.file_id;
	psymid = to_int( r.appid );

	file_chap = find_by_query( "file[ id = " + file_id + "]" );
	if ( file_chap != NULL )
	{
	    LoadDiagramIfNeeded( file_chap.name );
	    ok = insert_context( psymid, position, name, type, storage_class,
			    symbol_type);
	}
    }

    if ( ok == 0 )
      SEREReply( reply_handle, True );
    else
      SEREReply( reply_handle, False );
    return 0;
}



int
insert_context( int parent_psymid, int position, string name, string type,
		string storage_class, string symbol_type )
{
    // find the parent
    // get its inarcs - can only be 1
    // get the contexts from the arc
    // order them
    // get the box around them - using the routine above
    //
    int txn;
    gde_node pnode;
    list inarcs;

    pnode = gde_node_find_by_psymid( parent_psymid );
    if ( pnode != NULL )
    {
	inarcs = gde_node_inarcs_no_comments( pnode );
	// if we have no flow coming in, then lets create one 
	if ( list_count( inarcs ) == 0 )
	{
	    gde_save_state();
	    txn = gde_start_edit_transaction();
	    gde_node newanchor = gde_node_create( "Anchor", gde_node_x( pnode ),
				   gde_node_y( pnode ) - YDIFF );
	    gde_arc_create( "ModuleCall", newanchor, pnode);
	    gde_end_edit_transaction( GDE_CMD_INSERT, txn, False );
	    inarcs = gde_node_inarcs_no_comments( pnode );
	}

	if ( list_count( inarcs ) == 1 )
	{
	    gde_cntx newchap; 
	    gde_arc thisarc = list_get( inarcs, 0 );
	    list parms = find_cntxs_on_parent(thisarc, NULL);
	    sort_cntx_list_by_x_y( parms );
	    gde_save_state();
	    txn = gde_start_edit_transaction();
	    newchap = gde_cntx_create( symbol_type, thisarc, 0, 0 );
	    gde_cntx_set_label(newchap, name);

	    if ( parms == NULL )
		parms = list_create( "gde_cntx", 0 );

	    if ( position > list_count( parms ))
		list_append( parms, newchap );
	    else
		list_insert( parms, position - 1, newchap );

		list_insert( parms, position - 1, newchap );
	    position_contexts( parms, gde_node_x( gde_arc_fromnode( thisarc )),
				gde_node_y( gde_arc_fromnode( thisarc )),
				gde_node_x( pnode ), gde_node_y( pnode ));

	    gde_end_edit_transaction( GDE_CMD_INSERT, txn, False );

	    cntx node;
	    int handle;
	    node = get_proper_oms_cntx(newchap);
	    if (node != NULL)
	    {
		handle = send_start_edit_annot(node.id);

		editor_send_msg(NGOAE_CLASS,
			    OAE_CMD_NOTE_SEL + "ParameterDefinition",
			    handle);

		editor_send_msg(NGOAE_CLASS,
			    OAE_CMD_ITEM_SEL + "DataType",
			    handle);

		editor_send_msg(NGOAE_CLASS,
			    OAE_CMD_ITEM_SETVAL + stpem_string_val(type),
			    handle);

		if ( storage_class != "" )
		{
		    editor_send_msg(NGOAE_CLASS,
				    OAE_CMD_ITEM_SEL + "ParamStorageClass",
				    handle);
		    editor_send_msg(NGOAE_CLASS,
				    OAE_CMD_ITEM_SETVAL + stpem_string_val(storage_class),
				    handle);
		}
		send_oae_save_update_unload(handle);
	    }
	    gde_builtin( "FileSave" );
	    return 0;
	}
	else
	    print_message( "Cannot add parameter to function with more than 1 in link" );
    }
    return -1;
}



void
position_contexts( list cntxts, int x1, int y1, int x2, int y2 )
{
    int x_diff, y_diff, name_count, x_step, y_step, x_pos, y_pos;
    int side_step = FLOW_GAP, loop;

    x_diff = x2 - x1;
    y_diff = y2 - y1;

    name_count = list_count( cntxts );
    x_step = x_diff / ( name_count + 1 );
    y_step = y_diff / ( name_count + 1 );

    x_pos = x1;
    if ( y1 > y2 )
	y_pos = y2;
    else
	y_pos = y1;

    for ( loop = 0; loop < list_count( cntxts ); loop++ )
    {
	x_pos = x_pos + x_step;
	y_pos = y_pos + y_step;
	side_step = side_step * -1;
	
	gde_select_cntx( list_get( cntxts, loop ));
	gde_builtin("SymbolMove " + (x_pos + side_step) + " " + y_pos);
    }
}


int
ChangeSymbol( int oms_id, string symbol_type, int reply_handle )
{
    node_ref ref;
    file file_chap;
    string filename;
    int ok = 0, count = 0;
    gde_node thisnode;

    for_each_in_select( "node_ref[ node_id = " + oms_id + "]", ref )
    {
	count++;
	file_chap = find_by_query( "file[ id = " + ref.file_id + "]" );
	LoadDiagramIfNeeded( file_chap.name );
	thisnode = gde_node_find_by_psymid( to_int( ref.appid ));
	if ( thisnode != NULL )
	{
	    int txn;
	    gde_save_state();
	    txn = gde_start_edit_transaction();
	    gde_select_node( thisnode );
	    gde_builtin( "SymbolReplace " + symbol_type );
	    gde_end_edit_transaction( GDE_CMD_REPLACE, txn, False );
	    gde_builtin("FileSave");
	}
	else
	    ok++;
    }
    if ( count > 0 && ok == 0 )
      SEREReply( reply_handle, True );
    else
      SEREReply( reply_handle, False );
    return 0;
}



void
LoadDiagramIfNeeded( string fname )
{
    if ( fname != NULL && fname != "" )
    {
	if ( gde_diagram_name() != fname )
	{
//		This is failsafe stuff that  is needed because the messages
//		to ngoae are marking the diagram as modified
	    int times = 1;
	    if ( gde_is_modified() == True )
	    {
		print_message( "Synchronizer : Forcing file save - tries = " + times );
		gde_builtin("FileSave");
		times++;
	    }
	    gde_builtin("FileLoad " + fname);
	}
    }
}

