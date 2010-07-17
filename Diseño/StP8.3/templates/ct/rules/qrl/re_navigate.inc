//
//      sccsid[] = %W% %Y% %D%
//      StP/SE
//      Confidential property of Interactive Development Environments, Inc.
//      Copyright (c) 1995
//      All rights reserved
//

#include "qrl/include/general.inc"

// there is a dependency between the SE C Code Browser and this - the call to
// the function is guarded by an is_product_se() thing
#include_if_exists "qrl/properties/c_browse_cb.inc"

// common routines to browse to code and design.  Used by SE synchronization
// and by all the routines that navugate to code in the SMDB

struct cur_nrs_tp

{
    node_ref nr;
    file file;
    node node;
};


list NodeRefs;

void
navigate_to_editor(list nrs)

{
    list l = list_create("string", 0);
    int i;
    ui_prop prop;
    cur_nrs_tp nr;
    string diag;

    if (nrs == NULL || list_count(nrs) == 0)
        return;

    NodeRefs = list_create("cur_nrs_tp", 0);
    // build a second list of appropriate strings
    for (i = 0; i < list_count(nrs); i++)
    {
        nr.nr = list_get(nrs, i);
        nr.file = find_by_query("file[id = ${nr.nr.file_id}]");
        nr.node = find_by_query("node[id = ${nr.nr.node_id}]");

        // this used to have a bunch of ifs so that it could put
        // in nice names for editors - if we have time we should
        // lookup the editor type, and find its proper name, but
        // this will need changes to libdps, so I'll take the 
        // easy way for now.
        diag = nr.file.type;

        list_append(NodeRefs, nr);
        if (list_count(nrs) == 1)
        {
            DiagramObjectSelector_cb(NULL, NULL, -1, NULL);
            return;
        }
        else if (i == 0)
        {
            // bring up the selector
            ui_prop_sheet_pop_up("DiagramObjectSelector");
            prop = to_ui_prop("DiagramObjectSelector", "Chooser");
            ui_list_clear(prop);
        }
        ui_list_append(prop, nr.node.type + " `" + nr.node.name + "' in " + diag + " diagram `" + nr.file.name + "'");
    }
    
    return;
}


int
DiagramObjectSelector_cb(ui_event_tp event, ui_prop theprop, int id, string value)

{
    list items;
    int item, row, rc = 0;
    ui_prop selector = to_ui_prop("DiagramObjectSelector", "Chooser");
    cur_nrs_tp nr;

    if (id == -1)
        row = 0;
    else
    {
        row = ui_list_selected_row(selector);
        if (row < 0)
        {
            print_error("No object selected.  Choose an object before selecting Browse ...");
            return rc;
        }

        if (ui_button_is_ok(theprop))
            rc = 1;
    }
    nr = list_get(NodeRefs, row);

    list ed_names = file_editor_names( nr.file.type );
    if ( list_count( ed_names ) > 1 )
      {
        print_error( "re_navigate.inc: got multiple editor types to navigate to." );
        print_error( "using the first one." );
      }

    editor_send_msg( list_get( ed_names, 0 ), "EditorQrlEval \"NavLoadAndFind(\\\"" + nr.file.name + "\\\",\\\"" + nr.nr.appid + "\\\");\"", -1 );
    return rc;
}
 



void
dismiss_chooser()
{
    ui_prop p = to_ui_prop("BrowserCodeBrowseChooser", "Chooser");
    ui_list_clear(p);
    if (ui_owner_visible(p))
        ui_owner_dismiss(to_ui_owner("BrowserCodeBrowseChooser"));
}


list CurBrowseList;


// pass in NULL to footer if it is being called from the parser_sh to
// display files that failed parsing
int
execute_chooser(string tf, ui_prop footer, int from_parser)
{
    ui_prop prop;
    int i;
    string name, cmdstr, s, label;
    list l, choosel = list_create("string", 0);

    if ( read_file_access(tf))
        s = read_file(tf);
    else
        s = NULL;

    if (s == NULL || s == "")
    {
        dismiss_chooser();
        if (footer != NULL)
            ui_footer_set_value(footer, "No objects found.");
        // this may be needed by SE, but it isn't defined for UML.
        // do we need to move its defn into this file?
        // CurKey = -1;
        return 0;
    }

    CurBrowseList = string_to_list(s, "\n");

    if ( from_parser == 0 )
    {
        if (list_count(CurBrowseList) == 1)
        {       
            dismiss_chooser();
            browse_to_row(0);
            
            if (footer != NULL)
                ui_footer_set_value(footer, "1 object found.");
            return 0;
        }
        else if ( footer != NULL )
            ui_footer_set_value(footer, list_count(CurBrowseList) + " objects found.");
    }

    ui_prop_sheet_pop_up("BrowserCodeBrowseChooser");
    prop = to_ui_prop("BrowserCodeBrowseChooser", "Chooser");
    ui_list_clear(prop);

    if ( from_parser == 1 )
        ui_activate( to_ui_prop("BrowserCodeBrowseChooser", "Apply"), False );
    else
        ui_activate( to_ui_prop("BrowserCodeBrowseChooser", "Apply"), True );

    for (i = 0; i < list_count(CurBrowseList); i++)
    {
        s = list_get(CurBrowseList, i);
        l = string_to_list(s, "\t");

        if (list_count(l) < 4)
        {
            print_error( "Code Browse: Internal Format Error in xrb_search" );
            print_error( "Returned : " + s ); 
            return 1;
        }

        label = list_get(l, 0) + " <" + list_get(l, 3) + "/" + 
          list_get(l, 1) + ">";
        if (list_count(l) == 6)
            label = list_get(l, 5) + "::" + label; 
        list_append(choosel, label);
    }
    if (list_count(choosel) > 0)
        ui_list_append(prop, choosel);

    return 0;
}



int
REBrowserBrowseCodeDoIt_cb(ui_event_tp event, ui_prop theprop, int id, string value)
 
{
    ui_prop prop = to_ui_prop("BrowserCodeBrowseChooser", "Chooser");
    int row = ui_list_selected_row(prop);

    browse_to_row(row);
    return 0; // really the OK button, but we do not want the dialog to go away.
}


// see if the file we are navigating to needs to have part of its path changed
// the format of the substitution string is
// source_navigation_substitution=<source>=<object>[;<source>=<object>]*
// so multiple substitution paths are allowed
//
string
REBrowse_correct_path( string file )
{
    string result = file;
    string subst_list = toolinfo_variable( "source_navigation_substitution" );
    if ( subst_list != "" )
    {
        list substs = string_to_list( subst_list, ";" );
        string this_subst, source, object;
        int loop, eq;
        
        for( loop = 0; loop < list_count(substs); loop++ )
        {
            this_subst = list_get(substs, loop);
            eq = string_find(this_subst, 0, "=");
            if (eq > 0 && eq < string_length(this_subst))
            {
                source = string_extract(this_subst, 0, eq);
                object = string_extract(this_subst, eq + 1,
                                        string_length(this_subst) - eq - 1);
                result = string_search_and_replace(result, source, object);
            }
        }
    }
    return result;
}

// ECR 6662 begin
string
remove_multiple_backslashes_from_path(string str)
{
    if (!is_windows_platform())
        return str;
        
    string result = "", curr, last = "";
    int pos = 0, len = string_length(str);
    while (pos < len)
    {
        curr = string_extract(str, pos, 1);
        result = result + curr;
        pos++;
        last = curr;
        if (curr != "\\")
            break;
    }
    while (pos < len)
    {
        curr = string_extract(str, pos, 1);
        pos++;
        if (curr == "\\" && last == "\\")
            continue;
        result = result + curr;
        last = curr;
    }
    return result;
}

// Returns position of last occurence of <to_find> in <the_string>
int
str_find_last(string the_string, int start, string to_find)
{
    int last_pos = -1;
    int pos = start;
    int len = string_length(the_string);
    
    while (1)
    {
        pos = string_find(the_string, pos, to_find);
        if (pos == len)
            break;

        last_pos = pos;
        pos++;
    }

    return last_pos;
}

// Guess the name of xxx_source_editor toolinfo variable from file extension
// For example, giving file ABC.CPP we'll get "cpp_source_editor"
string
RE_get_specific_source_edit_cmd_prefix(string file)
{
    string result = NULL;
    list file_exts = string_to_list("c cpp cxx h hxx idl java cls ada adb ads", " ");
    list prefixes = string_to_list("cpp cpp cpp cpp cpp idl java vb ada ada ada", " ");
    
    string file_name = file_part(file);
    int pos = str_find_last(file_name, 0, ".");
    if (pos != -1)
    {
        string ext = string_extract(file_name, pos + 1, string_length(file_name) - pos - 1);
        pos = list_find(file_exts, 0, to_lower(ext));
        if (pos != list_count(file_exts))
            result = list_get(prefixes, pos);
    }
    
    list_clear(file_exts);
    list_clear(prefixes);
    return result;
}
// ECR 6662 end

// if a source editor (se_source_editor in TI file) is supplied, then 
// build a command string, and return it. If no editor has been supplied
// then return NULL.
string
REBrowse_subst_file_and_line( string file, int lineno )
{
    string edit_cmd;
        // this is here for historical reasons - remove in '98
    edit_cmd = toolinfo_variable( "se_source_editor" );
    if ( edit_cmd == NULL || edit_cmd == "" )
      edit_cmd = toolinfo_variable( "source_editor" );
// ECR 6662 begin
    string specific_edit_cmd_prefix = RE_get_specific_source_edit_cmd_prefix(file);
    if (specific_edit_cmd_prefix != NULL)
    {
        string specific_edit_cmd = toolinfo_variable(specific_edit_cmd_prefix + "_source_editor");
        if (specific_edit_cmd != NULL)
            edit_cmd = specific_edit_cmd;
    }
// ECR 6662 end
    if ( edit_cmd != NULL )
    {
      // it's set to MSDev if we are using the integration with Developer
      // Studio
        if ( edit_cmd == "MSDev" )
          return edit_cmd;

        int len = string_length( edit_cmd );

    // ECR 5378 start
        if ( string_find( edit_cmd, 0, "[FILE]" ) == len )
        {
        edit_cmd = edit_cmd + " " + file;
        } 
    else 
    {
        edit_cmd = string_search_and_replace( edit_cmd, "[FILE]", file );
    }
    // ECR 5378 end
        edit_cmd = string_search_and_replace( edit_cmd, "[LINE]", to_string( lineno ));
                                                                    
        return edit_cmd;
    }
    return NULL;
}



int
browse_to_row(int row)
 
{
    ui_prop tvprop, infile;
    string s, cmd, file, fcontents, edit_cmd;
    list l;
    int lineno;

    if (row >= 0)
    {
        s = list_get(CurBrowseList, row);
        l = string_to_list(s, "\t");
        file = list_get(l, 3) + "/" + list_get(l, 1);
        lineno = to_int(list_get(l, 2));

        // if there is a substitution needed for the real location of
        // the source file, then do that
        file = REBrowse_correct_path(file);
  
        // now see if the file has been modified since it was last parsed
        int modTime = file_modification_time(file);
        if (modTime == -1)
        {
            print_error( "Source file '" +  file + "' is not accessible." );
            //print_error( "Source file '%s' is not accessible.", file );
            return 1;
        }
        int parsedTime = to_int(list_get(l, list_count(l) - 1));
        if (parsedTime != 0 && modTime > parsedTime)
        {
            print_error( "Source file has been changed since it was last parsed." );
            print_error( "Navigation may not be exact." );
        }

            // if an editor has been provided instead
            // then go right ahead and use it
        edit_cmd = REBrowse_subst_file_and_line( file, lineno );
        if ( edit_cmd != NULL )
        {
            if ( edit_cmd == "MSDev" )
              {
                if ( navigate_to_msdev( file, lineno ) == 0 )
                  print_error( "Navigation to Microsoft Developer Studio failed - using default text widget" );
                else
                  {
                    print_message( "Navigation succeeded" );
                    return 0;
                  }
              }
            else
              {
                print_message( "Executing \"" + edit_cmd + "\"" );
        // ECR 5378 start
                // return system( edit_cmd ); // system is syncronous. You need async call here so use editor_system
        return editor_system( edit_cmd, NULL);
        // ECR 5378 end
              }
        }

        // the default is to pop up the code viewer
        ui_prop_sheet_pop_up("TextViewBuffer");
        tvprop = to_ui_prop("TextViewBuffer", "TheTextViewBuffer");
        if ( !is_product_se() )
          {
            ui_activate( to_ui_prop( "TextViewBuffer", "GotoSCE" ), False );
            ui_activate( to_ui_prop( "TextViewBuffer", "GotoDSE" ), False );
          }
        fcontents = read_file(file);
// ECR 6895 begin
    ui_text_set_value(tvprop, "");
// ECR 6895 end
        ui_text_set_value(tvprop, fcontents); 
        ui_text_scroll_to(tvprop, lineno - 1);
        ui_text_set_readonly(tvprop, True);
        ui_text_select(tvprop, lineno - 1, lineno - 1);

        // put the current file in the field at the top
        infile = to_ui_prop("TextViewBuffer", "TextFile");
        ui_text_set_value(infile, file);
    }
    else
        print_error("No object selected.  Choose an object before selecting Browse ...");

    return 0;
}




int
REBrowserBrowseDesignDoIt_cb(ui_event_tp event, ui_prop theprop, int id, string value)
 
{
    string interesting_file_types;
    ui_prop prop = to_ui_prop("BrowserCodeBrowseChooser", "Chooser");
    int row = ui_list_selected_row(prop), bracket;
    string name, s, query, sig = "";
    list nrs, l;

    if ( is_product_se())
      interesting_file_types = "[SceDiagram || DseDiagram || FceDiagram]";
    else
      interesting_file_types = "";

    if (row >= 0)
    { 
        s = list_get(CurBrowseList, row);
        l = string_to_list(s, "\t");

        name = list_get( l, 0 );
        if ( is_product_se() )
          {
            name = string_translate( name, "_\n\t", "   " );
            // knock off any brackets
            bracket = string_find( name, 0, "(" );
            if ( bracket < string_length( name ))
              name = string_extract( name, 0, bracket );
            else
              {
                int colon = string_find( name, 0, "::" );
                if ( colon < string_length( name ))
                  name = string_extract( name, 0, colon );
              }
          }
        else
          {
            bracket = string_find( name, 0, "(" );
            if ( bracket < string_length( name ))
              {
                sig = string_extract( name, bracket + 1,
                                      string_length( name ) - bracket - 2);
                // remove any dots and turn them into spaces
                sig = string_translate( sig, ".", " " );
                sig = " && sig = '" + sig + "'";
                name = string_extract( name, 0, bracket );
              }
          }

        query = "node_ref[file" + interesting_file_types + " && node[name = '" + name + "'" + sig + "]]";
        nrs = list_select(query);
        navigate_to_editor(nrs);
    }
    else
        print_error("No object selected.  Choose an object before selecting Browse ...");
    return 0;
}




int
REBrowserBrowseCodeSelectRow_cb(ui_event_tp event, ui_prop theprop, int id, string value)
 
{
  // in SE, when an item is selected, we see its values get copied back
  // into the Code browser - this may happen soon for OO too, but not yet
    if ( is_product_se() )
      CCodeBrowseNotifySelect();

    return 0;
}

// ECR 3651 begin - moved code from TextViewerLoad_cb to LoadAndShowTextFile
int LoadAndShowTextFile()
{
    ui_prop viewer, infile;
    string newf, fcontents;

    viewer = to_ui_prop("TextViewBuffer", "TheTextViewBuffer");
    infile = to_ui_prop("TextViewBuffer", "TextFile");

    newf = ui_text_value(infile);

    if (newf == NULL || newf == "")
        return 0;

    if (! file_exists(newf))
    {
        print_error("File `" + newf + "' does not exist.");
        return 0;
    }

    if (read_file_access(newf))
    {
        fcontents = read_file(newf);
        ui_text_set_value(viewer, fcontents);
        ui_text_set_readonly(viewer, True);
    }
    else
    {
        print_error("File `" + newf + "' is not readable.");
        return 0;
    }
    return 0;
}

int
TextViewerLoad_cb(ui_event_tp event, ui_prop theprop, int id, string value)
{
    LoadAndShowTextFile();
    return 0;
}
// ECR 3651 end



boolean
ok_identifier(string name)

{
    string newname;

    newname = string_translate(name, BAD_C_IDENTIFIERS_TRANSLATE_TABLE, GOOD_C_IDENTIFIERS_TRANSLATE_TABLE);

    if (newname != name)
    {
        print_error("Please select an identifier.");
        return False;
    }
    return True;
}


int
TextViewerBrowseType_cb(ui_event_tp event, ui_prop theprop, int id, string value)
 
{
    string  name;
    list nrs;
    ui_prop viewer;

    viewer = to_ui_prop("TextViewBuffer", "TheTextViewBuffer");
    name = ui_text_get_selection(viewer);

    if (str_no_val(name))
    {
        print_error("Select a type prior to browsing.");
        return 0;
    }

    if (! ok_identifier(name))
        return 0;

    nrs = list_select("node_ref[node[(Sequence | Selection | Enumeration | Typedef) && name = '${name}']]");

    if (nrs == NULL || list_count(nrs) == 0)
    {
        name = strip_prefix(strip_suffix(to_reveng_name(name)));
        nrs = list_select("node_ref[node[(Sequence | Selection | Enumeration | Typedef) && name = '${name}']]");
        if (nrs == NULL || list_count(nrs) == 0)
        {
            print_error("Type " + name + " not found.");
            return 0;
        }
    }

    navigate_to_editor(nrs);
    return 0;
}


int
TextViewerBrowseModule_cb(ui_event_tp event, ui_prop theprop, int id, string value)
 
{
    string name;
    list nrs;
    ui_prop viewer;

    viewer = to_ui_prop("TextViewBuffer", "TheTextViewBuffer");
    name = ui_text_get_selection(viewer);

    if (str_no_val(name))
    {
        print_error("Select a module prior to browsing.");
        return 0;
    }

    if (! ok_identifier(name))
        return 0;

    nrs = list_select("node_ref[node[Module && name = '${name}']]");

    if (nrs == NULL || list_count(nrs) == 0)
    {
        // try looking for it as a reveng name
        name = to_reveng_name(name);
        nrs = list_select("node_ref[node[Module && name = '${name}']]");
        if (nrs == NULL || list_count(nrs) == 0)
        {
            print_error("Module " + name + " not found.");
            return 0;
        }
    }

    navigate_to_editor(nrs);
    return 0;
}


// this stuff copied from omt/qrl/cxx/cxx.inc. Please change it if need be.
const string    BAD_CXX_OPERATION_IDENTIFERS_TRANSLATE_TABLE = "@#$\\{}?`;:'\"\n\t ";
const string    GOOD_CXX_OPERATION_IDENTIFERS_TRANSLATE_TABLE = string_repeat("_", string_length(BAD_CXX_OPERATION_IDENTIFERS_TRANSLATE_TABLE));
 
string
nav_operation_identifier(string s)
{
    return string_translate(s, BAD_CXX_OPERATION_IDENTIFERS_TRANSLATE_TABLE, GOOD_CXX_OPERATION_IDENTIFERS_TRANSLATE_TABLE);
}


string
get_class_members_classname(node n)
{
    node        sn;
    string      scname = NULL;

    if (n.scope_node_id != 0)
    {
        sn = node_get_scope_node(n);
        if (sn != NULL)
        {
            scname = sn.name;
        }
    }
    return scname;
}


node
get_states_scope_class(node n)
{
    node sn;
    while (n.scope_node_id != 0)
    {
        sn = node_get_scope_node(n);
        if (sn == NULL)
            break;
        n = sn;
    }
    if (n.scope_node_id == 0)
      return n;
    else
      return NULL;
}


// navigate to the source code by using the smdb
//
list
smdb_navigation(string class_name, string member_name, string type_arg)
{
    string cmd_string, tf;
    int system_return;
    if ( class_name == NULL && member_name == NULL )
      return NULL;

    if ( !is_smdb_available() )
      {
        navigate_to_source_without_smdb( class_name, member_name );
        return NULL;
      }
      
    cmd_string = "xrb_search -forward_messages " + editor_get_msgd_handle() +
        " -p " + current_projdir() + " -s " + current_system();

    if ( class_name != NULL && class_name != "" )
      if ( member_name != NULL && member_name != "" )
        cmd_string = cmd_string + " -name '" + member_name + "' -scope '" + class_name + "'";
      else
        cmd_string = cmd_string + " -name '" + class_name + "'";
    else
      cmd_string = cmd_string + " -name '" + member_name + "'";

    cmd_string = cmd_string + " -def -keep_trying";

    cmd_string = cmd_string + " " + type_arg;

    tf = temp_file("flist", "oo");
    cmd_string = cmd_string + " -of " + tf;

    system_return = system(cmd_string);
    if ( system_return == 0 )
        execute_chooser(tf, NULL, 0);

    delete_file( tf );
    // ECR 6723 ask the user to provide a directory if the class is not in smdb
// ECR 7665 begin
//    if ( system_return != 0 )
//        navigate_to_source_without_smdb( class_name, member_name );
    if ( system_return != 0 )
    {
        int pos = str_find_last(class_name, 0, ".");
        if (pos != -1)
        {
            // Construct search command using the last part of fully qualified class name
            //
            string lastpart_of_classname = string_extract(class_name, pos + 1, string_length(class_name) - pos - 1);
            cmd_string = "xrb_search -forward_messages " + editor_get_msgd_handle() +
                      " -p " + current_projdir() + " -s " + current_system();

            if ( lastpart_of_classname != NULL && lastpart_of_classname != "" )
                if ( member_name != NULL && member_name != "" )
                    cmd_string = cmd_string + " -name '" + member_name + "' -scope '" + lastpart_of_classname + "'";
                else
                    cmd_string = cmd_string + " -name '" + lastpart_of_classname + "'";
            else
                cmd_string = cmd_string + " -name '" + member_name + "'";

            cmd_string = cmd_string + " -def -keep_trying";

            cmd_string = cmd_string + " " + type_arg;

            tf = temp_file("flist", "oo");
            cmd_string = cmd_string + " -of " + tf;

            system_return = system(cmd_string);
            if ( system_return == 0 )
                execute_chooser(tf, NULL, 0);

            delete_file( tf );

            if (system_return == 0)
                return NULL;
        }

        // search without SMDB using fully qualified class name
        navigate_to_source_without_smdb( class_name, member_name );
    }
// ECR 7665 end
    return NULL;
}


string __held_navigation_class = NULL;
string __held_navigation_member = NULL;

int
navigate_to_source_without_smdb( string classname, string membername )
{
  // if there is a stored directory for where the code lives, then load 
  // it and try searching for it
    string filename = path_compose( current_projdir(),
                                    current_system(), "revc_files",
                                    SC_DIR_FILE );

    string dir;

    if ( classname == "" )
      {
        classname = __held_navigation_class;
        membername = __held_navigation_member;
      }
    else
      {
        __held_navigation_class = classname;
        __held_navigation_member = membername;
      }

    if ( !file_exists( filename ))
      {
        string guess_dir = path_compose( current_projdir(),
                                         current_system(), "src_files" );
        PropSheet__show( "SourceCodeDirectoryDialogue" );
        ui_text_set_value( to_ui_prop("SourceCodeDirectoryDialogue",
                                      "SCDir"), guess_dir );
        ui_set_value( to_ui_prop("SourceCodeDirectoryDialogue",
                                 "Status" ), "" );
        // this gets called again when the property sheet is filled in
        return 0;
      }

        // ECR6659
// ECR 6662 begin
    //dir = string_substitute(read_file( filename ));
    dir = remove_multiple_backslashes_from_path(path_normalize(string_substitute(read_file( filename ))));
// ECR 6662 end
        
    // now try searching, and if it fails, then pop up the sheet again
    string cmd = "nav_source -forward_messages " + editor_get_msgd_handle() +
                 " -p " + current_projdir() +
                 " -s " + current_system() +
                 " -d " + dir +
                 " -class " + classname;
    if ( membername != NULL && membername != "" )
      cmd = cmd + " -name '" + membername + "' -t member";
    else
      cmd = cmd + " -t class";

    string tempfile = temp_file("flist", "oo");
    int rc = system( cmd + " -of " + tempfile );
    if ( rc == 0 )
      {
        execute_chooser( tempfile, NULL, 0 );
        delete_file( tempfile );
        return 0;
      }

    delete_file( tempfile );

// ECR 7665 begin
    int pos = str_find_last(classname, 0, ".");
    if (pos != -1)
    {
        // Do search using last part of fully qualified class name
        string lastpart_of_classname = string_extract(classname, pos + 1, string_length(classname) - pos - 1);
        return navigate_to_source_without_smdb( lastpart_of_classname, membername );
    }
// ECR 7665 end

    // if the search failed, then pop up the directory
    // chooser and allow them to select the dir where the source code lives
    PropSheet__show( "SourceCodeDirectoryDialogue" );
    ui_text_set_value( to_ui_prop("SourceCodeDirectoryDialogue",
                                  "SCDir"), dir );
    ui_set_value( to_ui_prop("SourceCodeDirectoryDialogue",
                             "Status" ), "Search Failed - Try a different directory or specify a file?" );
    print_error( "Search Failed - Try a different directory or specify a file" );

    return 0;
}

// the callback from that will save its contents, and then call navigate
// to source without smdb - only hassle is remembering the names of the
// classname and the attribute we started the search on.

// if they hit cancel then it is all over

// need to define the dir for the source code property sheet, and add the
// callbacks into this bit here.


// take a look in oo/rules/qrl/objectada_nav.inc

const string SC_DIR_FILE = ".source_code_hint";

// a directory has been entered and OK has been hit
int
SourceCodeDir_cb(ui_event_tp e, ui_prop p, int id, string v)
{
    string dir = string_strip(ui_get_value(
                       to_ui_prop("SourceCodeDirectoryDialogue", "SCDir")),
                            "B", " ");

    if ( string_length( dir ) == 0 ||
         !read_file_access( dir ))
      {
        ui_set_value( to_ui_prop("SourceCodeDirectoryDialogue", "Status" ),
                      "Enter a proper directory, or 'Cancel'" );
        return 0;
      }
    else
      ui_set_value( to_ui_prop("SourceCodeDirectoryDialogue", "Status" ), "" );

    // so, save the value
    string filename = path_compose( current_projdir(),
                                    current_system(), "revc_files",
                                    SC_DIR_FILE );
    if ( !file_exists( filename ) || write_file_access( filename ))
      {
          // ECR6659
          string system_path = string_substitute("${projdir}/${system}");
          system_path = string_search_and_replace(system_path, "\\", "/");
          system_path = string_search_and_replace(system_path, "//", "/");
          dir = string_search_and_replace(dir, "\\", "/");
          dir = string_search_and_replace(dir, system_path, "${projdir}/${system}");
          // ECR6659-end

        write_file( filename, dir );
        // now run the search
        navigate_to_source_without_smdb( "", "" );
        return 1;
      }
    else
      {
        print_error( "Could not write the '" + 
                   filename + "' file." );
        return 0;
      }
}


// ECR 3651a begin
int select_source_code_dir_callback(ui_event_tp event, ui_prop theprop, int id, string value)
{
    string str;

    ui_owner uiOwner = to_ui_owner(theprop);
    str = ui_get_value(to_ui_prop(uiOwner, "SCDir"));

    str = editor_show_dirchooser("Select Source Code Directory", str);

    if (str != NULL && string_length(str) != 0)
    {
        str = path_normalize(str);
        ui_set_value(to_ui_prop(uiOwner, "SCDir"), str);
    }

    return 0;
}

int select_src_file_callback(ui_event_tp event, ui_prop theprop, int id, string value)
{
    string str;
    ui_owner uiOwner;

    uiOwner = to_ui_owner(theprop);
    str = ui_get_value(to_ui_prop(uiOwner, "TextFile"));
    
    str = editor_show_filechooser("Open File", str, "All Files (*.*)|*");

    if (str != NULL && string_length(str) != 0)
    {
        str = path_normalize(str);
        ui_set_value(to_ui_prop(uiOwner, "TextFile"), str);
        LoadAndShowTextFile();
    }

    return 0;
}
// ECR 3651a end

