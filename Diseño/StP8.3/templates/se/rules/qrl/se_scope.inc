//
//      StP/SE
//      Confidential property of Interactive Development Environments, Inc.
//      Copyright (c) 1995
//      All rights reserved
//

#include "rules/qrl/gde_qrl_std.inc"
#include "qrl/include/opt_displaymark.inc"
#include "qrl/include/se_utils.inc"
#include "rules/qrl/se_rename.inc"

string prev_file;
string prev_dir;


// dse_file_dmark and sce_file_dmark only differ in the suffix they pass by default

string
se_file_dmark(int id, boolean is_sce)

{
    gde_node gn;

    gn = gde_node_find_by_psymid(id);
    return get_file_string(gn, is_sce);
}


string
se_default_file_value(gde_node gn)

{
    string nt;

    nt = gde_nt_name(gde_node_nt(gn));

    if (nt == "Module" || nt == "GlobalData")
	return get_file_string(gn, True);
    else
	return get_file_string(gn, False);
}



string 
idehook_se_file_name_func(gde_mapsymbol thissym)

{
    gde_node gn;
    string nt;

    gn = to_gde_node(thissym);
    nt = gde_nt_name(gde_node_nt(gn));

    if (nt == "Module" || nt == "GlobalData")
        return get_file_string(gn, True);
    else
        return get_file_string(gn, False);
}






string 
get_file_string(gde_node gn, boolean is_sce)

{
    string val, label, equiv;
    list arcl;

    if (! is_sce)
    {
	arcl = gde_node_inarcs_no_comments(gn);
	if (list_count(arcl) > 0)
	    return "";
    }

    label = gde_node_label(gn);
    if (str_no_val(label))
	return "";

    val = gde_node_prop(gn, "SEFile");

    equiv = make_se_fname(label);
    if (is_sce)
	equiv += ".c";
    else
	equiv += ".h";

    if (str_has_val(val))
    {
	if (equiv == val)
	    gde_node_clear_prop(gn, "SEFile");
	return val;
    }

    else 
	return equiv;
}


string
se_dir_dmark(int id, boolean is_sce)
{
    gde_node gn;
    string val;
    list arcl;

    gn = gde_node_find_by_psymid(id);

    if (! is_sce)
    {
	arcl = gde_node_inarcs_no_comments(gn);
	if (list_count(arcl) > 0)
	    return "";
    }

    val = gde_node_prop(gn, "SEDirectory");

    if (str_no_val(val))
        return ".";
    else
	return val;
}



string 
idehook_se_directory_name_func(gde_mapsymbol thissym)

{
    gde_node gn;
    string val, nt;
    list arcl;

    gn = to_gde_node(thissym);

    nt = gde_nt_name(gde_node_nt(gn));
    if (nt != "Module" && nt != "GlobalData")
    {
	arcl = gde_node_inarcs_no_comments(gn);
	if (list_count(arcl) > 0)
	    return "";
    }

    val = gde_node_prop(gn, "SEDirectory");

    if (str_no_val(val))
    {
	gde_node_set_prop(gn, "SEDirectory", ".");
        return ".";
    }
    else
	return val;
}




boolean
SEDirectoryDisplayMarkCond()
{
    return True;
}
 


boolean
SEFileDisplayMarkCond()
{
    return True;
}


