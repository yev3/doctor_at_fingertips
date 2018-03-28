/* locale.h standard header */
/* Copyright 2003-2017 IAR Systems AB. */
#ifndef _LOCALE
#define _LOCALE

#ifndef _SYSTEM_BUILD
  #pragma system_include
#endif

#include <ycheck.h>
#include <yvals.h>
#include <xctype.h>

/* Module consistency. */
#pragma rtmodel="__dlib_full_locale_support", \
  _STRINGIFY(_DLIB_FULL_LOCALE_SUPPORT)

/* MACROS */
#ifndef NULL
  #define NULL   _NULL
#endif

/* LOCALE CATEGORY MASKS */
#define _CATMASK(n)     (1 << (n))

#define _M_COLLATE      _CATMASK(_LC_COLLATE)
#define _M_CTYPE        _CATMASK(_LC_CTYPE)
#define _M_MONETARY     _CATMASK(_LC_MONETARY)
#define _M_NUMERIC      _CATMASK(_LC_NUMERIC)
#define _M_TIME         _CATMASK(_LC_TIME)
#define _M_MESSAGE      _CATMASK(_LC_MESSAGE)
#define _M_MESSAGES     _M_MESSAGE
#define _M_ALL          (_CATMASK(_NCAT) - 1)

#if __AEABI_PORTABILITY_INTERNAL_LEVEL
  #ifndef __AEABI_PORTABLE
    #define __AEABI_PORTABLE
  #endif

  _EXTERN_C
    _DLIB_CONST_ATTR extern int const __aeabi_LC_COLLATE;
    _DLIB_CONST_ATTR extern int const __aeabi_LC_CTYPE;
    _DLIB_CONST_ATTR extern int const __aeabi_LC_MONETARY;
    _DLIB_CONST_ATTR extern int const __aeabi_LC_NUMERIC;
    _DLIB_CONST_ATTR extern int const __aeabi_LC_TIME;
    _DLIB_CONST_ATTR extern int const __aeabi_LC_MESSAGE;
    _DLIB_CONST_ATTR extern int const __aeabi_LC_MESSAGES;
    _DLIB_CONST_ATTR extern int const __aeabi_LC_ALL;
  _EXTERN_C_END
  #define LC_COLLATE    (_GLB __aeabi_LC_COLLATE)
  #define LC_CTYPE      (_GLB __aeabi_LC_CTYPE)
  #define LC_MONETARY   (_GLB __aeabi_LC_MONETARY)
  #define LC_NUMERIC    (_GLB __aeabi_LC_NUMERIC)
  #define LC_TIME       (_GLB __aeabi_LC_TIME)
  #define LC_MESSAGE    (_GLB __aeabi_LC_MESSAGE)
  #define LC_MESSAGES   LC_MESSAGE
  #define LC_ALL        (_GLB __aeabi_LC_ALL)
#else /* __AEABI_PORTABILITY_INTERNAL_LEVEL */
  /* LOCALE CATEGORY HANDLES */
  #define LC_COLLATE      _CATMASK(_LC_COLLATE)
  #define LC_CTYPE        _CATMASK(_LC_CTYPE)
  #define LC_MONETARY     _CATMASK(_LC_MONETARY)
  #define LC_NUMERIC      _CATMASK(_LC_NUMERIC)
  #define LC_TIME         _CATMASK(_LC_TIME)
  #define LC_MESSAGE      _CATMASK(_LC_MESSAGE)
  #define LC_MESSAGES     LC_MESSAGE
  #define LC_ALL          (_CATMASK(_NCAT) - 1)
#endif /* __AEABI_PORTABILITY_INTERNAL_LEVEL */

#define _X_COLLATE      LC_COLLATE
#define _X_CTYPE        LC_CTYPE
#define _X_MONETARY     LC_MONETARY
#define _X_NUMERIC      LC_NUMERIC
#define _X_TIME         LC_TIME
#define _X_MESSAGES     LC_MESSAGES
#define _X_MAX          _X_MESSAGES     /* highest real category */


/* TYPE DEFINITIONS */
#if __AEABI_PORTABILITY_INTERNAL_LEVEL
  #include <xlocale_alconv.h>
#else
  #include <xlocale_lconv.h>
#endif

/* DECLARATIONS */
_EXTERN_C
  #if _DLIB_FULL_LOCALE_SUPPORT
    #if _DLIB_SUPPORT_FOR_AEABI
      __ATTRIBUTES struct __aeabi_lconv *  __aeabi_localeconv(void);
    #endif
    #if !__AEABI_PORTABILITY_INTERNAL_LEVEL
      __ATTRIBUTES struct lconv *          localeconv(void);
    #endif
    __ATTRIBUTES char *                    setlocale(int, const char *);
  #endif /* _DLIB_FULL_LOCALE_SUPPORT */
_EXTERN_C_END

#endif /* _LOCALE */

/*
 * Copyright (c) by P.J. Plauger. All rights reserved.
 * Consult your license regarding permissions and restrictions.
V6.50:0576 */
