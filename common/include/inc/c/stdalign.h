/* stdalign.h standard header */
/* Copyright 2014-2017 IAR Systems AB. */
#ifndef _STDALIGN
#define _STDALIGN

#ifndef _SYSTEM_BUILD
  #pragma system_include
#endif

#include <ycheck.h>
#include <yvals.h>

#if _DLIB_ONLY_C89
  #error "Header is not supported in the C89 language mode"
#endif

#define __alignas_is_defined    1
#define __alignof_is_defined    1

#ifndef __cplusplus
  #define alignas               _Alignas
  #define alignof               _Alignof
#endif /* __cplusplus */

#endif /* _STDALIGN */

/*
 * Copyright (c) by P.J. Plauger. All rights reserved.
 * Consult your license regarding permissions and restrictions.
V6.50:0576 */
