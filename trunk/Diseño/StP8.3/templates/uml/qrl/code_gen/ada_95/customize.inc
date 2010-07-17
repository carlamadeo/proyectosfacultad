//
//  Affixes for Ada identifiers based on class name
//

const string  PACKAGE_NAME_PREFIX		  = "";
const string  PACKAGE_NAME_SUFFIX                 = "";
const string  CLASS_TYPE_NAME_PREFIX              = "";
const string  CLASS_TYPE_NAME_SUFFIX              = "";
const string  ACCESS_TYPE_NAME_PREFIX             = "";
const string  ACCESS_TYPE_NAME_SUFFIX             = "_ptr";
const string  CLASSWIDE_ACCESS_TYPE_NAME_PREFIX   = "";
const string  CLASSWIDE_ACCESS_TYPE_NAME_SUFFIX   = "_cptr";
const string  ASSOCIATION_REFERENCE_PREFIX        = "";
const string  ASSOCIATION_REFERENCE_SUFFIX        = "_asc";
const string  AGGREGATION_WHOLE_REFERENCE_PREFIX  = "";
const string  AGGREGATION_WHOLE_REFERENCE_SUFFIX  = "_whole";
const string  AGGREGATION_PART_REFERENCE_PREFIX   = "";
const string  AGGREGATION_PART_REFERENCE_SUFFIX   = "_part";
const string  COMPOSITION_WHOLE_REFERENCE_PREFIX  = "";
const string  COMPOSITION_WHOLE_REFERENCE_SUFFIX  = "_whole";
const string  COMPOSITION_PART_REFERENCE_PREFIX   = "";
const string  COMPOSITION_PART_REFERENCE_SUFFIX   = "_part";


//
//  File name generation
//
const string  FILE_NAME_DOT_REPLACEMENT_CHARACTER = "-";


//
//  Generic package and type names
//
const string  SET_PACKAGE_NAME   = "generic_set";
const string  SET_TYPE_NAME      = "set";
const string  LIST_PACKAGE_NAME  = "generic_list";
const string  LIST_TYPE_NAME     = "list";
const string  MAP_PACKAGE_NAME   = "generic_map";
const string  MAP_TYPE_NAME      = "map";
const string  TUPLE_PACKAGE_NAME = "generic_tuple";
const string  TUPLE_TYPE_NAME    = "tuple";


//
//  Formatting
//
const string  INDENT          = "    ";
const string  COMMENT_PREFIX  = "--StP|";

//
//  Used by tags for incremental code generation 
//
const string  TAG_MARK        = "--";
const string  _TO_            = "_to_";
const string  _OF_            = "_of_";


//
//  Tags for incremental code generation
//
//  These tags are used for determining the protected regions during
//  incremental code generation.
//  They must be unique strings, that is, extremely unlikely to match
//  any other string that may occur in a source file.


//  package specification

const string  START_PACKAGE_SPECIFICATION_TAG =
    COMMENT_PREFIX + "  begin scope package  " + TAG_MARK + "  ";

const string  END_PACKAGE_SPECIFICATION_TAG = 
    COMMENT_PREFIX + "  end scope package  " + TAG_MARK + "  ";

const string  START_PACKAGE_PUBLIC_FORWARD_TAG = 
    COMMENT_PREFIX + "  begin block public forward declarations  " + TAG_MARK + "  ";

const string  END_PACKAGE_PUBLIC_FORWARD_TAG = 
    COMMENT_PREFIX + "  end block public forward declarations  " + TAG_MARK + "  ";

const string  START_PACKAGE_PUBLIC_TAG = 
    COMMENT_PREFIX + "  begin block public declarations  " + TAG_MARK + "  ";

const string  END_PACKAGE_PUBLIC_TAG = 
    COMMENT_PREFIX + "  end block public declarations  " + TAG_MARK + "  ";

const string  START_PACKAGE_PRIVATE_FORWARD_TAG = 
    COMMENT_PREFIX + "  begin block private forward declarations  " + TAG_MARK + "  ";

const string  END_PACKAGE_PRIVATE_FORWARD_TAG = 
    COMMENT_PREFIX + "  end block private forward declarations  " + TAG_MARK + "  ";

const string  START_PACKAGE_PRIVATE_TAG = 
    COMMENT_PREFIX + "  begin block private declarations  " + TAG_MARK + "  ";

const string  END_PACKAGE_PRIVATE_TAG = 
    COMMENT_PREFIX + "  end block private declarations  " + TAG_MARK + "  ";

// generic package instantiations

const string  START_PACKAGE_INSTANTIATION_TAG =
    COMMENT_PREFIX + "  begin block package instantiation  " + TAG_MARK + "  ";

const string  END_PACKAGE_INSTANTIATION_TAG = 
    COMMENT_PREFIX + "  end block package instantiation " + TAG_MARK + "  ";

// package body

const string  START_PACKAGE_BODY_TAG =
    COMMENT_PREFIX + "  begin scope package body  " + TAG_MARK + "  ";

const string  END_PACKAGE_BODY_TAG = 
    COMMENT_PREFIX + "  end scope package body  " + TAG_MARK + "  ";

const string  START_PACKAGE_IMPLEMENTATION_TAG = 
    COMMENT_PREFIX + "  begin block implementation declarations  " + TAG_MARK + "  ";

const string  END_PACKAGE_IMPLEMENTATION_TAG = 
    COMMENT_PREFIX + "  end block implementation declarations  " + TAG_MARK + "  ";

// operations

const string  START_OPERATION_TAG = 
    COMMENT_PREFIX + "  begin scope operation  " + TAG_MARK + "  ";

const string  END_OPERATION_TAG = 
    COMMENT_PREFIX + "  end scope operation  " + TAG_MARK + "  ";

const string  START_OPERATION_STUB_TAG = 
    COMMENT_PREFIX + "  begin block operation stub  " + TAG_MARK + "  ";

const string  END_OPERATION_STUB_TAG = 
    COMMENT_PREFIX + "  end block operation stub  " + TAG_MARK + "  ";

const string  START_OPERATION_CODE_TAG = 
    COMMENT_PREFIX + "  begin block code  " + TAG_MARK + "  ";

const string  END_OPERATION_CODE_TAG = 
    COMMENT_PREFIX + "  end block code  " + TAG_MARK + "  ";

// generic instantiations for associations

const string  START_SET_TAG = 
    COMMENT_PREFIX + "  begin block " + SET_TYPE_NAME + "  " + TAG_MARK + "  ";

const string  END_SET_TAG = 
    COMMENT_PREFIX + "  end block " + SET_TYPE_NAME + "  " + TAG_MARK + "  ";

const string  START_LIST_TAG = 
    COMMENT_PREFIX + "  begin block " + LIST_TYPE_NAME + "  " + TAG_MARK + "  ";

const string  END_LIST_TAG = 
    COMMENT_PREFIX + "  end block " + LIST_TYPE_NAME + "  " + TAG_MARK + "  ";

const string  START_MAP_TAG = 
    COMMENT_PREFIX + "  begin block " + MAP_TYPE_NAME + "  " + TAG_MARK + "  ";

const string  END_MAP_TAG = 
    COMMENT_PREFIX + "  end block " + MAP_TYPE_NAME + "  " + TAG_MARK + "  ";

const string  START_MAP_TO_SET_TAG = 
    COMMENT_PREFIX + "  begin block " + MAP_TYPE_NAME + _TO_ + SET_TYPE_NAME + "  " + 
    TAG_MARK + "  ";

const string  END_MAP_TO_SET_TAG = 
    COMMENT_PREFIX + "  end block " + MAP_TYPE_NAME + _TO_ + SET_TYPE_NAME +
    "  " + TAG_MARK + "  ";

const string  START_MAP_TO_LIST_TAG = 
    COMMENT_PREFIX + "  begin block " + MAP_TYPE_NAME + _TO_ + LIST_TYPE_NAME + "  " + 
    TAG_MARK + "  ";

const string  END_MAP_TO_LIST_TAG = 
    COMMENT_PREFIX + "  end block " + MAP_TYPE_NAME + _TO_ + LIST_TYPE_NAME +
    "  " + TAG_MARK + "  ";

const string  START_TUPLE_TAG = 
    COMMENT_PREFIX + "  begin block " + TUPLE_TYPE_NAME + "  " + TAG_MARK + "  ";

const string  END_TUPLE_TAG = 
    COMMENT_PREFIX + "  end block " + TUPLE_TYPE_NAME + "  " + TAG_MARK + "  ";

const string  START_SET_OF_TUPLE_TAG = 
    COMMENT_PREFIX + "  begin block " + SET_TYPE_NAME + _OF_ + 
    TUPLE_TYPE_NAME + "  " + TAG_MARK + "  ";

const string  END_SET_OF_TUPLE_TAG = 
    COMMENT_PREFIX + "  end block " + SET_TYPE_NAME + _OF_ + 
    TUPLE_TYPE_NAME + "  " + TAG_MARK + "  ";

const string  START_LIST_OF_TUPLE_TAG = 
    COMMENT_PREFIX + "  begin block " + LIST_TYPE_NAME + _OF_ + 
    TUPLE_TYPE_NAME + "  " + TAG_MARK + "  ";

const string  END_LIST_OF_TUPLE_TAG = 
    COMMENT_PREFIX + "  end block " + LIST_TYPE_NAME + _OF_ + 
    TUPLE_TYPE_NAME + "  " + TAG_MARK + "  ";

const string  START_MAP_TO_TUPLE_TAG = 
    COMMENT_PREFIX + "  begin block " + MAP_TYPE_NAME + _TO_ + 
    TUPLE_TYPE_NAME + "  " + TAG_MARK + "  ";

const string  END_MAP_TO_TUPLE_TAG = 
    COMMENT_PREFIX + "  end block " + MAP_TYPE_NAME + _TO_ + 
    TUPLE_TYPE_NAME + "  " + TAG_MARK + "  ";

const string  START_MAP_TO_SET_OF_TUPLE_TAG = 
    COMMENT_PREFIX + "  begin block " + MAP_TYPE_NAME + _TO_ + SET_TYPE_NAME + _OF_ + 
    TUPLE_TYPE_NAME + "  " + TAG_MARK + "  ";

const string  END_MAP_TO_SET_OF_TUPLE_TAG = 
    COMMENT_PREFIX + "  end block " + MAP_TYPE_NAME + _TO_ + SET_TYPE_NAME +
    _OF_ + TUPLE_TYPE_NAME + "  " + TAG_MARK + "  ";

const string  START_MAP_TO_LIST_OF_TUPLE_TAG = 
    COMMENT_PREFIX + "  begin block " + MAP_TYPE_NAME + _TO_ + LIST_TYPE_NAME + _OF_ + 
    TUPLE_TYPE_NAME + "  " + TAG_MARK + "  ";

const string  END_MAP_TO_LIST_OF_TUPLE_TAG = 
    COMMENT_PREFIX + "  end block " + MAP_TYPE_NAME + _TO_ + LIST_TYPE_NAME +
    _OF_ + TUPLE_TYPE_NAME + "  " + TAG_MARK + "  ";


// This is to make the unit name functionality available for the
// ObjectAda navigation code 
#include "qrl/code_gen/ada_95/unit_name.inc"
