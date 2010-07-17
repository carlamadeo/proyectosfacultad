//      StP/Uml
//      Confidential property of Aonix
//      Copyright (c) 1997
//      All rights reserved
//


// Package rename 
const	string	PACKAGE_RENAME_PROP_SHEET = "PackageRename";

void
uml_package_rename_init(gde_mapsymbol the_package)
{
    uml_rename_set_curr_label(PACKAGE_RENAME_PROP_SHEET, the_package);
}

void
uml_package_rename(gde_mapsymbol the_package)
{
    gde_builtin("PropertySheetShow " + PACKAGE_RENAME_PROP_SHEET);
    gde_default_rename_clear_new_label(PACKAGE_RENAME_PROP_SHEET);
    uml_package_rename_init(the_package);
}

void
uml_package_rename_action_done()
{
    gde_mapsymbol ms = gde_selected_mapsymbol();
    ui_prop p = to_ui_prop(PACKAGE_RENAME_PROP_SHEET, "All");
    
    if (gde_rename_action_done_standard_checks(ms, p, "uml_package_rename") == False)
	return;
    
    uml_package_rename_init(ms);
}



