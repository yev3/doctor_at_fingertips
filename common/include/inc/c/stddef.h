/* stddef.h standard header */
/* Copyright 2009-2017 IAR Systems AB. */
#ifndef _STDDEF
#define _STDDEF

#ifndef _SYSTEM_BUILD
  #pragma system_include
#endif

#include <ycheck.h>
#include <yvals.h>
#include <ysizet.h>

/* macros */
#ifndef NULL
  #define NULL   _NULL
#endif

#ifndef offsetof
  #define offsetof(T, member)     (__INTADDR__((&((T *)0)->member)))
#endif

/* type definitions */
#ifndef _PTRDIFF_T
  #define   _PTRDIFF_T
  #define   _STD_USING_PTRDIFF_T
  typedef   __PTRDIFF_T_TYPE__ ptrdiff_t;
#endif

#ifndef _WCHART
  #define   _WCHART
  typedef   _Wchart wchar_t;
#endif

#ifdef __cplusplus
  typedef decltype(nullptr) nullptr_t;
#endif

#if (!defined(__cplusplus) && !_DLIB_ONLY_C89) || (defined(__cplusplus))
  #if !defined(_MAX_ALIGN_T)
    #define _MAX_ALIGN_T
    typedef union
    {
      long long _ll;
      long double _ld;
      void *_vp;
    } _Max_align_t;
    typedef _Max_align_t max_align_t;
  #endif
#endif

#if __STDC_WANT_LIB_EXT1__ == 1
  #if !defined(_RSIZE_T)
    #define _RSIZE_T
    typedef size_t rsize_t;
  #endif
#endif

#endif /* _STDDEF */

/*
 * Copyright (c) by P.J. Plauger. All rights reserved.
 * Consult your license regarding permissions and restrictions.
V6.50:0576 */
