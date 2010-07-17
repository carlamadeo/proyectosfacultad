//
//      sccsid[] = %W% %Y% %D%
//      StP/SE
//      Confidential property of Interactive Development Environments, Inc.
//      Copyright (c) 1995
//      All rights reserved
//

// Routine to navigate to source code.  Uses parser information

#include "qrl/properties/c_browse_cb.inc"


struct se_browse_detail
    {
	string name;
	string file;
	string dir;
	int id;
    };


se_browse_detail
SEBrowseGetNode(int id)
{
    se_browse_detail result;
    result.name = NULL;
    result.file = NULL;
    result.dir = NULL;
    node n = find_by_query( "node[id==" + id + "]" );
    if ( n )
    {
	result.name = to_underscore_name(n.name);
	result.id = id;
	if ( n.scope_node_id )
	{
	    node f = find_by_query( "node[id==" + n.scope_node_id + "]" );
	    if ( f )
	    {
		result.file = f.name;
		if ( f.scope_node_id )
		{
		    node d = find_by_query( "node[id==" + f.scope_node_id +
					    "]" );
		    if ( d )
			result.dir = d.name;
		}
	    }
	}
    }
    return result;
}



list
SEBrowseToFunctionDef(int id)

{
    se_browse_detail sbd = SEBrowseGetNode( id );
    return do_the_browse(sbd.name, " -def", " -T func", sbd.file, sbd.dir, sbd.id);
}



list
SEBrowseToFunctionRef(int id)

{
    se_browse_detail sbd = SEBrowseGetNode( id );
    return do_the_browse(sbd.name, " -ref", " -T func", NULL, NULL, sbd.id);
}

list
SEBrowseToGlobDef(int id)

{
    se_browse_detail sbd = SEBrowseGetNode( id );
    return do_the_browse(sbd.name, " -def", " -T glob", sbd.file, sbd.dir, sbd.id);
}



list
SEBrowseToGlobRef(int id)

{
    se_browse_detail sbd = SEBrowseGetNode( id );
    return do_the_browse(sbd.name, " -ref", " -T glob", NULL, NULL, sbd.id);
}


list
SEBrowseToLibRef(int id)

{
    se_browse_detail sbd = SEBrowseGetNode( id );
    return do_the_browse(sbd.name, " -ref", " -T lib", NULL, NULL, sbd.id);
}

list
SEBrowseToTypedefDef(int id)

{
    se_browse_detail sbd = SEBrowseGetNode( id );
    return do_the_browse(sbd.name, " -def", " -T typedef", sbd.file, sbd.dir, sbd.id);
}

list
SEBrowseToDefDef(int id)

{
    string xrb_query_type = " -T datas";
    se_browse_detail sbd = SEBrowseGetNode( id );
    // if this is a leaf node, then search in the field table in xrb_search
    // we can spot leaf nodes because they have a type annotation, or they
    // have in links and no outlinks
    if ( find_by_query( "node[ id=" + id + " & in_links ]" ) != NULL &&
	 find_by_query( "item[ DataType & obj_id=" + id + " ]" ) != NULL )
      xrb_query_type = " -T datam";
    return do_the_browse(sbd.name, " -def", xrb_query_type, sbd.file,
			 sbd.dir, sbd.id);
}


boolean ComingFromFcFile = False;

list
SEBrowseToFcCode(string fname)

{
    file f;
    f = find_by_query("file[FceDiagram && name = '${fname}']");

    if (f == NULL)
	return list_create("node_ref", 0);

    ComingFromFcFile = True;
    return do_the_browse(fname, " -def", " -T func", NULL, NULL, f.id);
}



list
do_the_browse(string name, string defref, string type, string fname, string dname, int id)

{
    string cmdstr, tf;
    int system_return;
    int x, y;
    gde_node gn;
 
    cmdstr = "xrb_search -forward_messages " + editor_get_msgd_handle() +
	" -p " + current_projdir() + " -s " + current_system();

    cmdstr = cmdstr + " -name '" + name + "'";
    cmdstr = cmdstr + type;
    cmdstr = cmdstr + " -read -write";
    cmdstr = cmdstr + defref;
    cmdstr = cmdstr + " -cs";

    tf = temp_file("flist", "se");

// ECR 6895 begin
    //cmdstr = cmdstr + " > " + tf;
    cmdstr = cmdstr + " -of " + tf;
// ECR 6895 end

    system_return = system(cmdstr);
    if ( system_return == 0 )
	execute_chooser(tf, NULL, 0);
    else
    {
	// system return == 100 if license checkout failed
	// 7 if search worked OK but nothing was found
	// 8 if there is no semantic model
	// 9 if the SM is locked
	// other value = general failure
	if ( system_return == 9 )
	    print_message( "Navigation failed - Semantic Model locked" );
	else if ( fname != NULL && fname != "" )
	{
	    // see if we can navigate by searching directly through the
	    // source code using the nav_source program

// ECR 6839 begin
//	    cmdstr = "nav_source -name '" + name + "' " + type;
        cmdstr = "nav_source -forward_messages " + editor_get_msgd_handle();
  	    cmdstr = cmdstr + " -name '" + name + "' " + type;
// ECR 6839 end

	    cmdstr = cmdstr + " -f " + fname;
	    if ( dname != NULL && dname != "" ) 
            {
                // ECR 7193 - commented out next line - what was purpose of it???
                // dname = path_compose(current_projdir(),current_system(),"src_files");
		cmdstr = cmdstr + " -d " + dname;
            }

// ECR 6895 begin
	    //cmdstr = cmdstr + " > " + tf;
	    cmdstr = cmdstr + " -of " + tf;
// ECR 6895 end

	    system_return = system(cmdstr);
	    if ( system_return == 0 )
            {
		execute_chooser(tf, NULL, 0);
// comment out for now - possible new feature
//                delete_file(tf);     // young
//		return 0;           // young
            }
	    else 
            {
		print_message( "Navigation failed" );
// prototype new feature-comment out for now
        //        PropSheet__show( "SE_SourceCodeDirectoryDialog");
        //       ui_text_set_value( to_ui_prop("SE_SourceCodeDirectoryDialogue", "SCDir"), dname );
        //        ui_set_value( to_ui_prop("SE_SourceCodeDirectoryDialogue", "Status" ), 
        //                     "Search Failed - Try a different directory or specify a file?" );
         //       print_error( "Search Failed - Try a different directory or specify a file" );
            }

	}
	else if ( system_return == 100 )
	    print_message( "License checkout failed for C Source Browser" );
    }

    delete_file(tf);

    if (ComingFromFcFile)
    {
	ComingFromFcFile = False;
	return list_select("node_ref[node[SEStartState] && file_id = ${id}]");
    }

    return NULL; 
}



// possible new feature - hasn't implemented yet
// a directory has been entered and OK has been hit
int
SE_SourceCodeDir_cb(ui_event_tp e, ui_prop p, int id, string v)
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
				    current_system(), "revc_files");
    if ( !file_exists( filename ) || write_file_access( filename ))
      {
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

