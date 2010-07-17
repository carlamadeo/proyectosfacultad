#ifndef OMS_MACROS_H
#define OMS_MACROS_H

/*
 * Confidential property of Interactive Development Environments, Inc. *
 * Copyright (c) 1992-1993 *    All rights reserved
 */

/************************************************************************
 *
 *
 * File:  oms_macros.h
 *
 *
 *
 ************************************************************************/


#define oms_cntx_ref_set_get(s, i)      \
        (oms_cntx_ref_tp *) gu_set_get_member((gu_set_tp) s, i)

#define oms_dirty_ref_set_get(s, i)     \
        (oms_dirty_ref_tp *) gu_set_get_member((gu_set_tp) s, i)

#define oms_cntx_set_get(s, i)  \
        (oms_cntx_tp *) gu_set_get_member((gu_set_tp) s, i)

#define oms_desc_set_get(s, i)  \
        (oms_desc_tp *) gu_set_get_member((gu_set_tp) s, i)

#define oms_file_hist_set_get(s, i)     \
        (oms_file_hist_tp *) gu_set_get_member((gu_set_tp) s, i)

#define oms_file_lock_set_get(s, i)     \
        (oms_file_lock_tp *) gu_set_get_member((gu_set_tp) s, i)

#define oms_file_set_get(s, i)  \
        (oms_file_tp *) gu_set_get_member((gu_set_tp) s, i)

#define oms_item_set_get(s, i)  \
        (oms_item_tp *) gu_set_get_member((gu_set_tp) s, i)

#define oms_node_set_get(s, i)  \
        (oms_node_tp *) gu_set_get_member((gu_set_tp) s, i)

#define oms_link_set_get(s, i)  \
        (oms_link_tp *) gu_set_get_member((gu_set_tp) s, i)

#define oms_node_ref_set_get(s, i)      \
        (oms_node_ref_tp *) gu_set_get_member((gu_set_tp) s, i)

#define oms_link_ref_set_get(s, i)      \
        (oms_link_ref_tp *) gu_set_get_member((gu_set_tp) s, i)

#define oms_note_set_get(s, i)  \
        (oms_note_tp *) gu_set_get_member((gu_set_tp) s, i)

#define oms_rename_label_set_get(s, i)  \
        (oms_rename_label_tp *) gu_set_get_member((gu_set_tp) s, i)

#define oms_viewpoint_set_get(s, i)     \
        (oms_viewpoint_tp *) gu_set_get_member((gu_set_tp) s, i)

#include "misc_macros.h"

#endif /* OMS_MACROS_H */

