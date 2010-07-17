


struct table_info {
 
list ted_file_info;
int incr_appid;
int incr_num_rows;

};


table_info
init_table_list()
{

table_info table_list;

table_list.ted_file_info = list_create("string", 0);
table_list.incr_appid = 2;
table_list.incr_num_rows = 0;

return table_list;

}

void
reset_table_list()
{
 
list_clear(table_list.ted_file_info);
table_list.incr_appid = 2;
table_list.incr_num_rows = 0;
 
}

table_info table_list = init_table_list();



string
curr_appid()
{

string curr_app = to_string(table_list.incr_appid);
table_list.incr_appid = table_list.incr_appid + 1;

return curr_app;

}

void
Begin_Table(int num_cols)
{


list_append(table_list.ted_file_info, "FillTable TedTable");
list_append(table_list.ted_file_info, "{");
list_append(table_list.ted_file_info, "{ Hsect { Appid " + curr_appid() + " }");
list_append(table_list.ted_file_info, "{ Hidden False }");
list_append(table_list.ted_file_info, "{ Vsect { Appid " + curr_appid() + " }");
list_append(table_list.ted_file_info, "{ NumCols " + num_cols + " }");
list_append(table_list.ted_file_info, "{ Hidden False }");
list_append(table_list.ted_file_info, "{ NumVisCols " + num_cols + " }");

}


void
End_Table()
{

list_append(table_list.ted_file_info, "} } }");

}



void
Begin_Row()
{

list_append(table_list.ted_file_info, "{ Row     { Appid " + curr_appid() + " }");

table_list.incr_num_rows = table_list.incr_num_rows + 1;

}

void
End_Row()
{

list_append(table_list.ted_file_info, "}");

}

void
Make_Cell(int width, int vspan, string shade, string lbordervis, 
          string lborderthick, string horzalign, string label)
{

list_append(table_list.ted_file_info, "{ Cell { Appid " + curr_appid() + " }");

if (width != NULL) 
list_append(table_list.ted_file_info, "{ Width " + width + " }");

if (label != NULL) 
list_append(table_list.ted_file_info, "{ Label \"" + string_search_and_replace(label, "\n", "\\n") + "\" }");

if (vspan != NULL) 
list_append(table_list.ted_file_info, "{ Vspan " + vspan + " }");

if (lbordervis != NULL) 
list_append(table_list.ted_file_info, "{ LeftBorder { Visible " + lbordervis + " } }");

if (horzalign != NULL) 
list_append(table_list.ted_file_info, "{ HorzAlignment " + horzalign + " }");

if (lborderthick != NULL) 
list_append(table_list.ted_file_info, "{ LeftBorder { Thickness " + lborderthick + " } }");

if (shade != NULL) 
list_append(table_list.ted_file_info, "{ Shading " + shade + " }");

list_append(table_list.ted_file_info, "}");

}


void
print_table(string file_name, string caption)
{
 
int x;
string target_dir = path_compose(current_projdir(), current_system(),
				 "ted_files");
string query, command;

list_insert(table_list.ted_file_info, 4, 
            "{ NumRows " + table_list.incr_num_rows + " }");
list_insert(table_list.ted_file_info, 7, 
            "{ NumVisRows " + table_list.incr_num_rows + " }");
 
write_file(path_compose(target_dir, file_name + ".ted"),
list_to_string(table_list.ted_file_info, "\n"));

command = "stputil -s " + current_system() + " -p " + current_projdir()
          + " -Quiet -C 'e ted' -C 'force on' -C 'undef "
          + file_name + "'";

system(command);

table_header_row_range_set("1");

if (table_list.incr_num_rows > 1) table_row_range_set("2-*");
else table_row_range_set("0");

table_caption_alignment_set(Center);
table_print_column_indices_set(False);
table_print_row_indices_set(False);

query = "file[TedTable && name = '" + file_name + "']";

file_print(find_by_query(query), caption);

reset_table_list();

}



void
print_table_list() 
{

int x;

for (x=0; x < list_count(table_list.ted_file_info); x=x+1)
{

message(list_get(table_list.ted_file_info, x));

}
}


