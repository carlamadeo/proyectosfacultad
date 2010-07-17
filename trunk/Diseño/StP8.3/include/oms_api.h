/////////////////////////////////////////////////////////////////////////
//
// File:  oms_api.h
//
// Confidential property of Aonix Inc.
// Copyright (c) 1998
// All rights reserved
//
// These procedures make up the public OMS API as described in Chapters 4
// and 5 of the Object Management System document.
//
// Any changes to this file should be reflected in the documentation
//
//////////////////////////////////////////////////////////////////////////

#ifndef __OMS_API_INCLUDED__
#define __OMS_API_INCLUDED__

#include "IdeOmsApi.h"
#include "oms_types.h"

#if __STDC__ || defined(__cplusplus)
#define PARAM_LIST(s) s
#else
#define PARAM_LIST(s) ()
#endif

// oms_app_type_tp Functions

IDE_OMS_API const char *oms_app_type_annot_filename PARAM_LIST((oms_app_type_tp type));
IDE_OMS_API oms_app_type_tp oms_app_type_code_get PARAM_LIST((oms_pdm_type_tp pdm_type, int index));
IDE_OMS_API const char *oms_app_type_code_to_text PARAM_LIST((oms_app_type_tp type));
IDE_OMS_API const char *oms_app_type_comment PARAM_LIST((oms_app_type_tp type));
IDE_OMS_API const char *oms_app_type_datatype PARAM_LIST((oms_app_type_tp type));
IDE_OMS_API oms_app_type_method_tp *oms_app_type_method PARAM_LIST((oms_app_type_tp type, const char *name));
IDE_OMS_API oms_pdm_type_tp oms_app_type_pdm_type PARAM_LIST((oms_app_type_tp type));
IDE_OMS_API const char *oms_app_type_printstring PARAM_LIST((oms_app_type_tp type));
IDE_OMS_API const char *oms_app_type_text_get PARAM_LIST((oms_pdm_type_tp pdm_type,
                                                          int index));
IDE_OMS_API char *oms_app_type_text_get_copy PARAM_LIST((oms_pdm_type_tp pdm_type, int index));
IDE_OMS_API oms_app_type_tp oms_app_type_text_to_code PARAM_LIST((oms_pdm_type_tp pdm_type,
                                                                  const char *text));

// oms_app_bigcoll_tp Functions

IDE_OMS_API void oms_bigcoll_free PARAM_LIST((oms_bigcoll_tp **bcoll));
IDE_OMS_API void oms_bigcoll_print PARAM_LIST((oms_bigcoll_tp *bcoll, FILE *fp));
IDE_OMS_API void oms_bigcoll_reset PARAM_LIST((oms_bigcoll_tp *bcoll));
IDE_OMS_API bool oms_bigcoll_set_destroy_current PARAM_LIST((oms_bigcoll_tp *bcoll, bool destroy));

// oms_cntx_ref_tp Creation and Retrieval Functions

IDE_OMS_API oms_cntx_ref_tp *oms_cntx_ref_create PARAM_LIST((oms_object_id_tp cntx_id,
                                                             oms_object_id_tp file_id, const char *appid));
IDE_OMS_API oms_cntx_ref_tp *oms_cntx_ref_find PARAM_LIST((oms_object_id_tp cntx_id,
                                                           oms_object_id_tp file_id, const char *appid));
IDE_OMS_API oms_cntx_ref_tp *oms_cntx_ref_find_by_id PARAM_LIST((oms_object_id_tp id));
IDE_OMS_API oms_cntx_ref_tp *oms_cntx_ref_find_by_query PARAM_LIST((const char *query));

// oms_cntx_ref_tp Utility Functions

IDE_OMS_API oms_cntx_ref_tp *oms_cntx_ref_copy PARAM_LIST((oms_cntx_ref_tp *cr));
IDE_OMS_API bool oms_cntx_ref_equal PARAM_LIST((oms_cntx_ref_tp *cr1,
                                                          oms_cntx_ref_tp *cr2));
IDE_OMS_API void oms_cntx_ref_free PARAM_LIST((oms_cntx_ref_tp **cr));
IDE_OMS_API void oms_cntx_ref_print PARAM_LIST((oms_cntx_ref_tp *cr, FILE *fp));
IDE_OMS_API void oms_cntx_ref_print_image_asgn PARAM_LIST((const char *cp));

// oms_cntx_ref_tp Attribute Access and Assign Functions

IDE_OMS_API oms_object_id_tp oms_cntx_ref_id PARAM_LIST((oms_cntx_ref_tp *cr));
IDE_OMS_API oms_object_id_tp oms_cntx_ref_cntx_id PARAM_LIST((oms_cntx_ref_tp *cr));
IDE_OMS_API oms_object_id_tp oms_cntx_ref_file_id PARAM_LIST((oms_cntx_ref_tp *cr));
IDE_OMS_API char *oms_cntx_ref_appid PARAM_LIST((oms_cntx_ref_tp *cr));
IDE_OMS_API oms_object_id_tp oms_cntx_ref_link_ref_id PARAM_LIST((oms_cntx_ref_tp *cr));
IDE_OMS_API int oms_cntx_ref_xcoord PARAM_LIST((oms_cntx_ref_tp *cr));
IDE_OMS_API int oms_cntx_ref_ycoord PARAM_LIST((oms_cntx_ref_tp *cr));
IDE_OMS_API char *oms_cntx_ref_appid_copy PARAM_LIST((oms_cntx_ref_tp *cr));
IDE_OMS_API void oms_cntx_ref_appid_asgn PARAM_LIST((oms_cntx_ref_tp *cr,
                                                     const char *appid));
IDE_OMS_API void oms_cntx_ref_cntx_id_asgn PARAM_LIST((oms_cntx_ref_tp *cr,
                                                       oms_object_id_tp cntx_id));
IDE_OMS_API void oms_cntx_ref_file_id_asgn PARAM_LIST((oms_cntx_ref_tp *cr,
                                                       oms_object_id_tp file_id));
IDE_OMS_API void oms_cntx_ref_link_ref_id_asgn PARAM_LIST((oms_cntx_ref_tp *cr,
                                                           oms_object_id_tp link_ref_id));
IDE_OMS_API void oms_cntx_ref_xcoord_asgn PARAM_LIST((oms_cntx_ref_tp *cr,
                                                      int xcoord));
IDE_OMS_API void oms_cntx_ref_ycoord_asgn PARAM_LIST((oms_cntx_ref_tp *cr,
                                                      int ycoord));

// oms_cntx_ref_tp Repository Management Functions

IDE_OMS_API bool oms_cntx_ref_update PARAM_LIST((oms_cntx_ref_tp *cr));
IDE_OMS_API bool oms_cntx_ref_delete PARAM_LIST((oms_cntx_ref_tp *cr));

// oms_cntx_ref_tp Typed Collection Functions

IDE_OMS_API bool oms_cntx_ref_coll_add PARAM_LIST((oms_coll_tp *coll, oms_cntx_ref_tp *cr));
IDE_OMS_API bool oms_cntx_ref_coll_add_unique PARAM_LIST((oms_coll_tp *coll,
                                                                   oms_cntx_ref_tp *cr));
IDE_OMS_API oms_coll_tp *oms_cntx_ref_coll_create PARAM_LIST((const char *query));
IDE_OMS_API bool oms_cntx_ref_coll_delete PARAM_LIST((oms_coll_tp *coll, oms_cntx_ref_tp *cr));
IDE_OMS_API bool oms_cntx_ref_coll_delete_equivalent PARAM_LIST((oms_coll_tp *coll,
                                                                          oms_cntx_ref_tp *cr));
IDE_OMS_API oms_cntx_ref_tp *oms_cntx_ref_coll_get PARAM_LIST((oms_coll_tp *coll, int i));

// oms_cntx_ref_tp Typed Big Collection Functions

IDE_OMS_API oms_bigcoll_tp *oms_cntx_ref_bigcoll_create PARAM_LIST((const char *query));
IDE_OMS_API oms_cntx_ref_tp *oms_cntx_ref_bigcoll_next PARAM_LIST((oms_bigcoll_tp *bcoll));

// oms_cntx_ref_tp Navigation Functions

IDE_OMS_API oms_cntx_tp *oms_cntx_ref_cntx PARAM_LIST((oms_cntx_ref_tp *cr));
IDE_OMS_API oms_file_tp *oms_cntx_ref_file PARAM_LIST((oms_cntx_ref_tp *cr));
IDE_OMS_API oms_link_ref_tp *oms_cntx_ref_link_ref PARAM_LIST((oms_cntx_ref_tp *cr));

// oms_cntx_tp Creation and Retrieval Functions

IDE_OMS_API oms_cntx_tp *oms_cntx_create PARAM_LIST((const char *name,
                                                     oms_app_type_tp type,
                                                     const char *sig,
                                                     oms_object_id_tp link_id));
IDE_OMS_API oms_cntx_tp *oms_cntx_find PARAM_LIST((const char *name,
                                                   oms_app_type_tp type,
                                                   const char *sig,
                                                   oms_object_id_tp link_id));
IDE_OMS_API oms_cntx_tp *oms_cntx_find_by_id PARAM_LIST((oms_object_id_tp id));
IDE_OMS_API oms_cntx_tp *oms_cntx_find_by_query PARAM_LIST((const char *query));

// oms_cntx_tp Utility Functions

IDE_OMS_API oms_cntx_tp *oms_cntx_copy PARAM_LIST((oms_cntx_tp *cntx));
IDE_OMS_API bool oms_cntx_equal PARAM_LIST((oms_cntx_tp *cntx1,
                                                      oms_cntx_tp *cntx2));
IDE_OMS_API void oms_cntx_free PARAM_LIST((oms_cntx_tp **cntx));
IDE_OMS_API void oms_cntx_print PARAM_LIST((oms_cntx_tp *cntx, FILE *fp));
IDE_OMS_API void oms_cntx_print_image_asgn PARAM_LIST((const char *cp));

// oms_cntx_tp Attribute Access and Assign Functions

IDE_OMS_API oms_object_id_tp oms_cntx_id PARAM_LIST((oms_cntx_tp *cntx));
IDE_OMS_API char *oms_cntx_name PARAM_LIST((oms_cntx_tp *cntx));
IDE_OMS_API oms_app_type_tp oms_cntx_type PARAM_LIST((oms_cntx_tp *cntx));
IDE_OMS_API char *oms_cntx_sig PARAM_LIST((oms_cntx_tp *cntx));
IDE_OMS_API oms_object_id_tp oms_cntx_link_id PARAM_LIST((oms_cntx_tp *cntx));
IDE_OMS_API oms_object_id_tp oms_cntx_scope_node_id PARAM_LIST((oms_cntx_tp *cntx));
IDE_OMS_API oms_object_id_tp oms_cntx_annot_file_id PARAM_LIST((oms_cntx_tp *cntx));
IDE_OMS_API char *oms_cntx_name_copy PARAM_LIST((oms_cntx_tp *cntx));
IDE_OMS_API void oms_cntx_name_asgn PARAM_LIST((oms_cntx_tp *cntx,
                                                const char *name));
// ECR 5673 begin
IDE_OMS_API char *oms_cntx_guid PARAM_LIST((oms_cntx_tp *cntx));
IDE_OMS_API char *oms_cntx_guid_copy PARAM_LIST((oms_cntx_tp *cntx));
IDE_OMS_API void oms_cntx_guid_asgn PARAM_LIST((oms_cntx_tp *cntx,
                                                const char *guid));
// ECR 5673 end
IDE_OMS_API char *oms_cntx_sig_copy PARAM_LIST((oms_cntx_tp *cntx));
IDE_OMS_API void oms_cntx_sig_asgn PARAM_LIST((oms_cntx_tp *cntx,
                                               const char *sig));
IDE_OMS_API void oms_cntx_type_asgn PARAM_LIST((oms_cntx_tp *cntx,
                                                oms_app_type_tp type));
IDE_OMS_API void oms_cntx_link_id_asgn PARAM_LIST((oms_cntx_tp *cntx,
                                                   oms_object_id_tp link_id));
IDE_OMS_API void oms_cntx_scope_node_id_asgn PARAM_LIST((oms_cntx_tp *cntx,
                                                         oms_object_id_tp scope_node_id));
IDE_OMS_API void oms_cntx_annot_file_id_asgn PARAM_LIST((oms_cntx_tp *cntx,
                                                         oms_object_id_tp annot_file_id));

// oms_cntx_tp Repository Management Functions

IDE_OMS_API bool oms_cntx_update PARAM_LIST((oms_cntx_tp *cntx));
IDE_OMS_API bool oms_cntx_delete PARAM_LIST((oms_cntx_tp *cntx));
IDE_OMS_API bool oms_cntx_check PARAM_LIST((oms_cntx_tp *cntx));

// oms_cntx_tp Typed Collection Functions

IDE_OMS_API bool oms_cntx_coll_add PARAM_LIST((oms_coll_tp *coll,
                                                        oms_cntx_tp *cntx));
IDE_OMS_API bool oms_cntx_coll_add_unique PARAM_LIST((oms_coll_tp *coll, oms_cntx_tp *cntx));
IDE_OMS_API oms_coll_tp *oms_cntx_coll_create PARAM_LIST((const char *query));
IDE_OMS_API bool oms_cntx_coll_delete PARAM_LIST((oms_coll_tp *coll,
                                                           oms_cntx_tp *cntx));
IDE_OMS_API bool oms_cntx_coll_delete_equivalent PARAM_LIST((oms_coll_tp *coll,
                                                                      oms_cntx_tp *cntx));
IDE_OMS_API oms_cntx_tp *oms_cntx_coll_get PARAM_LIST((oms_coll_tp *coll,
                                                       int i));

// oms_cntx_tp Typed Big Collection Functions

IDE_OMS_API oms_bigcoll_tp *oms_cntx_bigcoll_create PARAM_LIST((const char *query));
IDE_OMS_API oms_cntx_tp *oms_cntx_bigcoll_next PARAM_LIST((oms_bigcoll_tp *bcoll));

// oms_cntx_tp Navigation Functions

IDE_OMS_API oms_link_tp *oms_cntx_link PARAM_LIST((oms_cntx_tp *cntx));
IDE_OMS_API oms_coll_tp *oms_cntx_notes PARAM_LIST((oms_cntx_tp *cntx));
IDE_OMS_API oms_node_tp *oms_cntx_scope_node PARAM_LIST((oms_cntx_tp *cntx));
IDE_OMS_API oms_coll_tp *oms_cntx_cntx_refs PARAM_LIST((oms_cntx_tp *cntx));

// oms_coll_tp Functions

IDE_OMS_API int oms_coll_count PARAM_LIST((oms_coll_tp *coll));
IDE_OMS_API void oms_coll_free PARAM_LIST((oms_coll_tp **coll));
IDE_OMS_API void oms_coll_free_all PARAM_LIST((oms_coll_tp **coll));
IDE_OMS_API void oms_coll_print PARAM_LIST((oms_coll_tp *coll, FILE *fp));

// oms_file_hist_tp Creation and Retrieval Functions

IDE_OMS_API oms_file_hist_tp *oms_file_hist_create PARAM_LIST((oms_app_type_tp type,
                                                               oms_object_id_tp file_id,
                                                               oms_time_tp time,
                                                               const char *user,
                                                               const char *hostname));
IDE_OMS_API oms_file_hist_tp *oms_file_hist_find PARAM_LIST((oms_app_type_tp type,
                                                             oms_object_id_tp file_id,
                                                             oms_time_tp time,
                                                             const char *user,
                                                             const char *hostname));
IDE_OMS_API oms_file_hist_tp *oms_file_hist_find_by_id PARAM_LIST((oms_object_id_tp id));
IDE_OMS_API oms_file_hist_tp *oms_file_hist_find_by_query PARAM_LIST((const char *query));

// oms_file_hist_tp Utility Functions

IDE_OMS_API oms_file_hist_tp *oms_file_hist_copy PARAM_LIST((oms_file_hist_tp *fh));
IDE_OMS_API bool oms_file_hist_equal PARAM_LIST((oms_file_hist_tp *fh1, oms_file_hist_tp *fh2));
IDE_OMS_API void oms_file_hist_free PARAM_LIST((oms_file_hist_tp **fh));
IDE_OMS_API void oms_file_hist_print PARAM_LIST((oms_file_hist_tp *fh,
                                                 FILE *fp));
IDE_OMS_API void oms_file_hist_print_image_asgn PARAM_LIST((const char *cp));

// oms_file_hist_tp Attribute Access and Assign Functions

IDE_OMS_API oms_object_id_tp oms_file_hist_id PARAM_LIST((oms_file_hist_tp *fh));
IDE_OMS_API oms_object_id_tp oms_file_hist_file_id PARAM_LIST((oms_file_hist_tp *fh));
IDE_OMS_API oms_app_type_tp oms_file_hist_type PARAM_LIST((oms_file_hist_tp *fh));
IDE_OMS_API oms_time_tp oms_file_hist_time PARAM_LIST((oms_file_hist_tp *fh));
IDE_OMS_API char *oms_file_hist_user PARAM_LIST((oms_file_hist_tp *fh));
IDE_OMS_API char *oms_file_hist_hostname PARAM_LIST((oms_file_hist_tp *fh));
IDE_OMS_API char *oms_file_hist_hostname_copy PARAM_LIST((oms_file_hist_tp *fh));
IDE_OMS_API char *oms_file_hist_user_copy PARAM_LIST((oms_file_hist_tp *fh));
IDE_OMS_API void oms_file_hist_file_id_asgn PARAM_LIST((oms_file_hist_tp *fh,
                                                        oms_object_id_tp file_id));
IDE_OMS_API void oms_file_hist_type_asgn PARAM_LIST((oms_file_hist_tp *fh,
                                                     oms_app_type_tp type));
IDE_OMS_API void oms_file_hist_time_asgn PARAM_LIST((oms_file_hist_tp *fh,
                                                     oms_time_tp time));
IDE_OMS_API void oms_file_hist_user_asgn PARAM_LIST((oms_file_hist_tp *fh,
                                                     const char *user));
IDE_OMS_API void oms_file_hist_hostname_asgn PARAM_LIST((oms_file_hist_tp *fh,
                                                         const char *hostname));

// oms_file_hist_tp Repository Management Functions

IDE_OMS_API bool oms_file_hist_check PARAM_LIST((oms_file_hist_tp *fh));
IDE_OMS_API bool oms_file_hist_delete PARAM_LIST((oms_file_hist_tp *fh));
IDE_OMS_API bool oms_file_hist_update PARAM_LIST((oms_file_hist_tp *fh));

// oms_file_hist_tp Typed Collection Functions

IDE_OMS_API bool oms_file_hist_coll_add PARAM_LIST((oms_coll_tp *coll, oms_file_hist_tp *fh));
IDE_OMS_API bool oms_file_hist_coll_add_unique PARAM_LIST((oms_coll_tp *coll,
                                                                    oms_file_hist_tp *fh));
IDE_OMS_API oms_coll_tp *oms_file_hist_coll_create PARAM_LIST((const char *query));
IDE_OMS_API bool oms_file_hist_coll_delete PARAM_LIST((oms_coll_tp *coll, oms_file_hist_tp *fh));
IDE_OMS_API bool oms_file_hist_coll_delete_equivalent PARAM_LIST((oms_coll_tp *coll,
                                                                           oms_file_hist_tp *fh));
IDE_OMS_API oms_file_hist_tp *oms_file_hist_coll_get PARAM_LIST((oms_coll_tp *coll, int i));

// oms_file_hist_tp Typed Big Collection Functions

IDE_OMS_API oms_bigcoll_tp *oms_file_hist_bigcoll_create PARAM_LIST((const char *query));
IDE_OMS_API oms_file_hist_tp *oms_file_hist_bigcoll_next PARAM_LIST((oms_bigcoll_tp *bcoll));

// oms_file_hist_tp Navigation Functions

IDE_OMS_API oms_file_tp *oms_file_hist_file PARAM_LIST((oms_file_hist_tp *fh));

// oms_file_lock_tp Creation and Retrieval Functions

IDE_OMS_API oms_file_lock_tp *oms_file_lock_create PARAM_LIST((oms_object_id_tp file_id,
                                                               oms_app_type_tp type));
IDE_OMS_API oms_file_lock_tp *oms_file_lock_find PARAM_LIST((oms_object_id_tp file_id, oms_app_type_tp type));
IDE_OMS_API oms_file_lock_tp *oms_file_lock_find_by_id PARAM_LIST((oms_object_id_tp id));
IDE_OMS_API oms_file_lock_tp *oms_file_lock_find_by_query PARAM_LIST((const char *query));

// oms_file_lock_tp Utility Functions

IDE_OMS_API oms_file_lock_tp *oms_file_lock_copy PARAM_LIST((oms_file_lock_tp *fl));
IDE_OMS_API bool oms_file_lock_equal PARAM_LIST((oms_file_lock_tp *fl1, oms_file_lock_tp *fl2));
IDE_OMS_API void oms_file_lock_free PARAM_LIST((oms_file_lock_tp **fl));
IDE_OMS_API void oms_file_lock_print PARAM_LIST((oms_file_lock_tp *fl, FILE *fp));
IDE_OMS_API void oms_file_lock_print_image_asgn PARAM_LIST((const char *cp));

// oms_file_lock_tp Attribute Access and Assign Functions

IDE_OMS_API oms_object_id_tp oms_file_lock_id PARAM_LIST((oms_file_lock_tp *fl));
IDE_OMS_API oms_object_id_tp oms_file_lock_file_id PARAM_LIST((oms_file_lock_tp *fl));
IDE_OMS_API oms_app_type_tp oms_file_lock_type PARAM_LIST((oms_file_lock_tp *fl));
IDE_OMS_API char *oms_file_lock_user PARAM_LIST((oms_file_lock_tp *fl));
IDE_OMS_API char *oms_file_lock_accessors PARAM_LIST((oms_file_lock_tp *fl));
IDE_OMS_API char *oms_file_lock_accessors_copy PARAM_LIST((oms_file_lock_tp *fl));
IDE_OMS_API char *oms_file_lock_destroyers PARAM_LIST((oms_file_lock_tp *fl));
IDE_OMS_API char *oms_file_lock_destroyers_copy PARAM_LIST((oms_file_lock_tp *fl));
IDE_OMS_API char *oms_file_lock_user_copy PARAM_LIST((oms_file_lock_tp *fl));
IDE_OMS_API void oms_file_lock_file_id_asgn PARAM_LIST((oms_file_lock_tp *fl, oms_object_id_tp file_id));
IDE_OMS_API void oms_file_lock_type_asgn PARAM_LIST((oms_file_lock_tp *fl, oms_app_type_tp type));
IDE_OMS_API void oms_file_lock_user_asgn PARAM_LIST((oms_file_lock_tp *fl, const char *user));
IDE_OMS_API void oms_file_lock_accessors_asgn PARAM_LIST((oms_file_lock_tp *fl,
                                                          const char *accessors));
IDE_OMS_API void oms_file_lock_destroyers_asgn PARAM_LIST((oms_file_lock_tp *fl,
                                                           const char *destroyers));

// oms_file_lock_tp Repository Management Functions

IDE_OMS_API bool oms_file_lock_check PARAM_LIST((oms_file_lock_tp *fl));
IDE_OMS_API bool oms_file_lock_delete PARAM_LIST((oms_file_lock_tp *fl));
IDE_OMS_API bool oms_file_lock_update PARAM_LIST((oms_file_lock_tp *fl));

// oms_file_lock_tp Typed Collection Functions

IDE_OMS_API bool oms_file_lock_coll_add PARAM_LIST((oms_coll_tp *coll, oms_file_lock_tp *fl));
IDE_OMS_API bool oms_file_lock_coll_add_unique PARAM_LIST((oms_coll_tp *coll,
                                                                    oms_file_lock_tp *fl));
IDE_OMS_API oms_coll_tp *oms_file_lock_coll_create PARAM_LIST((const char *query));
IDE_OMS_API bool oms_file_lock_coll_delete PARAM_LIST((oms_coll_tp *coll, oms_file_lock_tp *fl));
IDE_OMS_API bool oms_file_lock_coll_delete_equivalent PARAM_LIST((oms_coll_tp *coll,
                                                                           oms_file_lock_tp *fl));
IDE_OMS_API oms_file_lock_tp *oms_file_lock_coll_get PARAM_LIST((oms_coll_tp *coll, int i));

// oms_file_lock_tp Typed Big Collection Functions

IDE_OMS_API oms_bigcoll_tp *oms_file_lock_bigcoll_create PARAM_LIST((const char *query));
IDE_OMS_API oms_file_lock_tp *oms_file_lock_bigcoll_next PARAM_LIST((oms_bigcoll_tp *bcoll));

// oms_file_lock_tp Navigation Functions

IDE_OMS_API oms_file_tp *oms_file_lock_file PARAM_LIST((oms_file_lock_tp *fl));

// oms_file_tp Creation and Retrieval Functions

IDE_OMS_API oms_file_tp *oms_file_create PARAM_LIST((const char *name,
                                                     oms_app_type_tp type));
IDE_OMS_API oms_file_tp *oms_file_find PARAM_LIST((const char *name,
                                                   oms_app_type_tp type));
IDE_OMS_API oms_file_tp *oms_file_find_by_id PARAM_LIST((oms_object_id_tp id));
IDE_OMS_API oms_file_tp *oms_file_find_by_query PARAM_LIST((const char *query));

// oms_file_tp Utility Functions

IDE_OMS_API oms_file_tp *oms_file_copy PARAM_LIST((oms_file_tp *file));
IDE_OMS_API bool oms_file_equal PARAM_LIST((oms_file_tp *file1,
                                                      oms_file_tp *file2));
IDE_OMS_API void oms_file_free PARAM_LIST((oms_file_tp **file));
IDE_OMS_API void oms_file_print PARAM_LIST((oms_file_tp *file, FILE *fp));
IDE_OMS_API void oms_file_print_image_asgn PARAM_LIST((const char *cp));

// oms_file_tp Attribute Access and Assign Functions

IDE_OMS_API oms_object_id_tp oms_file_id PARAM_LIST((oms_file_tp *file));
IDE_OMS_API char *oms_file_name PARAM_LIST((oms_file_tp *file));
IDE_OMS_API char *oms_file_name_copy PARAM_LIST((oms_file_tp *file));
IDE_OMS_API void oms_file_name_asgn PARAM_LIST((oms_file_tp *file,
                                                const char *name));
// ECR 7046 begin
IDE_OMS_API char *oms_file_guid PARAM_LIST((oms_file_tp *file));
IDE_OMS_API char *oms_file_guid_copy PARAM_LIST((oms_file_tp *file));
IDE_OMS_API void oms_file_guid_asgn PARAM_LIST((oms_file_tp *file,
                                                const char *guid));
// ECR 7046 end
IDE_OMS_API oms_app_type_tp oms_file_type PARAM_LIST((oms_file_tp *file));
IDE_OMS_API void oms_file_type_asgn PARAM_LIST((oms_file_tp *file,
                                                oms_app_type_tp type));
IDE_OMS_API char *oms_file_rev PARAM_LIST((oms_file_tp *file));
IDE_OMS_API char *oms_file_rev_copy PARAM_LIST((oms_file_tp *file));
IDE_OMS_API void oms_file_rev_asgn PARAM_LIST((oms_file_tp *file, const char *rev));
IDE_OMS_API char *oms_file_fname PARAM_LIST((oms_file_tp *file));
IDE_OMS_API char *oms_file_fname_copy PARAM_LIST((oms_file_tp *file));
IDE_OMS_API void oms_file_fname_asgn PARAM_LIST((oms_file_tp *file,
                                                 const char *fname));
IDE_OMS_API oms_object_id_tp oms_file_lview_id PARAM_LIST((oms_file_tp *file));
IDE_OMS_API void oms_file_lview_id_asgn PARAM_LIST((oms_file_tp *file,
                                                    oms_object_id_tp lview_id));
IDE_OMS_API oms_object_id_tp oms_file_lmdfy_id PARAM_LIST((oms_file_tp *file));
IDE_OMS_API void oms_file_lmdfy_id_asgn PARAM_LIST((oms_file_tp *file,
                                                    oms_object_id_tp lmdfy_id));
IDE_OMS_API oms_object_id_tp oms_file_lsync_id PARAM_LIST((oms_file_tp *file));
IDE_OMS_API void oms_file_lsync_id_asgn PARAM_LIST((oms_file_tp *file,
                                                    oms_object_id_tp lsync_id));
IDE_OMS_API oms_object_id_tp oms_file_annot_file_id PARAM_LIST((oms_file_tp *file));
IDE_OMS_API void oms_file_annot_file_id_asgn PARAM_LIST((oms_file_tp *file,
                                                         oms_object_id_tp annot_file_id));

// oms_file_tp Repository Management Functions

IDE_OMS_API bool oms_file_check PARAM_LIST((oms_file_tp *file));
IDE_OMS_API bool oms_file_delete PARAM_LIST((oms_file_tp *file));
IDE_OMS_API bool oms_file_update PARAM_LIST((oms_file_tp *file));

// oms_file_tp Typed Collection Functions

IDE_OMS_API bool oms_file_coll_add PARAM_LIST((oms_coll_tp *coll,
                                                        oms_file_tp *file));
IDE_OMS_API bool oms_file_coll_add_unique PARAM_LIST((oms_coll_tp *coll, oms_file_tp *file));
IDE_OMS_API oms_coll_tp *oms_file_coll_create PARAM_LIST((const char *query));
IDE_OMS_API bool oms_file_coll_delete PARAM_LIST((oms_coll_tp *coll,
                                                           oms_file_tp *file));
IDE_OMS_API bool oms_file_coll_delete_equivalent PARAM_LIST((oms_coll_tp *coll,
                                                                      oms_file_tp *file));
IDE_OMS_API oms_file_tp *oms_file_coll_get PARAM_LIST((oms_coll_tp *coll,
                                                       int i));

// oms_file_tp Typed Big Collection Functions

IDE_OMS_API oms_bigcoll_tp *oms_file_bigcoll_create PARAM_LIST((const char *query));
IDE_OMS_API oms_file_tp *oms_file_bigcoll_next PARAM_LIST((oms_bigcoll_tp *bcoll));

// oms_file_tp Navigation Functions

IDE_OMS_API oms_file_hist_tp *oms_file_lview_hist PARAM_LIST((oms_file_tp *file));
IDE_OMS_API oms_file_hist_tp *oms_file_lmdfy_hist PARAM_LIST((oms_file_tp *file));
IDE_OMS_API oms_file_hist_tp *oms_file_lsync_hist PARAM_LIST((oms_file_tp *file));
IDE_OMS_API oms_file_tp *oms_file_annot_file PARAM_LIST((oms_file_tp *file));
IDE_OMS_API oms_coll_tp *oms_file_notes PARAM_LIST((oms_file_tp *file));
IDE_OMS_API oms_coll_tp *oms_file_link_refs PARAM_LIST((oms_file_tp *file));
IDE_OMS_API oms_coll_tp *oms_file_node_refs PARAM_LIST((oms_file_tp *file));
IDE_OMS_API oms_coll_tp *oms_file_cntx_refs PARAM_LIST((oms_file_tp *file));
IDE_OMS_API oms_coll_tp *oms_file_file_hists PARAM_LIST((oms_file_tp *file));
IDE_OMS_API oms_coll_tp *oms_file_file_locks PARAM_LIST((oms_file_tp *file));

// oms_idlist_tp Functions

IDE_OMS_API bool oms_idlist_create PARAM_LIST((oms_pdm_type_tp pdm_type,
                                                        const char *name,
                                                        const char *query));
IDE_OMS_API void oms_idlist_free PARAM_LIST((const char *name));
IDE_OMS_API void oms_idlist_free_all PARAM_LIST((void));

// oms_item_tp Creation and Retrieval Functions

IDE_OMS_API oms_item_tp *oms_item_create PARAM_LIST((oms_object_id_tp note_id,
                                                     oms_app_type_tp type,
                                                     const char *value));
IDE_OMS_API oms_item_tp *oms_item_find PARAM_LIST((oms_object_id_tp note_id,
                                                   oms_app_type_tp type,
                                                   const char *value));
IDE_OMS_API oms_item_tp *oms_item_find_by_id PARAM_LIST((oms_object_id_tp id));
IDE_OMS_API oms_item_tp *oms_item_find_by_query PARAM_LIST((const char *query));

// oms_item_tp Utility Functions

IDE_OMS_API oms_item_tp *oms_item_copy PARAM_LIST((oms_item_tp *item));
IDE_OMS_API bool oms_item_equal PARAM_LIST((oms_item_tp *item1,
                                                      oms_item_tp *item2));
IDE_OMS_API void oms_item_free PARAM_LIST((oms_item_tp **item));
IDE_OMS_API void oms_item_print PARAM_LIST((oms_item_tp *item, FILE *fp));
IDE_OMS_API void oms_item_print_image_asgn PARAM_LIST((const char *cp));

// oms_item_tp Attribute Access and Assign Functions

IDE_OMS_API oms_object_id_tp oms_item_id PARAM_LIST((oms_item_tp *item));
IDE_OMS_API oms_object_id_tp oms_item_note_id PARAM_LIST((oms_item_tp *item));
IDE_OMS_API void oms_item_note_id_asgn PARAM_LIST((oms_item_tp *item,
                                                   oms_object_id_tp note_id));
IDE_OMS_API oms_app_type_tp oms_item_type PARAM_LIST((oms_item_tp *item));
IDE_OMS_API oms_object_id_tp oms_item_obj_id PARAM_LIST((oms_item_tp *item));
IDE_OMS_API void oms_item_obj_id_asgn PARAM_LIST((oms_item_tp *item,
                                                  oms_object_id_tp obj_id));
IDE_OMS_API char *oms_item_value PARAM_LIST((oms_item_tp *item));
IDE_OMS_API char *oms_item_value_copy PARAM_LIST((oms_item_tp *item));
IDE_OMS_API void oms_item_value_asgn PARAM_LIST((oms_item_tp *item,
                                                 const char *value));

// oms_item_tp Repository Management Functions

IDE_OMS_API bool oms_item_check PARAM_LIST((oms_item_tp *item));
IDE_OMS_API bool oms_item_delete PARAM_LIST((oms_item_tp *item));
IDE_OMS_API bool oms_item_update PARAM_LIST((oms_item_tp *item));

// oms_item_tp Typed Collection Functions

IDE_OMS_API bool oms_item_coll_add PARAM_LIST((oms_coll_tp *coll,
                                                        oms_item_tp *item));
IDE_OMS_API bool oms_item_coll_add_unique PARAM_LIST((oms_coll_tp *coll, oms_item_tp *item));
IDE_OMS_API oms_coll_tp *oms_item_coll_create PARAM_LIST((const char *query));
IDE_OMS_API bool oms_item_coll_delete PARAM_LIST((oms_coll_tp *coll, oms_item_tp *item));
IDE_OMS_API bool oms_item_coll_delete_equivalent PARAM_LIST((oms_coll_tp *coll,
                                                                      oms_item_tp *item));
IDE_OMS_API oms_item_tp *oms_item_coll_get PARAM_LIST((oms_coll_tp *coll,
                                                       int i));

// oms_item_tp Typed Big Collection Functions

IDE_OMS_API oms_bigcoll_tp *oms_item_bigcoll_create PARAM_LIST((const char *query));
IDE_OMS_API oms_item_tp *oms_item_bigcoll_next PARAM_LIST((oms_bigcoll_tp *bcoll));

// oms_item_tp Navigation Functions

IDE_OMS_API oms_note_tp *oms_item_note PARAM_LIST((oms_item_tp *item));
IDE_OMS_API oms_cntx_tp *oms_item_cntx PARAM_LIST((oms_item_tp *item));
IDE_OMS_API oms_file_tp *oms_item_file PARAM_LIST((oms_item_tp *item));
IDE_OMS_API oms_link_tp *oms_item_link PARAM_LIST((oms_item_tp *item));
IDE_OMS_API oms_node_tp *oms_item_node PARAM_LIST((oms_item_tp *item));

// oms_link_ref_tp Creation and Retrieval Functions

IDE_OMS_API oms_link_ref_tp *oms_link_ref_create PARAM_LIST((oms_object_id_tp link_id,
                                                             oms_object_id_tp file_id, const char *appid,
                                                             oms_object_id_tp from_node_ref_id,
                                                             oms_object_id_tp to_node_ref_id));
IDE_OMS_API oms_link_ref_tp *oms_link_ref_find PARAM_LIST((oms_object_id_tp link_id, oms_object_id_tp file_id,
                                                           const char *appid,
                                                           oms_object_id_tp from_node_ref_id,
                                                           oms_object_id_tp to_node_ref_id));
IDE_OMS_API oms_link_ref_tp *oms_link_ref_find_by_id PARAM_LIST((oms_object_id_tp id));
IDE_OMS_API oms_link_ref_tp *oms_link_ref_find_by_query PARAM_LIST((const char *query));

// oms_link_ref_tp Utility Functions

IDE_OMS_API oms_link_ref_tp *oms_link_ref_copy PARAM_LIST((oms_link_ref_tp *lr));
IDE_OMS_API bool oms_link_ref_equal PARAM_LIST((oms_link_ref_tp *lr1,
                                                          oms_link_ref_tp *lr2));
IDE_OMS_API void oms_link_ref_free PARAM_LIST((oms_link_ref_tp **lr));
IDE_OMS_API void oms_link_ref_print PARAM_LIST((oms_link_ref_tp *lr, FILE *fp));
IDE_OMS_API void oms_link_ref_print_image_asgn PARAM_LIST((const char *cp));

// oms_link_ref_tp Attribute Access and Assign Functions

IDE_OMS_API oms_object_id_tp oms_link_ref_id PARAM_LIST((oms_link_ref_tp *lr));
IDE_OMS_API oms_object_id_tp oms_link_ref_link_id PARAM_LIST((oms_link_ref_tp *lr));
IDE_OMS_API void oms_link_ref_link_id_asgn PARAM_LIST((oms_link_ref_tp *lr, oms_object_id_tp link_id));
IDE_OMS_API oms_object_id_tp oms_link_ref_file_id PARAM_LIST((oms_link_ref_tp *lr));
IDE_OMS_API void oms_link_ref_file_id_asgn PARAM_LIST((oms_link_ref_tp *lr,
                                                       oms_object_id_tp file_id));
IDE_OMS_API char *oms_link_ref_appid PARAM_LIST((oms_link_ref_tp *lr));
IDE_OMS_API char *oms_link_ref_appid_copy PARAM_LIST((oms_link_ref_tp *lr));
IDE_OMS_API void oms_link_ref_appid_asgn PARAM_LIST((oms_link_ref_tp *lr,
                                                     const char *appid));
IDE_OMS_API oms_object_id_tp oms_link_ref_from_node_ref_id PARAM_LIST((oms_link_ref_tp *lr));
IDE_OMS_API void oms_link_ref_from_node_ref_id_asgn PARAM_LIST((oms_link_ref_tp *lr,
                                                                oms_object_id_tp from_node_ref_id));
IDE_OMS_API oms_object_id_tp oms_link_ref_to_node_ref_id PARAM_LIST((oms_link_ref_tp *lr));
IDE_OMS_API void oms_link_ref_to_node_ref_id_asgn PARAM_LIST((oms_link_ref_tp *lr,
                                                              oms_object_id_tp to_node_ref_id));
IDE_OMS_API int oms_link_ref_xcoord PARAM_LIST((oms_link_ref_tp *lr));
IDE_OMS_API void oms_link_ref_xcoord_asgn PARAM_LIST((oms_link_ref_tp *lr,
                                                      int xcoord));
IDE_OMS_API int oms_link_ref_ycoord PARAM_LIST((oms_link_ref_tp *lr));
IDE_OMS_API void oms_link_ref_ycoord_asgn PARAM_LIST((oms_link_ref_tp *lr,
                                                      int ycoord));

// oms_link_ref_tp Repository Management Functions

IDE_OMS_API bool oms_link_ref_check PARAM_LIST((oms_link_ref_tp *lr));
IDE_OMS_API bool oms_link_ref_delete PARAM_LIST((oms_link_ref_tp *lr));
IDE_OMS_API bool oms_link_ref_update PARAM_LIST((oms_link_ref_tp *lr));

// oms_link_ref_tp Typed Collection Functions

IDE_OMS_API bool oms_link_ref_coll_add PARAM_LIST((oms_coll_tp *coll, oms_link_ref_tp *lr));
IDE_OMS_API bool oms_link_ref_coll_add_unique PARAM_LIST((oms_coll_tp *coll,
                                                                   oms_link_ref_tp *lr));
IDE_OMS_API oms_coll_tp *oms_link_ref_coll_create PARAM_LIST((const char *query));
IDE_OMS_API bool oms_link_ref_coll_delete PARAM_LIST((oms_coll_tp *coll, oms_link_ref_tp *lr));
IDE_OMS_API bool oms_link_ref_coll_delete_equivalent PARAM_LIST((oms_coll_tp *coll,
                                                                          oms_link_ref_tp *lr));
IDE_OMS_API oms_link_ref_tp *oms_link_ref_coll_get PARAM_LIST((oms_coll_tp *coll,
                                                               int i));

// oms_link_ref_tp Typed Big Collection Functions

IDE_OMS_API oms_bigcoll_tp *oms_link_ref_bigcoll_create PARAM_LIST((const char *query));
IDE_OMS_API oms_link_ref_tp *oms_link_ref_bigcoll_next PARAM_LIST((oms_bigcoll_tp *bcoll));

// oms_link_ref_tp Navigation Functions

IDE_OMS_API oms_link_tp *oms_link_ref_link PARAM_LIST((oms_link_ref_tp *lr));
IDE_OMS_API oms_file_tp *oms_link_ref_file PARAM_LIST((oms_link_ref_tp *lr));
IDE_OMS_API oms_node_ref_tp *oms_link_ref_from_node_ref PARAM_LIST((oms_link_ref_tp *lr));
IDE_OMS_API oms_node_ref_tp *oms_link_ref_to_node_ref PARAM_LIST((oms_link_ref_tp *lr));
IDE_OMS_API oms_coll_tp *oms_link_ref_cntx_refs PARAM_LIST((oms_link_ref_tp *lr));

// oms_link_tp Creation and Retrieval Functions

IDE_OMS_API oms_link_tp *oms_link_create PARAM_LIST((const char *name, oms_app_type_tp type,
                                                     oms_object_id_tp from_node_id,
                                                     oms_object_id_tp to_node_id, const char *sig));
IDE_OMS_API oms_link_tp *oms_link_find PARAM_LIST((const char *name,
                                                   oms_app_type_tp type,
                                                   oms_object_id_tp from_node_id,
                                                   oms_object_id_tp to_node_id,
                                                   const char *sig));
IDE_OMS_API oms_link_tp *oms_link_find_by_id PARAM_LIST((oms_object_id_tp id));
IDE_OMS_API oms_link_tp *oms_link_find_by_query PARAM_LIST((const char *query));

// oms_link_tp Utility Functions

IDE_OMS_API oms_link_tp *oms_link_copy PARAM_LIST((oms_link_tp *link));
IDE_OMS_API bool oms_link_equal PARAM_LIST((oms_link_tp *link1,
                                                      oms_link_tp *link2));
IDE_OMS_API void oms_link_free PARAM_LIST((oms_link_tp **link));
IDE_OMS_API void oms_link_print PARAM_LIST((oms_link_tp *link, FILE *fp));
IDE_OMS_API void oms_link_print_image_asgn PARAM_LIST((const char *cp));

// oms_link_tp Attribute Access and Assign Functions

IDE_OMS_API oms_object_id_tp oms_link_id PARAM_LIST((oms_link_tp *link));
IDE_OMS_API char *oms_link_name PARAM_LIST((oms_link_tp *link));
IDE_OMS_API char *oms_link_name_copy PARAM_LIST((oms_link_tp *link));
IDE_OMS_API void oms_link_name_asgn PARAM_LIST((oms_link_tp *link,
                                                const char *name));
// ECR 5673 begin
IDE_OMS_API char *oms_link_guid PARAM_LIST((oms_link_tp *link));
IDE_OMS_API char *oms_link_guid_copy PARAM_LIST((oms_link_tp *link));
IDE_OMS_API void oms_link_guid_asgn PARAM_LIST((oms_link_tp *link,
                                                const char *guid));
// ECR 5673 end
IDE_OMS_API oms_app_type_tp oms_link_type PARAM_LIST((oms_link_tp *link));
IDE_OMS_API void oms_link_type_asgn PARAM_LIST((oms_link_tp *link,
                                                oms_app_type_tp type));
IDE_OMS_API oms_object_id_tp oms_link_from_node_id PARAM_LIST((oms_link_tp *link));
IDE_OMS_API void oms_link_from_node_id_asgn PARAM_LIST((oms_link_tp *link,
                                                        oms_object_id_tp from_node_id));
IDE_OMS_API oms_object_id_tp oms_link_to_node_id PARAM_LIST((oms_link_tp *link));
IDE_OMS_API void oms_link_to_node_id_asgn PARAM_LIST((oms_link_tp *link,
                                                      oms_object_id_tp to_node_id));
IDE_OMS_API char *oms_link_sig PARAM_LIST((oms_link_tp *link));
IDE_OMS_API char *oms_link_sig_copy PARAM_LIST((oms_link_tp *link));
IDE_OMS_API void oms_link_sig_asgn PARAM_LIST((oms_link_tp *link,
                                               const char *sig));
IDE_OMS_API oms_object_id_tp oms_link_scope_node_id PARAM_LIST((oms_link_tp *link));
IDE_OMS_API void oms_link_scope_node_id_asgn PARAM_LIST((oms_link_tp *link,
                                                         oms_object_id_tp scope_node_id));
IDE_OMS_API oms_object_id_tp oms_link_annot_file_id PARAM_LIST((oms_link_tp *link));
IDE_OMS_API void oms_link_annot_file_id_asgn PARAM_LIST((oms_link_tp *link,
                                                         oms_object_id_tp annot_file_id));

// oms_link_tp Repository Management Functions

IDE_OMS_API bool oms_link_check PARAM_LIST((oms_link_tp *link));
IDE_OMS_API bool oms_link_delete PARAM_LIST((oms_link_tp *link));
IDE_OMS_API bool oms_link_update PARAM_LIST((oms_link_tp *link));

// oms_link_tp Typed Collection Functions

IDE_OMS_API bool oms_link_coll_add PARAM_LIST((oms_coll_tp *coll,
                                                        oms_link_tp *link));
IDE_OMS_API bool oms_link_coll_add_unique PARAM_LIST((oms_coll_tp *coll,
                                                               oms_link_tp *link));
IDE_OMS_API oms_coll_tp *oms_link_coll_create PARAM_LIST((const char *query));
IDE_OMS_API bool oms_link_coll_delete PARAM_LIST((oms_coll_tp *coll,
                                                           oms_link_tp *link));
IDE_OMS_API bool oms_link_coll_delete_equivalent PARAM_LIST((oms_coll_tp *coll,
                                                                      oms_link_tp *link));
IDE_OMS_API oms_link_tp *oms_link_coll_get PARAM_LIST((oms_coll_tp *coll,
                                                       int i));

// oms_link_tp Typed Big Collection Functions

IDE_OMS_API oms_bigcoll_tp *oms_link_bigcoll_create PARAM_LIST((const char *query));
IDE_OMS_API oms_link_tp *oms_link_bigcoll_next PARAM_LIST((oms_bigcoll_tp *bcoll));

// oms_link_tp Navigation Functions

IDE_OMS_API oms_node_tp *oms_link_from_node PARAM_LIST((oms_link_tp *link));
IDE_OMS_API oms_node_tp *oms_link_to_node PARAM_LIST((oms_link_tp *link));
IDE_OMS_API oms_node_tp *oms_link_scope_node PARAM_LIST((oms_link_tp *link));
IDE_OMS_API oms_file_tp *oms_link_annot_file PARAM_LIST((oms_link_tp *link));
IDE_OMS_API oms_coll_tp *oms_link_cntxs PARAM_LIST((oms_link_tp *link));
IDE_OMS_API oms_coll_tp *oms_link_link_refs PARAM_LIST((oms_link_tp *link));
IDE_OMS_API oms_coll_tp *oms_link_notes PARAM_LIST((oms_link_tp *link));

// oms_node_ref_tp Creation and Retrieval Functions

IDE_OMS_API oms_node_ref_tp *oms_node_ref_create PARAM_LIST((oms_object_id_tp node_id,
                                                             oms_object_id_tp file_id, const char *appid));
IDE_OMS_API oms_node_ref_tp *oms_node_ref_find PARAM_LIST((oms_object_id_tp node_id, oms_object_id_tp file_id,
                                                           const char *appid));
IDE_OMS_API oms_node_ref_tp *oms_node_ref_find_by_id PARAM_LIST((oms_object_id_tp id));
IDE_OMS_API oms_node_ref_tp *oms_node_ref_find_by_query PARAM_LIST((const char *query));

// oms_node_ref_tp Utility Functions

IDE_OMS_API oms_node_ref_tp *oms_node_ref_copy PARAM_LIST((oms_node_ref_tp *nr));
IDE_OMS_API bool oms_node_ref_equal PARAM_LIST((oms_node_ref_tp *nr1,
                                                          oms_node_ref_tp *nr2));
IDE_OMS_API void oms_node_ref_free PARAM_LIST((oms_node_ref_tp **nr));
IDE_OMS_API void oms_node_ref_print PARAM_LIST((oms_node_ref_tp *nr, FILE *fp));
IDE_OMS_API void oms_node_ref_print_image_asgn PARAM_LIST((const char *cp));

// oms_node_ref_tp Attribute Access and Assign Functions

IDE_OMS_API oms_object_id_tp oms_node_ref_id PARAM_LIST((oms_node_ref_tp *nr));
IDE_OMS_API oms_object_id_tp oms_node_ref_node_id PARAM_LIST((oms_node_ref_tp *nr));
IDE_OMS_API void oms_node_ref_node_id_asgn PARAM_LIST((oms_node_ref_tp *nr,
                                                       oms_object_id_tp node_id));
IDE_OMS_API oms_object_id_tp oms_node_ref_file_id PARAM_LIST((oms_node_ref_tp *nr));
IDE_OMS_API void oms_node_ref_file_id_asgn PARAM_LIST((oms_node_ref_tp *nr,
                                                       oms_object_id_tp file_id));
IDE_OMS_API char *oms_node_ref_appid PARAM_LIST((oms_node_ref_tp *nr));
IDE_OMS_API char *oms_node_ref_appid_copy PARAM_LIST((oms_node_ref_tp *nr));
IDE_OMS_API void oms_node_ref_appid_asgn PARAM_LIST((oms_node_ref_tp *nr,
                                                     const char *appid));
IDE_OMS_API oms_object_id_tp oms_node_ref_scope_node_ref_id PARAM_LIST((oms_node_ref_tp *nr));
IDE_OMS_API void oms_node_ref_scope_node_ref_id_asgn PARAM_LIST((oms_node_ref_tp *nr,
                                                                 oms_object_id_tp scope_node_ref_id));
IDE_OMS_API int oms_node_ref_xcoord PARAM_LIST((oms_node_ref_tp *nr));
IDE_OMS_API void oms_node_ref_xcoord_asgn PARAM_LIST((oms_node_ref_tp *nr,
                                                      int xcoord));
IDE_OMS_API int oms_node_ref_ycoord PARAM_LIST((oms_node_ref_tp *nr));
IDE_OMS_API void oms_node_ref_ycoord_asgn PARAM_LIST((oms_node_ref_tp *nr,
                                                      int ycoord));

// oms_node_ref_tp Repository Management Functions

IDE_OMS_API bool oms_node_ref_check PARAM_LIST((oms_node_ref_tp *nr));
IDE_OMS_API bool oms_node_ref_delete PARAM_LIST((oms_node_ref_tp *nr));
IDE_OMS_API bool oms_node_ref_update PARAM_LIST((oms_node_ref_tp *nr));

// oms_node_ref_tp Typed Collection Functions

IDE_OMS_API bool oms_node_ref_coll_add PARAM_LIST((oms_coll_tp *coll, oms_node_ref_tp *nr));
IDE_OMS_API bool oms_node_ref_coll_add_unique PARAM_LIST((oms_coll_tp *coll,
                                                                   oms_node_ref_tp *nr));
IDE_OMS_API oms_coll_tp *oms_node_ref_coll_create PARAM_LIST((const char *query));
IDE_OMS_API bool oms_node_ref_coll_delete PARAM_LIST((oms_coll_tp *coll,
                                                               oms_node_tp *nr));
IDE_OMS_API bool oms_node_ref_coll_delete_equivalent PARAM_LIST((oms_coll_tp *coll,
                                                                          oms_node_ref_tp *nr));
IDE_OMS_API oms_node_ref_tp *oms_node_ref_coll_get PARAM_LIST((oms_coll_tp *coll,
                                                               int i));

// oms_node_ref_tp Typed Big Collection Functions

IDE_OMS_API oms_bigcoll_tp *oms_node_ref_bigcoll_create PARAM_LIST((const char *query));
IDE_OMS_API oms_node_ref_tp *oms_node_ref_bigcoll_next PARAM_LIST((oms_bigcoll_tp *bcoll));

// oms_node_ref_tp Navigation Functions

IDE_OMS_API oms_node_tp *oms_node_ref_node PARAM_LIST((oms_node_ref_tp *nr));
IDE_OMS_API oms_file_tp *oms_node_ref_file PARAM_LIST((oms_node_ref_tp *nr));
IDE_OMS_API oms_node_ref_tp *oms_node_ref_scope_node_ref PARAM_LIST((oms_node_ref_tp *nr));
IDE_OMS_API oms_coll_tp *oms_node_ref_scoped_node_refs PARAM_LIST((oms_node_ref_tp *nr));
IDE_OMS_API oms_coll_tp *oms_node_ref_in_link_refs PARAM_LIST((oms_node_ref_tp *nr));
IDE_OMS_API oms_coll_tp *oms_node_ref_out_link_refs PARAM_LIST((oms_node_ref_tp *nr));
IDE_OMS_API oms_coll_tp *oms_node_ref_viewpoints PARAM_LIST((oms_node_ref_tp *nr));

// oms_node_tp Creation and Retrieval Functions

IDE_OMS_API oms_node_tp *oms_node_create PARAM_LIST((const char *name, oms_app_type_tp type,
                                                     oms_object_id_tp scope_node_id, const char *sig));
IDE_OMS_API oms_node_tp *oms_node_find PARAM_LIST((const char *name, oms_app_type_tp type,
                                                   oms_object_id_tp scope_node_id, const char *sig));
IDE_OMS_API oms_node_tp *oms_node_find_by_id PARAM_LIST((oms_object_id_tp id));
IDE_OMS_API oms_node_tp *oms_node_find_by_query PARAM_LIST((const char *query));

// oms_node_tp Utility Functions

IDE_OMS_API oms_node_tp *oms_node_copy PARAM_LIST((oms_node_tp *node));
IDE_OMS_API bool oms_node_equal PARAM_LIST((oms_node_tp *node1,
                                                      oms_node_tp *node2));
IDE_OMS_API void oms_node_free PARAM_LIST((oms_node_tp **node));
IDE_OMS_API void oms_node_print PARAM_LIST((oms_node_tp *node, FILE *fp));
IDE_OMS_API void oms_node_print_image_asgn PARAM_LIST((const char *cp));

// oms_node_tp Attribute Access and Assign Functions

IDE_OMS_API oms_object_id_tp oms_node_id PARAM_LIST((oms_node_tp *node));
IDE_OMS_API char *oms_node_name PARAM_LIST((oms_node_tp *node));
IDE_OMS_API char *oms_node_name_copy PARAM_LIST((oms_node_tp *node));
IDE_OMS_API void oms_node_name_asgn PARAM_LIST((oms_node_tp *node,
                                                const char *name));
// ECR 5673 begin
IDE_OMS_API char *oms_node_guid PARAM_LIST((oms_node_tp *node));
IDE_OMS_API char *oms_node_guid_copy PARAM_LIST((oms_node_tp *node));
IDE_OMS_API void oms_node_guid_asgn PARAM_LIST((oms_node_tp *node,
                                                const char *guid));
// ECR 5673 end
IDE_OMS_API oms_app_type_tp oms_node_type PARAM_LIST((oms_node_tp *node));
IDE_OMS_API void oms_node_type_asgn PARAM_LIST((oms_node_tp *node,
                                                oms_app_type_tp type));
IDE_OMS_API oms_object_id_tp oms_node_scope_node_id PARAM_LIST((oms_node_tp *node));
IDE_OMS_API void oms_node_scope_node_id_asgn PARAM_LIST((oms_node_tp *node,
                                                         oms_object_id_tp scope_node_id));
IDE_OMS_API char *oms_node_sig PARAM_LIST((oms_node_tp *node));
IDE_OMS_API char *oms_node_sig_copy PARAM_LIST((oms_node_tp *node));
IDE_OMS_API void oms_node_sig_asgn PARAM_LIST((oms_node_tp *node,
                                               const char *sig));
IDE_OMS_API oms_object_id_tp oms_node_annot_file_id PARAM_LIST((oms_node_tp *node));
IDE_OMS_API void oms_node_annot_file_id_asgn PARAM_LIST((oms_node_tp *node,
                                                         oms_object_id_tp annot_file_id));

// oms_node_tp Repository Management Functions

IDE_OMS_API bool oms_node_check PARAM_LIST((oms_node_tp *node));
IDE_OMS_API bool oms_node_delete PARAM_LIST((oms_node_tp *node));
IDE_OMS_API bool oms_node_update PARAM_LIST((oms_node_tp *node));

// oms_node_tp Typed Collection Functions

IDE_OMS_API bool oms_node_coll_add PARAM_LIST((oms_coll_tp *coll,
                                                        oms_node_tp *node));
IDE_OMS_API bool oms_node_coll_add_unique PARAM_LIST((oms_coll_tp *coll, oms_node_tp *node));
IDE_OMS_API oms_coll_tp *oms_node_coll_create PARAM_LIST((const char *query));
IDE_OMS_API bool oms_node_coll_delete PARAM_LIST((oms_coll_tp *coll,
                                                           oms_node_tp *node));
IDE_OMS_API bool oms_node_coll_delete_equivalent PARAM_LIST((oms_coll_tp *coll,
                                                                      oms_node_tp *node));
IDE_OMS_API oms_node_tp *oms_node_coll_get PARAM_LIST((oms_coll_tp *coll,
                                                       int i));

// oms_node_tp Typed Big Collection Functions

IDE_OMS_API oms_bigcoll_tp *oms_node_bigcoll_create PARAM_LIST((const char *query));
IDE_OMS_API oms_node_tp *oms_node_bigcoll_next PARAM_LIST((oms_bigcoll_tp *bcoll));

// oms_node_tp Navigation Functions

IDE_OMS_API oms_node_tp *oms_node_scope_node PARAM_LIST((oms_node_tp *node));
IDE_OMS_API oms_file_tp *oms_node_annot_file PARAM_LIST((oms_node_tp *node));
IDE_OMS_API oms_coll_tp *oms_node_in_links PARAM_LIST((oms_node_tp *node));
IDE_OMS_API oms_coll_tp *oms_node_node_refs PARAM_LIST((oms_node_tp *node));
IDE_OMS_API oms_coll_tp *oms_node_notes PARAM_LIST((oms_node_tp *node));
IDE_OMS_API oms_coll_tp *oms_node_out_links PARAM_LIST((oms_node_tp *node));
IDE_OMS_API oms_coll_tp *oms_node_scoped_cntxs PARAM_LIST((oms_node_tp *node));
IDE_OMS_API oms_coll_tp *oms_node_scoped_links PARAM_LIST((oms_node_tp *node));
IDE_OMS_API oms_coll_tp *oms_node_scoped_nodes PARAM_LIST((oms_node_tp *node));
IDE_OMS_API oms_coll_tp *oms_node_viewpoints PARAM_LIST((oms_node_tp *node));

// oms_note_tp Creation and Retrieval Functions

IDE_OMS_API oms_note_tp *oms_note_create PARAM_LIST((oms_object_id_tp obj_id,
                                                     oms_app_type_tp type,
                                                     const char *name));
IDE_OMS_API oms_note_tp *oms_note_find PARAM_LIST((oms_object_id_tp obj_id,
                                                   oms_app_type_tp type,
                                                   const char *name));
IDE_OMS_API oms_note_tp *oms_note_find_by_id PARAM_LIST((oms_object_id_tp id));
IDE_OMS_API oms_note_tp *oms_note_find_by_query PARAM_LIST((const char *query));

// oms_note_tp Utility Functions

IDE_OMS_API oms_note_tp *oms_note_copy PARAM_LIST((oms_note_tp *note));
IDE_OMS_API bool oms_note_equal PARAM_LIST((oms_note_tp *note1,
                                                      oms_note_tp *note2));
IDE_OMS_API void oms_note_free PARAM_LIST((oms_note_tp **note));
IDE_OMS_API void oms_note_print PARAM_LIST((oms_note_tp *note, FILE *fp));
IDE_OMS_API void oms_note_print_image_asgn PARAM_LIST((const char *cp));

// oms_note_tp Attribute Access and Assign Functions

IDE_OMS_API oms_object_id_tp oms_note_id PARAM_LIST((oms_note_tp *note));
IDE_OMS_API oms_object_id_tp oms_note_obj_id PARAM_LIST((oms_note_tp *note));
IDE_OMS_API void oms_note_obj_id_asgn PARAM_LIST((oms_note_tp *note,
                                                  oms_object_id_tp obj_id));
IDE_OMS_API oms_app_type_tp oms_note_type PARAM_LIST((oms_note_tp *note));
IDE_OMS_API char *oms_note_name PARAM_LIST((oms_note_tp *note));
IDE_OMS_API char *oms_note_name_copy PARAM_LIST((oms_note_tp *note));
IDE_OMS_API void oms_note_obj_name_asgn PARAM_LIST((oms_note_tp *note,
                                                    const char *name));
IDE_OMS_API oms_object_id_tp oms_note_file_id PARAM_LIST((oms_note_tp *note));
IDE_OMS_API void oms_note_file_id_asgn PARAM_LIST((oms_note_tp *note,
                                                   oms_object_id_tp file_id));
IDE_OMS_API char *oms_note_desc PARAM_LIST((oms_note_tp *note));
IDE_OMS_API char *oms_note_desc_copy PARAM_LIST((oms_note_tp *note));
IDE_OMS_API void oms_note_desc_asgn PARAM_LIST((oms_note_tp *note,
                                                const char *desc));
IDE_OMS_API void oms_note_obj_type_asgn PARAM_LIST((oms_note_tp *note,
                                                    oms_app_type_tp type));

// oms_note_tp Repository Management Functions

IDE_OMS_API bool oms_note_check PARAM_LIST((oms_note_tp *note));
IDE_OMS_API bool oms_note_delete PARAM_LIST((oms_note_tp *note));
IDE_OMS_API bool oms_note_update PARAM_LIST((oms_note_tp *note));


// oms_note_tp Typed Collection Functions

IDE_OMS_API bool oms_note_coll_add PARAM_LIST((oms_coll_tp *coll,
                                                        oms_note_tp *note));
IDE_OMS_API bool oms_note_coll_add_unique PARAM_LIST((oms_coll_tp *coll,
                                                               oms_note_tp *note));
IDE_OMS_API oms_coll_tp *oms_note_coll_create PARAM_LIST((const char *query));
IDE_OMS_API bool oms_note_coll_delete PARAM_LIST((oms_coll_tp *coll,
                                                           oms_note_tp *note));
IDE_OMS_API bool oms_note_coll_delete_equivalent PARAM_LIST((oms_coll_tp *coll,
                                                                      oms_note_tp *note));
IDE_OMS_API oms_note_tp *oms_note_coll_get PARAM_LIST((oms_coll_tp *coll,
                                                       int i));

// oms_note_tp Typed Big Collection Functions

IDE_OMS_API oms_bigcoll_tp *oms_note_bigcoll_create PARAM_LIST((const char *query));
IDE_OMS_API oms_note_tp *oms_note_bigcoll_next PARAM_LIST((oms_bigcoll_tp *bcoll));

// oms_note_tp Navigation Functions

IDE_OMS_API oms_file_tp *oms_note_annot_file PARAM_LIST((oms_note_tp *note));
IDE_OMS_API oms_cntx_tp *oms_note_cntx PARAM_LIST((oms_note_tp *note));
IDE_OMS_API oms_file_tp *oms_note_file PARAM_LIST((oms_note_tp *note));
IDE_OMS_API oms_coll_tp *oms_note_items PARAM_LIST((oms_note_tp *note));
IDE_OMS_API oms_link_tp *oms_note_link PARAM_LIST((oms_note_tp *note));
IDE_OMS_API oms_node_tp *oms_note_node PARAM_LIST((oms_note_tp *note));

// oms_object_id_tp Function

IDE_OMS_API oms_object_id_tp oms_object_id_new PARAM_LIST((void));

// oms_pdm_type_tp Functions

IDE_OMS_API char *oms_pdm_type_enum_to_text PARAM_LIST((oms_pdm_type_tp pdm_type));
IDE_OMS_API oms_pdm_type_tp oms_pdm_type_text_to_enum PARAM_LIST((const char *name));

// oms_repos_tp Functions

IDE_OMS_API bool oms_repos_open PARAM_LIST((const char *repositoryType,
                                                     const char *server_name,
                                                     const char *database_name));
IDE_OMS_API bool oms_repos_open PARAM_LIST((const char *server_name,
                                                     const char *database_name));
IDE_OMS_API bool oms_repos_close PARAM_LIST((void));
IDE_OMS_API char *oms_repos_current_rep_name PARAM_LIST((void));
IDE_OMS_API char *oms_repos_current_server_name PARAM_LIST((void));
IDE_OMS_API bool oms_sys_repos_open PARAM_LIST((const char *projdir,
                                                         const char *system));

// oms_time_tp Functions

IDE_OMS_API char *oms_time_input_format_asgn PARAM_LIST((const char *in_format));
IDE_OMS_API char *oms_time_lctime_env_asgn PARAM_LIST((const char *lang));
IDE_OMS_API oms_time_tp oms_time_now PARAM_LIST((void));
IDE_OMS_API char *oms_time_output_format_asgn PARAM_LIST((const char *out_format));
IDE_OMS_API oms_time_tp oms_time_text_to_time PARAM_LIST((const char *cp));
IDE_OMS_API char *oms_time_to_text PARAM_LIST((oms_time_tp time));

// oms_txn_tp Functions

IDE_OMS_API bool oms_txn_abort PARAM_LIST((void));
IDE_OMS_API int oms_txn_begin PARAM_LIST((void));
IDE_OMS_API int oms_txn_commit PARAM_LIST((void));

// oms_viewpoint_tp Creation and Retrieval Functions

IDE_OMS_API oms_viewpoint_tp *oms_viewpoint_create PARAM_LIST((oms_object_id_tp node_id,
                                                               oms_object_id_tp node_ref_id,
                                                               oms_app_type_tp type));
IDE_OMS_API oms_viewpoint_tp *oms_viewpoint_find PARAM_LIST((oms_object_id_tp node_id,
                                                             oms_object_id_tp node_ref_id,
                                                             oms_app_type_tp type));
IDE_OMS_API oms_viewpoint_tp *oms_viewpoint_find_by_id PARAM_LIST((oms_object_id_tp id));
IDE_OMS_API oms_viewpoint_tp *oms_viewpoint_find_by_query PARAM_LIST((const char *query));

// oms_viewpoint_tp Utility Functions

IDE_OMS_API oms_viewpoint_tp *oms_viewpoint_copy PARAM_LIST((oms_viewpoint_tp *vp));
IDE_OMS_API bool oms_viewpoint_equal PARAM_LIST((oms_viewpoint_tp *vp1,
                                                           oms_viewpoint_tp *vp2));
IDE_OMS_API void oms_viewpoint_free PARAM_LIST((oms_viewpoint_tp **vp));
IDE_OMS_API void oms_viewpoint_print PARAM_LIST((oms_viewpoint_tp *vp,
                                                 FILE *fp));
IDE_OMS_API void oms_viewpoint_print_image_asgn PARAM_LIST((const char *cp));

// oms_viewpoint_tp Attribute Access and Assign Functions

IDE_OMS_API oms_object_id_tp oms_viewpoint_id PARAM_LIST((oms_viewpoint_tp *vp));
IDE_OMS_API oms_object_id_tp oms_viewpoint_node_id PARAM_LIST((oms_viewpoint_tp *vp));
IDE_OMS_API void oms_viewpoint_node_id_asgn PARAM_LIST((oms_viewpoint_tp *vp,
                                                        oms_object_id_tp node_id));
IDE_OMS_API oms_app_type_tp oms_viewpoint_type PARAM_LIST((oms_viewpoint_tp *vp));
IDE_OMS_API void oms_viewpoint_type_asgn PARAM_LIST((oms_viewpoint_tp *vp,
                                                     oms_app_type_tp type));
IDE_OMS_API oms_object_id_tp oms_viewpoint_node_ref_id PARAM_LIST((oms_viewpoint_tp *vp));
IDE_OMS_API void oms_viewpoint_node_ref_id_asgn PARAM_LIST((oms_viewpoint_tp *vp,
                                                            oms_object_id_tp nr_id));


// oms_viewpoint_tp Repository Management Functions

IDE_OMS_API bool oms_viewpoint_check PARAM_LIST((oms_viewpoint_tp *vp));
IDE_OMS_API bool oms_viewpoint_delete PARAM_LIST((oms_viewpoint_tp *vp));
IDE_OMS_API bool oms_viewpoint_update PARAM_LIST((oms_viewpoint_tp *vp));


// oms_viewpoint_tp Typed Collection Functions

IDE_OMS_API bool oms_viewpoint_coll_add PARAM_LIST((oms_coll_tp *coll, oms_viewpoint_tp *vp));
IDE_OMS_API bool oms_viewpoint_coll_add_unique PARAM_LIST((oms_coll_tp *coll,
                                                                    oms_viewpoint_tp *vp));
IDE_OMS_API oms_coll_tp *oms_viewpoint_coll_create PARAM_LIST((const char *query));
IDE_OMS_API bool oms_viewpoint_coll_delete PARAM_LIST((oms_coll_tp *coll,
                                                                oms_viewpoint_tp *vp));
IDE_OMS_API bool oms_viewpoint_coll_delete_equivalent PARAM_LIST((oms_coll_tp *coll,
                                                                           oms_viewpoint_tp *vp));
IDE_OMS_API oms_viewpoint_tp *oms_viewpoint_coll_get PARAM_LIST((oms_coll_tp *coll, int i));


// oms_viewpoint_tp Typed Big Collection Functions

IDE_OMS_API oms_bigcoll_tp *oms_viewpoint_bigcoll_create PARAM_LIST((const char *query));
IDE_OMS_API oms_viewpoint_tp *oms_viewpoint_bigcoll_next PARAM_LIST((oms_bigcoll_tp *bcoll));

// oms_viewpoint_tp Navigation Functions

IDE_OMS_API oms_node_tp *oms_viewpoint_node PARAM_LIST((oms_viewpoint_tp *vp));
IDE_OMS_API oms_node_ref_tp *oms_viewpoint_node_ref PARAM_LIST((oms_viewpoint_tp *vp));

#undef PARAM_LIST

#endif /* __OMS_API_INCLUDED__ */
