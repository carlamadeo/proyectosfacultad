//
// 	sccsid[] = %W% %Y% %D%
// 	StP/IM
// 	Confidential property of Interactive Development Environments, Inc.
// 	Copyright (c) 1992-1993
// 	All rights reserved
//

//
// FILE debug.inc
//
// this file provides functions to output messages conditinally
// on the setting of the global variable "debug_messages".
// functions also (debug) messages for  types, lists of types.
//

boolean debug_messages = False;

void show_debug_messages() { debug_messages = True; }

void hide_debug_messages() { debug_messages = False; }

void debug_message( string m ) { if ( debug_messages ) message( m ); }

string debug_message_string_field( string s )
{
    if (s == NULL || s == "")
	return "null";
    else
	return s;
}

    void
debug_message_link_list( list l, string name )
{
    if ( debug_messages )
	message_link_list( l, name );
}

    void
message_link_list( list l, string name )
{
    int c,n;
    link o;

    if (l == NULL) 
    {
	message("link list " + name + " is null.");
	return;
    }

    message( name + " has " + list_count(l) + " links." );

    for(c=list_count(l),n=0;n<c;n=n+1)
	    message_link( list_get(l,n) );
}

void debug_message_link(link o) { if ( debug_messages ) message_link(o); }

    void
message_link(link o)
{
    message("");
    message( "link id: " + o.id );
    message( "     tp: " + o.type );
    message( "     nm: " + o.name );
    message( "     sc: " + o.scope_node_id );
    message( "     sg: " + o.sig );
    message( "     fi: " + o.from_node_id );
    message( "     ti: " + o.to_node_id );
    message( "     af: " + o.annot_file_id );
}

    void
debug_message_node_list( list l, string name )
{
    if ( debug_messages )
	message_node_list( l, name );
}

    void
message_node_list( list l, string name )
{
    int c,n;

    if (l == NULL) 
    {
	message("node list " + name + " is null.");
	return;
    }

    message( name + " has " + list_count(l) + " nodes." );

    for(c=list_count(l),n=0;n<c;n=n+1)
	message_node(list_get(l,n));
}

void debug_message_node(node o) { if ( debug_messages ) message_node(o); }

    void
message_node(node o)
{
    message("");
    message( "node id: " + o.id );
    message( "     tp: " + o.type );
    message( "     nm: " + o.name );
    message( "     sc: " + o.scope_node_id );
    message( "     sg: " + o.sig );
    message( "     af: " + o.annot_file_id );
}

    void
debug_message_item_list( list l, string name )
{
    if ( debug_messages )
	message_item_list( l, name );
}

    void
message_item_list( list l, string name )
{
    int c,n;

    if (l == NULL) 
    {
	message("item list " + name + " is null.");
	return;
    }

    message( name + " has " + list_count(l) + " items." );

    for(c=list_count(l),n=0;n<c;n=n+1)
	message_item(list_get(l,n));
}

void debug_message_item(item o) { if ( debug_messages ) message_item(o); }

    void
message_item(item o)
{
    if (o == NULL) 
    {
	message("item is null.");
	return;
    }
    message("");
    message( "item id: " + o.id );
    message( "     nt: " + o.note_id );
    message( "     tp: " + o.type );
    message( "     vl: " + o.value );
    message( "    oid: " + o.obj_id );
    message( "    svl: " + o.svalue );
}

    void
debug_message_int_list( list l, string name )
{
    if ( debug_messages )
	message_int_list( l, name );
}

    void
message_int_list( list l, string name )
{
    int c,n;

    if (l == NULL) 
    {
	message("int list " + name + " is null.");
	return;
    }

    message("");
    message( name + " has " + list_count(l) + " ints." );

    for(c=list_count(l),n=0;n<c;n=n+1)
	if (list_get(l,n)==NULL)
	    message("    (null)");
	else
	    message( "    " + list_get(l,n));
}

    void
debug_message_string_list( list l, string name )
{
    if ( debug_messages )
	message_string_list( l, name );
}

    void
message_string_list( list l, string name )
{
    int c,n;

    if (l == NULL) 
    {
	message("string list " + name + " is null.");
	return;
    }

    message("");
    message( name + " has " + list_count(l) + " strings." );

    for(c=list_count(l),n=0;n<c;n=n+1)
	if (list_get(l,n)==NULL)
	    message("    (null)");
	else
	    message( "    " + list_get(l,n));
}

    void
debug_message_cntx_list( list l, string name )
{
    if ( debug_messages )
	message_cntx_list( l, name );
}

    void
message_cntx_list( list l, string name )
{
    int c,n;

    if (l == NULL) 
    {
	message("cntx list " + name + " is null.");
	return;
    }

    message( name + " has " + list_count(l) + " cntxs." );

    for(c=list_count(l),n=0;n<c;n=n+1)
	message_cntx(list_get(l,n));
}

void debug_message_cntx(cntx o) { if ( debug_messages ) message_cntx(o); }

    void
message_cntx(cntx o)
{
    message("");
    message( "cntx id: " + o.id );
    message( "     tp: " + o.type );
    message( "     nm: " + o.name );
    message( "     sc: " + o.scope_node_id );
    message( "     sg: " + o.sig );
    message( "     pa: " + o.link_id );
    message( "     af: " + o.annot_file_id );
}

    void
debug_message_file_list( list l, string name )
{
    if ( debug_messages )
	message_file_list( l, name );
}

    void
message_file_list( list l, string name )
{
    int c,n;
    file o;

    if (l == NULL) 
    {
	message("file list " + name + " is null.");
	return;
    }

    message( name + " has " + list_count(l) + " files." );

    for(c=list_count(l),n=0;n<c;n=n+1)
	    message_file( list_get(l,n) );
}

void debug_message_file(file o) { if ( debug_messages ) message_file(o); }

    void
message_file(file o)
{
    message("");
    message( "file id: " + o.id );
    message( "     tp: " + o.type );
    message( "     nm: " + o.name );
    message( "     rv: " + o.rev );
    message( "     fn: " + o.fname );
    message( "     hv: " + o.lview_id );
    message( "     hm: " + o.lmdfy_id );
    message( "     hs: " + o.lsync_id );
    message( "     af: " + o.annot_file_id );
}

