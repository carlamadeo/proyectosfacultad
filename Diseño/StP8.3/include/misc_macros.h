/*
 * Confidential property of Interactive Development Environments, Inc. *
 * Copyright (c) 1992-1998 *    All rights reserved
 */

/************************************************************************
 *
 *
 * File:  misc_macros.h
 *
 *
 *
 ************************************************************************/

#ifndef __MISC_MACROS_H_DEFINED__
#define __MISC_MACROS_H_DEFINED__

#ifdef WIN32
#define strcasecmp stricmp
#define strncasecmp strnicmp
#endif

#define safe_print(s)           (s && *s)?(s):"null"
#define safe_strcat(s1, s2)     (s1 && s2)?strcat(s1, s2):(s1)
#define safe_strlen(s)          ((s)?strlen(s):0)

#define safe_strcmp(s1, s2)     ((!s1||!*s1)?((!s2||!*s2)?0:-1):\
                                 (!s2||!*s2)?1:strcmp(s1,s2))

#define safe_strequal(s1, s2)   (((!s1 || !*s1) && (!s2 || !*s2))?1:\
                                ((!s1 || !s2)?0:!strcmp(s1, s2)))

#define safe_strnequal(s1, s2, n)\
                                (((!s1 || !*s1) && (!s2 || !*s2))?1:\
                                ((!s1 || !s2)?0:!strncmp(s1, s2, n)))

#define safe_strcaseequal(s1, s2) \
                                (((!s1 || !*s1) && (!s2 || !*s2))?1:\
                                ((!s1 || !s2)?0:!strcasecmp(s1, s2)))

#define safe_strncaseequal(s1, s2, n) \
                                (((!s1 || !*s1) && (!s2 || !*s2))?1:\
                                ((!s1 || !s2)?0:!strncasecmp(s1, s2, n)))

#define safe_strcoll(s1, s2)    (((!s1 || !*s1) && (!s2 || !*s2))?0:\
                                ((!s1 || !s2)?(s1 - s2):strcoll(s1, s2)))

#define safe_strcasecmp(s1, s2) (((!s1 || !*s1) && (!s2 || !*s2))?0:\
                                ((!s1 || !s2)?(s1 - s2):strcasecmp(s1, s2)))

    // JED - added to support case-insensitive comparisons inside the
    // C-locale. This is for internal keywords which are not subject to
    // translation
#define safe_fast_strcasecmp(s1, s2)    (((!s1 || !*s1) && (!s2 || !*s2))?0:\
                                ((!s1 || !s2)?(s1 - s2):ide_fast_stricmp(s1, s2)))

#define safe_fast_strcaseequal(s1,s2)  \
                                (((!s1 || !*s1) && (!s2 || !*s2))?1:\
                                ((!s1 || !s2)?0:!ide_fast_stricmp(s1, s2)))

#if defined(IRIX)
#include <algobase.h>
#else   /* !defined(IRIX) */

#ifndef max
#define        max(a,b)                (((a) > (b)) ? (a) : (b))
#endif                         /* max */
#ifndef min
#define        min(a,b)                (((a) < (b)) ? (a) : (b))
#endif                         /* min */

#endif  /* !defined(IRIX) */


#endif                         /* __MISC_MACROS_H_DEFINED__ */
