//////////////////////////////////////////////////////////////////////
//
//	SRS External Variables
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


// 1.1 Identification

external string SECTION_1_1__Identification = "<IDENTIFICATION>";
external_help = "10.1.3.1  Identification.  This paragraph shall be
numbered 1.1 and shall contain the approved identification number,
title, and if applicable, abbreviation of the CSCI and the system to
which this SRS applies.";


// 1.2 CSCI overview

external string SECTION_1_2__CSCI_overview = "<CSCI OVERVIEW>";
external_help = "10.1.3.2  CSCI overview.  This paragraph shall be 
numbered 1.2, shall briefly state the purpose of the system and 
shall identify and describe the role, within the system, of the 
CSCI to which this SRS applies.";


// 1.3 Document overview

external string SECTION_1_3__Document_overview = "<DOCUMENT OVERVIEW>";
external_help = "10.1.3.3  Document overview.  This paragraph 
shall be numbered 1.3 and summarize the purpose and contents of 
this document";


// 2.1 Government documents

external string SECTION_2_1__Government_documents = "<GOVERNMENT DOCUMENTS>";
external_help = "10.2.4.1  Government documents.  This paragraph shall
be numbered 2.1 and begin with one of the following two paragraphs:

1.  The following documents of the exact issue shown form a
part of this specification to the extent specified herein.  In
the event of conflict between the documents referenced herein
and the contents of this specification, the contents of this
specification shall be considered a superseding requirement.

2.  The following documents of the exact issue shown form a
part of this specification to the extent specified herein.  In
the event of conflict between the documents referenced herein
and the contents of this specificatino, the contents of this
specification shall be considered a superseding requirement,
except for specification (enter number of next higher tiered
specification) listed below.
            
The following paragraph shall appear at the conclusion of the
list of documents:
 
Copies of specifications, standards, drawings, and
publications required by suppliers in connection with
specified procurement functions should be obtained from the
contracting agency or as directed by the contracting
officer.

Government documents shall be listed by document number and title
in the following order:

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
   the assembly drawing.)

OTHER PUBLICATIONS:
   Manuals
   Regulations
   Handbooks
   Bulletins
   etc.";


// 2.2 Non-Government documents

external string SECTION_2_2__Non_Government_documents = 
	"<NON-GOVERNMENT DOCUMENTS>";
external_help = 
"10.2.4.2  Non-Government documents.  This paragraph shall be
numbered 2.2 and shall begin with the following
paragraph:  \"The following documents of the exact issue shown
form a part of this specification to the extent specified
herein.  In the event of conflict between the documents
referenced herein and the contents of this specification,
the contents of this specification shall be considered a
superseding requirement.\"  The source for all documents not
available through normal Government stocking activities shall
be listed.   

The following paragraph shall be placed at the bottom
of the list when applicable:  \"Technical society and
technical association specifications and standards are
generally available for reference from libraries.  They are
also distributed among technical groups and using Federal
Agencies.\"
             
Non-Government documents shall be listed by document number
and title in the following order:

SPECIFICATIONS:  
STANDARDS:  
DRAWINGS:  
OTHER PUBLICATIONS:";



// 3.1 External interface requirements

external string SECTION_3_1__External_interface_requirements =
	"<EXTERNAL INTERFACE REQUIREMENTS>";
external_help =
"This is an introductory paragraph for section 3.1 preceding
descriptions of external interfaces.  See the attached excerpt 
from the 2167A DID:

10.1.5.1  CSCI external interface requirements.  This paragraph shall be
numbered 3.1 and shall identify the external interfaces of the CSCI.
An external interface diagram may be used to aid in this description.  
Each external interface shall be identified by name and project_unique
identifier and a brief description of each interface shall be provided.
Any identifying documentation, such as an Interface Control Document or
Interface Requirements Specification, shall be referenced for each
interface.";


// 3.2 CSCI capability requirements

external string SECTION_3_2__CSCI_capability_requirements = 
	"<CSCI CAPABILITY REQUIREMENTS>";
external_help = 
"This is an introductory paragraph for section 3.2 preceding 
an in-depth discussion of capabilities addressed by this
CSCI.  See the attached excerpt from the 2167A DID:

10.1.5.2  CSCI capability requirements.  This paragraph shall be
numbered 3.2 and shall identify, in the subparagraphs that follow, all 
of the capability requirements that the CSCI must satisfy.  If the system
of which the CSCI is a part can exist in various system states and modes
as documented in the system specification, this paragraph shall identify 
each such state and mode and shall correlate each CSCI capability to
those states and modes.  A table may be used to depict this correlation.";


// 01/31/94 JHSIA: Changed external variable name to be consistent
// 3.3 CSCI internal interfaces

external string SECTION_3_3__CSCI_internal_interfaces =
	"<CSCI INTERNAL INTERFACES>";
external_help =
"This is an introductory paragraph for section 3.3 that procedes a 
list of internal interfaces and their description.  See the attached 
excerpt from the 2167A DID:

10.1.5.3  CSCI internal interfaces.  This paragraph shall be numbered 3.3
and shall identify the interfaces between the capabilities identified above.
Each internal interface shall be identified by name and project-unique
identifier and a brief description of each interface shall be provided,
including a summary of the information transmitted over the interface.
Internal interface diagrams depicting data flow, control flow, and
other relevant information may be used to aid in this description.";


// 3.4 CSCI data element requirements

external string SECTION_3_4__CSCI_data_element_requirements =
	"<CSCI DATA ELEMENT REQUIREMENTS>";
external_help =
"This is an introductory paragraph for section 3.4 that precedes 
data elements internal and external to this CSCI.  See the attached
excerpt from the 2167A DID:

10.1.5.4  CSCI data element requirements.  This paragraph shall be
numbered 3.4 and shall specify the information identified below, as
applicable.  

a.  For data elements internal to the CSCI:

	(1) Assign a project-unique identifier to the data element
	(2) Provide a brief description of the data element
	(3) Identify the units of measure required for the data element,
	    such as seconds, meters, kilohertz, etc.
	(4) Identify the limit/range of values required for the data 
  	    element ( for constants provide the actual value)
	(5) Identify the accuracy required for the data element.
	(6) Identify the precision or resolution required for the data
        element in terms of significant digits
	(7) For data elements of the CSCI's internal interfaces:
	    o identify the interface by name and project-unique identifier
	    o identify the source capability of the data element by name
		  and project-unique identifier
	    o identify the destination capability of the data element by
		  name and project-unique identifier.

b.  For data elements of the CSCI's external interfaces:

	(1) Identify the data elements by project-unique identifier
	(2) Identify the interface by name and project-unique identifier
    (3) Identify the source or destination capability, as applicable,
		by name and project_unique identifier.
	(4) Reference the Interface Requirements Specification in which
		the interface is specified.";


// 3.5.1 Installation dependent data

external string SECTION_3_5_1__Installation_dependent_data =
	"<INSTALLATION DEPENDENT DATA>";      
external_help =
"This is an introductory paragraph for section 3.5.1 that precedes
the installation dependent data table.  See the attached excerpt from
the 2167A DID:

10.1.5.5.1  Installation dependent data.  This subparagraph shall be 
numbered 3.5.1 and shall describe the site-unique data required by each
installation.  Examples of such data are: site latitude and longitude,
radar ranges and areas of coverage, and prescribed safety limits.  
In addition, the subparagraph shall identify the CSCI capabilities
in which these data are used.";


// 3.5.2 Operational parameters

external string SECTION_3_5_2__Operational_parameters =
	"<OPERATIONAL PARAMETERS>";
external_help =
"This is an introductory paragraph for section 3.5.2 that precedes the
operational parameters table. See the attached excerpt from the 2167A DID:

10.1.5.5.2  Operational parameters.  This subparagraph shall be numbered 
3.5.2 and shall describe parameters required by the CSCI that may vary
within a specified range according to operational needs.
Examples of such data are:  allowed trajectory deviations, navigation set
model number, airplane performance characteristics,
interaction/isolation of sorties, and missile performance
characteristics.  This subparagraph shall identify the
CSCI capabilities in which these data are used.";


// 3.6 Sizing and timing requirements

external string SECTION_3_6__Sizing_and_timing_requirements =
	"<SIZING AND TIMING REQUIREMENTS>";
external_help = 
"10.1.5.6  Sizing and timing requirements.  This subparagraph shall
be numbered 3.6 and specify the amount and if applicable, the location
of internal and auxiliary memory and the amount of processing time
allocated to the CSCI.  This paragraph shall specify the resources
required of both memory and the central processing unit (CPU) for the
CSCI.";


// 3.7 Safety requirements

external string SECTION_3_7__Safety_requirements =
	"<SAFETY REQUIREMENTS>";
external_help =
"10.1.5.7  Safety requirements.  This subparagraph shall be numbered 
3.7 and shall specify safety requirements that are applicable to the
design of the CSCI, with respect to potential hazards to personnel,
property, and the physical environment.";


// 3.8 Security requirements

external string SECTION_3_8__Security_requirements =
	"<SECURITY REQUIREMENTS>";
external_help =
"10.1.5.8  Security requirements.  This subparagraph shall be numbered 
3.8 and shall specify security requirements that are applicable to the
design of the CSCI, with respect to potential compromise of sensitive
data.";


// 3.9 Design constraints

external string SECTION_3_9__Design_constraints =
	"<DESIGN CONSTRAINTS>";
external_help =
"10.1.5.9  Design constraints.  This subparagraph shall be numbered
3.9 and shall specify other requirements that contrain the CSCI design
such as the use of a particular processing configuration, etc.";


// 3.10 Software quality factors

external string SECTION_3_10__Software_quality_factors =
	"<SOFTWARE QUALITY FACTORS>";
external_help =
"10.1.5.10  Software quality factors.  This subparagraph shall be
numbered 3.10 and shall be divided into subparagraphs, as appropriate,
to specify each software quality factor identified in the contract or
derived from a higher level specification.  For each quality factor
required, the method of compliance shall be specified along with the
requirements for that factor.";


// 3.11 Human performance and human engineering requirements

external string 
	SECTION_3_11__Human_performance_and_human_engineering_requirements =
	"<HUMAN PERFORMANCE AND HUMAN ENGINEERING REQUIREMENTS>";
external_help =
"10.1.5.11  Human performance/human engineering requirements.  This 
subparagraph shall be numbered 3.11 and shall specify the applicable
human factors engineering requirements for the CSCI.  These
requirements shall include, as applicable, considerations for:
	
a.  Human information processing capabilities and limitations 
b.  Foreseeable human errors under both normal and extreme conditions
c.  Implications for the total system environment (including
    training, support, and operational environment).";


// 3.12 Requirements traceability

external string SECTION_3_12__Requirements_traceability =
	"<REQUIREMENTS TRACEABILITY>";
external_help =
"This is an introductory paragraph for section 3.12 that precedes
the requirements traceability table.  See the attached excerpt from
the 2167A DID:

10.1.5.12 Requirements traceability. This paragraph shall be numbered
3.12 and shall contain a mapping of the engineering requirements 
in this specification to the requirements applicable to this CSCI in 
the SSS, PIDS, or CIDS.  This paragraph shall also provide a mapping of
the allocation of the CSCI requirements from the SSS, PIDS, or CIDS to
the engineering requirements in this specification.";


// 4.1 Qualification methods

external string SECTION_4_1__Qualification_methods =
	"<QUALIFICATION METHODS>";
external_help =
"This is an introductory paragraph for section 4.1 that precedes
the qualification methods table.  See the attached excerpt from
the 2167A DID:

10.1.6 Qualification methods.  This paragraph shall be numbered
4.1 and shall specify the qualificaton methods to be used to 
ensure that the CSCI requirements of section 3 and 5 have
been satisfied.  Qualification methods include:

	a.  Demonstration - The operation of the CSCI ( or some part of
the CSCI) that relies on observable functional operation not
requiring the use of elaborate instrumentation or special
test equipment.

    b.  Analysis - The processing of accumulated data accumulated
from other qualification methods.  Examples are 
interpretation or extrapolation of test data.

    c.  Inspection - The visual examination of CSCI code, 
documentation, etc.";


// 4.2 Special qualification requirements

external string SECTION_4_2__Special_qualification_requirements =
	"<SPECIAL QUALIFICATION REQUIREMENTS>";
external_help =
"This is an introductory paragraph for section 4.2 that precedes the
special qualification requirements table.  See the attached excerpt
from the 2167A DID:

10.1.6.2  Special qualification requirements.  This paragraph
shall be numbered 4.2 and shall be divided into appropriate
subparagraphs to specify special requirements associated with 
qualification of the CSCI.  This paragraph shall identify and
describe, if applicable, special tools, techniques (e.g., test
formulas, algorithms), procedures, facilities, and acceptance limits.
For each special test the following information shall be specified:
	
a.  A project-unique identifier
b.  The paragraph number(s) of the capability requirement(s) to
    which the test applies.
c.  A description of the test, such as peak-load stress test
    for 24 hr. duration
d.  The level of the test (CSU, CSC, CSCI, segment, or system level).";


// 5. Preparation for delivery

external string SECTION_5__Preparation_for_delivery =
	"<PREPARATION FOR DELIVERY>";
external_help =
"10.1.7  Preparation for delivery.  This section shall be numbered 5.
and specify the type and characteristics of the delivery media for the
CSCI (e.g. 8-track magnetic tape 1600 bpi, 150 megabyte disk).  In
addition, this section shall specify the labeling, packaging, handling,
and classification marking requirements for the media, including the
CSCI name and project unique identifier.  Any unique delivery
requirements shall also be specified in this section.";


// 6. Notes

external string SECTION_6__Notes =
	"<NOTES>";
external_help =
"10.1.8  Notes.  This section shall be numbered 6 and shall contain
any general information that aids in understanding this specification
(e.g., background information, glossary, formula derivations).";


// 6. Abbreviations

external string SECTION_6__Abbreviations =
	"<ABBREVIATIONS>";
external_help =
"This section shall include an alphabetical listing of all acronyms,
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


external string SECTION_3_5_1__Installation_dependent_data_table_filename = "";
external_help =
"This is the name of the Table Editor File that contains a user enterred
table.  To define this table, bring up the Table Editor and store as the 
filename of your choice.  Then enter the chosen name in this field.

10.1.5.5.1  Installation dependent data.  This subparagraph shall be 
numbered 3.5.1 and shall describe the site-unique data required by each
installation.  Examples of such data are: site latitude and longitude,
radar ranges and areas of coverage, and prescribed safety limits.  
In addition, the subparagraph shall identify the CSCI capabilities
in which these data are used.";

external string SECTION_3_5_2__Operational_parameters_data_table_filename = "";
external_help =
"This is the name of the Table Editor File that contains a user enterred
table.  To define this table, bring up the Table Editor and store as the 
filename of your choice.  Then enter the chosen name in this field.

10.1.5.5.2  Operational parameters.  This subparagraph shall be numbered 
3.5.2 and shall describe parameters required by the CSCI that may vary
within a specified range according to operational needs.
Examples of such data are:  allowed trajectory deviations, navigation set
model number, airplane performance characteristics,
interaction/isolation of sorties, and missile performance
characteristics.  This subparagraph shall identify the
CSCI capabilities in which these data are used.";


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

//PEC mod 9/28/95 changed help text to refer to "Generating 2167A Reports"
//instead of "Printing Manual."
external boolean select_SRS_components = False;
external_help = 
"This setting determines how objects (data flows, processes, ...) are
selected for inclusion in this document.  If this variable is set to:

   True:  Only included if they are designated with the SRS document 
   		  annotation (see 'Generating 2167A Reports' Manual).
   False: The SRS document annotation will not be used.";

external boolean use_StP_generated_identifiers = True;
external_help = 
"The variable controls the source of identifiers used for interfaces, 
capabilities and other items.  If this variable is set to:

    True:  From StP Repository generated object identifiers.
    False: From annotations filled in by the user.";
    
external boolean include_Real_Time_Analysis = False;
external_help =
"This variable controls the inclusion of Real Time Analysis components
as defined by Hatley/Pirbhai.  StP/SE's support of Hatley/Pirbhai
includes state transition diagrams, control specification tables and
matrices (i.e. event logic table, action logic table, state transition
table, state transition matrix, process activation table, process 
activation matrix, and decision table).  If this variable is set to:

	True:  Real Time components will be included in either section 3.2.x.y
		   when capabilities are described or in an appendix (narrative or
		   reference respectively).
	False: No Real Time components will be included.";

// 01/17/94 JHSIA: Added control to handle pspec gen
external boolean use_Process_Specification_Generator = False;
external_help =
"The process specification report generated (Generate Pspec) from the
Data Flow Editor and from the StP DeskTop contains user entered
information (i.e. annotation - pspec note description) as well as
template generated information (i.e. repository, system, and user
information).  This variable controls the way the process specification
is included in section 3.2.x.y of the SRS.  The setting of this
variable will determine if just the pspec note description is included
or if the template generated information will also be included.  If
this variable is set to:

	True:  The same format and information used by the Generate Pspec
	       command will be used for Pspecs in section 3.2.x.y.
	False: Only the pspec annotation will be used for Pspecs in 
	       section 3.2.x.y.";
