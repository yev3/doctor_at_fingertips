////////////////////////////////////////////////////////////////////////////////
// Project 3, CPSC 5530 Embedded Systems, Seattle University
// Team "ARM Brewery"
// Edward Guevara, David Pierce, and Yevgeni Kamenski
//
// tasks.c - 10/21/2017 4:06 PM
// Responsible for setting up the global variable sharing between tasks.
// Declares initialization functions for Task Control Blocks used by the scheduler.
//
// This is free and unencumbered software released into the public domain.
////////////////////////////////////////////////////////////////////////////////

#include "drivers/pressure_cuff.h"

bool measureSystolic = false;   ///< Flag to measure systolic
bool measureDiastolic = false;   ///< Flag to measure diastolic
bool beginPressureMeasurement = false;   ///< Flag to begin measuring pressure
static float currentPressure = PRESSURE_MIN;  ///< initial pressure
/**
 * \brief timer that constantly checks the simulated pressure of the cuff
 */
void PressureCuffHandler(void)
{
  // Clear the timer interrupt.
  TimerIntClear(TIMER2_BASE, TIMER_TIMA_TIMEOUT);

  //compute and check if pressure is within Systolic measurement range
  float pressure = currentPressure * 2.0 + 9.0;
  if(pressure >110.0 && pressure < 150.0) measureSystolic = true;
  else measureSystolic = false;

  //compute and check if pressure is within Diastolic measurement range
  pressure = currentPressure * 1.5 + 6;
  if(pressure > 50.0 && pressure < 80.0) measureDiastolic = true;
  else measureDiastolic = false;
}

/**
 * \brief Initializes the pulse measurement driver
 */
void pressure_cuff_init() {
  // 5ms intervals to check the pressure of the cuff
  ulong clockPerSec = SysCtlClockGet() / 1000 * PRESSURE_TIMER_DEFAULT;

  // Enable the timer peripheral
  SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2);

  // Enable processor interrupts.
  IntMasterEnable();

  // Configure 32-bit periodic timer.
  TimerConfigure(TIMER2_BASE,  TIMER_CFG_32_BIT_PER);
  TimerLoadSet(TIMER2_BASE, TIMER_A, clockPerSec);

  // Setup the interrupt for the timer timeout.
  IntEnable(INT_TIMER2A);
  TimerIntEnable(TIMER2_BASE, TIMER_TIMA_TIMEOUT);

  // Enable the timer.
  TimerEnable(TIMER2_BASE, TIMER_A);
}

/**
 * \brief Increases the pressure of the cuff by default percent step
 */
void IncreasePressure(){
  // increase pressure by percent step
  currentPressure += currentPressure * PRESSURE_PERCENT_STEP;

  //check if pressure is at maximum
  if(PRESSURE_MAX < currentPressure) currentPressure = PRESSURE_MAX;
  beginPressureMeasurement = false;
}

/**
 * \brief Decreases the pressure of the cuff by default percent step
 */
void DecreasePressure(){
  currentPressure -= currentPressure * PRESSURE_PERCENT_STEP;
  //measure when pressure is decreasing
  beginPressureMeasurement = true;

  // check if pressure is at lowest
  if(PRESSURE_MIN > currentPressure) {
    currentPressure = PRESSURE_MIN;
    // do not measure if Pressure is at minimum
    beginPressureMeasurement = false;
  }
}

/**
 * \brief Resets pressure to minimum pressure
 */
void ResetPressure() {
  currentPressure = PRESSURE_MIN;
}

/**
 * \brief Get current pressure of the cuff
 */
float *GetPressure() { return &currentPressure; }