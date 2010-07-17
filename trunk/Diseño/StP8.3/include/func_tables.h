/************************************************************************
 *
 * File:  func_tables.h
 *
 ************************************************************************/

// these are really hooks prototypes.  Don't be fooled by the name, which
// is like a cfile in tools.

#ifndef __FUNC_TABLES_H_DEFINED__

#include "IdeHooksApi.h"
#include "IdeTChar.h"

#include       "misc_types.h"
//# include     "misc_constants.h"

typedef struct __status_func_entry_tp__
{
    char           *func_name;
    oms_status_tp_func_tp the_func;
}               status_func_entry_tp;

typedef struct __status_label_func_entry_tp__
{
    char           *func_name;
    ui_menu_label_func_tp the_func;
}               status_label_func_entry_tp;

typedef struct __void_ptr_func_entry_tp__
{
    char           *func_name;
    void_ptr_func_tp the_func;
}               void_ptr_func_entry_tp;

IDE_HOOKS_API oms_status_tp_func_tp node_name_func_lookup(CTSTR func_name);
IDE_HOOKS_API oms_status_tp_func_tp link_names_func_lookup(CTSTR func_name);
IDE_HOOKS_API oms_status_tp_func_tp cntx_names_func_lookup(CTSTR func_name);
IDE_HOOKS_API oms_status_tp_func_tp node_scope_func_lookup(CTSTR func_name);
IDE_HOOKS_API oms_status_tp_func_tp link_scope_func_lookup(CTSTR func_name);
IDE_HOOKS_API oms_status_tp_func_tp cntx_scope_func_lookup(CTSTR func_name);
IDE_HOOKS_API oms_status_tp_func_tp node_signature_func_lookup(CTSTR func_name);
IDE_HOOKS_API oms_status_tp_func_tp link_signature_func_lookup(CTSTR func_name);
IDE_HOOKS_API oms_status_tp_func_tp cntx_signature_func_lookup(CTSTR func_name);
IDE_HOOKS_API oms_status_tp_func_tp node_constructor_func_lookup(CTSTR func_name);
IDE_HOOKS_API oms_status_tp_func_tp arc_constructor_func_lookup(CTSTR func_name);
IDE_HOOKS_API oms_status_tp_func_tp cntx_constructor_func_lookup(CTSTR func_name);
IDE_HOOKS_API oms_status_tp_func_tp node_destructor_func_lookup(CTSTR func_name);
IDE_HOOKS_API oms_status_tp_func_tp arc_destructor_func_lookup(CTSTR func_name);
IDE_HOOKS_API oms_status_tp_func_tp cntx_destructor_func_lookup(CTSTR func_name);
IDE_HOOKS_API oms_status_tp_func_tp note_name_func_lookup(CTSTR func_name);
IDE_HOOKS_API oms_status_tp_func_tp note_desc_func_lookup(CTSTR func_name);
IDE_HOOKS_API oms_status_tp_func_tp item_value_func_lookup(CTSTR func_name);
IDE_HOOKS_API void_ptr_func_tp dependent_symbols_func_lookup(CTSTR func_name);
IDE_HOOKS_API void_ptr_func_tp symbol_func_lookup(CTSTR func_name);



#define __FUNC_TABLES_H_DEFINED__
#endif                         /* __FUNC_TABLES_H_DEFINED__ */
