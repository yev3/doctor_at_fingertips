/* signal.h standard header */
/* Copyright 2005-2017 IAR Systems AB. */
#ifndef _SIGNAL
#define _SIGNAL

#ifndef _SYSTEM_BUILD
  #pragma system_include
#endif

#include <ycheck.h>
#include <yvals.h>

_EXTERN_C

/* type definitions */
typedef void _Sigfun(int);

/* low-level functions */
__ATTRIBUTES _Sigfun * signal(int, _Sigfun *);

/* signal return values */
extern _Sigfun __aeabi_SIG_DFL;
extern _Sigfun __aeabi_SIG_IGN;
extern _Sigfun __aeabi_SIG_ERR;

#if __AEABI_PORTABILITY_INTERNAL_LEVEL
  #ifndef __AEABI_PORTABLE
    #define __AEABI_PORTABLE
  #endif

  typedef int sig_atomic_t;

  /* signal codes */
  _DLIB_CONST_ATTR extern int const __aeabi_SIGABRT;
  _DLIB_CONST_ATTR extern int const __aeabi_SIGINT;
  _DLIB_CONST_ATTR extern int const __aeabi_SIGILL;
  _DLIB_CONST_ATTR extern int const __aeabi_SIGFPE;
  _DLIB_CONST_ATTR extern int const __aeabi_SIGSEGV;
  _DLIB_CONST_ATTR extern int const __aeabi_SIGTERM;

  #define SIGABRT (_GLB __aeabi_SIGABRT)
  #define SIGINT  (_GLB __aeabi_SIGINT)
  #define SIGILL  (_GLB __aeabi_SIGILL)
  #define SIGFPE  (_GLB __aeabi_SIGFPE)
  #define SIGSEGV (_GLB __aeabi_SIGSEGV)
  #define SIGTERM (_GLB __aeabi_SIGTERM)
  #define _NSIG   32 /* one more than last code */

  #define SIG_DFL (_GLB __aeabi_SIG_DFL)
  #define SIG_ERR (_GLB __aeabi_SIG_ERR)
  #define SIG_IGN (_GLB __aeabi_SIG_IGN)
#else /* __AEABI_PORTABILITY_INTERNAL_LEVEL */
  typedef signed char sig_atomic_t;

  /* signal codes */
  #define SIGABRT 22
  #define SIGINT  2
  #define SIGILL  4
  #define SIGFPE  8
  #define SIGSEGV 11
  #define SIGTERM 15
  #define _NSIG   24 /* one more than last code */

  /* signal return values */
  #define SIG_DFL ((_GLB _Sigfun *)0)
  #define SIG_ERR ((_GLB _Sigfun *)-1)
  #define SIG_IGN ((_GLB _Sigfun *)1)
#endif /* __AEABI_PORTABILITY_INTERNAL_LEVEL */

/* declarations */
__ATTRIBUTES int raise(int);

_EXTERN_C_END
#endif /* _SIGNAL */

/*
 * Copyright (c) by P.J. Plauger. All rights reserved.
 * Consult your license regarding permissions and restrictions.
V6.50:0576 */
