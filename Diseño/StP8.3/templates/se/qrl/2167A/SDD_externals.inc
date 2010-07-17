//////////////////////////////////////////////////////////////////////
//
//	SDD External Variables
//	
//
//	This file defines all the externals variables necessary to 
//	support the SRS.  This file is divided into 3 groups: 
//
//		1. paragraphs & subsections
//		2. boiler plates
//		3. control
//
//	Each group begins with a variable used as a label in the
//	ScriptMan interface.  This is provided strictly for aesthetic
//	purposes.
//
//
//	1. Paragraph and Subsection
//
//	This group holds all external variables that would typically
//	hold text used for specific paragraphs or subsections.  The
//	values held in these variables usually are multi-line.  The
//	variables should be named:
//		
//		SECTION_<section#>_<title>
//
//	Unfortunately, no periods are allowed in vaiable labels so
//	underscores will have to do.  The external variables should
//	be defined in the order the section numbers appear in the
//	document as this is the order ScriptMan presents the variables.
//
//
//	2. Boiler Plates
//
//	These external variables are used in various boiler plates
//	through out the document (e.g. title page).  These are typically
//	one word or one phrase values.
//
//
//	3. Additional Diagrams & Tables
//
//	Some aspects of the document require diagrams and/or tables yet
//	they do not fall out of the analysis or design methodology.  These
//	diagrams and/or tables will have to be enterred in manually in an
//	editor provided by IDE.  The name and/or location of the specific
//	diagram or table will be enterred in the external variables in this
//	group.
//	
//
//	4. Control
//
//	These variables control the behaviour of the script thereby
//	controlling the final document generated.  These are typically
//	boolean variables
//
//////////////////////////////////////////////////////////////////////


//
//	Paragraph and Subsection contents
//


// Header

external boolean _____SECTIONS_________________________  = False;
external_help = "All the external variables that begin with SECTION_
will be used as an introductory paragraph.  The name should reflect
both the section number as well as the title of the respective 
section.";

// 1.2 System overview.

external string SECTION_1_2__System_overview = "<SYSTEM OVERVIEW>";
external_help =
"10.1.3.2 System Overview.  This paragraph shall be numbered 1.2,
and shall briefly state the purpose of the system and the CSCI
to which this SDD applies.";


// 1.3 Document overview

external string SECTION_1_3__Document_overview = "<DOCUMENT OVERVIEW>";
external_help =
"10.1.3.3 Document Overview.  This paragraph shall be numbered 1.3 and 
shall summarize the purpose and contents of this document.";


// 2.0 Referenced Documents

external string SECTION_2__Referenced_documents = "<REFERENCED DOCUMENTS>";
external_help =
"10.1.4 Referenced documents.  This section shall be numbered 2.  and
list by document number and title all documents referenced in the SDD.
This section shall also identify the source for all documents not
available through normal government stocking activities.";
                
// 8. Notes

external string SECTION_8__Notes =
	"<NOTES>";
external_help =
"10.1.10  Notes.  This section shall be numbered 8 and shall contain
any general information that aids in understanding this specification
(e.g., background information, glossary, formula derivations).";


// 8. Abbreviations

external string SECTION_8__Abbreviations =
	"<ABBREVIATIONS>";
external_help =
"10.1.10 Notes. ...
This section shall include an alphabetical listing of all acronyms,
abbreviations, and their meanings as used in this document.";



//
//	Boiler Plate values

external boolean _____VALUES___________________________ = False;
external_help = "All of the following external variables will be used 
in one or more boiler plates within this document.";

#include "qrl/2167A/2167A_global_externals.inc"

external string csci_name = "<CSCI NAME>";
external_help = "title/name of the CSCI to which the document applies";

external string csci_abbreviation = "";
external_help = "The abbreviation for the CSCI title/name, if applicable";

external string csci_number = "<CSCI NUMBER>";
external_help = "The number of the CSCI to which the document applies";

external string document_number = "<DOCUMENT NUMBER>";
external_help = "The identifying document control number, if applicable
and the date.  Also specify Volume x of y if necessary.";


//
//	Additional diagrams and tables

external boolean _____DIAGRAMS_AND_TABLES______________ = False;
external_help = "All of the following external variables will be used
to locate additional diagrams and tables needed in the generation of 
this document.";


//
//	Control for the document generation process

external boolean _____CONTROL__________________________ = False;
external_help = "All of the following external variables will be used
to control the generation of this document.";

external boolean narrative_document = False;
external_help = 
"This setting controls the narrative vs reference nature of this
document.  If this variable is set to:

    False: Most diagrams and tables will be found in appendices
    True:  Most diagrams and tables will be found in the body of the 
    	   document where they are first referenced.";

external boolean select_SDD_components = False;
external_help = 
"This setting determines how objects (data flows, processes, ...) are
selected for inclusion in this document.  If this variable is set to:

   True:  Only included if they are designated with the SDD document 
   		  annotation (see Printing Manual).
   False: The SDD document annotation will not be used.";

external boolean use_StP_generated_identifiers = True;
external_help = 
"The variable controls the source of identifiers used for interfaces, 
capabilities and other items.  If this variable is set to:

    True:  From StP Repository generated object identifiers.
    False: From annotations filled in by the user.";
    
external string use_PDL_generator = "";
external_help = 
"This variable controls the method by which PDLs are included in
section 4.x.y.z of the SDD.  PDLs can be formalized reports that
contains repository generated information in addition to user entered
description.  This is the case with the 'Generate PDL' command from the
Structure Chart Editor and the StP DeskTop.  If this variable (case
insensitive) is set to:

''		
	- set to empty string
	- default if not filled or if unexpected value
	- Only user entered description will be included
	
'c'	
	- anything that starts with the letter C
	- A C style code frame will be placed around the user entered 
	  description
	  
'Ada'
	- An Ada style code frame will be placed around the user entered 
	  description
	  
'Generic'
	- A generic, language independent code frame will be placed around
	  the user entered description";
	  
