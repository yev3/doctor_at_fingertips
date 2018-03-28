/* assert.h standard header */
/* Copyright 2003-2017 IAR Systems AB. */

/* Note that there is no include guard for this header. This is intentional. */

/*
 * The following symbols control the behaviour of "assert". The
 * default behaviour is to report if the test failed.
 *
 *    NDEBUG   -- If defined then the assert expression will not be tested. In
 *                fact it will not even be part of the application, so don't
 *                rely on side effects taking place! (If you create a "Release"
 *                project in Embedded Workbench, this symbol is defined.)
 *
 *    _DLIB_ASSERT_ABORT -- Call abort directly if the test fails.
 *
 *    _DLIB_ASSERT_VERBOSE -- Generate output on the terminal for all
 *                            successful assertions.
 *
 * Note: _VERBOSE_DEBUGGING left in as a synonym for
 *       _DLIB_ASSERT_VERBOSE for backward compatibility. 
 */

#ifndef _SYSTEM_BUILD
  #pragma system_include
#endif

#include <ycheck.h>
#include <yvals.h>

#undef assert   /* remove existing definition */

#if __AEABI_PORTABILITY_INTERNAL_LEVEL && !defined(__AEABI_PORTABLE)
  #define __AEABI_PORTABLE
#endif

#if !defined(__cplusplus) && _DLIB_ONLY_C89 == 0
  #define static_assert _Static_assert
#endif

_EXTERN_C
  #ifdef __ICCARM__
    #pragma no_bounds
    __ATTRIBUTES void __aeabi_assert(char const *, char const *, int);
  #else
    __ATTRIBUTES void __aeabi_assert(char const *, char const *, int,
                                     char const *);
  #endif
  #pragma no_bounds
  __ATTRIBUTES int  __iar_ReportAssert(const char *, const char *,
                                       const char *, const char *);
  __WEAK
  __ATTRIBUTES void __iar_EmptyStepPoint(void);
_EXTERN_C_END

#ifdef NDEBUG
  #define assert(test)  ((void)0)
#else /* NDEBUG */
  #if __AEABI_PORTABILITY_INTERNAL_LEVEL
    #define _STEPPOINT ((void)0)
  #else
    /* This allows the debugger to stop on a well-defined point after
     * the assertion. */
    #define _STEPPOINT (__iar_EmptyStepPoint())
  #endif

  #if _DLIB_ONLY_C89
    #define __FUNC 0
  #else
    #define __FUNC __func__
  #endif

  #ifdef __ICCARM__
    #define __CALL __aeabi_assert(_STRINGIFY(test), __FILE__, __LINE__)
  #else
    #define __CALL __aeabi_assert(_STRINGIFY(test), __FILE__, __LINE__, __FUNC)
  #endif

  #ifdef _DLIB_ASSERT_ABORT
    _EXTERN_C
      __ATTRIBUTES_NORETURN void abort(void);
    _EXTERN_C_END
    #define assert(test) ((test) ? (void) 0 : abort())
  #else
    #define assert(test) ((test) ? (void) 0 : (__CALL, _STEPPOINT))
  #endif /* _DLIB_ASSERT_XXX */
#endif /* NDEBUG */

/*
 * Copyright (c) by P.J. Plauger. All rights reserved.
 * Consult your license regarding permissions and restrictions.
V6.50:0576 */
