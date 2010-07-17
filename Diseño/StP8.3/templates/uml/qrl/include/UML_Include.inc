//*****************************************************************************
// DESCRIPTION: UML_Include.inc
//              ----------------------
//              (c) 1997 by AONIX
//*****************************************************************************
//
// Include Statements for all included files needed for all (i.e. language 
// independent) UML-Reports 
//*****************************************************************************
//*****************************************************************************
// Included Files:
// ALL_Globals.inc :  For stp-report datatypes, constants and variables
// ALL_Formats.inc :  For character and paragraph formats (stpreport template)
// ALL_Queries.inc :  For basic OMS queries
// ALL_Print.inc :    For global functions to optimize diagram printing
// UML_Globals.inc :  For UML-report datatypes, constants and variables
// UML_Strings.inc :  For messages, headings, labels and other text
// UML_Print.inc :    For UML specific print functions
// UML_Report.inc :   For the main file containing the UMLgeneration functions
// table.inc :        For table creation functions needed in AssocSummaryTable
//*****************************************************************************

#include "qrl/include/UML_Mapping.inc"
#include "qrl/include/ALL_Globals.inc"
#include "qrl/include/UML_Globals.inc"
#include "qrl/include/ALL_Formats.inc"
#include "qrl/include/UML_Strings.inc"
#include "qrl/include/ALL_Queries.inc"
#include "qrl/include/ALL_Print.inc"
#include "qrl/include/ALL_PrintDiagram.inc"
#include "qrl/include/ALL_PrintTable.inc"
#include "qrl/include/ALL_PrintDocument.inc"
#include "qrl/include/UML_Print.inc"
#include "qrl/include/ALL_Report.inc"
#include "qrl/include/UML_Report.inc"

#include "ct/qrl/include/table.inc"
