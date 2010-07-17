/************************************************************************
 *
 * File:  gu_str.h
 *
 * Macros:
 *
 * Changes:
 *      11/30/89, RJM:  Added declaration for gu_str_from_*
 *      3/7/90, RJM:    Added gu_str_trim, GU_StrSide
 *      4/27/90, DLW:   Added gu_str_equal macro
 *
 *
 ************************************************************************/

#ifndef gu_str_DEFINED
#define gu_str_DEFINED          1

#include "IdeToolsApi.h"

#include <string.h>
#include <ctype.h>
#include "IdeString.h"

typedef enum
{
    GU_STR_UPPER_CASE,
    GU_STR_LOWER_CASE,
    GU_STR_MIXED_CASE,
    GU_STR_INITCAPPED_CASE
}               GU_StrCase;

typedef enum
{
    GU_STR_LEFT_JUSTIFY,
    GU_STR_CENTER_JUSTIFY,
    GU_STR_RIGHT_JUSTIFY
}               GU_StrJustify;

typedef enum
{
    GU_STR_LEFT,
    GU_STR_RIGHT,
    GU_STR_BOTH_SIDES,
    GU_STR_NO_SIDE              /* used for NULL if necessary */
}               GU_StrSide;

IDE_TOOLS_API char    *gu_str_abbrev(char *s, int fieldlen, int delim);
IDE_TOOLS_API int      gu_str_contains(char s2[], char s1[]);
IDE_TOOLS_API char    *gu_str_copyn(char *first,...);
IDE_TOOLS_API char    *gu_str_substring(char *s1, int base, int bound, int padch);
IDE_TOOLS_API char    *gu_str_subword(char *s1, int n, int length, int delim);
IDE_TOOLS_API char    *gu_str_from_int(int i);
IDE_TOOLS_API char    *gu_str_from_long(long l);
IDE_TOOLS_API void     gu_str_lit_debug(int c);
IDE_TOOLS_API char    *gu_str_lit(char *text);
IDE_TOOLS_API char    *gu_str_case(unsigned char *s1, GU_StrCase type, int delim);
IDE_TOOLS_API char    *gu_str_trim(CTSTR string, GU_StrSide side, char *delims);

/*** this macro returns a non-zero value if the two strings are
 *** equal. This macro is different from a simple call to strcmp
 *** because it equates NULL pointers and empty strings.
 ***/

#define gu_str_equal(s1, s2)    \
        ( (  !(s1) && !(s2)  ) ||       \
          (  (s1) && !*(s1) && !(s2)  ) ||      \
          (  !(s1) && (s2) && !*(s2)  ) ||      \
          (  (s1) && (s2) && !strcmp((s1), (s2)))  )

#endif                          /* gu_str_DEFINED */
