/* Part of the locale.h standard header */
/* Copyright 2015-2017 IAR Systems AB.  */
#ifndef _XCTYPE
#define _XCTYPE

#ifndef _SYSTEM_BUILD
  #pragma system_include
#endif

#include <ycheck.h>
#include <yvals.h>

_EXTERN_C

/* Ctype categories, C++ 22.4.1 */
#define _ctype_space    (1 << 0)
#define _ctype_print    (1 << 1)
#define _ctype_cntrl    (1 << 2)
#define _ctype_upper    (1 << 3)
#define _ctype_lower    (1 << 4)
#define _ctype_alpha    (1 << 5)
#define _ctype_digit    (1 << 6)
#define _ctype_punct    (1 << 7)
#define _ctype_xdigit   (1 << 8)
#define _ctype_blank    (1 << 9)
#define _ctype_alnum    (_ctype_alpha | _ctype_digit)
#define _ctype_graph    (_ctype_alnum | _ctype_punct)

/* LOCALE CATEGORY INDEXES */
#define _LC_COLLATE             0
#define _LC_CTYPE               1
#define _LC_MONETARY            2
#define _LC_NUMERIC             3
#define _LC_TIME                4
#define _LC_MESSAGE             5
#define _NCAT                   6       /* one more than last index */

/* Type used for mask in C++ ctype facet */
typedef unsigned short _CtypeMask;

struct __iar_Locale;

typedef struct _Locale_Profile_t
{
  struct __iar_Locale const *_Array[_NCAT];
//  _EncodingType const *_Encoding;
} _Locale_Profile_t;

__ATTRIBUTES int __iar_isw(_Wintt, _CtypeMask);
__ATTRIBUTES _CtypeMask __iar_getw(_Wintt);

_EXTERN_C_END

#endif /* _XCTYPE */
