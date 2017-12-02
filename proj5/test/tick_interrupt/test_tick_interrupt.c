////////////////////////////////////////////////////////////////////////////////
// Project 3, CPSC 5530 Embedded Systems, Seattle University
// Team "ARM Brewery" 
// Edward Guevara, David Pierce, and Yevgeni Kamenski
// 
// test_tick_interrupt.c - 10/12/2017
// Test the hardware tick interrupt timing.
//
// This is free and unencumbered software released into the public domain.
////////////////////////////////////////////////////////////////////////////////
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

#include <inc/hw_types.h>
#include <inc/lm3s8962.h>
#include <driverlib/sysctl.h>
#include <utils/hardware_timer.h>
#include "../../utils/lcd_print.h"

/**
 * \brief Does not run if at least one hardware tick elapsed since last call.
 * Spins the cpu otherwise, waiting for a tick to occur.
 */
void waitForNextTick() {
  static system_tick_t prevTicksElapsed = 0;
  while (GetElapsedTicks() == prevTicksElapsed);
  prevTicksElapsed = GetElapsedTicks();
}

/**
 * \brief Main entry point
 * \return Not utilized
 */
int main(void) {
  // Set the clocking to run directly from the crystal.
  SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
                 SYSCTL_XTAL_8MHZ);

  // Initialize the board LCD
  lcd_init();

  // Initialize the hardware timer
  TimerInitDefault();

  // Display a timer to the user forever
  while (1) {
    // Do not run the loop faster than a single hardware tick
    waitForNextTick();

    // determine timer value in seconds
    double seconds =
      (double) GetElapsedTicks() * SYSTEM_TICK_DEFAULT_MS / 1000;

    // display the result to the user
    lcd_printf_at(0, 0, "Time elapsed: ");
    lcd_print_float(seconds, 7, 2);
  }
}

/**
 * \brief Needed to avoid changing the startup file for this test
 */
void Timer0IntHandler(void) {}

#pragma clang diagnostic pop