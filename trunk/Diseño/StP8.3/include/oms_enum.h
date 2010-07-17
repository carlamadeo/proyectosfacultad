/************************************************************************
 *
 *
 * File:  oms_enum.h
 *
 * Confidential property of Interactive Development Environments, Inc.
 * Copyright (c) 1992-1995
 * All rights reserved
 *
 *
 ************************************************************************/

#ifndef __OMS_ENUM_H_DEFINED__
#define __OMS_ENUM_H_DEFINED__

/*      static char sccsid[] = "%W% %Y% %D%"; */

#include "oms_general.h"

/*
 * DO NOT remove the integer assignments from the enumeration declared below.
 * These numbers must remain constant across changes to the repository
 * structure, because these values will actually be stored in the repository
 * and used to identify table to query.  Do not delete entries in this
 * enumeration, even if they become obsolete, for the same reason.
 *
 * For efficiency, the legal_rules[] in oms_app_type.C is now ordered like
 * this, so make sure the two stay in sync.
 */
typedef enum oms_pdm_type_enum
{
    OMS_NULL_PDM_TYPE = 0,
    OMS_CNTX_REF = 1,
    OMS_ALL_CNTX_REF = 2,
    OMS_CNTX = 3,
    OMS_FILE_HIST = 4,
    OMS_FILE_LOCK = 5,
    OMS_FILE = 6,
    OMS_ITEM = 7,
    OMS_LINK_REF = 8,
    OMS_ALL_LINK_REF = 9,
    OMS_LINK = 10,
    OMS_NODE_REF = 11,
    OMS_ALL_NODE_REF = 12,
    OMS_NODE = 13,
    OMS_NOTE = 14,
    OMS_VIEWPOINT = 15,
    OMS_DIRTY_REF = 16,
    OMS_SCHEMA_VERSION = 17,
    OMS_RENAME_LABEL = 18,
    OMS_OBJECT_ID = 19,
    OMS_REPOSITORY_VERSION = 20
} oms_pdm_type_tp;

#define OMS_LAST_PDM_TYPE OMS_REPOSITORY_VERSION

typedef enum oms_scope_cmp_enum
{
    OMS_SCOPES_DIFFERENT,
    OMS_SCOPES_EQUAL,
    OMS_SCOPE1_IN_SCOPE2,
    OMS_SCOPE2_IN_SCOPE1
}               oms_scope_cmp_tp;

/*
 * DO NOT remove the integer assignments from the enumeration declared below.
 * These numbers must remain constant across changes to the repository
 * structure, because these values will actually be stored in the repository
 * and used to identify table to query.  Do not delete entries in this
 * enumeration, even if they become obsolete, for the same reason.
 */

typedef enum oms_attr_id_enum
{
    OMS_ERROR_ATTR_ENUM = -1,
    OMS_NAME_ATTR_ENUM = 0,
    OMS_SIG_ATTR_ENUM  = 1
} oms_attr_tp;

typedef enum iv_type_enum
{
    IV_TYPE_NULL,
    IV_TYPE_INT,
    IV_TYPE_APP_TYPE,
    IV_TYPE_CHOICE,
    IV_TYPE_INDEX,
    IV_TYPE_SPECIAL,
    IV_TYPE_STRING,
    IV_TYPE_TEXT,
    IV_TYPE_TIME,
    LAST_IV_TYPE
}               iv_type_tp;

typedef enum oms_method_enum
{
    OMS_CMD_STRING,
    OMS_BUILTIN
}               oms_method_tp;


typedef enum oms_gocha_enum
{
    OMS_GOCHA_RENAME,
    OMS_GOCHA_DELETE,
    OMS_GOCHA_CHANGE,
    OMS_GOCHA_SCOPE,
    OMS_GOCHA_INSERT
}               oms_gocha_tp;


// In the new OMS we use the ObjectId table to store a whole bunch of
// different Ids - the next annotation file to use, the next code
// generation ids to use, etc, etc. Do not change any of the numeric
// values within this list, as they map into the row number of the
// table that holds the Id. If you add to it, then remember to put the
// item name into the table below, so that lookup works.
enum OmsIdEnum
{
    OMS_OMS_OBJECT_ID = 0,
    OMS_ANNOT_FILE_ID = 1,
    OMS_UML_CXX_CODE_GEN_ID = 2,
    OMS_OMT_CXX_CODE_GEN_ID = 3,
    OMS_OMT_ADA83_CODE_GEN_ID = 4,
    OMS_OMT_ADA95_CODE_GEN_ID = 5,
    OMS_OMT_JAVA_CODE_GEN_ID = 6,
    OMS_SE_CODE_GEN_ID = 7,
    OMS_SE60_MIGRATE_ID = 8,
    // insert new ones here
    OMS_LAST_ID
};


#endif                         /* __OMS_ENUM_H_DEFINED__ */
