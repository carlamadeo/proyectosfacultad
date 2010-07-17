///////////////////////////////////////////////////////////////////////////////
// DESCRIPTION: ALL_PrintDiagram.inc
//              ==============================
//              (c) 1997 by  S. Noack, Aonix
//
// Scope: Global
//        (packages/global/printing) 
//
///////////////////////////////////////////////////////////////////////////////
//
// Global Include-File with the functions for optimized diagram printing, i.e.
// diagrams are either printed only as large as neccessary or each diagram is
// printed on a whole page.
// Captions are printed as a picture paragraph below the diagram.
//
///////////////////////////////////////////////////////////////////////////////
//                
// uses externaly defined globals: <type name (package)>
//
///////////////////////////////////////////////////////////////////////////////
//
// uses externaly defined functions: <type name, package>
//
///////////////////////////////////////////////////////////////////////////////


// Types
struct EXTREMES
{
    int minx;
    int maxx;
    int miny;
    int maxy;
};


// Standard Print Units
float  DIAG_H_STD = 8.0;     // max. diag.frame.height is 7.5 Inches or 18.6 cm
float  DIAG_W_STD = 6.5;     // max. diag.frame.width is 6.5 Inches or 16.3 cm


// Need to call Default Print Settings ?
boolean NEEDS_DIAG_PRINT_SETTING = True;


///////////////////////////////////////////////////////////////////////////////
// PrintDiagram: Sets the default diagram print setting if needed (once).
//               If the external value for OPTIMISED_PRINTING is set
//               to True, the optimized diagram printing function 
//               OptPrintDiagram is called,
//               otherwise the standard printing function file_print. 
//
// Args: - DIAGRAM_FILE:       The StP diagram file to be printed. 
//       - OPTIMISED_PRINTING: True or False (can be the global mapping of
//                             a language depending external variable, 
//                             defined in XX_Lang_Externals.inc)
//       - P_SETTING:          If a user defined print setting exists, it
//                             can be used as argument here, otherwise 
//                             P_SETTING should be set to "". 
//       - NO_DISPLAYMARKS:    default or user defined suppress diplaymark 
//                             string (might be defined as external)
//       - BODY_NORMAL:        The standard paragraph format (should be 
//                             globaly defined in GL_Formats.inc)
//       - CAPTION:            The caption paragraph format (should be 
//                             globaly defined in GL_Formats.inc)
//       - DIAG_CAPTION:       Predefined Part of the diagram caption string
//                             (might be a global that is set to different  
//                             values depending on the chosen reporting
//                             language in XX_Reportstrings.inc)
//       - REP_DIAG:           Message text (might be a global that is set to
//                             different values depending on the chosen user
//                             interface language in XX_Messagestrings.inc)
//
///////////////////////////////////////////////////////////////////////////////
void PrintDiagram(file    DIAGRAM_FILE, 
                  boolean OPTIMISED_PRINTING,
                  string  P_SETTING,
		  string  NO_DISPLAYMARKS,
                  string  BODY_NORMAL,
                  string  CAPTION,
                  string  DIAG_CAPTION,
                  string  REP_DIAG)
{
    string manual_caption_text, automatic_caption_text;


    //------------------------------------------------
    // Print units are Inches, set sizes accordingly 
    // (ok for USLetter and A4)  
    //------------------------------------------------
    file_print_units_set(Inches);
    
    
    //----------------------------------------------
    // Call Print Setting function, if neccessary
    //----------------------------------------------
    if (NEEDS_DIAG_PRINT_SETTING)
    {
        //--------------------------
        // Suppress DisplayMarks
        //--------------------------
        
        NOPRINT_DM = set_create("string");
        if (StringToSet(NOPRINT_DM, StripInfo(NO_DISPLAYMARKS), " ") > 0) 
        {
            diagram_display_mark_no_print_set(NOPRINT_DM);
        }
        
        //-----------------------------------------
        // More Standard Diagram printing options
        //-----------------------------------------
        diagram_position_set(Centered);
        diagram_show_caption_set(False);
        diagram_frame_width_set(DIAG_W_STD);
        
        set_clear(NOPRINT_DM);
        NEEDS_DIAG_PRINT_SETTING = False;
    }
    
    if (e_GENERATION_INFO)
    {
        //---------------------------
        // Generation information
        //---------------------------
        PrintGenInfo(DIAGRAM_FILE.id);
    }
    
    if (to_string(target()) == "HTML")
    {
        diagram_caption_paragraph_format_set(CAPTION);
        automatic_caption_text = DIAGRAM_FILE.type + ":  " + 
            DIAGRAM_FILE.name;
    }
    else
    {
        automatic_caption_text = "";
    }
    
    //-----------------------------------------
    // Print the diagram
    //-----------------------------------------
//   if   (OPTIMISED_PRINTING == True) 
    OptPrintDiagram(DIAGRAM_FILE, OPTIMISED_PRINTING, 
                    P_SETTING, BODY_NORMAL, automatic_caption_text);
//   else 
//   {
//        diagram_orientation_scale_set(BestFit);
//        diagram_frame_height_set(DIAG_H_STD);
    
//        file_print(DIAGRAM_FILE, "", P_SETTING);
//   } 
    
    //-----------------------------------------
    // Print the caption for targets other than HTML.
    // We want the caption printed below the diagram, which is not
    // possible in RTF and with a running number, which is not
    // possible in Interleaf.
    // That's why we print it ourself.
    //----------------------------------------
    if (to_string(target()) != "HTML")
    {
        manual_caption_text = DIAG_CAPTION + DIAGRAM_FILE.type + " " + 
            DIAGRAM_FILE.name;
        paragraph(CAPTION);
        print(manual_caption_text);
    }
    
    return;
}


///////////////////////////////////////////////////////////////////////////////
// OptPrintDiagram: print diagram file with optimized size
//-----------------------------------------------------------------------------
// Args:            - DIAGRAM_FILE: the diagram to be printed
//                  - BODY_NORMAL:  the standard paragraph format 
//                  - CAPTION_TEXT: the actual caption text to print
//-----------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////

void  OptPrintDiagram(file    DIAGRAM_FILE,
                      boolean OPTIMISED_PRINTING,
                      string  P_SETTING,
                      string  BODY_NORMAL,
                      string  CAPTION_TEXT)
{
    EXTREMES  xy_extremes;
    
    list      all_xcoords, all_ycoords;
    string    query;


    //-----------------------------------------
    // find all noderefs sorted by the xcoord
    //-----------------------------------------
    all_xcoords = list_create("node_ref", 0);
    query       = "node_ref[file_id == ${DIAGRAM_FILE.id}] sort by xcoord";
    all_xcoords = list_select(query);
    
    //-----------------------------------------
    // find all noderefs sorted by the ycoord
    //-----------------------------------------
    all_ycoords = list_create("node_ref", 0);
    query       = "node_ref[file_id == ${DIAGRAM_FILE.id}] sort by ycoord";
    all_ycoords = list_select(query);
    
    //-----------------------------------------
    // find optimal orientation and size,
    // print the diagram (if not empty)
    //-----------------------------------------
    if (list_count(all_xcoords) != 0)
    {
        xy_extremes = FindExtremes(all_xcoords, all_ycoords);
        
        if   (IsPortrait(xy_extremes))
        {
            diagram_orientation_scale_set(PortraitFit);
            if   (OPTIMISED_PRINTING == True)
            { 
                DiagramPrintHightSet(xy_extremes);
                
//                if (to_string(target())== "RTF") 
//                   paragraph(BODY_NORMAL);
            }
            else 
            {
                diagram_frame_height_set(DIAG_H_STD);
                page(BODY_NORMAL);
            }
        }
        else
        {
            diagram_orientation_scale_set(LandscapeFit);
            diagram_frame_height_set(DIAG_H_STD);
            
            if (to_string(target())== "RTF") 
                diagram_frame_width_set(5.5);
            
            page(BODY_NORMAL);
        }
        
        file_print(DIAGRAM_FILE, CAPTION_TEXT, P_SETTING);	      
    }
    
    return;
}


///////////////////////////////////////////////////////////////////////////////
// IsPortrait: find out, whether the diagram should be printed in landscape
//             or in portrait format.
//-----------------------------------------------------------------------------
// Args:       - xy_extremes : extremes of the x- and y- coordinates
//-----------------------------------------------------------------------------
// Return:     True, if printable in Portrait format
//             False, otherwise
///////////////////////////////////////////////////////////////////////////////
boolean  IsPortrait(EXTREMES xy_extremes)
{
    //=========================================================================
    // This is basically a difference. However we add 50 to the Y coordinates
    // to try and give the diagram some balance i.e. a diagram may be only
    // fractionally wider than it is tall. 
    // We'll also try not to flip diagrams if they fit quite easily across the
    // page. Hence, (xy_extremes.maxx - xy_extremes.minx) > 1100.
    //=========================================================================
    if ((xy_extremes.maxx - xy_extremes.minx) > 1100 )
    {
        if   ((xy_extremes.maxx - xy_extremes.minx) > 
              ((xy_extremes.maxy - xy_extremes.miny) + 50))
            return(False);
        else return(True);
    }
    
    return(True);
}


///////////////////////////////////////////////////////////////////////////////
// DiagramPrintHightSet: Sets the Diagrame frame height for portrait oriented
//                       printing, depending on its Y-coordinates.
//                       (The width is already set to 6.5 in GlPrintDiagram)
//-----------------------------------------------------------------------------
// Args:                 - xy_extremes : extremes of the x- and y- coordinates
//-----------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////
void DiagramPrintHightSet(EXTREMES xy_extremes)
{
    float    yy, frame_height;
    
//  Begin change ECR 4872: Reason for too small diagrams was a missing 
//                         conversion from integer to float 
//  yy = xy_extremes.maxy - xy_extremes.miny;
    yy = to_float(xy_extremes.maxy - xy_extremes.miny);
//  End change ECR 4872
    
    yy = (yy/100) + 1;
    yy = yy - (yy/10);

// SPR 2665 - generated reports to small when optimized printing is turned on
//    if        (yy < 1.5)
//    { frame_height = 1.5; }  // 1.5 Inches or 3.8 cm
//    else if   (yy < 2)
//    { frame_height = 2.0; }  // 2.0 Inches or 5.1 cm
//    else if   (yy < 2.5)
//    { frame_height = 2.5; }  // 2.5 Inches or 6.3 cm
//    else if   (yy < 3)
//    { frame_height = 3.0; }  // 3.0 Inches or 7.6 cm
    if   (yy < 3.5)
    { frame_height = 3.5; }  // 3.5 Inches or 8.8 cm
    else if   (yy < 4)
    { frame_height = 4.0; }  // 4.0 Inches or 10.1 cm
    else if   (yy < 4.5)
    { frame_height = 4.5; }  // 4.5 Inches or 11.3 cm
    else if   (yy < 5)
    { frame_height = 5.0; }  // 5.0 Inches or 12.6 cm
    else if   (yy < 5.5)
    { frame_height = 5.5; }  // 5.5 Inches or 13.8 cm
    else if   (yy < 6)
    { frame_height = 6.0; }  // 6.0 Inches or 15.1 cm
    else if   (yy < 6.5)
    { frame_height = 6.5; }  // 6.5 Inches or 16.3 cm
    else if   (yy < 7)
    { frame_height = 7.0; }  // 7.0 Inches or 17.6 cm
    else if   (yy < 7.5)
    { frame_height = 7.5; }  // 7.5 Inches or 18.1 cm
    else      
    { frame_height = DIAG_H_STD; }
    
    diagram_frame_height_set(frame_height);
    
    return;
}


///////////////////////////////////////////////////////////////////////////////
// FindExtremes: Find the diagrams outmost coordinates (for the frame size)
//-----------------------------------------------------------------------------
// Args:          - xcoords : list of the diagrams x-coordinates
//                - ycoords : list of the diagrams y-coordinates
//-----------------------------------------------------------------------------
// Return:        struct Extremes filled with minimum and maximum coord.values
///////////////////////////////////////////////////////////////////////////////
EXTREMES  FindExtremes(list xcoords, list ycoords)
{
    EXTREMES  xy;
    int	     count;
    node_ref  coord;
    
    count   = list_count(xcoords) - 1;
    
    coord   = list_get(xcoords, count);
    xy.maxx = coord.xcoord;
    
    coord   = list_get(xcoords, 0);
    xy.minx = coord.xcoord;
    
    coord   = list_get(ycoords, count);
    xy.maxy = coord.ycoord;
    
    coord   = list_get(ycoords, 0);
    xy.miny = coord.ycoord;
    
    return xy;
}

