/* Part of the locale.h standard header */
/* Copyright 2009-2017 IAR Systems AB. */
#ifndef _LOCALE_LCONV
#define _LOCALE_LCONV

#ifndef _SYSTEM_BUILD
  #pragma system_include
#endif

struct lconv
{       /* locale-specific information */
  /* controlled by LC_MONETARY */
  char *currency_symbol;
  char *int_curr_symbol;
  char *mon_decimal_point;
  char *mon_grouping;
  char *mon_thousands_sep;
  char *negative_sign;
  char *positive_sign;

  char frac_digits;
  char n_cs_precedes;
  char n_sep_by_space;
  char n_sign_posn;
  char p_cs_precedes;
  char p_sep_by_space;
  char p_sign_posn;

  char int_frac_digits;

  /* controlled by LC_NUMERIC */
  char *decimal_point;
  char *grouping;
  char *thousands_sep;

  /* controlled by LC_MONETARY. Added with C99 */
  char int_n_cs_precedes;
  char int_n_sep_by_space;
  char int_n_sign_posn;
  char int_p_cs_precedes;
  char int_p_sep_by_space;
  char int_p_sign_posn;
};

#endif /* _LOCALE_LCONV */
