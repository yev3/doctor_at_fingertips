/* DLib float setup header for math functionality
   Copyright 2015-2017 IAR Systems AB.
*/
#ifndef _DLIB_FLOAT_SETUP
#define _DLIB_FLOAT_SETUP

#ifndef _SYSTEM_BUILD
  #pragma system_include
#endif

/*
  Possible Overrides
  (All in a packet must be overrided or not.)
*/

/* Overrides on the trigonometrics (sin, cos, and tan) defaults */
/* Default */
   #define _DLIB_trigonometric_IS_DEFAULT
/* or accurate
   #define _DLIB_trigonometric_accurate_IS_DEFAULT
   or small
   #define _DLIB_trigonometric_small_IS_DEFAULT
*/

/* Overrides on the exp-pow defaults */
/* Default */
   #define _DLIB_exp_pow_IS_DEFAULT
/* or accurate
   #define _DLIB_exp_pow_accurate_IS_DEFAULT
   or small
   #define _DLIB_exp_pow_small_IS_DEFAULT
*/

/* Overrides on the logx defaults */
/* Default */
   #define _DLIB_log_IS_DEFAULT
/* or small
   #define _DLIB_log_small_IS_DEFAULT
*/

/* Low-level math types
   (Used to contain flag-bits or small numbers.)
*/
typedef __UINT32_T_TYPE__ __iar_FlagUType;
typedef __INT32_T_TYPE__  __iar_FlagSType;

typedef __INT32_T_TYPE__  __iar_ExpType;


/* Low-level math functions
   (remove 64-bit versions if sizeof(float) == sizeof(double).)
*/

#define _FLT_MAX_EXP 0xFF
#define _FLT_BIAS    0x7E
#define _FLT_OFF     23

#define _DBL_MAX_EXP 0x7FF
#define _DBL_BIAS    0x3FE
#define _DBL_OFF     52

/*
   Return 0 if x is INF or NaN, !0 if subnormal, normal, or 0.
   It may return any integer type.
*/
#pragma no_arith_checks
#pragma inline=forced
__intrinsic __UINT32_T_TYPE__ __iar_isfinite32(float x)
{
  __INT32_T_TYPE__ ix = __iar_fp2bits32(x);
  return ((ix << 1) >> (_FLT_OFF + 1)) + 1;
}

#pragma no_arith_checks
#pragma inline=forced
__intrinsic __UINT32_T_TYPE__ __iar_isfinite64(double x)
{
  __INT32_T_TYPE__ ix = __iar_fpgethi64(x);
  return ((ix << 1) >> (_DBL_OFF - 31)) + 1;
}
/*
   Return !0 if NaN, else 0.
*/
#pragma no_arith_checks
#pragma inline=forced
__intrinsic __UINT32_T_TYPE__ __iar_isnan32(float x)
{
  __INT32_T_TYPE__ ix = __iar_fp2bits32(x) << 1;
  return (ix >> (_FLT_OFF + 1)) + 1 ? 0 : (ix << (31 - _FLT_OFF));
}

#pragma no_arith_checks
#pragma inline=forced
__intrinsic __UINT32_T_TYPE__ __iar_isnan64(double x)
{
  __INT32_T_TYPE__ ix = __iar_fpgethi64(x);
  return ((ix << 1) >> (_DBL_OFF - 31)) + 1 ? 0 : ix << (64 - _DBL_OFF);
}

/*
   Return !0 if +/-0, else 0. Do not need to check for NaN.
   It may return any integer type.
*/
#pragma no_arith_checks
#pragma inline=forced
__intrinsic __UINT32_T_TYPE__ __iar_iszero32(float x)
{
  __UINT32_T_TYPE__ ix = __iar_fp2bits32(x);
  return (ix & ~(1 << 31)) == 0;
}

#pragma no_arith_checks
#pragma inline=forced
__intrinsic __UINT32_T_TYPE__ __iar_iszero64(double x)
{
  __UINT64_T_TYPE__ ix = __iar_fp2bits64(x);
  return (ix & ~(1ULL << 63)) == 0;
}


/*
   Return !0 if INF, else 0. Do not need to check for NaN.
   It may return any integer type.
*/
#pragma no_arith_checks
#pragma inline=forced
__intrinsic __UINT32_T_TYPE__ __iar_isinf32(float x)
{
  __INT32_T_TYPE__ ix = __iar_fp2bits32(x);
  return ((ix << 1) >> (_FLT_OFF + 1)) + 1 ? 0 : ((ix << (32 - _FLT_OFF)) == 0);
}

#pragma no_arith_checks
#pragma inline=forced
__intrinsic __UINT32_T_TYPE__ __iar_isinf64(double x)
{
  __INT32_T_TYPE__ ix = __iar_fpgethi64(x);
  return ((ix << 1) >> (_DBL_OFF - 31)) + 1 ? 0 : ((ix << (64 - _DBL_OFF)) == 0);
}


/*
  Return !0 if subnormal, else 0. Do not need to check for NaN.
   It may return any integer type.
*/
#pragma no_arith_checks
#pragma inline=forced
__intrinsic __UINT32_T_TYPE__ __iar_issubnormal32(float x)
{
#if __SUBNORMAL_FLOATING_POINTS__
  __UINT32_T_TYPE__ ix = __iar_fp2bits32(x) & ~(1 << 31);
  return (ix != 0) && (ix < (1 << _FLT_OFF));
#else
  return 0;
#endif
}

#pragma no_arith_checks
#pragma inline=forced
__intrinsic __UINT32_T_TYPE__ __iar_issubnormal64(double x)
{
#if __SUBNORMAL_FLOATING_POINTS__
  __UINT64_T_TYPE__ ix = __iar_fp2bits64(x) & ~(1ULL << 63);
  return (ix != 0) && (ix < (1ULL << _DBL_OFF));
#else
  return 0;
#endif
}

/*
   Return !0 if normal, else 0. Do not need to check for NaN.
   It may return any integer type.
*/
#pragma no_arith_checks
#pragma inline=forced
__intrinsic __UINT32_T_TYPE__ __iar_isnormal32(float x)
{
  __INT32_T_TYPE__ exp = ((__INT32_T_TYPE__)(__iar_fp2bits32(x) << 1) >> (_FLT_OFF + 1));
  return ((exp + 1) >> 1);
}

#pragma no_arith_checks
#pragma inline=forced
__intrinsic __UINT32_T_TYPE__ __iar_isnormal64(double x)
{
  __INT32_T_TYPE__ exp = ((__INT32_T_TYPE__)(__iar_fpgethi64(x) << 1) >> (_DBL_OFF - 31));
  return ((exp + 1) >> 1);
}


/*
   Return the signbit (without NaN-check).
 */
#pragma no_arith_checks
#pragma inline=forced
__intrinsic __UINT32_T_TYPE__ __iar_signbit32(float x)
{
  __UINT32_T_TYPE__ ix = __iar_fp2bits32(x);
  return (ix >> 31);
}

#pragma no_arith_checks
#pragma inline=forced
__intrinsic __UINT32_T_TYPE__ __iar_signbit64(double x)
{
  __UINT64_T_TYPE__ ix = __iar_fp2bits64(x);
  return (ix >> 63);
}


#endif /* _DLIB_FLOAT_SETUP */
