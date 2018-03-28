/* stdnoreturn.h standard header */
/* Copyright 2014-2017 IAR Systems AB. */
#ifndef _STDNORETURN
#define _STDNORETURN

#ifndef _SYSTEM_BUILD
  #pragma system_include
#endif

#include <ycheck.h>
#include <yvals.h>

#ifndef __cplusplus
  #define noreturn _Noreturn
#endif

#endif /* _STDNORETURN */

/*
 * Copyright (c) by P.J. Plauger. All rights reserved.
 * Consult your license regarding permissions and restrictions.
V6.50:0576 */
