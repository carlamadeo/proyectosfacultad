string stputil_outfile;

string
GetStputil()
{
    return stputil_outfile;
}

void
SetStputil()
{
    stputil_outfile = GetDfeFileStoreDir() + "/stputil_" + getpid();
}

//  Prints Sptutil output to the message log
void
PrintStputil()
{
    int ix;
    list stputil_lines = string_to_list(read_file(stputil_outfile), "\n");
    
    for (ix = 0; ix < list_count(stputil_lines); ix = ix + 1)
    	gde_print_message(list_get(stputil_lines, ix));
}

void
RemoveStputil()
{
    delete_file(stputil_outfile);
}

