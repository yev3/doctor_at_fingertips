/**************************************************
 *
 * This file declares the ARM intrinsic inline functions.
 *
 * Copyright 1999-2006 IAR Systems. All rights reserved.
 *
 * $Revision: 117814 $
 *
 **************************************************/

#ifndef __INTRINSICS_INCLUDED
#define __INTRINSICS_INCLUDED

#include <yvals.h>

#ifndef _SYSTEM_BUILD
  #pragma system_include
#endif

/*
 * Check that the correct C compiler is used.
 */

#if ((__TID__ >> 8) & 0x007F) != 79
#error "File intrinsics.h can only be used together with iccarm."
#endif

/* Define function effects for intrinsics */
#define __IEFF_HS _Pragma("function_effects = hidden_state, always_returns")
#define __IEFF_NE _Pragma("function_effects = no_state, always_returns")
#define __IEFF_NENR2 _Pragma("function_effects = no_state, no_read(2), always_returns")
#define __IEFF_NENW1 _Pragma("function_effects = no_state, no_write(1), always_returns")

#ifdef __cplusplus
extern "C" {
#endif

#pragma language=save
#pragma language=extended

__ATTRIBUTES void    __no_operation(void);

__ATTRIBUTES void    __disable_interrupt(void);
__ATTRIBUTES void    __enable_interrupt(void);

typedef unsigned long __istate_t;

__ATTRIBUTES __istate_t __get_interrupt_state(void);
__ATTRIBUTES void __set_interrupt_state(__istate_t);

#ifdef __ARM_PROFILE_M__

/* System control access for Cortex-M cores */
__ATTRIBUTES unsigned long __get_PSR( void );
__ATTRIBUTES unsigned long __get_IPSR( void );
__IEFF_HS __ATTRIBUTES unsigned long __get_MSP( void );
__ATTRIBUTES void          __set_MSP( unsigned long );
__IEFF_HS __ATTRIBUTES unsigned long __get_PSP( void );
__ATTRIBUTES void          __set_PSP( unsigned long );
__IEFF_HS __ATTRIBUTES unsigned long __get_PRIMASK( void );
__ATTRIBUTES void          __set_PRIMASK( unsigned long );
__IEFF_HS __ATTRIBUTES unsigned long __get_CONTROL( void );
__ATTRIBUTES void          __set_CONTROL( unsigned long );

#ifdef __ARM7__

/* These are only available for v7M */
__IEFF_HS __ATTRIBUTES unsigned long __get_FAULTMASK( void );
__ATTRIBUTES void          __set_FAULTMASK(unsigned long);
__IEFF_HS __ATTRIBUTES unsigned long __get_BASEPRI( void );
__ATTRIBUTES void          __set_BASEPRI( unsigned long );

#endif /* __ARM7__ */
#else /* __ARM_PROFILE_M__ */

/* "Old" style intrerrupt control routines */
__ATTRIBUTES void __disable_irq(void);
__ATTRIBUTES void __enable_irq(void);
#endif /* __ARM_PROFILE_M__ */

__ATTRIBUTES void __disable_fiq(void);
__ATTRIBUTES void __enable_fiq(void);


/* ARM-mode intrinsics */

__IEFF_NE __ATTRIBUTES unsigned long __SWP( unsigned long, volatile unsigned long * );
__IEFF_NE __ATTRIBUTES unsigned char __SWPB( unsigned char, volatile unsigned char * );

typedef unsigned long __ul;

#define __cpid   unsigned __constrange(0,15)
#define __cpreg  unsigned __constrange(0,15)
#define __cpopcw unsigned __constrange(0,15)
#define __cpopc  unsigned __constrange(0,8)

/*  Co-processor operations */

__IEFF_HS __ATTRIBUTES void __CDP (__cpid coproc, __cpopcw opc1, __cpreg CRd, __cpreg CRn, __cpreg CRm, __cpopc opc2) ;
__IEFF_HS __ATTRIBUTES void __CDP2(__cpid coproc, __cpopcw opc1, __cpreg CRd, __cpreg CRn, __cpreg CRm, __cpopc opc2) ;

/*  Co-processor access */
__ATTRIBUTES void          __MCR( __cpid coproc, __cpopc opcode_1, __ul src,
                                 __cpreg CRn, __cpreg CRm, __cpopc opcode_2 )  ;
__ATTRIBUTES unsigned long __MRC( __cpid coproc, __cpopc opcode_1,
                                 __cpreg CRn, __cpreg CRm, __cpopc opcode_2 )  ;
__ATTRIBUTES void          __MCR2( __cpid coproc, __cpopc opcode_1, __ul src,
                                  __cpreg CRn, __cpreg CRm, __cpopc opcode_2 ) ;
__ATTRIBUTES unsigned long __MRC2( __cpid coproc, __cpopc opcode_1,
                                  __cpreg CRn, __cpreg CRm, __cpopc opcode_2 ) ;

__ATTRIBUTES void __MCRR (__cpid coproc, __cpopc opc1, unsigned long long src, __cpreg CRm) ;
__ATTRIBUTES void __MCRR2(__cpid coproc, __cpopc opc1, unsigned long long src, __cpreg CRm) ;

__ATTRIBUTES unsigned long long __MRRC (__cpid coproc, __cpopc opc1, __cpreg CRm) ;
__ATTRIBUTES unsigned long long __MRRC2(__cpid coproc, __cpopc opc1, __cpreg CRm) ;

/* Load coprocessor register. */
__ATTRIBUTES void __LDC  ( __cpid coproc, __cpreg CRn, volatile __ul const *src) ;
__ATTRIBUTES void __LDCL ( __cpid coproc, __cpreg CRn, volatile __ul const *src) ;
__ATTRIBUTES void __LDC2 ( __cpid coproc, __cpreg CRn, volatile __ul const *src) ;
__ATTRIBUTES void __LDC2L( __cpid coproc, __cpreg CRn, volatile __ul const *src) ;

/* Store coprocessor register. */
__ATTRIBUTES void __STC  ( __cpid coproc, __cpreg CRn, volatile __ul *dst) ;
__ATTRIBUTES void __STCL ( __cpid coproc, __cpreg CRn, volatile __ul *dst) ;
__ATTRIBUTES void __STC2 ( __cpid coproc, __cpreg CRn, volatile __ul *dst) ;
__ATTRIBUTES void __STC2L( __cpid coproc, __cpreg CRn, volatile __ul *dst) ;

/* Load coprocessor register (noindexed version with coprocessor option). */
__ATTRIBUTES void __LDC_noidx( __cpid coproc, __cpreg CRn, volatile __ul const *src,
                              unsigned __constrange(0,255) option);

__ATTRIBUTES void __LDCL_noidx( __cpid coproc, __cpreg CRn, volatile __ul const *src,
                               unsigned __constrange(0,255) option);

__ATTRIBUTES void __LDC2_noidx( __cpid coproc, __cpreg CRn, volatile __ul const *src,
                               unsigned __constrange(0,255) option);

__ATTRIBUTES void __LDC2L_noidx( __cpid coproc, __cpreg CRn, volatile __ul const *src,
                                unsigned __constrange(0,255) option);

/* Store coprocessor register (version with coprocessor option). */
__ATTRIBUTES void __STC_noidx( __cpid coproc, __cpreg CRn, volatile __ul *dst,
                              unsigned __constrange(0,255) option);

__ATTRIBUTES void __STCL_noidx( __cpid coproc, __cpreg CRn, volatile __ul *dst,
                               unsigned __constrange(0,255) option);

__ATTRIBUTES void __STC2_noidx( __cpid coproc, __cpreg CRn, volatile __ul *dst,
                               unsigned __constrange(0,255) option);

__ATTRIBUTES void __STC2L_noidx( __cpid coproc, __cpreg CRn, volatile __ul *dst,
                                unsigned __constrange(0,255) option);

#define __sys_reg  __spec_string const char *

/* Reads a system register */
__ATTRIBUTES unsigned long      __arm_rsr(__sys_reg special_register)   ;
__ATTRIBUTES unsigned long long __arm_rsr64(__sys_reg special_register) ;
__ATTRIBUTES void*              __arm_rsrp(__sys_reg special_register)  ;

/* Writes a system register */
__ATTRIBUTES void __arm_wsr(__sys_reg special_register, unsigned long value)        ;
__ATTRIBUTES void __arm_wsr64(__sys_reg special_register, unsigned long long value) ;
__ATTRIBUTES void __arm_wsrp(__sys_reg special_register, const void *value)         ;

/* Co-processor intrinsics, ACLE-style */
__IEFF_HS __ATTRIBUTES void __arm_cdp (__cpid coproc, __cpopcw opc1, __cpreg CRd, __cpreg CRn, __cpreg CRm, __cpopc opc2) ;
__IEFF_HS __ATTRIBUTES void __arm_cdp2(__cpid coproc, __cpopcw opc1, __cpreg CRd, __cpreg CRn, __cpreg CRm, __cpopc opc2) ;

__ATTRIBUTES void __arm_ldc  (__cpid coproc, __cpreg CRd, const void* p) ;
__ATTRIBUTES void __arm_ldcl (__cpid coproc, __cpreg CRd, const void* p) ;
__ATTRIBUTES void __arm_ldc2 (__cpid coproc, __cpreg CRd, const void* p) ;
__ATTRIBUTES void __arm_ldc2l(__cpid coproc, __cpreg CRd, const void* p) ;

__ATTRIBUTES void __arm_stc  (__cpid coproc, __cpreg CRd, const void* p) ;
__ATTRIBUTES void __arm_stcl (__cpid coproc, __cpreg CRd, const void* p) ;
__ATTRIBUTES void __arm_stc2 (__cpid coproc, __cpreg CRd, const void* p) ;
__ATTRIBUTES void __arm_stc2l(__cpid coproc, __cpreg CRd, const void* p) ;

__ATTRIBUTES void __arm_mcr (__cpid coproc, __cpopc opc1, __ul src, __cpreg CRn, __cpreg CRm, __cpopc opc2) ;
__ATTRIBUTES void __arm_mcr2(__cpid coproc, __cpopc opc1, __ul src, __cpreg CRn, __cpreg CRm, __cpopc opc2) ;

__ATTRIBUTES unsigned long __arm_mrc (__cpid coproc, __cpopc opc1, __cpreg CRn, __cpreg CRm, __cpopc opc2) ;
__ATTRIBUTES unsigned long __arm_mrc2(__cpid coproc, __cpopc opc1, __cpreg CRn, __cpreg CRm, __cpopc opc2) ;

__ATTRIBUTES void __arm_mcrr (__cpid coproc, __cpopc opc1, unsigned long long src, __cpreg CRm) ;
__ATTRIBUTES void __arm_mcrr2(__cpid coproc, __cpopc opc1, unsigned long long src, __cpreg CRm) ;

__ATTRIBUTES unsigned long long __arm_mrrc (__cpid coproc, __cpopc opc1, __cpreg CRm) ;
__ATTRIBUTES unsigned long long __arm_mrrc2(__cpid coproc, __cpopc opc1, __cpreg CRm) ;

#ifdef __ARM_PROFILE_M__
/* Status register access, v7M: */
__ATTRIBUTES unsigned long __get_APSR( void );
__ATTRIBUTES void          __set_APSR( unsigned long );
#else /* __ARM_PROFILE_M__ */
/* Status register access */
__ATTRIBUTES unsigned long __get_CPSR( void );
__ATTRIBUTES void          __set_CPSR( unsigned long );
#endif /* __ARM_PROFILE_M__ */

/* Floating-point status and control register access */
__ATTRIBUTES unsigned long __get_FPSCR( void );
__ATTRIBUTES void __set_FPSCR( unsigned long );

/* Architecture v5T, CLZ is also available in Thumb mode for Thumb2 cores */
__IEFF_NE __ATTRIBUTES unsigned char __CLZ( unsigned long );

/* Architecture v5TE */
#if !defined(__ARM_PROFILE_M__) || defined(__ARM_MEDIA__)
__IEFF_HS __ATTRIBUTES signed long __QADD( signed long, signed long );
__IEFF_HS __ATTRIBUTES signed long __QDADD( signed long, signed long );
__IEFF_HS __ATTRIBUTES signed long __QSUB( signed long, signed long );
__IEFF_HS __ATTRIBUTES signed long __QDSUB( signed long, signed long );

__IEFF_HS __ATTRIBUTES signed long __QDOUBLE( signed long );

__IEFF_HS __ATTRIBUTES int         __QFlag( void );
__IEFF_HS __ATTRIBUTES void __reset_Q_flag( void );
#endif

__IEFF_HS __ATTRIBUTES int         __QCFlag( void );
__IEFF_HS __ATTRIBUTES void __reset_QC_flag( void );

__IEFF_NE __ATTRIBUTES signed long __SMUL( signed short, signed short );

/* Architecture v6, REV and REVSH are also available in thumb mode */
__IEFF_NE __ATTRIBUTES unsigned long __REV( unsigned long );
__IEFF_NE __ATTRIBUTES signed long __REVSH( short );

__IEFF_NE __ATTRIBUTES unsigned long __REV16( unsigned long );
__IEFF_NE __ATTRIBUTES unsigned long __RBIT( unsigned long );

__IEFF_NENW1 __ATTRIBUTES unsigned char  __LDREXB( volatile unsigned char const * );
__IEFF_NENW1 __ATTRIBUTES unsigned short __LDREXH( volatile unsigned short const * );
__IEFF_NENW1 __ATTRIBUTES unsigned long  __LDREX ( volatile unsigned long const * );
__IEFF_NENW1 __ATTRIBUTES unsigned long long __LDREXD( volatile unsigned long long const * );

__IEFF_NENR2 __ATTRIBUTES unsigned long  __STREXB( unsigned char, volatile unsigned char * );
__IEFF_NENR2 __ATTRIBUTES unsigned long  __STREXH( unsigned short, volatile unsigned short * );
__IEFF_NENR2 __ATTRIBUTES unsigned long  __STREX ( unsigned long, volatile unsigned long * );
__IEFF_NENR2 __ATTRIBUTES unsigned long  __STREXD( unsigned long long, volatile unsigned long long * );

__ATTRIBUTES void __CLREX( void );

__ATTRIBUTES void __SEV( void );
__ATTRIBUTES void __WFE( void );
__ATTRIBUTES void __WFI( void );
__ATTRIBUTES void __YIELD( void );

__ATTRIBUTES void __PLI( volatile void const * );
__ATTRIBUTES void __PLD( volatile void const * );
__ATTRIBUTES void __PLDW( volatile void const * );

__IEFF_HS __ATTRIBUTES   signed long __SSAT     (unsigned long val,
                                      unsigned int __constrange( 1, 32 ) sat );
__IEFF_HS __ATTRIBUTES unsigned long __USAT     (unsigned long val,
                                      unsigned int __constrange( 0, 31 ) sat );


#ifdef __ARM_MEDIA__

/* Architecture v6 Media instructions */
__IEFF_HS __ATTRIBUTES unsigned long __SEL( unsigned long op1, unsigned long op2 );

__IEFF_HS __ATTRIBUTES unsigned long __SADD8    (unsigned long pair1, unsigned long pair2);
__IEFF_HS __ATTRIBUTES unsigned long __SADD16   (unsigned long pair1, unsigned long pair2);
__IEFF_HS __ATTRIBUTES unsigned long __SSUB8    (unsigned long pair1, unsigned long pair2);
__IEFF_HS __ATTRIBUTES unsigned long __SSUB16   (unsigned long pair1, unsigned long pair2);
__IEFF_HS __ATTRIBUTES unsigned long __SADDSUBX (unsigned long pair1, unsigned long pair2);
__IEFF_HS __ATTRIBUTES unsigned long __SSUBADDX (unsigned long pair1, unsigned long pair2);

__IEFF_NE __ATTRIBUTES unsigned long __SHADD8   (unsigned long pair1, unsigned long pair2);
__IEFF_NE __ATTRIBUTES unsigned long __SHADD16  (unsigned long pair1, unsigned long pair2);
__IEFF_NE __ATTRIBUTES unsigned long __SHSUB8   (unsigned long pair1, unsigned long pair2);
__IEFF_NE __ATTRIBUTES unsigned long __SHSUB16  (unsigned long pair1, unsigned long pair2);
__IEFF_NE __ATTRIBUTES unsigned long __SHADDSUBX(unsigned long pair1, unsigned long pair2);
__IEFF_NE __ATTRIBUTES unsigned long __SHSUBADDX(unsigned long pair1, unsigned long pair2);

__IEFF_HS __ATTRIBUTES unsigned long __QADD8    (unsigned long pair1, unsigned long pair2);
__IEFF_HS __ATTRIBUTES unsigned long __QADD16   (unsigned long pair1, unsigned long pair2);
__IEFF_HS __ATTRIBUTES unsigned long __QSUB8    (unsigned long pair1, unsigned long pair2);
__IEFF_HS __ATTRIBUTES unsigned long __QSUB16   (unsigned long pair1, unsigned long pair2);
__IEFF_HS __ATTRIBUTES unsigned long __QADDSUBX (unsigned long pair1, unsigned long pair2);
__IEFF_HS __ATTRIBUTES unsigned long __QSUBADDX (unsigned long pair1, unsigned long pair2);

__IEFF_HS __ATTRIBUTES unsigned long __UADD8    (unsigned long pair1, unsigned long pair2);
__IEFF_HS __ATTRIBUTES unsigned long __UADD16   (unsigned long pair1, unsigned long pair2);
__IEFF_HS __ATTRIBUTES unsigned long __USUB8    (unsigned long pair1, unsigned long pair2);
__IEFF_HS __ATTRIBUTES unsigned long __USUB16   (unsigned long pair1, unsigned long pair2);
__IEFF_HS __ATTRIBUTES unsigned long __UADDSUBX (unsigned long pair1, unsigned long pair2);
__IEFF_HS __ATTRIBUTES unsigned long __USUBADDX (unsigned long pair1, unsigned long pair2);

__IEFF_NE __ATTRIBUTES unsigned long __UHADD8   (unsigned long pair1, unsigned long pair2);
__IEFF_NE __ATTRIBUTES unsigned long __UHADD16  (unsigned long pair1, unsigned long pair2);
__IEFF_NE __ATTRIBUTES unsigned long __UHSUB8   (unsigned long pair1, unsigned long pair2);
__IEFF_NE __ATTRIBUTES unsigned long __UHSUB16  (unsigned long pair1, unsigned long pair2);
__IEFF_NE __ATTRIBUTES unsigned long __UHADDSUBX(unsigned long pair1, unsigned long pair2);
__IEFF_NE __ATTRIBUTES unsigned long __UHSUBADDX(unsigned long pair1, unsigned long pair2);

__IEFF_HS __ATTRIBUTES unsigned long __UQADD8   (unsigned long pair1, unsigned long pair2);
__IEFF_HS __ATTRIBUTES unsigned long __UQADD16  (unsigned long pair1, unsigned long pair2);
__IEFF_HS __ATTRIBUTES unsigned long __UQSUB8   (unsigned long pair1, unsigned long pair2);
__IEFF_HS __ATTRIBUTES unsigned long __UQSUB16  (unsigned long pair1, unsigned long pair2);
__IEFF_HS __ATTRIBUTES unsigned long __UQADDSUBX(unsigned long pair1, unsigned long pair2);
__IEFF_HS __ATTRIBUTES unsigned long __UQSUBADDX(unsigned long pair1, unsigned long pair2);

__IEFF_NE __ATTRIBUTES unsigned long __USAD8(unsigned long x, unsigned long y );
__IEFF_NE __ATTRIBUTES unsigned long __USADA8(unsigned long x, unsigned long y,
                                   unsigned long acc );

__IEFF_HS __ATTRIBUTES unsigned long __SSAT16   (unsigned long pair,
                                      unsigned int __constrange( 1, 16 ) sat );
__IEFF_HS __ATTRIBUTES unsigned long __USAT16   (unsigned long pair,
                                      unsigned int __constrange( 0, 15 ) sat );

__IEFF_HS __ATTRIBUTES long __SMUAD (unsigned long x, unsigned long y);
__IEFF_NE __ATTRIBUTES long __SMUSD (unsigned long x, unsigned long y);
__IEFF_HS __ATTRIBUTES long __SMUADX(unsigned long x, unsigned long y);
__IEFF_NE __ATTRIBUTES long __SMUSDX(unsigned long x, unsigned long y);

__IEFF_HS __ATTRIBUTES long __SMLAD (unsigned long x, unsigned long y, long sum);
__IEFF_HS __ATTRIBUTES long __SMLSD (unsigned long x, unsigned long y, long sum);
__IEFF_HS __ATTRIBUTES long __SMLADX(unsigned long x, unsigned long y, long sum);
__IEFF_HS __ATTRIBUTES long __SMLSDX(unsigned long x, unsigned long y, long sum);

__IEFF_NE __ATTRIBUTES long long __SMLALD (unsigned long pair1,
                                 unsigned long pair2,
                                 long long acc);

__IEFF_NE __ATTRIBUTES long long __SMLALDX(unsigned long pair1,
                                 unsigned long pair2,
                                 long long acc);

__IEFF_NE __ATTRIBUTES long long __SMLSLD (unsigned long pair1,
                                 unsigned long pair2,
                                 long long acc);

__IEFF_NE __ATTRIBUTES long long __SMLSLDX(unsigned long pair1,
                                 unsigned long pair2,
                                 long long acc);

__IEFF_NE __ATTRIBUTES unsigned long __PKHBT(unsigned long x,
                                  unsigned long y,
                                  unsigned __constrange(0,31) count);
__IEFF_NE __ATTRIBUTES unsigned long __PKHTB(unsigned long x,
                                  unsigned long y,
                                  unsigned __constrange(0,32) count);

__IEFF_HS __ATTRIBUTES long __SMLABB(unsigned long x, unsigned long y, long acc);
__IEFF_HS __ATTRIBUTES long __SMLABT(unsigned long x, unsigned long y, long acc);
__IEFF_HS __ATTRIBUTES long __SMLATB(unsigned long x, unsigned long y, long acc);
__IEFF_HS __ATTRIBUTES long __SMLATT(unsigned long x, unsigned long y, long acc);

__IEFF_HS __ATTRIBUTES long __SMLAWB(long x, unsigned long y, long acc);
__IEFF_HS __ATTRIBUTES long __SMLAWT(long x, unsigned long y, long acc);

__IEFF_NE __ATTRIBUTES long __SMMLA (long x, long y, long acc);
__IEFF_NE __ATTRIBUTES long __SMMLAR(long x, long y, long acc);
__IEFF_NE __ATTRIBUTES long __SMMLS (long x, long y, long acc);
__IEFF_NE __ATTRIBUTES long __SMMLSR(long x, long y, long acc);

__IEFF_NE __ATTRIBUTES long __SMMUL (long x, long y);
__IEFF_NE __ATTRIBUTES long __SMMULR(long x, long y);

__IEFF_NE __ATTRIBUTES long __SMULBB(unsigned long x, unsigned long y);
__IEFF_NE __ATTRIBUTES long __SMULBT(unsigned long x, unsigned long y);
__IEFF_NE __ATTRIBUTES long __SMULTB(unsigned long x, unsigned long y);
__IEFF_NE __ATTRIBUTES long __SMULTT(unsigned long x, unsigned long y);

__IEFF_NE __ATTRIBUTES long __SMULWB(long x, unsigned long y);
__IEFF_NE __ATTRIBUTES long __SMULWT(long x, unsigned long y);

__IEFF_NE __ATTRIBUTES long __SXTAB (long x, unsigned long y);
__IEFF_NE __ATTRIBUTES long __SXTAH (long x, unsigned long y);
__IEFF_NE __ATTRIBUTES unsigned long __UXTAB (unsigned long x, unsigned long y);
__IEFF_NE __ATTRIBUTES unsigned long __UXTAH (unsigned long x, unsigned long y);

__IEFF_NE __ATTRIBUTES unsigned long long __UMAAL(unsigned long x,
                                       unsigned long y,
                                       unsigned long a,
                                       unsigned long b);

__IEFF_NE __ATTRIBUTES long long __SMLALBB(unsigned long x,
                                 unsigned long y,
                                 long long acc);

__IEFF_NE __ATTRIBUTES long long __SMLALBT(unsigned long x,
                                 unsigned long y,
                                 long long acc);

__IEFF_NE __ATTRIBUTES long long __SMLALTB(unsigned long x,
                                 unsigned long y,
                                 long long acc);

__IEFF_NE __ATTRIBUTES long long __SMLALTT(unsigned long x,
                                 unsigned long y,
                                 long long acc);

__IEFF_NE __ATTRIBUTES unsigned long __UXTB16(unsigned long x);
__IEFF_NE __ATTRIBUTES unsigned long __UXTAB16(unsigned long acc, unsigned long x);

__IEFF_NE __ATTRIBUTES unsigned long __SXTB16(unsigned long x);
__IEFF_NE __ATTRIBUTES unsigned long __SXTAB16(unsigned long acc, unsigned long x);

__IEFF_HS __ATTRIBUTES unsigned long __SASX(unsigned long, unsigned long) ;
__IEFF_HS __ATTRIBUTES unsigned long __SSAX(unsigned long, unsigned long) ;
__IEFF_NE __ATTRIBUTES unsigned long __SHASX(unsigned long, unsigned long) ;
__IEFF_NE __ATTRIBUTES unsigned long __SHSAX(unsigned long, unsigned long) ;
__IEFF_HS __ATTRIBUTES unsigned long __QASX(unsigned long, unsigned long) ;
__IEFF_HS __ATTRIBUTES unsigned long __QSAX(unsigned long, unsigned long) ;

__IEFF_HS __ATTRIBUTES unsigned long __UASX(unsigned long, unsigned long) ;
__IEFF_HS __ATTRIBUTES unsigned long __USAX(unsigned long, unsigned long) ;
__IEFF_NE __ATTRIBUTES unsigned long __UHASX(unsigned long, unsigned long) ;
__IEFF_NE __ATTRIBUTES unsigned long __UHSAX(unsigned long, unsigned long) ;
__IEFF_HS __ATTRIBUTES unsigned long __UQASX(unsigned long, unsigned long) ;
__IEFF_HS __ATTRIBUTES unsigned long __UQSAX(unsigned long, unsigned long) ;
#endif /* __ARM_MEDIA__ */

/* Architecture v7 instructions */
__ATTRIBUTES void __DMB(void);
__ATTRIBUTES void __DSB(void);
__ATTRIBUTES void __ISB(void);

/* Architecture v8-M instructions */
__ATTRIBUTES unsigned long __TT(unsigned long);
__ATTRIBUTES unsigned long __TTT(unsigned long);
__ATTRIBUTES unsigned long __TTA(unsigned long);
__ATTRIBUTES unsigned long __TTAT(unsigned long);

#define __fabs(x)  fabs(x)
#define __fabsf(x) fabsf(x)

__ATTRIBUTES unsigned long __get_LR(void);
__ATTRIBUTES void __set_LR(unsigned long);

__ATTRIBUTES unsigned long __get_SP(void);
__ATTRIBUTES void __set_SP(unsigned long);

#pragma language=restore

#ifdef __cplusplus
}
#endif

#undef __IEFF_HS
#undef __IEFF_NE
#undef __IEFF_NENR2
#undef __IEFF_NENW1

#endif  /* __INTRINSICS_INCLUDED */
