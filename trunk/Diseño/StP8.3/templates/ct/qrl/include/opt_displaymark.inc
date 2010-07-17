////////////////////////////////////////////////////////////////////////////////
//
//      sccsid[] = %W% %Y% %D%
//      StP/Core
//      Confidential property of Interactive Development Environments, Inc.
//      Copyright (c) 1992-1995
//      All rights reserved
//
////////////////////////////////////////////////////////////////////////////////

const	string	USE_MAPPING_VAR = "UseMappings";

struct	itemstr_tp
{
    string	type;
    string	value;
};

list
item_list_to_struct_list(list l)
{
    int		i;
    list	r;
    itemstr_tp	iv;
    item	it;

    if (l == NULL)
    {
	return(NULL);
    }
    r = list_create("itemstr_tp", 0);
    for(i = 0; i < list_count(l); i = i + 1)
    {
	it = list_get(l, i);
	iv.type = it.type;
	iv.value = it.value;
	list_append(r, iv);
    }
    return(r);
}

list 
struct_list_to_string_list(list items)
{
    itemstr_tp	one_item;
    int		i;
    list	res;

    res = list_create("string", 0);


    for (i = 0; i < list_count(items); i = i + 1)
    {
	one_item = list_get(items, i);

	if (one_item.value == "")
	    continue;
	list_append(res, one_item.value);
    }

    return res;
}

struct opt_dm_tp
{
    string	var;
    string	map;
    string	val;
};

void
print_opt_dm_tp(opt_dm_tp v)
{
    message("var: " + v.var);
    message("map: " + v.map);
    message("val: " + v.val);
}

list	opt_dm_vals = NULL;

int
opt_dmark_get_val_ind(string var, string mapping)
{
    int		i;
    opt_dm_tp	e;

    if (opt_dm_vals == NULL)
    {
	opt_dm_vals = list_create("opt_dm_tp", 0);
	return(-1);
    }
    for(i = 0; i < list_count(opt_dm_vals); i = i + 1)
    {
	e = list_get(opt_dm_vals, i);
	if (e.var == var && e.map == mapping)
	{
	    return(i);
	}
    }
    return(-1);
}

string
get_current_mapping()
{
    string	map;

    map = vstring_variable(USE_MAPPING_VAR);

    if (map == NULL || map == "")
    {
	map = toolinfo_variable(USE_MAPPING_VAR);
    }
    return(map);
}

void
opt_dmark_set_val(string var, string value)
{
    opt_dm_tp	v;
    int		i;
    string	map;

    map = get_current_mapping();

    // message("opt_dmark_set_val: map set to " + map);

    i = opt_dmark_get_val_ind(var, map);

    if (i < 0)
    {
	if (value == NULL)
	{
	    return;
	}
	v.var = var;
	v.val = value;
	v.map = map;
	list_append(opt_dm_vals, v);
    } else if (value == NULL)
    {
	v = list_get(opt_dm_vals, i);
	list_delete(opt_dm_vals, i);
    } else {
	v.var = var;
	v.val = value;
	v.map = map;
	list_set(opt_dm_vals, i, v);
    }
    // print_opt_dm_tp(v);
}

string
opt_dmark_val(string var, string map)
{
    opt_dm_tp	v;
    int		i;
    string	res;

    i = opt_dmark_get_val_ind(var, map);
    if (i < 0)
    {
	res = NULL;
    } else 
    {
	v = list_get(opt_dm_vals, i);
	res = v.val;
    }
    // message("opt_dmark_val (" + var + "," + map + "): " + res);
    return(res);
}

void
opt_updmark(int symid)
{
    // message("opt_updmark: symid: " + symid);
    gde_builtin("SymbolUpdateDmarks " + symid);
}

int
get_index_from_item_list(list l, string type, string val)
{
    int		i;
    itemstr_tp	it;

    for(i = 0; i < list_count(l); i = i + 1)
    {
	it = list_get(l, i);

	if (it.type == type)
	{
	    if (val == NULL)
		return(i);
	    if (val == it.value)
		return(i);
	}
    }
    return(-1);
}

int
put_in_item_list(list l, string type, string val, boolean dupl)
{
    int		i;
    itemstr_tp	it;

    if (dupl == False)
	i = get_index_from_item_list(l, type, NULL);
    else
	i = get_index_from_item_list(l, type, val);
    if (i < 0)
    {
	it.type = type;
	it.value = val;
	list_append(l, it);
	i = list_count(l);
    } else {
	it.type = type;
	it.value = val;
	list_set(l, i, it);
    }
    return(i);
}

int
delete_from_item_list(list l, string type, string value)
{
    int		i;

    i = get_index_from_item_list(l, type, value);
    if (i >= 0)
    {
	list_delete(l, i);
    }
    return(i);
}

// Finds the annot file for the node, checks it out if
// necessary and then executes the qrl_expression.  Finally,
// checks it back in (iff it had to check it out).
 
void
node_annot_checkout_then_exec(int id, string qrl_expression)
{
    file          file;

    file = find_by_query("file[annot_node[" + id + "]]");
    annot_checkout_then_exec(file, qrl_expression);
}

// Finds the annot file for the link, checks it out if
// necessary and then executes the qrl_expression.  Finally,
// checks it back in (iff it had to check it out).
 
void
link_annot_checkout_then_exec(int id, string qrl_expression)
{
    file          file;

    file = find_by_query("file[annot_link[" + id + "]]");
    annot_checkout_then_exec(file, qrl_expression);
}

void
annot_checkout_then_exec(file file, string qrl_expression)
{
    string        fname, cmd = C_PRG + C_ANNOT_EDT + REDIRECT_OUT;
    string        outfile;
    subproc_tp    proc;

    // file doesn't exist yet - no checkout needed

    if (file == NULL)
    {
	qrl_eval(qrl_expression);
	return;
    }
    if (file.rev != NULL)
    {
	fname = path_compose(current_projdir(), current_system(), "ant_files",
		file.name + ".ant");

	if (!write_file_access(fname))
	{
	    print_message("Annotation " + file.name + " is Versioned, " +
			  "Attempting Check Out...");
	    outfile = tmpfile(file.name, 0);
	    proc.pid = editor_system(cmd, "annotCheckoutDoneProc");
	    proc.success_msg = "Check Out Succeeded.";
	    proc.failure_msg = "Check Out Annotation " + file.name + " Failed.";
	    proc.tmpfile = NULL;
	    proc.outfile = outfile;
	    proc.client_data = file.name + ":" + qrl_expression;
	    list_append(subProcs, proc);
	    return;
	}
    }
    qrl_eval(qrl_expression);
}

int
annotCheckoutDoneProc(int pid, int status)
{
    subproc_tp  proc;
    string      outfile, infile, fname, expression;
    string      cmd = C_PRG + C_ANNOT_DTA + C_ANNOT_GET + 
		      REDIRECT_IN + REDIRECT_OUT;
    int         pos;

    proc = subProc(pid);
    doneProc(pid, status);

    pos = string_find(proc.client_data, 0, ":");
    fname = string_extract(proc.client_data, 0, pos);
    expression = string_extract(proc.client_data, pos + 1, 
				string_length(proc.client_data) - pos - 1);
    qrl_eval(expression);

    // now check it back in...

    print_message("Attempting Check In of Annotation " + fname + "...");
    outfile = proc.outfile;
    infile =  proc.outfile + "0";
    write_file(infile, "New version created by StP\n");
    proc.pid = editor_system(cmd, "doneProc");
    proc.success_msg = "Check In Succeeded.";
    proc.failure_msg = "Check In Failed.";
    proc.tmpfile = NULL;
    proc.outfile = outfile;
    proc.client_data = NULL;
    list_append(subProcs, proc);
    return(0);
}

int
send_start_edit_annot(int id)
{
    string name = editor_get_msgd_name();
    string handle = to_string(editor_get_msgd_handle());

    return(editor_send_msg(NGOAE_CLASS, 
			   OAE_CMD_ANNOT_EDIT + id + " " + name + " " + handle,
			   CH_FIRST));
}

// these two functions are now identical. The first used to just
// update the note and save the file, now the OAE does all this
// in one fell swoop (i.e. FileSave only updates notes that have changed)

void
send_oae_save_update_unload_note(int handle)
{
    send_oae_save_update_unload(handle);
}

void
send_oae_save_update_unload(int handle)
{
    editor_send_msg(NGOAE_CLASS, OAE_CMD_FILE_SAVE, handle);
    editor_send_msg(NGOAE_CLASS, OAE_CMD_ANNOT_UNLOAD, handle);
}

// ECR 7174 begin
int
send_start_edit_annot_sync(int id)
{
    string name = editor_get_msgd_name();
    string handle = to_string(editor_get_msgd_handle());

    return(editor_send_msg_sync(NGOAE_CLASS, 
			   OAE_CMD_ANNOT_EDIT + id + " " + name + " " + handle,
			   CH_FIRST));
}

// these two functions are now identical. The first used to just
// update the note and save the file, now the OAE does all this
// in one fell swoop (i.e. FileSave only updates notes that have changed)

void
send_oae_save_update_unload_note_sync(int handle)
{
    send_oae_save_update_unload_sync(handle);
}

void
send_oae_save_update_unload_sync(int handle)
{
    editor_send_msg_sync(NGOAE_CLASS, OAE_CMD_FILE_SAVE, handle);
    editor_send_msg_sync(NGOAE_CLASS, OAE_CMD_ANNOT_UNLOAD, handle);
}
// ECR 7174 end

string
stpem_string_val(string v)
{
    return("\"" + string_escape(v, "\"") + "\"");
}

void
send_change_note_item_value(int oid, string ntp, string itp, string v)
{
    int	handle = send_start_edit_annot(oid);
    string	cmd;

    cmd = OAE_CMD_NOTE_SEL + ntp;
    editor_send_msg(NGOAE_CLASS, cmd, handle);

    cmd = OAE_CMD_ITEM_SEL + itp;
    editor_send_msg(NGOAE_CLASS, cmd, handle);

    cmd = OAE_CMD_ITEM_DEL;
    editor_send_msg(NGOAE_CLASS, cmd, handle);

    cmd = OAE_CMD_ITEM_SETVAL + stpem_string_val(v);
    editor_send_msg(NGOAE_CLASS, cmd, handle);

    send_oae_save_update_unload_note(handle);
}

// ECR 7174 begin
void
send_change_note_item_value_sync(int oid, string ntp, string itp, string v)
{
    int	handle = send_start_edit_annot_sync(oid);
    string	cmd;

    cmd = OAE_CMD_NOTE_SEL + ntp;
    editor_send_msg_sync(NGOAE_CLASS, cmd, handle);

    cmd = OAE_CMD_ITEM_SEL + itp;
    editor_send_msg_sync(NGOAE_CLASS, cmd, handle);

    cmd = OAE_CMD_ITEM_DEL;
    editor_send_msg_sync(NGOAE_CLASS, cmd, handle);

    cmd = OAE_CMD_ITEM_SETVAL + stpem_string_val(v);
    editor_send_msg_sync(NGOAE_CLASS, cmd, handle);

    send_oae_save_update_unload_note_sync(handle);
}
// ECR 7174 end
