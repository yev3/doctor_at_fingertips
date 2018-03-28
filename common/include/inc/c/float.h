/* float.h standard header -- IEEE 754 version */
/* Copyright 2003-2017 IAR Systems AB. */
#ifndef _FLOAT
#define _FLOAT

#ifndef _SYSTEM_BUILD
  #pragma system_include
#endif

#include <ycheck.h>
#include <yvals.h>

#if __SHORT_SIZE__ != 2
  #error "Float implementation assumes short is 2 bytes"
#endif

/* COMMON PROPERTIES */
#define FLT_RADIX       2
#define FLT_ROUNDS      1 /* Towards nearest */

#if !_DLIB_ONLY_C89
  #define FLT_EVAL_METHOD 0 /* All operations are performed in own type */

                            /*
                             * If converting a floating-point number to
                             * and then back from a decimal number with
                             * DECIMAL_DIG digits then no information
                             * should be lost.
                             */

  #if __LONG_DOUBLE_SIZE__ == 8
    #define DECIMAL_DIG 17
  #else
    #define DECIMAL_DIG 10
  #endif
#endif /* !_DLIB_ONLY_C89 */

/* Setup 4 byte floating point values */
#define _FP4_MAX         3.40282347e38   /* 0x1.FFFFFEp127 */
#define _FP4_EPSILON     1.19209290e-7   /* 0x1.0p-23 */
#define _FP4_MIN         1.17549436e-38  /* 0x1.0p-126 */
#define _FP4_TRUE_MIN    1.40129846E-45  /* 0x1.0p-149 */

/* float properties */
#if __FLOAT_SIZE__ != 4
  #error "Float size must be 4 bytes"
#endif
#define FLT_MANT_DIG    24
#define FLT_DIG         6
#define FLT_MIN_EXP     -125
#define FLT_MIN_10_EXP  -37
#define FLT_MAX_EXP     128
#define FLT_MAX_10_EXP  38
#define FLT_MAX         _GLUE(_FP4_MAX, f)
#define FLT_EPSILON     _GLUE(_FP4_EPSILON, f)
#define FLT_MIN         _GLUE(_FP4_MIN, f)
#define FLT_DECIMAL_DIG 6
#define FLT_HAS_SUBNORM __SUBNORMAL_FLOATING_POINTS__
#if __SUBNORMAL_FLOATING_POINTS__
  #define FLT_TRUE_MIN  _GLUE(_FP4_TRUE_MIN, f)
#else
  #define FLT_TRUE_MIN  FLT_MIN
#endif

/* double properties */
#if __DOUBLE_SIZE__ == 4
  #define DBL_MANT_DIG    FLT_MANT_DIG
  #define DBL_DIG         FLT_DIG
  #define DBL_MIN_EXP     FLT_MIN_EXP
  #define DBL_MIN_10_EXP  FLT_MIN_10_EXP
  #define DBL_MAX_EXP     FLT_MAX_EXP
  #define DBL_MAX_10_EXP  FLT_MAX_10_EXP
  #define DBL_MAX         _FP4_MAX
  #define DBL_EPSILON     _FP4_EPSILON
  #define DBL_MIN         _FP4_MIN
  #define DBL_DECIMAL_DIG FLT_DECIMAL_DIG
  #define DBL_TRUE_MIN    FLT_TRUE_MIN
#elif __DOUBLE_SIZE__ == 8
  #define DBL_MANT_DIG    53
  #define DBL_DIG         15
  #define DBL_MIN_EXP     -1021
  #define DBL_MIN_10_EXP  -307
  #define DBL_MAX_EXP     1024
  #define DBL_MAX_10_EXP  308
  #define DBL_MAX         1.7976931348623157e308  /* 0x1.FFFFFFFFFFFFFp1023 */
  #define DBL_EPSILON     2.2204460492503131e-16  /* 0x1.0p-52 */
  #define DBL_MIN         2.2250738585072014e-308 /* 0x1.0p-1022 */
  #define DBL_DECIMAL_DIG 15
  #if __SUBNORMAL_FLOATING_POINTS__
    #define DBL_TRUE_MIN  4.9406564584124654E-324 /* 0x1.0p-1074 */
  #else
    #define DBL_TRUE_MIN  DBL_MIN
  #endif
#else
  #error "Double type must be either 4 or 8 bytes"
#endif
#define DBL_HAS_SUBNORM __SUBNORMAL_FLOATING_POINTS__

/* long double properties */
#if __LONG_DOUBLE_SIZE__ != __DOUBLE_SIZE__
  #error "Long double size must be the same as double size"
#endif
#define LDBL_MANT_DIG    DBL_MANT_DIG
#define LDBL_DIG         DBL_DIG
#define LDBL_MIN_EXP     DBL_MIN_EXP
#define LDBL_MIN_10_EXP  DBL_MIN_10_EXP
#define LDBL_MAX_EXP     DBL_MAX_EXP
#define LDBL_MAX_10_EXP  DBL_MAX_10_EXP
#define LDBL_MAX         _GLUE(DBL_MAX, l)
#define LDBL_EPSILON     _GLUE(DBL_EPSILON, l)
#define LDBL_MIN         _GLUE(DBL_MIN, l)
#define LDBL_DECIMAL_DIG DBL_DIG
#define LDBL_HAS_SUBNORM DBL_HAS_SUBNORM
#define LDBL_TRUE_MIN    DBL_TRUE_MIN

#endif /* _FLOAT */

/*
 * Copyright (c) by P.J. Plauger. All rights reserved.
 * Consult your license regarding permissions and restrictions.
V6.50:0576 */
