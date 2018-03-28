/***************************************************
 *
 * DLib_Defaults.h is the library configuration manager.
 *
 * Copyright 2003-2017 IAR Systems AB.  
 *
 * This configuration header file performs the following tasks:
 *
 * 1. Includes the configuration header file, defined by _DLIB_CONFIG_FILE,
 *    that sets up a particular runtime environment.
 *
 * 2. Includes the product configuration header file, DLib_Product.h, that
 *    specifies default values for the product and makes sure that the
 *    configuration is valid.
 *
 * 3. Sets up default values for all remaining configuration symbols.
 *
 * This configuration header file, the one defined by _DLIB_CONFIG_FILE, and
 * DLib_Product.h configures how the runtime environment should behave. This
 * includes all system headers and the library itself, i.e. all system headers
 * includes this configuration header file, and the library has been built
 * using this configuration header file.
 *
 ***************************************************
 *
 * DO NOT MODIFY THIS FILE!
 *
 ***************************************************/

#ifndef _DLIB_DEFAULTS_H
#define _DLIB_DEFAULTS_H

#ifndef _SYSTEM_BUILD
  #pragma system_include
#endif

/* Include the main configuration header file. */
#if defined(_DLIB_CONFIG_FILE_HEADER_NAME)
  #include _DLIB_CONFIG_FILE_HEADER_NAME
  /* _DLIB_CONFIG_FILE_STRING is the quoted variant of above */
#elif defined(_DLIB_CONFIG_FILE)
  #include _STRINGIFY(_DLIB_CONFIG_FILE)
#else
  #pragma message("Library configuration file is not specified. Use\
  --dlib_config, please see the compiler reference guide for details.")
#endif

/* Include the product specific header file. */
#ifndef __NO_DLIB_PRODUCT_FILE
  #include <DLib_Product.h>
#endif


/*
 * The remainder of the file sets up defaults for a number of
 * configuration symbols, each corresponds to a feature in the
 * libary.
 *
 * The value of the symbols should either be 1, if the feature should
 * be supported, or 0 if it shouldn't. (Except where otherwise
 * noted.)
 */


/*
 * File handling
 *
 * Determines whether FILE descriptors and related functions exists or not.
 * When this feature is selected, i.e. set to 1, then FILE descriptors and
 * related functions (e.g. fprintf, fopen) exist. All files, even stdin,
 * stdout, and stderr will then be handled with a file system mechanism that
 * buffers files before accessing the lowlevel I/O interface (__open, __read,
 * __write, etc).
 *
 * If not selected, i.e. set to 0, then FILE descriptors and related functions
 * (e.g. fprintf, fopen) does not exist. All functions that normally uses
 * stderr will use stdout instead. Functions that uses stdout and stdin (like
 * printf and scanf) will access the lowlevel I/O interface directly (__open,
 * __read, __write, etc), i.e. there will not be any buffering.
 *
 * The default is not to have support for FILE descriptors.
 */

#ifndef _DLIB_FILE_DESCRIPTOR
  #define _DLIB_FILE_DESCRIPTOR 0
#endif


/*
 * Use static buffers for stdout
 *
 * This setting controls whether the stream stdout uses a static 80 bytes
 * buffer or uses a one byte buffer allocated in the file descriptor. This
 * setting is only applicable if the FILE descriptors are enabled above.
 *
 * Default is to use a static 80 byte buffer.
 */

#ifndef _DLIB_STDOUT_USES_STATIC_BUFFER
  #define _DLIB_STDOUT_USES_STATIC_BUFFER 1
#endif


/*
 * Support of locale interface
 *
 * "Locale" is the system in C that support language- and
 * contry-specific settings for a number of areas, including currency
 * symbols, date and time, and multibyte encodings.
 *
 * This setting determines whether the locale interface exist or not.
 * When this feature is selected, i.e. set to 1, the locale interface exist
 * (setlocale, etc). A number of preselected locales can be activated during
 * runtime. The preselected locales and encodings are choosen at linkage. The
 * application will start with the "C" locale choosen. (Single byte encoding is
 * always supported in this mode.)
 *
 *
 * If not selected, i.e. set to 0, the locale interface (setlocale, etc) does
 * not exist. The C locale is then preset and cannot be changed.
 *
 * The default is not to have support for the locale interface with the "C"
 * locale and the single byte encoding.
 */

#ifndef _DLIB_FULL_LOCALE_SUPPORT
  #define _DLIB_FULL_LOCALE_SUPPORT 0
#endif

/*
 * Define what memory to place the locale table segment (.iar.locale_table)
 * in.
 */
#ifndef _DLIB_LOCALE_TABLE_MEMORY
  #define _DLIB_LOCALE_TABLE_MEMORY
#endif

/*
 * Wide character and multi byte character support in library.
 */

#ifndef _DLIB_WIDE_CHARACTERS
  #define _DLIB_WIDE_CHARACTERS 1
#endif
#ifndef _DLIB_CONFIG_AGNOSTIC
  #if _DLIB_FILE_DESCRIPTOR && !_DLIB_WIDE_CHARACTERS
    #error "_DLIB_FILE_DESCRIPTOR == 0 if wide characters are disabled"
  #endif
  #if _DLIB_FULL_LOCALE_SUPPORT && !_DLIB_WIDE_CHARACTERS
    #error "_DLIB_FULL_LOCALE_SUPPORT == 0 if wide characters are disabled"
  #endif
#endif

/*
 * Support of multibytes in printf- and scanf-like functions
 *
 * This is the default value for _DLIB_PRINTF_MULTIBYTE and
 * _DLIB_SCANF_MULTIBYTE. See them for a description.
 *
 * Default is to not have support for multibytes in printf- and scanf-like
 * functions.
 */

#ifndef _DLIB_FORMATTED_MULTIBYTE
  #define _DLIB_FORMATTED_MULTIBYTE 0
#endif
#ifndef _DLIB_CONFIG_AGNOSTIC
  #if _DLIB_FORMATTED_MULTIBYTE && !_DLIB_WIDE_CHARACTERS
    #error "_DLIB_FORMATTED_MULTIBYTE == 0 if wide characters are disabled"
  #endif
#endif

/*
 * Hexadecimal floating-point numbers in strtod
 *
 * If selected, i.e. set to 1, strtod supports C99 hexadecimal floating-point
 * numbers. This also enables hexadecimal floating-points in internal functions
 * used for converting strings and wide strings to float, double, and long
 * double.
 *
 * If not selected, i.e. set to 0, C99 hexadecimal floating-point numbers
 * aren't supported.
 *
 * Default is not to support hexadecimal floating-point numbers.
 */

#ifndef _DLIB_STRTOD_HEX_FLOAT
  #define _DLIB_STRTOD_HEX_FLOAT 0
#endif


/*
 * Printf configuration symbols.
 *
 * All the configuration symbols described further on controls the behaviour
 * of printf, sprintf, and the other printf variants.
 *
 * The library proves four formatters for printf: 'tiny', 'small',
 * 'large', and 'default'.  The setup in this file controls all except
 * 'tiny'.  Note that both small' and 'large' explicitly removes
 * some features.
 */

/*
 * Handle multibytes in printf
 *
 * This setting controls whether multibytes and wchar_ts are supported in
 * printf. Set to 1 to support them, otherwise set to 0.
 *
 * See _DLIB_FORMATTED_MULTIBYTE for the default setting.
 */
#ifndef _DLIB_PRINTF_MULTIBYTE
  #define _DLIB_PRINTF_MULTIBYTE _DLIB_FORMATTED_MULTIBYTE
#endif
#ifndef _DLIB_CONFIG_AGNOSTIC
  #if _DLIB_PRINTF_MULTIBYTE && !_DLIB_FULL_LOCALE_SUPPORT
    #error "_DLIB_PRINTF_MULTIBYTE needs full locale"
  #endif
  #if _DLIB_PRINTF_MULTIBYTE && !_DLIB_WIDE_CHARACTERS
    #error "_DLIB_PRINTF_MULTIBYTE == 0 if wide characters are disabled"
  #endif
#endif

/*
 * Support of formatting anything larger than int in printf
 *
 * This setting controls if 'int' should be used internally in printf, rather
 * than the largest existing integer type. If 'int' is used, any integer or
 * pointer type formatting use 'int' as internal type even though the
 * formatted type is larger. Set to 1 to use 'int' as internal type, otherwise
 * set to 0.
 *
 * See also next configuration.
 *
 * Default is to internally use largest existing internally type.
 */
#ifndef _DLIB_PRINTF_INT_TYPE_IS_INT
  #define _DLIB_PRINTF_INT_TYPE_IS_INT 0
#endif

/*
 * Support of formatting anything larger than long in printf
 *
 * This setting controls if 'long' should be used internally in printf, rather
 * than the largest existing integer type. If 'long' is used, any integer or
 * pointer type formatting use 'long' as internal type even though the
 * formatted type is larger. Set to 1 to use 'long' as internal type,
 * otherwise set to 0.
 *
 * See also previous configuration.
 *
 * Default is to internally use largest existing internally type.
 */
#ifndef _DLIB_PRINTF_INT_TYPE_IS_LONG
  #define _DLIB_PRINTF_INT_TYPE_IS_LONG 0
#endif

#if _DLIB_PRINTF_INT_TYPE_IS_INT && _DLIB_PRINTF_INT_TYPE_IS_LONG
  #error "At most one of _DLIB_PRINTF_INT_TYPE_IS_INT and _DLIB_PRINTF_INT_TYPE_IS_LONG can be defined."
#endif

/*
 * Emit a char a time in printf
 *
 * This setting controls internal output handling. If selected, i.e. set to 1,
 * then printf emits one character at a time, which requires less code but
 * can be slightly slower for some types of output.
 *
 * If not selected, i.e. set to 0, then printf buffers some outputs.
 *
 * Note that it is recommended to either use full file support (see
 * _DLIB_FILE_DESCRIPTOR) or -- for debug output -- use the linker
 * option "-e__write_buffered=__write" to enable buffered I/O rather
 * than deselecting this feature.
 */
#ifndef _DLIB_PRINTF_CHAR_BY_CHAR
  #define _DLIB_PRINTF_CHAR_BY_CHAR 1
#endif


/*
 * Scanf configuration symbols.
 *
 * All the configuration symbols described here controls the
 * behaviour of scanf, sscanf, and the other scanf variants.
 *
 * The library proves three formatters for scanf: 'small', 'large',
 * and 'default'.  The setup in this file controls all, however both
 * 'small' and 'large' explicitly removes some features.
 */

/*
 * Handle multibytes in scanf
 *
 * This setting controls whether multibytes and wchar_t:s are supported in
 * scanf. Set to 1 to support them, otherwise set to 0.
 *
 * See _DLIB_FORMATTED_MULTIBYTE for the default.
 */
#ifndef _DLIB_SCANF_MULTIBYTE
  #define _DLIB_SCANF_MULTIBYTE _DLIB_FORMATTED_MULTIBYTE
#endif
#ifndef _DLIB_CONFIG_AGNOSTIC
  #if _DLIB_SCANF_MULTIBYTE && !_DLIB_FULL_LOCALE_SUPPORT
    #error "_DLIB_SCANF_MULTIBYTE needs full locale"
  #endif
  #if _DLIB_SCANF_MULTIBYTE && !_DLIB_WIDE_CHARACTERS
    #error "_DLIB_SCANF_MULTIBYTE == 0 if wide characters are disabled"
  #endif
#endif

/*
 * Handle multibytes in asctime and strftime.
 *
 * This setting controls whether multibytes and wchar_ts are
 * supported.Set to 1 to support them, otherwise set to 0.
 *
 * See _DLIB_FORMATTED_MULTIBYTE for the default setting.
 */
#ifndef _DLIB_STRFTIME_MULTIBYTE
  #define _DLIB_STRFTIME_MULTIBYTE _DLIB_FORMATTED_MULTIBYTE
#endif
#ifndef _DLIB_CONFIG_AGNOSTIC
  #if _DLIB_STRFTIME_MULTIBYTE && !_DLIB_FULL_LOCALE_SUPPORT
    #error "_DLIB_STRFTIME_MULTIBYTE needs full locale"
  #endif
  #if _DLIB_STRFTIME_MULTIBYTE && !_DLIB_WIDE_CHARACTERS
    #error "_DLIB_STRFTIME_MULTIBYTE == 0 if wide characters are disabled"
  #endif
#endif

/*
 * Implement "qsort" using a bubble sort algorithm.
 *
 * Bubble sort is less efficient than quick sort but requires less RAM
 * and ROM resources.
 */

#ifndef _DLIB_QSORT_USE_BUBBLE_SORT
  #define _DLIB_QSORT_USE_BUBBLE_SORT 0
#endif


/*
 * Set Buffert size used in qsort
 */

#ifndef _DLIB_QSORT_BUF_SIZE
  #define _DLIB_QSORT_BUF_SIZE 256
#endif


/*
 * Use a simple rand implementation to reduce memory footprint.
 *
 * The default "rand" function uses an array of 32 long:s of memory to
 * store the current state.
 *
 * The simple "rand" function uses only a single long. However, the
 * quality of the generated psuedo-random numbers are not as good as
 * the default implementation.
 */

#ifndef _DLIB_RAND_USE_SIMPLE
  #define _DLIB_RAND_USE_SIMPLE 0
#endif


/*
 * Set attributes for the function used by the C-SPY debug interface to stop at.
 */

#ifndef _DLIB_DEBUG_BREAK_FUNCTION_ATTRIBUTE
  #define _DLIB_DEBUG_BREAK_FUNCTION_ATTRIBUTE
#endif

/*
 * Used by products where one runtime library can be used by applications
 * with different data models, in order to reduce the total number of
 * libraries required. Typically, this is used when the pointer types does
 * not change over the data models used, but the placement of data variables
 * or/and constant variables do.
 *
 * If defined, this symbol is typically defined to the memory attribute that
 * is used by the runtime library. The actual define must use a
 * _Pragma("type_attribute = xxx") construct. In the header files, it is used
 * on all statically linked data objects seen by the application.
 */
#ifndef _DLIB_DATA_ATTR
  #define _DLIB_DATA_ATTR
#endif

#ifndef _DLIB_CONST_ATTR
  #ifdef _DLIB_DATA_ATTR
    #define _DLIB_CONST_ATTR _DLIB_DATA_ATTR
  #else
    #define _DLIB_CONST_ATTR
  #endif
#endif


/*
 * Turn on support for the Target-specific ABI. The ABI is based on the
 * ARM AEABI. A target, except ARM, may deviate from it.
 */

#ifndef _DLIB_SUPPORT_FOR_AEABI
  #ifdef __EDG_IA64_ABI
    #define _DLIB_SUPPORT_FOR_AEABI 1
  #else
    #define _DLIB_SUPPORT_FOR_AEABI 0
  #endif
#endif

#if _DLIB_SUPPORT_FOR_AEABI == 1 
  /* Possible AEABI deviations */
  #ifndef _DLIB_MBSTATET_IS_UNSIGNED_LONG
    #if __INT_SIZE__ != 4
      #define _DLIB_MBSTATET_USES_UNSIGNED_LONG 1
    #else
      #define _DLIB_MBSTATET_USES_UNSIGNED_LONG 0
    #endif
  #endif

  #ifndef _DLIB_TIME_USES_LONG
    #if __INT_SIZE__ != 4
      #define _DLIB_TIME_USES_LONG 1
    #else
      #define _DLIB_TIME_USES_LONG 0
    #endif
  #endif

  /*
   * The "difunc" table contains information about C++ objects that
   * should be dynamically initialized, where each entry in the table
   * represents an initialization function that should be called. When
   * the symbol _DLIB_AEABI_DIFUNC_CONTAINS_OFFSETS is true, each
   * entry in the table is encoded as an offset from the entry
   * location. When false, the entries contain the actual addresses to
   * call.
   */
  #ifndef _DLIB_AEABI_DIFUNC_CONTAINS_OFFSETS
    #define _DLIB_AEABI_DIFUNC_CONTAINS_OFFSETS 1
  #endif
#endif /* _DLIB_SUPPORT_FOR_AEABI == 1  */


/*
 * Turn on usage of a pragma to tell the linker the number of elements used
 * in a setjmp jmp_buf.
 */

#ifndef _DLIB_USE_RTMODEL_JMP_BUF_NUM_ELEMENTS
  #define _DLIB_USE_RTMODEL_JMP_BUF_NUM_ELEMENTS 0
#endif


/*
 * If true, the product supplies a "DLib_Product_string.h" file that
 * is included from "string.h".
 */

#ifndef _DLIB_PRODUCT_STRING
  #define _DLIB_PRODUCT_STRING 0
#endif

/*
 * Determine whether the math fma routines are fast or not.
 */
#ifndef _DLIB_FAST_FMA
  #define _DLIB_FAST_FMA 0
#endif


/*
 * Favor speed versus some size enlargements in floating point functions.
 */
#ifndef _DLIB_SPEEDY_FLOATS
  #define _DLIB_SPEEDY_FLOATS 0
#endif


/*
 * Include dlmalloc as an alternative heap manager in the product.
 *
 * Typically, an application will use a "malloc" heap manager that is
 * relatively small but not that efficient. An application can
 * optionally use the "dlmalloc" package, which provides a more
 * effective "malloc" heap manager, if it is included in the product
 * and supported by the settings.
 *
 * See the product documentation on how to use it, and whether or not
 * it is included in the product.
 */

#ifndef _DLIB_INCLUDE_DLMALLOC_ALTERNATIVE
  /* size_t/ptrdiff_t must be a 4 bytes unsigned integer. */
  #if __SIZE_T_MAX__ < 0xfffffffful
    #define _DLIB_INCLUDE_DLMALLOC_ALTERNATIVE 0
  #else
    #define _DLIB_INCLUDE_DLMALLOC_ALTERNATIVE 1
  #endif
#endif


/*
 * Make sure certain C++ functions use the soft floating point variant.
 */

#ifndef __SOFTFP
  #define __SOFTFP
#endif


/*
 * Allow the 64-bit time_t interface?
 *
 * Default is yes if long long is 64 bits.
 */

#ifndef _DLIB_TIME_ALLOW_64
  #if __SIGNED_LONG_LONG_MAX__ > __SIGNED_LONG_MAX__
    #define _DLIB_TIME_ALLOW_64 1
  #else
    #define _DLIB_TIME_ALLOW_64 0
  #endif
#endif


/*
 * Is time_t 64 or 32 bits?
 *
 * Default is 32 bits.
 */

#ifndef _DLIB_TIME_USES_64_DEFAULT
  #define _DLIB_TIME_USES_64_DEFAULT 0
#endif


/*
 * Do we include math functions that demands lots of constant bytes?
 * (like erf, erfc, expm1, fma, lgamma, tgamma, and *_accurate)
 *
 */

#ifndef _DLIB_ALLOW_LARGE_CONSTANT_TABLES_FOR_MATH
  #define _DLIB_ALLOW_LARGE_CONSTANT_TABLES_FOR_MATH 1
#endif


/*
 * Support of weak.
 *
 * __weak must be supported. Support of weak means that the call to
 * a weak declared function that isn't part of the application will be
 * executed as a nop instruction.
 *
 */

#ifndef __WEAK
  #if !defined(__HAS_WEAK__) || !__HAS_WEAK__
    #error "Compiler must support __weak"
  #endif
  #define __WEAK _Pragma("object_attribute = __weak")
  #define __WEAK_DEFS 1
#endif

/*
 * Deleted options
 */
#ifdef _DLIB_FLOAT_ENVIRONMENT
  #error "Option has been removed"
#endif

#ifdef _DLIB_SMALL_TARGET
  #error "Option has been removed"
#endif

#endif /* _DLIB_DEFAULTS_H */
