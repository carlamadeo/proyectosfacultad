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
external_help = "10.1.3.2   System Overview.  This paragraph shall be numbered
1.2 and shall briefly state the purpose of the system and
shall identify and describe the role, within the system, of
the interfaces to which this IRS applies.";


// 1.3 Document overview

external string SECTION_1_3_Document_overview = "<DOCUMENT OVERVIEW>";
external_help = "10.2.3.3  Document Overview.  This paragraph shall be numbered
1.3 and shall summarize the purpose and contents of this document.";


// 2.1 Government documents

external string SECTION_2_1_Government_documents = "<GOVERNMENT DOCUMENTS>";
external_help = "10.1.4.1  Government Documents.  This paragraph shall be
numbered 2.1 and begin with one of the following paragraphs:
1. The following documents of the exact issue shown form a
part of this specification to the extent specified herein.
In the event of conflict between the documents referenced
herein and the contents of this specification, the contents
of this specification shall be considered a superseding
requirement.
2. The following documents of the exact issue shown form a
part of this specification to the extent specified herein.
In the event of conflict between the documents referenced
herein and the contents of this specification, the contents
of this specification shall be considered a superseding
requirement, except for specification (enter number of next
higher-tiered specification) listed below.
 
Government documents shall be listed by document number and
title in the following order:
             
SPECIFICATIONS:               
   Federal
   Military
   Other Government Agency
             
STANDARDS:                   
   Federal
   Military
   Other Government Agency
              
DRAWINGS:
(Where detailed drawings referred to in a specification are
listed on an assembly drawing, it is only necessary to list
the assembly drawing.

OTHER PUBLICATIONS:   
   Manuals
   Regulations
   Handbooks
   Bulletins
   etc.
The following paragraph shall appear at the conclusion of
the list of documents:
Copies of specifications, standards, drawings, and
publications required by suppliers in connection with
specified procurement functions should be obtained from the
contracting agency or as directed by the contracting
officer.";


// 2.2 Non-Government documents

external string SECTION_2_2_Non_Government_documents = "<NON GOVERNMENT DOCUMENTS>";
external_help = "10.1.4.2  Non-Government Documents.  This paragraph shall be
numbered 2.2 and begin with the following paragraph:  The
following documents of the exact issue shown form a part of
this specification to the extent specified herein.  In the
event of conflict between the documents referenced herein and
the contents of this specification, the contents of this
specification shall be considered a superseding requirement.

Non-Government documents shall be listed by document number  
and title in the following order:
 
SPECIFICATIONS:
STANDARDS:
DRAWINGS:
OTHER PUBLICATIONS:
 
(List source for all documents not available through normal
Government stocking activities.)
 
The following source paragraph shall be placed at the bottom
of the list when applicable:  Technical society and
technical association specifications and standards are
generally available for reference from libraries.  They are
also distributed among technical groups and using Federal
Agencies.";


// 6. Notes

external string SECTION_6_notes = "<NOTES>";
external_help = "10.2.8  NOTES. This section shall be numbered 6.  
and contain any general information that aids in understanding this 
document (e.g., background information, glossary, formula derivations).";


// 6. Abbreviations

external string SECTION_6_abbreviations = "<ABBREVIATIONS>";
external_help = "This section shall include an alphabetical listing of 
all acronyms, abbreviations, and their meanings as used in this document.";



external boolean __________VALUES____________ = True;
//external_help = "All of the following external variables will be used
//in one or more boiler plates within this document.";

#include "qrl/2167A/2167A_global_externals.inc"


external string csci_name = "<CSCI NAME>";
external_help = "title/name of the CSCI to which the document applies";

external string csci_abbreviation = "<SAME CSCI ABBREVIATION>";
external_help = "The abbreviation for the CSCI title, if applicable, between
             parentheses";

// Number of the Computer Software Configuration Item 
external string csci_number = "<SAME CSCI NUMBER>";
external_help = "The number of the CSCI to which the document applies";

external string document_number = "<SAME DOCUMENT NUMBER>";
external_help = "identifies the document control number and date; also
	volume x of y if multi-volume";


//
//	Control for the document generation process

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

external boolean select_IRS_components = False;
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
    
