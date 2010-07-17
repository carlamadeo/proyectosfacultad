//*****************************************************************************
// DESCRIPTION: UML_Externals_E.inc
//              ----------------------
//              (c) 1997 by AONIX
//*****************************************************************************
//
// Include file for the UML-Report with the definition of all english externals
// and their mapping to the globals used in the language independent parts.
//*****************************************************************************
//
// functions: - void   MapExternals()
//*****************************************************************************

script_help = "UML_Report_E generates an UML-Report, consisting of several chapters of your choice, like Requirements, Users View, logical and physical Systemarchitecture,  Package Report, Class Report, Stereotype Definition and a Glossary. 

The external variables in the right section of the script manager window allow You, to influence the layout and content of the generated report.
If You want to limit the evaluation to special files or classes, please use the following syntax: 
<file1> <file2> <file3> ...., 
using Blank as delimitor and leaving out file extensions.  
You can also use the wildcardsymbol \"*\", like Appl*, to evaluate only files or classes starting with \"Appl\".
The default is always \"*\", i.e. all Symbols or files of the given filetype.

Please use \"Delete External Value\" to reset external variables to their default value !

This report uses the format file \"stpreport\" or any other format file based on this default. Supported target formats are Framemaker MIF and  Word for Windows RTF.";


//*****************************************************************************
// EXTERNAL VARIABLES
//*****************************************************************************

external string DOCUMENT_INFORMATION__________ = "Not evaluated"; 
external_help = "-------------------";
     
external string __Company_Name = "Aonix";
external_help = "The company name is shown on the frontpage.
The default is \"Aonix\". You can replace it by any other string. 
If you want to use your company logo in the page headers, simply replace the Aonix logo in your format files.";
     
external string __Division_Name = "Development";
external_help = "The division name is shown on the frontpage.
The default is \"Development\". You can replace it by any other string.";
     
external string __Document_Author = "Customization Team";
external_help = "The author of the document is shown on the page footers.
The default is \"Customization Team\". You can replace it by any other string.";

external string __Document_Title = "UML-Report";
external_help = "The document title is shown on the frontpage and the page headers.
The default is \"UML-Report \". You can replace it by any other string.";

external string __Document_Subtitle = "for <StP-system>";
external_help = "The document subtitle is shown on the frontpage and the page headers below the document title.
The default is \"for <StP-system>\", where <StP-system> is replaced by the name of your StP-system . 
If you change this value, no substitution is possible and you will see only the text you entered.";

external string __Document_Version = "V1.0";
external_help = "The version of the document is shown on the frontpage and the page footers.
The default is \"V1.0\". You can replace it by any other string.";
     
external string __Document_Status = "Analysis";
external_help = "The status of the document is shown on the frontpage.
The default is \"Analysis\" and can be replaced by any other string like \"Design\", \"Draft\" or \"Review\".";


external string DOCUMENT_LAYOUT_______________ = "Not evaluated"; 
external_help = "-------------------";
     
external string __Default_Format_File = "stpreport";
external_help = "Name of the format file, used for the document generation.
The default is \"stpreport\". You can replace it by the name of any other existing format file (for the chosen target format). If your format file contains other paragraph and character formats than stpreport, you have to adjust theit names in the template file ALL_Formats.inc. 

Please make sure that your chosen format file is accessable, by checking that it is shown in the script managers choice of format files. 

The Report automaticaly adds the extension (.mif or .rtf) matching your chosen target format to the name of the format file, i.e. if you select Framemaker as a target in the script manager dialogue, the format file \"stpreport.mif\" is used.";
     
external string __Target_Language = "E";
external_help = "Language of the generated document. This report can generate english (\"E\"), german (\"G\") and french (\"F\") documents.";

external boolean __Generate_Single_File_For_HTML = False;
external_help = "Specifies, when generating HTML, whether to generate the report into a single file, or to break it up into muliple files.  The multiple files will correspond to the pages which are created when generating for other publishing targets.";

external boolean __Optimized_Diagram_Printing = True;
external_help  = "Use optimized diagram printing ? (Otherwise every diagram is printed on its own page)";

external string __Use_Print_Setting = "";
external_help  = "If you want to use your own print setting for diagrams and tables (use one name for all settings), you can enter the name here";

external string __Suppress_Displaymarks = "UseCaseScenariosExist ClassTableExists AttributeIsDerived AttributeTypeAndValue AttributeVisibility AttrIsClass ClassInfo ClassParameters ClassScoping Navigability OperationReturnType OperationVisibility OpIsClass RefinedStateExists RefinementArguments StateTableExists Visibility";
external_help  = "You can redifine the default set of suppressed displaymarks. Please use a blank as delimiter between the single displaymark names to be suppressed.";

external boolean __Order_Symbols_Alphabetically = False;
external_help  = "Prints symbols in alphabetical order (True) or according to their vertical (diagram) or horizontal (table) position ?";

external boolean __Include_Introductions = True ; 
external_help = "Print an introducing paragraph at the begining of each chapter (the textstrings are predefined in UML_Text.inc) ?";

external boolean __Include_Descriptions = True ; 
external_help = "Print the diagram- and symbol descriptions (entered via OAE) ?";

external boolean ____Notify_Missing_Descriptions = True; 
external_help = "Print remarks for missing diagram and symbol descriptions ?";

external boolean ____Print_Multiple_Descriptions = False; 
external_help = "Print descriptions for every occurence of a symbol (package, use case, ...) in each chapter or only at the first time ?";

external boolean __Include_Generation_Information = False ; 
external_help = "Print author and date of generation (entered via OAE) for use cases, classes and diagrams ?";


external string DOCUMENT_CHAPTERS_____________ = "Not evaluated"; 
external_help = "-------------------";

external boolean __Generate_the_Requirement_Chapter = True;
external_help = "Generate the requirements chapter ?";

external boolean ____In_Requirement_Table_Form = False;
external_help = "Print requirements as shown in the requirement table or in form of structured text ?
Choose the table form only if your requirement tables have less than 4 refinement levels, otherwise the columns are too small to fit on the page.";

external boolean ____Show_Unallocated_Requirements = False;
external_help = "Mark unallocated requirements by a questionmark ?
This option works only for structured text output !";

external boolean ____Include_Superseded_Requirements = False;
external_help = "Print superseded requirements ?";

external boolean __Generate_the_Users_View_Chapter = True;
external_help = "Generate the chapter with the users view (Use Cases with connected Sequence-, Collaboration- and Activity-Diagrams) ?";

external boolean ____With_detailed_Use_Case_Description = False;
external_help  = "Print detailed information about the symbols in the use case diagrams (and scenarios) ?";

external boolean ____Include_Use_Case_Scenarios = True;
external_help = "Print Scenarios (Sequence, Collaboration- and Activity-Diagrams) for every Use Case ?";

external boolean __Generate_the_System_Architecture_Chapter = True;
external_help = "Generate the chapter with information about logical and physical components of the system ?";

external boolean ____Include_logical_Architecture = True;
external_help = "Generate a subchapter with the package structure ?";

external boolean ____Include_physical_Architecture = True;
external_help = "Generate a subchapter with component and deployment information ?";

external boolean ______With_detailed_Architecture_Description = False;
external_help = "Print detailed information about the modules and processes ?";

external boolean __Generate_the_Package_Report_Chapter = True;
external_help = "Generate the package report chapter ?";

external boolean ____With_detailed_Package_Description = False;
external_help = "Print detailed information about the packages ? 
Use False in the start phase of the project.";

external boolean ____Include_Package_and_Class_Diagrams = True;
external_help = "Print package- and class diagrams ?";

external boolean __Generate_the_Class_Report_Chapter = True;
external_help = "Generate the class report chapter ?";

external boolean ____With_detailed_Class_Description = False;
external_help = "Print detailed information about the classes ? 
Use False in the start phase of the project.";

external boolean ____Include_Attributes_and_Operations = True;
external_help = "Print information about attributes and operations of the class ?";

external boolean ______In_Class_Table_Form = False;
external_help = "Print attributes and operations as shown in the classtable or as structured text with more detailed description ?";

external boolean ______With_Analysis_Information = False;
external_help = "Print existing analysis information for attributes and operations ?";

external boolean ______With_Cxx_Information = False;
external_help = "Print existing C++ information for attributes and operations ?";

external boolean ______With_Java_Information = False;
external_help = "Print existing Java information for attributes and operations ?";

external boolean ______With_IDL_Information = False;
external_help = "Print existing IDL information for attributes and operations ?";

external boolean ______With_Ada95_Information = False;
external_help = "Print existing Ada95 information for attributes and operations ?";

external boolean ____Include_Interaction_Diagrams = True;
external_help = "Print sequence- and collaboration diagrams with references of the class?";

external boolean ____Include_Behavior_Diagrams = True;
external_help = "Print state- and activity diagrams scoped to the class?";

external boolean ____Include_the_Association_Summary_Table = True;
external_help = "Generate the association summary table which lists class relationships, multiplicity and descriptions ?";

external boolean __Generate_the_Stereotype_Definition_Chapter = True;
external_help = "Generate the chapter with stereotype definitions ?";

external boolean ____Include_Stereotype_Diagrams = False;
external_help = "Print Stereotype definition diagrams at the beginning of the chapter ?";

external boolean __Generate_Glossary_Chapter = False;
external_help = "Generate a glossary at the end of the report ?";

external boolean ____For_Scope_of_Report_only = False;
external_help = "Reduce the glossary to items defined for diagrams and symbols mentioned in the report ?";

external string LIMIT_EVALUATION_______________ = "Not evaluated"; 
external_help = "-------------------";

external string __To_Scope_Of_Prefix = "*";
external_help = "Limit the whole report to files and modeling elements matching the given prefix. (See \"Help on Script\" for syntax)";

external string __To_Packages = "*";
external_help = "Limit the report to files containing and modeling elements contained in the given packages. (See \"Help on Script\" for syntax)";

external string __To_Classes = "*";
external_help = "Limit the class report chapter to the given list of classes. (See \"Help on Script\" for syntax)";

external string __To_Stereotypes = "*";
external_help = "Limit the stereotype definition chapter to the given stereotypes. (See \"Help on Script\" for syntax)";

external string __To_Requirement_Tables = "*";
external_help = "Limit the requirement chapter to the given list of requirement table files. (See \"Help on Script\" for syntax).";

external string __To_Use_Case_Diagrams = "*";
external_help = "Limit the users view chapter to the given list of use case diagram files. (See \"Help on Script\" for syntax)";

external string __To_Implementation_Diagrams = "*";
external_help = "Limit the physical system architecture chapter to the given list of component and deployment diagram files. (See \"Help on Script\" for syntax)";

 
//*****************************************************************************
// MapExternals: This function maps all the english externals to their language
//               independent globaly defined equivalents
//*****************************************************************************
void MapExternals()
{
    // Document Information Section
    e_COMPANY              = __Company_Name;
    e_DIVISION             = __Division_Name;
    e_DOC_AUTHOR           = __Document_Author;
    e_DOC_TITLE            = __Document_Title;
    e_DOC_SUBTITLE         = __Document_Subtitle;
    e_DOC_VERSION          = __Document_Version;
    e_DOC_STATUS           = __Document_Status;

    // Document Layout Section
    e_DEFAULT_FORMAT       = __Default_Format_File;
    e_DOCUMENT_LANGUAGE    = to_upper(__Target_Language);
    e_GEN_SINGLE_HTML_FILE = __Generate_Single_File_For_HTML;
    e_OPTIMIZED_PRINTING   = __Optimized_Diagram_Printing;
    e_PRINT_SETTING        = __Use_Print_Setting;
    e_NO_DISPLAYMARKS      = StripInfo(__Suppress_Displaymarks);
    e_PRINT_ALPHA_ORDER    = __Order_Symbols_Alphabetically;
    e_GEN_INTRODUCTIONS    = __Include_Introductions;
    e_GEN_DESCRIPTIONS     = __Include_Descriptions;
    e_MISSING_DESCRIPTION  = ____Notify_Missing_Descriptions;
    e_MULTIPLE_DESCRIPTION = ____Print_Multiple_Descriptions;
    e_GENERATION_INFO      = __Include_Generation_Information;

    if (!e_GEN_DESCRIPTIONS)
       e_MISSING_DESCRIPTION = False;

    // Document Chapter Section

    e_GEN_REQUIREMENTS      = __Generate_the_Requirement_Chapter;
    e_IN_RQ_TABLE_FORM      = ____In_Requirement_Table_Form;
    e_SHOW_UNALLOCATED      = ____Show_Unallocated_Requirements;
    e_SUPERSEDED_REQ        = ____Include_Superseded_Requirements;

    e_GEN_USERS_VIEW        = __Generate_the_Users_View_Chapter;
    e_UV_DETAILS            = ____With_detailed_Use_Case_Description;
    e_SCENARIOS             = ____Include_Use_Case_Scenarios;

    e_GEN_SYS_ARCH          = __Generate_the_System_Architecture_Chapter;
    e_LOG_ARCH              = ____Include_logical_Architecture;
    e_PHYS_ARCH             = ____Include_physical_Architecture;
    e_IMPL_DETAILS          = ______With_detailed_Architecture_Description;

    if (!e_LOG_ARCH && !e_PHYS_ARCH)
       e_GEN_SYS_ARCH = False;

    e_GEN_PACKAGE_REPORT    = __Generate_the_Package_Report_Chapter;
    e_PACK_DETAILS          = ____With_detailed_Package_Description;
    e_PACK_DIAGRAMS         = ____Include_Package_and_Class_Diagrams;

    e_GEN_CLASS_REPORT      = __Generate_the_Class_Report_Chapter;
    e_CLASS_DETAILS         = ____With_detailed_Class_Description;
    e_CL_ATT_OP             = ____Include_Attributes_and_Operations;
    e_IN_CL_TABLE_FORM      = ______In_Class_Table_Form;
    e_WITH_ANALYSIS_INFO    = ______With_Analysis_Information;
    e_WITH_CXX_INFO         = ______With_Cxx_Information;
    e_WITH_JAVA_INFO        = ______With_Java_Information;
    e_WITH_IDL_INFO         = ______With_IDL_Information;
    e_WITH_ADA_INFO         = ______With_Ada95_Information;
    e_CL_INTERACTION        = ____Include_Interaction_Diagrams;
    e_CL_BEHAVIOR           = ____Include_Behavior_Diagrams;
    e_GEN_ASSOC_SUM_TBL	    = ____Include_the_Association_Summary_Table;

    e_GEN_STEREOTYPES       = __Generate_the_Stereotype_Definition_Chapter;
    e_STEREOTYPE_FILES      = ____Include_Stereotype_Diagrams;

    e_GEN_GLOSSARY          = __Generate_Glossary_Chapter;
    e_REFD_GLOSSARY         = ____For_Scope_of_Report_only;

    // Reduce Evaluation Section
    e_PREFIX              = __To_Scope_Of_Prefix;
    e_PACKAGES            = __To_Packages;
    e_CLASSES             = __To_Classes;
    e_STEREOTYPES         = __To_Stereotypes;
    e_RQTBL_FILES         = __To_Requirement_Tables;
    e_USECASE_FILES       = __To_Use_Case_Diagrams;
    e_IMPL_FILES          = __To_Implementation_Diagrams;

    return;
}
