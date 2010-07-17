//
//      sccsid[] = %W% %Y% %D%
//      StP/Booch
//      StP/OMT
//      StP/OO
//      StP Core
//      Confidential property of Interactive Development Environments, Inc.
//      Copyright (c) 1992-1995
//      All rights reserved
//


// functions to handle the navigations between ObjectAda and StP and
// vice-versa

// bring in naming conventions
#include "qrl/code_gen/ada_95/customize.inc"

const string OA_REGISTRATION_FILE = ".object_ada";
const string    OBJECTADA_CLASS = "stpoad";

string held_oa_parameter = NULL;


// always call this with zero as the second parameter - it is only
// called with a different value when it is called from the property
// sheet callback function
void
nav_to_ada( string class_name, int internal_name , node node)
{
    string oa_cntx, msg;

    // this gets set if we are entering the function from the
    // property sheet (indirect recursion, as we create the prop
    // sheet here)
    if (internal_name)
      {
	oa_cntx = class_name;
	class_name = held_oa_parameter;
      }
    else
      oa_cntx = get_oa_cntx();

    held_oa_parameter = NULL;

    // if we do not know where the ObjectAda system is, then we must
    // prompt for it.
    if ( oa_cntx == "" )
      {
	held_oa_parameter = class_name;

	PropSheet__show( "ObjectAdaDialogue" );
	ui_text_set_value( to_ui_prop("ObjectAdaDialogue", "OA_Context"),
			   current_projdir() + current_system());
	ui_set_value( to_ui_prop("ObjectAdaDialogue", "Status" ), "" );

	return;
      }

    // get ready to send the message
    if ( is_product_uml() )
    {
	msg = library_unit_name(node) + "!Unit!!";
    }
    else
        msg = CLASS_PACKAGE_NAME_PREFIX + class_name + CLASS_PACKAGE_NAME_SUFFIX + "!Unit!!";

    editor_send_msg(OBJECTADA_CLASS,
		    "Ada_Navigate " + editor_get_msgd_handle() + " " +
		    current_projdir() +
		    current_system() + " " + oa_cntx + " " + msg, 0);
}



// here we handle an incoming navigation message from ObjectAda.
// We have to handle things such as name mappings (stripping suffices
// and prefices from packages) and replying
void
nav_from_ada( int cmd_id, string from_dir, string name,
	      string type, string scope, string sig )
{
  string default_editor;  
  // If we are registered to a different context, or we are not yet
  // registered, then set it up
  if ( from_dir != get_oa_cntx())
    {
      set_oa_cntx( from_dir );
      print_message( "Registering ObjectAda context as '" + from_dir + "'." );
    }

  if ( is_product_uml() )
    default_editor = "uclassd";
  else if ( is_product_booch() )
    default_editor = "class";
  else
    default_editor = "ome";

  string classname = map_name_from_oa( name, type );

  if ( type == "Unit" )
    {
      // firstly try the modified name, if that fails then just try the
      // original name
      int ok = OMTFromProgrammingEnvironment( default_editor, classname, "" );
      if ( ok != 0 && classname != name )
	ok = OMTFromProgrammingEnvironment( default_editor, name, "" );
      if ( ok == 0 )
	editor_send_msg(OBJECTADA_CLASS,
			"OA_Reply " + cmd_id + " 0", 0 );
      else
	editor_send_msg(OBJECTADA_CLASS,
			"OA_Reply " + cmd_id + " 1", 0 );
    }
  else
    {
      print_error( "Cannot navigate to type '" + type + "'." );
      editor_send_msg(OBJECTADA_CLASS,
		      "OA_Reply " + cmd_id + " 1", 0 );
    }
  return;
}


// map the various names of things in the object ada source code into
// the corresponding item in the design
string
map_name_from_oa( string source, string type )
{
    string classname;
    if ( type == "Unit" )
      {
        classname = strip_nav_prefix( source, CLASS_PACKAGE_NAME_PREFIX );
	classname = strip_nav_suffix( classname, CLASS_PACKAGE_NAME_SUFFIX );
	return classname;
      }
    else
      return source;
}

// extract the name from the source by whipping off the prefix pattern if
// it is there
string
strip_nav_prefix( string source, string pattern )
{
    int l = string_length( pattern );
    if ( l == 0 )
      return source;
    if ( string_find( source, 0, pattern ) == 0 )
      return string_extract( source, l,
			     string_length( source ) - l );
    else
      return source;
}


// extract the name from the source by whipping off the suffix pattern if
// there is a match
string
strip_nav_suffix( string source, string pattern )
{
    int l = string_length( pattern );
    if ( l == 0 )
      return source;
    int t = string_length( source ) - l;
    if ( t > 0 && string_find( source, t, pattern ) == t )
      return string_extract( source, 0, t );
    else
      return source;	 
}

// find out the currently registered context of the ObjectAda session
// associated with this system
string
get_oa_cntx()
{
    string filename = path_compose(current_projdir(), current_system(),
				   OA_REGISTRATION_FILE);
    if ( file_exists( filename ))
      {
	if ( read_file_access( filename ))
	  {
	    return read_file( filename );
	  }
	else
	  {
	    print_error( "No read access to '" + 
		       filename + "'" );
	    print_error( "Make sure the file is read/writable by all." );
	  }
      }
    return "";
}


// store the ObjectAda dir for future navs from this system
int
set_oa_cntx( string dir )
{
    string filename = path_compose(current_projdir(),
				   current_system(),
				   OA_REGISTRATION_FILE);
    if ( !file_exists( filename ) || write_file_access( filename ))
      {
	write_file( filename, dir );
	return 0;
      }
    else
      {
	print_error( "Could not write the '" + 
		   filename + "' file." );
	print_error( "Registration to ObjectAda failed." );	
      }
    return -1;
}


// Callback for the objectada property sheet 
int
OA_Registration_cb(ui_event_tp e, ui_prop p, int id, string v)
{
    string ObjectAda_Context = string_strip(ui_get_value(
			  to_ui_prop("ObjectAdaDialogue", "OA_Context")),
				     "B", " ");

    if ( string_length( ObjectAda_Context ) == 0 ||
	 !read_file_access( ObjectAda_Context ))
      {
	ui_set_value( to_ui_prop("ObjectAdaDialogue", "Status" ),
		      "Enter a proper directory, or 'Cancel'" );
	return 0;
      }
    else
      ui_set_value( to_ui_prop("ObjectAdaDialogue", "Status" ), "" );

    set_oa_cntx( ObjectAda_Context );

    if ( ui_get_value( to_ui_prop("ObjectAdaDialogue", "StartOA" )) == 1 )
      {
	editor_send_msg(OBJECTADA_CLASS,
			"Ada_Start " + ObjectAda_Context + " " +
			current_projdir() + current_system(), 0 );
      }

    if ( held_oa_parameter != NULL )
      nav_to_ada( ObjectAda_Context, 1, NULL );
    return 1;
}


// routine to populate and display the objectada dialogue
void
ObjectAdaDialogue()
{
  PropSheet__show( "ObjectAdaDialogue" );
  string val = get_oa_cntx();
  if ( val == "" )
    val = current_projdir() + current_system();
  ui_text_set_value( to_ui_prop("ObjectAdaDialogue", "OA_Context"), val );
  ui_set_value( to_ui_prop("ObjectAdaDialogue", "Status" ), "" );
  held_oa_parameter = NULL;   
}
