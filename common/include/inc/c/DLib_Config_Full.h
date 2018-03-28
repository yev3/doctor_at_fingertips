/* DLib configuration. */
/* Copyright 2003-2017 IAR Systems AB.  */

#ifndef _DLIB_CONFIG_H
#define _DLIB_CONFIG_H

#ifndef _SYSTEM_BUILD
  #pragma system_include
#endif

/* Turn on locale support. */
#define _DLIB_FULL_LOCALE_SUPPORT 1

/* Turn on FILE descriptor support. */
#define _DLIB_FILE_DESCRIPTOR 1

/* Turn on multibyte formatting. */
#define _DLIB_FORMATTED_MULTIBYTE 1

/* Turn on support for hex-floats in strtod. */
#define _DLIB_STRTOD_HEX_FLOAT 1

#endif /* _DLIB_CONFIG_H */
