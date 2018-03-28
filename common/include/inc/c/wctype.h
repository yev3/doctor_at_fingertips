/* wctype.h standard header */
/* Copyright 2003-2017 IAR Systems AB.  */
#ifndef _WCTYPE
#define _WCTYPE

#ifndef _SYSTEM_BUILD
  #pragma system_include
#endif

#include <ycheck.h>
#include <yvals.h>

/* Consistency check */
#if !_DLIB_WIDE_CHARACTERS
  #error "This library configuration does not support wide characters."
#endif

/* Support for portable C++ object model. */
#if __AEABI_PORTABILITY_INTERNAL_LEVEL
  #ifndef __AEABI_PORTABLE
    #define __AEABI_PORTABLE
  #endif
#endif

/* Module consistency. */
#pragma rtmodel="__dlib_full_locale_support", \
  _STRINGIFY(_DLIB_FULL_LOCALE_SUPPORT)


/* MACROS */
#define WEOF    ((_GLB wint_t)(-1))

/* TYPE DEFINITIONS */
#ifndef _WCTYPET
  #define _WCTYPET
  #if _DLIB_SUPPORT_FOR_AEABI && !defined(__AS_BOUNDS__)
    typedef void * wctype_t;
  #else
    typedef _Sizet wctype_t;
  #endif
#endif
#ifndef _WCTRANST
  #define _WCTRANST
  #if _DLIB_SUPPORT_FOR_AEABI && !defined(__AS_BOUNDS__)
    typedef void * wctrans_t;
  #else
    typedef _Sizet wctrans_t;
  #endif
#endif

#ifndef _WINTT
  #define _WINTT
  typedef _Wintt wint_t;
#endif

/* ctype DECLARATIONS */
_EXTERN_C
  __ATTRIBUTES wctrans_t  wctrans(const char *);
  __ATTRIBUTES wctype_t   wctype(const char *);

  __ATTRIBUTES int        iswalnum(wint_t);
  __ATTRIBUTES int        iswalpha(wint_t);
  #if !_DLIB_ONLY_C89
    __ATTRIBUTES int      iswblank(wint_t);
  #endif
  __ATTRIBUTES int        iswcntrl(wint_t);
  __ATTRIBUTES int        iswctype(wint_t, wctype_t);
  __ATTRIBUTES int        iswdigit(wint_t);
  __ATTRIBUTES int        iswgraph(wint_t);
  __ATTRIBUTES int        iswlower(wint_t);
  __ATTRIBUTES int        iswprint(wint_t);
  __ATTRIBUTES int        iswpunct(wint_t);
  __ATTRIBUTES int        iswspace(wint_t);
  __ATTRIBUTES int        iswupper(wint_t);
  __ATTRIBUTES int        iswxdigit(wint_t);
  __ATTRIBUTES wint_t     towlower(wint_t);
  __ATTRIBUTES wint_t     towupper(wint_t);
  __ATTRIBUTES wint_t     towctrans(wint_t, wctrans_t);
_EXTERN_C_END

#endif /* _WCTYPE */

#ifdef __IAR_TEST_SYSTEM_HEADER
  #pragma this_isnt_a_proper_pragma
  void __iar_test_system_header(void);
#endif

/*
 * Copyright (c) by P.J. Plauger. All rights reserved.
 * Consult your license regarding permissions and restrictions.
V6.50:0576 */
