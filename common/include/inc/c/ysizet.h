/* ysizet.h internal header file. */
/* Copyright 2003-2017 IAR Systems AB.  */

#ifndef _YSIZET_H
#define _YSIZET_H

#ifndef _SYSTEM_BUILD
  #pragma system_include
#endif

#include <ycheck.h>
#include <yvals.h>

/* type definitions */
#ifndef _SIZE_T
  #define _SIZE_T
  #define _STD_USING_SIZE_T
  typedef _Sizet size_t;
#endif

#define __DATA_PTR_MEM_HELPER1__(M, I) \
typedef __DATA_MEM##I##_SIZE_TYPE__ M##_size_t;
__DATA_PTR_MEMORY_LIST1__()
#undef __DATA_PTR_MEM_HELPER1__

#endif /* _YSIZET_H */

