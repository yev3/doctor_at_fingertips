/*****************************************************************************
 * Project 5, CPSC 5530 Embedded Systems, Seattle University, 2017           *
 * Team "ARM Brewery": Edward Guevara, David Pierce, and Yevgeni Kamenski    *
 *                                                                           *
 * pulse_transducer.h - 9/30/2017                                            *
 * Declares the pulse driver init routines and functions for setting up pulse
 * measurement interrupts.
 *                                                                           *
 * "THE BEER-WARE LICENSE" (Revision 42):                                    *
 * As long as you retain this notice, you can do whatever you want with this *
 * software. If we meet some day, and you think this software is worth it,   *
 * you can buy us a beer in return. - Team "ARM Brewery"                     *
 *****************************************************************************/
#pragma once

#include <inc/hw_ints.h>
#include <inc/hw_memmap.h>
#include <inc/hw_types.h>
#include <driverlib/interrupt.h>
#include <driverlib/sysctl.h>
#include <driverlib/timer.h>
#include "utils/hardware_timer.h"

#define PULSE_TRANSDUCER_MIN 10     /* In beats per minute */
#define PULSE_TRANSDUCER_MAX 200    /* In beats per minute */
#define PULSE_TRANSDUCER_STEP 3     /* In beats per minute */

#define PULSE_MEASURE_SAMPLES 5     /* Number of samples to measure */

/**
 * \brief Signature of the function that gets called on every pulse.
 * This simulates an interrupt that will be implemented with actual
 * hardware
 */
typedef void (*pulseIntHook_t)(int);

/**
 * \brief Initializes the pulse measurement driver
 */
void pulse_init();

/**
 * \brief Sets the function to be called on every timer interrupt
 * \param hook the function that is called when a pulse occurs
 */
void pulseHookSet(pulseIntHook_t hook);

/**
 * \brief Clears the interrupt function. Simulates disabling the pulse
 * measurement interrupt.
 */
void pulseHookClear();