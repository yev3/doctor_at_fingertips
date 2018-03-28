/* string.h standard header */
/* Copyright 2009-2017 IAR Systems AB. */
#ifndef _STRING
#define _STRING

#ifndef _SYSTEM_BUILD
  #pragma system_include
#endif

#include <ycheck.h>
#include <yvals.h>
#include <ysizet.h>

#if _DLIB_PRODUCT_STRING
  #include <DLib_Product_string.h>
#endif

/* macros */
#ifndef NULL
  #define NULL  _NULL
#endif

/* type definitions */
#if __STDC_WANT_LIB_EXT1__ == 1
  #if !defined(_ERRNO_T)
    #define _ERRNO_T
    typedef int errno_t;
  #endif

  #if !defined(_RSIZE_T)
    #define _RSIZE_T
    typedef size_t rsize_t;
  #endif
#endif

/* declarations */
_EXTERN_C
  __EFF_NENW1NW2   __ATTRIBUTES   int       memcmp(const void *, const void *,
                                                   size_t);
  __EFF_NENR1NW2R1 __DEPREC_ATTRS void *    memcpy(void *_Restrict, 
                                                   const void *_Restrict,
                                                   size_t);
  __EFF_NENR1NW2R1 __DEPREC_ATTRS void *    memmove(void *, const void *,
                                                    size_t);
  __EFF_NENR1R1    __DEPREC_ATTRS void *    memset(void *, int, size_t);
  __EFF_NENW2R1    __DEPREC_ATTRS char *    strcat(char *_Restrict, 
                                                   const char *_Restrict);
  __EFF_NENW1NW2   __ATTRIBUTES   int       strcmp(const char *, const char *);
  __EFF_NW1NW2     __ATTRIBUTES   int       strcoll(const char *, const char *);
  __EFF_NENR1NW2R1 __DEPREC_ATTRS char *    strcpy(char *_Restrict, 
                                                   const char *_Restrict);
  __EFF_NENW1NW2   __ATTRIBUTES   size_t    strcspn(const char *, const char *);
                   __DEPREC_ATTRS char *    strerror(int);
  __EFF_NENW1      __ATTRIBUTES   size_t    strlen(const char *);
  __EFF_NENW2R1    __DEPREC_ATTRS char *    strncat(char *_Restrict, 
                                                    const char *_Restrict,
                                                    size_t);
  __EFF_NENW1NW2   __ATTRIBUTES   int       strncmp(const char *, const char *, 
                                                    size_t);
  __EFF_NENR1NW2R1 __DEPREC_ATTRS char *    strncpy(char *_Restrict, 
                                                    const char *_Restrict,
                                                    size_t);
  __EFF_NENW1NW2   __ATTRIBUTES   size_t    strspn(const char *, const char *);
  __EFF_NW2        __DEPREC_ATTRS char *    strtok(char *_Restrict, 
                                                   const char *_Restrict);
  __EFF_NW2        __ATTRIBUTES   size_t    strxfrm(char *_Restrict, 
                                                    const char *_Restrict,
                                                    size_t);
  #if _DLIB_ADD_EXTRA_SYMBOLS
    __EFF_NW1      __ATTRIBUTES   char *    strdup(const char *);
    __EFF_NW1NW2   __ATTRIBUTES   int       strcasecmp(const char *,
                                                       const char *);
    __EFF_NW1NW2   __ATTRIBUTES   int       strncasecmp(const char *,
                                                        const char *, size_t);
    __EFF_NENW2    __ATTRIBUTES   char *    strtok_r(char *, const char *,
                                                     char **);
    __EFF_NENW1    __DEPREC_ATTRS size_t    strnlen(char const *, size_t);
  #endif
_EXTERN_C_END

#ifdef __cplusplus
  _EXTERN_C
    __EFF_NENW1    __ATTRIBUTES void *      __iar_Memchr(const void *, int,
                                                         size_t);
    __EFF_NENW1    __ATTRIBUTES char *      __iar_Strchr(const char *, int);
    __EFF_NENW1NW2 __ATTRIBUTES char *      __iar_Strpbrk(const char *,
                                                          const char *);
    __EFF_NENW1    __ATTRIBUTES char *      __iar_Strrchr(const char *, int);
    __EFF_NENW1NW2 __ATTRIBUTES char *      __iar_Strstr(const char *,
                                                         const char *);
  _EXTERN_C_END
  extern "C++" {
    __EFF_NENW1    __ATTRIBUTES const void *memchr(const void *_S, int _C, 
                                                   size_t _N);
    __EFF_NENW1    __ATTRIBUTES const char *strchr(const char *_S, int _C);
    __EFF_NENW1NW2 __ATTRIBUTES const char *strpbrk(const char *_S, 
                                                    const char *_P);
    __EFF_NENW1    __ATTRIBUTES const char *strrchr(const char *_S, int _C);
    __EFF_NENW1NW2 __ATTRIBUTES const char *strstr(const char *_S, 
                                                   const char *_P);
    __EFF_NENW1    __ATTRIBUTES void *      memchr(void *_S, int _C, size_t _N);
    __EFF_NENW1    __ATTRIBUTES char *      strchr(char *_S, int _C);
    __EFF_NENW1NW2 __ATTRIBUTES char *      strpbrk(char *_S, const char *_P);
    __EFF_NENW1    __ATTRIBUTES char *      strrchr(char *_S, int _C);
    __EFF_NENW1NW2 __ATTRIBUTES char *      strstr(char *_S, const char *_P);

    /* inlines and overloads, for C and C++ */
    /* First the const overloads for C++. */
    #pragma inline=forced
    const void *memchr(const void *_S, int _C, size_t _N)
    {
      return __iar_Memchr(_S, _C, _N);
    }

    #pragma inline=forced
    const char *strchr(const char *_S, int _C)
    {
      return __iar_Strchr(_S, _C);
    }

    #pragma inline=forced
    const char *strpbrk(const char *_S, const char *_P)
    {
      return __iar_Strpbrk(_S, _P);
    }

    #pragma inline=forced
    const char *strrchr(const char *_S, int _C)
    {
      return __iar_Strrchr(_S, _C);
    }

    #pragma inline=forced
    const char *strstr(const char *_S, const char *_P)
    {
      return __iar_Strstr(_S, _P);
    }

    /* Then the non-const overloads for C++. */
    #pragma inline=forced
    void *memchr(void *_S, int _C, size_t _N)
    {
      return __iar_Memchr(_S, _C, _N);
    }

    #pragma inline=forced
    char *strchr(char *_S, int _C)
    {
      return __iar_Strchr(_S, _C);
    }

    #pragma inline=forced
    char *strpbrk(char *_S, const char *_P)
    {
      return __iar_Strpbrk(_S, _P);
    }

    #pragma inline=forced
    char *strrchr(char *_S, int _C)
    {
      return __iar_Strrchr(_S, _C);
    }

    #pragma inline=forced
    char *strstr(char *_S, const char *_P)
    {
      return __iar_Strstr(_S, _P);
    }
  } /* extern "C++" */
#else /* !__cplusplus */
  __EFF_NENW1    __ATTRIBUTES void *memchr(const void *_S, int _C, size_t _N);
  __EFF_NENW1    __ATTRIBUTES char *strchr(const char *_S, int _C);
  __EFF_NENW1NW2 __ATTRIBUTES char *strpbrk(const char *_S, const char *_P);
  __EFF_NENW1    __ATTRIBUTES char *strrchr(const char *_S, int _C);
  __EFF_NENW1NW2 __ATTRIBUTES char *strstr(const char *_S, const char *_P);
#endif /* __cplusplus */

#if __STDC_WANT_LIB_EXT1__ == 1
  _EXTERN_C
    __ATTRIBUTES errno_t   memcpy_s(void *_Restrict, rsize_t,
                                    const void *_Restrict, rsize_t);
    __ATTRIBUTES errno_t   memmove_s(void *, rsize_t,
                                     const void *, rsize_t);
    __ATTRIBUTES errno_t   memset_s(void *, rsize_t, int, rsize_t);
    __ATTRIBUTES errno_t   strcpy_s(char *_Restrict, rsize_t,
                                    const char *_Restrict);
    __ATTRIBUTES errno_t   strncpy_s(char *_Restrict, rsize_t,
                                     const char *_Restrict, rsize_t);
    __ATTRIBUTES errno_t   strcat_s(char *_Restrict, rsize_t,
                                    const char *_Restrict);
    __ATTRIBUTES errno_t   strncat_s(char *_Restrict, rsize_t,
                                     const char *_Restrict, rsize_t);
    __ATTRIBUTES char *    strtok_s(char *_Restrict, rsize_t *_Restrict,
                                    const char *_Restrict, char **_Restrict);
    __ATTRIBUTES errno_t   strerror_s(char *, rsize_t, errno_t);
    __ATTRIBUTES size_t    strerrorlen_s(errno_t);
    __ATTRIBUTES size_t    strnlen_s(const char *_Restrict, size_t);
  _EXTERN_C_END
#endif /* __STDC_WANT_LIB_EXT1__ */

#endif /* _STRING */

/*
 * Copyright (c) by P.J. Plauger. All rights reserved.
 * Consult your license regarding permissions and restrictions.
V6.50:0576 */
