/* ycheck.h internal checking header file. */
/* Copyright 2005-2017 IAR Systems AB. */

/* Note that there is no include guard for this header. This is intentional. */

#ifndef _SYSTEM_BUILD
  #pragma system_include
#endif

/* __AEABI_PORTABILITY_INTERNAL_LEVEL
 *
 * Note: Redefined each time ycheck.h is included, i.e. for each
 * system header, to ensure that ABI support could be turned off/on
 * individually for each file.
 *
 * Possible values for this preprocessor symbol:
 *
 * 0 - ABI portability mode is disabled.
 *
 * 1 - ABI portability mode (version 1) is enabled.
 *
 * All other values are reserved for future use.
 */

#ifdef __AEABI_PORTABILITY_INTERNAL_LEVEL
  #undef __AEABI_PORTABILITY_INTERNAL_LEVEL
#endif

#ifdef _AEABI_PORTABILITY_LEVEL
  /* The user can define this in the source code before including
   * a standard header. */
  #define __AEABI_PORTABILITY_INTERNAL_LEVEL _AEABI_PORTABILITY_LEVEL
#elif defined(__AEABI_PORTABILITY_LEVEL)
  /* The user can define this in the source code before including
   * a standard header. */
  #define __AEABI_PORTABILITY_INTERNAL_LEVEL __AEABI_PORTABILITY_LEVEL
#elif defined(__DEFAULT_AEABI_PORTABILITY_LEVEL)
  /* The tool chain defines this if the project has enabled abi
   * portability. */
  #define __AEABI_PORTABILITY_INTERNAL_LEVEL __DEFAULT_AEABI_PORTABILITY_LEVEL
#else
  #define __AEABI_PORTABILITY_INTERNAL_LEVEL 0
#endif
