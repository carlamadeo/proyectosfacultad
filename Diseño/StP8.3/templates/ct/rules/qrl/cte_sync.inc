//
//      sccsid[] = @(#)cte_sync.inc	1.0  97/05/27
//      StP/OMT
//      Confidential property of Interactive Development Environments, Inc.
//      Copyright (c) 1997
//      All rights reserved
//


// routines to synchronize the contents of a class table with changes
// picked up during RE

void
cte_update_member( string parent_class, int reply_handle, string name,
		   string sig, int probable_place, string value_list )
{
    // need to make sure that the correct table is loaded - if it isn't
    // then return with an error
    string class_table_name;

    class_table_name = gte_get_cell_label(1,1);
    if ( class_table_name != parent_class )
      {
	//SPR 2542 - spelling error
	print_error( "Received update message from RE for a different table" );
	editor_send_msg( "RE_sync", "FAILED \"-1\"", reply_handle );
	return;
      }

    // find the entry with the given name and optionally the signature
    int row;
    if ( sig != NULL )
      row = find_row_for_entry( name, sig, probable_place, False );
    else
      row = find_row_for_entry( name, NULL, probable_place, True );

    // the name list is newline separated, within that, there is a tuple,
    // this time colon separated, consisting of vsect name, cell name, and
    // cell value
    list values = string_to_list( value_list, "\001" );
    string two_bits, value;
    int colon, i, col_no, changed = 0, at_sign;
    for( i = 0; i < list_count( values ); i++ )
      {
	two_bits = list_get( values, i );
	at_sign = string_find( two_bits, 0, "@" );
	if ( at_sign != string_length( two_bits ))
	  {
	    col_no = to_int( string_extract( two_bits, 0, at_sign ));
	    if ( at_sign + 1 == string_length( two_bits ))
	      value = "";
	    else
	      value = string_extract( two_bits, at_sign + 1,
				      string_length( two_bits ) - at_sign - 1);
	    
	    if ( gte_get_cell_label( row, col_no ) != value )
	      {
		CoordCellLabel( row, col_no, value );
		changed++;
	      }
	  }
	else
	{
	  print_error( "cte_update_operation: error - syntax error in parameter string\n" );
	  print_error( value_list );
	  print_error( "processing " + i + " : " + two_bits );
	}
      }

    if ( changed > 0 )
      gte_refresh_table();

    // send an ok message back down the message handle
    editor_send_msg( "RE_sync", "OK \"" + changed + "\"", reply_handle );

    return;
}


int   __static_for_find_row_entry_hs_attr = 0;
int   __static_for_find_row_entry_hs_op = 0;

int
find_row_for_entry( string name, string sig, int probable_place,
		    boolean is_attr )
{
    int hs, firstrow, lastrow, i;

    if ( __static_for_find_row_entry_hs_attr == 0 )
      {
	if ( is_product_uml() )
	  {
	    __static_for_find_row_entry_hs_attr = gte_lookup_symbol( "AttributeHsect" );
	    __static_for_find_row_entry_hs_op = gte_lookup_symbol( "OperationHsect" );
	  }
	else
	  {
	    __static_for_find_row_entry_hs_attr = gte_lookup_symbol( "OMTAttributeHsect" );
	    __static_for_find_row_entry_hs_op = gte_lookup_symbol( "OMTOperationHsect" );
	  }
      }

    if ( is_attr == True )
      hs = __static_for_find_row_entry_hs_attr;
    else
      hs = __static_for_find_row_entry_hs_op;

    firstrow = gte_get_first_row(hs);
    lastrow = gte_get_last_row(hs);

    if ( probable_place == 1 )
      {
	cte_sync_init_held_rows_in_table( firstrow, lastrow );
      }

    for( i = firstrow; i <= lastrow; i++ )
      {
	if ( gte_get_cell_label( i, 1 ) == name )
	  {
	    if ( is_attr == True )
	      {
		cte_sync_match_held_row_in_table( i - firstrow );
		return i;
	      }
	    else
	      {
		if ( gte_get_cell_label( i, 2 ) == sig )
		  {
		    cte_sync_match_held_row_in_table( i - firstrow );
		    return i;
		  }
	      }
	  }
      }

    if ( firstrow == lastrow &&
	 gte_get_cell_label( firstrow, 1 ) == "" )
      {
	CoordCellLabel( firstrow, 1, name );
	if ( is_attr == False )
	  CoordCellLabel( firstrow, 2, sig );
	cte_sync_match_held_row_in_table( 0 );
	return firstrow;
      }

    // so its not here, lets create a new row for it in its probable place
    int the_place = firstrow + probable_place - 1;
    if ( the_place >= lastrow )
      {
	gte_insert_row_after( lastrow, 1 );
	cte_sync_add_row_to_table( lastrow - firstrow );
	the_place = lastrow + 1;
      }
    else
      {
	gte_insert_row( the_place + 1, 1 );
	the_place++;
	cte_sync_add_row_to_table( the_place - firstrow );
      }
    CoordCellLabel( the_place, 1, name );
    if ( is_attr == False )
      CoordCellLabel( the_place, 2, sig );
    return the_place;
}


list _static_extra_rows_in_table = list_create( "int", 0 );
int  _static_extra_rows_start = 0;

void
cte_sync_init_held_rows_in_table( int from, int to )
{
  // cte_sync_report_extra_rows(); called directly by class capture update
    list_clear( _static_extra_rows_in_table );
    int loop;
    for( loop = from; loop <= to; loop++ )
      list_append( _static_extra_rows_in_table, 0 );
    _static_extra_rows_start = from;
}

void
cte_sync_match_held_row_in_table( int row )
{
    list_set( _static_extra_rows_in_table, row, 1 );
}

void
cte_sync_add_row_to_table( int row )
{
    list_insert( _static_extra_rows_in_table, row, 1 );
}

void
cte_sync_report_extra_rows()
{
    int len = list_count( _static_extra_rows_in_table );
    if ( len > 0 )
      {
	int loop;
	for( loop = 0; loop < len; loop++ )
	  {
	    if ( list_get( _static_extra_rows_in_table, loop ) == 0 )
	      gte_print_error( "Row " + (loop + _static_extra_rows_start) + " has no corresponding source code in the SMDB.", gte_get_cell_id( loop + _static_extra_rows_start, 1 ));
	  }
      }
}


