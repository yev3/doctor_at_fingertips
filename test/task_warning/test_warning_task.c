/*****************************************************************************
 * Project 3, CPSC 5530 Embedded Systems, Seattle University, 2017           *
 * Team "ARM Brewery": Edward Guevara, David Pierce, and Yevgeni Kamenski    *
 *                                                                           *
 * test_warning_task.c - 10/21/2017                                          *
 * Tests ranges of values and checks if LED flashes accordingly              *
 *                                                                           *
 * "THE BEER-WARE LICENSE" (Revision 42):                                    *
 * As long as you retain this notice, you can do whatever you want with this *
 * software. If we meet some day, and you think this software is worth it,   *
 * you can buy us a beer in return. - Team "ARM Brewery"                     *
 *****************************************************************************/

#include <inc/lm3s8962.h>
#include <driverlib/sysctl.h>
#include <utils/hardware_timer.h>
#include "utils/lcd_print.h"
#include "tasks/tasks.h"
#include "tasks/ranges.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_pwm.h"
#include "driverlib/pwm.h"
#include "driverlib/gpio.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
KeyScanData   *keyScanData   = NULL;     ///< Data of the key scan task
EnunciateData *enunciateData = NULL;     ///< Data of the warning task
CorrectedBuffers* buf = NULL;            ///< Enunciate task data

/**
 * \brief Dispatches a single TCB
 * \param tcb TCB to dispatch
 */
void dispatch(TaskName id) {
  TaskControlBlock *const tcb = systemTasks[id];
  tcb->pTask(tcb->pTaskData);
}


/**
 * \brief Does not run if at least one hardware tick elapsed since last call.
 * Spins the cpu otherwise, waiting for a tick to occur.
 */
void dispatchEnunciateWaitForKey() {
  static system_tick_t prevTicksElapsed = 0;
  dispatch(TCB_ENUNCIATE);
  while (1) {
    while (GetElapsedTicks() == prevTicksElapsed);
    prevTicksElapsed = GetElapsedTicks();
    dispatch(TCB_KEYSCAN);
    if (*keyScanData->keyAvailable) return;
  }
}

/**
 * \brief Resets the corrected measurements to the values known to raise no
 * alarm or warning indication.
 * \param data pointer to WarningAlarmData used by the warning module
 */
void resetDataToNormal() {
  buf->pressures[buf->pressureIndex] = 120;         // normal syst pressure val
  buf->pressures[buf->pressureIndex+BUF_SIZE] = 80; // normal diast pressure val
  buf->temperatures[buf->temperatureIndex] = 37.0f; // normal temperature value
  buf->pulseRates[buf->pulseRateIndex] = 80;        // normal pulse rate
  *enunciateData->batteryPercentage = 100;         // normal battery state
}

/**
 * \brief Main entry point of the program
 * \return not utilized
 */
int main(void) {
  // Set the clocking to run directly from the crystal.
  SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
      SYSCTL_XTAL_8MHZ);

  // Initialize all project hardware peripherals, globals, and TCBs
  initVarsAndTasks();

  // Get initialized warning task and it's data
  enunciateData = (EnunciateData *) systemTasks[TCB_ENUNCIATE]->pTaskData;
  buf = enunciateData->correctedBuffers;
  keyScanData = (KeyScanData *) systemTasks[TCB_KEYSCAN]->pTaskData;

  while (true) {
    lcd_printf_at(0, 0, "%21s", "All values normal.");
    lcd_printf_at(1, 0, "%21s", "Expect solid LED0");
    resetDataToNormal();
    dispatchEnunciateWaitForKey();

    lcd_printf_at(0, 0, "%21s", "Systolic Press high.");
    lcd_printf_at(1, 0, "%21s", "Expect 0.5s LED1 lit");
    resetDataToNormal();
    buf->pressures[buf->pressureIndex] = SYS_PRESS_NORMAL_MAX + 1;
    dispatchEnunciateWaitForKey();

    lcd_printf_at(0, 0, "%21s", "Systolic Press >20% !");
    lcd_printf_at(1, 0, "%21s", "Expect 0.5s LED1 lit");
    lcd_printf_at(2, 0, "%21s", "       1.0s alarm");
    resetDataToNormal();
    buf->pressures[buf->pressureIndex] = SYS_PRESS_NORMAL_MAX * 1.3f;
    dispatchEnunciateWaitForKey();

    lcd_printf_at(0, 0, "%21s", "Diastolic Press high.");
    lcd_printf_at(1, 0, "%21s", "Expect 0.5s LED1 lit");
    lcd_printf_at(2, 0, "%21s", "                     ");
    resetDataToNormal();
    buf->pressures[buf->pressureIndex + BUF_SIZE] = SYS_PRESS_NORMAL_MAX + 1;
    dispatchEnunciateWaitForKey();

    lcd_printf_at(0, 0, "%21s", "Temperature high.");
    lcd_printf_at(1, 0, "%21s", "Expect 1.0s LED0 lit");
    resetDataToNormal();
    buf->temperatures[buf->temperatureIndex] = TEMP_NORM_MAX + 1;
    dispatchEnunciateWaitForKey();

    lcd_printf_at(0, 0, "%21s", "Pulse rate low.");
    lcd_printf_at(1, 0, "%21s", "Expect 2.0s LED0 lit");
    resetDataToNormal();
    buf->pulseRates[buf->pulseRateIndex] = PULSE_NORM_MIN - 1;
    dispatchEnunciateWaitForKey();

    lcd_printf_at(0, 0, "%21s", "Battery is low.");
    lcd_printf_at(1, 0, "%21s", "Expect 1.0s LED2 lit");
    resetDataToNormal();
    *enunciateData->batteryPercentage = BATT_WARN_LOW_PCT - 1;
    dispatchEnunciateWaitForKey();
  }
}

/*
 * Needed to compile
 */
void serial_init() {}
void serial_comms(void* x) {}
void ui_controller (void* x) {}
void taskScheduleForExec (TaskName x) {}
void taskSuspend (TaskName x) {}


#pragma clang diagnostic pop
