/**************************************************
 *
 * ARM-specific configuration for string.h in DLib.
 *
 * Copyright 2006-2017 IAR Systems AB.
 *
 * $Id: DLib_Product_string.h 112610 2017-02-09 08:49:48Z danielfl $
 *
 **************************************************/

#ifndef _DLIB_PRODUCTS_STRING_H_
#define _DLIB_PRODUCTS_STRING_H_

#ifndef _SYSTEM_BUILD
  #pragma system_include
#endif

#ifndef _NO_DEFINITIONS_IN_HEADER_FILES

  /*
   * The following is pre-declared by the compiler.
   *
   * __INTRINSIC void __aeabi_memset (void *, size_t, int);
   * __INTRINSIC void __aeabi_memcpy (void *, const void *, size_t);
   * __INTRINSIC void __aeabi_memmove(void *, const void *, size_t);
   */


  /*
   * Inhibit inline definitions for routines with an effective
   * ARM-specific implementation.
   *
   * Not in Cortex-M1 and Cortex-MS1
   */

  #if  __CORE__ != 11 && __CORE__ != 12

  #define _DLIB_STRING_SKIP_INLINE_MEMCHR
  #define _DLIB_STRING_SKIP_INLINE_MEMCMP
  #define _DLIB_STRING_SKIP_INLINE_STRCHR
  #define _DLIB_STRING_SKIP_INLINE_STRCMP
  #define _DLIB_STRING_SKIP_INLINE_STRLEN
  #define _DLIB_STRING_SKIP_INLINE_STRNCPY

  #endif

  /*
   * Redirect calls to standard functions to the corresponding
   * __aeabi_X function.
   */
_EXTERN_C

  #define _DLIB_STRING_SKIP_INLINE_MEMCPY
  #pragma inline=forced_no_body
  __EFF_NENR1NW2R1 __ATTRIBUTES void * memcpy(void * _D, const void * _S, size_t _N)
  {
    __aeabi_memcpy(_D, _S, _N);
    return _D;
  }

  #define _DLIB_STRING_SKIP_INLINE_MEMMOVE
  #pragma inline=forced_no_body
  __EFF_NENR1NW2R1 __ATTRIBUTES void * memmove(void * _D, const void * _S, size_t _N)
  {
    __aeabi_memmove(_D, _S, _N);
    return _D;
  }

  #define _DLIB_STRING_SKIP_INLINE_MEMSET
  #pragma inline=forced_no_body
  __EFF_NENR1R1 __ATTRIBUTES void * memset(void * _D, int _C, size_t _N)
  {
    __aeabi_memset(_D, _N, _C);
    return _D;
  }

_EXTERN_C_END
#endif /* _NO_DEFINITIONS_IN_HEADER_FILES */

#endif /* _DLIB_PRODUCTS_STRING_H_ */
