/* Copyright 2017 IAR Systems AB. */

#ifndef _DLIB_PRODUCT_FENV_H
#define _DLIB_PRODUCT_FENV_H

#ifndef _SYSTEM_BUILD
  #pragma system_include
#endif

_EXTERN_C

  /* The floating-point status flags. */
  typedef unsigned long fexcept_t;


  /* The entire floating-point environment. */
  typedef struct 
  {
    fexcept_t _e;
  } fenv_t;

  #define FE_TONEAREST    0x00

  /* Floating-point exception flags. */
  #define FE_INVALID   (1 << 0)
  #define FE_DIVBYZERO (1 << 1)
  #define FE_OVERFLOW  (1 << 2)
  #define FE_UNDERFLOW (1 << 3)
  #define FE_INEXACT   (1 << 4)

  #define FE_ALL_EXCEPT \
  (FE_INVALID|FE_DIVBYZERO|FE_OVERFLOW|FE_UNDERFLOW|FE_INEXACT)

_EXTERN_C_END
#endif /* _DLIB_PRODUCT_FENV_H */
