/************************************************************************
 *
 *
 * File:  gu_list.h
 *
 ************************************************************************/

#ifndef __GU_LIST_H_DEFINED
#define __GU_LIST_H_DEFINED

#include "IdeToolsApi.h"

typedef void   *gu_list_item_handle;

typedef void (*gu_list_destroyfunc_tp) (gu_list_item_handle);
typedef gu_list_item_handle (*gu_list_copyfunc_tp) (gu_list_item_handle);
typedef int (*gu_list_comparefunc_tp) (gu_list_item_handle, gu_list_item_handle);

typedef struct
{
    int                     item_num;
    int                     items_allocated;
    int                     alloc_size;
    gu_list_item_handle    *items;
    gu_list_destroyfunc_tp  destroy_item;
    gu_list_copyfunc_tp     copy_item;
    gu_list_comparefunc_tp  compare_items;
}               gu_list_tp;

typedef gu_list_tp *gu_list_handle;

#define GU_LIST_ILLEGAL_HANDLE (-1)
#define GU_LIST_ILLEGAL_INDEX (-2)
#define GU_LIST_ALLOC_FAILED (-3)
#define GU_LIST_OK 1

IDE_TOOLS_API gu_list_handle gu_list_create(void);
IDE_TOOLS_API int gu_list_append_item(gu_list_handle list, gu_list_item_handle item);
IDE_TOOLS_API int      gu_list_length(gu_list_handle list);
IDE_TOOLS_API gu_list_item_handle gu_list_get_item(gu_list_handle list, int index);
IDE_TOOLS_API int      gu_list_delete_item(gu_list_handle list, int index);
IDE_TOOLS_API int gu_list_insert_item(gu_list_handle list, int index, gu_list_item_handle item);
IDE_TOOLS_API int gu_list_set_item(gu_list_handle list, int index, gu_list_item_handle item);
IDE_TOOLS_API int      gu_list_destroy(gu_list_handle * list);
IDE_TOOLS_API int gu_list_set_destroy_item( gu_list_handle list, void (*f) (gu_list_item_handle));
IDE_TOOLS_API void     gu_list_clear(gu_list_handle list);
IDE_TOOLS_API gu_list_handle gu_list_copy(gu_list_handle list);
IDE_TOOLS_API int gu_list_set_copy_item(gu_list_handle list, gu_list_item_handle(*f) (gu_list_item_handle));
IDE_TOOLS_API int gu_list_concatenate(gu_list_handle l1, gu_list_handle l2);
IDE_TOOLS_API int gu_list_set_alloc_size(gu_list_handle list, int alloc_size);
IDE_TOOLS_API int gu_list_find_item(gu_list_handle list, gu_list_item_handle item);
IDE_TOOLS_API int gu_list_find_next_item(gu_list_handle list, gu_list_item_handle item, int start_pos);
IDE_TOOLS_API int gu_list_sort(gu_list_handle list, gu_list_comparefunc_tp cmp_items);
IDE_TOOLS_API int gu_list_set_compare_items( gu_list_handle list, gu_list_comparefunc_tp cmp_items);

#endif                          /* __GU_LIST_H_DEFINED */
