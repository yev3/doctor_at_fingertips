/* stdlib.h standard header */
/* Copyright 2005-2017 IAR Systems AB. */

#ifndef _STDLIB
#define _STDLIB

#ifndef _SYSTEM_BUILD
  #pragma system_include
#endif

#include <ycheck.h>
#include <yvals.h>
#include <ysizet.h>

/* Module consistency. */
#pragma rtmodel="__dlib_full_locale_support", \
  _STRINGIFY(_DLIB_FULL_LOCALE_SUPPORT)

#if __AEABI_PORTABILITY_INTERNAL_LEVEL
  #ifndef __AEABI_PORTABLE
    #define __AEABI_PORTABLE
  #endif
  _EXTERN_C
    __ATTRIBUTES int    __aeabi_MB_CUR_MAX(void);
  _EXTERN_C_END
  #define MB_CUR_MAX    (_GLB __aeabi_MB_CUR_MAX())
#else
  _EXTERN_C
    __ATTRIBUTES size_t __iar_Mbcurmax(void);
  _EXTERN_C_END
  #define MB_CUR_MAX    (_GLB __iar_Mbcurmax())
#endif


/* MACROS */
#ifndef NULL
  #define NULL  _NULL
#endif

#define EXIT_FAILURE    1
#define EXIT_SUCCESS    0

#if __INT_SIZE__ == 4
  #define RAND_MAX      0x3fffffff
#else
  #define RAND_MAX      0x7fff
#endif

/* TYPE DEFINITIONS */
#ifndef _WCHART
  #define _WCHART
  typedef _Wchart wchar_t;
#endif

typedef struct
{ /* result of int divide */
  int quot;
  int rem;
} div_t;

typedef struct
{ /* result of long divide */
  long quot;
  long rem;
} ldiv_t;

#if !_DLIB_ONLY_C89
  typedef struct
  { /* result of long long divide */
    long long quot;
    long long rem;
  } lldiv_t;
#endif

#if __STDC_WANT_LIB_EXT1__ == 1
  #ifndef _ERRNO_T
    #define _ERRNO_T
    typedef int errno_t;
  #endif

  #ifndef _RSIZE_T
    #define _RSIZE_T
    typedef size_t rsize_t;
  #endif
#endif

/* DECLARATIONS */
_EXTERN_C /* low-level functions */
  __ATTRIBUTES int             atexit(void (*)(void));
  #if !_DLIB_ONLY_C89
    __ATTRIBUTES          int  at_quick_exit(void (*)(void)) _NOEXCEPT;
    __ATTRIBUTES_NORETURN void _Exit(int) _NOEXCEPT;
    __ATTRIBUTES_NORETURN void quick_exit(int) _NOEXCEPT;
  #endif
  __ATTRIBUTES_NORETURN void   exit(int);
  __DEPREC_ATTRS        char * getenv(const char *);
  __ATTRIBUTES          int    system(const char *);
_EXTERN_C_END

_EXTERN_C
  #if !_DLIB_ONLY_C89
               __ATTRIBUTES void *    aligned_alloc(size_t, size_t);
  #endif
          __ATTRIBUTES_NORETURN void  abort(void) _NOEXCEPT;
  __EFF_NE     __ATTRIBUTES int       abs(int);
               __ATTRIBUTES void *    calloc(size_t, size_t);
  __EFF_NE     __ATTRIBUTES div_t     div(int, int);
               __ATTRIBUTES void      free(void *);
  __EFF_NE     __ATTRIBUTES long      labs(long);
  __EFF_NE     __ATTRIBUTES ldiv_t    ldiv(long, long);
  #if !_DLIB_ONLY_C89
    __EFF_NE   __ATTRIBUTES long long llabs(long long);
    __EFF_NE   __ATTRIBUTES lldiv_t   lldiv(long long, long long);
  #endif
               __ATTRIBUTES void *    malloc(size_t);
  __EFF_NW1    __ATTRIBUTES int       mblen(const char *, size_t);
  #if _DLIB_WIDE_CHARACTERS != 0
    __EFF_NR1NW2 __DEPREC_ATTRS size_t mbstowcs(wchar_t *_Restrict, 
                                                const char *_Restrict, size_t);
    __EFF_NR1NW2 __ATTRIBUTES   int    mbtowc(wchar_t *_Restrict,
                                              const char *_Restrict, size_t);
  #endif
               __ATTRIBUTES int    rand(void);
               __ATTRIBUTES void   srand(unsigned int);
               __ATTRIBUTES void * realloc(void *, size_t);
  #if !_DLIB_ONLY_C89
               __ATTRIBUTES void * __iar_realloc_in_place(void *, size_t);
  #endif
  __EFF_NW1NR2 __ATTRIBUTES long          strtol(const char *_Restrict, 
                                                 char **_Restrict, int);
  __EFF_NW1NR2 __ATTRIBUTES unsigned long strtoul(const char *, char **, int);
  #if _DLIB_WIDE_CHARACTERS != 0
    __EFF_NR1NW2 __DEPREC_ATTRS size_t wcstombs(char *_Restrict, 
                                               const wchar_t *_Restrict,
                                               size_t);
    __EFF_NR1    __DEPREC_ATTRS int    wctomb(char *, wchar_t);
  #endif
  #if !_DLIB_ONLY_C89
    __EFF_NW1NR2 __ATTRIBUTES long long strtoll(const char *, char **, int);
    __EFF_NW1NR2 __ATTRIBUTES unsigned long long strtoull(const char *, 
                                                          char **, int);
  #endif

  #if __AEABI_PORTABILITY_INTERNAL_LEVEL == 0
    #if __MULTIPLE_HEAPS__
      #pragma language = save
      #pragma language = extended

      #define __HEAP_MEM_HELPER1__(M, I)                               \
      __ATTRIBUTES void M##_free(void M *);                            \
      __ATTRIBUTES void M * M##_malloc(M##_size_t);                    \
      __ATTRIBUTES void M * M##_calloc(M##_size_t, M##_size_t);        \
      __ATTRIBUTES void M * M##_realloc(void M *, M##_size_t);         \
      __ATTRIBUTES void M * M##_aligned_alloc(M##_size_t, M##_size_t); \
                                                                       \
      __ATTRIBUTES void M * M##_realloc_in_place(void M *, M##_size_t);
      __HEAP_MEMORY_LIST1__()
      #undef __HEAP_MEM_HELPER1__
      #pragma language = restore
    #endif /* __MULTIPLE_HEAPS__ */
  #endif /* __AEABI_PORTABILITY_INTERNAL_LEVEL == 0 */


  typedef int _Cmpfun(const void *, const void *);
  __EFF_NW1NW2 __DEPREC_ATTRS_CAN_THROW void * bsearch(const void *, 
                                                       const void *, size_t,
                                                       size_t, _Cmpfun *);
               __DEPREC_ATTRS_CAN_THROW void   qsort(void *, size_t, size_t, 
                                                     _Cmpfun *);
               __ATTRIBUTES_CAN_THROW void     __qsortbbl(void *, size_t,
                                                          size_t, _Cmpfun *);
  __EFF_NW1    __ATTRIBUTES double             atof(const char *);
  __EFF_NW1    __ATTRIBUTES int                atoi(const char *);
  __EFF_NW1    __ATTRIBUTES long               atol(const char *);
  #if !_DLIB_ONLY_C89
     __EFF_NW1 __ATTRIBUTES long long        atoll(const char *);
     __EFF_NW1NR2 __ATTRIBUTES float         strtof(const char *_Restrict, 
                                                    char **_Restrict);
     __EFF_NW1NR2 __ATTRIBUTES long double   strtold(const char *, char **);
  #endif
  __EFF_NW1NR2 __ATTRIBUTES double           strtod(const char *_Restrict, 
                                                    char **_Restrict);
  #if __AEABI_PORTABILITY_INTERNAL_LEVEL == 0
    __EFF_NE   __ATTRIBUTES int              __iar_DLib_library_version(void);
  #endif  
_EXTERN_C_END

#ifndef _NO_DEFINITIONS_IN_HEADER_FILES
  _EXTERN_C
  #pragma inline=no_body
  int abs(int i)
  {      /* compute absolute value of int argument */
    return i < 0 ? -i : i;
  }

  #pragma inline=no_body
  long labs(long i)
  { /* compute absolute value of long argument */
    return i < 0 ? -i : i;
  }

  #if !_DLIB_ONLY_C89
    #pragma inline=no_body
    long long llabs(long long i)
    { /* compute absolute value of long long argument */
      return i < 0 ? -i : i;
    }
  #endif
  _EXTERN_C_END
#endif /* _NO_DEFINITIONS_IN_HEADER_FILES */

#ifdef __cplusplus
  extern "C++" {
    __EFF_NE long      abs(long);
    __EFF_NE long long abs(long long);
    __EFF_NE ldiv_t    div(long, long);
    __EFF_NE lldiv_t   div(long long, long long);

    #pragma inline=forced
    inline long abs(long _X)     /* OVERLOADS */
    { /* compute abs */
      return labs(_X);
    }

    #pragma inline=forced
    inline ldiv_t div(long _X, long _Y)
    { /* compute quotient and remainder */
      return ldiv(_X, _Y);
    }

    #pragma inline=forced
    inline long long abs(long long _X)     /* OVERLOADS */
    { /* compute abs */
      return llabs(_X);
    }

    #pragma inline=forced
    inline lldiv_t div(long long _X, long long _Y)
    { /* compute quotient and remainder */
      return lldiv(_X, _Y);
    }
  } /* extern "C++" */
#endif /* __cplusplus */

#if __STDC_WANT_LIB_EXT1__ == 1
  _EXTERN_C
    typedef void (*constraint_handler_t)(const char *_Restrict,
                                         void *_Restrict, errno_t);
    typedef int _Cmpfun_s(const void *, const void *, void *);

    __ATTRIBUTES constraint_handler_t set_constraint_handler_s(
                                                         constraint_handler_t);
    __ATTRIBUTES void                 abort_handler_s(const char *_Restrict,
                                                      void *_Restrict,
                                                      errno_t);
    __ATTRIBUTES void                 ignore_handler_s(const char *_Restrict,
                                                       void *_Restrict,
                                                       errno_t);
    __ATTRIBUTES errno_t              getenv_s(size_t *_Restrict,
                                               char *_Restrict,
                                               rsize_t, const char *_Restrict);
    __EFF_NW1NW2 __ATTRIBUTES_CAN_THROW void *bsearch_s(const void *,
                                                        const void *,
                                                        rsize_t, rsize_t,
                                                        _Cmpfun_s *, void *);
    __ATTRIBUTES_CAN_THROW errno_t    qsort_s(void *, rsize_t, rsize_t,
                                              _Cmpfun_s *, void *);
    #if _DLIB_WIDE_CHARACTERS != 0
      __ATTRIBUTES errno_t            wctomb_s(int *_Restrict,
                                               char *_Restrict, rsize_t,
                                               wchar_t);
      __ATTRIBUTES errno_t            mbstowcs_s(size_t *_Restrict,
                                                 wchar_t *_Restrict, rsize_t,
                                                 const char *_Restrict,
                                                 rsize_t);
      __ATTRIBUTES errno_t            wcstombs_s(size_t *_Restrict,
                                                 char *_Restrict, rsize_t,
                                                 const wchar_t *_Restrict,
                                                 rsize_t);
    #endif
  _EXTERN_C_END
#endif /* __STDC_WANT_LIB_EXT1__ == 1 */

#if _DLIB_PRODUCT_STDLIB
  #include <DLib_Product_stdlib.h>
#endif  

#endif /* _STDLIB */

/*
 * Copyright (c) by P.J. Plauger. All rights reserved.
 * Consult your license regarding permissions and restrictions.
V6.50:0576 */
