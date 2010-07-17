////////////////////////////////////////////////////////////////////////////////
// DESCRIPTION: ALL_PrintTable.inc
//              ==============================
//              (c) 1997 by  S. Noack, Aonix
//
// Scope: Global
//        (packages/global/printing) 
//
////////////////////////////////////////////////////////////////////////////////
//
// Global Include-File with the functions for optimized table printing, which
// include some table style options and the limitation to the columns and rows,
// given as Arguments. Print settings are evaluated and override the standard
// options defined below.
// Captions are printed as a picture paragraph below the table.
//
////////////////////////////////////////////////////////////////////////////////
//
// Call: PrintTable (file    TABLE_FILE,
//                   string  P_SETTING,
//                   string  TAB_COLS,
//                   string  TAB_ROWS,
//                   string  CELLHEAD,
//                   string  CELLBODY,
//                   string  CAPTION,
//                   string  TABLE_CAPTION,
//                   string  REP_TABLE,
//                   boolean VERBOSE)
//
// Args: - TABLE_FILE:     The StP table file to be printed. 
//       - P_SETTING:      If a user defined print setting exists, it
//                         can be used as argument here, otherwise 
//                         P_SETTING should be set to "". 
//       - TAB_COLS:       Contains the table columns to be printed,  
//                         default is "" (see QRL Manual, Page 5-43)
//       - TAB_ROWS:       Contains the table rows to be printed,  
//                         default is "2-*" (see QRL Manual, Page 5-43)
//       - TAB_H_ROWS:     Contains the table header rows to be printed,  
//                         default is "1" (see QRL Manual, Page 5-43)
//       - CELLHEAD:       The paragraph format for table header cells  
//                         (should be globaly defined in XX_Formats.inc)
//       - CELLBODY:       The paragraph format for table body cells  
//                         (should be globaly defined in XX_Formats.inc)
//       - CAPTION:        The caption paragraph format (should be 
//                         globaly defined in XX_Formats.inc)
//       - TABLE_CAPTION:  Predefined Part of the table caption string
//                         (might be a global that is set to different  
//                         values depending on the chosen reporting
//                         language in XX_Reportstrings.inc)
//       - REP_TABLE:      Message text (might be a global that is set to
//                         different values depending on the chosen user
//                         interface language in XX_Messagestrings.inc)
//       - VERBOSE:        Report on the currently printed table
//
////////////////////////////////////////////////////////////////////////////////
//                
// Globals:   - const string STD_TAB_H_ROWS = "1";
//            - const string STD_TAB_ROWS = "2-*";
//            - const string STD_TAB_COLS = "";
//            - const float   TAB_W_STD
//            -       boolean NEEDS_TABLE_PRINT_SETTING
////////////////////////////////////////////////////////////////////////////////
//
// Functions: - void      PrintTable(file TABLE_FILE, .....)
////////////////////////////////////////////////////////////////////////////////
//
// uses externaly defined globals: <type name (package)>
////////////////////////////////////////////////////////////////////////////////
//
// uses externaly defined functions: <type name, package>
////////////////////////////////////////////////////////////////////////////////

// Standard Table Row and Column Values
const string STD_TAB_H_ROWS = "1";
const string STD_TAB_ROWS   = "2-*";
const string STD_TAB_COLS   = "";


// Standard Print Units
const float  TAB_W_STD = 6.8;     // max. table-width is 6.8 Inches or 17.0 cm


// Need to call Default Print Settings ?

boolean NEEDS_TABLE_PRINT_SETTING = True;


///////////////////////////////////////////////////////////////////////////////
// PrintTable: Sets the default table print setting if needed (once). 
// 			   Prints the table with the given current settings for header,
// 			   rows and columns 
//-----------------------------------------------------------------------------
// Args:	   - see description in file header
//-----------------------------------------------------------------------------
// Return:	   void
///////////////////////////////////////////////////////////////////////////////
void PrintTable   (file    TABLE_FILE, 
                   string  P_SETTING,
                   string  TAB_COLS,
                   string  TAB_ROWS,
                   string  TAB_H_ROWS,
                   string  CELLHEAD,
                   string  CELLBODY,
                   string  CAPTION,
				   string  TABLE_CAPTION,
                   string  REP_TABLE,
                   boolean VERBOSE)
{
   string caption;

   //-----------------------------------------------------------
   // Call Print Setting function, if neccessary
   //-----------------------------------------------------------
   if (NEEDS_TABLE_PRINT_SETTING)
   {
      //--------------------------------------------------------
      // No caption is printed, we do that ourself  
      //--------------------------------------------------------
      table_show_caption_set(False);     

      //--------------------------------------------------------
      // Index Rows and Columns to be printed  
      //--------------------------------------------------------
      table_print_column_indices_set(False);
      table_print_row_indices_set(False);

      //--------------------------------------------------------
      // Paragraph formats for table cells (see xx_Formats.inc)  
      //--------------------------------------------------------
      table_cell_paragraph_format_set(CELLBODY);
      table_cell_bold_paragraph_format_set(CELLHEAD);

      //--------------------------------------------------------
      // table shades  
      //--------------------------------------------------------
      table_shade1_pattern_set(5);
      table_shade2_pattern_set(6);

      //--------------------------------------------------------
      // Print units are Inches, size fits USLetter and A4)  
      //--------------------------------------------------------
      file_print_units_set(Inches);
      table_width_set(TAB_W_STD);    // ok for A4 and US-Letter 

      NEEDS_TABLE_PRINT_SETTING = False;
   }
	      
   //----------------------------------------------
   // Row and Column values change for each table  
   //----------------------------------------------
   table_column_range_set(TAB_COLS);
   table_row_range_set(TAB_ROWS);
   table_header_row_range_set(TAB_H_ROWS);

   //----------------------------------------------
   // Print the table (with some distance)
   //----------------------------------------------
   paragraph(BODY_NORMAL);
   file_print(TABLE_FILE, "", P_SETTING);

   //-----------------------------------------------------------------
   // Print the caption
   // We want the caption printed below the table, using a predefined
   // running number, which is not possible in Interleaf.
   // That's why we print it ourself
   //-----------------------------------------------------------------
   caption = TABLE_CAPTION + TABLE_FILE.type + " " + TABLE_FILE.name;
   paragraph(CAPTION);
   print(caption);

   if (VERBOSE)
      message(REP_TABLE + TABLE_FILE.name);

   return;
}
