
// DOORS interface QRL support code

#include "rules/qrl/doors_export.inc"
#include "rules/qrl/doors_nav.inc"
#include "rules/qrl/doors_push_renames.inc"
#include "rules/qrl/commands.inc"

const string doors_annot_apptype = "UniqueID";


string
doors_get_obj_GUID(int oms_id)
{
	item it = find_by_query("item[${doors_annot_apptype} && obj_id==${oms_id}]");
	if (NULL == it) {
		return NULL;
	}
	return it.value;
}


string
doors_create_GUID(int oms_id)
{
    // First, make sure the annotation file is not locked
    list annot_locks = list_select("file_lock[file[annot_node[${oms_id}]]]");
    if (annot_locks != NULL && list_count(annot_locks) > 0)
        return ""; // indicate that the annot is locked
    // repeat for links && cntxs
    annot_locks = list_select("file_lock[file[annot_link[${oms_id}]]]");
    if (annot_locks != NULL && list_count(annot_locks) > 0)
        return ""; // indicate that the annot is locked
    annot_locks = list_select("file_lock[file[annot_cntx[${oms_id}]]]");
    if (annot_locks != NULL && list_count(annot_locks) > 0)
        return ""; // indicate that the annot is locked

	string guid = generate_guid();
    note genericNote = find_by_query("note[obj_id=" + oms_id + "]");

	// Execute NGOAE to add the GUID
    editor_send_msg(NGOAE_CLASS, OAE_CMD_ANNOT_EDIT + oms_id, CH_FIRST);

    // JED - fix for 13501
    // To prevent spurious errors, only add the note if necessary
    if (NULL == genericNote)
        editor_send_msg(NGOAE_CLASS, OAE_CMD_NOTE_ADD + "GenericObject",
                CH_FIRST);
    
    editor_send_msg(NGOAE_CLASS, OAE_CMD_NOTE_SEL + "GenericObject", CH_FIRST);
    editor_send_msg(NGOAE_CLASS, OAE_CMD_ITEM_ADD + doors_annot_apptype,
            CH_FIRST);
    editor_send_msg(NGOAE_CLASS, OAE_CMD_ITEM_SEL + doors_annot_apptype,
            CH_FIRST);
    editor_send_msg(NGOAE_CLASS, OAE_CMD_ITEM_SETVAL + "\"" + guid + "\"",
            CH_FIRST);
    editor_send_msg(NGOAE_CLASS, OAE_CMD_FILE_SAVE, CH_FIRST);
    editor_send_msg(NGOAE_CLASS, OAE_CMD_ANNOT_UNLOAD, CH_FIRST);
	return guid;
}

string _sys = current_system();
string _prod = current_product();
string _projdir = string_search_and_replace(current_projdir(),"\\","/");

string
doors_bool_str(boolean b)
{
	if (b)
		return "true";
	return "false";
}

// return a string of DXL code that can be executed under DOORS to add this
// object to DOORS
string
doors_node_export_stmt(node n, string guid, boolean do_nav )
{
    return "stp_create_obj(\"" + _sys + "\",\"" + _projdir + "\", \"node\", \"" + n.type +
    "\", \"" + guid + "\",\"" + string_escape(aonix_print_value(n),"\"")
    + "\", \"" +
    _prod +"\"," + doors_bool_str(do_nav) + ")\n";
}
string
doors_cntx_export_stmt(cntx c, string guid, boolean do_nav)
{
    return "stp_create_obj(\"" + _sys + "\",\"" + _projdir + "\", \"cntx\", \"" + c.type +
        "\", \"" + guid + "\",\"" + 
        string_escape(aonix_print_value(c),"\"") + "\", \"" +
		_prod +"\"," + doors_bool_str(do_nav) + ")\n";
}
string
doors_link_export_stmt(link l, string guid, boolean do_nav)
{
	return "stp_create_obj(\"" + _sys + "\",\"" + _projdir + "\", \"link\", \"" + l.type +
		"\", \"" + guid + "\",\"" + string_escape(aonix_print_value(l),"\"") + "\", \"" +
		_prod +"\"," + doors_bool_str(do_nav) + ")\n";
}

string
doors_make_export_stmt(set sel_nodes, set sel_links, set sel_cntxs, boolean is_sync)
{
	return doors_make_export_nav_stmt(sel_nodes,sel_links,sel_cntxs, False, is_sync);
}

// The do_nav flag tells DOORS whether or not to display the object after
// adding/updating it
string
doors_make_export_nav_stmt(set sel_nodes, set sel_links, set sel_cntxs, 
		boolean do_nav, boolean is_sync)
{
	string export_stmt = "";
	int i;
	int tot_objs = set_count(sel_nodes) + set_count(sel_links) +
		set_count(sel_cntxs);
	node n;
	cntx c;
	link l;
	string guid;
    int totalSelected=0;
    int totalSkipped=0;

	// put out a header that will allocate storage buffers in DOORS
	export_stmt += "stp_create_obj_init(" + tot_objs + ", " +
        doors_bool_str(is_sync) + ")\n";

	for(i=0;i<set_count(sel_nodes);i++) {
		n = set_get_element(sel_nodes,i);
		guid = doors_get_obj_GUID(n.id);
		if (NULL == guid)
			guid = doors_create_GUID(n.id);
        if ("" == guid) // annot file is locked
        {
            totalSkipped++;
            continue;
        }
		export_stmt += doors_node_export_stmt(n,guid,do_nav);
	}
    totalSelected += i;

	for(i=0;i<set_count(sel_links);i++) {
		l = set_get_element(sel_links,i);
		guid = doors_get_obj_GUID(l.id);
		if (NULL == guid)
			guid = doors_create_GUID(l.id);
        if ("" == guid) // annot file is locked
        {
            totalSkipped++;
            continue;
        }
		export_stmt += doors_link_export_stmt(l,guid,do_nav);
	}
    totalSelected += i;

	for(i=0;i<set_count(sel_cntxs);i++) {
		c = set_get_element(sel_cntxs,i);
		guid = doors_get_obj_GUID(c.id);
		if (NULL == guid)
			guid = doors_create_GUID(c.id);
        if ("" == guid) // annot file is locked
        {
            totalSkipped++;
            continue;
        }
		export_stmt += doors_cntx_export_stmt(c,guid,do_nav);
	}
    totalSelected += i;

    // if we couldn't export ANYTHING, bail
    if (totalSelected == totalSkipped)
        return "";
	// a footer that will process the storage buffers
	export_stmt += "stp_create_obj_finish\n";
	return export_stmt;
}

string
doors_tmp_file()
{
    return temp_file("doorsstp","dxl") + ".dxl";
}

// This writes out the string to a file and calls dxlsend

void
doors_send_string_to_doors(string cmds)
{
	string dxlfile = doors_tmp_file();
    string editor = current_editor();
    string qrlCall;
    string lastArg;

    if (NULL == editor || "QRP" == to_upper(editor))
    {
        // JED - use system()
        qrlCall = "system("; 
        lastArg = "";
        dxlFileName = "";
    }
    else
    {
        qrlCall = "editor_system(";
        lastArg = ", \"doors_dummy_cb\"";
        dxlFileName = dxlfile;
    }

	write_file(dxlfile,cmds);

    // This is necessary because the UNIX version of the dxlfile
    // program can't handle files over 2K in size
    if (is_windows_platform())
        qrlCall += "\"dxlfile " + dxlfile + "\"" + lastArg + ");";
    else
        qrlCall += "\"dxlsend '#include \"" + dxlfile + "\"'\"" + lastArg + ");";

    qrl_eval(string_escape(qrlCall,"\\"));
    
    if ("" == dxlFileName)
    {
         // using system, which is synchronous and has no callback
        // delete_file(dxlfile);
    }
}

string dxlFileName;

void
doors_dummy_cb(int i, int j) 
{
    delete_file(dxlFileName);
}
