//*****************************************************************************
//DESCRIPTION: ALL_Formats.inc
//             ==============================
//             (c) 1997 by Aonix GmbH
//*****************************************************************************
//
// Include-file for all ooreports with declaraions of paragraph and character
// formats.
// If you want to use another template with different formats, you can map them
// here.
//
// FrameMaker MIF Generation: FORMATS USED IN ooreport.mif
// The definition of these formats can be found in the paragraph and character
// format designer of the framemaker template ooreport.mif 
//
// RTF Generation: FORMATS USED IN ooreport.rtf
// The definition of these formats can be found in the paragraph and character
// format designer of the rtf template ooreport.rtf 
//*****************************************************************************
//
// functions: - int      SetTargetFormat() 
//*****************************************************************************

//*****************************************************************************
// Global format variables 
//*****************************************************************************

//=================
// Paragraphs
//=================

string AUTHOR;
string BODY;
string BODY_BOLD;
string BODY_NORMAL;
string BULLET;
string CAPTION;
string CBULLET;
string CELLBODY;
string CELLHEAD;
string COMPANY;
string CSTEP;
string DIVISION;
string DOCAUTHOR; 
string DOCINFO;                 
string DOCSUBTITLE;       
string DOCTITLE;   
string DOCVERSION;    
string EBENE;              // is postfixed with a level like Ebene1
string FOOTER;
string HEAD1;
string HEAD1NOIVZ;
string HEAD1TOC;
string HEAD2;
string HEAD2TOC;
string HEAD3;
string HEAD3TOC;
string HEAD4 ;
string HEADER;
string NHEAD1;
string NHEAD1TOC;
string NHEAD2;
string NHEAD2TOC;
string NHEAD3;
string NHEAD3TOC;
string STEP0;
string STEP1;
string SUBTITLE;
string TITLE;

//===================
// Character Formats 
//===================

string BOLD10;
string BOLD11;
string BOLD12;
string BOLDSLIM;
string BOLDITALIC;
string BOLDUNDER;
string EXTRASMALL;
string ITALIC;
string INHEAD;
string STANDARD;
string UNDERLINE;


//*****************************************************************************
// SetTargetFormat: Sets the paragraph and character formats depending
//                  on the target format of the actual generation.
//*****************************************************************************
int  SetTargetFormat() 
{
   //====================================================================
   // Use a target string to prevent the parser from complaining about 
   // illegal format RTF in StP/Core < 2.3
   //====================================================================
   string MY_TARGET = to_string(target());   

   if (MY_TARGET == "Framemaker") 
   {
       //-------------------- 
       // Paragraph Formats 
       //-------------------- 

       AUTHOR      = "Author";
       BODY        = "Body";
       BODY_BOLD   = "Body_bold";
       BODY_NORMAL = "Body_normal";
       BULLET      = "Bullet";
       CAPTION     = "Figure";
       CBULLET     = "CBullet";
       CELLBODY    = "CellBody";
       CELLHEAD    = "CellHeading";
       COMPANY     = "Company";
       CSTEP       = "CStep";
       DIVISION    = "Division";
       DOCAUTHOR   = "DocAuthor"; 
       DOCINFO     = "DocInfo";                 
       DOCSUBTITLE = "DocSubTitle";       
       DOCTITLE    = "DocTitle";   
       DOCVERSION  = "DocVersion";    
       EBENE       = "Ebene";         // is postfixed with a level like Ebene1
       FOOTER      = "Footer";
       HEAD1       = "1Heading";
       HEAD1NOIVZ  = "Heading1NoIVZ";
       HEAD1TOC    = "1HeadingTOC";
       HEAD2       = "2Heading";
       HEAD2TOC    = "2HeadingTOC";
       HEAD3       = "3Heading";
       HEAD3TOC    = "3HeadingTOC";
       HEAD4       = "4Heading";
       HEADER      = "Header";
       NHEAD1      = "N1Heading";
       NHEAD1TOC   = "N1HeadingTOC";
       NHEAD2      = "N2Heading";
       NHEAD2TOC   = "N2HeadingTOC";
       NHEAD3      = "N3Heading";
       NHEAD3TOC   = "N3HeadingTOC";
       STEP0       = "Step";
       STEP1       = "1Step";
       SUBTITLE    = "Subtitle";  
       TITLE       = "Title";                   

       //-------------------- 
       // Character Formats 
       //-------------------- 

       BOLD10     = "Bold";
       BOLD11     = "Bold11";
       BOLD12     = "Bold12";
       BOLDITALIC = "BoldItalic";
       BOLDSLIM   = "Bold11";
       BOLDUNDER  = "BoldUnderline";
       EXTRASMALL = "ExtraSmall";
       ITALIC     = "Italic";
       INHEAD     = "Standard";
       STANDARD   = "Standard";
       UNDERLINE  = "Underline";

       e_DEFAULT_FORMAT = e_DEFAULT_FORMAT + ".mif";
       return IS_OK;
   }

   else if (MY_TARGET == "RTF")
   {	          
       //-------------------- 
       // Paragraph Formats 
       //-------------------- 

       AUTHOR      = "Autor";
       BODY        = "Body";
       BODY_BOLD   = "BodyFett";
       BODY_NORMAL = "BodyoA";
       BULLET      = "Punkt";
       CAPTION     = "Figure";
       CBULLET     = "OPunkt";
       CELLBODY    = "StdZelle";
       CELLHEAD    = "TitelZelle";
       COMPANY     = "DFirma";
       CSTEP       = "ONummer";
       DIVISION    = "DAbteilung";
       DOCAUTHOR   = "DAutor"; 
       DOCINFO     = "DInfo";                 
       DOCSUBTITLE = "DUntertitel";       
       DOCTITLE    = "DTitel";   
       DOCVERSION  = "DVersion";    
       EBENE       = "Ebene";         // is postfixed with a level like Ebene1
       FOOTER      = "footer";
       HEAD1       = "heading 1";
       HEAD1NOIVZ  = "NoIVZ";
       HEAD1TOC    = "Verzeichnis 1";
       HEAD2       = "heading 2";
       HEAD2TOC    = "Verzeichnis 2";
       HEAD3       = "heading 3";
       HEAD3TOC    = "Verzeichnis 3";
       HEAD4       = "Heading4";
       HEADER      = "header";
       NHEAD1      = "heading 1";
       NHEAD1TOC   = "Verzeichnis 1";
       NHEAD2      = "heading 2";
       NHEAD2TOC   = "Verzeichnis 2";
       NHEAD3      = "heading 3";
       NHEAD3TOC   = "Verzeichnis 3";
       STEP0       = "Nummer";
       STEP1       = "Nummer1";
       SUBTITLE    = "AONIXSubtitle";  
       TITLE       = "AONIXTitle";                   

       //-------------------- 
       // Character Formats 
       //-------------------- 

       BOLD10     = "Fett10";
       BOLD11     = "Fett11";
       BOLD12     = "Fett12";
       BOLDITALIC = "FettKursiv";
       BOLDSLIM   = "Fett11";
       BOLDUNDER  = "FettUnter";
       EXTRASMALL = "Klein";
       ITALIC     = "Kursiv";
       INHEAD     = "ImKopf";             // For Display in Header
       STANDARD   = "OttoNormal";
       UNDERLINE  = "Unterstrichen";

       e_DEFAULT_FORMAT = e_DEFAULT_FORMAT + ".rtf";
       return IS_OK;
   } 

   else if (MY_TARGET == "Interleaf")
   {	   

       //-------------------- 
       // Paragraph Formats 
       //-------------------- 

       AUTHOR      = "Autor";
       BODY        = "Body";
       BODY_BOLD   = "BodyFett";
       BODY_NORMAL = "BodyoA";
       BULLET      = "Punkt";
       CAPTION     = "Figure";
       CBULLET     = "OPunkt";
       CELLBODY    = "StdZelle";
       CELLHEAD    = "TitelZelle";
       COMPANY     = "DFirma";
       CSTEP       = "ONummer";
       DIVISION    = "DAbteilung";
       DOCAUTHOR   = "DAutor"; 
       DOCINFO     = "DInfo";                 
       DOCSUBTITLE = "DUntertitel";       
       DOCTITLE    = "DTitel";   
       DOCVERSION  = "DVersion";    
       EBENE       = "Ebene";         // is postfixed with a level like Ebene1
       FOOTER      = "footer";
       HEAD1       = "heading 1";
       HEAD1NOIVZ  = "NoIVZ";
       HEAD1TOC    = "Verzeichnis 1";
       HEAD2       = "heading 2";
       HEAD2TOC    = "Verzeichnis 2";
       HEAD3       = "heading 3";
       HEAD3TOC    = "Verzeichnis 3";
       HEAD4       = "Heading4";
       HEADER      = "header";
       NHEAD1      = "heading 1";
       NHEAD1TOC   = "Verzeichnis 1";
       NHEAD2      = "heading 2";
       NHEAD2TOC   = "Verzeichnis 2";
       NHEAD3      = "heading 3";
       NHEAD3TOC   = "Verzeichnis 3";
       STEP0       = "Nummer";
       STEP1       = "Nummer1";
       SUBTITLE    = "AONIXSubtitle";  
       TITLE       = "AONIXTitle";                   

       //-------------------- 
       // Character Formats 
       //-------------------- 

       BOLD10     = "F14";
       BOLD11     = "F13";
       BOLD12     = "F23";
       BOLDITALIC = "F31";
       BOLDSLIM   = "F13";
       BOLDUNDER  = "F23";
       EXTRASMALL = "F20";
       ITALIC     = "F22";
       INHEAD     = "F10";
       STANDARD   = "F11";
       UNDERLINE  = "F11";

       e_DEFAULT_FORMAT = e_DEFAULT_FORMAT + ".doc";
       return IS_OK;
   } 

   else if (MY_TARGET == "HTML")
   {
       //-------------------- 
       // Paragraph Formats 
       //-------------------- 

       AUTHOR      = "Author";
       BODY        = "Body";
       BODY_BOLD   = "Body_bold";
       BODY_NORMAL = "Body_normal";
       BULLET      = "LI.Bullet";
       CAPTION     = "Figure";
       CBULLET     = "CBullet";
       CELLBODY    = "CellBody";
       CELLHEAD    = "CellHeading";
       COMPANY     = "Company";
       CSTEP       = "CStep";
       DIVISION    = "Division";
       DOCAUTHOR   = "DocAuthor"; 
       DOCINFO     = "DocInfo";                 
       DOCSUBTITLE = "DocSubTitle";       
       DOCTITLE    = "DocTitle";   
       DOCVERSION  = "DocVersion";    
       EBENE       = "Ebene";         // is postfixed with a level like Ebene1
       FOOTER      = "Footer";
       HEAD1       = "Heading1";
       HEAD1NOIVZ  = "Heading1NoIVZ";
       HEAD1TOC    = "HeadingTOC1";
       HEAD2       = "Heading2";
       HEAD2TOC    = "HeadingTOC2";
       HEAD3       = "Heading3";
       HEAD3TOC    = "HeadingTOC3";
       HEAD4       = "Heading4";
       HEADER      = "Header";
       NHEAD1      = "N1Heading";
       NHEAD1TOC   = "N1HeadingTOC";
       NHEAD2      = "N2Heading";
       NHEAD2TOC   = "N2HeadingTOC";
       NHEAD3      = "N3Heading";
       NHEAD3TOC   = "N3HeadingTOC";
       STEP0       = "Step";
       STEP1       = "Step1";
       SUBTITLE    = "Subtitle";  
       TITLE       = "Title";                   

       //-------------------- 
       // Character Formats 
       //-------------------- 

       BOLD10     = "Bold";
       BOLD11     = "Bold11";
       BOLD12     = "Bold12";
       BOLDITALIC = "BoldItalic";
       BOLDSLIM   = "Bold11";
       BOLDUNDER  = "BoldUnderline";
       EXTRASMALL = "ExtraSmall";
       ITALIC     = "Italic";
       INHEAD     = "Standard";
       STANDARD   = "Standard";
       UNDERLINE  = "Underline";

       e_DEFAULT_FORMAT = e_DEFAULT_FORMAT + ".css";
       return IS_OK;
   }

   //------------------------------------------------------------
   // If we reach this section, we have an unsupported target !
   //------------------------------------------------------------
   return NOT_OK;
}
