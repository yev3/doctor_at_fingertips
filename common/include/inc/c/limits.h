/* limits.h standard header -- 8-bit version */
/* Copyright 2003-2017 IAR Systems AB. */
#ifndef _LIMITS
#define _LIMITS

#ifndef _SYSTEM_BUILD
  #pragma system_include
#endif

#include <ycheck.h>
#include <yvals.h>

#if __AEABI_PORTABILITY_INTERNAL_LEVEL
  #ifndef __AEABI_PORTABLE
    #define __AEABI_PORTABLE
  #endif

  /* MULTIBYTE PROPERTIES */
  _EXTERN_C
    _DLIB_CONST_ATTR extern int const __aeabi_MB_LEN_MAX;
  _EXTERN_C_END
  #define MB_LEN_MAX (_GLB __aeabi_MB_LEN_MAX)
#else
  /* MULTIBYTE PROPERTIES */
  #define MB_LEN_MAX  _MBMAX
#endif


/* CHAR PROPERTIES */
#define CHAR_BIT              __CHAR_BITS__

#define CHAR_MAX              __CHAR_MAX__
#define CHAR_MIN              __CHAR_MIN__

/* INT PROPERTIES */
#define INT_MAX               __SIGNED_INT_MAX__
#define INT_MIN               __SIGNED_INT_MIN__
#define UINT_MAX              __UNSIGNED_INT_MAX__

/* SIGNED CHAR PROPERTIES */
#define SCHAR_MAX             __SIGNED_CHAR_MAX__
#define SCHAR_MIN             __SIGNED_CHAR_MIN__

/* SHORT PROPERTIES */
#define SHRT_MAX              __SIGNED_SHORT_MAX__
#define SHRT_MIN              __SIGNED_SHORT_MIN__

/* LONG PROPERTIES */
#define LONG_MAX              __SIGNED_LONG_MAX__
#define LONG_MIN              __SIGNED_LONG_MIN__

/* UNSIGNED PROPERTIES */
#define UCHAR_MAX             __UNSIGNED_CHAR_MAX__
#define USHRT_MAX             __UNSIGNED_SHORT_MAX__
#define ULONG_MAX             __UNSIGNED_LONG_MAX__

/* LONG LONG PROPERTIES */
#if !_DLIB_ONLY_C89
  #define LLONG_MIN           __SIGNED_LONG_LONG_MIN__
  #define LLONG_MAX           __SIGNED_LONG_LONG_MAX__
  #define ULLONG_MAX          __UNSIGNED_LONG_LONG_MAX__
#endif

#endif /* _LIMITS */

/*
 * Copyright (c) by P.J. Plauger. All rights reserved.
 * Consult your license regarding permissions and restrictions.
V6.50:0576 */
