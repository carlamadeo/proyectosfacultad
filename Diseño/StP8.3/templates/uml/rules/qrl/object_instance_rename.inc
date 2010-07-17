//ECR 6433 begin

// Functions for object instance GOR
// file is included at the end of object_instance.inc

const string OBJECT_RENAME_PROP_SHEET = "ObjectInstanceRename";

void
uml_object_instance_rename_init(gde_mapsymbol object_mapsymbol)
{
    uml_rename_set_curr_label(OBJECT_RENAME_PROP_SHEET, object_mapsymbol);
}

void
uml_object_instance_rename(gde_mapsymbol object_mapsymbol)
{
    gde_builtin("FileSaveNoUpdate"); // ECR 6847
    gde_builtin("PropertySheetShow " + OBJECT_RENAME_PROP_SHEET);
    gde_default_rename_clear_new_label(OBJECT_RENAME_PROP_SHEET);
    uml_object_instance_rename_init(object_mapsymbol);
}

void 
uml_object_instance_rename_action_done()
{
}

//ECR 6433 end