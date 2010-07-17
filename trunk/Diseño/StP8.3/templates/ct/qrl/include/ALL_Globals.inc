//*****************************************************************************
// DESCRIPTION: ALL_Globals.inc
//              ----------------------
//              (c) 1997 by AONIX
//*****************************************************************************
//
// Include-file for all stp reports with declarations of globaly defined   
// datatypes, constants and variables.
//*****************************************************************************


//*****************************************************************************
// Datatypes
//*****************************************************************************

struct PRT_IN                             // Struct with all information
{                                         // about already printed objects
       int    OBJ_ID;
       string REFERENCE;
};

struct T_ROWS                             // Struct with first and last row
{                                         // of a table section, occupied by
       int first;                         // requirements, attributes, ...
       int last;
};

//*****************************************************************************
// Global Constants
//*****************************************************************************

const int  IN  = 0;                // Constants for better readability
const int  OUT = 1;

const int  IS_OK    = 0;           // Constants for Error handling
const int  WELL_HMM = 1;
const int  NOT_OK   = 2;

const string OX = "xcoord";        // Constants for ordered queries
const string OY = "ycoord";

const string ANY = " name $ '*' "; // Constant for any node / file


//*****************************************************************************
// Global Variables 
//*****************************************************************************
//---------------------------------------------------------------------
// Language independent variables for language dependent externals used 
// in all StP reports
//---------------------------------------------------------------------
boolean e_GEN_SINGLE_HTML_FILE;  // generate HTML output to single file
boolean e_GEN_DESCRIPTIONS;      // generate descriptions for objects and diagr.
boolean e_GEN_GLOSSARY;          // generate appendix with glossary
boolean e_GEN_INTRODUCTIONS;     // generate phase titles
boolean e_GEN_REQUIREMENTS;      // generate requirement chapters
boolean e_GENERATION_INFO;       // print generation info 
boolean e_IN_RQ_TABLE_FORM;      // print requirements in table form
boolean e_MISSING_DESCRIPTION;   // generate comments for missing descriptions
boolean e_MULTIPLE_DESCRIPTION;  // generate multiple object descriptions
boolean e_OPTIMIZED_PRINTING;    // optimize diagram printing to best fit on pages
boolean e_PRINT_ALPHA_ORDER;     // with ablphabeticaly ordered objects
boolean e_REFD_GLOSSARY;         // reduce glossary to referenced objects 
boolean e_SHOW_UNALLOCATED;      // show unallocated requirements
boolean e_SUPERSEDED_REQ;        // print superseded requirements too

string e_COMPANY;                // name of the company (doctitle)
string e_DEFAULT_FORMAT;         // Default Format File
string e_DIVISION;               // name of the division (doctitle)
string e_DOC_AUTHOR;             // Author of this document
string e_DOC_STATUS;             // Status of this document
string e_DOC_SUBTITLE;           // Subtitle of this dokument
string e_DOC_TITLE;              // Title of this dokument
string e_DOC_VERSION;            // Version of this dokument
string e_DOCUMENT_LANGUAGE;      // Target language
string e_PRINT_SETTING;          // Name of a user defined print setting
string e_NO_DISPLAYMARKS;        // displaymarks to be suppressed
string e_PREFIX;                 // reduce to given prefix
string e_RQTBL_FILES;            // reduce to given rqtbl files

//---------------------------------------------------------------------
// Global Sets and lists  
//---------------------------------------------------------------------
set GLOSSARY_ITEMS;   // collect the referenced glossary topic items
set PRINTED_FILES;    // set of already printed files with reference

//---------------------------------------------------------------------
// Global Set of suppressed displaymarks 
//---------------------------------------------------------------------
set NOPRINT_DM;
