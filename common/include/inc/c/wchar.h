/* wchar.h standard header */
/* Copyright 2003-2017 IAR Systems AB.  */
#ifndef _WCHAR
#define _WCHAR

#ifndef _SYSTEM_BUILD
  #pragma system_include
#endif

#include <ycheck.h>
#include <yvals.h>
#include <ysizet.h>

/* Consistency check */
#if !_DLIB_WIDE_CHARACTERS
  #error "This library configuration does not support wide characters."
#endif

/* Module consistency. */
#pragma rtmodel="__dlib_file_descriptor",_STRINGIFY(_DLIB_FILE_DESCRIPTOR)

/* Support for portable C++ object model. */
#if __AEABI_PORTABILITY_INTERNAL_LEVEL
  #ifndef __AEABI_PORTABLE
    #define __AEABI_PORTABLE
  #endif
#endif


/* MACROS */
#ifndef NULL
  #define NULL  _NULL
#endif

#define WCHAR_MIN       _WCMIN
#define WCHAR_MAX       _WCMAX
#define WEOF            ((_GLB wint_t)(-1))

#if !defined(__NO_WCHAR__) && _WCMAX < __UNSIGNED_SHORT_MAX__
  #error "<wchart.h> wchar_t is too small."
#endif

/* TYPE DEFINITIONS */
#ifndef MBSTATE_T_DEFINED
  #define MBSTATE_T_DEFINED
  typedef _Mbstatet mbstate_t;
#endif
struct tm;

#ifndef _WCTYPET
  #define _WCTYPET
  #if _DLIB_SUPPORT_FOR_AEABI && !defined(__AS_BOUNDS__)
    typedef void * wctype_t;
  #else
    typedef _Sizet wctype_t;
  #endif
#endif

#ifndef _WCHART
  #define _WCHART
  typedef _Wchart wchar_t;
#endif

#ifndef _WINTT
  #define _WINTT
  typedef _Wintt wint_t;
#endif

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

_EXTERN_C
  /* stdio DECLARATIONS */
  #ifndef _DLIB_CONFIG_AGNOSTIC
    #if _DLIB_FILE_DESCRIPTOR
      __ATTRIBUTES   wint_t    fgetwc(_Filet *);
      __ATTRIBUTES   wchar_t * fgetws(wchar_t *_Restrict, int,
                                      _Filet *_Restrict);
      __ATTRIBUTES   wint_t    fputwc(wchar_t, _Filet *);
      __ATTRIBUTES   int       fputws(const wchar_t *_Restrict,
                                      _Filet *_Restrict);
      __ATTRIBUTES   int       fwide(_Filet *, int);
      __DEPREC_ATTRS int       fwprintf(_Filet *_Restrict,
                                        const wchar_t *_Restrict, ...);
      __DEPREC_ATTRS int       fwscanf(_Filet *_Restrict,
                                       const wchar_t *_Restrict, ...);
      __ATTRIBUTES   wint_t    getwc(_Filet *);
      __ATTRIBUTES   wint_t    putwc(wchar_t, _Filet *);
      __ATTRIBUTES   wint_t    ungetwc(wint_t, _Filet *);
      __DEPREC_ATTRS int       vfwprintf(_Filet *_Restrict,
                                         const wchar_t *_Restrict, __Va_list);
      #if !_DLIB_ONLY_C89
        __DEPREC_ATTRS int     vfwscanf(_Filet *_Restrict,
                                        const wchar_t *_Restrict, __Va_list);
      #endif
    #endif /* _DLIB_FILE_DESCRIPTOR */
  #endif /* _DLIB_CONFIG_AGNOSTIC */

  __ATTRIBUTES   wint_t        getwchar(void);
  __ATTRIBUTES   wint_t        __ungetwchar(wint_t);
  __ATTRIBUTES   wint_t        putwchar(wchar_t);
  __DEPREC_ATTRS int           swprintf(wchar_t *_Restrict, size_t, 
                                        const wchar_t *_Restrict, ...);
  __DEPREC_ATTRS int           swscanf(const wchar_t *_Restrict,
                                       const wchar_t *_Restrict, ...);
  __DEPREC_ATTRS int           vswprintf(wchar_t *_Restrict, size_t,
                                         const wchar_t *_Restrict, __Va_list);
  __DEPREC_ATTRS int           vwprintf(const wchar_t *_Restrict, __Va_list);
  #if !_DLIB_ONLY_C89
    __DEPREC_ATTRS int         vswscanf(const wchar_t *_Restrict,
                                        const wchar_t *_Restrict, __Va_list);
    __DEPREC_ATTRS int         vwscanf(const wchar_t *_Restrict, __Va_list);
  #endif
  __DEPREC_ATTRS int           wprintf(const wchar_t *_Restrict, ...);
  __DEPREC_ATTRS int           wscanf(const wchar_t *_Restrict, ...);

  /* stdlib DECLARATIONS */
  __ATTRIBUTES   size_t        mbrlen(const char *_Restrict, size_t,
                                      mbstate_t *_Restrict);
  __ATTRIBUTES   size_t        mbrtowc(wchar_t *_Restrict, const char *, size_t,
                                       mbstate_t *_Restrict);
  __DEPREC_ATTRS size_t        mbsrtowcs(wchar_t *_Restrict,
                                         const char **_Restrict,
                                         size_t, mbstate_t *_Restrict);
  __ATTRIBUTES   int           mbsinit(const mbstate_t *);
  __DEPREC_ATTRS size_t        wcrtomb(char *_Restrict, wchar_t,
                                       mbstate_t *_Restrict);
  __DEPREC_ATTRS size_t        wcsrtombs(char *_Restrict,
                                         const wchar_t **_Restrict,
                                         size_t, mbstate_t *_Restrict);
  __ATTRIBUTES   long          wcstol(const wchar_t *_Restrict,
                                      wchar_t **_Restrict, int);
  __ATTRIBUTES   unsigned long wcstoul(const wchar_t *_Restrict,
                                       wchar_t **_Restrict, int);

  #if !_DLIB_ONLY_C89
    #pragma language=save
    #pragma language=extended
    __ATTRIBUTES long long     wcstoll(const wchar_t *_Restrict, 
                                       wchar_t **_Restrict, int);
    __ATTRIBUTES unsigned long long wcstoull(const wchar_t *_Restrict, 
                                             wchar_t **_Restrict, int);
    #pragma language=restore
  #endif

  /* string DECLARATIONS */
  __DEPREC_ATTRS wchar_t *     wcscat(wchar_t *_Restrict,
                                      const wchar_t *_Restrict);
  __ATTRIBUTES   int           wcscmp(const wchar_t *, const wchar_t *);
  __ATTRIBUTES   int           wcscoll(const wchar_t *, const wchar_t *);
  __DEPREC_ATTRS wchar_t *     wcscpy(wchar_t *_Restrict,
                                      const wchar_t *_Restrict);
  __ATTRIBUTES   size_t        wcscspn(const wchar_t *, const wchar_t *);
  __ATTRIBUTES   size_t        wcslen(const wchar_t *);
  __DEPREC_ATTRS wchar_t *     wcsncat(wchar_t *_Restrict,
                                       const wchar_t *_Restrict, size_t);
  __ATTRIBUTES   int           wcsncmp(const wchar_t *, const wchar_t *,
                                       size_t);
  __DEPREC_ATTRS wchar_t *     wcsncpy(wchar_t *_Restrict,
                                       const wchar_t *_Restrict, size_t);
  __ATTRIBUTES   size_t        wcsspn(const wchar_t *, const wchar_t *);
  __DEPREC_ATTRS wchar_t *     wcstok(wchar_t *_Restrict,
                                      const wchar_t *_Restrict,
                                      wchar_t **_Restrict);
  __ATTRIBUTES   size_t        wcsxfrm(wchar_t *_Restrict,
                                       const wchar_t *_Restrict, size_t);
  __ATTRIBUTES   int           wmemcmp(const wchar_t *, const wchar_t *,
                                       size_t);
  __DEPREC_ATTRS wchar_t *     wmemcpy(wchar_t *_Restrict,
                                       const wchar_t *_Restrict, size_t);
  __DEPREC_ATTRS wchar_t *     wmemmove(wchar_t *, const wchar_t *, size_t);
  __ATTRIBUTES   wchar_t *     wmemset(wchar_t *, wchar_t, size_t);

  /* time DECLARATIONS */
  __ATTRIBUTES size_t          wcsftime(wchar_t *_Restrict, size_t,
                                        const wchar_t *_Restrict, 
                                        const struct tm *_Restrict);


  __ATTRIBUTES wint_t          btowc(int);
  #if !_DLIB_ONLY_C89
    __ATTRIBUTES float         wcstof(const wchar_t *_Restrict,
                                      wchar_t **_Restrict);
    __ATTRIBUTES long double   wcstold(const wchar_t *_Restrict,
                                       wchar_t **_Restrict);
  #endif
  __ATTRIBUTES double          wcstod(const wchar_t *_Restrict,
                                      wchar_t **_Restrict);
  __ATTRIBUTES int             wctob(wint_t);
_EXTERN_C_END

#ifdef __cplusplus
  _EXTERN_C
    __ATTRIBUTES wchar_t *     __iar_Wmemchr(const wchar_t *, wchar_t, size_t);
    __ATTRIBUTES wchar_t *     __iar_Wcschr(const wchar_t *, wchar_t);
    __ATTRIBUTES wchar_t *     __iar_Wcspbrk(const wchar_t *, const wchar_t *);
    __ATTRIBUTES wchar_t *     __iar_Wcsrchr(const wchar_t *, wchar_t);
    __ATTRIBUTES wchar_t *     __iar_Wcsstr(const wchar_t *, const wchar_t *);
  _EXTERN_C_END

  extern "C++" {
    __ATTRIBUTES const wchar_t * wmemchdr(const wchar_t *, wchar_t, size_t);
    __ATTRIBUTES const wchar_t * wcschr(const wchar_t *, wchar_t);
    __ATTRIBUTES const wchar_t * wcspbrk(const wchar_t *, const wchar_t *);
    __ATTRIBUTES const wchar_t * wcsrchr(const wchar_t *, wchar_t);
    __ATTRIBUTES const wchar_t * wcsstr(const wchar_t *, const wchar_t *);
    __ATTRIBUTES wchar_t *       wmemchr(wchar_t *, wchar_t, size_t);
    __ATTRIBUTES wchar_t *       wcschr(wchar_t *, wchar_t);
    __ATTRIBUTES wchar_t *       wcspbrk(wchar_t *, const wchar_t *);
    __ATTRIBUTES wchar_t *       wcsrchr(wchar_t *, wchar_t);
    __ATTRIBUTES wchar_t *       wcsstr(wchar_t *, const wchar_t *);

    /* INLINES AND OVERLOADS, FOR C++ */
    #pragma inline=forced
    const wchar_t * wmemchr(const wchar_t *_S, wchar_t _C, size_t _N)
    {
      return __iar_Wmemchr(_S, _C, _N);
    }

    #pragma inline=forced
    const wchar_t * wcschr(const wchar_t *_S, wchar_t _C)
    {
      return __iar_Wcschr(_S, _C);
    }

    #pragma inline=forced
    const wchar_t * wcspbrk(const wchar_t *_S, const wchar_t *_P)
    {
      return __iar_Wcspbrk(_S, _P);
    }

    #pragma inline=forced
    const wchar_t * wcsrchr(const wchar_t *_S, wchar_t _C)
    {
      return __iar_Wcsrchr(_S, _C);
    }

    #pragma inline=forced
    const wchar_t * wcsstr(const wchar_t *_S, const wchar_t *_P)
    {
      return __iar_Wcsstr(_S, _P);
    }

    #pragma inline=forced
    wchar_t * wmemchr(wchar_t *_S, wchar_t _C, size_t _N)
    {
      return __iar_Wmemchr(_S, _C, _N);
    }

    #pragma inline=forced
    wchar_t * wcschr(wchar_t *_S, wchar_t _C)
    {
      return __iar_Wcschr(_S, _C);
    }

    #pragma inline=forced
    wchar_t * wcspbrk(wchar_t *_S, const wchar_t *_P)
    {
      return __iar_Wcspbrk(_S, _P);
    }

    #pragma inline=forced
    wchar_t * wcsrchr(wchar_t *_S, wchar_t _C)
    {
      return __iar_Wcsrchr(_S, _C);
    }

    #pragma inline=forced
    wchar_t * wcsstr(wchar_t *_S, const wchar_t *_P)
    {
      return __iar_Wcsstr(_S, _P);
    }
  } /* EXTERN "c++" */
#else /* !__cplusplus */
  __ATTRIBUTES wchar_t * wmemchr(const wchar_t *, wchar_t, size_t);
  __ATTRIBUTES wchar_t * wcschr(const wchar_t *, wchar_t);
  __ATTRIBUTES wchar_t * wcspbrk(const wchar_t *, const wchar_t *);
  __ATTRIBUTES wchar_t * wcsrchr(const wchar_t *, wchar_t);
  __ATTRIBUTES wchar_t * wcsstr(const wchar_t *, const wchar_t *);
#endif /* __cplusplus */

#if __STDC_WANT_LIB_EXT1__ == 1
  _EXTERN_C
    __ATTRIBUTES int       snwprintf_s(wchar_t *_Restrict, rsize_t,
                                       const wchar_t *_Restrict, ...);
    __ATTRIBUTES int       swprintf_s(wchar_t *_Restrict, rsize_t,
                                      const wchar_t *_Restrict, ...);
    __ATTRIBUTES int       swscanf_s(const wchar_t *_Restrict,
                                     const wchar_t *_Restrict, ...);
    __ATTRIBUTES int       vsnwprintf_s(wchar_t *_Restrict, rsize_t,
                                        const wchar_t *_Restrict, __Va_list);
    __ATTRIBUTES int       vswprintf_s(wchar_t *_Restrict, rsize_t,
                                       const wchar_t *_Restrict, __Va_list);
    __ATTRIBUTES int       vswscanf_s(const wchar_t *_Restrict,
                                      const wchar_t *_Restrict, __Va_list);
    __ATTRIBUTES int       vwprintf_s(const wchar_t *_Restrict, __Va_list);
    __ATTRIBUTES int       vwscanf_s(const wchar_t *_Restrict, __Va_list);
    __ATTRIBUTES int       wprintf_s(const wchar_t *_Restrict, ...);
    __ATTRIBUTES int       wscanf_s(const wchar_t *_Restrict, ...);
    __ATTRIBUTES errno_t   wcscpy_s(wchar_t *_Restrict, rsize_t,
                                    const wchar_t *_Restrict);
    __ATTRIBUTES errno_t   wcsncpy_s(wchar_t *_Restrict, rsize_t,
                                     const wchar_t *_Restrict, rsize_t);
    __ATTRIBUTES errno_t   wmemcpy_s(wchar_t *_Restrict, rsize_t,
                                     const wchar_t *_Restrict, rsize_t);
    __ATTRIBUTES errno_t   wmemmove_s(wchar_t *, rsize_t,
                                      const wchar_t *, rsize_t);
    __ATTRIBUTES errno_t   wcscat_s(wchar_t *_Restrict, rsize_t,
                                    const wchar_t *_Restrict);
    __ATTRIBUTES errno_t   wcsncat_s(wchar_t *_Restrict, rsize_t,
                                     const wchar_t *_Restrict, rsize_t);
    __ATTRIBUTES wchar_t * wcstok_s(wchar_t *_Restrict, rsize_t *_Restrict,
                                    const wchar_t *_Restrict,
                                    wchar_t **_Restrict);
    __ATTRIBUTES size_t    wcsnlen_s(const wchar_t *, size_t);
    __ATTRIBUTES errno_t   wcrtomb_s(size_t *_Restrict, char *_Restrict,
                                     rsize_t, wchar_t, mbstate_t *_Restrict);
    __ATTRIBUTES errno_t   mbsrtowcs_s(size_t *_Restrict, wchar_t *_Restrict,
                                       rsize_t, const char **_Restrict, rsize_t,
                                       mbstate_t *_Restrict);
    __ATTRIBUTES errno_t   wcsrtombs_s(size_t *_Restrict, char *_Restrict,
                                       rsize_t, const wchar_t **_Restrict,
                                       rsize_t, mbstate_t *_Restrict);
    #ifndef _DLIB_CONFIG_AGNOSTIC
      #if _DLIB_FILE_DESCRIPTOR
        __ATTRIBUTES int   fwprintf_s(_Filet *_Restrict,
                                      const wchar_t *_Restrict, ...);
        __ATTRIBUTES int   fwscanf_s(_Filet *_Restrict,
                                     const wchar_t *_Restrict, ...);
        __ATTRIBUTES int   vfwprintf_s(_Filet *_Restrict,
                                       const wchar_t *_Restrict, __Va_list);
        __ATTRIBUTES int   vfwscanf_s(_Filet *_Restrict,
                                      const wchar_t *_Restrict, __Va_list);
      #endif /* _DLIB_FILE_DESCRIPTOR */
    #endif /* _DLIB_CONFIG_AGNOSTIC */
  _EXTERN_C_END
#endif /* __STDC_WANT_LIB_EXT1__ == 1 */

#endif /* _WCHAR */

/*
 * Copyright (c) by P.J. Plauger. All rights reserved.
 * Consult your license regarding permissions and restrictions.
V6.50:0576 */
