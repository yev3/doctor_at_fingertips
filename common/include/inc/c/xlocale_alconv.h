/* Part of the locale.h standard header */
/* Copyright 2009-2017 IAR Systems AB. */
#ifndef _LOCALE_AEABI_LCONV
#define _LOCALE_AEABI_LCONV

#ifndef _SYSTEM_BUILD
  #pragma system_include
#endif

struct __aeabi_lconv
{       /* locale-specific information */
  char *decimal_point; /* LC_NUMERIC */
  char *thousands_sep;
  char *grouping;
  char *int_curr_symbol; /* LC_MONETARY */
  char *currency_symbol;
  char *mon_decimal_point;
  char *mon_thousands_sep;
  char *mon_grouping;
  char *positive_sign;
  char *negative_sign;
  char int_frac_digits;
  char frac_digits;
  char p_cs_precedes;
  char p_sep_by_space;
  char n_cs_precedes;
  char n_sep_by_space;
  char p_sign_posn;
  char n_sign_posn;

  /* Added with C99 */
  char int_p_cs_precedes; 
  char int_n_cs_precedes;
  char int_p_sep_by_space;
  char int_n_sep_by_space;
  char int_p_sign_posn;
  char int_n_sign_posn;
};

#endif /* _LOCALE_AEABI_LCONV */
