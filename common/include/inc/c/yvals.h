/* yvals.h internal configuration header file. */
/* Copyright 2001-2017 IAR Systems AB. */

#ifndef _YVALS
#define _YVALS

#ifndef _SYSTEM_BUILD
  #pragma system_include
#endif

/* Convenience macros */
#define _GLUE_B(x,y) x##y
#define _GLUE(x,y) _GLUE_B(x,y)

#define _GLUE3_B(x,y,z) x##y##z
#define _GLUE3(x,y,z) _GLUE3_B(x,y,z)

#define _STRINGIFY_B(x) #x
#define _STRINGIFY(x) _STRINGIFY_B(x)

/* Used to refer to '__aeabi' symbols in the library. */ 
#define _ABINAME(x) _GLUE_B(__aeabi_, x)

/* Dinkum version */
#define _CPPLIB_VER     650

/* DLib version */
#ifndef __IAR_SYSTEMS_LIB__
  #define __IAR_SYSTEMS_LIB__ 6
#endif

/* Module consistency. */
#pragma rtmodel = "__dlib_version", _STRINGIFY(__IAR_SYSTEMS_LIB__)

/* IAR compiler version check */
#if (__IAR_SYSTEMS_ICC__ < 9) || (__IAR_SYSTEMS_ICC__ > 9)
  #error "DLib compiled with wrong (version of IAR) compiler"
#endif


/* Read configuration */
#include <DLib_Defaults.h>

#if __AEABI_PORTABILITY_INTERNAL_LEVEL && !_DLIB_SUPPORT_FOR_AEABI
  #error "__AEABI_PORTABILITY_LEVEL != 0 needs a library built with _DLIB_SUPPORT_FOR_AEABI turned on"
#endif




/* A definiton for a function of what effects it has.
   NS  = no_state, errno, i.e. it uses no internal or external state. It may
         write to errno though
   NE  = no_state, i.e. it uses no internal or external state, not even
         writing to errno. 
   NRx = no_read(x), i.e. it doesn't read through pointer parameter x.
   NWx = no_write(x), i.e. it doesn't write through pointer parameter x.
   Rx  = returns x, i.e. the function will return parameter x.
   
   All the functions with effects also has "always_returns", 
   i.e. the function will always return.
*/

#define __EFF_NS       _Pragma("function_effects = no_state, write_errno, always_returns")
#define __EFF_NSNW1    _Pragma("function_effects = no_state, write_errno, no_write(1), always_returns")
#define __EFF_NE       _Pragma("function_effects = no_state, always_returns")
#define __EFF_NENR1    _Pragma("function_effects = no_state, no_read(1), always_returns")
#define __EFF_NENR1R1  _Pragma("function_effects = no_state, no_read(1), returns 1, always_returns")
#define __EFF_NENR2    _Pragma("function_effects = no_state, no_read(2), always_returns")
#define __EFF_NENR1NW2 _Pragma("function_effects = no_state, no_read(1), no_write(2), always_returns")
#define __EFF_NENR1NW2R1 _Pragma("function_effects = no_state, no_read(1), no_write(2), returns 1, always_returns")
#define __EFF_NENW1    _Pragma("function_effects = no_state, no_write(1), always_returns")
#define __EFF_NENW2    _Pragma("function_effects = no_state, no_write(2), always_returns")
#define __EFF_NENW2R1  _Pragma("function_effects = no_state, no_write(2), returns 1, always_returns")
#define __EFF_NENW1NW2 _Pragma("function_effects = no_state, no_write(1,2), always_returns")
#define __EFF_NR1      _Pragma("function_effects = no_read(1), always_returns")
#define __EFF_NR1NW2   _Pragma("function_effects = no_read(1), no_write(2), always_returns")
#define __EFF_NR1NW3   _Pragma("function_effects = no_read(1), no_write(3), always_returns")
#define __EFF_NW1      _Pragma("function_effects = no_write(1), always_returns")
#define __EFF_NW2      _Pragma("function_effects = no_write(2), always_returns")
#define __EFF_NW1NR2   _Pragma("function_effects = no_write(1), no_read(2), always_returns")
#define __EFF_NW1NW2   _Pragma("function_effects = no_write(1,2), always_returns")

/* Common function attribute macros */
#define __ATTRIBUTES             __intrinsic __nounwind
#define __ATTRIBUTES_CAN_THROW   __intrinsic 
#define __ATTRIBUTES_NORETURN    __intrinsic __noreturn __nounwind
#define __ATTRIBUTES_NORETURN_CAN_THROW __intrinsic __noreturn



/* namespace and extern "C" handling */
#ifdef __cplusplus
  #define _EXTERN_C           extern "C" {
  #define _EXTERN_C_END       }
  #define _GLB                ::
#else
  #define _EXTERN_C
  #define _EXTERN_C_END
  #define _GLB
#endif



/*
 * Support for C99/C11 functionality, C99 secure C functionality, and some
 * other functionality.
 *
 * This setting makes available some macros, functions, etc that are
 * either mandatory in C99/C11 or beneficial.
 *
 * Default is to include them.
 *
 * Disabling this in C++ mode will not compile (some C++ functions uses C99
 * functionality).
 */

#ifndef _DLIB_ONLY_C89
  /* Default turned on only when compiling C89 (not C++, C99, or C11). */
  #if defined(__cplusplus)
    #define _DLIB_ONLY_C89 0
  #elif defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
    #define _DLIB_ONLY_C89 0
  #else
    #define _DLIB_ONLY_C89 1
  #endif
#endif /* _DLIB_ONLY_C89 */

#ifndef _DLIB_ADD_EXTRA_SYMBOLS
  #define _DLIB_ADD_EXTRA_SYMBOLS 1
#endif /* _DLIB_ADD_EXTRA_SYMBOLS */

/* Secure C */
#ifndef __STDC_WANT_LIB_EXT1__ 
  #define __STDC_WANT_LIB_EXT1__ 0 /* Off by default */
#else
  #if __STDC_WANT_LIB_EXT1__ != 0 && __STDC_WANT_LIB_EXT1__ != 1
    #error "Invalid value used on __STDC_WANT_LIB_EXT1__"
  #endif
#endif /* __STDC_WANT_LIB_EXT1__ */

#if __STDC_WANT_LIB_EXT1__ == 1
  #define __DEPREC _Pragma("deprecated")
#else
  #define __DEPREC
#endif

#define __DEPREC_ATTRS           __DEPREC __ATTRIBUTES
#define __DEPREC_ATTRS_CAN_THROW __DEPREC __ATTRIBUTES_CAN_THROW 

/* C++ language setup */
#ifdef __cplusplus 
  /* Enables/Disables iterator debugging 
     (the setting must be the same for the whole application). */ 
  #ifndef _HAS_ITERATOR_DEBUGGING
    #define _HAS_ITERATOR_DEBUGGING 0
  #elif _HAS_ITERATOR_DEBUGGING != 0 && _HAS_ITERATOR_DEBUGGING != 1
    #error "Faulty value used"
  #endif

  /* Sets up exception support handling */
  #define _HAS_EXCEPTIONS      defined(__EXCEPTIONS)
  #define _HAS_NOEXCEPT        _HAS_EXCEPTIONS
  #if _HAS_NOEXCEPT
    #define _NOEXCEPT          noexcept
    #define _NOEXCEPT_OP(x)    noexcept(x)
  #else 
    #define _NOEXCEPT
    #define _NOEXCEPT_OP(x)
  #endif

  #define _STATIC_ASSERT2(test, mesg)                                     \
    static_assert(test, mesg) /* else dummy version in <xstddef> */

  #define _POINTER_TO(x)        ::std::pointer_traits<pointer>::pointer_to(x)

  #define _NOINLINE _Pragma("inline = never")

  #define _HAS_CPP17 1

  #ifndef __ARRAY_OPERATORS
    #error "<yvals.h> __ARRAY_OPERATORS not defined (c++)"
  #endif /* __ARRAY_OPERATORS */

  namespace std {
    typedef bool _Bool;
  }

  #ifndef _THREAD_LOCAL
    #define _THREAD_LOCAL thread_local
  #endif

  #define _Restrict       __restrict
#else /*  !__cplusplus */
  #define _NOEXCEPT

  #ifndef _THREAD_LOCAL
    #if _DLIB_ONLY_C89
      #define _THREAD_LOCAL
    #else
      #define _THREAD_LOCAL _Thread_local
    #endif
  #endif

  #if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
    #define _Restrict     restrict
  #else
    #define _Restrict     __restrict
  #endif
#endif /* __cplusplus */

/* MB_LEN_MAX (max for utf-8 is 4) */
#define _MBMAX          4

/* for parsing numerics */
#define _MAX_EXP_DIG    8
#define _MAX_INT_DIG    32
#define _MAX_SIG_DIG    36

/* wchar_t setup */
#ifdef __cplusplus
  #define _WCHART
  typedef wchar_t _Wchart;
  #if __WCHAR_T_SIZE__ >= __INT_SIZE__
    typedef wchar_t _Wintt;
  #elif defined(__SIGNED_WCHAR_T__)
    typedef signed int _Wintt;
  #else
    typedef unsigned int _Wintt;
  #endif
#else
  typedef __WCHAR_T_TYPE__ _Wchart;
  #if __WCHAR_T_SIZE__ >= __INT_SIZE__
    typedef __WCHAR_T_TYPE__ _Wintt;
  #elif defined(__SIGNED_WCHAR_T__)
    typedef signed int _Wintt;
  #else
    typedef unsigned int _Wintt;
  #endif
#endif
#ifndef __NO_WCHAR__
  #if __WCHAR_T_SIZE__ < __INT_SIZE__
    #error "Not supported, for now"
  #endif
#endif  
#ifdef __SIGNED_WCHAR_T__
  #define _WCMIN  __WCHAR_T_MIN__
  #define _WIMIN  __WCHAR_T_MIN__
#else
  #define _WCMIN  0
  #define _WIMIN  0
#endif
#define _WCMAX  __WCHAR_T_MAX__
#define _WIMAX  __WCHAR_T_MAX__

/* POINTER PROPERTIES */
#define _NULL           0       /* 0L if pointer same as long */

/* size_t setup */
typedef __SIZE_T_TYPE__     _Sizet;

/* mbstatet setup */
typedef struct _Mbstatet
{ /* state of a multibyte translation */
  #if _DLIB_SUPPORT_FOR_AEABI && !_DLIB_MBSTATET_USES_UNSIGNED_LONG
    unsigned int _Wchar;  /* Used as an intermediary value (up to 32-bits) */
    unsigned int _State;  /* Used as a state value (only some bits used) */
    #if __INT_SIZE__ != 4
      #pragma error "sizeof int must be 4"
    #endif
  #else
    unsigned long _Wchar;  /* Used as an intermediary value (up to 32-bits) */
    unsigned long _State;  /* Used as a state value (only some bits used) */
  #endif

  #ifdef __cplusplus
    _Mbstatet()
      : _Wchar(0), _State(0)
    {	// default construct
    }

    _Mbstatet(const _Mbstatet& _Right)
      : _Wchar(_Right._Wchar), _State(_Right._State)
    {	// copy construct
    }

    _Mbstatet& operator=(const _Mbstatet& _Right)
    {	// assign
      _Wchar = _Right._Wchar;
      _State = _Right._State;
      return (*this);
    }

    _Mbstatet(int i)
      : _Wchar(i), _State(0)
    {	// constructor that handles _Mbstatet t = {0};
    }
  #endif
} _Mbstatet;
#ifdef __cplusplus
  #define _Mbstinit(x)	  _Mbstatet x
#else
  #define _Mbstinit(x)    _Mbstatet x = {0}
#endif

/* printf setup */
#define _HAS_PRAGMA_PRINTF_ARGS

/* stdarg PROPERTIES */
#ifndef _VA_DEFINED
  #ifndef _VA_LIST_STACK_MEMORY_ATTRIBUTE
    #define _VA_LIST_STACK_MEMORY_ATTRIBUTE
  #endif

  typedef struct __va_list
  {
    char _VA_LIST_STACK_MEMORY_ATTRIBUTE *_Ap;
  } __va_list;
  typedef __va_list __Va_list;
#else
  typedef _VA_LIST __Va_list;
#endif

#ifdef __cplusplus
  namespace std {
    typedef ::__Va_list va_list;
  }
#endif /* __cplusplus */

#ifndef _DLIB_CONFIG_AGNOSTIC
  #if _DLIB_FILE_DESCRIPTOR
    typedef struct __FILE _Filet;
  #endif
#endif

/* File position */
typedef struct
{       
  #if _DLIB_SUPPORT_FOR_AEABI
    long long _Off;    /* can be system dependent */
  #else
    long _Off;    /* can be system dependent */
  #endif
  _Mbstatet _Wstate;
} _Fpost;

#ifndef _FPOSOFF
  #define _FPOSOFF(fp)  ((fp)._Off)
#endif

/* THREAD AND LOCALE CONTROL */
#define _MULTI_THREAD 1

/* MULTITHREAD PROPERTIES */
#if _MULTI_THREAD
  _EXTERN_C
  /* The lock interface for DLib to use. */ 
  __WEAK __ATTRIBUTES void __iar_Locksyslock_Malloc(void);
  __WEAK __ATTRIBUTES void __iar_Locksyslock_Stream(void);
  __WEAK __ATTRIBUTES void __iar_Locksyslock_Debug(void);
  __WEAK __ATTRIBUTES void __iar_Locksyslock_StaticGuard(void);
  __WEAK __ATTRIBUTES void __iar_Unlocksyslock_Malloc(void);
  __WEAK __ATTRIBUTES void __iar_Unlocksyslock_Stream(void);
  __WEAK __ATTRIBUTES void __iar_Unlocksyslock_Debug(void);
  __WEAK __ATTRIBUTES void __iar_Unlocksyslock_StaticGuard(void);

  #ifndef _DLIB_CONFIG_AGNOSTIC
    #if _DLIB_FILE_DESCRIPTOR
      __WEAK __ATTRIBUTES void __iar_Lockfilelock(_Filet *);
      __WEAK __ATTRIBUTES void __iar_Unlockfilelock(_Filet *);
    #endif
  #endif

  typedef void *__iar_Rmtx;

  __WEAK __ATTRIBUTES void __iar_Initdynamiclock(__iar_Rmtx *);
  __WEAK __ATTRIBUTES void __iar_Dstdynamiclock(__iar_Rmtx *);
  __WEAK __ATTRIBUTES void __iar_Lockdynamiclock(__iar_Rmtx *);
  __WEAK __ATTRIBUTES void __iar_Unlockdynamiclock(__iar_Rmtx *);

  _EXTERN_C_END
#else /* _MULTI_THREAD */
  #define __iar_Locksyslock_Malloc()        (void)0
  #define __iar_Locksyslock_Stream()        (void)0
  #define __iar_Locksyslock_Debug()         (void)0
  #define __iar_Locksyslock_StaticGuard()   (void)0
  #define __iar_Unlocksyslock_Malloc()      (void)0
  #define __iar_Unlocksyslock_Stream()      (void)0
  #define __iar_Unlocksyslock_Debug()       (void)0
  #define __iar_Unlocksyslock_StaticGuard() (void)0

  #ifndef _DLIB_CONFIG_AGNOSTIC
    #if _DLIB_FILE_DESCRIPTOR
      #define __iar_Lockfilelock(x)             (void)x
      #define __iar_Unlockfilelock(x)           (void)x
    #endif
  #endif

  typedef void *__iar_Rmtx;

  #define __iar_Initdynamiclock(x)          (void)x
  #define __iar_Dstdynamiclock(x)           (void)x
  #define __iar_Lockdynamiclock(x)          (void)x
  #define __iar_Unlockdynamiclock(x)        (void)x
#endif /* _MULTI_THREAD */

#ifdef __cplusplus
  namespace std {
    class __iar_Lockit_Malloc
    { // lock while object in existence -- MUST NEST
    public:
      explicit __iar_Lockit_Malloc() // set the lock
      {
        __iar_Locksyslock_Malloc();
      }
      ~__iar_Lockit_Malloc()         // clear the lock
      {
        __iar_Unlocksyslock_Malloc();
      }
    private:
      __iar_Lockit_Malloc(const __iar_Lockit_Malloc&);            // not defined
      __iar_Lockit_Malloc& operator=(const __iar_Lockit_Malloc&); // not defined
    };
    class __iar_Lockit_Debug
    { // lock while object in existence -- MUST NEST
    public:
      explicit __iar_Lockit_Debug() // set the lock
      {
        __iar_Locksyslock_Debug();
      }
      ~__iar_Lockit_Debug()         // clear the lock
      {
        __iar_Unlocksyslock_Debug();
      }
    private:
      __iar_Lockit_Debug(const __iar_Lockit_Debug&);            // not defined
      __iar_Lockit_Debug& operator=(const __iar_Lockit_Debug&); // not defined
    };

    enum _Uninitialized
    { // tag for suppressing initialization
      _Noinit
    };
  } /* namespace std */
#endif /* __cplusplus */

#endif /* _YVALS */

/*
 * Copyright (c) by P.J. Plauger. All rights reserved.
 * Consult your license regarding permissions and restrictions.
V6.50:0576 */
