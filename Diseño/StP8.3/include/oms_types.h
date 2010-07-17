/************************************************************************
 *
 *
 * File:  oms_types.c
 *
 * Confidential property of Interactive Development Environments, Inc.
 * Copyright (c) 1992-1995
 * All rights reserved
 *
 ************************************************************************/

#ifndef __OMS_TYPES_H_DEFINED__
#define __OMS_TYPES_H_DEFINED__

/* static char sccsid[] = "%W% %Y% %D%" */

#include <stdio.h>
#include "oms_general.h"
#include "oms_enum.h"

class IdeString;
class IdeTStringColl;
class IdeDatabaseCursor;

class IdePdmObject;

class IdePdmCntxRef;
class IdePdmCntx;
class IdePdmFileHist;
class IdePdmFileLock;
class IdePdmFile;
class IdePdmItem;
class IdePdmLinkRef;
class IdePdmLink;
class IdePdmNodeRef;
class IdePdmNode;
class IdePdmNote;
class IdePdmViewpoint;
class IdePdmDirtyRef;
class IdePdmRenameLabel;
class IdePdmObjidRec;
typedef IdePdmCntxRef oms_cntx_ref_tp;
typedef IdePdmCntx oms_cntx_tp;
typedef IdePdmFileHist oms_file_hist_tp;
typedef IdePdmFileLock oms_file_lock_tp;
typedef IdePdmFile oms_file_tp;
typedef IdePdmItem oms_item_tp;
typedef IdePdmLinkRef oms_link_ref_tp;
typedef IdePdmLink oms_link_tp;
typedef IdePdmNodeRef oms_node_ref_tp;
typedef IdePdmNode oms_node_tp;
typedef IdePdmNote oms_note_tp;
typedef IdePdmViewpoint oms_viewpoint_tp;
typedef IdePdmDirtyRef oms_dirty_ref_tp;
typedef IdePdmRenameLabel oms_rename_label_tp;
typedef IdePdmObjidRec oms_objid_rec_tp;


typedef long    oms_object_id_tp;
typedef int     oms_app_type_tp;
typedef long    oms_time_tp;
typedef short   oms_repos_id_tp;

typedef struct oms_bigcoll_struct
{
    oms_pdm_type_tp pdm_type;
    int             position;
    IdePdmObject   *current;
    IdeDatabaseCursor *cursor;
    IdeTStringColl *resetSql; // sql to execute whenever bigcoll is reset
    IdeTStringColl *deallocSql; // sql to execute whenever bigcoll is freed
    bool            keepCurrent;
}               oms_bigcoll_tp;

typedef struct oms_coll_struct
{
    oms_pdm_type_tp pdm_type;
    void           *list;
}               oms_coll_tp;

typedef struct oms_repos_struct
{
    int             id;
    char           *repository_type;
    char           *server_name;
    char           *rep_name;
    oms_object_id_tp current_objid;
    oms_object_id_tp max_allocated_objid;
    struct {
        unsigned usesNewLockingStrategy:1;
        unsigned RESERVED:31;
    } flags;
}               oms_repos_tp;

typedef struct oms_app_type_method_struct
{
    oms_method_tp    type;
    char            *cmd;
}               oms_app_type_method_tp;

typedef enum value_type {
    OMS_INT_TYPE,
    OMS_CHAR_TYPE,
    OMS_LONG_CHAR_TYPE
} oms_value_type_tp;

/*** structure for expression tree build by yacc ***/
typedef struct query_node_impl
{
    int             token;
    char           *value;
    iv_type_tp      convert;
    struct query_node_impl *left, *right, *parent;
}               query_node_tp;

/*** structure for idlist's that are created by applications ***/
typedef struct idlist_struct
{
    oms_pdm_type_tp pdm_type;
    char           *name;
    char           *table;
    char           *index;
}               idlist_tp;

typedef         bool(*insertfunc_tp) (void *);
typedef         bool(*updatefunc_tp) (void *, void *);
typedef         bool(*deletefunc_tp) (void *);
typedef         void*(*findfunc_tp) (oms_object_id_tp);
typedef         void(*freefunc_tp) (void **);
typedef         bool(*checkfunc_tp) (void *);
typedef         void(*printfunc_tp) (void *, FILE *);
typedef         void*(*readfunc_tp) (void *);
typedef         bool(*equalfunc_tp) (void *, void *);
typedef         void*(*copyfunc_tp) (void *);
typedef         oms_object_id_tp(*idfunc_tp) (void *);

#ifdef CPROTO
typedef void *FILE;
typedef void *DBPROCESS;
typedef void IdeMapStringToPtr;
typedef void IdeDatabase;
typedef void IdeDatabaseCursor;
typedef void IdeException;
typedef void IdeDBException;
#endif

#endif                         /* __OMS_TYPES_H_DEFINED__ */


