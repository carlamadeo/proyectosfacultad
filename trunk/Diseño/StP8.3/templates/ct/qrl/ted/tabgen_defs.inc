// Definitions for table generation service

// ENUMS
//
enum weight_type { STANDARD, BOLD };
enum section_type { BODY, HEADER };

// Border Characteristics
//
enum thick_type { Thick, Medium, Thin };
enum pat_type { Dotted, Faint, Dashed, Solid };
enum num_type { Single, Double };
enum vis_type { Visible, Invisible };
enum pos_type { BOTTOM, TOP, RIGHT, LEFT };


// ************************* PRIVATE *****************************

//
// Structures
//

struct tabborder
{
    thick_type thickness;
    pat_type 	pattern;
    num_type 	number;
    vis_type 	visible;
    pos_type 	position;
};

struct tabcell
{
    int     	    index;  	    // column number
    int     	    width;	    // width in pixels
    weight_type     weight;	    // weight of label font
    alignment_enum  alignment;	    // alignment of label
    tabborder 	    border;	    // right/left border 
    string  	    label;   	    // text of label
    int     	    hspan;	    // width (in columns) of cell span
};

struct tabrow
{
    tabborder border;	    	    // bottom/top border
};

struct tabsection
{
    int     current_ix; 	    // current column being printed
    int     num_cells;   	    // number of (unspanned) cells in body
    tabrow  row;     	    	    // row 
    list    cells;   	     	    // list of cells
};

struct tabformat
{
    string  	type;	    	    // format identifier
    tabsection 	header;	    	    // header section
    tabsection	body;   	    // body section
    string  	fill_string;	    // fill string for type
    string  	fill_file;	    // fill file for type
    string  	full_fill_file;     // fill file with full pathname
    boolean 	outputted;	    // fill file generated to repository?
    string  	caption; 	    // table caption
    int	    	num_body_rows;      // number of rows in table body
};

//
// Globals
//

// list of all currently defined table formats
list table_formats = list_create("tabformat", 0);

// ************************* END PRIVATE *****************************

//
//  Functions to retrieve and set structure elements
//

//
//  Finds a table format of the given type
//
tabformat find_format(string table_type)
{
    return find_internal_format(table_formats, table_type);
}

//
//  Finds a table format of the given type in the given list
//
tabformat find_internal_format(list formats, string table_type)
{
    int ix, count;
    tabformat format;

    for (ix = 0, count = list_count(formats); ix < count; ix = ix + 1)
    {
    	format = list_get(formats, ix);
    	if (format.type == table_type)
    	    return format;
    }
    return NULL;        	
}

//
//  Sets a table format of the given type in the given list to the given 
//  format
//
void set_format(string table_type, tabformat new)
{
    set_internal_format(table_formats, table_type, new);
}

//
//  Sets a table format of the given type in the given list to the given 
//  format
//
void set_internal_format(list formats, string table_type, tabformat new)
{
    int ix, count;
    tabformat format;

    for (ix = 0, count = list_count(formats); ix < count; ix = ix + 1)
    {
    	format = list_get(formats, ix);
    	if (format.type == table_type)
    	{
    	    list_set(formats, ix, new);
    	    return;
    	}
    }
}

//
//  Finds a table section of the given type in the given format.
//
tabsection find_section (tabformat format, section_type type)
{
    if (format == NULL || type == NULL)
    	return NULL;

    if (type == HEADER)
    	return format.header;
    else if (type == BODY)
    	return format.body;
}

//
//  Finds a table section of the given type in the given format.
//
void set_section (tabformat format, section_type type, tabsection new)
{
    if (format == NULL || type == NULL)
    	return;    	   

    if (type == HEADER)
    	format.header = new;
    else if (type == BODY)
    	format.body = new;
}

//
//  Finds the row of the given section, or returns a new
//
tabrow find_row (tabsection sect)
{
    if (sect == NULL)
    	return NULL;
    else
    	return sect.row;
}

//
//  Sets the row of the given section to the given row.
//
void set_row (tabsection sect, tabrow row)
{
    if (sect == NULL)
        return;

    sect.row = row;
}
