/* time64.h standard header for 64-bit implementation */
/* Copyright 2003-2017 IAR Systems AB. */
#ifndef _TIME64
#define _TIME64

#ifndef _SYSTEM_BUILD
  #pragma system_include
#endif

#include <ycheck.h>
#include <yvals.h>
#include <ysizet.h>

#if __AEABI_PORTABILITY_INTERNAL_LEVEL
  #ifndef __AEABI_PORTABLE
    #define __AEABI_PORTABLE
  #endif

  _EXTERN_C
    _DLIB_CONST_ATTR extern int const __aeabi_CLOCKS_PER_SEC;
  _EXTERN_C_END
  #define CLOCKS_PER_SEC  (_GLB __aeabi_CLOCKS_PER_SEC)
#else
  #define CLOCKS_PER_SEC  1
#endif

#if  _DLIB_SUPPORT_FOR_AEABI
  #if _DLIB_TIME_USES_LONG
    typedef signed long  clock_t;
  #else
    typedef unsigned int clock_t;
  #endif
#else
  typedef signed long    clock_t;
#endif

typedef signed long long __time64_t;

typedef __time64_t       time_t;

#define TIME_UTC 1

struct tm
{       /* date and time components */
  int tm_sec;
  int tm_min;
  int tm_hour;
  int tm_mday;
  int tm_mon;
  int tm_year;
  int tm_wday;
  int tm_yday;
  int tm_isdst;
  #if  _DLIB_SUPPORT_FOR_AEABI
    int __BSD_bug_filler1;
    int __BSD_bug_filler2;
  #endif
};

struct __timespec64
{
  __time64_t tv_sec;
  long tv_nsec;
};

struct timespec
{
  time_t tv_sec;
  long tv_nsec;
};

#ifndef NULL
  #define NULL   _NULL
#endif

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

_EXTERN_C
  /* low-level functions */
  __ATTRIBUTES time_t time(time_t *);

  /* declarations */
  __DEPREC_ATTRS char *          asctime(const struct tm *);
  __ATTRIBUTES   clock_t         clock(void);
  __DEPREC_ATTRS char *          ctime(const time_t *);
  __EFF_NE __ATTRIBUTES double   difftime(time_t, time_t);
  __DEPREC_ATTRS struct tm *     gmtime(const time_t *);
  __DEPREC_ATTRS struct tm *     localtime(const time_t *);
  __ATTRIBUTES   time_t          mktime(struct tm *);
  __ATTRIBUTES   int             timespec_get(struct timespec *, int);

  #if !__AEABI_PORTABILITY_INTERNAL_LEVEL
    __ATTRIBUTES __time64_t      __time64(__time64_t *);
    __ATTRIBUTES char *          __ctime64(const __time64_t *);
    __EFF_NE __ATTRIBUTES double __difftime64(__time64_t, __time64_t);
    __ATTRIBUTES struct tm *     __gmtime64(const __time64_t *);
    __ATTRIBUTES struct tm *     __localtime64(const __time64_t *);
    __ATTRIBUTES __time64_t      __mktime64(struct tm *);
    __ATTRIBUTES int             __timespec_get64(struct __timespec64 *, int);
  #endif
  __ATTRIBUTES size_t              strftime(char *_Restrict, size_t,
                                            const char *_Restrict,
                                            const struct tm *_Restrict);
_EXTERN_C_END

#if !defined(_NO_DEFINITIONS_IN_HEADER_FILES) && \
    !__AEABI_PORTABILITY_INTERNAL_LEVEL && \
    !_DLIB_TIME_USES_64_DEFAULT
  /* C inline definitions */

  #pragma inline=forced
  time_t time(time_t *p)
  {
    return __time64(p);
  }

  #pragma inline=forced
  char * ctime(const time_t *p)
  {
    return __ctime64(p);
  }

  #pragma inline=forced
  double difftime(time_t t1, time_t t2)
  {
    return __difftime64(t1, t2);
  }

  #pragma inline=forced
  struct tm * gmtime(const time_t *p)
  {
    return __gmtime64(p);
  }

  #pragma inline=forced
  struct tm * localtime(const time_t *p)
  {
    return __localtime64(p);
  }

  #pragma inline=forced
  time_t mktime(struct tm *p)
  {
    return __mktime64(p);
  }

  #pragma inline=forced
  int timespec_get(struct timespec *ts, int base)
  {
    return __timespec_get64((struct __timespec64 *)ts, base);
  }
#endif /* !defined(_NO_DEFINITIONS_IN_HEADER_FILES) && 
          !__AEABI_PORTABILITY_INTERNAL_LEVEL &&
          !_DLIB_TIME_USES_64_DEFAULT */

#if __STDC_WANT_LIB_EXT1__ == 1
  _EXTERN_C
    __ATTRIBUTES errno_t        asctime_s(char *, rsize_t, const struct tm *);
    __ATTRIBUTES errno_t        ctime_s(char *, rsize_t, const time_t *);
    __ATTRIBUTES struct tm *    gmtime_s(const time_t *_Restrict,
                                         struct tm *_Restrict);
    __ATTRIBUTES struct tm *    localtime_s(const time_t *_Restrict,
                                            struct tm *_Restrict);
    #if !__AEABI_PORTABILITY_INTERNAL_LEVEL
      __ATTRIBUTES errno_t      __ctime_s64(char *, rsize_t,
                                            const __time64_t *);
      __ATTRIBUTES struct tm *  __gmtime_s64(const __time64_t *_Restrict,
                                             struct tm *_Restrict);
      __ATTRIBUTES struct tm *  __localtime_s64(const __time64_t *_Restrict,
                                                struct tm *_Restrict);
    #endif
  _EXTERN_C_END

  #if !defined(_NO_DEFINITIONS_IN_HEADER_FILES) && \
      !__AEABI_PORTABILITY_INTERNAL_LEVEL && \
    !_DLIB_TIME_USES_64_DEFAULT
    #pragma inline=forced
    errno_t ctime_s(char *p, rsize_t rs, const time_t *tp)
    {
      return __ctime_s64(p, rs, tp);
    }

    #pragma inline=forced
    struct tm *gmtime_s(const time_t *_Restrict tp, struct tm *_Restrict tmp)
    {
      return __gmtime_s64(tp, tmp);
    }

    #pragma inline=forced
    struct tm *localtime_s(const time_t *_Restrict tp, struct tm *_Restrict tmp)
    {
      return __localtime_s64(tp, tmp);
    }
  #endif /* !defined(_NO_DEFINITIONS_IN_HEADER_FILES) && 
            !__AEABI_PORTABILITY_INTERNAL_LEVEL && \
            !_DLIB_TIME_USES_64_DEFAULT */
#endif /* __STDC_WANT_LIB_EXT1__ == 1 */

#endif /* _TIME64 */

/*
 * Copyright (c) by P.J. Plauger. All rights reserved.
 * Consult your license regarding permissions and restrictions.
V6.50:0576 */
