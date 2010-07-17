//
//      StP/UML
//      Confidential property of Aonix
//      Copyright (c) 1997
//      All rights reserved


//
//  File for UML C++ code generation -- string constants
//

const string    SD = "::";             // scope delimiter
const string    SD_REPLACEMENT = "_";  // SD replacement in file/identifer names


//
//  Tags for incremental code generation
//
//  These tags are used for determining the protected regions during
//  incremental code generation.
//  They must be unique strings, that is, extremely unlikely to match
//  any other string that may occur in a source file.
//  The spaces at the end of certain strings are required to allow for
//  an ID to follow.
//
const string    START_CLASS_DECLARATION_TAG = "// stp class declarations";
const string    END_CLASS_DECLARATION_TAG = "// end stp class declarations";
const string    START_CLASS_DEFINITION_TAG = "// stp class definition ";
const string    END_CLASS_DEFINITION_TAG = "// end stp class definition";
const string    START_CLASS_BODY_TAG = "// stp class members";
const string    END_CLASS_BODY_TAG = "// end stp class members";
const string    START_CLASS_SECOND_BODY_TAG = "// stp more class members";
const string    END_CLASS_SECOND_BODY_TAG = "// end stp more class members";
const string    START_IMPLEMENTATION_DECLS_TAG = "// stp attributes ";
const string    END_IMPLEMENTATION_DECLS_TAG = "// end stp attributes";
const string    START_OPERATION_TAG = "// stp operation ";
const string    END_OPERATION_TAG = "// end stp operation";
const string    START_OP_IMPLEMENTATION_TAG = "// stp code";
const string    END_OP_IMPLEMENTATION_TAG = "// end stp code";
const string    START_FOOTER_TAG = "// stp footer";
const string    END_FOOTER_TAG = "// end stp footer";
const string    START_INCLUDES_TAG = "// stp includes ";
const string    END_INCLUDES_TAG = "// end stp includes";
const string    START_CLASS_IMPLEMENTATION_TAG = "// stp class implementation ";
const string    END_CLASS_IMPLEMENTATION_TAG = "// end stp class implementation";
const string    START_CLASS_DERIVATIVES_TAG = "// stp class derivatives ";
const string    END_CLASS_DERIVATIVES_TAG = "// end stp class derivatives";
const string    START_EXTRA_CLASS_DEFINITION_TAG = "// stp extra class definition ";
const string    END_EXTRA_CLASS_DEFINITION_TAG = "// end stp extra class definition ";
const string    START_EXTRA_CLASS_IMPLEMENTATION_TAG = "// stp extra class implementation ";
const string    END_EXTRA_CLASS_IMPLEMENTATION_TAG = "// end stp extra class implementation";




