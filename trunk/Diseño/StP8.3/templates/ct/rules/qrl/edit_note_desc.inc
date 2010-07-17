// New file for ECR 3789

// This is a helper function for building temp-file names
// in the NoteDescription mechanism.
// We want this, because some customers want to have more
// meaningful filenames for the tempfiles, so they can
// see from the document title in Winword etc. what note
// they are editing. However, since temp_file() is more
// safe (we never know what characters might be in an
// object name), we enable that feature only with a switch
//
//
string
ps_NoteDescription_temp_file( int objid )
{
    string tmp_file = temp_file( "note","" );
    string oname;
    if( NULL != toolinfo_variable("NoteDescFancyTempFile" ) )
    {
	tmp_file = path_part( tmp_file );

	// field_by_query is defined in ct.inc
	//
	oname = field_by_query("node[id="+objid+"]", "name", "XX" );
	if( oname == "//" )
	    oname = field_by_query("link[id="+objid+"]", "name", "XX" );
	
	// oname might contain characters that are invalid for 
	// filenames
	//
	oname = string_translate( oname, " ", "_" );
	tmp_file = path_compose( tmp_file, oname+".tmp" );
    }
    return tmp_file;
}

int
ps_EOD_logic( int objid, string objectType, string note_name )
{	
    // ECR 6335
    // Now let's see if we have a file lock.
    //
    file_lock lock_check_file_lock = find_by_query( "file_lock[file[ObjectAnnotation & contains_notes[obj_id="+objid+"]]]" );
    if( NULL!= lock_check_file_lock )
    {
        if( ! editor_confirm( "This object's description is locked by "+lock_check_file_lock.user+".\nAny changes in the description would not be saved, continue ?","OK","Cancel" ))
        {
            return 0;
        }
    }
    // end ECR 6335
    string tmp_file = ps_NoteDescription_temp_file( objid ); 
    note n = find_by_query( "note[${note_name} && obj_id=${objid}]" );


    if( n != NULL && n.desc != NULL && n.desc != "" )
    {
	write_file( tmp_file, n.desc );
    }
    else
    {

	write_file( tmp_file, GetDefaultDescription( objid, objectType, 
						     note_name ) );
    }
    if( NULL == toolinfo_variable("NoteDescEditor" ) )
    {
	ui_prop_sheet_pop_up( "UmlObjectDescriptionSheet" );
	fill_objectdescription_ps( "", NULL, 0, tmp_file, objid, note_name );
	//gde_builtin("EditorActionDone 0214951139  ObjectDescriptionPS_deactivate();");
	ObjectDescriptionPS_activate( True );

    }
    else
    {

	ps_edit_note_desc( tmp_file );

	if( ps_NoteDescription_cb_debug )
	    //message( "New Desc":+read_file( tmp_file ) ); // ECR 8008 - moved ':' inside string
	    message( "New Desc:"+read_file( tmp_file ) ); 
	
	// For efficency check if the text has changed, so we do only
	// bother the (ng)oae if we really have to
	
	string newtext = read_file( tmp_file );
	

	if( n == NULL || newtext != n.desc )
	{
	    string stmp_file = temp_file("not","xx" );
	    string tf = temp_file( to_string(objid), 
				   to_string( ps_NoteDescriptionExternal ) );
			
	    write_file( stmp_file,
			to_string(objid) +":"+note_name+":NULL:NULL:"+tf+"\n" );

	    write_file( tf, newtext );
			
	    int handle = editor_send_msg( NGOAE_CLASS,
					  "EditorQrlEval SetAnnotValuesFromFiles(\""+
					  string_escape(stmp_file,"\\")+"\");", 
					  -1 );
			
	    editor_send_msg( NGOAE_CLASS,
			     "EditorQrlEval 	CleanAnnotTmpFiles(\""+
			     string_escape(stmp_file,"\\")+"\");", 
			     handle );


	}
	else
	{
	    if( ps_NoteDescription_cb_debug )
		message( "No Change" );
	}
    }
    delete_file( tmp_file );
    

    return 0;

}

// This calls the external editor for the note description mechanism
//
void
ps_edit_note_desc( string f )
{
	string exteditcmd = "wordnoteedit.exe -html ";
	
	if( NULL != toolinfo_variable("NoteDescEditor" ) )
		exteditcmd = toolinfo_variable( "NoteDescEditor" );
		
	if( 1 || debug_edit_nd_shortcut )
		message( "Calling "+exteditcmd+" "+f );
	system( exteditcmd + " " + f );
}

void
ps_edit_note_desc_with_cmd( string f, string cmd )
{
	system( cmd+" "+f );
}

