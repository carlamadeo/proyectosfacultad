/************************************************************************
 *
 *
 * File:  gu_set.h
 *
 *
 * Functions:
 *
 *
 * Changes:
 *      6/9/90, PP: Added gu_set_create_empty define to just create an
 *              empty set.
 *      6/4/96, PN: Added gu_set_free, with interface that can be called
 *              when sets are embedded within string_maps
 *
 ************************************************************************/

#ifndef GU_SET_H
#define GU_SET_H

#include "IdeToolsApi.h"
#include "IdeColl.h"

#ifndef _GU_SET_ELEM_TP_DEF_
typedef void    *gu_set_elem_tp;
#define GU_SET_NULL_ELEM        ((gu_set_elem_tp)0)
#define _GU_SET_ELEM_TP_DEF_
#endif                          /* _GU_SET_ELEM_TP_DEF_ */

#include <stdio.h>

#ifndef _GU_SET_STRUCT_TP_DEF_

typedef int     (*comparefunc_tp) (gu_set_elem_tp, gu_set_elem_tp);
typedef void    (*destroyfunc_tp) (gu_set_elem_tp);
typedef         gu_set_elem_tp(*gu_set_copyfunc_tp) (gu_set_elem_tp);

typedef struct _gu_set_struct_
{
    int             set_num_alloc, set_num_el;
    gu_set_elem_tp *set_elem;
    comparefunc_tp  gu_set_cmp_el;
    destroyfunc_tp  gu_set_destroy_el;
    int             trap_duplicates_flag;
}               gu_set_impl_tp;

typedef gu_set_impl_tp *gu_set_tp;

#define _GU_SET_STRUCT_TP_DEF_
#endif                          /* _GU_SET_STRUCT_TP_DEF_ */

#ifndef gu_set_null
#define gu_set_null     ((gu_set_tp) 0)
#endif                          /* gu_set_null */

#ifndef GU_SET_ERROR
#define GU_SET_ERROR  (-1)
#endif                          /* GU_SET_ERROR */


#define gu_set_create_empty()   gu_set_create(GU_SET_NULL_ELEM)

IDE_TOOLS_API gu_set_tp gu_set_copy(gu_set_tp rspa);
IDE_TOOLS_API gu_set_tp gu_set_deep_copy(gu_set_tp rspa, gu_set_copyfunc_tp copy_el_fun);
IDE_TOOLS_API gu_set_tp gu_set_create(gu_set_elem_tp first,...);
IDE_TOOLS_API void     gu_set_cmp_el_fun(gu_set_tp rspa, comparefunc_tp cmp_fun);
IDE_TOOLS_API void     gu_set_trap_duplicates(gu_set_tp rspa, int flag);
IDE_TOOLS_API void     gu_set_destroy_el_fun(gu_set_tp rspa, destroyfunc_tp destroy_fn);
IDE_TOOLS_API void     gu_set_free(gu_set_tp rspa);
IDE_TOOLS_API void     gu_set_destroy(gu_set_tp * rspa);
IDE_TOOLS_API int      gu_set_add(gu_set_tp rspa, gu_set_elem_tp elm);
IDE_TOOLS_API int      gu_set_delete(gu_set_tp rspa, gu_set_elem_tp elm);
IDE_TOOLS_API int      gu_set_clear(gu_set_tp rspa);
IDE_TOOLS_API int      gu_set_count(gu_set_tp rspa);
IDE_TOOLS_API int      gu_set_is_empty(gu_set_tp rspa);
IDE_TOOLS_API int      gu_set_union(gu_set_tp rspa, gu_set_tp rspa1);
IDE_TOOLS_API int      gu_set_is_member(gu_set_elem_tp el, gu_set_tp rspa);
IDE_TOOLS_API int      gu_set_find_member(gu_set_elem_tp el, gu_set_tp rspa);
IDE_TOOLS_API int      gu_set_is_subset(gu_set_tp rspa1, gu_set_tp rspa);
IDE_TOOLS_API int      gu_set_is_proper_subset(gu_set_tp rspa1, gu_set_tp rspa);
IDE_TOOLS_API int      gu_set_inter(gu_set_tp rspa, gu_set_tp rspa1);
IDE_TOOLS_API int      gu_set_diff(gu_set_tp rspa, gu_set_tp rspa1);
IDE_TOOLS_API int      gu_set_delete_member(gu_set_tp rspa, int which);
IDE_TOOLS_API gu_set_elem_tp gu_set_get_member(gu_set_tp rspa, int which);
IDE_TOOLS_API int      gu_set_sort(gu_set_tp rspa, comparefunc_tp cmp_fun);
IDE_TOOLS_API gu_set_tp gu_set_from_string(char *string, char *seperators);
IDE_TOOLS_API char    *gu_set_to_string(gu_set_tp set);
IDE_TOOLS_API void gu_set_add_to_collection(gu_set_tp aSet, IdeTStringColl& coll);
IDE_TOOLS_API gu_set_tp gu_set_create_from_collection(IdeTStringColl& coll);
IDE_TOOLS_API int      gu_set_print(gu_set_tp rspa);
IDE_TOOLS_API int      gu_set_print_fp(gu_set_tp rspa, FILE * fp);

#define FOR_EACH_ELEM_IN_SET(s,e,i,t) \
        for (i=gu_set_count(s), e = (t) gu_set_get_member(s,i-1); \
            (s) && i-- > 0; \
            e = (t) gu_set_get_member(s,i-1))

#define FOR_EACH_ELEM_IN_SET_INCR(s,e,i,t) \
        for (i = 0; ((i < gu_set_count(s))? \
                        (e = (t) gu_set_get_member(s,i)),1:0); i++)


#define FOR_EACH_ELEM_IN_ENUM_SET_INCR(s,e,i,t,j) \
        for (i = 0; ((i < gu_set_count(s)) && \
                    ((j = (int) gu_set_get_member(s,i)) == j) && \
                    ((e = (t) j) == e)); i++)

#endif /* GU_SET_H */

