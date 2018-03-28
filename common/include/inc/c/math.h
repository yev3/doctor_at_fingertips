/* math.h standard header */
/* Copyright 2003-2017 IAR Systems AB. */
#ifndef _MATH
#define _MATH

#ifndef _SYSTEM_BUILD
  #pragma system_include
#endif

#include <ycheck.h>
#include <yvals.h>
#if !_DLIB_ONLY_C89
  #include <DLib_float_setup.h>
#endif
#ifdef __cplusplus
  #include <xtgmath.h>
#endif

#if __AEABI_PORTABILITY_INTERNAL_LEVEL
  #ifndef __AEABI_PORTABLE
    #define __AEABI_PORTABLE
  #endif

  _EXTERN_C
    _DLIB_CONST_ATTR extern float const       __aeabi_HUGE_VALF;
    _DLIB_CONST_ATTR extern float const       __aeabi_INFINITY;
    _DLIB_CONST_ATTR extern float const       __aeabi_NAN;
    _DLIB_CONST_ATTR extern double const      __aeabi_HUGE_VAL;
    _DLIB_CONST_ATTR extern long double const __aeabi_HUGE_VALL;
  _EXTERN_C_END

  #define HUGE_VAL        (_GLB __aeabi_HUGE_VAL)

  #if !_DLIB_ONLY_C89
    #define HUGE_VALF     (_GLB __aeabi_HUGE_VALF)
    #define HUGE_VALL     (_GLB __aeabi_HUGE_VALL)
    #define INFINITY      (_GLB __aeabi_INFINITY)
    #define NAN           (_GLB __aeabi_NAN)
  #endif
#else /* __AEABI_PORTABILITY_INTERNAL_LEVEL */
  #define HUGE_VAL        (0.Infinity)

  #if !_DLIB_ONLY_C89
    #define HUGE_VALF     ((float)       0.Infinity)
    #define HUGE_VALL     ((long double) 0.Infinity)
    #define INFINITY      ((float)       0.Infinity)
    #define NAN           ((float)       0.NaN)
  #endif
#endif /* __AEABI_PORTABILITY_INTERNAL_LEVEL */

#if !_DLIB_ONLY_C89
  typedef float float_t;
  typedef double double_t;
#endif


_EXTERN_C
  /* double declarations */
  __EFF_NS __ATTRIBUTES      double       acos(double);
  __EFF_NS __ATTRIBUTES      double       asin(double);
  __EFF_NS __ATTRIBUTES      double       atan(double);
  __EFF_NS __ATTRIBUTES      double       atan2(double, double);
  __EFF_NE __ATTRIBUTES      double       ceil(double);
  __EFF_NS __ATTRIBUTES      double       cos(double);
  __EFF_NS __ATTRIBUTES      double       cosh(double);
  __EFF_NS __ATTRIBUTES      double       exp(double);
  __EFF_NE __ATTRIBUTES      double       fabs(double);
  __EFF_NE __ATTRIBUTES      double       floor(double);
  __EFF_NS __ATTRIBUTES      double       fmod(double, double);
  __EFF_NENR2 __ATTRIBUTES   double       frexp(double, int *);
  __EFF_NS __ATTRIBUTES      double       ldexp(double, int);
  __EFF_NS __ATTRIBUTES      double       log(double);
  __EFF_NS __ATTRIBUTES      double       log10(double);
  __EFF_NENR2 __ATTRIBUTES   double       modf(double, double *);
  __EFF_NS __ATTRIBUTES      double       pow(double, double);
  __EFF_NS __ATTRIBUTES      double       sin(double);
  __EFF_NS __ATTRIBUTES      double       sinh(double);
  __EFF_NS __ATTRIBUTES      double       sqrt(double);
  __EFF_NS __ATTRIBUTES      double       tan(double);
  __EFF_NS __ATTRIBUTES      double       tanh(double);

  #if !_DLIB_ONLY_C89
    __EFF_NS __ATTRIBUTES    double       acosh(double);
    __EFF_NS __ATTRIBUTES    double       asinh(double);
    __EFF_NS __ATTRIBUTES    double       atanh(double);
    __EFF_NE __ATTRIBUTES    double       cbrt(double);
    __EFF_NE __ATTRIBUTES    double       copysign(double, double);
    #if _DLIB_ALLOW_LARGE_CONSTANT_TABLES_FOR_MATH
      __EFF_NS __ATTRIBUTES  double       erf(double);
      __EFF_NS __ATTRIBUTES  double       erfc(double);
      __EFF_NS __ATTRIBUTES  double       expm1(double);
    #endif /* _DLIB_ALLOW_LARGE_CONSTANT_TABLES_FOR_MATH */
    __EFF_NS __ATTRIBUTES    double       exp2(double);
    __EFF_NE __ATTRIBUTES    double       fdim(double, double);
    #if _DLIB_ALLOW_LARGE_CONSTANT_TABLES_FOR_MATH
      __EFF_NS __ATTRIBUTES  double       fma(double, double, double);
    #endif /* _DLIB_ALLOW_LARGE_CONSTANT_TABLES_FOR_MATH */
    __EFF_NE __ATTRIBUTES    double       fmax(double, double);
    __EFF_NE __ATTRIBUTES    double       fmin(double, double);
    __EFF_NS __ATTRIBUTES    double       hypot(double, double);
    __EFF_NE __ATTRIBUTES    int          ilogb(double);
    #if _DLIB_ALLOW_LARGE_CONSTANT_TABLES_FOR_MATH
      __EFF_NS __ATTRIBUTES  double       lgamma(double);
    #endif /* _DLIB_ALLOW_LARGE_CONSTANT_TABLES_FOR_MATH */
    __EFF_NS __ATTRIBUTES    long long    llrint(double);
    __EFF_NS __ATTRIBUTES    long long    llround(double);
    __EFF_NS  __ATTRIBUTES   double       log1p(double);
    __EFF_NS  __ATTRIBUTES   double       log2(double);
    __EFF_NS  __ATTRIBUTES   double       logb(double);
    __EFF_NS  __ATTRIBUTES   long         lrint(double);
    __EFF_NS  __ATTRIBUTES   long         lround(double);
    __EFF_NE  __ATTRIBUTES   double       nan(const char *);
    __EFF_NS __ATTRIBUTES    double       nearbyint(double);
    __EFF_NS __ATTRIBUTES    double       nextafter(double, double);
    __EFF_NS __ATTRIBUTES    double       nexttoward(double, long double);
    __EFF_NS __ATTRIBUTES    double       remainder(double, double);
    __EFF_NS __ATTRIBUTES    double       remquo(double, double, int *);
    __EFF_NS __ATTRIBUTES    double       rint(double);
    __EFF_NE __ATTRIBUTES    double       round(double);
    __EFF_NS __ATTRIBUTES    double       scalbn(double, int);
    __EFF_NS __ATTRIBUTES    double       scalbln(double, long);
    #if _DLIB_ALLOW_LARGE_CONSTANT_TABLES_FOR_MATH
      __EFF_NS __ATTRIBUTES  double       tgamma(double);
    #endif /* _DLIB_ALLOW_LARGE_CONSTANT_TABLES_FOR_MATH */
    __EFF_NE __ATTRIBUTES    double       trunc(double);

    /* float declarations */
    __EFF_NS __ATTRIBUTES    float        acosf(float);
    __EFF_NS __ATTRIBUTES    float        acoshf(float);
    __EFF_NS __ATTRIBUTES    float        asinf(float);
    __EFF_NS __ATTRIBUTES    float        asinhf(float);
    __EFF_NS __ATTRIBUTES    float        atanf(float);
    __EFF_NS __ATTRIBUTES    float        atanhf(float);
    __EFF_NS __ATTRIBUTES    float        atan2f(float, float);
    __EFF_NE __ATTRIBUTES    float        ceilf(float);
    __EFF_NS __ATTRIBUTES    float        coshf(float);
    __EFF_NS __ATTRIBUTES    float        cosf(float);
    __EFF_NE __ATTRIBUTES    float        cbrtf(float);
    __EFF_NE __ATTRIBUTES    float        copysignf(float, float);
    #if _DLIB_ALLOW_LARGE_CONSTANT_TABLES_FOR_MATH
      __EFF_NS __ATTRIBUTES  float        erff(float);
      __EFF_NS __ATTRIBUTES  float        erfcf(float);
    #endif /* _DLIB_ALLOW_LARGE_CONSTANT_TABLES_FOR_MATH */
    __EFF_NS __ATTRIBUTES    float        expf(float);
    #if _DLIB_ALLOW_LARGE_CONSTANT_TABLES_FOR_MATH
      __EFF_NS __ATTRIBUTES  float        expm1f(float);
    #endif /* _DLIB_ALLOW_LARGE_CONSTANT_TABLES_FOR_MATH */
    __EFF_NS __ATTRIBUTES    float        exp2f(float);
    __EFF_NE __ATTRIBUTES    float        fabsf(float);
    __EFF_NE __ATTRIBUTES    float        fdimf(float, float);
    __EFF_NE __ATTRIBUTES    float        floorf(float);
    #if _DLIB_ALLOW_LARGE_CONSTANT_TABLES_FOR_MATH
      __EFF_NS __ATTRIBUTES  float        fmaf(float, float, float);
    #endif /* _DLIB_ALLOW_LARGE_CONSTANT_TABLES_FOR_MATH */
    __EFF_NE __ATTRIBUTES    float        fmaxf(float, float);
    __EFF_NE __ATTRIBUTES    float        fminf(float, float);
    __EFF_NS __ATTRIBUTES    float        fmodf(float, float);
    __EFF_NENR2 __ATTRIBUTES float        frexpf(float, int *);
    __EFF_NS __ATTRIBUTES    float        hypotf(float, float);
    __EFF_NE __ATTRIBUTES    int          ilogbf(float);
    __EFF_NS __ATTRIBUTES    float        ldexpf(float, int);
    #if _DLIB_ALLOW_LARGE_CONSTANT_TABLES_FOR_MATH
      __EFF_NS __ATTRIBUTES  float        lgammaf(float);
    #endif /* _DLIB_ALLOW_LARGE_CONSTANT_TABLES_FOR_MATH */
    __EFF_NS __ATTRIBUTES    long long    llrintf(float);
    __EFF_NS __ATTRIBUTES    long long    llroundf(float);
    __EFF_NS __ATTRIBUTES    float        logbf(float);
    __EFF_NS __ATTRIBUTES    float        logf(float);
    __EFF_NS __ATTRIBUTES    float        log1pf(float);
    __EFF_NS __ATTRIBUTES    float        log2f(float);
    __EFF_NS __ATTRIBUTES    float        log10f(float);
    __EFF_NS __ATTRIBUTES    long         lrintf(float);
    __EFF_NS __ATTRIBUTES    long         lroundf(float);
    __EFF_NENR2 __ATTRIBUTES float        modff(float, float *);
    __EFF_NE __ATTRIBUTES    float        nanf(const char *);
    __EFF_NS __ATTRIBUTES    float        nearbyintf(float);
    __EFF_NS __ATTRIBUTES    float        nextafterf(float, float);
    __EFF_NS __ATTRIBUTES    float        nexttowardf(float, long double);
    __EFF_NS __ATTRIBUTES    float        powf(float, float);
    __EFF_NS __ATTRIBUTES    float        remainderf(float, float);
    __EFF_NS __ATTRIBUTES    float        remquof(float, float, int *);
    __EFF_NS __ATTRIBUTES    float        rintf(float);
    __EFF_NE __ATTRIBUTES    float        roundf(float);
    __EFF_NS __ATTRIBUTES    float        scalbnf(float, int);
    __EFF_NS __ATTRIBUTES    float        scalblnf(float, long);
    __EFF_NS __ATTRIBUTES    float        sinf(float);
    __EFF_NS __ATTRIBUTES    float        sinhf(float);
    __EFF_NS __ATTRIBUTES    float        sqrtf(float);
    __EFF_NS __ATTRIBUTES    float        tanf(float);
    __EFF_NS __ATTRIBUTES    float        tanhf(float);
    #if _DLIB_ALLOW_LARGE_CONSTANT_TABLES_FOR_MATH
      __EFF_NS __ATTRIBUTES  float        tgammaf(float);
    #endif /* _DLIB_ALLOW_LARGE_CONSTANT_TABLES_FOR_MATH */
    __EFF_NE __ATTRIBUTES    float        truncf(float);

    /* long double declarations */
    __EFF_NS __ATTRIBUTES    long double  acoshl(long double);
    __EFF_NS __ATTRIBUTES    long double  acosl(long double);
    __EFF_NS __ATTRIBUTES    long double  asinhl(long double);
    __EFF_NS __ATTRIBUTES    long double  asinl(long double);
    __EFF_NS __ATTRIBUTES    long double  atanl(long double);
    __EFF_NS __ATTRIBUTES    long double  atanhl(long double);
    __EFF_NS __ATTRIBUTES    long double  atan2l(long double, long double);
    __EFF_NE __ATTRIBUTES    long double  ceill(long double);
    __EFF_NE __ATTRIBUTES    long double  cbrtl(long double);
    __EFF_NE __ATTRIBUTES    long double  copysignl(long double, long double);
    __EFF_NS __ATTRIBUTES    long double  coshl(long double);
    __EFF_NS __ATTRIBUTES    long double  cosl(long double);
    #if _DLIB_ALLOW_LARGE_CONSTANT_TABLES_FOR_MATH
      __EFF_NS __ATTRIBUTES  long double  erfl(long double);
      __EFF_NS __ATTRIBUTES  long double  erfcl(long double);
    #endif /* _DLIB_ALLOW_LARGE_CONSTANT_TABLES_FOR_MATH */
    __EFF_NS __ATTRIBUTES    long double  expl(long double);
    #if _DLIB_ALLOW_LARGE_CONSTANT_TABLES_FOR_MATH
      __EFF_NS __ATTRIBUTES  long double  expm1l(long double);
    #endif /* _DLIB_ALLOW_LARGE_CONSTANT_TABLES_FOR_MATH */
    __EFF_NS __ATTRIBUTES    long double  exp2l(long double);
    __EFF_NE __ATTRIBUTES    long double  fabsl(long double);
    __EFF_NE __ATTRIBUTES    long double  fdiml(long double, long double);
    __EFF_NE __ATTRIBUTES    long double  floorl(long double);
    #if _DLIB_ALLOW_LARGE_CONSTANT_TABLES_FOR_MATH
      __EFF_NS __ATTRIBUTES  long double  fmal(long double, long double, 
                                               long double);
    #endif /* _DLIB_ALLOW_LARGE_CONSTANT_TABLES_FOR_MATH */
    __EFF_NE __ATTRIBUTES    long double  fmaxl(long double, long double);
    __EFF_NE __ATTRIBUTES    long double  fminl(long double, long double);
    __EFF_NS __ATTRIBUTES    long double  fmodl(long double, long double);
    __EFF_NENR2 __ATTRIBUTES long double  frexpl(long double, int *);
    __EFF_NS __ATTRIBUTES    long double  hypotl(long double, long double);
    __EFF_NE __ATTRIBUTES    int          ilogbl(long double);
    __EFF_NS __ATTRIBUTES    long double  ldexpl(long double, int);
    #if _DLIB_ALLOW_LARGE_CONSTANT_TABLES_FOR_MATH
      __EFF_NS __ATTRIBUTES  long double  lgammal(long double);
    #endif /* _DLIB_ALLOW_LARGE_CONSTANT_TABLES_FOR_MATH */
    __EFF_NS __ATTRIBUTES    long long    llrintl(long double);
    __EFF_NS __ATTRIBUTES    long long    llroundl(long double);
    __EFF_NS __ATTRIBUTES    long double  logbl(long double);
    __EFF_NS __ATTRIBUTES    long double  logl(long double);
    __EFF_NS __ATTRIBUTES    long double  log1pl(long double);
    __EFF_NS __ATTRIBUTES    long double  log10l(long double);
    __EFF_NS __ATTRIBUTES    long double  log2l(long double);
    __EFF_NS __ATTRIBUTES    long         lrintl(long double);
    __EFF_NS __ATTRIBUTES    long         lroundl(long double);
    __EFF_NENR2 __ATTRIBUTES long double  modfl(long double, long double *);
    __EFF_NE __ATTRIBUTES    long double  nanl(const char *);
    __EFF_NS __ATTRIBUTES    long double  nearbyintl(long double);
    __EFF_NS __ATTRIBUTES    long double  nextafterl(long double, long double);
    __EFF_NS __ATTRIBUTES    long double  nexttowardl(long double, long double);
    __EFF_NS __ATTRIBUTES    long double  powl(long double, long double);
    __EFF_NS __ATTRIBUTES    long double  remainderl(long double, long double);
    __EFF_NS __ATTRIBUTES    long double  remquol(long double, long double,
                                                  int *);
    __EFF_NS __ATTRIBUTES    long double  rintl(long double);
    __EFF_NE __ATTRIBUTES    long double  roundl(long double);
    __EFF_NS __ATTRIBUTES    long double  scalbnl(long double, int);
    __EFF_NS __ATTRIBUTES    long double  scalblnl(long double, long);
    __EFF_NS __ATTRIBUTES    long double  sinhl(long double);
    __EFF_NS __ATTRIBUTES    long double  sinl(long double);
    __EFF_NS __ATTRIBUTES    long double  sqrtl(long double);
    __EFF_NS __ATTRIBUTES    long double  tanl(long double);
    __EFF_NS __ATTRIBUTES    long double  tanhl(long double);
    #if _DLIB_ALLOW_LARGE_CONSTANT_TABLES_FOR_MATH
      __EFF_NS __ATTRIBUTES  long double  tgammal(long double);
    #endif /* _DLIB_ALLOW_LARGE_CONSTANT_TABLES_FOR_MATH */
    __EFF_NE __ATTRIBUTES    long double  truncl(long double);
  #endif /* !_DLIB_ONLY_C89 */

  /* Alternative functions */
  #if __AEABI_PORTABILITY_INTERNAL_LEVEL == 0 && !_DLIB_ONLY_C89
    __EFF_NS __ATTRIBUTES   double        __iar_cos_medium(double);
    __EFF_NS __ATTRIBUTES   double        __iar_exp_medium(double);
    __EFF_NS __ATTRIBUTES   double        __iar_log_medium(double);
    __EFF_NS __ATTRIBUTES   double        __iar_log10_medium(double);
    __EFF_NS __ATTRIBUTES   double        __iar_log2_medium(double);
    __EFF_NS __ATTRIBUTES   double        __iar_pow_medium(double, double);
    __EFF_NS __ATTRIBUTES   double        __iar_sin_medium(double);
    __EFF_NS __ATTRIBUTES   double        __iar_tan_medium(double);

    __EFF_NS __ATTRIBUTES   float         __iar_cos_mediumf(float);
    __EFF_NS __ATTRIBUTES   float         __iar_exp_mediumf(float);
    __EFF_NS __ATTRIBUTES   float         __iar_log_mediumf(float);
    __EFF_NS __ATTRIBUTES   float         __iar_log10_mediumf(float);
    __EFF_NS __ATTRIBUTES   float         __iar_log2_mediumf(float);
    __EFF_NS __ATTRIBUTES   float         __iar_pow_mediumf(float, float);
    __EFF_NS __ATTRIBUTES   float         __iar_sin_mediumf(float);
    __EFF_NS __ATTRIBUTES   float         __iar_tan_mediumf(float);

    __EFF_NS __ATTRIBUTES   long double   __iar_cos_mediuml(long double);
    __EFF_NS __ATTRIBUTES   long double   __iar_exp_mediuml(long double);
    __EFF_NS __ATTRIBUTES   long double   __iar_log_mediuml(long double);
    __EFF_NS __ATTRIBUTES   long double   __iar_log10_mediuml(long double);
    __EFF_NS __ATTRIBUTES   long double   __iar_log2_mediuml(long double);
    __EFF_NS __ATTRIBUTES   long double   __iar_pow_mediuml(long double,
                                                           long double);
    __EFF_NS __ATTRIBUTES   long double   __iar_sin_mediuml(long double);
    __EFF_NS __ATTRIBUTES   long double   __iar_tan_mediuml(long double);

    #if _DLIB_ALLOW_LARGE_CONSTANT_TABLES_FOR_MATH && \
        !defined(_DLIB_DO_NOT_ADD_ACCURATE_FUNCTIONS)
      __EFF_NS __ATTRIBUTES double        __iar_cos_accurate(double);
      __EFF_NS __ATTRIBUTES double        __iar_pow_accurate(double, double);
      __EFF_NS __ATTRIBUTES double        __iar_sin_accurate(double);
      __EFF_NS __ATTRIBUTES double        __iar_tan_accurate(double);

      __EFF_NS __ATTRIBUTES float         __iar_cos_accuratef(float);
      __EFF_NS __ATTRIBUTES float         __iar_pow_accuratef(float, float);
      __EFF_NS __ATTRIBUTES float         __iar_sin_accuratef(float);
      __EFF_NS __ATTRIBUTES float         __iar_tan_accuratef(float);

      __EFF_NS __ATTRIBUTES long double   __iar_cos_accuratel(long double);
      __EFF_NS __ATTRIBUTES long double   __iar_pow_accuratel(long double, 
                                                              long double); 
      __EFF_NS __ATTRIBUTES long double   __iar_sin_accuratel(long double);
      __EFF_NS __ATTRIBUTES long double   __iar_tan_accuratel(long double);
    #endif /* _DLIB_ALLOW_LARGE_CONSTANT_TABLES_FOR_MATH && 
              !defined(_DLIB_DO_NOT_ADD_ACCURATE_FUNCTIONS) */

    #ifndef _DLIB_DO_NOT_ADD_SMALL_FUNCTIONS
      __EFF_NS __ATTRIBUTES double        __iar_cos_small(double);
      __EFF_NS __ATTRIBUTES double        __iar_exp_small(double);
      __EFF_NS __ATTRIBUTES double        __iar_log_small(double);
      __EFF_NS __ATTRIBUTES double        __iar_log10_small(double);
      __EFF_NS __ATTRIBUTES double        __iar_log2_small(double);
      __EFF_NS __ATTRIBUTES double        __iar_pow_small(double, double);
      __EFF_NS __ATTRIBUTES double        __iar_sin_small(double);
      __EFF_NS __ATTRIBUTES double        __iar_tan_small(double);

      __EFF_NS __ATTRIBUTES float         __iar_cos_smallf(float);
      __EFF_NS __ATTRIBUTES float         __iar_exp_smallf(float);
      __EFF_NS __ATTRIBUTES float         __iar_log_smallf(float);
      __EFF_NS __ATTRIBUTES float         __iar_log10_smallf(float);
      __EFF_NS __ATTRIBUTES float         __iar_log2_smallf(float);
      __EFF_NS __ATTRIBUTES float         __iar_pow_smallf(float, float);
      __EFF_NS __ATTRIBUTES float         __iar_sin_smallf(float);
      __EFF_NS __ATTRIBUTES float         __iar_tan_smallf(float);

      __EFF_NS __ATTRIBUTES long double   __iar_cos_smalll(long double);
      __EFF_NS __ATTRIBUTES long double   __iar_exp_smalll(long double);
      __EFF_NS __ATTRIBUTES long double   __iar_log_smalll(long double);
      __EFF_NS __ATTRIBUTES long double   __iar_log10_smalll(long double);
      __EFF_NS __ATTRIBUTES long double   __iar_log2_smalll(long double);
      __EFF_NS __ATTRIBUTES long double   __iar_pow_smalll(long double,
                                                         long double);
      __EFF_NS __ATTRIBUTES long double   __iar_sin_smalll(long double);
      __EFF_NS __ATTRIBUTES long double   __iar_tan_smalll(long double);
    #endif /* _DLIB_DO_NOT_ADD_SMALL_FUNCTIONS */
  #endif /* __AEABI_PORTABILITY_INTERNAL_LEVEL == 0 && !_DLIB_ONLY_C89 */
_EXTERN_C_END

#ifdef __cplusplus
  extern "C++" {
    __EFF_NE __ATTRIBUTES   double        abs(double);
    __EFF_NS __ATTRIBUTES   double        pow(double, int);

    __EFF_NE __ATTRIBUTES   float         abs(float);
    __EFF_NS __ATTRIBUTES   float         acos(float);
    __EFF_NS __ATTRIBUTES   float         acosh(float);
    __EFF_NS __ATTRIBUTES   float         asin(float);
    __EFF_NS __ATTRIBUTES   float         asinh(float);
    __EFF_NS __ATTRIBUTES   float         atan(float);
    __EFF_NS __ATTRIBUTES   float         atanh(float);
    __EFF_NS __ATTRIBUTES   float         atan2(float, float);
    __EFF_NE __ATTRIBUTES   float         cbrt(float);
    __EFF_NE __ATTRIBUTES   float         ceil(float);
    __EFF_NE __ATTRIBUTES   float         copysign(float, float);
    __EFF_NS __ATTRIBUTES   float         cos(float);
    __EFF_NS __ATTRIBUTES   float         cosh(float);
    #if _DLIB_ALLOW_LARGE_CONSTANT_TABLES_FOR_MATH
      __EFF_NS __ATTRIBUTES float         erf(float);
      __EFF_NS __ATTRIBUTES float         erfc(float);
    #endif /* _DLIB_ALLOW_LARGE_CONSTANT_TABLES_FOR_MATH */
    __EFF_NS __ATTRIBUTES   float         exp(float);
    #if _DLIB_ALLOW_LARGE_CONSTANT_TABLES_FOR_MATH
      __EFF_NS __ATTRIBUTES float         expm1(float);
    #endif /* _DLIB_ALLOW_LARGE_CONSTANT_TABLES_FOR_MATH */
    __EFF_NS __ATTRIBUTES   float         exp2(float);
    __EFF_NE __ATTRIBUTES   float         fabs(float);
    __EFF_NE __ATTRIBUTES   float         fdim(float, float);
    __EFF_NE __ATTRIBUTES   float         floor(float);
    #if _DLIB_ALLOW_LARGE_CONSTANT_TABLES_FOR_MATH
      __EFF_NS __ATTRIBUTES float         fma(float, float, float);
    #endif /* _DLIB_ALLOW_LARGE_CONSTANT_TABLES_FOR_MATH */
    __EFF_NE __ATTRIBUTES   float         fmax(float, float);
    __EFF_NE __ATTRIBUTES   float         fmin(float, float);
    __EFF_NS __ATTRIBUTES   float         fmod(float, float);
    __EFF_NENR2 __ATTRIBUTES float        frexp(float, int *);
    __EFF_NS __ATTRIBUTES   float         hypot(float, float);
    __EFF_NE __ATTRIBUTES   int           ilogb(float);
    __EFF_NS __ATTRIBUTES   float         ldexp(float, int);
    #if _DLIB_ALLOW_LARGE_CONSTANT_TABLES_FOR_MATH
      __EFF_NS __ATTRIBUTES float         lgamma(float);
    #endif /* _DLIB_ALLOW_LARGE_CONSTANT_TABLES_FOR_MATH */
    __EFF_NS __ATTRIBUTES   long long     llrint(float);
    __EFF_NS __ATTRIBUTES   long long     llround(float);
    __EFF_NS __ATTRIBUTES   float         log(float);
    __EFF_NS __ATTRIBUTES   float         logb(float);
    __EFF_NS __ATTRIBUTES   float         log1p(float);
    __EFF_NS __ATTRIBUTES   float         log10(float);
    __EFF_NS __ATTRIBUTES   float         log2(float);
    __EFF_NS __ATTRIBUTES   long          lrint(float);
    __EFF_NS __ATTRIBUTES   long          lround(float);
    __EFF_NENR2 __ATTRIBUTES float        modf(float, float *);
    __EFF_NS __ATTRIBUTES   float         nearbyint(float);
    __EFF_NS __ATTRIBUTES   float         nextafter(float, float);
    __EFF_NS __ATTRIBUTES   float         nexttoward(float, long double);
    __EFF_NS __ATTRIBUTES   float         pow(float, float);
    __EFF_NS __ATTRIBUTES   float         pow(float, int);
    __EFF_NS __ATTRIBUTES   float         remainder(float, float);
    __EFF_NS __ATTRIBUTES   float         remquo(float, float, int *);
    __EFF_NS __ATTRIBUTES   float         rint(float);
    __EFF_NE __ATTRIBUTES   float         round(float);
    __EFF_NS __ATTRIBUTES   float         scalbn(float, int);
    __EFF_NS __ATTRIBUTES   float         scalbln(float, long);
    __EFF_NS __ATTRIBUTES   float         sin(float);
    __EFF_NS __ATTRIBUTES   float         sinh(float);
    __EFF_NS __ATTRIBUTES   float         sqrt(float);
    __EFF_NS __ATTRIBUTES   float         tan(float);
    __EFF_NS __ATTRIBUTES   float         tanh(float);
    #if _DLIB_ALLOW_LARGE_CONSTANT_TABLES_FOR_MATH
      __EFF_NS __ATTRIBUTES float         tgamma(float);
    #endif /* _DLIB_ALLOW_LARGE_CONSTANT_TABLES_FOR_MATH */
    __EFF_NE __ATTRIBUTES   float         trunc(float);

    __EFF_NE __ATTRIBUTES   long double   abs(long double);
    __EFF_NS __ATTRIBUTES   long double   acos(long double);
    __EFF_NS __ATTRIBUTES   long double   acosh(long double);
    __EFF_NS __ATTRIBUTES   long double   asin(long double);
    __EFF_NS __ATTRIBUTES   long double   asinh(long double);
    __EFF_NS __ATTRIBUTES   long double   atan(long double);
    __EFF_NS __ATTRIBUTES   long double   atanh(long double);
    __EFF_NS __ATTRIBUTES   long double   atan2(long double, long double);
    __EFF_NE __ATTRIBUTES   long double   ceil(long double);
    __EFF_NE __ATTRIBUTES   long double   cbrt(long double);
    __EFF_NE __ATTRIBUTES   long double   copysign(long double, long double);
    __EFF_NS __ATTRIBUTES   long double   cos(long double);
    __EFF_NS __ATTRIBUTES   long double   cosh(long double);
    #if _DLIB_ALLOW_LARGE_CONSTANT_TABLES_FOR_MATH
      __EFF_NS __ATTRIBUTES long double   erf(long double);
      __EFF_NS __ATTRIBUTES long double   erfc(long double);
    #endif /* _DLIB_ALLOW_LARGE_CONSTANT_TABLES_FOR_MATH */
    __EFF_NS __ATTRIBUTES   long double   exp(long double);
    #if _DLIB_ALLOW_LARGE_CONSTANT_TABLES_FOR_MATH
      __EFF_NS __ATTRIBUTES long double   expm1(long double);
    #endif /* _DLIB_ALLOW_LARGE_CONSTANT_TABLES_FOR_MATH */
    __EFF_NS __ATTRIBUTES   long double   exp2(long double);
    __EFF_NE __ATTRIBUTES   long double   fabs(long double);
    __EFF_NE __ATTRIBUTES   long double   fdim(long double, long double);
    __EFF_NE __ATTRIBUTES   long double   floor(long double);
    #if _DLIB_ALLOW_LARGE_CONSTANT_TABLES_FOR_MATH
      __EFF_NS __ATTRIBUTES long double   fma(long double, long double, 
                                              long double);
    #endif /* _DLIB_ALLOW_LARGE_CONSTANT_TABLES_FOR_MATH */
    __EFF_NE __ATTRIBUTES   long double   fmax(long double, long double);
    __EFF_NE __ATTRIBUTES   long double   fmin(long double, long double);
    __EFF_NS __ATTRIBUTES   long double   fmod(long double, long double);
    __EFF_NENR2 __ATTRIBUTES long double  frexp(long double, int *);
    __EFF_NS __ATTRIBUTES   long double   hypot(long double, long double);
    __EFF_NE __ATTRIBUTES   int           ilogb(long double);
    __EFF_NS __ATTRIBUTES   long double   ldexp(long double, int);
    #if _DLIB_ALLOW_LARGE_CONSTANT_TABLES_FOR_MATH
      __EFF_NS __ATTRIBUTES long double   lgamma(long double);
    #endif /* _DLIB_ALLOW_LARGE_CONSTANT_TABLES_FOR_MATH */
    __EFF_NS __ATTRIBUTES   long long     llrint(long double);
    __EFF_NS __ATTRIBUTES   long long     llround(long double);
    __EFF_NS __ATTRIBUTES   long double   log(long double);
    __EFF_NS __ATTRIBUTES   long double   logb(long double);
    __EFF_NS __ATTRIBUTES   long double   log1p(long double);
    __EFF_NS __ATTRIBUTES   long double   log10(long double);
    __EFF_NS __ATTRIBUTES   long double   log2(long double);
    __EFF_NS __ATTRIBUTES   long          lrint(long double);
    __EFF_NS __ATTRIBUTES   long          lround(long double);
    __EFF_NENR2 __ATTRIBUTES long double  modf(long double, long double *);
    __EFF_NS __ATTRIBUTES   long double   nearbyint(long double);
    __EFF_NS __ATTRIBUTES   long double   nextafter(long double, long double);
    __EFF_NS __ATTRIBUTES   long double   nexttoward(long double, long double);
    __EFF_NS __ATTRIBUTES   long double   pow(long double, long double);
    __EFF_NS __ATTRIBUTES   long double   pow(long double, int);
    __EFF_NS __ATTRIBUTES   long double   remainder(long double, long double);
    __EFF_NS __ATTRIBUTES   long double   remquo(long double, long double,
                                                 int *);
    __EFF_NS __ATTRIBUTES   long double   rint(long double);
    __EFF_NE __ATTRIBUTES   long double   round(long double);
    __EFF_NS __ATTRIBUTES   long double   scalbn(long double, int);
    __EFF_NS __ATTRIBUTES   long double   scalbln(long double, long);
    __EFF_NS __ATTRIBUTES   long double   sin(long double);
    __EFF_NS __ATTRIBUTES   long double   sinh(long double);
    __EFF_NS __ATTRIBUTES   long double   sqrt(long double);
    __EFF_NS __ATTRIBUTES   long double   tan(long double);
    __EFF_NS __ATTRIBUTES   long double   tanh(long double);
    #if _DLIB_ALLOW_LARGE_CONSTANT_TABLES_FOR_MATH
      __EFF_NS __ATTRIBUTES long double   tgamma(long double);
    #endif /* _DLIB_ALLOW_LARGE_CONSTANT_TABLES_FOR_MATH */
    __EFF_NE __ATTRIBUTES   long double   trunc(long double);
  } /* extern "C++" */
#endif /* __cplusplus */


/* Helper functions for classify */
enum __FPclass { __kInfinity, __kNan, __kFinite, __kDenorm, __kZero };

#pragma inline=forced
__intrinsic int __iar_FPclassify32(float x)
{
  if (!__iar_isfinite32(x))
  {
    if (__iar_isnan32(x))
    {
      return __kNan;
    }
    return __kInfinity;
  }
  if (__iar_iszero32(x))
  {
    return __kZero;
  }
  if (__iar_issubnormal32(x))
  {
    return __kDenorm;
  }
  return __kFinite;
}

#if __DOUBLE_SIZE__ == 8
#pragma inline=forced
__intrinsic int __iar_FPclassify64(double x)
{
  if (!__iar_isfinite64(x))
  {
    if (__iar_isnan64(x))
    {
      return __kNan;
    }
    return __kInfinity;
  }
  if (__iar_iszero64(x))
  {
    return __kZero;
  }
  if (__iar_issubnormal64(x))
  {
    return __kDenorm;
  }
  return __kFinite;
}
#endif /* __DOUBLE_SIZE__ == 8 */

/* Low-level helper functions */
#define   __iar_signbitf      __iar_signbit32
#define   __iar_FPclassifyf   __iar_FPclassify32
#define   __iar_isinff        __iar_isinf32
#define   __iar_isnanf        __iar_isnan32
#define   __iar_iszerof       __iar_iszero32
#define   __iar_isnormalf     __iar_isnormal32
#define   __iar_issubnormalf  __iar_issubnormal32
#define   __iar_isfinitef     __iar_isfinite32
#define   __iar_copysignf     __iar_copysign32

#if __FLOAT_SIZE__ == __DOUBLE_SIZE__
  #define __iar_signbit       __iar_signbit32
  #define __iar_FPclassify    __iar_FPclassify32
  #define __iar_isinf         __iar_isinf32
  #define __iar_isnan         __iar_isnan32
  #define __iar_iszero        __iar_iszero32
  #define __iar_isnormal      __iar_isnormal32
  #define __iar_issubnormal   __iar_issubnormal32
  #define __iar_isfinite      __iar_isfinite32
  #define __iar_copysign      __iar_copysign32
#else /*  __FLOAT_SIZE__ != __DOUBLE_SIZE__ */
  #define __iar_signbit       __iar_signbit64
  #define __iar_FPclassify    __iar_FPclassify64
  #define __iar_isinf         __iar_isinf64
  #define __iar_isnan         __iar_isnan64
  #define __iar_iszero        __iar_iszero64
  #define __iar_isnormal      __iar_isnormal64
  #define __iar_issubnormal   __iar_issubnormal64
  #define __iar_isfinite      __iar_isfinite64
  #define __iar_copysign      __iar_copysign32
#endif /* __FLOAT_SIZE__ == __DOUBLE_SIZE__ */

#define   __iar_signbitl      __iar_signbit
#define   __iar_FPclassifyl   __iar_FPclassify
#define   __iar_isinfl        __iar_isinf
#define   __iar_isnanl        __iar_isnan
#define   __iar_iszerol       __iar_iszero
#define   __iar_isnormall     __iar_isnormal
#define   __iar_issubnormall  __iar_issubnormal
#define   __iar_isfinitel     __iar_isfinite
#define   __iar_copysignl     __iar_copysign


#if defined(__cplusplus)
  extern "C++" {
    /* C++ definitions */

    #pragma inline=forced
    inline double abs(double _X)
    {       /* return absolute value */
      return fabs(_X);
    }

    #pragma inline=forced
    inline double pow(double _X, int _Y)
    {       /* raise to integer power */
      unsigned int _N = (unsigned int)_Y;
      if (_Y < 0)
        _N = 0 - _N;
      
      for (double _Z = 1; ; _X *= _X)
      {
        if ((_N & 1) != 0)
          _Z *= _X;
        if ((_N >>= 1) == 0)
          return _Y < 0 ? (_Z == 0.0 ? HUGE_VAL : (double)(1) / _Z) : _Z;
      }
    }

    #pragma inline=forced
    inline float abs(float _X)
    {       /* return absolute value */
      return fabsf(_X);
    }

    #pragma inline=forced
    inline float acos(float _X)
    {       /* return arccosine */
      return acosf(_X);
    }

    #pragma inline=forced
    inline float asin(float _X)
    {       /* return arcsine */
      return asinf(_X);
    }

    #pragma inline=forced
    inline float atan(float _X)
    {       /* return arctangent */
      return atanf(_X);
    }

    #pragma inline=forced
    inline float atan2(float _Y, float _X)
    {       /* return arctangent */
      return atan2f(_Y, _X);
    }

    #pragma inline=forced
    inline float ceil(float _X)
    {       /* return ceiling */
      return ceilf(_X);
    }

    #pragma inline=forced
    inline float cos(float _X)
    {       /* return cosine */
      return cosf(_X);
    }

    #pragma inline=forced
    inline float cosh(float _X)
    {       /* return hyperbolic cosine */
      return coshf(_X);
    }

    #pragma inline=forced
    inline float exp(float _X)
    {       /* return exponential */
      return expf(_X);
    }

    #pragma inline=forced
    inline float fabs(float _X)
    {       /* return absolute value */
      return fabsf(_X);
    }

    #pragma inline=forced
    inline float floor(float _X)
    {       /* return floor */
      return floorf(_X);
    }

    #pragma inline=forced
    inline float fmod(float _X, float _Y)
    {       /* return modulus */
      return fmodf(_X, _Y);
    }

    #pragma inline=forced
    inline float frexp(float _X, int *_Y)
    {       /* unpack exponent */
      return frexpf(_X, _Y);
    }

    #pragma inline=forced
    inline float ldexp(float _X, int _Y)
    {       /* pack exponent */
      return ldexpf(_X, _Y);
    }

    #pragma inline=forced
    inline float log(float _X)
    {       /* return natural logarithm */
      return logf(_X);
    }

    #pragma inline=forced
    inline float log10(float _X)
    {       /* return base-10 logarithm */
      return log10f(_X);
    }

    #pragma inline=forced
    inline float modf(float _X, float *_Y)
    {       /* unpack fraction */
      return modff(_X, _Y);
    }

    #pragma inline=forced
    inline float pow(float _X, float _Y)
    {       /* raise to power */
      return powf(_X, _Y);
    }

    #pragma inline=forced
    inline float pow(float _X, int _Y)
    {       /* raise to integer power */
    #if __FLOAT_SIZE__ == __DOUBLE_SIZE__
      return powf(_X, _Y);
    #else
      unsigned int _N = (unsigned int)_Y;
      if (_Y < 0)
        _N = 0 - _N;
  
      for (float _Z = 1; ; _X *= _X)
      {
        if ((_N & 1) != 0)
          _Z *= _X;
        if ((_N >>= 1) == 0)
          return _Y < 0 ? (_Z == 0.0F ? (float)HUGE_VAL : (float)(1) / _Z) : _Z;
      }
    #endif /* __FLOAT_SIZE__ == __DOUBLE_SIZE__ */
    }

    #pragma inline=forced
    inline float sin(float _X)
    {       /* return sine */
      return sinf(_X);
    }

    #pragma inline=forced
    inline float sinh(float _X)
    {       /* return hyperbolic sine */
      return sinhf(_X);
    }

    #pragma inline=forced
    inline float sqrt(float _X)
    {       /* return square root */
      return sqrtf(_X);
    }

    #pragma inline=forced
    inline float tan(float _X)
    {       /* return tangent */
      return tanf(_X);
    }

    #pragma inline=forced
    inline float tanh(float _X)
    {       /* return hyperbolic tangent */
      return tanhf(_X);
    }

    #pragma inline=forced
    inline float acosh(float _Left)
    {       // return hyperbolic arccosine
      return acoshf(_Left);
    }

    #pragma inline=forced
    inline float asinh(float _Left)
    {       // return hyperbolic arcsine
      return asinhf(_Left);
    }

    #pragma inline=forced
    inline float atanh(float _Left)
    {       // return hyperbolic arctangent
      return atanhf(_Left);
    }

    #pragma inline=forced
    inline float cbrt(float _Left)
    {       // return cube root
      return cbrtf(_Left);
    }

    #pragma inline=forced
    inline float copysign(float _Left, float _Right)
    {       // return copysign
      return copysignf(_Left, _Right);
    }

    #if _DLIB_ALLOW_LARGE_CONSTANT_TABLES_FOR_MATH
      #pragma inline=forced
      inline float erf(float _Left)
      {       // return erf
        return erff(_Left); 
      }

      #pragma inline=forced
      inline float erfc(float _Left)
      {       // return erfc
        return erfcf(_Left);
      }

      #pragma inline=forced
      inline float expm1(float _Left)
      {       // return expml
        return expm1f(_Left);
      }
    #endif /* _DLIB_ALLOW_LARGE_CONSTANT_TABLES_FOR_MATH */

    #pragma inline=forced
    inline float exp2(float _Left)
    {       // return exp2
      return exp2f(_Left);
    }

    #pragma inline=forced
    inline float fdim(float _Left, float _Right)
    {       // return fdim
      return fdimf(_Left, _Right);
    }

    #if _DLIB_ALLOW_LARGE_CONSTANT_TABLES_FOR_MATH
      #pragma inline=forced
      inline float fma(float _Left, float _Right, float _Addend)
      {       // return fma
        return fmaf(_Left, _Right, _Addend);
      }
    #endif /* _DLIB_ALLOW_LARGE_CONSTANT_TABLES_FOR_MATH */

    #pragma inline=forced
    inline float fmax(float _Left, float _Right)
    {       // return fmax
      return fmaxf(_Left, _Right);
    }

    #pragma inline=forced
    inline float fmin(float _Left, float _Right)
    {       // return fmin
      return fminf(_Left, _Right);
    }

    #pragma inline=forced
    inline float hypot(float _Left, float _Right)
    {       // return hypot
      return hypotf(_Left, _Right);
    }

    #pragma inline=forced
    inline int ilogb(float _Left)
    {       // return ilogb
      return ilogbf(_Left);
    }

    #if _DLIB_ALLOW_LARGE_CONSTANT_TABLES_FOR_MATH
      #pragma inline=forced
      inline float lgamma(float _Left)
      {       // return lgamma
        return lgammaf(_Left);
      }
    #endif /* _DLIB_ALLOW_LARGE_CONSTANT_TABLES_FOR_MATH */

    #pragma inline=forced
    inline long long llrint(float _Left)
    {       // return llrint
      return llrintf(_Left);
    }

    #pragma inline=forced
    inline long long llround(float _Left)
    {       // return llround
      return llroundf(_Left);
    }

    #pragma inline=forced
    inline float log1p(float _Left)
    {       // return loglp
      return log1pf(_Left);
    }

    #pragma inline=forced
    inline float log2(float _Left)
    {       // return log2
      return log2f(_Left);
    }

    #pragma inline=forced
    inline float logb(float _Left)
    {       // return logb
      return logbf(_Left);
    }

    #pragma inline=forced
    inline long lrint(float _Left)
    {       // return lrint
      return lrintf(_Left);
    }

    #pragma inline=forced
    inline long lround(float _Left)
    {       // return lround
      return lroundf(_Left);
    }

    #pragma inline=forced
    inline float nearbyint(float _Left)
    {       // return nearbyint
      return nearbyintf(_Left);
    }

    #pragma inline=forced
    inline float nextafter(float _Left, float _Right)
    {       // return nextafter
      return nextafterf(_Left, _Right);
    }

    #pragma inline=forced
    inline float nexttoward(float _Left, long double _Right)
    {       // return nexttoward
      return nexttowardf(_Left, _Right);
    }

    #pragma inline=forced
    inline float remainder(float _Left, float _Right)
    {       // return remainder
      return remainderf(_Left, _Right);
    }

    #pragma inline=forced
    inline float remquo(float _Left, float _Right, int *_Pval)
    {       // return remquo
      return remquof(_Left, _Right, _Pval);
    }

    #pragma inline=forced
    inline float rint(float _Left)
    {       // return rint
      return rintf(_Left);
    }

    #pragma inline=forced
    inline float round(float _Left)
    {       // return round
      return roundf(_Left);
    }

    #pragma inline=forced
    inline float scalbn(float _Left, int _Right)
    {       // return scalbn
      return scalbnf(_Left, _Right);
    }

    #pragma inline=forced
    inline float scalbln(float _Left, long _Right)
    {       // return scalbln
      return scalblnf(_Left, _Right);
    }

    #if _DLIB_ALLOW_LARGE_CONSTANT_TABLES_FOR_MATH
      #pragma inline=forced
      inline float tgamma(float _Left)
      {       // return tgamma
        return tgammaf(_Left);
      }
    #endif /* _DLIB_ALLOW_LARGE_CONSTANT_TABLES_FOR_MATH */

    #pragma inline=forced
    inline float trunc(float _Left)
    {       // return trunc
      return truncf(_Left);
    }

    #pragma inline=forced
    inline long double abs(long double _X)  /* OVERLOADS */
    {       /* return absolute value */
      return fabsl(_X);
    }

    #pragma inline=forced
    inline long double acos(long double _X)
    {       /* return arccosine */
      return acosl(_X);
    }

    #pragma inline=forced
    inline long double asin(long double _X)
    {       /* return arcsine */
      return asinl(_X);
    }

    #pragma inline=forced
    inline long double atan(long double _X)
    {       /* return arctangent */
      return atanl(_X);
    }

    #pragma inline=forced
    inline long double atan2(long double _Y, long double _X)
    {       /* return arctangent */
      return atan2l(_Y, _X);
    }

    #pragma inline=forced
    inline long double ceil(long double _X)
    {       /* return ceiling */
      return ceill(_X);
    }

    #pragma inline=forced
    inline long double cos(long double _X)
    {       /* return cosine */
      return cosl(_X);
    }

    #pragma inline=forced
    inline long double cosh(long double _X)
    {       /* return hyperbolic cosine */
      return coshl(_X);
    }

    #pragma inline=forced
    inline long double exp(long double _X)
    {       /* return exponential */
      return expl(_X);
    }

    #pragma inline=forced
    inline long double fabs(long double _X)
    {       /* return absolute value */
      return fabsl(_X);
    }

    #pragma inline=forced
    inline long double floor(long double _X)
    {       /* return floor */
      return floorl(_X);
    }

    #pragma inline=forced
    inline long double fmod(long double _X, long double _Y)
    {       /* return modulus */
      return fmodl(_X, _Y);
    }

    #pragma inline=forced
    inline long double frexp(long double _X, int *_Y)
    {       /* unpack exponent */
      return frexpl(_X, _Y);
    }

    #pragma inline=forced
    inline long double ldexp(long double _X, int _Y)
    {       /* pack exponent */
      return ldexpl(_X, _Y);
    }

    #pragma inline=forced
    inline long double log(long double _X)
    {       /* return natural logarithm */
      return logl(_X);
    }

    #pragma inline=forced
    inline long double log10(long double _X)
    {       /* return base-10 logarithm */
      return log10l(_X);
    }

    #pragma inline=forced
    inline long double modf(long double _X, long double *_Y)
    {       /* unpack fraction */
      return modfl(_X, _Y);
    }

    #pragma inline=forced
    inline long double pow(long double _X, long double _Y)
    {       /* raise to power */
      return powl(_X, _Y);
    }

    #pragma inline=forced
    inline long double pow(long double _X, int _Y)
    {       /* raise to integer power */
    #if __LONG_DOUBLE_SIZE__ == __DOUBLE_SIZE__
      return powl(_X, _Y);
    #else
      unsigned int _N = (unsigned int)_Y;
      if (_Y < 0)
        _N = 0 - _N;
      
      for (long double _Z = 1; ; _X *= _X)
      {
        if ((_N & 1) != 0)
          _Z *= _X;
        if ((_N >>= 1) == 0)
          return _Y < 0 
            ? (_Z == 0.0L 
               ? (long double)HUGE_VAL 
               : (long double)(1) / _Z) 
            : _Z;
      }
    #endif /* __LONG_DOUBLE_SIZE__ == __DOUBLE_SIZE__ */
    }

    #pragma inline=forced
    inline long double sin(long double _X)
    {       /* return sine */
      return sinl(_X);
    }

    #pragma inline=forced
    inline long double sinh(long double _X)
    {       /* return hyperbolic sine */
      return sinhl(_X);
    }

    #pragma inline=forced
    inline long double sqrt(long double _X)
    {       /* return square root */
      return sqrtl(_X);
    }

    #pragma inline=forced
    inline long double tan(long double _X)
    {       /* return tangent */
      return tanl(_X);
    }

    #pragma inline=forced
    inline long double tanh(long double _X)
    {       /* return hyperbolic tangent */
      return tanhl(_X);
    }

    #pragma inline=forced
    inline long double acosh(long double _Left)
    {   // return acosh
      return acoshl(_Left);
    }

    #pragma inline=forced
    inline long double asinh(long double _Left)
    {   // return asinh
      return asinhl(_Left);
    }

    #pragma inline=forced
    inline long double atanh(long double _Left)
    {       // return atanh
      return atanhl(_Left);
    }

    #pragma inline=forced
    inline long double cbrt(long double _Left)
    {       // return cbrt
      return cbrtl(_Left);
    }

    #pragma inline=forced
    inline long double copysign(long double _Left, long double _Right)
    {       // return copysign
      return copysignl(_Left, _Right);
    }

    #if _DLIB_ALLOW_LARGE_CONSTANT_TABLES_FOR_MATH
      #pragma inline=forced
      inline long double erf(long double _Left)
      {       // return erf
        return erfl(_Left);
      }

      #pragma inline=forced
      inline long double erfc(long double _Left)
      {       // return erfc
        return erfcl(_Left);
      }

      #pragma inline=forced
      inline long double expm1(long double _Left)
      {       // return expml
        return expm1l(_Left);
      }
    #endif /* _DLIB_ALLOW_LARGE_CONSTANT_TABLES_FOR_MATH */

    #pragma inline=forced
    inline long double exp2(long double _Left)
    {       // return exp2
      return exp2l(_Left);
    }

    #pragma inline=forced
    inline long double fdim(long double _Left, long double _Right)
    {       // return fdim
      return fdiml(_Left, _Right);
    }

    #if _DLIB_ALLOW_LARGE_CONSTANT_TABLES_FOR_MATH
      #pragma inline=forced
      inline long double fma(long double _Left, long double _Right,
                             long double _Addend)
      {       // return fma
        return fmal(_Left, _Right, _Addend);
      }
    #endif /* _DLIB_ALLOW_LARGE_CONSTANT_TABLES_FOR_MATH */

    #pragma inline=forced
    inline long double fmax(long double _Left, long double _Right)
    {       // return fmax
      return fmaxl(_Left, _Right);
    }

    #pragma inline=forced
    inline long double fmin(long double _Left, long double _Right)
    {       // return fmin
      return fminl(_Left, _Right);
    }

    #pragma inline=forced
    inline long double hypot(long double _Left, long double _Right)
    {       // return hypot
      return hypotl(_Left, _Right);
    }

    #pragma inline=forced
    inline int ilogb(long double _Left)
    {       // return ilogb
      return ilogbl(_Left);
    }

    #if _DLIB_ALLOW_LARGE_CONSTANT_TABLES_FOR_MATH
      #pragma inline=forced
      inline long double lgamma(long double _Left)
      {       // return lgamma
        return lgammal(_Left);
      }
    #endif /* _DLIB_ALLOW_LARGE_CONSTANT_TABLES_FOR_MATH */

    #pragma inline=forced
    inline long long llrint(long double _Left)
    {       // return llrint
      return llrintl(_Left);
    }

    #pragma inline=forced
    inline long long llround(long double _Left)
    {       // return llround
      return llroundl(_Left);
    } 

    #pragma inline=forced
    inline long double log1p(long double _Left)
    {       // return loglp
      return log1pl(_Left);
    }

    #pragma inline=forced
    inline long double log2(long double _Left)
    {       // return log2
      return log2l(_Left);
    }

    #pragma inline=forced
    inline long double logb(long double _Left)
    {       // return logb
      return logbl(_Left);
    }

    #pragma inline=forced
    inline long lrint(long double _Left)
    {       // return lrint
      return lrintl(_Left);
    }

    #pragma inline=forced
    inline long lround(long double _Left)
    {       // return lround
      return lroundl(_Left);
    }

    #pragma inline=forced
    inline long double nearbyint(long double _Left)
    {       // return nearbyint
      return nearbyintl(_Left);
    }

    #pragma inline=forced
    inline long double nextafter(long double _Left, long double _Right)
    {       // return nextafter
      return nextafterl(_Left, _Right);
    }

    #pragma inline=forced
    inline long double nexttoward(long double _Left, long double _Right)
    {       // return nexttoward
      return nexttowardl(_Left, _Right);
    }

    #pragma inline=forced
    inline long double remainder(long double _Left, long double _Right)
    {       // return remainder
      return remainderl(_Left, _Right);
    }

    #pragma inline=forced
    inline long double remquo(long double _Left, long double _Right,
                              int *_Pval)
    {       // return remquo
      return remquol(_Left, _Right, _Pval);
    }

    #pragma inline=forced
    inline long double rint(long double _Left)
    {       // return rint
      return rintl(_Left);
    }

    #pragma inline=forced
    inline long double round(long double _Left)
    {       // return round
      return roundl(_Left);
    }

    #pragma inline=forced
    inline long double scalbn(long double _Left, int _Right)
    {       // return scalbn
      return scalbnl(_Left, _Right);
    }

    #pragma inline=forced
    inline long double scalbln(long double _Left, long _Right)
    {       // return scalbln
      return scalblnl(_Left, _Right);
    }

    #if _DLIB_ALLOW_LARGE_CONSTANT_TABLES_FOR_MATH
      #pragma inline=forced
      inline long double tgamma(long double _Left)
      {       // return tgamma
        return tgammal(_Left);
      }
    #endif /* _DLIB_ALLOW_LARGE_CONSTANT_TABLES_FOR_MATH */

    #pragma inline=forced
    inline long double trunc(long double _Left)
    {       // return trunc
      return truncl(_Left);
    }
  } /* extern "C++" */
#endif /* defined(__cplusplus) */

#if !_DLIB_ONLY_C89
  /* C floating point functionality */
  #define FP_INFINITE  __kInfinity
  #define FP_NAN       __kNan
  #define FP_NORMAL    __kFinite
  #define FP_SUBNORMAL __kDenorm
  #define FP_ZERO      __kZero

  #define MATH_ERRNO 1
  #define MATH_ERREXCEPT 2
  #ifndef math_errhandling
    #define math_errhandling MATH_ERRNO
  #endif

  #if _DLIB_FAST_FMA
    #define FP_FAST_FMA  1
    #define FP_FAST_FMAF 1
    #define FP_FAST_FMAL 1
  #endif

  #define FP_ILOGB0     __SIGNED_INT_MIN__
  #define FP_ILOGBNAN   __SIGNED_INT_MAX__

  #ifdef __cplusplus
    extern "C++" {
      // FUNCTION fpclassify
      inline int fpclassify(float _Left)
      {       // classify argument
        return __iar_FPclassifyf(_Left);
      }

      inline int fpclassify(double _Left)
      {       // classify argument
        return __iar_FPclassify(_Left);
      }

      inline int fpclassify(long double _Left)
      {       // classify argument
        return __iar_FPclassifyl(_Left);
      }

      template<class _Ty> inline
      int fpclassify(_Ty _Left)
      {       // classify floating-point argument
        typedef typename std::_Promote_to_float<_Ty>::type type;
        return fpclassify((type)_Left);
      }

      // FUNCTION signbit
      inline bool signbit(float _Left)
      {       // test sign bit
        return __iar_signbitf(_Left);
      }

      inline bool signbit(double _Left)
      {       // test sign bit
        return __iar_signbit(_Left);
      }
    
      inline bool signbit(long double _Left)
      {       // test sign bit
        return __iar_signbitl(_Left);
      }

      template<class _Ty> inline
      bool signbit(_Ty _Left)
      {       // test floating-point sign bit
        typedef typename std::_Promote_to_float<_Ty>::type type;
        return signbit((type)_Left);
      }

      inline bool isfinite(float _Left)
      {       // test for finite
        return __iar_isfinitef(_Left);
      }

      inline bool isfinite(double _Left)
      {       // test for finite
        return __iar_isfinite(_Left);
      }

      inline bool isfinite(long double _Left)
      {       // test for finite
        return __iar_isfinitel(_Left);
      }

      template<class _Ty> inline
      bool isfinite(_Ty _Left)
      {
        typedef typename std::_Promote_to_float<_Ty>::type type;
        return isfinite((type)_Left);
      }

      inline bool isinf(float _Left)
      {       // test for infinite
        return !__iar_isnanf(_Left) && __iar_isinff(_Left);
      }

      inline bool isinf(double _Left)
      {       // test for infinite
        return !__iar_isnan(_Left) && __iar_isinf(_Left);
      }

      inline bool isinf(long double _Left)
      {       // test for infinite
        return !__iar_isnanl(_Left) && __iar_isinfl(_Left);
      }

      template<class _Ty> inline
      bool isinf(_Ty _Left)
      {
        typedef typename std::_Promote_to_float<_Ty>::type type;
        return isinf((type)_Left);
      }

      inline bool isnan(float _Left)
      {       // test for NaN
        return __iar_isnanf(_Left);
      }

      inline bool isnan(double _Left)
      {       // test for NaN
        return __iar_isnan(_Left);
      }

      inline bool isnan(long double _Left)
      {       // test for NaN
        return __iar_isnanl(_Left);
      }

      template<class _Ty> inline
      bool isnan(_Ty _Left)
      {
        typedef typename std::_Promote_to_float<_Ty>::type type;
        return isnan((type)_Left);
      }

      inline bool isnormal(float _Left)
      {       // test for normal
        return !__iar_isnanf(_Left) && __iar_isnormalf(_Left);
      }

      inline bool isnormal(double _Left)
      {       // test for normal
        return !__iar_isnan(_Left) && __iar_isnormal(_Left);
      }

      inline bool isnormal(long double _Left)
      {       // test for normal
        return !__iar_isnanl(_Left) && __iar_isnormall(_Left);
      }

      template<class _Ty> inline
      bool isnormal(_Ty _Left)
      {
        typedef typename std::_Promote_to_float<_Ty>::type type;
        return isnormal((type)_Left);
      }

      template<class _Ty1, class _Ty2> inline
      bool isgreater(_Ty1 _Left, _Ty2 _Right)
      {       // test for _Left > _Right
        return _Left > _Right;
      }

      template<class _Ty1, class _Ty2> inline
      bool isgreaterequal(_Ty1 _Left, _Ty2 _Right)
      {       // test for _Left >= _Right
        return _Left >= _Right;
      }

      template<class _Ty1, class _Ty2> inline
      bool isless(_Ty1 _Left, _Ty2 _Right)
      {       // test for _Left < _Right
        return  _Left < _Right;
      }

      template<class _Ty1, class _Ty2> inline
      bool islessequal(_Ty1 _Left, _Ty2 _Right)
      {       // test for _Left <= _Right
        return _Left <= _Right;
      }

      template<class _Ty1, class _Ty2> inline
      bool islessgreater(_Ty1 _Left, _Ty2 _Right)
      {       // test for _Left != _Right
        return _Left < _Right || _Left > _Right;
      }

      template<class _Ty1, class _Ty2> inline
      bool isunordered(_Ty1 _Left, _Ty2 _Right)
      {       // test for _Left unorderd w.r.t. _Right
        return __iar_isnan(_Left) || __iar_isnan(_Right);
      }
    } /* extern "C++" */
  #else /* defined cplusplus */
    #pragma inline
    __iar_FlagUType __isnormalf(float _Left)
    {       // test for normal
      return !__iar_isnanf(_Left) && __iar_isnormalf(_Left);
    }
    #pragma inline
    __iar_FlagUType __isnormal(double _Left)
    {       // test for normal
      return !__iar_isnan(_Left) && __iar_isnormal(_Left);
    }
    #pragma inline
    __iar_FlagUType __isnormall(long double _Left)
    {       // test for normal
      return !__iar_isnanl(_Left) && __iar_isnormall(_Left);
    }
    #pragma inline
     __iar_FlagUType __isinff(float _Left)
    {       // test for infinite
      return !__iar_isnanf(_Left) && __iar_isinff(_Left);
    }
    #pragma inline
     __iar_FlagUType __isinf(double _Left)
    {       // test for infinite
      return !__iar_isnan(_Left) && __iar_isinf(_Left);
    }
    #pragma inline
     __iar_FlagUType __isinfl(long double _Left)
    {       // test for infinite
      return !__iar_isnanl(_Left) && __iar_isinfl(_Left);
    }


    #define _CARGR(x, fd, ff, fl)     __c99_generic(x,,, fd, ff, fl,,,)(x)
    #define _CARG2R(x, y, fd, ff, fl) __c99_generic(x, y,, fd, ff, fl,,,)(x, y)
      
    #define fpclassify(x) _CARGR(x, __iar_FPclassify,   \
                                    __iar_FPclassifyf,  \
                                    __iar_FPclassifyl)
    #define signbit(x)    _CARGR(x, __iar_signbit,      \
                                    __iar_signbitf,     \
                                    __iar_signbitl)
    #define isfinite(x)   _CARGR(x, __iar_isfinite,     \
                                    __iar_isfinitef,    \
                                    __iar_isfinitel)
    #define isinf(x)      _CARGR(x, __isinf,            \
                                    __isinff,           \
                                    __isinfl)
    #define isnan(x)      _CARGR(x, __iar_isnan,        \
                                    __iar_isnanf,       \
                                    __iar_isnanl)
    #define isnormal(x)   _CARGR(x, __isnormal,         \
                                    __isnormalf,        \
                                    __isnormall)

    #pragma inline
    __iar_FlagUType __isunorderedf(float _x, float _y)
    {
      return __iar_isnanf(_x) || __iar_isnanf(_y);
    }
    #pragma inline
    __iar_FlagUType __isunordered(double _x, double _y)
    {
      return __iar_isnan(_x) || __iar_isnan(_y);
    }
    #pragma inline
    __iar_FlagUType __isunorderedl(long double _x, long double _y)
    {
      return __iar_isnanl(_x) || __iar_isnanl(_y);
    }
    #pragma inline
    __iar_FlagUType __islessgreaterf(float _x, float _y)
    {
      if (__isunorderedf(_x, _y))
        return 0;
      return _x < _y || _x > _y;
    }
    #pragma inline
    __iar_FlagUType __islessgreater(double _x, double _y)
    {
      if (__isunordered(_x, _y))
        return 0;
      return _x < _y || _x > _y;
    }
    #pragma inline
    __iar_FlagUType __islessgreaterl(long double _x, long double _y)
    {
      if (__isunorderedl(_x, _y))
        return 0;
      return _x < _y || _x > _y;
    }

    #define isgreater(x, y)       ((x) >  (y))
    #define isgreaterequal(x, y)  ((x) >= (y))
    #define isless(x, y)          ((x) <  (y))
    #define islessequal(x, y)     ((x) <= (y))
    #define islessgreater(x, y)   _CARG2R(x, y, __islessgreater,    \
                                                __islessgreaterf,   \
                                                __islessgreaterl)
    #define isunordered(x, y)     _CARG2R(x, y, __isunordered,      \
                                                __isunorderedf,     \
                                                __isunorderedl)

  #endif /* defined __cplusplus */
#endif /* !_DLIB_ONLY_C89 */

#endif /* _MATH */

/*
 * Copyright (c) by P.J. Plauger. All rights reserved.
 * Consult your license regarding permissions and restrictions.
V6.50:0576 */
