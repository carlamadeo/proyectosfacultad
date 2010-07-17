/*
 * Confidential property of Interactive Development Environments, Inc. *
 * Copyright (c) 1992-1993 *    All rights reserved
 */

/************************************************************************
 *
 *
 * File:  oms_defines.h
 *
 *
 *
 ************************************************************************/

#ifndef OMS_DEFINES_H
#define OMS_DEFINES_H

#define OMS_STRING_LEN_MAX                      220

#define OMS_DEFAULT_ROOT_DIR                    "/usr/local/ide"
#define OMS_BASE_DIR                            "IDEDIR"

#define OMS_REPNAME_LEN                         30      /* ECR 5876 */

static const unsigned U1 = 1U;

#define PREFIX                          "xx_"
#define BUFLEN                          1024

#define DB_CNTX_OBJS                    "xx_cntx"
#define DB_ALL_CNTX_REF_OBJS            "xx_cntx_ref"
#define DB_CNTX_REF_OBJS                "xx_cmtd_cntx_ref"
#define DB_DIRTY_REF_OBJS               "xx_dirty_ref"
#define DB_FILE_HIST_OBJS               "xx_file_hist"
#define DB_FILE_LOCK_OBJS               "xx_file_lock"
#define DB_FILE_OBJS                    "xx_file"
#define DB_ITEM_OBJS                    "xx_item"
#define DB_ALL_LINK_REF_OBJS            "xx_link_ref"
#define DB_LINK_REF_OBJS                "xx_cmtd_link_ref"
#define DB_LINK_OBJS                    "xx_link"
#define DB_ALL_NODE_REF_OBJS            "xx_node_ref"
#define DB_NODE_REF_OBJS                "xx_cmtd_node_ref"
#define DB_NODE_OBJS                    "xx_node"
#define DB_NOTE_OBJS                    "xx_note"
#define DB_OBJECT_ID_OBJS               "xx_object_id"
#define DB_RENAME_LABEL_OBJS            "xx_rename_label"
#define DB_VIEWPOINT_OBJS               "xx_viewpoint"

#define OMS_CNTX_OBJS                   "cntx"
#define OMS_ALL_CNTX_REF_OBJS           "all_cntx_ref"
#define OMS_CNTX_REF_OBJS               "cntx_ref"
#define OMS_DIRTY_REF_OBJS              "dirty_ref"
#define OMS_FILE_HIST_OBJS              "file_hist"
#define OMS_FILE_LOCK_OBJS              "file_lock"
#define OMS_FILE_OBJS                   "file"
#define OMS_ITEM_OBJS                   "item"
#define OMS_ALL_LINK_REF_OBJS           "all_link_ref"
#define OMS_LINK_REF_OBJS               "link_ref"
#define OMS_LINK_OBJS                   "link"
#define OMS_ALL_NODE_REF_OBJS           "all_node_ref"
#define OMS_NODE_REF_OBJS               "node_ref"
#define OMS_NODE_OBJS                   "node"
#define OMS_NOTE_OBJS                   "note"
#define OMS_OBJECT_ID_OBJS              "object_id"
#define OMS_RENAME_LABEL_OBJS           "rename_label"
#define OMS_VIEWPOINT_OBJS              "viewpoint"


#define DB_ACCESSORS_ATTR               "xx_accessors"
#define DB_ANNOT_FILE_ID_ATTR           "xx_annot_file_id"
#define DB_APPID_ATTR                   "xx_appid"
#define DB_ATTR_ATTR                    "xx_attribute"
#define DB_CNTX_ID_ATTR                 "xx_cntx_id"
#define DB_DEPENDENCY_NAME_ATTR         "xx_dependency_name"
#define DB_DESC_ATTR                    "xx_desc"
#define DB_DESTROYERS_ATTR              "xx_destroyers"
#define DB_DIRTY_ATTR                   "xx_dirty"
#define DB_DURATION_ATTR                "xx_duration"
#define DB_FNAME_ATTR                   "xx_fname"
#define DB_FILE_ID_ATTR                 "xx_file_id"
#define DB_FROM_NODE_REF_ID_ATTR        "xx_from_node_ref_id"
#define DB_FROM_NODE_ID_ATTR            "xx_from_node_id"
#define DB_GOCHA_ATTR                   "xx_gocha_type"
#define DB_GUID_ATTR                    "xx_guid"               /* ECR 5673 */
#define DB_HOSTNAME_ATTR                "xx_hostname"
#define DB_ID_ATTR                      "xx_id"
#define DB_LABEL_ATTR                   "xx_label"
#define DB_LMDFY_ID_ATTR                "xx_lmdfy_id"
#define DB_LSYNC_ID_ATTR                "xx_lsync_id"
#define DB_LVIEW_ID_ATTR                "xx_lview_id"
#define DB_LINK_ID_ATTR                 "xx_link_id"
#define DB_LINK_REF_ID_ATTR             "xx_link_ref_id"
#define DB_NAME_ATTR                    "xx_name"
#define DB_NEW_ID_ATTR                  "xx_new_id"
#define DB_NODE_ID_ATTR                 "xx_node_id"
#define DB_NODE_REF_ID_ATTR             "xx_node_ref_id"
#define DB_NOTE_ID_ATTR                 "xx_note_id"
#define DB_OBJ_ID_ATTR                  "xx_obj_id"
#define DB_PDM_TYPE_ATTR                "xx_pdm_type"
#define DB_PID_ATTR                     "xx_pid"
#define DB_REV_ATTR                     "xx_rev"
#define DB_ROWNUM_ATTR                  "xx_rownum"
#define DB_SCOPE_NODE_ID_ATTR           "xx_scope_node_id"
#define DB_SCOPE_NODE_REF_ID_ATTR       "xx_scope_node_ref_id"
#define DB_SIG_ATTR                     "xx_sig"
#define DB_SRC_OBJ_ID_ATTR                      "xx_src_obj_id"
#define DB_SVALUE_ATTR                  "xx_svalue"
#define DB_TIME_ATTR                    "xx_time"
#define DB_TO_NODE_REF_ID_ATTR          "xx_to_node_ref_id"
#define DB_TO_NODE_ID_ATTR              "xx_to_node_id"
#define DB_TYPE_ATTR                    "xx_type"
#define DB_UNCOMMITTED_ATTR             "xx_uncommitted"
#define DB_USER_ATTR                    "xx_user"
#define DB_VALUE_ATTR                   "xx_value"
#define DB_XCOORD_ATTR                  "xx_xcoord"
#define DB_YCOORD_ATTR                  "xx_ycoord"

#define OMS_ACCESSORS_ATTR              "accessors"
#define OMS_ANNOT_FILE_ID_ATTR          "annot_file_id"
#define OMS_APPID_ATTR                  "appid"
#define OMS_ATTR_ATTR                   "attribute"
#define OMS_CNTX_ID_ATTR                "cntx_id"
#define OMS_DESC_ATTR                   "desc"
#define OMS_DESTROYERS_ATTR             "destroyers"
#define OMS_DIRTY_ATTR                  "dirty"
#define OMS_DURATION_ATTR               "duration"
#define OMS_FILE_ID_ATTR                "file_id"
#define OMS_FNAME_ATTR                  "fname"
#define OMS_FROM_NODE_REF_ID_ATTR       "from_node_ref_id"
#define OMS_FROM_NODE_ID_ATTR           "from_node_id"
#define OMS_GUID_ATTR                   "guid"                  /* ECR 5673 */
#define OMS_HOSTNAME_ATTR               "hostname"
#define OMS_ID_ATTR                     "id"
#define OMS_LABEL_ATTR                  "label"
#define OMS_LMDFY_ID_ATTR               "lmdfy_id"
#define OMS_LSYNC_ID_ATTR               "lsync_id"
#define OMS_LVIEW_ID_ATTR               "lview_id"
#define OMS_LINK_ID_ATTR                "link_id"
#define OMS_LINK_REF_ID_ATTR            "link_ref_id"
#define OMS_NAME_ATTR                   "name"
#define OMS_NEW_ID_ATTR                 "new_id"
#define OMS_NODE_ID_ATTR                "node_id"
#define OMS_NODE_REF_ID_ATTR            "node_ref_id"
#define OMS_NOTE_ID_ATTR                "note_id"
#define OMS_OBJ_ID_ATTR                 "obj_id"
#define OMS_PDM_TYPE_ATTR               "pdm_type"
#define OMS_PID_ATTR                    "pid"
#define OMS_REV_ATTR                    "rev"
#define OMS_ROWNUM_ATTR                 "rownum"
#define OMS_SCOPE_NODE_ID_ATTR          "scope_node_id"
#define OMS_SCOPE_NODE_REF_ID_ATTR      "scope_node_ref_id"
#define OMS_SIG_ATTR                    "sig"
#define OMS_SVALUE_ATTR                 "svalue"
#define OMS_TIME_ATTR                   "time"
#define OMS_TO_NODE_REF_ID_ATTR         "to_node_ref_id"
#define OMS_TO_NODE_ID_ATTR             "to_node_id"
#define OMS_TYPE_ATTR                   "type"
#define OMS_UNCOMMITTED_ATTR            "uncommitted"
#define OMS_USER_ATTR                   "user"
#define OMS_VALUE_ATTR                  "value"
#define OMS_XCOORD_ATTR                 "xcoord"
#define OMS_YCOORD_ATTR                 "ycoord"

/* total number of values in emum definition */
#define OMS_MAX_PDM_TYPES               18

#define OMS_FILE_OUT_OF_DATE_IDS        0x01
#define OMS_FILE_DIRTY_REFERENCES       0x02

#endif /* OMS_DEFINES_H */

