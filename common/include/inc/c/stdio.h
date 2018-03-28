/* stdio.h standard header */
/* Copyright 2003-2017 IAR Systems AB.  */
#ifndef _STDIO
#define _STDIO

#ifndef _SYSTEM_BUILD
  #pragma system_include
#endif

#include <ycheck.h>
#include <yvals.h>
#include <ysizet.h>

/* Module consistency. */
#pragma rtmodel="__dlib_file_descriptor",_STRINGIFY(_DLIB_FILE_DESCRIPTOR)

/* macros */
#ifndef NULL
  #define NULL          _NULL
#endif

#if _DLIB_FILE_DESCRIPTOR
  typedef _Filet FILE;

  #ifndef _FD_TYPE
    #define _FD_TYPE      signed char
  #endif
#endif

#if __AEABI_PORTABILITY_INTERNAL_LEVEL
  #if !defined(__AEABI_PORTABLE)
    #define __AEABI_PORTABLE
  #endif

  #if _DLIB_FILE_DESCRIPTOR
    _EXTERN_C
      _DLIB_DATA_ATTR extern FILE *__aeabi_stdin; 
      _DLIB_DATA_ATTR extern FILE *__aeabi_stdout;
      _DLIB_DATA_ATTR extern FILE *__aeabi_stderr;
    _EXTERN_C_END
    #define stdin     (_GLB __aeabi_stdin)
    #define stdout    (_GLB __aeabi_stdout)
    #define stderr    (_GLB __aeabi_stderr)

    _EXTERN_C
      _DLIB_CONST_ATTR extern int const __aeabi_IOFBF;
      _DLIB_CONST_ATTR extern int const __aeabi_IOLBF;
      _DLIB_CONST_ATTR extern int const __aeabi_IONBF;
      _DLIB_CONST_ATTR extern int const __aeabi_BUFSIZ;
      _DLIB_CONST_ATTR extern int const __aeabi_FOPEN_MAX;
      _DLIB_CONST_ATTR extern int const __aeabi_TMP_MAX;
      _DLIB_CONST_ATTR extern int const __aeabi_FILENAME_MAX;
      _DLIB_CONST_ATTR extern int const __aeabi_L_tmpnam;
    _EXTERN_C_END
    #define _IOFBF          (_GLB __aeabi_IOFBF)
    #define _IOLBF          (_GLB __aeabi_IOLBF)
    #define _IONBF          (_GLB __aeabi_IONBF)
    #define BUFSIZ          (_GLB __aeabi_BUFSIZ)
    #define FOPEN_MAX       (_GLB __aeabi_FOPEN_MAX)
    #define TMP_MAX         (_GLB __aeabi_TMP_MAX)
    #define FILENAME_MAX    (_GLB __aeabi_FILENAME_MAX)
    #define L_tmpnam        (_GLB __aeabi_L_tmpnam)
  #endif /* _DLIB_FILE_DESCRIPTOR */
#else /* !__AEABI_PORTABILITY_INTERNAL_LEVEL */
  #if _DLIB_FILE_DESCRIPTOR
    #if defined(_NO_DEFINITIONS_IN_HEADER_FILES)
      _EXTERN_C
        _DLIB_DATA_ATTR extern FILE *__aeabi_stdin; 
        _DLIB_DATA_ATTR extern FILE *__aeabi_stdout;
        _DLIB_DATA_ATTR extern FILE *__aeabi_stderr;
      _EXTERN_C_END
      #define stdin          (_GLB __aeabi_stdin)
      #define stdout         (_GLB __aeabi_stdout)
      #define stderr         (_GLB __aeabi_stderr)
    #else
      _EXTERN_C
        _DLIB_DATA_ATTR extern FILE __iar_Stdin;
        _DLIB_DATA_ATTR extern FILE __iar_Stdout;
        _DLIB_DATA_ATTR extern FILE __iar_Stderr;
      _EXTERN_C_END
      #define stdin           (&_GLB __iar_Stdin)
      #define stdout          (&_GLB __iar_Stdout)
      #define stderr          (&_GLB __iar_Stderr)
    #endif

    #define _IOFBF          0
    #define _IOLBF          1
    #define _IONBF          2
    #define BUFSIZ          512
    #ifndef FOPEN_MAX
      #define FOPEN_MAX     8
    #endif
    #define TMP_MAX         256
    #define FILENAME_MAX    260
    #define L_tmpnam        16
  #endif /* _DLIB_FILE_DESCRIPTOR */
#endif /* __AEABI_PORTABILITY_INTERNAL_LEVEL */

#define EOF             (-1)

#define SEEK_SET        0
#define SEEK_CUR        1
#define SEEK_END        2

/* type definitions */
typedef _Fpost fpos_t;

#if __STDC_WANT_LIB_EXT1__ == 1
  #if !defined(_ERRNO_T)
    #define _ERRNO_T
    typedef int errno_t;
  #endif

  #if !defined(_RSIZE_T)
    #define _RSIZE_T
    typedef size_t rsize_t;
  #endif
#endif

/* printf and scanf pragma support */
#ifdef _HAS_PRAGMA_PRINTF_ARGS
  #define __PRINTFPR _Pragma("__printf_args")                                 \
                     _Pragma("library_default_requirements _Printf = unknown")\
                     __ATTRIBUTES
  #define __SCANFPR  _Pragma("__scanf_args")                                  \
                     _Pragma("library_default_requirements _Scanf = unknown") \
                     __ATTRIBUTES
  #define __SCANFSPR /*_Pragma("__scanf_args") */                             \
                     _Pragma("library_default_requirements _Scanf = unknown") \
                     __ATTRIBUTES
#else
  #define __PRINTFPR __ATTRIBUTES
  #define __SCANFPR  __ATTRIBUTES
  #define __SCANFSPR __ATTRIBUTES
#endif

#define __DEPREC_PRINTF __DEPREC __PRINTFPR
#define __DEPREC_SCANF  __DEPREC __SCANFPR

#if _DLIB_FILE_DESCRIPTOR
  /* declarations */
  _EXTERN_C
    __ATTRIBUTES    void   clearerr(FILE *);
    __ATTRIBUTES    int    fclose(FILE *);
    __ATTRIBUTES    int    feof(FILE *);
    __ATTRIBUTES    int    ferror(FILE *);
    __ATTRIBUTES    int    fflush(FILE *);
    __ATTRIBUTES    int    fgetc(FILE *);
    __ATTRIBUTES    int    fgetpos(FILE *_Restrict, fpos_t *_Restrict);
    __ATTRIBUTES    char * fgets(char *_Restrict, int, FILE *_Restrict);
    __DEPREC_ATTRS  FILE * fopen(const char *_Restrict, const char *_Restrict);
    __DEPREC_PRINTF int    fprintf(FILE *_Restrict,
                                   const char *_Restrict, ...);
    __ATTRIBUTES    int    fputc(int, FILE *);
    __ATTRIBUTES    int    fputs(const char *_Restrict, FILE *_Restrict);
    __ATTRIBUTES    size_t fread(void *_Restrict, size_t, size_t,
                                 FILE *_Restrict);
    __DEPREC_ATTRS  FILE * freopen(const char *_Restrict,
                                   const char *_Restrict, FILE *_Restrict);
    __DEPREC_SCANF  int    fscanf(FILE *_Restrict, const char *_Restrict, ...);
    __ATTRIBUTES    int    fseek(FILE *, long, int);
    __ATTRIBUTES    int    fsetpos(FILE *, const fpos_t *);
    __ATTRIBUTES    long   ftell(FILE *);
    __ATTRIBUTES    size_t fwrite(const void *_Restrict, size_t, size_t, 
                                  FILE *_Restrict);
    __ATTRIBUTES    void   rewind(FILE *);
    __ATTRIBUTES    void   setbuf(FILE *_Restrict, char *_Restrict);
    __ATTRIBUTES    int    setvbuf(FILE *_Restrict, char *_Restrict,
                                   int, size_t);
    __DEPREC_ATTRS  char * tmpnam(char *);
    __DEPREC_ATTRS  FILE * tmpfile(void);
    __ATTRIBUTES    int    ungetc(int, FILE *);
    __DEPREC_PRINTF int    vfprintf(FILE *_Restrict, const char *_Restrict,
                                    __Va_list);
    #if !_DLIB_ONLY_C89
      __DEPREC_SCANF int   vfscanf(FILE *_Restrict, const char *_Restrict,
                                   __Va_list);
    #endif
    #if _DLIB_ADD_EXTRA_SYMBOLS
      __ATTRIBUTES FILE *   fdopen(_FD_TYPE, const char *);
      __ATTRIBUTES _FD_TYPE fileno(FILE *);
      __ATTRIBUTES int      getw(FILE *);
      __ATTRIBUTES int      putw(int, FILE *);
    #endif
    __ATTRIBUTES int        getc(FILE *);
    __ATTRIBUTES int        putc(int, FILE *);
  _EXTERN_C_END
#endif /* _DLIB_FILE_DESCRIPTOR */

_EXTERN_C
  #ifndef __cplusplus
    /* Corresponds to fgets(char *, int, stdin); */
    __EFF_NR1 __ATTRIBUTES   char * __gets(char *, int);
    __EFF_NR1 __DEPREC_ATTRS char * gets(char *);
  #endif
  __EFF_NW1    __ATTRIBUTES    void   perror(const char *);
  __EFF_NW1    __DEPREC_PRINTF int    printf(const char *_Restrict, ...);
  __EFF_NW1    __ATTRIBUTES    int    puts(const char *);
  __EFF_NW1    __DEPREC_SCANF  int    scanf(const char *_Restrict, ...);
  __EFF_NR1NW2 __DEPREC_PRINTF int    sprintf(char *_Restrict, 
                                              const char *_Restrict, ...);
  __EFF_NW1NW2 __DEPREC_SCANF  int    sscanf(const char *_Restrict, 
                                             const char *_Restrict, ...);
                                      /* Corresponds to "ungetc(c, stdout)" */
  __ATTRIBUTES                 int    __ungetchar(int);
  __EFF_NW1    __DEPREC_PRINTF int    vprintf(const char *_Restrict,
                                              __Va_list);
  #if !_DLIB_ONLY_C89
    __EFF_NW1    __DEPREC_SCANF int vscanf(const char *_Restrict, __Va_list);
    __EFF_NW1NW2 __DEPREC_SCANF int vsscanf(const char *_Restrict, 
                                            const char *_Restrict, __Va_list);
  #endif
  __EFF_NR1NW2  __DEPREC_PRINTF int vsprintf(char *_Restrict, 
                                             const char *_Restrict, __Va_list);
                                /* Corresponds to fwrite(p, x, y, stdout); */
  __EFF_NW1 __ATTRIBUTES size_t   __write_array(const void *, size_t, size_t);
  #if !_DLIB_ONLY_C89
    __EFF_NR1NW3 __DEPREC_PRINTF int snprintf(char *_Restrict, size_t, 
                                              const char *_Restrict, ...);
    __EFF_NR1NW3 __DEPREC_PRINTF int vsnprintf(char *_Restrict, size_t,
                                               const char *_Restrict,
                                               __Va_list);
  #endif

  __ATTRIBUTES int                getchar(void);
  __ATTRIBUTES int                putchar(int);
  /* File system functions that have debug variants. They are agnostic on 
     whether the library is full or normal. */
  __ATTRIBUTES int                remove(const char *);
  __ATTRIBUTES int                rename(const char *, const char *);
_EXTERN_C_END

#if __STDC_WANT_LIB_EXT1__ == 1
  #if _DLIB_FILE_DESCRIPTOR
    #define L_tmpnam_s L_tmpnam
    #define TMP_MAX_S  TMP_MAX

    #if __AEABI_PORTABILITY_INTERNAL_LEVEL != 1 && TMP_MAX_S < 25
      #error "TMP_MAX_S too small"
    #endif
  #endif

  _EXTERN_C
    __EFF_NR1 __ATTRIBUTES char * gets_s(char *, rsize_t);
    __PRINTFPR int                printf_s(const char *_Restrict, ...);
    __SCANFSPR int                scanf_s(const char *_Restrict, ...);
    __PRINTFPR int                snprintf_s(char *_Restrict, rsize_t,
                                             const char *_Restrict, ...);
    __PRINTFPR int                sprintf_s(char *_Restrict, rsize_t,
                                            const char *_Restrict, ...);
    __SCANFSPR int                sscanf_s(const char *_Restrict,
                                           const char *_Restrict, ...);
    __PRINTFPR int                vprintf_s(const char *_Restrict, __Va_list);
    __SCANFSPR int                vscanf_s(const char *_Restrict, __Va_list);
    __PRINTFPR int                vsnprintf_s(char *_Restrict, rsize_t,
                                              const char *_Restrict, __Va_list);
    __PRINTFPR int                vsprintf_s(char *_Restrict, rsize_t,
                                             const char *_Restrict, __Va_list);
    __SCANFSPR int                vsscanf_s(const char *_Restrict,
                                            const char *_Restrict, __Va_list);

    #if _DLIB_FILE_DESCRIPTOR
      __ATTRIBUTES errno_t        fopen_s(FILE *_Restrict *_Restrict,
                                          const char *_Restrict,
                                          const char *_Restrict);
      __ATTRIBUTES errno_t        freopen_s(FILE *_Restrict *_Restrict,
                                            const char *_Restrict,
                                            const char *_Restrict,
                                            FILE *_Restrict);
      __PRINTFPR int              fprintf_s(FILE *_Restrict, 
                                            const char *_Restrict, ...);
      __SCANFSPR int              fscanf_s(FILE *_Restrict,
                                           const char *_Restrict, ...);
      __ATTRIBUTES errno_t        tmpfile_s(FILE *_Restrict *_Restrict);
      __ATTRIBUTES errno_t        tmpnam_s(char *, rsize_t);
      __PRINTFPR __ATTRIBUTES int vfprintf_s(FILE *_Restrict, 
                                             const char *_Restrict, __Va_list);
      __SCANFSPR __ATTRIBUTES int vfscanf_s(FILE *_Restrict,
                                            const char *_Restrict, __Va_list);
    #endif /*  _DLIB_FILE_DESCRIPTOR */
  _EXTERN_C_END
#endif /* __STDC_WANT_LIB_EXT1__ == 1 */

#endif /* _STDIO */

/*
 * Copyright (c) by P.J. Plauger. All rights reserved.
 * Consult your license regarding permissions and restrictions.
V6.50:0576 */
