
extern char *getenv(char *);
extern int strcat(char *, char *);
extern int strcpy(char *, char *);


void
stp_send_command(char *tool, char *cmd, int nowait)
{
    char           *p, *s;
    char            fullcmd[1024];

    strcpy(fullcmd, "stpem -first");

    if (tool)
    {
        strcat(fullcmd, " -ed ");
        strcat(fullcmd, tool);
    }
    p = getenv("projdir");
    if (p)
    {
        strcat(fullcmd, " -p ");
        strcat(fullcmd, p);
    }
    s = getenv("system");
    if (s)
    {
        strcat(fullcmd, " -s ");
        strcat(fullcmd, s);
    }
    if (cmd)
    {
        strcat(fullcmd, " -C '");
        strcat(fullcmd, cmd);
        strcat(fullcmd, "' ");
    }
    if (nowait)
    {
        strcat(fullcmd, " &");
    }
    centerline_sh(fullcmd);
}




void
stp_start(char *editor)
{
    stp_send_command(editor, "EditorStart", 0);
}



void
stp_browse(char *token, int ed)
{
    char            buf[1024];
    char            editor[10];

    if (!token)
    {
        printf("You must select an object to browse to...\n");
        return;
    }

    if (ed == 1)
        strcpy(editor, "sce");
    else if (ed == 2)
        strcpy(editor, "dse");

    sprintf(buf, "EditorQrlEval \"SEFromProgrammingEnvironment(\\\"%s\\\", \\\"%s\\\");\"", editor, token);
    stp_send_command(editor, buf, 0);

    return;
}



