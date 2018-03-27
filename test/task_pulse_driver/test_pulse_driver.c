/*****************************************************************************
 * Project 3, CPSC 5530 Embedded Systems, Seattle University, 2017           *
 * Team "ARM Brewery": Edward Guevara, David Pierce, and Yevgeni Kamenski    *
 *                                                                           *
 * test_pulse_driver.c - 9/30/2017                                           *
 * Verification of correct operation of the measure task                     *
 *                                                                           *
 * "THE BEER-WARE LICENSE" (Revision 42):                                    *
 * As long as you retain this notice, you can do whatever you want with this *
 * software. If we meet some day, and you think this software is worth it,   *
 * you can buy us a beer in return. - Team "ARM Brewery"                     *
 *****************************************************************************/

#include "tasks/tasks.h"
#include "utils/lcd_print.h"
#include "utils/led_control.h"
#include <drivers/pulse_transducer.h>


#define PULSE_MEASURE_SAMPLES 5     /* Number of samples to measure */

static uint rawPulseMeasurement = 0;
static uint rawSamplesMeasured = 0;
static bool isMeasureComplete = false;


/**
 * \brief Dispatches a single TCB
 * \param tcb TCB to dispatch
 */
void dispatch(TaskName id) {
  TaskControlBlock *const tcb = systemTasks[id];
  tcb->pTask(tcb->pTaskData);
}

void* getDataFor(TaskName id) {
  return systemTasks[id]->pTaskData;
}

void ledFlash(int ms) {
  static bool isOn = false;
  isOn = !isOn;
  if (isOn) {
    led0_on();
  } else {
    led0_off();
  }
}

void pulseMeasureHook(int ms) {
  rawPulseMeasurement += ms;
  if (++rawSamplesMeasured == PULSE_MEASURE_SAMPLES) {
    isMeasureComplete = true;
    pulseHookClear();
  }
}

void startPulseMeasurement() {
  rawPulseMeasurement = 0;
  rawSamplesMeasured = 0;
  isMeasureComplete = false;
  pulseHookSet(pulseMeasureHook);
}

/**
 * \brief computes the pulse rate from raw measurement
 * \param rawMs number of milliseconds elapsed in 5 successive beats
 * \return corrected pulse rate in beats ber pecond
 */
float computePulseMeasurement(uint rawMs) {
  return (60000 * PULSE_MEASURE_SAMPLES) / (float)rawMs;
}



/**
 * \brief Main entry point of the program
 * \return not utilized
 */
int main(void) {
  // Set the clocking to run directly from the crystal.
  SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
                 SYSCTL_XTAL_8MHZ);

  initVarsAndTasks();
  KeyScanData *keys = getDataFor(TCB_KEYSCAN);
  
  pulseHookSet(ledFlash);

  while (1) {
    lcd_clear();

    // wait for user to press select key
    lcd_printf_at(0, 0, "Press select to start");

    while (!*keys->keyPressedSelect) {
      dispatch(TCB_KEYSCAN);
    }

    lcd_printf_at(1, 0, "Measuring..");

    startPulseMeasurement();

    while (!isMeasureComplete);

    lcd_printf_at(2, 0, "Raw: %16u", rawPulseMeasurement);

    float corrected = computePulseMeasurement(rawPulseMeasurement);

    lcd_printf_at(3, 0, "Corrected: ");
    lcd_print_float(corrected, 9, 1);

    pulseHookSet(ledFlash);
    dispatch(TCB_KEYSCAN);
    while (!*keys->keyPressedSelect) {
      dispatch(TCB_KEYSCAN);
    }
  }

}
