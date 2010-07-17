
/* sniff stuff (gde&gte) */

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////


string
SNiFF__editor_get_class_name()
{
    if (PropSheet__editor_type() == "gte")
	return OMTClassTableGetClassCellLabel();
    else if (PropSheet__editor_type() == "gde")
	return gde_node_label(to_gde_node(list_get(gde_selected_symbols(), 0)));
    return NULL;
}

////////////////////////////////////////////////////////////////////////////////

void
SNiFF__Navigate(string project, string classname, string membername, node n)
{
    string          projectname;

    if (project == NULL || project == "")
	project = Sniff__Project();
    if (project == "")
    {
        print_error("You need to set the 'SNiFFProject' ToolInfo variable before
navigating to SNiFF+");
	return;
    }
    projectname = Sniff__ProjectNameOnly();
    if (projectname == "")
    {
        print_error("You need to set the 'SNiFFProject' ToolInfo variable before
navigating to SNiFF+");
	return;
    }
    if (classname == NULL || classname == "")
	classname = SNiFF__editor_get_class_name();
    if (classname == NULL || classname == "")
    {
	print_error("A class must be specified before you SNiFF it!");
	return;
    }
    system("sniffaccess open_project " + project);
    if (membername == NULL || membername == "")
	system("sniffaccess edit_symbol " + projectname + " \"" + classname + "\" " + "CLASS");
    else if (n != NULL)
    {
	if (n.type == "OMTAttribute")
	    system("sniffaccess edit_symbol " + projectname + " \"" + classname + "::" + membername + "\"");
	else if (n.type == "OMTOperation")
	{
	    int lparen = string_find(membername, 0, "(");
	    if (lparen < string_length(membername))
		membername = string_extract(membername, 0, lparen);
	    system("sniffaccess edit_symbol " + projectname + " \"" + classname + "::" + membername + "\" METHOD_IMPL");
	}
    }
    return;
}

////////////////////////////////////////////////////////////////////////////////

const string    SNIFF_PROJECT_TOOLINFO_NAME = "SNiFFProject";

const string    SNIFF_DIALOG_PROPERTY_NAME = "SNiFFDialog";
const string    SNIFF_DIALOG_PROJECT_PROPERTY_NAME = "SNiFFProject";

string          Sniff__Project = toolinfo_variable(SNIFF_PROJECT_TOOLINFO_NAME);

string
Sniff__Project()
{
    if (Sniff__Project == NULL)
	Sniff__Project = "";
    Sniff__Project = string_strip(Sniff__Project, "B", " ");
    if (Sniff__Project == "")
	return "";
    Sniff__Project = string_search_and_replace(Sniff__Project, "${projdir}", current_projdir());
    Sniff__Project = string_search_and_replace(Sniff__Project, "${system}", current_system());
    Sniff__Project = string_strip(Sniff__Project, "B", " ");
    return Sniff__Project;
}

string
Sniff__ProjectNameOnly()
{
    string          s = Sniff__Project();
    int             i = strrchr(s, "/");
    int             l = string_length(s);

    if (i == l)
	return "";
    return string_extract(s, i, l - i);
}

