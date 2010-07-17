/************************************************************************
 *
 *
 * File:  misc_constants.h
 *
 *
 *
 ************************************************************************/

#ifndef __MISC_CONSTANTS_H_DEFINED__
#define __MISC_CONSTANTS_H_DEFINED__

#include <math.h>

#if !defined(M_PI)
#define M_PI            3.14159265358979323846
#endif

#define BEL             '\007'  /* bell flashes sun screen */
#define CTL_D           '\004'
#define CTL_G           '\007'  /* bell flashes sun screen */
#define CTL_H           '\010'
#define CTL_U           '\025'
#define CTL_W           '\027'
#define CTL_DEL '\177'

#ifndef ESC
#define ESC             '\033'
#endif

#define NEWLINE '\012'

#ifndef CR
#define CR      '\015'
#endif                          /* CR */
/*
 * some project database defines
 */

#define PDB             "pdb"
#define FILE_STORE      R_diag_names

/*
 * Different sizes, the following constraints need to be true to guarantee
 * the editors to work without problems: SYS_NAME_LEN >= 14 DIAG_LEN >= 20 &&
 * DIAG_LEN < LINE_SIZE-6 && DIAG_LEN >= LABEL_SIZE FILE_NAME_SIZE >
 * 2+DIAG_LEN+SYS_NAME_LEN
 */

#define SYS_NAME_LEN    40      /* max number of chars in a sys name */
#define DIAG_LEN        70      /* max number of chars in a diag names */
#define FILE_NAME_SIZE  140     /* max number of chars in a filename */

#define STACK_SIZE      20      /* maximum subconv stack size */
#define DD_NAME_LEN     75      /* Length inserted in DD per name */
#define MAX_ANCH        10      /* maximum number of rubber band echos */
#define BEFORE          (-32764)/* random constant */
#define AFTER           (-32763)/* random constant */


/* random constants */

#ifndef INT_MAX
#include <limits.h>
#endif

// EH this def is coming before values.h
#ifndef AIX41PWR
#ifndef MAXINT
#define MAXINT  ((int) ~(1<<(8*sizeof(int)-1)))
#endif
#endif // AIX41PWR


#define UNDEFINT        ((-INT_MAX)-1)
#define UNDEFULONG      0xFFFFFFFF


#ifndef TRUE
#define TRUE            1
#endif
#ifndef FALSE
#define FALSE           0
#endif

/* Special characters used in file-names */

#define DFE_PIND_DLM    '#'
#define SCE_SPEC_DLM    '='
#define UTIL_P_S_DLM    ':'
#define ALL_DBG_CHAR    '#'
#define EDIT_REV_CHAR   ','

/* get_pick flags */

#define P_RUB_BAND      0x0001  /* use rubber band echo */
#define P_AROW          0x0002  /* use arrow echo */
#define P_SADT_HORIZ    0x0004
#define P_SADT_VERT     0x0008

/* minimum number of pixels that a symbol can be scaled down to: */

#define MIN_SCALE_X 5
#define MIN_SCALE_Y 5

/* range of valid zoom levels */

#define MIN_ZOOM_LEVEL 0
#define MAX_ZOOM_LEVEL 2

/* offset to paste at */
#define PASTE_INCREMENT  10

/* macros */

#ifdef ABS
#undef ABS
#endif                          /* ABS */
#define ABS(n)                  ((n) < 0 ? -(n) : (n))
#define INTERRUPT_RETURN        longjmp(shortcut,1)
#define FLOOR(x)                ((int)((((x)>0.0)||((x)==(int)(x)))?(x):((x) - 1.0)))
#define ROUND(x)                (FLOOR((x) + 0.5))
#define AVG(x,y)                (((x) + (y)) / 2)

#if defined(IRIX)

#include <algobase.h>

#ifndef MIN
#define MIN(a, b) min((a), (b))
#endif

#ifndef MAX
#define MAX(a, b) max((a), (b))
#endif

#else   /* !defined(IRIX) */

#ifndef MIN
#define MIN(a,b) (((a)<(b))?(a):(b))
#endif

#ifndef MAX
#define MAX(a,b) (((a)>(b))?(a):(b))
#endif

#ifndef min
#define min(x,y)                MIN((x),(y))
#endif

#ifndef max
#define max(x,y)                MAX((x),(y))
#endif

#endif  /* !defined(IRIX) */

#if !defined(WIN32) && (!defined(MAXPATHLEN) || !defined(MAXHOSTNAMELEN))
#include <sys/param.h>
#endif // !defined(WIN32) && (!defined(MAXPATHLEN) || !defined(MAXHOSTNAMELEN))

#if !defined(MAXPATHLEN)
#if defined(WIN32)
#define MAXPATHLEN      260
#else // !defined(WIN32)
#define MAXPATHLEN      512
#endif // !defined(WIN32)
#endif // !defined(MAXPATHLEN)
#if !defined(MAXHOSTNAMELEN)
#define MAXHOSTNAMELEN  256
#endif // !defined(MAXHOSTNAMELEN)

#endif /* __MISC_CONSTANTS_H_DEFINED__ */
