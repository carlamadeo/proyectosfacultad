
/*  This utility replaces Process cell labels which contain process indexes
    with the name of the process which has that index.
    Customers who have 4.2D tables they are migrating to 5.0 can run this
    utility after the sysmigration to avoid Process reference errors in 
    the tables 
*/

/* Works for ALT, PAT and PAM */

// important things to keep this working:
// Activate processes stay in 1st hsect, 2nd vsect.  Since symbolic names are different
// In addition, they ar always in the second row of that hsect, vsect.
// in the different editors, they will not work here.

void
AdjustProcessCells(string filename)
{
    int i, row, col;
    int count = gte_get_last_col(2);

    row = gte_get_row(1, 2);

    for (i = 1; i <= count; i++)
    {
    	col = gte_get_col(2, i);
    	AdjustLabel(filename, row, col);
    }
    gte_refresh_table();
}

void
AdjustLabel(string filename, int row, int col)
{
    string query, index, cur_token;
    node process;

    index = NULL;
    InitToken(gte_get_cell_label(row, col)); 
    while ((cur_token = NextToken()) != NULL)
    	if ((index = FindIndex(cur_token)) != NULL)
    	    break;

    if (index == NULL)
    	return;

    /* find the analogous DFE process and use its name as the label */
    query = "node[Process && " + 
    	    "node_refs[file[DfeDiagram && name == '${filename}']] && " + 
    	    "notes[ProcessIndex && items[RelativeIndex && value == '${index}']]]";

    process = find_by_query(query);
    if (process != NULL)
    	CoordCellLabel(row, col, process.name);
}

/* Globals for parsing */
string cur_label = "";
int    token_length = 0;

void
InitToken(string label)
{
    cur_label = label;
    token_length = string_length(label);
}

/* Returns the next token from the label.  Tokens are delimited by 
   whitespace. */
string 
NextToken()
{
    int blank, newline, tab, end;
    string result;

    /* no label */
    if (cur_label == "" || cur_label == NULL)
    	return NULL;

    blank = string_find(cur_label, 0, " ");
    newline = string_find(cur_label, 0, "\n");
    tab = string_find(cur_label, 0, "\t");    

    if (blank > newline)
    	if (newline > tab)
    	    end = tab;
    	else
    	    end = newline;
    else
    	if (blank > tab)
    	    end = tab;
    	else
    	    end = blank;

    /* hit end of label */
    if (end >= token_length - 1)
    {
    	result = cur_label;
    	cur_label = NULL;
    	token_length = 0;
    }    	
    else
    {
        result = string_extract(cur_label, 0, end);
    	cur_label = string_extract(cur_label, end + 1, token_length - (end + 1));
        token_length = string_length(cur_label);
    }
    return result;
}

/*  Checks if the token is an index (all periods and numbers) and returns
    the relative index (number after the last period) if it is.
 */
string
FindIndex (string token)
{
    string char;
    int ix, length, last_period = -1;

    if (token == NULL)
    {
    	gte_print_error("NULL token passed to FindIndex");
    	return NULL;
    }

    length = string_length(token);
    for (ix = 0; ix < length; ix = ix + 1)
    {

    	char = string_extract(token, ix, 1);
    	if (char == ".")
    	{
    	    if (last_period == ix - 1)	    	/* two periods in a row */
    	    	return NULL;
    	    else
    	    	last_period = ix;
    	}
    	else if (!string_converts(char, "int"))
    	    return NULL;
    }

    if (length == 0 || last_period >= length - 1)
    	return NULL;
    return string_extract(token, last_period + 1, 
    	    	    	  length - (last_period + 1));    
}    	    
