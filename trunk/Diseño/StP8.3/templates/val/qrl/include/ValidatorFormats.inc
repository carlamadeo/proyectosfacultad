
external string  e_DEFAULT_FORMAT = "";
external boolean e_FORMAT_TESTCASE_DATA = True;

external string  e_DOCAUTHOR   = " ";
external string  e_COMPANY     = " ";
external string  e_DIVISION    = " ";
external string  e_DOC_VERSION = "1.0";
external string  e_DOC_STATUS  = "Analysis";
external string  e_REPORT_FILE = " ";
external string  e_REPORT_TYPE = "rtf";

#include "qrl/include/ALL_Formats.inc"
#include "val/qrl/include/tdrvMessaging.inc"

const int  IS_OK    = 0;

// GenerateTraceabilityReport.qrl
string gsValFormat_TableTitle;  // "TableTitle"
string gsValFormat_CellHeading; // "CellHeading" 
string gsValFormat_CellBody;    // "CellBody"

// GenerateOneRequrementsDocumentForUML.qrl
string gsValFormat_Title;
string gsValFormat_BTitle;
string gsValFormat_BSubTitle;
string gsValFormat_Sec1;
string gsValFormat_Sec2;
string gsValFormat_Sec3;
string gsValFormat_Sec4;
string gsValFormat_Values;
string gsValFormat_description;

// GenerateOneTestSpecificationDocumentForUML(OMT).qrl
string gsValFormat_EO;
string gsValFormat_EO1;
string gsValFormat_EVAL;
string gsValFormat_EVAL1;
string gsValFormat_EXER;
string gsValFormat_P1;
// string gsValFormat_P3;
string gsValFormat_testmetrics;
string gsValFormat_testmetricsB;

string gsValFormat_P2;
string gsValFormat_P3;
string gsValFormat_P4;
string gsValFormat_P5;
string gsValFormat_P6;
string gsValFormat_BP6;

// ========= add ons to ALL_Formats.inc
string TABLETITLE;
// ========= end add ons 

void initReport()
{
    
    SetTargetFormat();
    TABLETITLE = HEAD1; // "TableTitle";

    gsValFormat_TableTitle = TABLETITLE;
    gsValFormat_CellHeading = CELLHEAD;
    gsValFormat_CellBody = CELLBODY;

    gsValFormat_Title = TITLE;     
    gsValFormat_BTitle = TITLE;    
    gsValFormat_BSubTitle = SUBTITLE; 
    gsValFormat_Sec1 = HEAD1;
    gsValFormat_Sec2 = HEAD2;      
    gsValFormat_Sec3 = HEAD3; 
    gsValFormat_Sec4 = HEAD4;
    gsValFormat_Values = CELLHEAD;
    gsValFormat_description = BODY_NORMAL;

    gsValFormat_EO = BODY_NORMAL;
    gsValFormat_EO1 = BODY_NORMAL;
    gsValFormat_EVAL = BODY_NORMAL;
    gsValFormat_EVAL1 = BODY_NORMAL;
    gsValFormat_EXER = BODY_NORMAL;
    gsValFormat_P1 = BODY_NORMAL;
    // gsValFormat_P3 = ;
    gsValFormat_testmetrics = BODY_NORMAL;
    gsValFormat_testmetricsB = BODY_BOLD;

    gsValFormat_P2 = BODY_NORMAL; 
    gsValFormat_P3 = BODY_NORMAL; 
    gsValFormat_P4 = BODY_NORMAL; 
    gsValFormat_P6 = BODY_NORMAL;
    gsValFormat_P5 = BODY_NORMAL;
    gsValFormat_BP6 = BODY_BOLD;

}

string getTargetOption()
{

    if (e_REPORT_TYPE == "html") // HTML
        return  " -t html ";

    if (e_REPORT_TYPE == "mif") // FrameMaker
        return  " -t mif ";
    
    if (e_REPORT_TYPE == "rtf") // MSWord
        return  " -t rtf ";

    if (e_REPORT_TYPE == "leaf") // Inteleaf
        return  " -t leaf ";

    return "";

}

string getFormatExtention()
{

    if (e_REPORT_TYPE == "html") // HTML
        return  ".css";

    if (e_REPORT_TYPE == "mif") // FrameMaker
        return  ".mif";
    
    if (e_REPORT_TYPE == "rtf") // MSWord
        return  ".rtf";

    if (e_REPORT_TYPE == "leaf") // Inteleaf
        return  ".doc";

    return "";

}

string getReportExtention()
{

    if (e_REPORT_TYPE == "html") // HTML
        return ".html";

    if (e_REPORT_TYPE == "mif") // FrameMaker
        return ".mif";
    
    if (e_REPORT_TYPE == "rtf") // MSWord
        return ".rtf";

    if (e_REPORT_TYPE == "leaf") // Inteleaf
        return ".doc";
    
    return "";

}

string getFormatFileName()
{
    //if( string_find(e_DEFAULT_FORMAT, 0, ".") == string_length(e_DEFAULT_FORMAT))
    //    return e_DEFAULT_FORMAT + getFormatExtention();
    return e_DEFAULT_FORMAT;
}

