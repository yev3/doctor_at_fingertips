/* ctype.h standard header */
/* Copyright 2003-2017 IAR Systems AB. */
#ifndef _CTYPE
#define _CTYPE

#ifndef _SYSTEM_BUILD
  #pragma system_include
#endif

#include <ycheck.h>
#include <yvals.h>

#if __AEABI_PORTABILITY_INTERNAL_LEVEL
  #ifndef __AEABI_PORTABLE
    #define __AEABI_PORTABLE
  #endif
#endif

/* Module consistency. */
#pragma rtmodel="__dlib_full_locale_support", \
  _STRINGIFY(_DLIB_FULL_LOCALE_SUPPORT)

_EXTERN_C
         __ATTRIBUTES int    isalnum(int);
         __ATTRIBUTES int    isalpha(int);
         #if !_DLIB_ONLY_C89
           __ATTRIBUTES int  isblank(int);
         #endif
         __ATTRIBUTES int    iscntrl(int);
__EFF_NE __ATTRIBUTES int    isdigit(int);
         __ATTRIBUTES int    isgraph(int);
         __ATTRIBUTES int    islower(int);
         __ATTRIBUTES int    isprint(int);
         __ATTRIBUTES int    ispunct(int);
         __ATTRIBUTES int    isspace(int);
         __ATTRIBUTES int    isupper(int);
__EFF_NE __ATTRIBUTES int    isxdigit(int);
         __ATTRIBUTES int    tolower(int);
         __ATTRIBUTES int    toupper(int);
_EXTERN_C_END

#if __AEABI_PORTABILITY_INTERNAL_LEVEL || _DLIB_SUPPORT_FOR_AEABI
  /* Aeabi table constants */
  #define __A   1 /* alphabetic */
  #define __X   2 /* A-F, a-f and 0-9 */
  #define __P   4 /* punctuation */
  #define __B   8 /* blank */
  #define __S  16 /* white space */
  #define __L  32 /* lower case letters */
  #define __U  64 /* upper case letters */
  #define __C 128 /* control chars */
#endif

#if !defined(_NO_DEFINITIONS_IN_HEADER_FILES)
  #if !__AEABI_PORTABILITY_INTERNAL_LEVEL
    #if !_DLIB_ONLY_C89
      #pragma inline=no_body
      int isblank(int _C)
      {
        return _C == ' ' || _C == '\t';
      }
    #endif

    #pragma inline=no_body
    int isdigit(int _C)
    {
      return _C >= '0' && _C <= '9';
    }

    #pragma inline=no_body
    int isxdigit(int _C)
    {
      return    (_C >= 'a' && _C <= 'f')
             || (_C >= 'A' && _C <= 'F')
             || isdigit(_C);
    }

    #pragma inline=no_body
    int isalnum(int _C)
    {
      return isalpha(_C) || isdigit(_C);
    }

    #pragma inline=no_body
    int isprint(int _C)
    {
      return (_C >= ' ' && _C <= '\x7e') || isalpha(_C) || ispunct(_C);
    }

    #pragma inline=no_body
    int isgraph(int _C)
    {
      return _C != ' ' && isprint(_C);
    }

    #if !_DLIB_FULL_LOCALE_SUPPORT
      #pragma inline=no_body
      int islower(int _C)
      {
        return _C >= 'a' && _C <= 'z';
      }

      #pragma inline=no_body
      int isupper(int _C)
      {
        return _C >= 'A' && _C <= 'Z';
      }

      #pragma inline=no_body
      int isalpha(int _C)
      {
        return islower(_C) || isupper(_C);
      }

      #pragma inline=no_body
      int iscntrl(int _C)
      {
        return (_C >= '\x00' && _C <= '\x1f') || _C == '\x7f';
      }

      #pragma inline=no_body
      int ispunct(int _C)
      {
        return    (_C>='\x21' && _C<='\x2f')
               || (_C>='\x3a' && _C<='\x40')
               || (_C>='\x5b' && _C<='\x60')
               || (_C>='\x7b' && _C<='\x7e');
      }

      #pragma inline=no_body
      int isspace(int _C)
      {
        return (_C >= '\x09' && _C <= '\x0d') || _C == ' ';
      }

      #pragma inline=no_body
      int tolower(int _C)
      {
        return isupper(_C) ? _C - 'A' + 'a' : _C;
      }

      #pragma inline=no_body
      int toupper(int _C)
      {
        return islower(_C) ? _C - 'a' + 'A' : _C;
      }
    #endif /* !_DLIB_FULL_LOCALE_SUPPORT */
  #else /* !__AEABI_PORTABILITY_INTERNAL_LEVEL */
    #if defined(__AEABI_BIND_STATICALLY) && __AEABI_BIND_STATICALLY
      /* We only have the default table (ASCII only) */
      _EXTERN_C
      _DLIB_CONST_ATTR extern char const __aeabi_ctype_table_[257];
      _DLIB_CONST_ATTR extern char const __aeabi_ctype_table_C[257];
      _EXTERN_C_END
      #ifdef _AEABI_LC_CTYPE
        #define __aeabi_ctype_table _GLUE(__aeabi_ctype_table_, _AEABI_LC_CTYPE)
      #else
        #define __aeabi_ctype_table __aeabi_ctype_table_
      #endif

      #pragma inline=forced
      int isdigit(int _C)
      {
        return _C >= '0' && _C <= '9';
      }
      #pragma inline=forced
      int isspace (int _C)
      {
        return __aeabi_ctype_table[_C + 1] & __S;
      }
      #pragma inline=forced
      int isalpha (int _C)
      {
        return __aeabi_ctype_table[_C + 1] & __A;
      }
      #pragma inline=forced
      int isalnum (int _C)
      {
        return __aeabi_ctype_table[_C + 1] & (__A | __X);
      }
      #pragma inline=forced
      int isprint (int _C)
      {
        return __aeabi_ctype_table[_C + 1] & (__A | __X | __P | __B);
      }
      #pragma inline=forced
      int isupper (int _C)
      {
        return __aeabi_ctype_table[_C + 1] & __U;
      }
      #pragma inline=forced
      int islower (int _C)
      {
        return __aeabi_ctype_table[_C + 1] & __L;
      }
      #pragma inline=forced
      int isxdigit(int _C)
      {
        return __aeabi_ctype_table[_C + 1] & __X;
      }
      #if !_DLIB_ONLY_C89
        #pragma inline=forced
        int isblank (int _C)
        {
          return __aeabi_ctype_table[_C + 1] & __B;
        }
      #endif
      #pragma inline=forced
      int isgraph (int _C)
      {
        return __aeabi_ctype_table[_C + 1] & (__A | __X | __P);
      }
      #pragma inline=forced
      int iscntrl (int _C)
      {
        return __aeabi_ctype_table[_C + 1] & __C;
      }
      #pragma inline=forced
      int ispunct (int _C)
      {
        return __aeabi_ctype_table[_C + 1] & __P;
      }

      #pragma inline=forced
      int tolower(int _C)
      {
        return isupper(_C) ? (_C + ('A' - 'a')) : _C;
      }
      #pragma inline=forced
      int toupper(int _C)
      {
        return islower(_C) ? (_C + ('a' - 'A')) : _C;
      }
    #endif /* __AEABI_BIND_STATICALLY */
  #endif /* !__AEABI_PORTABILITY_INTERNAL_LEVEL */
#endif /* !defined(_NO_DEFINITIONS_IN_HEADER_FILES) */

#endif /* _CTYPE */

/*
 * Copyright (c) by P.J. Plauger. All rights reserved.
 * Consult your license regarding permissions and restrictions.
V6.50:0576 */
