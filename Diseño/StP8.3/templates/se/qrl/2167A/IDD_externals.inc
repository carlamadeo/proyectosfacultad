//
//      sccsid[] = %W% %Y% %D%
//      StP/Core
//      Confidential property of Interactive Development Environments, Inc.
//      Copyright (c) 1992-1993
//      All rights reserved
//



external boolean __________SECTIONS__________ = True;

// 1.2 System overview

external string SECTION_1_2_System_overview = "<SYSTEM OVERVIEW>";
external_help = "10.1.3.2 System Overview.  This paragraph shall be numbered
1.2 and shall briefly state the purpose of the system and shall
identify and describe the role of the interfaces, to which this 
IDD applies, within the system.";


external string SECTION_1_3_Document_overview = "<DOCUMENT OVERVIEW>";
external_help = "10.1.3.3 Document Overview.  This paragraph shall 
be numbered 1.3 and shall summarize the purpose and contents of this 
document.";


external string SECTION_2_Referenced_documents = "<REFERENCED DOCUMENTS>";
external_help = "10.1.4 This section shall be numbered 2 and list by document
and title all documents referenced in this document.  This
section shall also identify the source for all documents not
available through normal Government stocking activities.";

// Gets the text for the IDD Notes section from a file

external string SECTION_4_Notes = "<NOTES>";
external_help = "10.1.6 This section shall be numbered 4, and contain any 
general information that aids in understanding this document (e.g., 
background information, glossary, formula derivations).";

// Gets the text for the IDD Notes abbreviation section from a file

external string SECTION_4_abbreviations = "<ABBREVIATIONS>";
external_help = "10.1.6 . . .  This section shall include an alphabetical 
listing of all acronyms, abbreviations, and their meanings as used in
this document.";



external boolean __________VALUES____________ = True;
//external_help = "All of the following external variables will be used
//in one or more boiler plates within this document.";

#include "qrl/2167A/2167A_global_externals.inc"

external string csci_title = "<CSCI TITLE>";
external_help = "title of the CSCI to which the document applies";

external string csci_abbreviation = "<CSCI ABBREVIATION";
external_help = "The abbreviation for the CSCI title , if applicable, between
parentheses";

// Number of the Computer Software Configuration Item 
external string csci_number = "<CSCI NUMBER>";
external_help = "The number of the CSCI to which the document applies";


external string document_number = "<DOCUMENT NUMBER>";
external_help = "Identify the document control number and date.  Identify
volume x of y if multi-volume";


external boolean __________CONTROL___________ = False;
external_help = "All of the following external variables will be used
to control the generation of this document.";

external boolean narrative_document = False;
external_help =
"This setting controls the narrative vs reference nature of this
document.  If this variable is set to:

    False: Most diagrams and tables will be found in appendices
    True:  Most diagrams and tables will be found in the body of the
           document where they are first referenced.";


external boolean select_IDD_components = False;
external_help =
"This setting determines how objects (data flows, processes, ...) are
selected for inclusion in this document.  If this variable is set to:

   True:  Only included if they are designated with the IRS document
                  annotation (see Printing Manual).
   False: The IRS document annotation will not be used.";

external boolean use_StP_generated_identifiers = True;
external_help =
"The variable controls the source of identifiers used for interfaces,
capabilities and other items.  If this variable is set to:

    True:  From StP Repository generated object identifiers.
    False: From annotations filled in by the user.";
    
