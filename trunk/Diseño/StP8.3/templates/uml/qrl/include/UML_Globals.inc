//*****************************************************************************
// DESCRIPTION: UML_Globals.inc
//              ----------------------
//              (c) 1997 by AONIX 
//*****************************************************************************
//
// Include-file for UML-Reports with declarations of globaly defined datatypes, 
// constants and variables.
//*****************************************************************************


//*****************************************************************************
// Datatypes
//*****************************************************************************
struct STATE_INFO                             // Struct for the combined 
{                                             // information about one state
       node classe ;
       list states ;
       list events ;
       list actions ;
       list sendevents ;
};

struct SEND_EVENT                             // Struct with all information
{                                             // about an event
       string Event ;
       string Class ;
} ;


//*****************************************************************************
// Global Constants
//*****************************************************************************
//----------------------------------------------------------------------------
// Class Table Rows: Constants for the class table rows to be printed must be 
//                   checked for every new OMT/Booch version
//----------------------------------------------------------------------------
const string MEMBER_COL      = "1,2,3";
const string ANALYSIS_COL_A  = "1,4,5,6";
const string ANALYSIS_COL_O  = "1,4,5,6,7";
const string PROG_CXX_COL_A  = "1,8,9,10";
const string PROG_CXX_COL_O  = "1,8,9,10,11,12";
const string PROG_JAVA_COL   = "1,16,17,18,19";
const string PROG_IDL_COL_A  = "1,20,21";
const string PROG_IDL_COL_O  = "1,20,21,22,23";
const string PROG_ADA_COL_A  = "1,13,14";
const string PROG_ADA_COL_O  = "1,13,14,15";
const string ATT_H_ROWS      = "1,2";
const string OP_H_ROWS       = "1,";



//*****************************************************************************
// Global Variables 
//*****************************************************************************
//---------------------------------------------------------------------
// Language independent variables for languagedependent externals used 
// in the OMT report
//---------------------------------------------------------------------
boolean e_CL_ATT_OP;           // include attributes and operations
boolean e_CL_INTERACTION;      // include interaction diagrams
boolean e_CL_BEHAVIOR;         // include classes behavior
boolean e_CLASS_DETAILS;       // generate the class report in detail
boolean e_GEN_ASSOC_SUM_TBL;   // generate the association summary table
boolean e_GEN_CLASS_REPORT;    // generate the class report
boolean e_GEN_PACKAGE_REPORT;  // generate the package report
boolean e_GEN_STEREOTYPES;     // generate the Stereotype chapter
boolean e_GEN_SYS_ARCH;        // generate the system arcitektur chapter
boolean e_GEN_USERS_VIEW;      // generate the Users View
boolean e_IMPL_DETAILS;        // generate the phys. arc in detail
boolean e_IN_CL_TABLE_FORM;    // print atts and ops in table form
boolean e_LOG_ARCH;            // generate the logical system architecture
boolean e_PACK_DETAILS;        // generate the package report in detail
boolean e_PACK_DIAGRAMS;       // generate the package report in detail
boolean e_PHYS_ARCH;           // generate the physical system architecture
boolean e_SCENARIOS;           // generate the Use Case Scenarios
boolean e_STEREOTYPE_FILES;    // include stereotype diagrams
boolean e_UV_DETAILS;          // generate the Users View in detail
boolean e_WITH_ADA_INFO;       // include ada items for att and op
boolean e_WITH_ANALYSIS_INFO;  // include analysis items for att and op
boolean e_WITH_CXX_INFO;       // include c++ items for att and op
boolean e_WITH_IDL_INFO;       // include idl items for att and op
boolean e_WITH_JAVA_INFO;      // include java items for att and op

string e_CLASSES;              // reduce to given classes
string e_PACKAGES;             // reduce to given packages
string e_STEREOTYPES;          // reduce to given stereotypes
string e_USECASE_FILES;        // reduce to given uusecased files
string e_IMPL_FILES;           // reduce to given ucomponent/udeployment files

