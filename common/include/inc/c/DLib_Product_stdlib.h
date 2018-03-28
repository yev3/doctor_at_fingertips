/**************************************************
 *
 * ARM-specific configuration for stdlib.h in DLib.
 *
 * Copyright 2017 IAR Systems AB.
 *
 **************************************************/

#ifndef _DLIB_PRODUCTS_STDLIB_H_
#define _DLIB_PRODUCTS_STDLIB_H_

#ifndef _SYSTEM_BUILD
  #pragma system_include
#endif

#ifndef _NO_DEFINITIONS_IN_HEADER_FILES

#ifdef __cplusplus
extern "C" {
#endif  

#pragma type_attribute=__value_in_regs
div_t __aeabi_idivmod(int n, int d);

#if !_DLIB_ONLY_C89
  #pragma type_attribute=__value_in_regs
  lldiv_t __aeabi_ldivmod(long long n, long long d);
#endif  

#pragma inline=forced_no_body
__ATTRIBUTES
div_t div(int a, int b)
{
#if __ARM_FEATURE_IDIV  
  int q = a / b;
  int r = a - (q*b);
  div_t d;
  d.quot = q;
  d.rem = r;
  return d;
#else
  return __aeabi_idivmod(a, b);
#endif    
}

#pragma inline=forced_no_body
__ATTRIBUTES
ldiv_t ldiv(long a, long b)
{
#if __ARM_FEATURE_IDIV  
  int q = a / b;
  int r = a - (q*b);
  ldiv_t ld;
  ld.quot = q;
  ld.rem = r;
#else
  div_t d = __aeabi_idivmod(a, b);
  ldiv_t ld;
  ld.quot = d.quot;
  ld.rem  = d.rem;
#endif  
  return ld;
}

#if !_DLIB_ONLY_C89
  #pragma inline=forced_no_body
  __ATTRIBUTES
  lldiv_t lldiv(long long a, long long b)
  {
    return __aeabi_ldivmod(a, b);
  }
#endif  

#ifdef __cplusplus
};
#endif  

#endif /* _NO_DEFINITIONS_IN_HEADER_FILES */

#endif /* _DLIB_PRODUCTS_STDLIB_H_ */
