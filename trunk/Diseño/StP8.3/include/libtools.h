/************************************************************************
 *
 * File:  libtools.h
 *
 ************************************************************************/

#ifndef LIBTOOLS_H_INCLUDED
#define LIBTOOLS_H_INCLUDED

#include "IdePlatform.h"

#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>

#if defined(WIN32)

#include <io.h>
#include <direct.h>
#include <process.h>
#include <sys/stat.h>

#else /* !defined(WIN32) */

#include <unistd.h>
#include <sys/time.h>

#if defined(IRIX)
#include <bstring.h>
#include <sys/param.h>
#endif // defined(IRIX)

#if defined(AIX)
#include <strings.h>
#include <sys/select.h>
#endif // defined(AIX)

#endif //!defined(WIN32)

#if !defined(ASSERT)
#include <assert.h>
#define ASSERT(exp) assert(exp)
#endif //!defined(ASSERT)

#include "IdeToolsApi.h"
#include "IdeString.h"
#include "IdeApp.h"
#include "gu_set.h"

#include "conf.h"
#include "misc_types.h"
#include "misc_constants.h"
#include "ide_env.h"

#define NITEMS(array)   (sizeof (array) / sizeof (*(array)))
#define IDETRACE()      (fprintf(stderr, "%s: %d\n", __FILE__, __LINE__), fflush(stderr))

inline void     sfree(void* p) { free(p); }
inline void     safe_free(void* p) { free(p); }

/*
 * date_format.C
 */
IDE_TOOLS_API char    *FmtDate(CTSTR fmt, int date);
IDE_TOOLS_API char    *FmtTime(CTSTR fmt, int date);
IDE_TOOLS_API int      FmtItem(CTSTR* fmt, char *fmtstr, int *len, int *prec);
IDE_TOOLS_API char    *DayName(int day);
IDE_TOOLS_API char    *MonName(int mon);

/*
 * ide_calc_crc.C
 */
IDE_TOOLS_API unsigned long ideCalculateCRC32(void *in, long inLength);

/*
 * dates.C
 */
#ifdef TIMEINSYS
#include <sys/time.h>
#else
#include <time.h>
#endif

#define VARREP int
IDE_TOOLS_API int       chkdate(char *str);
IDE_TOOLS_API int       datetoint(char *str);
IDE_TOOLS_API char      *inttodate(int ival);
IDE_TOOLS_API int       chktime(char *str);
IDE_TOOLS_API int       timetoint(char *str);
IDE_TOOLS_API char      *inttotime(int ival);
IDE_TOOLS_API int       datenow(void);
IDE_TOOLS_API int       Weekday(int date);
IDE_TOOLS_API int       timenow(void);
IDE_TOOLS_API int       partime(const char *astr, struct tm * atm, int *zone);
IDE_TOOLS_API time_t    str_to_timet(const char *str);
IDE_TOOLS_API time_t    str_to_gmtimet(const char *str);

/*
 * defaults.C
 */
IDE_TOOLS_API int       do_set_names_def(int val);
IDE_TOOLS_API int       set_max_name_len(int len);
IDE_TOOLS_API void      get_sys_name_def();
IDE_TOOLS_API void      get_file_limits(char *inval, int *sn, int *flen, char *dotchar);
IDE_TOOLS_API int       set_file_name_defs(int s_in_dfe_n, int fnl, int dot_ch, int need_import);
IDE_TOOLS_API int       set_dot_char_in_file(int dot_c);
IDE_TOOLS_API void      name_set_defaults(void);

/*
 * fast_stricmp.C
 */
IDE_TOOLS_API int     ide_fast_stricmp(const char *lhs, const char *rhs);

/*
 * ide_argv.C
 */
IDE_TOOLS_API void     arg_shift_argv(int* argc, char** argv);
IDE_TOOLS_API void     argv_to_glob(int argc, char** argv, int *glac, char ***glav);
IDE_TOOLS_API int      get_editor_from_argv(int* argc, char** argv, char **pname, int change_argv);
IDE_TOOLS_API int      argv_check_for_arg_present(char** argv, char *aname, int num_opts);

/*
 * ide_path.C
 */
IDE_TOOLS_API IdeTString ide_cat_user_path(const IdeTString& path, CTSTR name, bool append = true);
IDE_TOOLS_API int       ide_path_get_next_comp(TSTR comp, CTSTR path, CTSTR* next_comp);
IDE_TOOLS_API int       ide_path_cat_all_dir(TSTR* full_path, CTSTR comp_path, CTSTR rel_name);
IDE_TOOLS_API TSTR      ide_fullname_in_path(CTSTR name);

/*
 * itoa.C
 */
IDE_TOOLS_API void     TI_itoa(int n, char *s, int radix);

/*
 * label_name.C
 */
IDE_TOOLS_API char     get_name_sep_char(void);
IDE_TOOLS_API int      dd_compress_blanks(char buf[]);
IDE_TOOLS_API int      dd_delete_blanks(char buf[]);
IDE_TOOLS_API int      dd_make_ident(char buf[]);
IDE_TOOLS_API char     *strip_blanks(char *str);
IDE_TOOLS_API char     *EStrLead(char* s, char* cs);
IDE_TOOLS_API char     *EStrTrim(char *s, char *cs);

IDE_TOOLS_API int
str_to_str_array(char *str,
                 char *str_array[],
                 int lim,
                 int line_l,
                 int need_copy);

/*
 * mkcpy.C
 */
IDE_TOOLS_API char     *mkcpy(const char *from);
IDE_TOOLS_API char     *concat(const char *s1, const char *s2);
IDE_TOOLS_API char     *concat(const char *s1, const char *s2, const char *s3);
IDE_TOOLS_API char     *concat(const char *s1, const char *s2, const char *s3,
                                const char *s4);
IDE_TOOLS_API char     *concat(const char *s1, const char *s2, const char *s3,
                                const char *s4, const char *s5);
IDE_TOOLS_API char     *concat(const char *s1, const char *s2, const char *s3,
                                const char *s4, const char *s5, const char *s6);
IDE_TOOLS_API char     *concat(const char *s1, const char *s2, const char *s3,
                                const char *s4, const char *s5, const char *s6,
                                const char *s7);
IDE_TOOLS_API char     *concat(const char *s1, const char *s2, const char *s3,
                                const char *s4, const char *s5, const char *s6,
                                const char *s7, const char *s8);
IDE_TOOLS_API char     *concat(const char *s1, const char *s2, const char *s3,
                                const char *s4, const char *s5, const char *s6,
                                const char *s7, const char *s8, const char *s9);
IDE_TOOLS_API char     *text_substr(CTSTR text, int which, int field_sep, int esc_char);
IDE_TOOLS_API void     string_replace_char(char *str, int old_char, int new_char);
IDE_TOOLS_API char     last_nonwhite_char(char *str);
IDE_TOOLS_API char     first_nonwhite_char(char *str);

/*
 * msg_from_f.C
 */
IDE_TOOLS_API char     *TextLook(int txt_no);
IDE_TOOLS_API char     *CTextLook(int txt_no);
IDE_TOOLS_API void     Textperror(int tno);
IDE_TOOLS_API void     TextStats(void);
IDE_TOOLS_API int      TI_TextPutNo(char *text, int num);
IDE_TOOLS_API int      TI_TextPut(char *text);

/*
 * nl_escape.C
 */
IDE_TOOLS_API char     *nl_escape(char *cp);
IDE_TOOLS_API char     *filter(char *str);

/*
 * osemul.C
 */

#ifdef BSD42
/* for gethostid() */
#include <sysent.h>
#endif                         /* BSD42 */

#ifdef VFORK_EMUL
#ifdef __cplusplus
extern "C"
{
#endif
    IDE_TOOLS_API pid_t  vfork(void);
#ifdef __cplusplus
}                               /*  "C" {} */
#endif                         /* __cplusplus */
#endif                         /* VFORK_EMUL */

#ifdef GETHOSTID_EMUL
#ifdef __cplusplus
extern "C"
{
#endif                         /* __cplusplus */
    IDE_TOOLS_API int      gethostid(void);
#ifdef __cplusplus
}                               /*  "C" {} */
#endif                         /* __cplusplus */
#endif                         /* GETHOSTID_EMUL */

#if defined(WIN32)
inline int strcasecmp(const char *a, const char *b) { return _stricmp(a, b); }
inline int strncasecmp(const char *a, const char *b, int n) { return _strnicmp(a, b, n); }
#else /* !defined(WIN32) */
inline int stricmp(const char *a, const char *b) { return strcasecmp(a, b); }
inline int strnicmp(const char *a, const char *b, int n) { return strncasecmp(a, b, n); }
#ifdef STRCASECMP_EMUL
IDE_TOOLS_API int      strcasecmp(const char *a, const char *b);
IDE_TOOLS_API int      strncasecmp(const char *a, const char *b, int n);
#endif
IDE_TOOLS_API char     *strlwr(char *s);
IDE_TOOLS_API char     *strupr(char *s);
#endif /* !defined(WIN32) */

/* The same as struct timeval from the BSD file sys/time.h */
typedef struct
{
    long    tv_sec;     /* seconds */
    long    tv_usec;    /* and microseconds */
} IdeTimeval;

IDE_TOOLS_API char     *ide_getwd(char *path);
IDE_TOOLS_API char     *TI_username(void);
IDE_TOOLS_API const char* TI_gethostname(void);
IDE_TOOLS_API int      TI_gettimeofday(IdeTimeval* t);
IDE_TOOLS_API long     TI_unique_id(void);
IDE_TOOLS_API int      TI_ascii_to_iso(char *str);
IDE_TOOLS_API int      TI_iso_to_ascii(unsigned char *str, unsigned char **res);
IDE_TOOLS_API int      strcmp_func(char *s1, char *s2);

#if defined(WIN32)

//
// We should include winsock.h here to get the definition
// of SOCKET and use that in the typedef. But that causes
// problems because it pulls in header files that use the
// word Status and Xlib.h has a "#define Status int". This
// combinaion winds up causing compilation errors in a lot
// of places where X header files are pulled in before this
// header file.
//
// typedef SOCKET IdeSocket;
// const IdeSocket IdeInvalidSocket = INVALID_SOCKET;
// const int IdeSocketError = SOCKET_ERROR;

typedef unsigned int IdeSocket;
const IdeSocket IdeInvalidSocket = ~0;
const int IdeSocketError = -1;

#include <sys/types.h>
typedef unsigned long* IdeIoctlData;
typedef unsigned long mode_t;

#ifndef TELEUSE
typedef unsigned int pid_t;
#endif

#else // !defined(WIN32)

typedef int IdeSocket;
const IdeSocket IdeInvalidSocket = -1;
const int IdeSocketError = -1;

typedef char* IdeIoctlData;

#endif // !defined(WIN32)

IDE_TOOLS_API int      TI_ioctl(int fd, int request, IdeIoctlData arg);
IDE_TOOLS_API int      TI_getLastSocketError(void);
IDE_TOOLS_API int      TI_closeSocket(IdeSocket s);

IDE_TOOLS_API int      TI_mkdir(const char *path, mode_t mode);
IDE_TOOLS_API char*    TI_getcwd(char* buf, int size);
IDE_TOOLS_API  int      TI_rmdir(const char *directory);

#if defined(WIN32)

#define S_IRWXU 00700
#define S_IRUSR 00400
#define S_IWUSR 00200
#define S_IXUSR 00100

#define S_IRWXG 00070
#define S_IRGRP 00040
#define S_IWGRP 00020
#define S_IXGRP 00010

#define S_IRWXO 00007
#define S_IROTH 00004
#define S_IWOTH 00002
#define S_IXOTH 00001

#define S_ISREG(m)      (((m) & S_IFMT) == S_IFREG)

#define L_cuserid   36

IDE_TOOLS_API int getdomainname(char *name, int namelen);
IDE_TOOLS_API char* cuserid(char* s);
IDE_TOOLS_API char* getlogin(void);
IDE_TOOLS_API unsigned int sleep(unsigned int seconds);
inline int getpid(void) { return _getpid(); }
inline int umask(int pmode) { return _umask(pmode); }
inline int close(int handle) { return _close(handle); }
inline int dup(int handle) { return _dup(handle); }
inline int write(int handle, const void *buf, unsigned int count) { return _write(handle, buf, count); }
inline FILE* popen(const char* s1, const char* s2) { return _popen(s1, s2); }
inline int pclose(FILE *file) { return _pclose(file); }

#ifdef __cplusplus
extern "C"
{
#endif                         /* __cplusplus */

#undef isascii
IDE_TOOLS_API int isascii(int);

#ifdef __cplusplus
}                               /*  "C" {} */
#endif                         /* __cplusplus */

#endif

#define ACC_EX          0x00
#define ACC_EXEC        0x01
#define ACC_CD          0x01
#define ACC_WR          0x02
#define ACC_RD          0x04
#define ACC_RW          0x06

#if !defined(WIN32)
#define FILE_ACCESS(fn, md)     access(fn, md)
#define DIR_ACCESS(dir, md)     access(dir, md)
#else // defined(WIN32)
#define FILE_ACCESS(fn, md)     _access(fn, md)
#define DIR_ACCESS(dir, md)     _access(dir, md)
#endif // defined(WIN32)

/*
 * parse_cmd.C
 */
IDE_TOOLS_API int      str_to_argv(const char* str, char *argv[], int lim);
IDE_TOOLS_API int      shell_cmd(const char* cmd_buf, char *argv[], int lim);
IDE_TOOLS_API void     set_tmp_nm(char *cont);
IDE_TOOLS_API int      background_exec(char* cmd_buf);

/*
 * pattern.C
 */
IDE_TOOLS_API int      is_pattern(CTSTR name, int sh);
IDE_TOOLS_API void     sh_re_ex_to_pat(char *match,
                                        char rbuf[],
                                        int full_line,
                                        int len);
IDE_TOOLS_API char     *sh_re_ex(char *match);
IDE_TOOLS_API char     *eq_re_ex(char *match);

/*
 * qtput.C
 */
IDE_TOOLS_API void     de_escape_char(char *string, int ch);
IDE_TOOLS_API TSTR     qtput(CTSTR cp);
IDE_TOOLS_API TSTR     dbl_qtput(CTSTR cp);
IDE_TOOLS_API TSTR     cpqtput(CTSTR str);
IDE_TOOLS_API char     *write_indent(int indent);

/*
 * qtstrp.C
 */
IDE_TOOLS_API char    *qtstrp(char *string);
IDE_TOOLS_API char    *cpqtstrp(char *string);

/*
 * read_a_dir.C
 */

#include "IdeDirent.h"

struct __dir_fl__
{
    char           *d_path;
    char           *d_file;
};

IDE_TOOLS_API int      read_a_dir(char* pat, struct __dir_fl__** context, DIR** handle, char* a_file);
IDE_TOOLS_API int      show_next_file(struct __dir_fl__** context, DIR** handle, char* a_file);
IDE_TOOLS_API int      make_real_dir(char* indir, char* realdir);

/*
 * regex.C
 */
IDE_TOOLS_API void     my_reg_err(int c);
IDE_TOOLS_API int      get_my_err(int cl);

#if defined(WIN32) || defined(REGEX_EMUL)
IDE_TOOLS_API char     *re_comp(const char *ex);
IDE_TOOLS_API int      re_exec(const char *str);
#else // !defined(WIN32) && !defined(REGEX_EMUL)
#ifdef __cplusplus
#ifndef __lucid
extern "C"
{
    extern int      ide_re_exec(const char *);
    extern char    *re_comp(const char *);
}
#endif /* __lucid */
#endif /* __cplusplus */
#endif // !defined(WIN32) && !defined(REGEX_EMUL)

/*
 * set_msg_edit.C
 */
IDE_TOOLS_API int      set_msg_editor(char **name);

/*
 * strip_suffix.C
 */
IDE_TOOLS_API void strip_suffix(TSTR output, CTSTR input, CTSTR suffix);
IDE_TOOLS_API void add_suffix(TSTR with_suffix, TSTR no_suffix,
                               CTSTR input, CTSTR suffix);

#include <stdio.h>

// Platform dependent strings
#if !defined(WIN32)
#define RETURN_STRING        "Return"
#else /* defined(WIN32) */
#define RETURN_STRING        "Enter"
#endif /* defined(WIN32) */


/*
 * ti_cfile.C
 */
IDE_TOOLS_API int      TI_Name(char buf[]);
IDE_TOOLS_API void     _TI_reset_name(void);
IDE_TOOLS_API int      TI_Vr_done(void);
IDE_TOOLS_API int      TI_Rr_done(void);
IDE_TOOLS_API void     TI_getfpath(char name[], int len);
IDE_TOOLS_API FILE     *TI_pathopen_mode(const char path[],
                                          const char name[], char mode[]);
IDE_TOOLS_API FILE     *TI_pathopen(const char path[], const char name[]);
IDE_TOOLS_API FILE     *_TI_Cfile(void);
IDE_TOOLS_API int      TI_skip_blanks(char **bufp);

/*
 * ti_files.C
 */
IDE_TOOLS_API char*     TI_filename(char const *f_path);
IDE_TOOLS_API char*     TI_pathname(char const *f_path);
IDE_TOOLS_API CTSTR     TI_lastSlash(CTSTR path);   // don't use this function, use ideDirPartLength instead
inline TSTR             TI_lastSlash(TSTR path) { return (TSTR)TI_lastSlash((CTSTR)path); }
IDE_TOOLS_API char*     TI_tmpfile(char *fname, CTSTR base_name, CTSTR f_type);
IDE_TOOLS_API char*     TI_tmpstdfile(char *fname, CTSTR base_name, CTSTR f_type);
IDE_TOOLS_API char*     TI_gettmp(void);
IDE_TOOLS_API int       TI_makedir(char *dname);
IDE_TOOLS_API int       TI_makedir(char *dname, int lim);
IDE_TOOLS_API int       TI_catfile(char* fullname, const char* dir, const char* filen, int lim);
IDE_TOOLS_API int       TI_putenv(const char* name, const char* newval);
IDE_TOOLS_API char*     TI_cfilename(char *f_path);
IDE_TOOLS_API char*     TI_cpathname(char *f_path);
IDE_TOOLS_API char*     TI_parentdir(char const *dir);
IDE_TOOLS_API char*     TI_progname(const char* p_path);
IDE_TOOLS_API char*     TI_cprogname(const char* p_path);
IDE_TOOLS_API int       TI_lock_a_file(int fd);
IDE_TOOLS_API int       TI_unlock_a_file(int fd);
IDE_TOOLS_API bool      TI_isRealDirectory(char const *dir, bool *hasParent = NULL,
                            ino_t *dirid = NULL, dev_t *devid = NULL);
IDE_TOOLS_API void      prog_exit(int status);
IDE_TOOLS_API void      _prog_exit(int status);
IDE_TOOLS_API void      memory_panic(const char *funcname) throw (IdeMemoryException);

/*
 * ti_filesys.C
 */
IDE_TOOLS_API int       ideGetDbOpen(void);
IDE_TOOLS_API void      ideSetDbOpen(int state);
IDE_TOOLS_API bool      ideIsDbReadonly(void);
IDE_TOOLS_API void      ideSetDbReadonly(bool readonly);
IDE_TOOLS_API void      ide_fs_filestore_dir_asgn(const IdeTString& filestore);
IDE_TOOLS_API char*     ide_fs_filestore_path(void);
inline bool    edit_set_diag(CTSTR dgname)
                            { return ide_env_diagram_asgn(dgname); }
inline CTSTR            edit_get_diag(void)
                            { return ide_env_diagram(); }
inline bool    edit_set_proj_dir(CTSTR dname)
                            { return ide_env_projdir_asgn(dname); }
inline bool    edit_set_system_name(CTSTR sname)
                            { return ide_env_system_asgn(sname); }
IDE_TOOLS_API int       ide_fs_cat_dir(char *base_name, char *ends,
                                       const char *append_name);
IDE_TOOLS_API mode_t    ide_fs_system_umask(void);
IDE_TOOLS_API mode_t    ide_fs_file_umask(CTSTR fname);
IDE_TOOLS_API FILE*     ide_fs_system_umask_fopen(const char *path,
                                                   const char *mode);
IDE_TOOLS_API FILE*     ide_fs_open_sys_file(char *filename, char *mode);
IDE_TOOLS_API FILE*     ide_fs_open_filestore_file(char *filename, char *mode);
IDE_TOOLS_API char*     edit_get_sys_dir(void);
IDE_TOOLS_API CTSTR     ide_fs_filestore_dir_name(void);
IDE_TOOLS_API CTSTR     ide_illegal_diag_chars(void);
IDE_TOOLS_API CTSTR     ide_replace_diag_chars(void);
IDE_TOOLS_API void      ide_diag_strcpy(TSTR to, CTSTR from);
IDE_TOOLS_API int       edit_make_dir_name(char *dname);
IDE_TOOLS_API CTSTR     edit_make_name(void);
IDE_TOOLS_API int       edit_set_store_dir(void);
IDE_TOOLS_API void      ide_fs_make_absolute_dir(char *dir);
IDE_TOOLS_API int       edit_name_is_legal(int flags, int give_error);

/*
 * repinfo.C
 */
class IdeStringToStringMap;
IDE_TOOLS_API bool ide_repinfo_create_map(IdeStringToStringMap &map,
                                                   CTSTR projdir, CTSTR system);

IDE_TOOLS_API void     ide_repinfo_reload();
IDE_TOOLS_API CTSTR    ide_repinfo_find(CTSTR variable);

/*
 * ti_home.C
 */
IDE_TOOLS_API char*         _TI_hm(char* cont);
inline char const* _TI_hm(char const* name) { return (char const*)(_TI_hm((char*)name)); }
IDE_TOOLS_API const char*   ideGetUserHomeDir(void);
IDE_TOOLS_API void          TI_userdir(char *dname, int lim);
// Returns full path to the root directory of StP installation
IDE_TOOLS_API IdeTString    ideStpRootDir(void);
IDE_TOOLS_API const char*   TI_myRootDir(void); // obsolete, use ideStpRootDir instead

// Returns full path to the StP bin directory.
// Depending on configuration it can be bin/PLATFORM, bin/PLATFORM/Debug,
// or bin/PLATFORM/Release.
IDE_TOOLS_API IdeTString    ideStpBinDir(void);

/*
 * ti_vars.C
 */
// ToolInfo variable lookup
IDE_TOOLS_API CTSTR     TI_Vcont(CTSTR name);
// Combined Environment/ToolInfo variable lookup
IDE_TOOLS_API CTSTR     TI_EnvVcont(CTSTR name);
IDE_TOOLS_API CTSTR     TI_SameEnvVcont(CTSTR name);
IDE_TOOLS_API int       TI_Vinit(void);
IDE_TOOLS_API int       TI_Vasg(CTSTR name, CTSTR cont);
IDE_TOOLS_API int       TI_Var(CTSTR buf);
IDE_TOOLS_API bool      TI_V_set_overwrite(bool val);                   // used in util_vcc.C
IDE_TOOLS_API int       TI_V_user_file(CTSTR fname, bool overwrite);    // used in util_vcc.C
IDE_TOOLS_API bool      TI_yesorno(const char *string, bool default_value);
IDE_TOOLS_API bool      TI_yes(const char *str);
IDE_TOOLS_API bool      TI_no(const char *str);

#define MAX_TI_VAL_LEN  5120

/*
 * ti_wait.C
 */
IDE_TOOLS_API int      TI_wait_status(STATUS_TYPE status);
IDE_TOOLS_API int      TI_wait_termsig(STATUS_TYPE status);
IDE_TOOLS_API int      TI_wait_core(STATUS_TYPE status);
IDE_TOOLS_API int      TI_wait_WUNTRACED(void);
IDE_TOOLS_API int      TI_wait_WNOHANG(void);
IDE_TOOLS_API int      TI_wait_st_to_int(STATUS_TYPE status);
IDE_TOOLS_API int      TI_wait_int_to_st(int st_int, int *status_t);

#if defined(POSIX_PORT)
IDE_TOOLS_API int      TI_wait3(int *st, int op, int *ru);
#else /* defined(POSIX_PORT) */
IDE_TOOLS_API int      TI_wait3(STATUS3_TYPE * st, int op, struct rusage * ru);
#endif /* defined(POSIX_PORT) */


IDE_TOOLS_API int      TI_wait_WIFSTOPPED(STATUS_TYPE status);
IDE_TOOLS_API int      TI_wait_WIFSIGNALED(STATUS_TYPE status);
IDE_TOOLS_API int      TI_wait_WIFEXITED(STATUS_TYPE status);


/*
 *  copy_file.C
 */

typedef enum
{
    noConversion,
    unixToDos,
    dosToUnix,
    automatic
} filetypeConversion;
IDE_TOOLS_API int      copy_convert_file(const char *from, const char *to,
                                         filetypeConversion convert,
                                         bool preservePermission = false);
IDE_TOOLS_API int      copy_file(const char *from, const char *to,
                                 bool preservePermission = false);
IDE_TOOLS_API void     copy_file(FILE * source_fp, FILE * dest_fp);
IDE_TOOLS_API void     delete_file(FILE * fp, const IdeTString name);
IDE_TOOLS_API void     copy_and_delete_file(const char *source_name,
                                            FILE * source_fp,
                                            FILE * dest_fp,
                                            bool must_rewind);

/*
 *  IdeMSDev.C
 */
IDE_TOOLS_API int      ideNavigateToMSDev(const char *aFilename, int aLine, int aColumn);

#define        STARTUP_CMD_MSG         'm'
#define        STARTUP_CMD_HDR         'n'
#define        STARTUP_CMD_STATUS_TXT  's'
#define        STARTUP_CMD_LEFTF       'l' //API obsolete?
#define        STARTUP_CMD_RIGHTF      'r' //API obsolete?
#define        STARTUP_CMD_QUIT        'q'
#define        STARTUP_CMD_PROD        'p'
#define        STARTUP_CMD_GAUGE_INIT  'G'
#define        STARTUP_CMD_GAUGE_PROG  'g'
#define        STARTUP_CMD_PUSH        'v'
#define        STARTUP_CMD_POP         '^'

#define        STARTUP_PROG_NAME       "stp_startup"

/* func_tables.C */
#include "func_tables.h"

IDE_TOOLS_API oms_status_tp_func_tp status_func_lookup(const
                                                       status_func_entry_tp*
                                                       func_table, CTSTR name);
IDE_TOOLS_API void_ptr_func_tp void_ptr_func_lookup(void_ptr_func_entry_tp *
                                                    func_table, CTSTR func_name);
IDE_TOOLS_API int      void_ptr_func_table_merge(void_ptr_func_entry_tp ** tbl, void_ptr_func_entry_tp * nt, int *al);
IDE_TOOLS_API int      status_func_table_merge(status_func_entry_tp ** tbl, status_func_entry_tp * nt, int *al);
IDE_TOOLS_API int      status_label_func_table_merge(status_label_func_entry_tp ** tbl, status_label_func_entry_tp * nt, int *al);

#ifdef I18N
#include <kanji_code.h>
#endif

// print_msg.C

IDE_TOOLS_API int   tty_start(const char *title);
IDE_TOOLS_API void  tty_end(int channel);
IDE_TOOLS_API void  tty_msg(int channel, const char *format, ...);
IDE_TOOLS_API void  vtty_msg(int channel, const char *format, va_list va);

IDE_TOOLS_API void  print_msg(const char *format, ...);
IDE_TOOLS_API void  vprint_msg(const char *format, va_list va);
IDE_TOOLS_API void  print_err(const char *format, ...);
IDE_TOOLS_API void  vprint_err(const char *format, va_list va);
IDE_TOOLS_API void  fatal_err(const char *format, ...);          // does not return control
IDE_TOOLS_API void  vfatal_err(const char *format, va_list va);  // does not return control

IDE_TOOLS_API void  print_sym_msg(gu_set_tp syms, const char *format, ...);
IDE_TOOLS_API void  print_sym_err(gu_set_tp syms, const char *format, ...);
IDE_TOOLS_API void  print_single_sym_msg(symbol_id_tp sym, const char *format, ...);
IDE_TOOLS_API void  print_single_sym_err(symbol_id_tp sym, const char *format, ...);

// Before inserting a call to this function please look up the last
// used error number in editors.src/MaxInternalError, increment that
// number by one, update MaxInternalError and use the new number as an
// argument to ideInternalError().
IDE_TOOLS_API void  ideInternalError(unsigned int number);

#endif /* LIBTOOLS_H_INCLUDED */
