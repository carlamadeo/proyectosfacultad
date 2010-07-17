//
// Fill Utilities
//

//
//  Appends new string to fill string
//
void
add_to_fill(tabformat format, string new)
{
    format.fill_string = format.fill_string + new;
}

//
//  Writes the table type's fill string to its fill file
//
void
write_fill(tabformat format)
{
    write_file(format.full_fill_file, format.fill_string);
}

void
generate_table_finish(string table_type)
{
    string command;
    tabformat format = find_format(table_type);

    write_fill(format);   

    // get the table in the database
    command = "stputil -s " + current_system() + " -p " + current_projdir()  
    	      + " -Quiet -C 'e ted' -C 'force on' -C 'undef " 
    	      + format.fill_file + "'";
    system(command);
    format.outputted = True;
    set_format(table_type, format);
}

//
//  Previews table in TED editor
//
void
preview_table(string table_type)
{
    string command;
    tabformat format = find_format(table_type);

    // Load the table into the editor.
    command = "stpem -ed ted -first -s " + current_system() 
    	    	+ " -p " + current_projdir() 
    	        + " -C 'FileLoad " + format.fill_file + "'" 
    	    	+ " -C 'BufferHideShow -h 0 -v 0' ";
    system(command);    
}

//
//  Prints the table to the script output
//
void
output_table(string table_type)
{
    string command, query;
    tabformat format = find_format(table_type);

    table_header_row_range_set("1");
    if (format.num_body_rows > 0)
	table_row_range_set("2-*");
    else
	table_row_range_set("0");
    table_caption_alignment_set(Center);
    table_print_column_indices_set(False);
    table_print_row_indices_set(False);
    query = "file[TedTable && name == '" + format.fill_file + "']";
    file_print(find_by_query(query), format.caption);
}
