/**************************************************
 *
 * This file shall be included in appropriate CMSIS header
 * files, to provide required functions and intrinsics when
 * building with the IAR C/C++ Compiler for ARM (iccarm).
 *
 * Copyright 2011 IAR Systems. All rights reserved.
 *
 * $Revision: 114409 $
 *
 **************************************************/

#ifndef __CMSIS_IAR_H__
#define __CMSIS_IAR_H__

#ifndef __ICCARM__
  #error This file should only be compiled by ICCARM
#endif

#pragma system_include

#include <intrinsics.h>

/* Define compiler macros for CPU architecture, used in CMSIS 5.
 */
#if __ARM_ARCH_6M__ || __ARM_ARCH_7M__ || __ARM_ARCH_7EM__ || __ARM_ARCH_8M_BASE__ || __ARM_ARCH_8M_BASE__
/* Macros already defined */
#else
  #if defined(__ARM8M_MAINLINE__) || defined(__ARM8EM_MAINLINE__)
    #define __ARM_ARCH_8M_MAIN__ 1
  #elif defined(__ARM8M_BASELINE__)
    #define __ARM_ARCH_8M_BASE__ 1
  #elif defined(__ARM_ARCH_PROFILE) && __ARM_ARCH_PROFILE == 'M'
    #if __ARM_ARCH == 6
      #define __ARM_ARCH_6M__ 1
    #elif __ARM_ARCH == 7
      #if __ARM_FEATURE_DSP
        #define __ARM_ARCH_7EM__ 1
      #else
        #define __ARM_ARCH_7M__ 1
      #endif
    #endif /* __ARM_ARCH */
  #endif /* __ARM_ARCH_PROFILE == 'M' */
#endif

/* __NO_RETURN
 * Override for compiler macro used in CMSIS 5.
 */
#ifndef __NO_RETURN
#define __NO_RETURN __attribute__((noreturn))
#endif

/* __USED
 * Override for compiler macro used in CMSIS 5,
 * inform that a variable shall be retained in executable image. Code Example:
 *
 * __USED uint32_t const CMSIS_RTOS_API_Version = osCMSIS;
 */
#ifndef __USED
#define __USED __attribute__((used))
#endif

/* __WEAK
 * Override for compiler macro used in CMSIS 5,
 * export a function or variable weakly to allow overwrites. Code Example:
 *
 * __WEAK void SystemInit(void)
 * {
 *   SystemCoreSetup();
 *   SystemCoreClockSetup();
 * }
 */
#ifndef __WEAK
#define __WEAK __attribute__((weak))
#endif

/* __ALIGNED
 * Override for compiler macro used in CMSIS 5,
 * minimum alignment for a variable. Code example:
 *
 *  uint32_t stack_space[0x100] __ALIGNED(8);
 */
#ifndef __ALIGNED
#define __ALIGNED(x) __attribute__((aligned(x)))
#endif

/* __PACKED
 * Override for compiler macro used in CMSIS 5,
 * request smallest possible alignment. Code Example:
 *
 * struct __PACKED {
 *   uint8_t  len;
 *   uint8_t  type;
 *   uint16_t langid;
 * } desc_langid;
 */
#ifndef __PACKED
#define __PACKED __attribute__((packed, aligned(1)))
#endif

#if (__CORE__ == __ARM6M__)
  /* Avoid clash between intrinsics.h and arm_math.h when compiling for Cortex-M0. */
  #define __CLZ __cmsis_iar_clz
  #define __SSAT __cmsis_iar_ssat
#endif

#pragma diag_suppress=Pe940
#pragma diag_suppress=Pe177

#define __enable_irq    __enable_interrupt
#define __disable_irq   __disable_interrupt
#define __NOP           __no_operation

#if (__VER__ >= 6040000)  /* If iccarm version is 6.40.x or newer ---------- */

#define __BKPT(value)   asm volatile ("BKPT\t%0" : : "i"(value))

#endif

#if (__VER__ < 6020000)  /* If iccarm version is older than 6.20.0 ---------- */

#if (__VER__ < 6010002)  /* If iccarm version is older than 6.10.2 ---------- */

static uint32_t __get_APSR(void)
{
  __ASM("mrs r0, apsr");
}

static uint32_t __get_xPSR(void)
{
  __ASM("mrs r0, psr");           /* assembler does not know "xpsr" */
}

#endif                   /* __VER__ < 6010002 */

static uint32_t __get_IPSR(void)
{
  __ASM("mrs r0, ipsr");
}

static uint32_t __get_PSR(void)
{
  __ASM("mrs r0, psr");
}

static uint32_t __get_PSP(void)
{
  __ASM("mrs r0, psp");
}

static void __set_PSP(uint32_t topOfProcStack)
{
  __ASM("msr psp, r0");
}

static uint32_t __get_MSP(void)
{
  __ASM("mrs r0, msp");
}

static void __set_MSP(uint32_t topOfMainStack)
{
  __ASM("msr msp, r0");
}

static __INLINE  void __WFI(void)
{
  __ASM ("wfi");
}

static __INLINE  void __WFE(void)
{
  __ASM ("wfe");
}

static __INLINE  void __SEV(void)
{
  __ASM ("sev");
}

static uint32_t __REV16(uint32_t value)
{
  __ASM("rev16 r0, r0");
}

#else                   /* __VER__ < 6020000 */

static uint32_t __get_xPSR(void)
{
  return __get_PSR();   /* __get_PSR() intrinsic introduced in iccarm 6.20 */
}

#endif                   /* __VER__ < 6020000 */

#if (__CORTEX_M >= 0x03)   /* __CORTEX_M is defined in core_cm0.h, core_cm3.h and core_cm4.h. */

#if (__VER__ < 6020000)  /* If iccarm version is older than 6.20.0 ---------- */

static __INLINE void __enable_fault_irq(void)
{
  __ASM ("cpsie f");
}

static __INLINE void __disable_fault_irq(void)
{
  __ASM ("cpsid f");
}

static uint32_t __RBIT(uint32_t value)
{
  __ASM("rbit r0, r0");
}

static uint8_t __LDREXB(volatile uint8_t *addr)
{
  __ASM("ldrexb r0, [r0]");
}

static uint16_t __LDREXH(volatile uint16_t *addr)
{
  __ASM("ldrexh r0, [r0]");
}

static uint32_t __LDREXW(volatile uint32_t *addr)
{
  __ASM("ldrex r0, [r0]");
}

static uint32_t __STREXB(uint8_t value, volatile uint8_t *addr)
{
  __ASM("strexb r0, r0, [r1]");
}

static uint32_t __STREXH(uint16_t value, volatile uint16_t *addr)
{
  __ASM("strexh r0, r0, [r1]");
}

static uint32_t __STREXW(uint32_t value, volatile uint32_t *addr)
{
  __ASM("strex r0, r0, [r1]");
}

static __INLINE void __CLREX(void)
{
  __ASM ("clrex");
}

#else                  /* __VER__ >= 6020000 --------------------- */

#define __LDREXW        __LDREX
#define __STREXW        __STREX
#define __enable_fault_irq __enable_fiq
#define __disable_fault_irq __disable_fiq

#endif                 /* __VER__ < 6020000 */

#endif /* (__CORTEX_M >= 0x03) */

#if (__CORTEX_M == 0x04)   /* __CORTEX_M is defined in core_cm0.h, core_cm3.h and core_cm4.h. */

#if (__VER__ < 6020000)  /* If iccarm version is older than 6.20.0 ---------- */

static uint32_t __get_FPSCR(void)
{
#if (__FPU_PRESENT == 1)   /* __FPU_PRESENT is defined in the device header file, if present in current device. */
  __ASM("vmrs r0, fpscr");
#else
  return(0);
#endif
}

static void __set_FPSCR(uint32_t fpscr)
{
#if (__FPU_PRESENT == 1)   /* __FPU_PRESENT is defined in the device header file, if present in current device. */
  __ASM("vmsr fpscr, r0");
#endif
}

#endif                 /* __VER__ < 6020000 */

#endif /* (__CORTEX_M == 0x04) */

#if (__VER__ >= 7000000)  /* If iccarm version is 7.x or newer ---------- */

#if (__CORTEX_M >= 0x03)   /* __CORTEX_M is defined in core_cm0.h, core_cm3.h and core_cm4.h. */

static __INLINE uint32_t __RRX(uint32_t value)
{
  uint32_t result;
  __ASM("RRX %0, %1" : "=r"(result) : "r" (value) );
  return(result);
}

static __INLINE uint8_t __LDRBT(volatile uint8_t *addr)
{
  uint32_t result;
  __ASM("LDRBT %0, [%1]" : "=r" (result) : "r" (addr) : "memory" );
  return ((uint8_t) result);
}

static __INLINE uint16_t __LDRHT(volatile uint16_t *addr)
{
  uint32_t result;
  __ASM("LDRHT %0, [%1]" : "=r" (result) : "r" (addr) : "memory" );
  return ((uint16_t) result);
}

static __INLINE uint32_t __LDRT(volatile uint32_t *addr)
{
  uint32_t result;
  __ASM("LDRT %0, [%1]" : "=r" (result) : "r" (addr) : "memory" );
  return(result);
}

static __INLINE void __STRBT(uint8_t value, volatile uint8_t *addr)
{
  __ASM("STRBT %1, [%0]" : : "r" (addr), "r" ((uint32_t)value) : "memory" );
}

static __INLINE void __STRHT(uint16_t value, volatile uint16_t *addr)
{
  __ASM("STRHT %1, [%0]" : : "r" (addr), "r" ((uint32_t)value) : "memory" );
}

static __INLINE void __STRT(uint32_t value, volatile uint32_t *addr)
{
  __ASM("STRT %1, [%0]" : : "r" (addr), "r" (value) : "memory" );
}

#endif /* (__CORTEX_M >= 0x03) */

#endif                 /* __VER__ >= 7000000 */

static __INLINE uint32_t __ROR(uint32_t op1, uint32_t op2)
{
  return (op1 >> op2) | (op1 << ((sizeof(op1)*8)-op2));
}

#pragma diag_default=Pe940
#pragma diag_default=Pe177

#endif /* __CMSIS_IAR_H__ */
