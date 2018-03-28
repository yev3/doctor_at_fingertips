//
// This file is NOT used by ICCARM, ICCARM uses a compiler builtin version.
// 
// This file is provided for other tools that analyzes code that uses these
// header files.
// Note that the functionallity of the file depends on several macros normally
// defined by ICCARM.
//
// Copyright 2017 IAR Systems AB.
//

#if  defined(__ICCARM__) && !defined(__ICCARM_OLD_DEFINED_VAARGS__)
  #error "Reading built-in header-file. If you used upper case, try #include <stdarg.h>"
#endif

#ifndef _STDARG
#define _STDARG

#ifndef _SYSTEM_BUILD
  #pragma system_include
#endif

#include <ycheck.h>
#include <yvals.h>

_EXTERN_C
  __intrinsic char *__va_start1(void);
_EXTERN_C_END

typedef struct
{
  char *_Ap;
} _VA_LIST;

typedef __Va_list va_list;

#define _SIZE_ON_STACK(T)                                                \
          (((sizeof(T) + __VA_STACK_ALIGN__ - 1) / __VA_STACK_ALIGN__) * \
          __VA_STACK_ALIGN__)

#define _VA_ARG(ap, T)                                      \
      (  __ALIGNOF__(T) > __VA_STACK_ALIGN__                \
       ? (  (*(long *) &(ap)._Ap) += (__ALIGNOF__(T) - 1),  \
            (*(long *) &(ap)._Ap) &= ~(__ALIGNOF__(T) - 1), \
            (*(*(T **) &(ap)._Ap)++))                       \
       : (_SIZE_ON_STACK(T) == sizeof(T)                    \
            ? (*(*(T **) &(ap)._Ap)++)                      \
            : ((ap)._Ap += _SIZE_ON_STACK(T),               \
                    *(T *) ((ap)._Ap - (_SIZE_ON_STACK(T))))))


#define va_start(ap, A) (ap._Ap = _GLB __va_start1())
#define va_end(ap)      ((void) 0)
#define va_arg(ap, T)   _VA_ARG(ap, T)
  
#if !_DLIB_ONLY_C89
  #define va_copy(d,s) ((d) = (s))
#endif /* _DLIB_ADD_C99_SYMBOLS */

#endif /* _STDARG */

/*
 * Copyright (c) 1992-2009 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
V6.42:0576 */
