//
// 	StP/UML
// 	Confidential property of Aonix
// 	Copyright (c) 2000
// 	All rights reserved
//

#include "uml/rules/qrl/model_management.inc"

// SPR 4052 - Model Management

string
AppendDelimiter( string path )
{
    int path_len = string_length(path);
    int last_delimiter = path_last_delimiter(path);
    // If the last character in the path is not a
    // delimiter then add one.
    if(last_delimiter + 1 != path_len)
        path += "\\";

    // Make all the delimiter look the same
    return path_normalize(path);
}

// ECR 4649 get all subsystems, locked and unlocked
list
GetSubSystem(string sub_sys)
{
    list sub_sys_list;
    string sub_name;
    int i;
    list all_sub_sys_list = list_create("string", 0);

    if(sub_sys == NULL)
	return all_sub_sys_list;

    sub_sys_list = oms_get_subsystems(sub_sys, True, False);
    if(list_count(sub_sys_list) != 0) {
	list_concatenate(all_sub_sys_list, sub_sys_list);
	for(i = 0; i < list_count(sub_sys_list); ++ i) {
	    sub_name = list_get(sub_sys_list, i);
	    list_concatenate(all_sub_sys_list, GetSubSystem(sub_name));
	}
    }
    return all_sub_sys_list;
}

// ECR 4969, lock & unlock subsystems
int LockSubsystemBaselineFuncIsInitialized = 0;

void
LockSubsystemBaselineFunc(ui_event_tp event, ui_prop theprop, int id, string v)
{
    // ECR 5026
    if(stp_repos_is_open() == 0)
        return;
    if(LockSubsystemBaselineFuncIsInitialized == 1) {
	// set the intial baseline name
	// ECR 4969, show only unlocked subsystems
	SetSubsysBaselineName("LockSubsystem", "basename", True, False);
    }
    else
	++ LockSubsystemBaselineFuncIsInitialized;
}

int
LockSubsystemBaseline_cb(ui_event_tp event, ui_prop theprop, int id, string v)
{
    ui_owner uiOwner = to_ui_owner(theprop);
    list selectedList = ui_list_selected_items(to_ui_prop(uiOwner, "basename"));

    // ECR 5026
    if(selectedList == NULL || list_count(selectedList) == 0) {
        print_error("Select subsystem to lock.");
        return -1;
    }

    doLockSubsystem(list_get(selectedList, 0), True);
    return 0;
}

int UnlockSubsystemBaselineFuncIsInitialized = 0;

void
UnlockSubsystemBaselineFunc(ui_event_tp event, ui_prop theprop, int id, string v)
{
    // ECR 5026
    if(stp_repos_is_open() == 0)
        return;
    if(UnlockSubsystemBaselineFuncIsInitialized == 1) {
	// set the intial baseline name
	SetSubsysBaselineName("UnlockSubsystem", "basename", True, True);
    }
    else
	++ UnlockSubsystemBaselineFuncIsInitialized;
}

int
UnlockSubsystemBaseline_cb(ui_event_tp event, ui_prop theprop, int id, string v)
{
    ui_owner uiOwner = to_ui_owner(theprop);
    list selectedList = ui_list_selected_items(to_ui_prop(uiOwner, "basename"));

    if(selectedList == NULL || list_count(selectedList) == 0) {
        print_error("Select subsystem to unlock.");
        return -1;
    }

    doLockSubsystem(list_get(selectedList, 0), False);
    return 0;
}

void
doLockSubsystem(string subsystem, boolean LockOrUnlock)
{
    // ECR 5441
    if(selection_count("item[UmlSubsystemIsLocked & value = 'True' & node[UmlPackage & name = '${subsystem}']]") > 0) {
        if(LockOrUnlock == True) {
            print_error("Subsystem '" + subsystem + "' is already locked");
            return;
        }
    } else if(LockOrUnlock == False) {
        print_error("Subsystem '" + subsystem + "' is not locked");
        return;
    }
    // ECR 5441 end
    list all_sub_sys_list = list_create("string", 0);
    list_append(all_sub_sys_list, subsystem);
    list_concatenate(all_sub_sys_list, GetSubSystem(subsystem));
    // ECR 5161 start
    int idx;
    file_lock lck;
    file lck_file;
    string revision;
    boolean lockExists = False;
    for(idx = 0; idx < list_count(all_sub_sys_list); ++ idx) {
        subsystem = list_get(all_sub_sys_list, idx);
        createIdLists(subsystem, False);
        for_each_in_select("file_lock[Automatic & file[SubsystemFiles]]", lck)
        {
            lck_file = find_by_query("file[${lck.file_id}]");
            if(lck_file != NULL)
            {
                if(lck_file.rev == "")
                    revision = " ";
                else
                    revision = " " + lck_file.rev + " ";

                print_error(formatFile(lck_file) + revision + "is Locked by: "
                          + lck.user + " since " + lck.time);
                lockExists = True;
            }
        }
        id_list_free("SubsystemFiles");
        id_list_free("NotSubsystemFiles");
    }
    if(lockExists == True) {
        print_message("Please quit these editors before locking the subsystem");
        return;
    }
    // ECR 5161 end
    int stpemHandle = editor_send_msg("ngoae", "EditorStart " + to_string(editor_get_msgd_name()) + " "
		    + to_string(editor_get_msgd_handle()), -1);
    editor_send_msg("ngoae", "CiForwardMessages " + to_string(editor_get_msgd_handle()), stpemHandle); // ECR 6853
    string cmd = "oms_lock_subsystem(\\\"" + list_to_string(all_sub_sys_list, ",") + "\\\"," + to_string(LockOrUnlock) + ");";
    editor_send_msg("ngoae", "EditorQrlEval " + cmd, stpemHandle);
}
// ECR 4969 end

int CheckSubsystemBaselineFuncIsInitialized = 0;

void
CheckSubsystemBaselineFunc(ui_event_tp event, ui_prop theprop, int id, string v)
{
    // ECR 5026
    if(stp_repos_is_open() == 0)
        return;

    if(CheckSubsystemBaselineFuncIsInitialized == 1) {
	// set the intial baseline name
	SetSubsysBaselineName("CheckSubsystem", "basename", False, False);
    }
    else
	++ CheckSubsystemBaselineFuncIsInitialized;
}

// ECR 5114
int
CheckSubsystemBaseline_cb(ui_event_tp event, ui_prop theprop, int id, string v)
{
    ui_owner uiOwner = to_ui_owner(theprop);
    ui_prop mySubSysList = to_ui_prop( uiOwner, "basename" );
    list selectedList = ui_list_selected_items(mySubSysList);

    if(selectedList == NULL || list_count(selectedList) == 0) {
        print_error("Select subsystems to check.");
        return -1;
    }

    return CheckListSubsystemSemantics(selectedList);
}

boolean
// ECR 4969
WriteSubsystemBaselineFile(string basedir, string subsystem_list, int lock_after)
{
    if(CheckSubsystems(subsystem_list) == False)
	return False;

    string curproj, cursys;

    // Archive the system
    curproj = current_projdir();
    cursys = current_system();

    path_convert_to_right_slash(curproj);
    path_convert_to_right_slash(basedir);

    // ECR 4969
    string do_lock;
    if(lock_after == 0)
        do_lock = "False";
    else
        do_lock = "True";

    int stpemHandle = editor_send_msg("ngoae", "EditorStart " + to_string(editor_get_msgd_name()) + " "
		    + to_string(editor_get_msgd_handle()), -1);
    editor_send_msg("ngoae", "CiForwardMessages " + to_string(editor_get_msgd_handle()), stpemHandle); // ECR 6853
    // ECR 4969
    string cmd = "sys_dump_baseline(\\\"" + curproj + "\\\",\\\"" + cursys + "\\\",\\\"" + basedir + "\\\",\\\""
	       + subsystem_list + "\\\"," + do_lock + ");";
    editor_send_msg("ngoae", "EditorQrlEval " + cmd, stpemHandle);
    return True;
}

// ECR 4969 only locked, unlocked or all
void
SetSubsysBaselineName(string owner, string field, boolean restrictToLocked, boolean lockedOrUnlocked)
{
    ui_prop myProp = to_ui_prop( owner, field );
    string new_filename;

    if ( myProp != NULL ) {
	ui_list_clear(myProp);

	int i;
        list sub_sys_list = oms_get_subsystems("", False, restrictToLocked, lockedOrUnlocked);
        for(i = 0; i < list_count(sub_sys_list); ++ i) {
            new_filename = list_get(sub_sys_list, i);
            // ECR 5073, add all subsystems, even if they contain no files
            // if (list_count(oms_get_diagram_files_in_subsystem(new_filename, False)) > 0)
                ui_list_append(myProp, new_filename);
        }
    }
}

int CreateSubsystemBaselineFuncIsInitialized = 0;

void
CreateSubsystemBaselineFunc(ui_event_tp event, ui_prop theprop, int id, string v)
{
    // ECR 5026
    if(stp_repos_is_open() == 0)
        return;
    if(CreateSubsystemBaselineFuncIsInitialized == 1) {
	// set the intial baseline name
	// ECR 4969, show only unlocked
	SetSubsysBaselineName("CreateSubsystemBaseline", "basename", True, False);
	// set the intial baseline directory
	SetBaselineDir( "CreateSubsystemBaseline", "basedir" );
    }
    else
	++ CreateSubsystemBaselineFuncIsInitialized;
}

// ECR 5327 added a property sheet to select the directory
int RestoreExternalsFuncIsInitialized = 0;

void
RestoreExternalsFunc(ui_event_tp event, ui_prop theprop, int id, string v)
{
    if(stp_repos_is_open() == 0)
        return;
    if(RestoreExternalsFuncIsInitialized == 1) {
	// set the intial baseline name
	SetBaselineDir( "ReconfigureExternals", "basedir" );
    }
    else
	++ RestoreExternalsFuncIsInitialized;
}

int RestoreExternals1FuncIsInitialized = 0;

void
RestoreExternals1Func(ui_event_tp event, ui_prop theprop, int id, string v)
{
    if(stp_repos_is_open() == 0)
        return;
    if(RestoreExternals1FuncIsInitialized == 1) {
	// set the intial baseline name
        string baseline_path = toolinfo_variable("baseline_path");

        ui_owner uiOwner = to_ui_owner(theprop);
        // If the env variable is not set then
        // set it to a default directory
        if( baseline_path == NULL )
            ui_text_set_value(to_ui_prop(uiOwner, "basedir"), path_normalize( current_projdir() + "baseline\\" ) );
        else
            ui_text_set_value(to_ui_prop(uiOwner, "basedir"), baseline_path);
    }
    else
	++ RestoreExternals1FuncIsInitialized;
}

int
RestoreExternals1_cb(ui_event_tp event, ui_prop theprop, int id, string v)
{
    return RestoreExternals_cb(event, theprop, id, v);
}

int
RestoreExternals_cb(ui_event_tp event, ui_prop theprop, int id, string v)
{
    ui_owner uiOwner = to_ui_owner(theprop);
    string baseline_path = ui_get_value(to_ui_prop(uiOwner, "basedir"));

    string restore_to_dir = current_projdir();
    string current_sys = current_system();

    // Check to see if the directory we're going to restore it to exist
    if( !file_exists(restore_to_dir) ) {
        print_error("Reconfigure not done.  Directory \"" + restore_to_dir + "\" does not exist.");
        return 0;
    }

    list externalReferences;
    externalReferences = list_select( "item[UmlExternalSubsystemName]" );
    if(externalReferences == NULL || list_count(externalReferences) == 0) {
	print_message("Current system contains no external subsystems");
	return 0;
    }
    // message("external subsystems: " + list_count(externalReferences));
    // message(list_to_string(externalReferences, ","));
    print_message("Starting: Reconfigure External Model Elements");

    int i;
    item subsystem;
    string zip_file_path;
    string cmd;
    int result;
    set processedSubs = set_create( "string" );
    set_add( processedSubs, "unassigned" );
    list dirFiles;
    int stpemHandle = -1;

    list zips = list_create("string", 0);

    for (i = 0; i < list_count(externalReferences); ++ i) {
	subsystem = list_get( externalReferences, i );
        if ( !set_is_member( processedSubs, subsystem.value ) ) {
	    // message("Processing: " + subsystem.value);
	    zip_file_path = path_compose(baseline_path, subsystem.value + zip_ext);
	    // message("Checking: " + zip_file_path);
            if( file_exists( zip_file_path ) == True ) {
                path_convert_to_right_slash(zip_file_path);
                list_append( zips, zip_file_path );
	    }
            set_add( processedSubs, subsystem.value );
	}
    }

    // message(list_to_string(zips, ","));
    if(list_count(zips) > 0) {
        path_convert_to_right_slash(restore_to_dir);
        if(stpemHandle == -1)
            stpemHandle = editor_send_msg("ngoae", "EditorStart " + to_string(editor_get_msgd_name()) + " "
                            + to_string(editor_get_msgd_handle()), -1);
        editor_send_msg("ngoae", "CiForwardMessages " + to_string(editor_get_msgd_handle()), stpemHandle); // ECR 6853
        cmd = "sys_load_baseline(\\\"" + restore_to_dir + "\\\",\\\"" + current_sys + "\\\",\\\"" + list_to_string( zips, "\1" )
            + "\\\",True);";
        // message(cmd);
        editor_send_msg("ngoae", "EditorQrlEval " + cmd, stpemHandle);
    }
    else {
        print_message("could not find any subsystem baseline archive in " + baseline_path +
                      " which references external elements in the system " + current_sys);
    }

    return 0;
}

int
CreateSubsystemBaseline_cb(ui_event_tp event, ui_prop theprop, int id, string v)
{
    ui_owner uiOwner = to_ui_owner(theprop);

    ui_prop mySubSysList = to_ui_prop( uiOwner, "basename" );
    list selectedList = ui_list_selected_items(mySubSysList);

    if(selectedList == NULL || list_count(selectedList) == 0) {
        print_error("Select subsystem to baseline.");
        return -1;
    }

    string subsystem = list_get(selectedList, 0);
    // ECR 5012
    return CreateSubsystemBaseLineDoit(uiOwner, subsystem);
}

// ECR 5012
int CreateSubsystemBaseLineDoit(ui_owner uiOwner, string subsystem)
{
    string basedir = ui_get_value(to_ui_prop(uiOwner, "basedir"));

    if ( subsystem != "" && basedir != "" ) {
        if( !HandleSaveDirectory(basedir, uiOwner, "create_dir") )
            // something went wrong.  get out.
            return 0;

        int ignore_lock = ui_choice_value(
            to_ui_prop( uiOwner, "ignore_lock" ));

        // Get the list of lock files in the system
        string lock_list = SystemLockFile(" ", "", "\n");

        if( lock_list != "" ) {
            if( ignore_lock ) {
                // The user wants to ignore lock file(s).
                // We're just write out the lock list so
                // that when they restore the system they'll know
                // that lock file(s) exist when the baseline was created.
                WriteLockInfo(lock_list);
            }
            else {
                // We don't want to create baseline if
                // lock files exist
                list locks = string_to_list(lock_list, "\n");
                int idx;
                for(idx = 0; idx < list_count(locks); ++ idx)
                    print_error(list_get(locks, idx));
                print_error("Baseline not created.  Locks exist.");
                return 0;
            }

        }

	string basename = "";
        if ( use_version_control )
	    basename = subsystem;
	else {
//	    string date_str = time_to_string( time_now(), "%m%d%y");
//	    basename = current_system() + "_" + subsystem + "_" + date_str;
            basename = subsystem;
	}

        int overwrite_file = ui_choice_value(
	    to_ui_prop( uiOwner, "overwrite_file" ));
        // ECR 4969
        int lock_after = ui_choice_value(
	    to_ui_prop( uiOwner, "lock_after" ));

        // TODO: We will probably need to pass the overwrite into the C code
        // and delay removing the file until we are sure no other errors have
	// been encountered. This will be ok until we solidify the C code.
	// TODO: if multiple baselines, we have to check for all of them, not only the top-baseline
        if ( overwrite_file && BaselineFileExist( path_compose( basedir, basename ), 1) )
            delete_file(path_compose( basedir, basename + zip_ext ));
        if ( !BaselineFileExist(path_compose( basedir, basename ), 1) ) {
	    int i;
            // ECR 4649
	    list all_sub_sys_list = list_create("string", 0);
	    list_append(all_sub_sys_list, subsystem);
	    list_concatenate(all_sub_sys_list, GetSubSystem(subsystem));
            // ECR 4969
            if(WriteSubsystemBaselineFile(basedir, list_to_string(all_sub_sys_list, ","), lock_after) == False)
		return 0; // no message here, this is done in WriteSubsystemBaselineFile

           // Check if version control should be started.
           // TODO: since the baseline is created in oae, here needs to be some additional hacking done,
           // maybe a callback?
            if( use_version_control )
                 ShowVersionControlDialog(basedir, basename + zip_ext);
            return 1;
        }

	print_error("Baseline not created.  \"" + path_compose( basedir, basename ) + "\" already exist.");
	return 0;
    }

    if( basedir == "" )
	print_error("Baseline not created.  Missing directory.");
    else
	print_error("Baseline not created.  Missing filename.");

    return 0;
}

int RestoreSubsystemBaselineFuncIsInitialized = 0;

void
RestoreSubsystemBaselineFunc(ui_event_tp event, ui_prop theprop, int id, string v)
{
    // ECR 5026
    if(stp_repos_is_open() == 0)
        return;
    if(RestoreSubsystemBaselineFuncIsInitialized == 1) {
//	// If the "baseline_path" toolinfo variable is set then
//	// we'll use that as a default.
	// set the intial baseline directory
	SetBaselineDir( "RestoreSubsystemBaseline", "basename" );
    }
    else
	++ RestoreSubsystemBaselineFuncIsInitialized;
}

int
RestoreSubsystemBaseline_cb(ui_event_tp event, ui_prop theprop, int id, string v)
{
    ui_owner uiOwner = to_ui_owner(theprop);
    string restore_basename = ui_get_value(to_ui_prop(uiOwner, "basename"));

    if(restore_basename == NULL || restore_basename == "") {
        print_error("Select subsystem to restore.");
        return -1;
    }

    string restore_to_dir = current_projdir();
    string current_sys = current_system();

    // Check to see if the directory we're going to restore it to exist
    if( !file_exists(restore_to_dir) )
    {
        print_error("System not restored.  Directory \"" + restore_to_dir + "\" does not exist.");
        return 0;
    }

    path_convert_to_right_slash(restore_to_dir);
    path_convert_to_right_slash(restore_basename);
    int stpemHandle = editor_send_msg("ngoae", "EditorStart " + editor_get_msgd_name() + " "
			+ editor_get_msgd_handle(), -1);
    editor_send_msg("ngoae", "CiForwardMessages " + to_string(editor_get_msgd_handle()), stpemHandle); // ECR 6853
    string cmd = "sys_load_baseline(\\\"" + restore_to_dir + "\\\",\\\"" + current_sys + "\\\",\\\"" + restore_basename
               + "\\\",False);";

    // ECR 6973 begin
    // editor_send_msg("ngoae", "EditorQrlEval " + cmd, stpemHandle);
    editor_send_msg_sync("ngoae", "EditorQrlEval " + cmd, stpemHandle);
	// ECR 6973 end

    return 1;
}

boolean
CheckSubsystems(string subsystems)
{
    int idx;
    boolean ret = True;
    list subsystem_list = string_to_list(subsystems, ",");
    string subsystem;
    file_lock syslock;

    // ECR 5114
    subsystemMessages = list_create("subsystemMessage", 0);
    // Check for System lock, we cannot baseline a locked system!
    if((syslock = find_by_query("file_lock[system]")) != NULL) {
        print_error("System is locked by " + syslock.user + " at " + syslock.time);
        ret = False;
    }
    // Check for multiple containments of a class
    else if(errorInAssignmentGraph(0) == True) {
        ret = False;
    }
    else {
        for(idx = 0; idx < list_count(subsystem_list); ++ idx) {
            subsystem = list_get(subsystem_list, idx);
            if(selection_count("node[UmlPackage & name = '${subsystem}' & items[UmlStereotype & value = 'subsystem']]") == 0) {
                print_error("No subsystem '" + subsystem + "' is defined");
                ret = False;
            }
            // ECR 4969, cannot create baseline of locked subsystem
            else if(selection_count("item[UmlSubsystemIsLocked & value = 'True' & node[UmlPackage & name = '${subsystem}']]") > 0) {
                print_error("Subsystem '" + subsystem + "' is locked");
                ret = False;
            }
            else {
                // ECR 5114, 5140
                createIdLists(subsystem, idx == 0);
                if(CheckOneSubsystem(0, subsystem, subsystem_list) == False)
                    ret = False;
                id_list_free("SubsystemFiles");
                id_list_free("NotSubsystemFiles");
            }
        }
    }

    if(ret == True)
        print_message("Finished checking subsystems without errors");
    else
        print_error("Finished checking subsystems with errors");
    return ret;
}

// ECR 3651
int select_subsystem_baseline_basedir_callback(ui_event_tp event, ui_prop theprop, int id, string value)
{
    string str;
    ui_owner uiOwner = to_ui_owner(theprop);
    str = ui_get_value(to_ui_prop(uiOwner, "basedir"));
    str = editor_show_dirchooser("Select Subsystem Baseline Directory", str);
    if (str != NULL && string_length(str) != 0) {
        str = path_normalize(str);
        ui_set_value(to_ui_prop(uiOwner, "basedir"), str);
    }

    return 0;
}

int select_subsystem_baseline_file_callback(ui_event_tp event, ui_prop theprop, int id, string value)
{
    string str;
    ui_owner uiOwner = to_ui_owner(theprop);
    str = ui_get_value(to_ui_prop(uiOwner, "basename"));
    str = editor_show_filechooser("Open Subsystem Baseline File", str,
                                  "ZIP Files (*.zip)|*.ZIP|All Files (*.*)|*");
    if (str != NULL && string_length(str) != 0) {
        str = path_normalize(str);
        ui_set_value(to_ui_prop(uiOwner, "basename"), str);
    }

    return 0;
}
// ECR 3651

const string AllSubSysFiles = "(All System Files)";
const string UnassignedFiles = "unassigned";
const string UnassignedFilesDisplay = "(Unassigned Files)";

struct FileRec {
    string filename;
    string sub_sys;
    boolean modified;
};

list allAssignableFiles;
list subSystems;

string
GetSystemName(string sub_sys)
{
    if(sub_sys == UnassignedFilesDisplay)
	return UnassignedFiles;
    return sub_sys;
}

void
InitializeSubsystem(string sub_sys)
{
    int i;
    list file_list = oms_get_diagram_files_in_subsystem(sub_sys, False);
    FileRec entry;

    for(i = 0; i < list_count(file_list); ++ i) {
	entry.filename = list_get(file_list, i);

	entry.sub_sys = GetSystemName(sub_sys);
	entry.modified = False;
	list_append(allAssignableFiles, entry);
    }

    // Sort the list
    allAssignableFiles = list_sort(allAssignableFiles);
}

void
UISetSubSysList(string mySubSysProp, string myFilesInSubSysProp)
{
    ui_prop sub_sys_prop = to_ui_prop("AssignFiles", mySubSysProp);
    ui_prop files_in_sub_sys_prop = to_ui_prop("AssignFiles", myFilesInSubSysProp);
    ui_list_item_tp ui_item;
    list sub_sys_selected_list = ui_list_selected_items(sub_sys_prop);
    string sub_sys_selected;
    int i;

    if( sub_sys_selected_list == NULL || list_count(sub_sys_selected_list) == 0 )
	sub_sys_selected = AllSubSysFiles;
    else
	sub_sys_selected = GetSystemName(list_get( sub_sys_selected_list, 0));

    ui_list_clear(files_in_sub_sys_prop);

    FileRec file_rec;
    for(i = 0; i < list_count(allAssignableFiles); ++ i) {
	file_rec = list_get(allAssignableFiles, i);

	if( sub_sys_selected == AllSubSysFiles || file_rec.sub_sys == sub_sys_selected ) {
  	    ui_item.label = file_rec.filename;
	    ui_list_append(files_in_sub_sys_prop, ui_item);
	}
    }
}

void
UIInitializeSubSysList()
{
    ui_prop sub_sys_left_prop = to_ui_prop("AssignFiles", "SubSysListLeft");
    ui_prop sub_sys_right_prop = to_ui_prop("AssignFiles", "SubSysListRight");
    ui_list_item_tp ui_item;
    int i;

    // Clear the list so we can get a fresh start
    ui_list_clear(sub_sys_left_prop);
    ui_list_clear(sub_sys_right_prop);

    string sub_sys_name;
    for ( i = 0; i < list_count(subSystems); ++ i) {
	sub_sys_name = list_get(subSystems, i);
	// We need to check if the files is unassign..which
	// in this case it's assign to the "default" subsystem.
	// Since we don't want to display "default" we'll just substitue
	// "(Unassigned Files)"
	if(sub_sys_name == UnassignedFiles)
	    sub_sys_name == UnassignedFilesDisplay;
  	ui_item.label = sub_sys_name;
	if(sub_sys_name != AllSubSysFiles)
	    ui_list_append(sub_sys_left_prop, ui_item);
	ui_list_append(sub_sys_right_prop, ui_item);
    }
}

void
InitializeDialog()
{
   int i;
   allAssignableFiles = list_create("FileRec", 0);
   subSystems = list_create("string", 2);
   list_set(subSystems, 0, AllSubSysFiles);
   list_set(subSystems, 1, UnassignedFilesDisplay);
   // ECR 4969 show only unlocked subsystems
   list sub_sys_list = oms_get_subsystems("", False, True, False);

   list_concatenate(subSystems, sub_sys_list);

   // sort the list
   subSystems = list_sort(subSystems);

   string sub_sys_name;
   for(i = 0; i < list_count(subSystems); ++ i) {
	sub_sys_name = GetSystemName(list_get(subSystems, i));
   	if(sub_sys_name == AllSubSysFiles)
	    continue;
	InitializeSubsystem(sub_sys_name);
   }

   UIInitializeSubSysList();
   UISetSubSysList("SubSysListLeft", "FilesInSubSysListLeft");
   UISetSubSysList("SubSysListRight", "FilesInSubSysListRight");
}

int ShowMMDialogFuncIsInitialized = 0;

void
ShowMMDialog(ui_event_tp event, ui_prop theprop, int id, string v)
{
    if(ShowMMDialogFuncIsInitialized == 1) {
	int i;
	FileRec filerec;
	InitializeDialog();
	for(i = 0; i < list_count(allAssignableFiles); ++ i)
	    filerec = list_get(allAssignableFiles, i);
    }
    else
	++ ShowMMDialogFuncIsInitialized;
}

// Delete functions

int
delete_sub_sys_files(ui_prop prop)
{
    int row, i;
    list rows;

    rows = ui_list_selected(prop);

    for (i = 0; i < list_count(rows); ++ i)
    {
	row = list_get(rows, i);
        ui_list_delete(prop, row - i);
    }

    return 0;
}

int
MMOK_cb(ui_event_tp event, ui_prop theprop, int id, string v) // OK
{
    int i;
    FileRec file_rec;
    FileRec last_file_rec = NULL;
    list file_list;
    set subsystem_set = set_create("string");
    string subsystem;
    boolean stopOae = False;

    // ECR 5009
    for(i = 0; i < list_count(allAssignableFiles); ++ i) {
	file_rec = list_get(allAssignableFiles, i);
	if(file_rec.modified == True) {
            if(last_file_rec != NULL)
                oms_set_subsystem_of_app_file(last_file_rec.sub_sys,
                                              last_file_rec.filename,
                                              False);
            last_file_rec = file_rec;
            stopOae = True;
            set_add(subsystem_set, file_rec.sub_sys);
	}
    }

    if(last_file_rec != NULL)
        oms_set_subsystem_of_app_file(last_file_rec.sub_sys,
                                      last_file_rec.filename,
                                      True);

    for(i = 0; i < set_count(subsystem_set); ++ i) {
	subsystem = set_get_element(subsystem_set, i);
        oms_reassign_subsystem_elements(subsystem);
    }

    if(stopOae == True)
        editor_send_msg("ngoae", "EditorQuit", -1);

    return 0;
}

boolean
AssignFileToSubSystem(string sub_sys, string filename)
{
    int i;
    FileRec file_rec;

    for(i = 0; i < list_count(allAssignableFiles); ++ i) {
	file_rec = list_get(allAssignableFiles, i);
	if(file_rec.filename == filename && file_rec.sub_sys != sub_sys) {
	    file_rec.sub_sys = sub_sys;
	    file_rec.modified = True;
     	    // Remove the old entry
	    list_delete(allAssignableFiles, i);
	    // Put in the new one
	    list_append(allAssignableFiles, file_rec);
	    return True;
	}
    }
    return False;
}

void
DisplaySubSysFiles(string sub_sys)
{
    int i;
    FileRec file_rec;
    ui_prop files_in_sub_sys_prop = to_ui_prop("AssignFiles", "FilesInSubSysList");
    ui_list_item_tp ui_item;

    ui_list_clear(files_in_sub_sys_prop);

    for(i = 0; i < list_count(allAssignableFiles); ++ i) {
	file_rec = list_get(allAssignableFiles, i);
	if(file_rec.sub_sys == sub_sys) {
	    ui_item.label = file_rec.filename;
	    ui_list_append(files_in_sub_sys_prop, ui_item);
	}	
    }
}

int
SubSysListLeft_cb(ui_event_tp event, ui_prop theprop, int id, string v)
{
    UISetSubSysList("SubSysListLeft", "FilesInSubSysListLeft");
    return 0;
}

int
SubSysListRight_cb(ui_event_tp event, ui_prop theprop, int id, string v)
{
    UISetSubSysList("SubSysListRight", "FilesInSubSysListRight");
    return 0;
}

void
MoveSubFiles(string src_sub_sys_name,
	string src_files_name,
	string target_sub_sys_name,
	string target_files_name)
{
    list source_list;
    list src_sub_sys_list_selected;
    list target_sub_sys_list_selected;
    string filename;
    string src_sub_sys_selected;
    string target_sub_sys_selected;
    list files_selected_list;
    ui_list_item_tp ui_item;
    int i;
    int j;
	
    ui_prop src_files_prop = to_ui_prop("AssignFiles", src_files_name);
    ui_prop src_sub_sys_prop = to_ui_prop("AssignFiles", src_sub_sys_name);
    ui_prop target_files_prop = to_ui_prop("AssignFiles", target_files_name);
    ui_prop target_sub_sys_prop = to_ui_prop("AssignFiles", target_sub_sys_name);

    src_sub_sys_list_selected = ui_list_selected_items(src_sub_sys_prop);
    target_sub_sys_list_selected = ui_list_selected_items(target_sub_sys_prop);

    // If there are no subsystem selected then we'll use the default system
    if( list_count(src_sub_sys_list_selected) == 0 )
    	src_sub_sys_selected = AllSubSysFiles;
    else
    	src_sub_sys_selected = GetSystemName(list_get( src_sub_sys_list_selected, 0));

    if( list_count(target_sub_sys_list_selected) == 0)
    	target_sub_sys_selected = AllSubSysFiles;
    else
    	target_sub_sys_selected = GetSystemName(list_get( target_sub_sys_list_selected, 0));

    // Don't do anything if src and target are the same
    if( src_sub_sys_selected == target_sub_sys_selected ||
	target_sub_sys_selected == AllSubSysFiles)
	return;

    boolean duplicate = False;
    files_selected_list = ui_list_selected_items(src_files_prop);

    // Don't need to do anything is none of the files are selected
    if(list_count(files_selected_list) == 0)
	return;

    list existing_list = ui_list_value(target_files_prop);

    for (i = 0; i < list_count(files_selected_list); ++ i) {
        filename = list_get(files_selected_list, i);
	if(AssignFileToSubSystem(target_sub_sys_selected, filename)) {
    	    duplicate = False;
	    for(j = 0; j < list_count(existing_list); ++ j) {
		if(filename == list_get(existing_list, j)) {
		    duplicate = True;
		    break;
		}
	    }
	    if(!duplicate) {
	    	ui_item.label = filename;
    	    	ui_list_append(target_files_prop, ui_item);
	    }
	}
    }

    if( src_sub_sys_selected != AllSubSysFiles )
	delete_sub_sys_files(src_files_prop);
}

int
MoveSubFilesToRight_cb(ui_event_tp event, ui_prop theprop, int id, string v) // OK
{
    MoveSubFiles("SubSysListLeft", "FilesInSubSysListLeft",
		 "SubSysListRight", "FilesInSubSysListRight");
    return 0;
}

int
MoveSubFilesToLeft_cb(ui_event_tp event, ui_prop theprop, int id, string v) // OK
{
    MoveSubFiles("SubSysListRight", "FilesInSubSysListRight",
		 "SubSysListLeft", "FilesInSubSysListLeft");
    return 0;
}
