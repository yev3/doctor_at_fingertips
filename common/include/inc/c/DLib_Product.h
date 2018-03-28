/* Copyright 2017 IAR Systems AB. */

#ifndef _DLIB_PRODUCTS_H_
#define _DLIB_PRODUCTS_H_

#ifndef _SYSTEM_BUILD
   #pragma system_include
#endif


/*********************************************************************
*
*       Configuration
*
*********************************************************************/

/* Wide character and multi byte character support in library.
 * This is not allowed to vary over configurations, since math-library
 * is built with wide character support.
 */
#define _DLIB_WIDE_CHARACTERS 1

/* This ensures that the standard header file "string.h" includes
 * the Arm-specific file "DLib_Product_string.h". */
#define _DLIB_PRODUCT_STRING 1

/* This ensures that the standard header file "fenv.h" includes
 * the Arm-specific file "DLib_Product_fenv.h". */
#define _DLIB_PRODUCT_FENV 1

/* This ensures that the standard header file "stdlib.h" includes
 * the Arm-specific file "DLib_Product_stdlib.h". */
#define _DLIB_PRODUCT_STDLIB 1

/* Max buffer used for swap in qsort */
#if !defined(_DLIB_QSORT_BUF_SIZE)
#define _DLIB_QSORT_BUF_SIZE 128
#endif

#if defined(__RWPI__)
  /* No thread local storage with RWPI */
  #define _THREAD_LOCAL
#endif  

/* Enable AEABI support */
#define _DLIB_SUPPORT_FOR_AEABI 1

/* Enable rtmodel for setjump buffer size */
#define _DLIB_USE_RTMODEL_JMP_BUF_NUM_ELEMENTS 1

/* Enable parsing of hex floats */
#if !defined(_DLIB_STRTOD_HEX_FLOAT)
#define _DLIB_STRTOD_HEX_FLOAT 1
#elif _DLIB_STRTOD_HEX_FLOAT != 1
#error "_DLIB_STRTOD_HEX_FLOAT must be 1 to match the  math-library"
#endif

/* Special placement for locale structures when building ropi libraries */
#if defined(__ROPI__)
#define _LOCALE_PLACEMENT_ static
#endif

/* Use atomic if possible */
#ifndef __STDC_NO_ATOMICS__
  #define _USE_ATOMIC_OPS 1
#endif  

/* CPP-library uses software floatingpoint interface */
#define __SOFTFP  _Pragma( "type_attribute=__softfp" )

/* functions for setting errno should be __no_scratch */
#define __NO_SCRATCH  _Pragma( "type_attribute=__no_scratch" )

/* Use speedy implementation of floats (simple quad). */
#define _DLIB_SPEEDY_FLOATS 1

/* Configure generic ELF init routines. */
#define _DLIB_ELF_INIT_STATIC_BASE_DECLARATION __no_init uint32_t __iar_SB @ r9
#define _DLIB_ELF_INIT_STATIC_BASE __iar_SB
#define _DLIB_ELF_INIT_FUNCTION_ATTRIBUTES \
  _Pragma("type_attribute=__interwork __thumb")
#define _DLIB_ELF_INIT_TABLE_MEMORY       __data
#define _DLIB_ELF_INIT_SOURCE_MEMORY      __data
#define _DLIB_ELF_INIT_DESTINATION_MEMORY __data
#define _DLIB_ELF_INIT_USE_RELATIVE_ROM_ADDRESSES 1
#define _DLIB_ELF_INIT_REGION_COUNT_TYPE uint32_t
#ifdef AVOID_ROPI
#define _DLIB_ELF_INIT_MODULE_ATTRIBUTES          \
  _Pragma("build_attribute vfpcc_compatible")     \
  _Pragma("build_attribute arm_thumb_compatible") \
  _Pragma("build_attribute rwpi_compatible")      \
  _Pragma("diag_suppress=Pa107")                  \
  _Pragma("rtmodel=\"CppFlavor\",\"*\"")          \
  _Pragma("diag_default=Pa107")
#else
#define _DLIB_ELF_INIT_MODULE_ATTRIBUTES          \
  _Pragma("build_attribute vfpcc_compatible")     \
  _Pragma("build_attribute arm_thumb_compatible") \
  _Pragma("build_attribute ropi_compatible")      \
  _Pragma("build_attribute rwpi_compatible")      \
  _Pragma("diag_suppress=Pa107")                  \
  _Pragma("rtmodel=\"CppFlavor\",\"*\"")          \
  _Pragma("diag_default=Pa107")
#endif
#define _DLIB_ELF_INIT_INTERFACE_VERSION 3

#define _DLIB_USE_RTMODEL_JMP_BUF_NUM_ELEMENTS 1

#define _XMATHWRAPPERS_DEF xarmmathwrappers.h

#define _CPPINIT_ATTR _Pragma("no_bounds")

#endif

