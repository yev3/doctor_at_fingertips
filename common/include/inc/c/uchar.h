/* uchar.h added header for TR 19769 */
/* Copyright 2009-2017 IAR Systems AB.  */
#ifndef _UCHAR
#define _UCHAR

#ifndef _SYSTEM_BUILD
  #pragma system_include
#endif

#include <ycheck.h>
#include <yvals.h>
#include <ysizet.h>

                /* macros */
#ifndef NULL
  #define NULL  _NULL
#endif

/* Values of char16_t are UTF-16 encoded, __STDC_UTF_16__ is predefined */
/* Values of char32_t are UTF-32 encoded, __STDC_UTF_32__ is predefined */

/* TYPE DEFINITIONS */
#ifndef MBSTATE_T_DEFINED
  #define MBSTATE_T_DEFINED
  typedef _Mbstatet mbstate_t;
#endif
#ifndef _CHAR16T
  #define _CHAR16T
  #ifndef __cplusplus
    typedef __UINT16_T_TYPE__  char16_t;
    typedef __UINT32_T_TYPE__  char32_t;
  #endif
#endif

_EXTERN_C
  /* declarations */
  __ATTRIBUTES size_t mbrtoc16(char16_t *_Restrict, const char *_Restrict,
                               size_t, mbstate_t *_Restrict);
  __ATTRIBUTES size_t c16rtomb(char *_Restrict, char16_t,
                               mbstate_t *_Restrict);

  __ATTRIBUTES size_t mbrtoc32(char32_t *_Restrict, const char *_Restrict,
                               size_t, mbstate_t *_Restrict);
  __ATTRIBUTES size_t c32rtomb(char *_Restrict, char32_t,
                               mbstate_t *_Restrict);

  __ATTRIBUTES size_t __utf8rtoc16(char16_t *_Restrict, const char *_Restrict,
                                   size_t, mbstate_t *_Restrict);
  __ATTRIBUTES size_t __c16rtoutf8(char *_Restrict, char16_t,
                                   mbstate_t *_Restrict);

  __ATTRIBUTES size_t __utf8rtoc32(char32_t *_Restrict, const char *_Restrict,
                                   size_t, mbstate_t *_Restrict);
  __ATTRIBUTES size_t __c32rtoutf8(char *_Restrict, char32_t,
                                   mbstate_t *_Restrict);
_EXTERN_C_END

#endif /* _UCHAR */

/*
 * Copyright (c) by P.J. Plauger. All rights reserved.
 * Consult your license regarding permissions and restrictions.
V6.50:0576 */
