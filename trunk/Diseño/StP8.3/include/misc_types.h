/*
 * Confidential property of Interactive Development Environments, Inc. *
 * Copyright (c) 1992-1998 *    All rights reserved
 */

/************************************************************************
 *
 *
 * File:  misc_types.h
 *
 *
 *
 ************************************************************************/


#ifndef __MISC_TYPES_H_DEFINED__
#define __MISC_TYPES_H_DEFINED__

#include "oms_general.h"
#include "IdeString.h"

/*
 * The following typedef is used with search_case. Respect indicates that the
 * user wants to repsect case in searches; Ignore indicates that the user
 * wants to ignore case in searches.
 */

typedef enum
{
    Respect,
    Ignore
}               search_case_tp;

/* Unique identifier of a graphical object */

typedef unsigned long index_tp;

typedef short   symbol_id_tp;

/*
 * Typedef to make function declarations easier I never remember the syntax
 * for a function returning a pointer to a function
 */

typedef int     (*FUNCTION_PTR)();

typedef short   diag_zoom_tp;
typedef int     coord_tp;
typedef int     dimen_tp;

typedef struct
{
    coord_tp        r_left;
    coord_tp        r_top;
    coord_tp        r_width;
    coord_tp        r_height;
}               coord_rect_tp;

typedef struct __dbfiles
{
    char           *dg_name;
    long            dg_defpt;
}               DbName;


typedef char*   (*char_ptr_func_tp)();
typedef void*   (*void_ptr_func_tp)();
typedef int     (*int_func_tp)();
typedef void    (*void_func_tp)();
typedef bool    (*oms_status_tp_func_tp)();
typedef int     (*notifyfunc_tp)(int pid, int s, char* msg);

typedef bool    (*ui_menu_active_func_tp)(void*);
typedef bool    (*ui_menu_label_func_tp)(IdeTString&, void*);

#endif /* __MISC_TYPES_H_DEFINED__ */
