////////////////////////////////////////////////////////////////////////////////
// Project 5, CPSC 5530 Embedded Systems, Seattle University
// Team "ARM Brewery" 
// Edward Guevara, David Pierce, and Yevgeni Kamenski
// 
// hardware_timer.c - 09/25/2017 5:17 PM
// Implementations of the functions needed for the hardware timer keeping
// track of the number of elapsed system ticks
//
// This is free and unencumbered software released into the public domain.
////////////////////////////////////////////////////////////////////////////////

#include <assert.h>
#include <inc/hw_ints.h>
#include <inc/hw_memmap.h>
#include <inc/hw_types.h>
#include <driverlib/interrupt.h>
#include <driverlib/sysctl.h>
#include <driverlib/timer.h>
#include <utils/hardware_timer.h>
#include "FreeRTOS.h"
#include "task.h"
#include "types.h"

/**
 * \brief Current elapsed number of ticks, local to this module, accessed
 * globally using GetElapsedTicks().
 */
static volatile system_tick_t elapsedTicks = 0;

/**
 * \brief Custom user hook to be called every hardware tick
 */
static volatile system_tick_hook_t systemTickHook = NULL;

/**
 * \brief Handler for LED and Speaker control and ticks.
 */
void Timer1IntHandler(){
    // Clear the timer interrupt.
  TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
  elapsedTicks++;
  if (systemTickHook) systemTickHook();
}

/**
 * \brief Initialize the hardware timer
 * \param tickDurationMs desired hardware tick duration in milliseconds
 */
void Timer1Init(ulong tickDurationMs) {
  // Get the number of system time cycles (not ticks) for one sys tick
  ulong sysTickPeriod = SysCtlClockGet() / 1000 * SYSTEM_TICK_DEFAULT_MS;

  // Enable the timer peripheral
  SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);

  // Enable processor interrupts.
  IntMasterEnable();

  // Configure 32-bit periodic timer.
  TimerConfigure(TIMER1_BASE,  TIMER_CFG_32_BIT_PER);
  TimerLoadSet(TIMER1_BASE, TIMER_A, sysTickPeriod);
  
  // Setup the interrupt for the timer timeout.
  IntEnable(INT_TIMER1A);
  TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);

  // Enable the timer.
  TimerEnable(TIMER1_BASE, TIMER_A);
}

/**
 * \brief Initializes the hardware tick timer using default tick duration
 */
void Timer1InitDefault() {
  Timer1Init(SYSTEM_TICK_DEFAULT_MS);
}

/**
 * \brief Attaches a user hook to be called every hardware tick.
 * \param hook function to call every hardware tick.
 */
void SysTickHookSet(system_tick_hook_t hook) {
  systemTickHook = hook;
}

/**
 * \brief Clears the user hook from being called.
 */
void SysTickHookClear() {
  systemTickHook = NULL;
}

/**
 * \brief Returns elapsed number of system ticks
 * \return number of hardware ticks
 */
system_tick_t GetElapsedTicks() {
  return elapsedTicks;
}
