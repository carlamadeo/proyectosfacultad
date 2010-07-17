#include "ct/rules/qrl/qrl_std.inc"
#include "qrl/ted/tabgen_defs.inc"
#include "qrl/ted/tabgen_output.inc"

//
//  Primitive functions for table generation scripts:
//  See enums in tabgen_defs.inc for allowed values for weights, positions,
//  justifications, etc.
//
//  The sequence of use of these functions is as follows:
//  For each table type:
//	define_table_format(<type>, <table name>);
//	define_table_caption(<type>, <caption>);
//      For each table cell (column):
//  	    define_table_cell(<type>, width);
//  	For each header (span) column:
//    	     define_table_header_span(<type>, <number of cells>, <font weight>,
//    	    	    	    	      <label justification>, <label text>);
//    	define_table_body_cells(<type>, <font weight>, <label justification>);
//
//  	If there are any special borders,
//  	    define_table_border(<type>, <section>, <cell index>, 
//    	    	    	    	<border position>, <border thickness>, 
//    	    	    	    	<border pattern>, <border number>, 
//    	    	    	    	<border visibility>);
//    	    the cell index is 1-n; 0 means all cells in row
//
//  	generate_table_header(<type>);
//  	generate_table_body_start(<type>);
//  	
//  	Extract the data you wish to put in the table.
//  	For each datum,
//  	generate_table_body_cell(<type>, <datum value>);
//
//  	If you wish to alter or undefine a border (revert it to the default),
//  	you may.
//
//  	generate_table_body_end(<type>);
//
//  	This command will create a table file and an object in the repository.
//  	generate_table_finish(<type>);
//  
//  	Now you may preview the table in a TED editor:
//  	    preview_table(<type>);	
//  	or output it
//  	    output_table(<type>);
//
//	You can re-preview or output a table without regenerating it.
//

const string TED_STORE = "ted_files";
const string TED_SUFFIX = "ted";

//
//  Allocates and initializes a table format structure.
//  
void define_table_format (string table_type, string tabname)
{
    tabformat current_format = find_format(table_type);
    tabsection current_section;
    tabcell cell;
    string fill_file;
    int ix;

    if (current_format != NULL)
    {
    	ide_error("Current format " + table_type + " already defined");
    	return;
    }

    current_format.type = table_type;
    current_format.fill_string = "";
    current_format.outputted = False;

    /* In filenames, replace spaces with underscores */
    fill_file = string_search_and_replace(table_type, " ", "_");

    // add a table name if any was supplied
    if (tabname != "")
    	fill_file = fill_file + "_" 
	     + string_search_and_replace(tabname, " ", "_");

    current_format.fill_file = fill_file;

    // create a full pathname for the fill file
    current_format.full_fill_file = current_projdir() + "/" + current_system() 
				    + "/" + TED_STORE + "/" + fill_file + "."
				    + TED_SUFFIX;
    current_format.num_body_rows = 0;
    current_format.caption = "";
    current_section.num_cells = 0;
    current_section.cells = list_create("tabcell", 0);
    current_section.current_ix = NULL;
    set_row(current_section, NULL);
    set_section(current_format, HEADER, current_section);

    current_section.num_cells = 0;
    current_section.cells = list_create("tabcell", 0);
    current_section.current_ix = 0;
    set_row(current_section, NULL);
    set_section(current_format, BODY, current_section);
    list_append(table_formats, current_format);    
}

//
//  Sets the caption of a table.
//  
void define_table_caption (string table_type, string caption)
{
    tabformat format = find_format(table_type);
    format.caption = caption;
    set_format(table_type, format);
}

//
//  Adds a cell to a table
//
void define_table_cell (string table_type, int width)
{
    tabformat format = find_format(table_type);
    tabsection sect = find_section(format, HEADER);
    tabcell cell;

    if (format == NULL || sect == NULL)
    	return;

    cell.index = sect.num_cells;
    cell.width = width;
    cell.weight = NULL;
    cell.alignment = NULL;
    cell.border = NULL;
    cell.label = NULL;
    cell.hspan = 1;

    list_append(sect.cells, cell);

    sect.num_cells = sect.num_cells + 1;
    set_section(format, HEADER, sect);

    // the header's cell width will be used for the body's as well, so
    // there's no point in writing it out again.
    cell.width = NULL;
    sect = find_section(format, BODY);

    if (sect == NULL)
    	return;

    list_append(sect.cells, cell);

    sect.num_cells = sect.num_cells + 1;
    set_section(format, BODY, sect);
    set_format(table_type, format);
}

//
//  Defines a table header span.  The default span is one column.  
//
void define_table_header_span (string table_type, int span,
			       weight_type weight, alignment_enum alignment,
    	    	    	       string label)
{
    tabformat format = find_format(table_type);
    tabsection sect = find_section(format, HEADER);
    tabcell cell;
    
    if (format == NULL || sect == NULL)
    	return;

    if (span == NULL)
    	span = 1;

    if (sect.current_ix == NULL)
    	sect.current_ix = 0;

    cell = list_get(sect.cells, sect.current_ix);
    cell.weight = weight;
    cell.alignment = alignment;
    cell.border = NULL;
    cell.label = label;
    cell.hspan = span;

    list_set(sect.cells, sect.current_ix, cell);

    sect.current_ix = sect.current_ix + span;    
    set_section(format, HEADER, sect);
    set_format(table_type, format);
}

//
//  Defines elements of a table body which don't vary by cell.
//  Returns the number of cells in a row of the table.
//
int define_table_body_cells (string table_type, weight_type weight, 
    	    	    	      alignment_enum alignment)
{
    tabformat format = find_format(table_type);
    tabsection sect = find_section(format, BODY);
    tabcell cell;
    int ix;

    if (format == NULL)
    	return;

    for (ix = 0; ix < sect.num_cells; ix = ix + 1)
    {
    	cell = list_get(sect.cells, ix);
        cell.weight = weight;
    	cell.alignment = alignment;
    	list_set(sect.cells, ix, cell);
    }
    set_section(format, BODY, sect);
    set_format(table_type, format);
    return sect.num_cells;
}

//
//  Defines cell border characteristics.  A border_type of BOTTOM/TOP
//  will set the entire current row; one of RIGHT/LEFT will set the entire 
//  column indexed by the 1-based border_index.
//
void define_table_border (string table_type, section_type sect_type,
    	    	    	  int border_index, pos_type position, 
    	    	    	  thick_type thickness, pat_type pattern,
    	    	    	  num_type number, vis_type visible)
{
    tabformat format = find_format(table_type);
    tabsection sect = find_section(format, sect_type);
    tabrow row;
    tabcell cell;
    tabborder bord;

    if (format == NULL || sect == NULL)
    	return;

    bord.position = position;
    bord.thickness = thickness;
    bord.pattern = pattern;
    bord.number = number;
    bord.visible = visible;

    if (border_index < 1)
    	border_index = 1;

    row = find_row(sect);
    cell = list_get(sect.cells, border_index - 1);

    if (position == BOTTOM || position == TOP)
    	row.border = bord;
    else if (position == RIGHT || position == LEFT)
    	cell.border = bord;

    list_set(sect.cells, border_index - 1, cell);
    set_row(sect, row);
    set_section(format, sect_type, sect);
    set_format(table_type, format);
}

//
//  Erases a border setting (defaults are used again)
//  
void undefine_table_border (string table_type, section_type sect_type,
    	    	    	  int border_index, pos_type position)
{
    tabformat format = find_format(table_type);
    tabsection sect = find_section(format, sect_type);
    tabrow row;
    tabcell cell;

    if (format == NULL || sect == NULL)
    	return;

    if (border_index < 1)
    	border_index = 1;

    row = find_row(sect);
    cell = list_get(sect.cells, border_index - 1);

    if (position == RIGHT || position == LEFT)
    	cell.border = NULL;
    else 	    	    	// if (position == BOTTOM || position == TOP)
    	row.border = NULL;

    list_set(sect.cells, border_index - 1, cell);
    set_row(sect, row);
    set_section(format, sect_type, sect);
    set_format(table_type, format);
}

//
//  Generates the header of a table.
//
void generate_table_header (string table_type)
{
    tabformat format = find_format(table_type);
    tabsection sect = find_section(format, HEADER);
    int ix, count; 

    if (format == NULL || sect == NULL)
    	return;

    // reset the current ix
    sect.current_ix = 0;
    set_section(format, HEADER, sect);
    set_format(table_type, format);

    add_to_fill(format, "FillTable TedTable{");
    add_to_fill(format, 
    	       "{Hsect {Index 1}{Vsect");// {NumCols " + sect.num_cells + "}");

    for (ix = 0, count = list_count(sect.cells); ix < count; ix = ix + 1)
    	generate_cell(format, HEADER, NULL);
    // row should always be closed on last cell

    set_format(table_type, format);
}

//
//  Generates the start of the body of a table.
//
void generate_table_body_start (string table_type)
{
    return;
}

//
//  Generates the end of a table
//
void generate_table_body_end (string table_type)
{
    tabformat format = find_format(table_type);
    tabsection sect = find_section(format, BODY);

    // Finish off a row if necessary
    if (sect.current_ix != 0)
    	generate_row_end(format, BODY);

    // Close Vsect and Hsect and TedTable
    add_to_fill(format, "}}}\n");
    set_format(table_type, format);
}

//
//  Generates a cell in the body of the table
//
void generate_body_cell (string table_type, string label)
{
    tabformat format = find_format(table_type);

    generate_cell(format, BODY, label);
    set_format(table_type, format);
}


//
//  Generates a cell in the body of the table
//
void clear_table_for_regeneration (string table_type)
{
    tabformat format = find_format(table_type);

    if (format.fill_string == "" || format.outputted)
    {
        format.fill_string = "";
    	format.outputted = False;
    	set_format(table_type, format);
    }
    else
    {
    	ide_error("Attempted to clear body of current format " 
    	    	  + table_type + " before outputting the old body.");
    }
}

// ************************* Private Functions *************************

//
//  Generates an single cell of a table.
//
void generate_cell (tabformat format, section_type sect_type, string label)
{
    tabsection sect = find_section(format, sect_type);
    tabrow row = find_row(sect);
    tabcell cell;
    int ix;

    if (format == NULL || sect == NULL)
    	return;

    cell = list_get(sect.cells, sect.current_ix);

    if (cell == NULL)
    	return;

    if (sect.current_ix == 0)
    	generate_row_start(format, sect.row);

    // Cell indexes are 1-based
    add_to_fill(format, "{Cell {Index " + (cell.index + 1) + "}");

    if (cell.width != NULL && cell.width != 0)
    	add_to_fill(format, "{Width " + cell.width + "}");

    if (label != NULL) 
    	add_to_fill(format, "{Label \"" + string_escape(label, "\"") + "\"}");
    else if (cell.label != NULL)
    	add_to_fill(format, "{Label \"" + string_escape(cell.label, "\"") + "\"}");

    if (cell.weight == BOLD)
    	add_to_fill(format, "{Bold True}");

    if (cell.alignment != NULL && cell.alignment != Center)
    	add_to_fill(format, "{HorzAlignment " + cell.alignment + "}");

    generate_border(format, cell.border);

    if (cell.hspan != 1)
    	add_to_fill(format, "{Hspan " + cell.hspan + "}");

    // finish cell
    add_to_fill(format, "}");

    // set current_ix for next time
    if (sect.current_ix == list_count(sect.cells) - 1)
    {
    	generate_row_end(format, sect_type);
    	sect.current_ix = 0;
    }
    else
        sect.current_ix = sect.current_ix + 1;

    set_section(format, sect_type, sect);
}

//
//  Starts a Row
//
void generate_row_start (tabformat format, tabrow row)
{
    add_to_fill(format, "{Row");
    if (row == NULL)
    	return;

    if (row.border != NULL)
    {
    	add_to_fill(format, "{Cell");
        add_to_fill(format, "{RepeatToEnd True}");
    	generate_border(format, row.border);
    	add_to_fill(format, "}");
    }
}

//
//  Ends a Row  
//
void generate_row_end (tabformat format, section_type sect_type)
{
    add_to_fill(format, "}");
    if (sect_type == BODY)
    	format.num_body_rows = format.num_body_rows + 1;
}

//
//  Generates border Characteristics
//
void generate_border (tabformat format, tabborder bord)
{
    string position;

    if (bord == NULL)
    	return;

    add_to_fill(format, "{ ");

    if (bord.position == RIGHT)
    	position = "Right";
    else if (bord.position == LEFT)
    	position = "Left";
    else if (bord.position == TOP)
    	position = "Top";
    else if (bord.position == BOTTOM)
    	position = "Bottom";

    add_to_fill(format, position + "Border");

    if (bord.visible == Invisible)
    	add_to_fill(format, "{Visible False}");
    if (bord.thickness != NULL && bord.thickness != Thin)
    	add_to_fill(format, "{Thickness " + bord.thickness + "}");
    if (bord.pattern != NULL && bord.pattern != Solid)
    	add_to_fill(format, "{Pattern " + bord.pattern + "}");
    if (bord.number == Double)
    	add_to_fill(format, "{Double True}");
    add_to_fill(format, "}");
}
