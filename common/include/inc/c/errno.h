/* errno.h standard header */
/* Copyright 2003-2017 IAR Systems AB. */
#ifndef _ERRNO
#define _ERRNO

#ifndef _SYSTEM_BUILD
  #pragma system_include
#endif

#include <ycheck.h>
#include <yvals.h>

#if __AEABI_PORTABILITY_INTERNAL_LEVEL
  #ifndef __AEABI_PORTABLE
    #define __AEABI_PORTABLE
  #endif

  /* ERROR CODES */
  _EXTERN_C
    _DLIB_CONST_ATTR extern int const __aeabi_EDOM;
    _DLIB_CONST_ATTR extern int const __aeabi_ERANGE;
    _DLIB_CONST_ATTR extern int const __aeabi_EFPOS;
    _DLIB_CONST_ATTR extern int const __aeabi_EILSEQ;
    _DLIB_CONST_ATTR extern int const __aeabi_ERRMAX;
  _EXTERN_C_END
  #define EDOM    __aeabi_EDOM
  #define ERANGE  __aeabi_ERANGE
  #define EFPOS   __aeabi_EFPOS
  #define EILSEQ  __aeabi_EILSEQ
  #define _NERR   __aeabi_ERRMAX /* one more than last code */
#else /* __AEABI_PORTABILITY_INTERNAL_LEVEL */
  /* ERROR CODES */
  #define EDOM    33
  #define ERANGE  34
  #define EFPOS   35
  #define EILSEQ  36
  #define _NERR   37 /* one more than last code */
#endif /* __AEABI_PORTABILITY_INTERNAL_LEVEL */

#if __STDC_WANT_LIB_EXT1__ == 1
  /* type definitions */
  #if !defined(_ERRNO_T)
    #define _ERRNO_T
    typedef int errno_t;
  #endif
#endif

/* DECLARATIONS */
_EXTERN_C
  __ATTRIBUTES int volatile *__aeabi_errno_addr(void);

  #define errno (* (int *) __aeabi_errno_addr())
_EXTERN_C_END

#endif /* _ERRNO */

/*
 * Copyright (c) by P.J. Plauger. All rights reserved.
 * Consult your license regarding permissions and restrictions.
V6.50:0576 */
