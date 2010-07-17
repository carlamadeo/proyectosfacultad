//
//      sccsid[] = %W% %Y% %D%
//      StP/Core
//      Confidential property of Interactive Development Environments, Inc.
//      Copyright (c) 1992-1994
//      All rights reserved
//


#include_if_exists "user/ct/rules/qrl/gather_comment.inc"


/*

This file holds information about writing customized comment gathering
functions for use with the StP Reverse Engineering tool.

There is no need to modify this file. Whatever you decide to write, you
should put into the "user/ct/rules/qrl/gather_comment.inc" file - it
is automatically included by this file.

You can provide any of five customized functions that will be called
during the comment gathering process. Here are the prototypes of the
custom functions you may add:

string smdb_gather_class_comment( string classname, int file_id, int line );

string smdb_gather_attribute_comment( string attrname, string classname,
                                      int file_id, int line );

string smdb_gather_operation_comment( string opname, string classname,
                                      int decl_file_id, int decl_line,
                                      int defn_file_id, int defn_line );

string smdb_gather_function_comment( string fnname, int file_id, int line );

string smdb_gather_identifier_comment( string idename, int file_id,
                                       int line );

(SMDB stands for Semantic Model Database, and is where all Reverse
 Engineering data is stored)

Simple examples of these five functions appear later in this document.

If you want to customize the way that operation comments are extracted,
you write a QRL function called "smdb_gather_operation_comment", making
sure the arguments match those of the function above, and it will be
called for each operation in the source code. The function returns the
string of the comment that it wants to be stored.


  Notes
  -----

 For smdb_gather_operation_comment, the decl_file_id and decl_line are the
 positions in the class definition. If the operation also has a 
 definition of a function body (even an inline one on the same line) then
 the defn_file_id and defn_line will also be filled in - otherwise these
 will be zeroes.

 smdb_gather_function_comment (if defined) will be called for every function
 that has a body defined - this includes global functions and class members,
 it is called with the file and line of the definition - the place where the
 body is defined.

 smdb_gather_identifier_comment is only called by StP/SE. It is passed in the
 definition location of global identifiers.


Builtin functions for writing custom comment extraction
-------------------------------------------------------

  string smdb_get_file_name( file_id );

   returns the name of the source file, for the id passed in. This is
   useful if you want to find out what the name of a file is, perhaps
   to implement a different comment gathering strategy for the file.

 string smdb_get_file_path( file_id );

   returns the complete path (directory and filename combined) for the
   file with the given id. This could be used if you wished to write 
   a very complicated custom gatherer that needed to bring in the source
   file text into qrl (for example, you may implement a different comment
   strategy for each author, so you could read in the file (using
   read_file() on the file path returned from this function) then search
   for 'Author:' then extract the name, and use that to lookup the
   strategy you wish to use.

 int smdb_move_to_first_comment( int file_id, int line = 0, int col = 0 );

   moves the internal database cursor to the first comment for a given
   file. If you also specify a line, then it will position itself at the
   first comment on or after the specified line. You can also specify a
   column, in which case it will position itself on or after the comment
   at the specified column, on the line, in the file specified.

   smdb_move_to_first_comment() returns 0 if the move was successful, and
   -1 if it failed. The reason for it failing is usually that there is no
   comment to move to because you have moved off the end of the table.

 smdb_comment smdb_get_next_comment(void);

   Returns an smdb_comment structure which looks like this:

   struct smdb_comment
   {
       int file_id;
       int line;
       int column;
       string text;
   };

   The next comment is created as one of these structures and returned.
   If there are no more comments, then you will receive NULL.

   Notice that the smdb_get_next_comment will move from one file to the
   next at the end of the first one. So you must supply checks to make
   sure that you have not returned a comment structure pertaining to
   the next file in the SMDB. A good example of how to do this is shown
   in the get_comment_between() function later in this file.
   
 smdb_comment smdb_get_prev_comment();

   This is a complementary function to smdb_get_next_comment(), except this
   will extract the comment before the current position, and move the 
   cursor backwards.

   Here are some trivial examples of the five routines you can supply
   to perform custom comment gathering. Each calls the get_comment_between()
   function which is also shown here. To actually use that function
   in your functions you would have to uncomment it and copy it into the
   user/ct/rules/qrl/gather_comment.inc file.



// a general purpose routine that extracts all the comments between
// a range of lines in a file
//
string get_comment_between( int file_id, int lower, int upper )
{
  // move to the first comment on or after the line in the file
  if ( smdb_move_to_first_comment( file_id, lower ) == 0 )
    {
      smdb_comment next_comm;
      string result = "";

      // while we haven't reached the end of the comment table
      while( (next_comm = smdb_get_next_comment() ) != NULL )
	{
	  // if we have gone into the next file, or if the comment we
	  // have comes from beyond the upper line boundary, then break
	  if ( next_comm.file_id != file_id ||
	       next_comm.line > upper )
	    break;
	  else
	    result = result + next_comm.text;
	}
      return result;
    }
  return "";
}


// this is a sample function that scans through all the comments in a
// source file, printing them out in reverse order
//
void demo_scan_comments_backwards( int file_id )
{
  // by providing a falsely high line number parameter we can move to
  // the end of the comments for this file
  if ( smdb_move_to_first_comment( file_id, 99999 ) == 0 )
    {
      smdb_comment prev_comm;

      // we keep getting the previous comment until there are no more...
      while( (prev_comm = smdb_get_prev_comment() ) != NULL )
	{
	  // or until we run out of comments for this file
	  if ( prev_comm.file_id != file_id )
	    break;
	  print_message( "Comment before is '" + prev_comm.text + "'." );
	}
    }
  else
    print_message( "Failed to move to end of file." );
}


// Here are the sample override functions you can supply...


string smdb_gather_class_comment( string classname, int file_id, int line )
{
    print_message( "Overriding class comment gathering" );
    print_message( "Parms = " + classname + " " + file_id + " " + line );
    print_message( "The file is " + smdb_get_file_name( file_id ));
    print_message( "The full path is " + smdb_get_file_path( file_id ));
    return get_comment_between( file_id, line - 2, line );
}

string smdb_gather_attribute_comment( string attrname, string classname,
                                      int file_id, int line )
{
    print_message( "Overriding attribute comment gathering" );
    print_message( "Parms = " + classname + "::" + attrname + " " +
	       file_id + " " + line );
    return get_comment_between( file_id, line, line + 1 );
}

string smdb_gather_operation_comment( string opname, string classname,
                                      int decl_file_id, int decl_line,
                                      int defn_file_id, int defn_line )
{
    print_message( "Overriding operation comment gathering" );
    print_message( "Parms = " + classname + "::" + opname + " " +
	       decl_file_id + " " + decl_line + " " +
	       defn_file_id + " " + defn_line );
    return get_comment_between( decl_file_id, decl_line - 1, decl_line );
}

string smdb_gather_function_comment( string fnname, int file_id, int line )
{
    print_message( "Overriding function comment gathering" );
    print_message( "Parms = " + fnname + " " + file_id + " " + line );
    return get_comment_between( file_id, line - 4, line );
}

string smdb_gather_identifier_comment( string idename, int file_id,
                                       int line )
{
    print_message( "Overriding identifier comment gathering" );
    print_message( "Parms = " + idename + " " + file_id + " " + line );
    return get_comment_between( file_id, line, line );
}

*/
