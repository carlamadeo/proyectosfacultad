// This function is to prevent differences between
// UNIX and NT directory handling.
// Too bad QRP has no #ifdef :(
// The UNIX version should be:

// void CreateDirectory(string sDirectory)
// {
//     system("mkdir " + sDirectory);
// }


void CreateDirectory(string sDirectory)
{
    string cmdline;
    cmdline = sDirectory;
    cmdline = string_search_and_replace(cmdline,"/","\\");
    cmdline = string_search_and_replace(cmdline,"'","");
    cmdline = "cmd /c md " + cmdline;
    message(cmdline);
    system(cmdline);
}

