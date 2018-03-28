/********************************************************
 *
 * This file declares ARM ITM macros used with the C-SPY
 * event logging mechanism.
 *
 * Copyright 2011-2017 IAR Systems AB.
 *
 * $Revision: 112610 $
 *
 ********************************************************/

#ifndef __ARM_ITM_INCLUDED
#define __ARM_ITM_INCLUDED

#ifndef _SYSTEM_BUILD
  #pragma system_include
#endif

#if defined( __ARM_PROFILE_M__ )

#pragma language=extended

#include <intrinsics.h>


/* __ITM_EVENTx(channel, value)                                        */
/* Write a 8, 16, or 32-bit value to the specified ITM channel.        */
/* For the event logging mechanism channels 1-4 are available.         */
#define __ITM_EVENT8(channel, value)   {while ((*((volatile unsigned long *)(0xE0000000+4*(channel)))) == 0) ; *((volatile unsigned char *)(0xE0000000+4*(channel))) = (value);}
#define __ITM_EVENT16(channel, value)  {while ((*((volatile unsigned long *)(0xE0000000+4*(channel)))) == 0) ; *((volatile unsigned short *)(0xE0000000+4*(channel))) = (value);}
#define __ITM_EVENT32(channel, value)  {while ((*((volatile unsigned long *)(0xE0000000+4*(channel)))) == 0) ; *((volatile unsigned long *)(0xE0000000+4*(channel))) = (value);}

/* Address of the Debug Exception and Monitor Control Register (DEMCR) is 0xE000EDFC. */
/* The TRCENA bit controls the global enabling for all features configured and        */
/* controlled by the DWT and ITM blocks.                                              */
#define __DEMCR     (*((volatile unsigned long *)(0xE000EDFC)))
#define __TRCENA    0x01000000

/* ITM_EVENTx(channel, value)                                          */
/* If ITM is enabled and ready to send, then                           */
/* write a 8, 16, or 32-bit value to the specified ITM channel.        */
/* For the event logging mechanism channels 1-4 are available.         */
#define ITM_EVENT8(channel, value)   {if (__DEMCR & __TRCENA) {__ITM_EVENT8(channel, value)}}
#define ITM_EVENT16(channel, value)  {if (__DEMCR & __TRCENA) {__ITM_EVENT16(channel, value)}}
#define ITM_EVENT32(channel, value)  {if (__DEMCR & __TRCENA) {__ITM_EVENT32(channel, value)}}

/* ITM_EVENTx_WITH_PC(channel, value)                                                   */
/* Write a 8, 16, or 32-bit value to the specified ITM channel with a corresponding PC. */
/* For the event logging mechanism channels 1-4 are available.                          */
/* The current PC (program counter) is passed to the debugger using channel 5.          */
#define ITM_EVENT8_WITH_PC(channel, value)    {if (__DEMCR & __TRCENA) {__ITM_EVENT32(5, __get_PC()); __ITM_EVENT8((channel), (value))}}
#define ITM_EVENT16_WITH_PC(channel, value)   {if (__DEMCR & __TRCENA) {__ITM_EVENT32(5, __get_PC()); __ITM_EVENT16((channel), (value))}}
#define ITM_EVENT32_WITH_PC(channel, value)   {if (__DEMCR & __TRCENA) {__ITM_EVENT32(5, __get_PC()); __ITM_EVENT32((channel), (value))}}

#endif

#endif /* __ARM_ITM_INCLUDED */
