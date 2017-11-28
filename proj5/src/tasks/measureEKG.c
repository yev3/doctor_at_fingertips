////////////////////////////////////////////////////////////////////////////////
// Project 3, CPSC 5530 Embedded Systems, Seattle University
// Team "ARM Brewery"
// Edward Guevara, David Pierce, and Yevgeni Kamenski
//
// measureEKG.c - 10/21/2017 4:00 PM
// Responsible for measuring EKG measurements using a simulation
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
#include <math.h>
#include "tasks/system.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
#define PI 3.1417f

static float ekgMeasurement = 0;
static ulong ticks = 0;


/**
 * \brief Handler for simulated EKG measurements. It uses a sinusoidal sensor output.
 */
void Timer2IntHandler() {
  TimerIntClear(TIMER2_BASE, TIMER_TIMA_TIMEOUT);
  ticks ++;

}
/**
 * \brief Initialization of EKG measurement timer interrupt.
 */
void measure_ekg_init() {
  // Get the number of system time cycles (not ticks) for one sys tick
  ulong sysTickPeriod = SysCtlClockGet();

  // Enable the timer peripheral
  SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2);

  // Enable processor interrupts.
  IntMasterEnable();

  // Configure 32-bit periodic timer.
  TimerConfigure(TIMER2_BASE,  TIMER_CFG_32_BIT_PER);
  TimerLoadSet(TIMER2_BASE, TIMER_A, sysTickPeriod);

  // Load the next timer period
  Timer2IntHandler();

  // Setup the interrupt for the timer timeout.
  IntEnable(INT_TIMER2A);
  TimerIntEnable(TIMER2_BASE, TIMER_TIMA_TIMEOUT);

  // Enable the timer.
  TimerEnable(TIMER2_BASE, TIMER_A);
}

/**
 * \brief The MeasureEKG task will measure the current EKG measurement that is
 * simulated in the Timer1IntHandler function (above)
 * \param rawData raw data passed by the task scheduler
 */

void measureEKG(void* rawData) {
  for (;;) {
    MeasureEKGData *data = (MeasureEKGData *) rawData;
    EKGBuffer *ekgBuffer = data->ekgBuffer;
    float w = 2 * PI * 1000;
    float t = 0;
    for (uint i = 0; i < BUF_SIZE_EKG; i++) {
      ekgBuffer->ekgMeasures[i] = (int)(30.0*sin(w*t));
      t += 0.000125;
    }
    *data->completedEKGMeasure = true;
    // when done measuring, call ComputeEKG to get frequency
    if(*data->completedEKGMeasure) {
      taskScheduleForExec(sysTCB_COMP_EKG);
      vTaskSuspend(NULL);
    }
  }
}
#pragma clang diagnostic pop