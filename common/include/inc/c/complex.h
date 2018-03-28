/* complex.h standard header */
/* Copyright 2001-2017 IAR Systems AB. */
#ifndef _COMPLEX
#define _COMPLEX

#ifndef _SYSTEM_BUILD
  #pragma system_include
#endif

#include <yvals.h>
#include <ycheck.h>
#ifdef __cplusplus
  #include <xtgmath.h>
#endif

#if _DLIB_ONLY_C89
  #error "Header is not supported in the C89 language mode"
#endif

/* MACROS */
#define _Fcomplex               _Fcomplex       /* signal definitions */
#define _DCOMPLEX_(re, im)      __iar_Cbuild(re, im)
#define _FCOMPLEX_(re, im)      __iar_Cbuildf(re, im)
#define _LCOMPLEX_(re, im)      __iar_Cbuildl(re, im)

#ifdef __cplusplus 
  extern "C++" {
    #define _Complex_I    __iar_Cbuildf(0.0F, 1.0F)

                // TYPES
    #ifndef _C_COMPLEX_T
      #define _C_COMPLEX_T

      typedef struct _C_double_complex
      {       /* double complex */
        double _Val[2];
      } _C_double_complex;

      typedef struct _C_float_complex
      {       /* float complex */
        float _Val[2];
      } _C_float_complex;

      typedef struct _C_ldouble_complex
      {       /* long double complex */
        long double _Val[2];
      } _C_ldouble_complex;
    #endif /* _C_COMPLEX_T */

    extern "C" {
      typedef _C_double_complex _Dcomplex;
      typedef _C_float_complex _Fcomplex;
      typedef _C_ldouble_complex _Lcomplex;

      __EFF_NE __ATTRIBUTES double      cimag(_Dcomplex);
      __EFF_NE __ATTRIBUTES double      creal(_Dcomplex);
      __EFF_NE __ATTRIBUTES float       cimagf(_Fcomplex);
      __EFF_NE __ATTRIBUTES float       crealf(_Fcomplex);
      __EFF_NE __ATTRIBUTES long double cimagl(_Lcomplex);
      __EFF_NE __ATTRIBUTES long double creall(_Lcomplex);

      #pragma inline=forced
      inline double cimag(_Dcomplex _Left)
      { // get imaginary part
        return _Left._Val[1];
      }
      #pragma inline=forced
      inline double creal(_Dcomplex _Left)
      { // get real part
        return _Left._Val[0];
      }

      #pragma inline=forced
      inline float cimagf(_Fcomplex _Left)
      { // get imaginary part
        return _Left._Val[1];
      }
      #pragma inline=forced
      inline float crealf(_Fcomplex _Left)
      { // get real part
        return _Left._Val[0];
      }

      #pragma inline=forced
      inline long double cimagl(_Lcomplex _Left)
      { // get imaginary part
        return _Left._Val[1];
      }
      #pragma inline=forced
      inline long double creall(_Lcomplex _Left)
      { // get real part
        return _Left._Val[0];
      }

      __EFF_NE __ATTRIBUTES _Dcomplex __iar_Cbuild(double, double);
      __EFF_NE __ATTRIBUTES _Fcomplex __iar_Cbuildf(float, float);
      __EFF_NE __ATTRIBUTES _Lcomplex __iar_Cbuildl(long double, long double);
      #pragma inline=forced
      inline _Dcomplex __iar_Cbuild(double _re, double _im)
      {
        double ans[2];
        
        ans[0] = _re;
        ans[1] = _im;
        return *(_Dcomplex *)&ans[0];
      }
      #pragma inline=forced
      inline _Fcomplex __iar_Cbuildf(float _re, float _im)
      {
        float ans[2];
        
        ans[0] = _re;
        ans[1] = _im;
        return *(_Fcomplex *)&ans[0];
      }
      #pragma inline=forced
      inline _Lcomplex __iar_Cbuildl(long double _re, long double _im)
      {
        long double ans[2];
        
        ans[0] = _re;
        ans[1] = _im;
        return *(_Lcomplex *)&ans[0];
      }
    } /* extern "C" */
  } /* extern "C++" */
#else /* defined(__cplusplus) */
  /* TYPES */
  typedef double      _Complex _Dcomplex;
  typedef float       _Complex _Fcomplex;
  typedef long double _Complex _Lcomplex;

  #define complex     _Complex

  #define _Complex_I  ((float _Complex)__I__)

  __EFF_NE __ATTRIBUTES _Dcomplex __iar_Cbuild(double, double);
  __EFF_NE __ATTRIBUTES _Fcomplex __iar_Cbuildf(float, float);
  __EFF_NE __ATTRIBUTES _Lcomplex __iar_Cbuildl(long double, long double);

  #define __iar_Cbuild(re, im)    (*(_Dcomplex *)(double []){re, im})
  #define __iar_Cbuildf(re, im)   (*(_Fcomplex *)(float []){re, im})
  #define __iar_Cbuildl(re, im)   (*(_Lcomplex *)(long double []){re, im})

  __EFF_NE __ATTRIBUTES double      cimag(_Dcomplex);
  __EFF_NE __ATTRIBUTES double      creal(_Dcomplex);
  __EFF_NE __ATTRIBUTES float       cimagf(_Fcomplex);
  __EFF_NE __ATTRIBUTES float       crealf(_Fcomplex);
  __EFF_NE __ATTRIBUTES long double cimagl(_Lcomplex);
  __EFF_NE __ATTRIBUTES long double creall(_Lcomplex);

  #pragma inline=no_body /* complex parameter */
  double cimag(_Dcomplex _Left)
  {       // get imaginary part
    return ((double *)&_Left)[1];
  }
  #pragma inline=no_body /* complex parameter */
  double creal(_Dcomplex _Left)
  {       // get real part
    return ((double *)&_Left)[0];
  }

  #pragma inline=no_body /* complex parameter */
  float cimagf(_Fcomplex _Left)
  {       // get imaginary part
    return ((float *)&_Left)[1];
  }
  #pragma inline=no_body /* complex parameter */
  float crealf(_Fcomplex _Left)
  {       // get real part
    return ((float *)&_Left)[0];
  }

  #pragma inline=no_body /* complex parameter */
  long double cimagl(_Lcomplex _Left)
  {       // get imaginary part
    return ((long double *)&_Left)[1];
  }
  #pragma inline=no_body /* complex parameter */
  long double creall(_Lcomplex _Left)
  {       // get real part
    return ((long double *)&_Left)[0];
  }

  #pragma alias_def __iar_cimag32 = cimagf
  #pragma alias_def __iar_creal32 = crealf
  #if __DOUBLE_SIZE__ == 8
    #pragma alias_def __iar_cimag64 = cimag
    #pragma alias_def __iar_creal64 = creal
  #endif
#endif /* defined(__cplusplus) etc. */

/* MACROS */
#ifndef __cplusplus
  #define imaginary      _Imaginary
  #define _Imaginary_I   ((float _Imaginary)_Complex_I)
  #define I              _Imaginary_I
#endif

/* Macros to create complex types */
#define CMPLX(x, y) ((double complex)((double)(x) + _Imaginary_I * (double)(y)))
#define CMPLXF(x, y) ((float complex)((float)(x) + _Imaginary_I * (float)(y)))
#define CMPLXL(x, y) ((long double complex)((long double)(x) + \
                                            _Imaginary_I * (long double)(y)))

/* FUNCTIONS */
_EXTERN_C
  __EFF_NS __ATTRIBUTES double          cabs(_Dcomplex);
  __EFF_NS __ATTRIBUTES _Dcomplex       cacos(_Dcomplex);
  __EFF_NS __ATTRIBUTES _Dcomplex       cacosh(_Dcomplex);
  __EFF_NS __ATTRIBUTES double          carg(_Dcomplex);
  __EFF_NS __ATTRIBUTES _Dcomplex       casin(_Dcomplex);
  __EFF_NS __ATTRIBUTES _Dcomplex       casinh(_Dcomplex);
  __EFF_NS __ATTRIBUTES _Dcomplex       catan(_Dcomplex);
  __EFF_NS __ATTRIBUTES _Dcomplex       catanh(_Dcomplex);
  __EFF_NS __ATTRIBUTES _Dcomplex       ccos(_Dcomplex);
  __EFF_NS __ATTRIBUTES _Dcomplex       ccosh(_Dcomplex);
  __EFF_NS __ATTRIBUTES _Dcomplex       cexp(_Dcomplex);
  /* __EFF_NS __ATTRIBUTES double       cimag(_Dcomplex); */
  /* __EFF_NS __ATTRIBUTES _Dcomplex    clog(_Dcomplex); */
  __EFF_NS __ATTRIBUTES _Dcomplex       clog10(_Dcomplex);
  __EFF_NS __ATTRIBUTES _Dcomplex       conj(_Dcomplex);
  __EFF_NS __ATTRIBUTES _Dcomplex       cpow(_Dcomplex, _Dcomplex);
  __EFF_NS __ATTRIBUTES _Dcomplex       cproj(_Dcomplex);
  /* __EFF_NS __ATTRIBUTES double       creal(_Dcomplex); */
  __EFF_NS __ATTRIBUTES _Dcomplex       csin(_Dcomplex);
  __EFF_NS __ATTRIBUTES _Dcomplex       csinh(_Dcomplex);
  __EFF_NS __ATTRIBUTES _Dcomplex       csqrt(_Dcomplex);
  __EFF_NS __ATTRIBUTES _Dcomplex       ctan(_Dcomplex);
  __EFF_NS __ATTRIBUTES _Dcomplex       ctanh(_Dcomplex);
  __EFF_NS __ATTRIBUTES double          norm(_Dcomplex);

  __EFF_NS __ATTRIBUTES float           cabsf(_Fcomplex);
  __EFF_NS __ATTRIBUTES _Fcomplex       cacosf(_Fcomplex);
  __EFF_NS __ATTRIBUTES _Fcomplex       cacoshf(_Fcomplex);
  __EFF_NS __ATTRIBUTES float           cargf(_Fcomplex);
  __EFF_NS __ATTRIBUTES _Fcomplex       casinf(_Fcomplex);
  __EFF_NS __ATTRIBUTES _Fcomplex       casinhf(_Fcomplex);
  __EFF_NS __ATTRIBUTES _Fcomplex       catanf(_Fcomplex);
  __EFF_NS __ATTRIBUTES _Fcomplex       catanhf(_Fcomplex);
  __EFF_NS __ATTRIBUTES _Fcomplex       ccosf(_Fcomplex);
  __EFF_NS __ATTRIBUTES _Fcomplex       ccoshf(_Fcomplex);
  __EFF_NS __ATTRIBUTES _Fcomplex       cexpf(_Fcomplex);
  /* __EFF_NS __ATTRIBUTES float        cimagf(_Fcomplex); */
  __EFF_NS __ATTRIBUTES _Fcomplex       clogf(_Fcomplex);
  __EFF_NS __ATTRIBUTES _Fcomplex       clog10f(_Fcomplex);
  __EFF_NS __ATTRIBUTES _Fcomplex       conjf(_Fcomplex);
  __EFF_NS __ATTRIBUTES _Fcomplex       cpowf(_Fcomplex, _Fcomplex);
  __EFF_NS __ATTRIBUTES _Fcomplex       cprojf(_Fcomplex);
  /* __EFF_NS __ATTRIBUTES float        crealf(_Fcomplex); */
  __EFF_NS __ATTRIBUTES _Fcomplex       csinf(_Fcomplex);
  __EFF_NS __ATTRIBUTES _Fcomplex       csinhf(_Fcomplex);
  __EFF_NS __ATTRIBUTES _Fcomplex       csqrtf(_Fcomplex);
  __EFF_NS __ATTRIBUTES _Fcomplex       ctanf(_Fcomplex);
  __EFF_NS __ATTRIBUTES _Fcomplex       ctanhf(_Fcomplex);
  __EFF_NS __ATTRIBUTES float           normf(_Fcomplex);

  __EFF_NS __ATTRIBUTES long double     cabsl(_Lcomplex);
  __EFF_NS __ATTRIBUTES _Lcomplex       cacosl(_Lcomplex);
  __EFF_NS __ATTRIBUTES _Lcomplex       cacoshl(_Lcomplex);
  __EFF_NS __ATTRIBUTES long double     cargl(_Lcomplex);
  __EFF_NS __ATTRIBUTES _Lcomplex       casinl(_Lcomplex);
  __EFF_NS __ATTRIBUTES _Lcomplex       casinhl(_Lcomplex);
  __EFF_NS __ATTRIBUTES _Lcomplex       catanl(_Lcomplex);
  __EFF_NS __ATTRIBUTES _Lcomplex       catanhl(_Lcomplex);
  __EFF_NS __ATTRIBUTES _Lcomplex       ccosl(_Lcomplex);
  __EFF_NS __ATTRIBUTES _Lcomplex       ccoshl(_Lcomplex);
  __EFF_NS __ATTRIBUTES _Lcomplex       cexpl(_Lcomplex);
  /* __EFF_NS __ATTRIBUTES long double  cimagl(_Lcomplex); */
  __EFF_NS __ATTRIBUTES _Lcomplex       clogl(_Lcomplex);
  __EFF_NS __ATTRIBUTES _Lcomplex       clog10l(_Lcomplex);
  __EFF_NS __ATTRIBUTES _Lcomplex       conjl(_Lcomplex);
  __EFF_NS __ATTRIBUTES _Lcomplex       cpowl(_Lcomplex, _Lcomplex);
  __EFF_NS __ATTRIBUTES _Lcomplex       cprojl(_Lcomplex);
  /* __EFF_NS __ATTRIBUTES long double  creall(_Lcomplex); */
  __EFF_NS __ATTRIBUTES _Lcomplex       csinl(_Lcomplex);
  __EFF_NS __ATTRIBUTES _Lcomplex       csinhl(_Lcomplex);
  __EFF_NS __ATTRIBUTES _Lcomplex       csqrtl(_Lcomplex);
  __EFF_NS __ATTRIBUTES _Lcomplex       ctanl(_Lcomplex);
  __EFF_NS __ATTRIBUTES _Lcomplex       ctanhl(_Lcomplex);
  __EFF_NS __ATTRIBUTES long double     norml(_Lcomplex);
_EXTERN_C_END

#ifdef __cplusplus
  namespace std {
    extern "C++" {
      __EFF_NS __ATTRIBUTES _Dcomplex   acos(_Dcomplex _Left);
      __EFF_NS __ATTRIBUTES _Dcomplex   acosh(_Dcomplex _Left);
      __EFF_NS __ATTRIBUTES _Dcomplex   asin(_Dcomplex _Left);
      __EFF_NS __ATTRIBUTES _Dcomplex   asinh(_Dcomplex _Left);
      __EFF_NS __ATTRIBUTES _Dcomplex   atan(_Dcomplex _Left);
      __EFF_NS __ATTRIBUTES _Dcomplex   atanh(_Dcomplex _Left);
      __EFF_NS __ATTRIBUTES _Dcomplex   cos(_Dcomplex _Left);
      __EFF_NS __ATTRIBUTES _Dcomplex   cosh(_Dcomplex _Left);
      __EFF_NS __ATTRIBUTES _Dcomplex   exp(_Dcomplex _Left);
      /* __EFF_NS __ATTRIBUTES _Dcomplex log(_Dcomplex _Left); */
      __EFF_NS __ATTRIBUTES _Dcomplex   log10(_Dcomplex _Left);
      __EFF_NS __ATTRIBUTES _Dcomplex   pow(_Dcomplex _Left, _Dcomplex _Right);
      __EFF_NS __ATTRIBUTES _Dcomplex   proj(_Dcomplex _Left);
      __EFF_NS __ATTRIBUTES _Dcomplex   sin(_Dcomplex _Left);
      __EFF_NS __ATTRIBUTES _Dcomplex   sinh(_Dcomplex _Left);
      __EFF_NS __ATTRIBUTES _Dcomplex   sqrt(_Dcomplex _Left);
      __EFF_NS __ATTRIBUTES _Dcomplex   tan(_Dcomplex _Left);
      __EFF_NS __ATTRIBUTES _Dcomplex   tanh(_Dcomplex _Left);
      __EFF_NS __ATTRIBUTES double      abs(_Dcomplex _Left);
      __EFF_NS __ATTRIBUTES double      arg(_Dcomplex _Left);
      __EFF_NS __ATTRIBUTES double      fabs(_Dcomplex _Left);
      __EFF_NS __ATTRIBUTES double      imag(_Dcomplex _Left);
      __EFF_NS __ATTRIBUTES double      real(_Dcomplex _Left);

      __EFF_NS __ATTRIBUTES _Fcomplex   acos(_Fcomplex _Left);
      __EFF_NS __ATTRIBUTES _Fcomplex   acosh(_Fcomplex _Left);
      __EFF_NS __ATTRIBUTES _Fcomplex   asin(_Fcomplex _Left);
      __EFF_NS __ATTRIBUTES _Fcomplex   asinh(_Fcomplex _Left);
      __EFF_NS __ATTRIBUTES _Fcomplex   atan(_Fcomplex _Left);
      __EFF_NS __ATTRIBUTES _Fcomplex   atanh(_Fcomplex _Left);
      __EFF_NS __ATTRIBUTES _Fcomplex   conj(_Fcomplex _Left);
      __EFF_NS __ATTRIBUTES _Fcomplex   cos(_Fcomplex _Left);
      __EFF_NS __ATTRIBUTES _Fcomplex   cosh(_Fcomplex _Left);
      __EFF_NS __ATTRIBUTES _Fcomplex   cproj(_Fcomplex _Left);
      __EFF_NS __ATTRIBUTES _Fcomplex   exp(_Fcomplex _Left);
      __EFF_NS __ATTRIBUTES _Fcomplex   log(_Fcomplex _Left);
      __EFF_NS __ATTRIBUTES _Fcomplex   log10(_Fcomplex _Left);
      __EFF_NS __ATTRIBUTES float       norm(_Fcomplex _Left);
      __EFF_NS __ATTRIBUTES _Fcomplex   pow(_Fcomplex _Left, _Fcomplex _Right);
      __EFF_NS __ATTRIBUTES _Fcomplex   proj(_Fcomplex _Left);
      __EFF_NS __ATTRIBUTES _Fcomplex   sin(_Fcomplex _Left);
      __EFF_NS __ATTRIBUTES _Fcomplex   sinh(_Fcomplex _Left);
      __EFF_NS __ATTRIBUTES _Fcomplex   sqrt(_Fcomplex _Left);
      __EFF_NS __ATTRIBUTES _Fcomplex   tan(_Fcomplex _Left);
      __EFF_NS __ATTRIBUTES _Fcomplex   tanh(_Fcomplex _Left);
      __EFF_NS __ATTRIBUTES float       abs(_Fcomplex _Left);
      __EFF_NS __ATTRIBUTES float       arg(_Fcomplex _Left);
      __EFF_NS __ATTRIBUTES float       carg(_Fcomplex _Left);
      __EFF_NS __ATTRIBUTES float       cimag(_Fcomplex _Left);
      __EFF_NS __ATTRIBUTES float       creal(_Fcomplex _Left);
      __EFF_NS __ATTRIBUTES float       fabs(_Fcomplex _Left);
      __EFF_NS __ATTRIBUTES float       imag(_Fcomplex _Left);
      __EFF_NS __ATTRIBUTES float       real(_Fcomplex _Left);

      __EFF_NS __ATTRIBUTES _Lcomplex   acos(_Lcomplex _Left);
      __EFF_NS __ATTRIBUTES _Lcomplex   acosh(_Lcomplex _Left);
      __EFF_NS __ATTRIBUTES _Lcomplex   asin(_Lcomplex _Left);
      __EFF_NS __ATTRIBUTES _Lcomplex   asinh(_Lcomplex _Left);
      __EFF_NS __ATTRIBUTES _Lcomplex   atan(_Lcomplex _Left);
      __EFF_NS __ATTRIBUTES _Lcomplex   atanh(_Lcomplex _Left);
      __EFF_NS __ATTRIBUTES _Lcomplex   conj(_Lcomplex _Left);
      __EFF_NS __ATTRIBUTES _Lcomplex   cos(_Lcomplex _Left);
      __EFF_NS __ATTRIBUTES _Lcomplex   cosh(_Lcomplex _Left);
      __EFF_NS __ATTRIBUTES _Lcomplex   cproj(_Lcomplex _Left);
      __EFF_NS __ATTRIBUTES _Lcomplex   exp(_Lcomplex _Left);
      __EFF_NS __ATTRIBUTES _Lcomplex   log(_Lcomplex _Left);
      __EFF_NS __ATTRIBUTES _Lcomplex   log10(_Lcomplex _Left);
      __EFF_NS __ATTRIBUTES long double norm(_Lcomplex _Left);
      __EFF_NS __ATTRIBUTES _Lcomplex   pow(_Lcomplex _Left, _Lcomplex _Right);
      __EFF_NS __ATTRIBUTES _Lcomplex   proj(_Lcomplex _Left);
      __EFF_NS __ATTRIBUTES _Lcomplex   sin(_Lcomplex _Left);
      __EFF_NS __ATTRIBUTES _Lcomplex   sinh(_Lcomplex _Left);
      __EFF_NS __ATTRIBUTES _Lcomplex   sqrt(_Lcomplex _Left);
      __EFF_NS __ATTRIBUTES _Lcomplex   tan(_Lcomplex _Left);
      __EFF_NS __ATTRIBUTES _Lcomplex   tanh(_Lcomplex _Left);
      __EFF_NS __ATTRIBUTES long double abs(_Lcomplex _Left);
      __EFF_NS __ATTRIBUTES long double arg(_Lcomplex _Left);
      __EFF_NS __ATTRIBUTES long double carg(_Lcomplex _Left);
      __EFF_NS __ATTRIBUTES long double cimag(_Lcomplex _Left);
      __EFF_NS __ATTRIBUTES long double creal(_Lcomplex _Left);
      __EFF_NS __ATTRIBUTES long double fabs(_Lcomplex _Left);
      __EFF_NS __ATTRIBUTES long double imag(_Lcomplex _Left);
      __EFF_NS __ATTRIBUTES long double real(_Lcomplex _Left);

      // double complex OVERLOADS
      #pragma inline=forced
      inline _Dcomplex acos(_Dcomplex _Left)
      { // compute cacos
        return cacos(_Left);
      }

      #pragma inline=forced
      inline _Dcomplex acosh(_Dcomplex _Left)
      { // compute cacosh
        return cacosh(_Left);
      }

      #pragma inline=forced
      inline _Dcomplex asin(_Dcomplex _Left)
      { // compute casin
        return casin(_Left);
      }

      #pragma inline=forced
      inline _Dcomplex asinh(_Dcomplex _Left)
      { // compute casinh
        return casinh(_Left);
      }

      #pragma inline=forced
      inline _Dcomplex atan(_Dcomplex _Left)
      { // compute catan
        return catan(_Left);
      }

      #pragma inline=forced
      inline _Dcomplex atanh(_Dcomplex _Left)
      { // compute catanh
        return catanh(_Left);
      }

      #pragma inline=forced
      inline _Dcomplex cos(_Dcomplex _Left)
      { // compute ccos
        return ccos(_Left);
      }

      #pragma inline=forced
      inline _Dcomplex cosh(_Dcomplex _Left)
      { // compute ccosh
        return ccosh(_Left);
      }

      #pragma inline=forced
      inline _Dcomplex exp(_Dcomplex _Left)
      { // compute cexp
        return cexp(_Left);
      }

      //  #pragma inline=forced
      //  _Dcomplex log(_Dcomplex _Left)
      //  { // compute clog
      //    return clog(_Left); 
      //  }

      #pragma inline=forced
      inline _Dcomplex log10(_Dcomplex _Left)
      { // compute clog10
        return clog10(_Left);
      }

      #pragma inline=forced
      inline _Dcomplex pow(_Dcomplex _Left, _Dcomplex _Right)
      { // compute cpow
        return cpow(_Left, _Right);
      }

      #pragma inline=forced
      inline _Dcomplex proj(_Dcomplex _Left)
      { // compute csin
        return cproj(_Left);
      }

      #pragma inline=forced
      inline _Dcomplex sin(_Dcomplex _Left)
      { // compute csin
        return csin(_Left);
      }

      #pragma inline=forced
      inline _Dcomplex sinh(_Dcomplex _Left)
      { // compute csinh
        return csinh(_Left);
      }

      #pragma inline=forced
      inline _Dcomplex sqrt(_Dcomplex _Left)
      { // compute csqrt
        return csqrt(_Left);
      }

      #pragma inline=forced
      inline _Dcomplex tan(_Dcomplex _Left)
      { // compute ctan
        return ctan(_Left);
      }

      #pragma inline=forced
      inline _Dcomplex tanh(_Dcomplex _Left)
      { // compute ctanh
        return ctanh(_Left);
      }

      #pragma inline=forced
      inline double abs(_Dcomplex _Left)
      { // compute cabs
        return cabs(_Left);
      }

      #pragma inline=forced
      inline double arg(_Dcomplex _Left)
      { // compute carg
        return carg(_Left);
      }

      #pragma inline=forced
      inline double fabs(_Dcomplex _Left)
      { // compute cabs
        return cabs(_Left);
      }

      #pragma inline=forced
      inline double imag(_Dcomplex _Left)
      { // compute cimag
        return cimag(_Left);
      }

      #pragma inline=forced
      inline double real(_Dcomplex _Left)
      { // compute creal
        return creal(_Left);
      }

      // float complex OVERLOADS
      #pragma inline=forced
      inline _Fcomplex acos(_Fcomplex _Left)
      { // compute cacos
        return cacosf(_Left);
      }

      #pragma inline=forced
      inline _Fcomplex acosh(_Fcomplex _Left)
      { // compute cacosh
        return cacoshf(_Left);
      }

      #pragma inline=forced
      inline _Fcomplex asin(_Fcomplex _Left)
      { // compute casin
        return casinf(_Left);
      }

      #pragma inline=forced
      inline _Fcomplex asinh(_Fcomplex _Left)
      { // compute casinh
        return casinhf(_Left);
      }

      #pragma inline=forced
      inline _Fcomplex atan(_Fcomplex _Left)
      { // compute catan
        return catanf(_Left);
      }

      #pragma inline=forced
      inline _Fcomplex atanh(_Fcomplex _Left)
      { // compute catanh
        return catanhf(_Left);
      }

      #pragma inline=forced
      inline _Fcomplex conj(_Fcomplex _Left)
      { // compute conj
        return conjf(_Left);
      }

      #pragma inline=forced
      inline _Fcomplex cos(_Fcomplex _Left)
      { // compute ccos
        return ccosf(_Left);
      }

      #pragma inline=forced
      inline _Fcomplex cosh(_Fcomplex _Left)
      { // compute ccosh
        return ccoshf(_Left);
      }

      #pragma inline=forced
      inline _Fcomplex cproj(_Fcomplex _Left)
      { // compute cproj
        return cprojf(_Left);
      }

      #pragma inline=forced
      inline _Fcomplex exp(_Fcomplex _Left)
      { // compute cexp
        return cexpf(_Left);
      }

      #pragma inline=forced
      inline _Fcomplex log(_Fcomplex _Left)
      { // compute clog
        return clogf(_Left);
      }

      #pragma inline=forced
      inline _Fcomplex log10(_Fcomplex _Left)
      { // compute clog10
        return clog10f(_Left);
      }

      #pragma inline=forced
      inline float norm(_Fcomplex _Left)
      { // compute norm -- added with TR1
        return normf(_Left);
      }

      #pragma inline=forced
      inline _Fcomplex pow(_Fcomplex _Left, _Fcomplex _Right)
      { // compute cpow
        return cpowf(_Left, _Right);
      }

      #pragma inline=forced
      inline _Fcomplex proj(_Fcomplex _Left)
      { // compute csin
        return cprojf(_Left);
      }

      #pragma inline=forced
      inline _Fcomplex sin(_Fcomplex _Left)
      { // compute csin
        return csinf(_Left);
      }

      #pragma inline=forced
      inline _Fcomplex sinh(_Fcomplex _Left)
      { // compute csinh
        return csinhf(_Left);
      }

      #pragma inline=forced
      inline _Fcomplex sqrt(_Fcomplex _Left)
      { // compute csqrt
        return csqrtf(_Left);
      }

      #pragma inline=forced
      inline _Fcomplex tan(_Fcomplex _Left)
      { // compute ctan
        return ctanf(_Left);
      }

      #pragma inline=forced
      inline _Fcomplex tanh(_Fcomplex _Left)
      { // compute ctanh
        return ctanhf(_Left);
      }

      #pragma inline=forced
      inline float abs(_Fcomplex _Left)
      { // compute cabs
        return cabsf(_Left);
      }

      #pragma inline=forced
      inline float arg(_Fcomplex _Left)
      { // compute carg
        return cargf(_Left);
      }

      #pragma inline=forced
      inline float carg(_Fcomplex _Left)
      { // compute carg
        return cargf(_Left);
      }

      #pragma inline=forced
      inline float cimag(_Fcomplex _Left)
      { // compute cimag
        return cimagf(_Left);
      }

      #pragma inline=forced
      inline float creal(_Fcomplex _Left)
      { // compute creal
        return crealf(_Left);
      }

      #pragma inline=forced
      inline float fabs(_Fcomplex _Left)
      { // compute cabs
        return cabsf(_Left);
      }

      #pragma inline=forced
      inline float imag(_Fcomplex _Left)
      { // compute cimag
        return cimagf(_Left);
      }

      #pragma inline=forced
      inline float real(_Fcomplex _Left)
      { // compute creal
        return crealf(_Left);
      }

      // long double complex OVERLOADS
      #pragma inline=forced
      inline _Lcomplex acos(_Lcomplex _Left)
      { // compute cacos
        return cacosl(_Left);
      }

      #pragma inline=forced
      inline _Lcomplex acosh(_Lcomplex _Left)
      { // compute cacosh
        return cacoshl(_Left);
      }

      #pragma inline=forced
      inline _Lcomplex asin(_Lcomplex _Left)
      { // compute casin
        return casinl(_Left);
      }

      #pragma inline=forced
      inline _Lcomplex asinh(_Lcomplex _Left)
      { // compute casinh
        return casinhl(_Left);
      }

      #pragma inline=forced
      inline _Lcomplex atan(_Lcomplex _Left)
      { // compute catan
        return catanl(_Left);
      }

      #pragma inline=forced
      inline _Lcomplex atanh(_Lcomplex _Left)
      { // compute catanh
        return catanhl(_Left);
      }

      #pragma inline=forced
      inline _Lcomplex conj(_Lcomplex _Left)
      { // compute conj
        return conj(_Left);
      }

      #pragma inline=forced
      inline _Lcomplex cos(_Lcomplex _Left)
      { // compute ccos
        return ccosl(_Left);
      }

      #pragma inline=forced
      inline _Lcomplex cosh(_Lcomplex _Left)
      { // compute ccosh
        return ccoshl(_Left);
      }

      #pragma inline=forced
      inline _Lcomplex cproj(_Lcomplex _Left)
      { // compute cproj
        return cprojl(_Left);
      }

      #pragma inline=forced
      inline _Lcomplex exp(_Lcomplex _Left)
      { // compute cexp
        return cexpl(_Left);
      }

      #pragma inline=forced
      inline _Lcomplex log(_Lcomplex _Left)
      { // compute clog
        return clogl(_Left);
      }

      #pragma inline=forced
      inline _Lcomplex log10(_Lcomplex _Left)
      { // compute clog10
        return clog10l(_Left);
      }

      #pragma inline=forced
      inline long double norm(_Lcomplex _Left)
      { // compute norm -- added with TR1
        return norml(_Left);
      }

      #pragma inline=forced
      inline _Lcomplex pow(_Lcomplex _Left, _Lcomplex _Right)
      { // compute cpow
        return cpowl(_Left, _Right);
      }

      #pragma inline=forced
      inline _Lcomplex proj(_Lcomplex _Left)
      { // compute csin
        return cprojl(_Left);
      }

      #pragma inline=forced
      inline _Lcomplex sin(_Lcomplex _Left)
      { // compute csin
        return csinl(_Left);
      }

      #pragma inline=forced
      inline _Lcomplex sinh(_Lcomplex _Left)
      { // compute csinh
        return csinhl(_Left);
      }

      #pragma inline=forced
      inline _Lcomplex sqrt(_Lcomplex _Left)
      { // compute csqrt
        return csqrtl(_Left);
      }

      #pragma inline=forced
      inline _Lcomplex tan(_Lcomplex _Left)
      { // compute ctan
        return ctanl(_Left);
      }

      #pragma inline=forced
      inline _Lcomplex tanh(_Lcomplex _Left)
      { // compute ctanh
        return ctanhl(_Left);
      }

      #pragma inline=forced
      inline long double abs(_Lcomplex _Left)
      { // compute cabs
        return cabsl(_Left);
      }

      #pragma inline=forced
      inline long double arg(_Lcomplex _Left)
      { // compute carg
        return cargl(_Left);
      }

      #pragma inline=forced
      inline long double carg(_Lcomplex _Left)
      { // compute carg
        return cargl(_Left);
      }

      #pragma inline=forced
      inline long double cimag(_Lcomplex _Left)
      { // compute cimag
        return cimagl(_Left);
      }

      #pragma inline=forced
      inline long double creal(_Lcomplex _Left)
      { // compute creal
        return creall(_Left);
      }

      #pragma inline=forced
      inline long double fabs(_Lcomplex _Left)
      { // compute cabs
        return cabsl(_Left);
      }

      #pragma inline=forced
      inline long double imag(_Lcomplex _Left)
      { // compute cimag
        return cimagl(_Left);
      }

      #pragma inline=forced
      inline long double real(_Lcomplex _Left)
      { // compute creal
        return creall(_Left);
      }

      // GENERIC TEMPLATES
      template<class _Ty>
      struct _Promote_to_complex
      { // promote to _Dcomplex
        typedef _Dcomplex type;
      };

      template<>
      struct _Promote_to_complex<float>
      { // promote to _Fcomplex
        typedef _Fcomplex type;
      };

      template<>
      struct _Promote_to_complex<long double>
      { // promote to _Lcomplex
        typedef _Lcomplex type;
      };

      template<class _Ty>
      struct _Complex_helper
      { // attributes for non-complex
        static const bool _Is_complex = false;
        typedef typename _Promote_to_float<_Ty>::type _Float_type;
      };

      template<>
      struct _Complex_helper<_Fcomplex>
      { // attributes for _Fcomplex
        static const bool _Is_complex = true;
        typedef float _Float_type;

        static _Fcomplex _To_complex(_Float_type _Left)
        { // convert to complex
          return _FCOMPLEX_(_Left, 0);
        }

        static _Fcomplex _To_complex(_Fcomplex _Left)
        { // convert to complex
          return _Left;
        }

        static _Fcomplex _To_complex(_Dcomplex _Left)
        { // convert to complex
          return _FCOMPLEX_((float)real(_Left), (float)imag(_Left));
        }

        static _Fcomplex _To_complex(_Lcomplex _Left)
        { // convert to complex
          return _FCOMPLEX_((float)real(_Left), (float)imag(_Left));
        }
      };

      template<>
      struct _Complex_helper<_Dcomplex>
      { // attributes for _Dcomplex
        static const bool _Is_complex = true;
        typedef double _Float_type;
        
        static _Dcomplex _To_complex(_Float_type _Left)
        { // convert to complex
          return _DCOMPLEX_(_Left, 0);
        }

        static _Dcomplex _To_complex(_Fcomplex _Left)
        { // convert to complex
          return _DCOMPLEX_(real(_Left), imag(_Left));
        }

        static _Dcomplex _To_complex(_Dcomplex _Left)
        { // convert to complex
          return _Left;
        }

        static _Dcomplex _To_complex(_Lcomplex _Left)
        { // convert to complex
          return _DCOMPLEX_((double)real(_Left), (double)imag(_Left));
        }
      };

      template<>
      struct _Complex_helper<_Lcomplex>
      { // attributes for _Lcomplex
        static const bool _Is_complex = true;
        typedef long double _Float_type;
        
        static _Lcomplex _To_complex(_Float_type _Left)
        { // convert to complex
          return _LCOMPLEX_(_Left, 0);
        }
        
        static _Lcomplex _To_complex(_Fcomplex _Left)
        { // convert to complex
          return _LCOMPLEX_(real(_Left), imag(_Left));
        }

        static _Lcomplex _To_complex(_Dcomplex _Left)
        { // convert to complex
          return _LCOMPLEX_(real(_Left), imag(_Left));
        }
        
        static _Lcomplex _To_complex(_Lcomplex _Left)
        { // convert to complex
          return _Left;
        }
      };

      template<class _Ty1, class _Ty2> inline
        typename enable_if<   _Complex_helper<_Ty1>::_Is_complex
                           || _Complex_helper<_Ty2>::_Is_complex,
        typename _Promote_to_complex<
        typename _Common_float_type<
        typename _Complex_helper<_Ty1>::_Float_type,
        typename _Complex_helper<_Ty2>::_Float_type>::type>::type>::type
      pow(const _Ty1 _Left, const _Ty2 _Right)
      {     // bring mixed types to a common type
        typedef typename _Promote_to_complex<
          typename _Common_float_type<
          typename _Complex_helper<_Ty1>::_Float_type,
          typename _Complex_helper<_Ty2>::_Float_type>::type>::type type;
        return std::pow(_Complex_helper<type>::_To_complex(_Left),
                        _Complex_helper<type>::_To_complex(_Right));
      }

      #define _GENERIC_MATHC0(FUN, VAL)                                \
        template<class _Ty> inline                                     \
        typename enable_if<                                            \
        is_arithmetic<_Ty>::value,                                     \
          typename _Promote_to_float<_Ty>::type>::type                 \
        FUN(_Ty)                                                       \
        {                                                              \
          typedef typename _Promote_to_float<_Ty>::type type;          \
          return (type)VAL;                                            \
        }

      #define _GENERIC_MATHC1(FUN, VAL)                                \
        template<class _Ty> inline                                     \
        typename enable_if<                                            \
        is_arithmetic<_Ty>::value,                                     \
          typename _Promote_to_float<_Ty>::type>::type                 \
        FUN(_Ty _Left)                                                 \
        {                                                              \
          typedef typename _Promote_to_float<_Ty>::type type;          \
          return (type)VAL;                                            \
        }

      #define _GENERIC_MATHC(FUN)                                      \
        template<class _Ty> inline                                     \
        typename enable_if<                                            \
        is_arithmetic<_Ty>::value,                                     \
          typename _Promote_to_complex<                                \
          typename _Promote_to_float<_Ty>::type>::type>::type          \
        FUN(_Ty _Left)                                                 \
        {                                                              \
          typedef typename _Promote_to_complex<                        \
            typename _Promote_to_float<_Ty>::type>::type type;         \
          return FUN(_Complex_helper<type>::_To_complex(_Left));       \
        }

      _GENERIC_MATHC1(cabs,  _Left)
      _GENERIC_MATHC0(carg,  0)
      _GENERIC_MATHC0(cimag, 0)
      _GENERIC_MATHC1(creal, _Left)
      _GENERIC_MATHC (cproj)
      _GENERIC_MATHC1(norm,  (_Left * _Left))
      _GENERIC_MATHC (conj)
      _GENERIC_MATHC (proj)
    } /* extern "C++" */
  } /* namespace std */
#endif /* __cplusplus */

/* SPECIAL HANDLING FOR clog */
#ifdef __cplusplus
  namespace _iar_clog 
  {
    extern "C" {
      __EFF_NS __ATTRIBUTES _Dcomplex clog(::_Dcomplex);
    }
  } /* namespace _iar_clog */

  extern "C++" {
    __EFF_NS __ATTRIBUTES _Dcomplex log(_Dcomplex _Left);

    #pragma inline=forced
    _Dcomplex log(_Dcomplex _Left)
    {   // compute clog
      return _iar_clog::clog(_Left);
    }
  }
#else /* !__cplusplus */
  __EFF_NS __ATTRIBUTES _Dcomplex clog(_Dcomplex);
#endif /* __cplusplus */

#endif /* _COMPLEX */

/*
 * Copyright (c) by P.J. Plauger. All rights reserved.
 * Consult your license regarding permissions and restrictions.
V6.50:0576 */
