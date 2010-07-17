////////////////////////////////////////////////////////////////////////////////
// DESCRIPTION: ALL_PrintDocument.inc
//              ==============================
//              (c) 1997 by  S. Noack, Aonix
//
// Scope: Global
//        (packages/global/printing) 
//
////////////////////////////////////////////////////////////////////////////////
//
// Global Include-File with the functions for printing several FrameMaker  
// Document Variables and Markers and RTF Fields. 
// Theese are needed for cross references, user specific text references and
// include fields for external files in the generated document.
////////////////////////////////////////////////////////////////////////////////
//                
// Globals:   -
// Constants: const string PIC_TAG  = "@#$"  needed for postprocessing frame
//                                           documents with the insertpic tool 
//            const string RTF_PIC1 = see definition below 
//            const string RTF_PIC2 = see definition below 
////////////////////////////////////////////////////////////////////////////////
//
// Functions: - void  PrintDocVar(string  F_VARNAME, string  F_VARID,
//                                string  R_FIELD,   string  R_DEFRESULT)
//            - void  PrintFrameMarker(string  M_TYPE, string  M_TEXT)
//            - void  PrintPicture(int LENGTH, string P_STRING)
////////////////////////////////////////////////////////////////////////////////
//
// uses externaly defined globals: <type name (package)>
////////////////////////////////////////////////////////////////////////////////
//
// uses externaly defined functions: <type name, package>
////////////////////////////////////////////////////////////////////////////////

const string PIC_TAG  = "@#$"; 
const string RTF_PIC1 = "{\\field{\\*\\fldinst {\\cs76\\fs18  INCLUDEPICTURE \"";
const string RTF_PIC2 = "\"\\\\* MERGEFORMAT \\\\d }}{\\fldrslt " +
                        "{\\fs18\\lang1024 }}}";
boolean HAS_PICTURES  = False;

///////////////////////////////////////////////////////////////////////////////
// PrintDocVar: print the value of the given document-variable (field). 
//
//              Frame: The Framemaker-variable must be defined in the
//                     MIF format-file. We have to use a dangerous trick
//                     here, because the variable-id might change !!!
//                     Parameters are F_VARNAME and F_VARID
//
//              RTF:   We use word for windows std fields with results,
//                     Parameters are R_FIELD and R_DEFRESULT
//
//              - F_VARNAME:   name of the framevariable
//              - F_VARID:     unique id of the framevariable (danger !!)
//				- R_FIELD:     name of the (std word for windows) field
//				- R_DEFRESULT: predefined result (until the first update)
//
//
// Example:     PrintDocVar("Page Count", "286", "NUMPAGES", "1"); 
///////////////////////////////////////////////////////////////////////////////
void PrintDocVar(string F_VARNAME, string F_VARID,
                 string R_FIELD,   string R_DEFRESULT)
{
   string variable ;

   if (target() == Framemaker) 
   {
      variable = "\n";
      variable = variable + "  # Insert Variable\n";
      variable = variable + "  <ParaLine \n";
      variable = variable + "   <Variable\n";
      variable = variable + "    <VariableName `" + F_VARNAME + "'>\n";
      variable = variable + "    <Unique " + F_VARID + ">\n"; // VERY RISCY !!
      variable = variable + "   > # end of Variable \n";
      variable = variable + "  > \n";
      print_raw(variable);
   }

   else if (target() == RTF) 
   {
      variable = "{\\field{\\*\\fldinst  " + R_FIELD + " \\\\* MERGEFORMAT }";
      variable = variable + "{\\fldrslt  " + R_DEFRESULT + "}}";
      print_raw(variable);
   }

//   else if (target() == Interleaf)    NOT IMPLEMENTED

   return;
}


///////////////////////////////////////////////////////////////////////////////
// PrintFrameMarker: FrameMaker specific 
//                   Print a FrameMaker marker of the given type and text. 
//                   We need this to be able to include more than 2  
//                   userspecific Document Variables in the head- and 
//                   footlines of the document or to create cross references 
//                   for other documents. 
//                   - M_TYPE: type of the marker
//                   - M_TEXT: text of the marker
//
// Example:          PrintFrameMarker("1", "This is the text of marker 1");
///////////////////////////////////////////////////////////////////////////////
void PrintFrameMarker(string M_TYPE, string M_TEXT)
{
   string marker ;

   if (target() == Framemaker) 
   {
      marker = "\n";
      marker = marker + "  # Insert Marker\n";
      marker = marker + "  <ParaLine \n";
      marker = marker + "   <Marker\n";
      marker = marker + "    <MType " + M_TYPE + ">\n";
      marker = marker + "    <MText `" + M_TEXT + "'>\n";
      marker = marker + "   > # end of Marker \n";
      marker = marker + "  > \n";
      print_raw(marker);
   }

   return;
}


///////////////////////////////////////////////////////////////////////////////
// PrintPicture: This function handles the graphic import for stp reports.   
//               A description string, which is preprocessed and therefore not
//               empty, is searched for the picture tag @#$. 
//
//               RTF: RTF field tags are printed to import this picture, which
//                    is assumed to be in the reports directory. 
//               Mif: The picture is expected in the systems pictures 
//                    directory. 
//
//               If a picture tag is found, the picture global is set to True. 
//               If no picture tag is found or the target is not RTF, the
//               string is printed as it is (for the insertpic postprocessing).
//
//               - LENGTH:   length of the string
//               - P_STRING: preprocessed object note description string 
///////////////////////////////////////////////////////////////////////////////
void PrintPicture(int LENGTH, string P_STRING)
{
   int    from, 
          part, 
          position,
          to;

   string picture, 
          postfix,
          prefix,
          wsp; 

   HAS_PICTURES = True;  // Set the global Switch
   position     = 0;

   if   (target() == RTF)   
   {
        while (position < LENGTH)
        {
            from = string_find(P_STRING, position, PIC_TAG);

            if (from == LENGTH)
            {
               print(postfix);
               break;
            }

            for (to = from; to < LENGTH; to = to + 1)
            {
                wsp = string_extract(P_STRING, to, 1);
                if ((wsp == " ") || (wsp == "\n") || 
                    (wsp == "\t") || (wsp == "\r"))
                break;
            }
 
            part     = to - from;
            picture  = string_extract(P_STRING, from, part);

            part     = from - position;
            if   (part > 0)
                 prefix = string_extract(P_STRING, position, part);
            else prefix = "";

            part     = LENGTH - to;
            if   (part > 0)
                 postfix = string_extract(P_STRING, to, part);
            else postfix = "";

            picture  = string_strip(picture, "L", PIC_TAG);
            position = to;

            print(prefix + "\n\n");
            print_raw(RTF_PIC1 + picture + RTF_PIC2);
            print("\n");
            message(rep_graf + picture);
        }
   }
   else print(P_STRING);      // Framemaker

   return;
}


