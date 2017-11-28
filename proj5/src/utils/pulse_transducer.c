/*****************************************************************************
 * Project 3, CPSC 5530 Embedded Systems, Seattle University, 2017           *
 * Team "ARM Brewery": Edward Guevara, David Pierce, and Yevgeni Kamenski    *
 *                                                                           *
 * pulse_transducer.c - 9/30/2017                                            *
 * Verify that the LEDs are all working properly                             *
 *                                                                           *
 * "THE BEER-WARE LICENSE" (Revision 42):                                    *
 * As long as you retain this notice, you can do whatever you want with this *
 * software. If we meet some day, and you think this software is worth it,   *
 * you can buy us a beer in return. - Team "ARM Brewery"                     *
 *****************************************************************************/

#include <drivers/pulse_transducer.h>
#include "utils/float_print.h"

#define PULSE_TIMER_PRESCALE (0x10)           /* Prescale for timer */
static ulong clockPerSec = 0;                 ///< CPU clk used when converting
static volatile bool isIncrementing = true;   ///< When T, counting up
static pulseIntHook_t timerHook = NULL;       ///< Simulated pulse interrupt

/**
 * \brief current pulse transducer heart rate value
 */
static volatile int bpm = (PULSE_TRANSDUCER_MIN + PULSE_TRANSDUCER_MAX) / 2;

/**
 * \brief Helper that calculates the next timer period in ms, used to simulate
 * the pulse transducer driver signals
 * \return next timer period in ms
 */
static inline int getNextTimerPeriodMs() {
  // number of milliseconds for the next timer period
  int periodMs = 60 * 1000 / bpm;

  // update the bpm for the next call
  if (isIncrementing) {
    bpm += PULSE_TRANSDUCER_STEP;
  } else {
    bpm -= PULSE_TRANSDUCER_STEP;
  }

  // update incr or decr or next call
  if (bpm >= PULSE_TRANSDUCER_MAX) {
    isIncrementing = false;
  } else if (bpm <= PULSE_TRANSDUCER_MIN) {
    isIncrementing = true;
  }
  
  return periodMs;
}

/**
 * \brief Hardware timer that simulates the heart rate transducer
 */
void Timer0IntHandler(void)
{
  static int expirationsLeft = 1;

  // Clear the timer interrupt.
  TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

  if (0 == (--expirationsLeft)) {
    // Set the duration of the next timer interval
    int periodMs = getNextTimerPeriodMs();

    // Convert ms to CPU cycles
    ullong totalCycles = ((ullong)clockPerSec * periodMs) / 1000ull;
    ulong partialCycles = (ulong) (totalCycles / PULSE_TIMER_PRESCALE);

    TimerLoadSet(TIMER0_BASE, TIMER_A, partialCycles);
    expirationsLeft = PULSE_TIMER_PRESCALE;

    if (timerHook) timerHook(periodMs);
  }
}

/**
 * \brief Initializes the pulse measurement driver
 */
void pulse_init() {
  // Save the timer ticks per second for later calcs of the BPM
  clockPerSec = SysCtlClockGet();

  // Enable the timer peripheral
  SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);

  // Enable processor interrupts.
  IntMasterEnable();

  // Configure 32-bit periodic timer.
  TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);

  // Load the next timer period
  Timer0IntHandler();

  // Setup the interrupt for the timer timeout.
  IntEnable(INT_TIMER0A);
  TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

  // Enable the timer.
  TimerEnable(TIMER0_BASE, TIMER_A);
}

/**
 * \brief Sets the function to be called on every timer interrupt
 * \param hook the function that is called when a pulse occurs
 */
void pulseHookSet(pulseIntHook_t hook) {
  timerHook = hook;
}

/**
 * \brief Clears the interrupt function. Simulates disabling the pulse
 * measurement interrupt.
 */
void pulseHookClear() {
  timerHook = NULL;
}