/* fenv.h standard header */
/* Copyright 2003-2017 IAR Systems AB.  */

#ifndef _FENV
#define _FENV

#ifndef _SYSTEM_BUILD
  #pragma system_include
#endif

#include <yvals.h>

#ifdef _DLIB_PRODUCT_FENV
  #include <DLib_Product_fenv.h>
#else
  _EXTERN_C

    /* TYPES */
    /* The floating-point status flags. */
    typedef unsigned long fexcept_t;
    /* The entire floating-point environment. */
    typedef unsigned long fenv_t;

    /* MACROS */
    /* Supported rounding modes. */
    /* #define FE_DOWNWARD     0x02 */
    #define FE_TONEAREST    0x00
    /* #define FE_TOWARDZERO   0x03 */
    /* #define FE_UPWARD       0x01 */

    /* Supported status flags. */
    /* #define FE_DIVBYZERO    0x02 */
    /* #define FE_INEXACT      0x10 */
    /* #define FE_INVALID      0x01 */
    /* #define FE_OVERFLOW     0x04 */
    /* #define FE_UNDERFLOW    0x08 */
    /* #define FE_ALL_EXCEPT   (  FE_DIVBYZERO | FE_INEXACT    \
                                | FE_INVALID   | FE_OVERFLOW | FE_UNDERFLOW) */
    #define FE_ALL_EXCEPT 0

  _EXTERN_C_END
#endif /* _DLIB_PRODUCT_FENV */


_EXTERN_C
  /* Default floating-point environment. */
  #define FE_DFL_ENV      (_GLB __iar_GetDefaultFenv())

  /* FUNCTION DECLARATIONS */
  __ATTRIBUTES int             feclearexcept(int);
  __ATTRIBUTES int             fegetexceptflag(fexcept_t *, int);
  __ATTRIBUTES int             feraiseexcept(int);
  __ATTRIBUTES int             fesetexceptflag(const fexcept_t *, int);
  __ATTRIBUTES int             fetestexcept(int);

  __ATTRIBUTES int             fegetround(void);
  __ATTRIBUTES int             fesetround(int);

  __ATTRIBUTES int             fegetenv(fenv_t *);
  __ATTRIBUTES int             feholdexcept(fenv_t *);
  __ATTRIBUTES int             fesetenv(const fenv_t *);
  __ATTRIBUTES int             feupdateenv(const fenv_t *);

  #if _DLIB_ADD_EXTRA_SYMBOLS
    __ATTRIBUTES fexcept_t     fegettrapenable(void);
    __ATTRIBUTES int           fesettrapenable(fexcept_t);
  #endif

  __ATTRIBUTES fenv_t const *  __iar_GetDefaultFenv(void);

_EXTERN_C_END

#endif /* _FENV */

/*
 * Copyright (c) by P.J. Plauger. All rights reserved.
 * Consult your license regarding permissions and restrictions.
V6.50:0576 */
