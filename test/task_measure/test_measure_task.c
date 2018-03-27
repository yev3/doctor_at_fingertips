/*****************************************************************************
 * Project 3, CPSC 5530 Embedded Systems, Seattle University, 2017           *
 * Team "ARM Brewery": Edward Guevara, David Pierce, and Yevgeni Kamenski    *
 *                                                                           *
 * test_measure_task.c - 9/30/2017                                           *
 * Verification of correct operation of the measure task                     *
 *                                                                           *
 * "THE BEER-WARE LICENSE" (Revision 42):                                    *
 * As long as you retain this notice, you can do whatever you want with this *
 * software. If we meet some day, and you think this software is worth it,   *
 * you can buy us a beer in return. - Team "ARM Brewery"                     *
 *****************************************************************************/
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

// =====================
// Selection: Pressure
// Selection: Temprature
// Selection: Heart Rate
// Measuring:  No
//  Syst Dias Temp Pulse
//  ###< ###< ###< ###<
//  ###< ###< ###< ###<
//  ###< ###< ###< ###<
//  ###< ###< ###< ###<
//  ###< ###< ###< ###<
//  ###< ###< ###< ###<
//  ###< ###< ###< ###<
//  ###< ###< ###< ###<

#include <inc/lm3s8962.h>
#include <utils/hardware_timer.h>
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "tasks/tasks.h"
#include "utils/lcd_print.h"

/*
 * User's current selection
 */
static MeasureSelection userPick = MEASURE_BEGIN;

/**
 * \brief Dispatches a single TCB
 * \param tcb TCB to dispatch
 */
void dispatch(TaskName id) {
  TaskControlBlock *const tcb = systemTasks[id];
  tcb->pTask(tcb->pTaskData);
}

/**
 * \brief String representing the measurement type
 * \param sel measurement type
 * \return string of measure type
 */
const char *getSelectionStr(MeasureSelection sel) {
  switch (userPick) {
    case MEASURE_NONE: return "None";
    case MEASURE_PRESSURE :return "Pressure";
    case MEASURE_TEMPERATURE : return "Temprature";
    case MEASURE_PULSE: return "Heart Rate";
    default: return "ERROR";
  }
}

/**
 * \brief Prints the raw data values of the measure task
 */
void printRawDataValues(MeasureData *data) {
  lcd_printf_at(0, 0, "Selection: %10s", getSelectionStr(userPick));
  lcd_printf_at(1, 0, "Measuring: %10s",
                getSelectionStr(*data->measurementSelection));
  lcd_printf_at(2, 0, " Syst Dias Temp Pulse");

  const RawBuffers *const buf = data->rawBuffers;
  for (int i = 0; i < BUF_SIZE; ++i) {
    lcd_set_cursor(3 + i, 0);
    // Systolic
    lcd_printf(" %3d", buf->pressures[i]);
    lcd_print(buf->pressureIndex == i ? "< " : "  ");
    // Diastolic
    lcd_printf("%3d", buf->pressures[i + BUF_SIZE]);
    lcd_print(buf->pressureIndex == i ? "< " : "  ");
    // Temperature
    lcd_printf("%3d", buf->temperatures[i]);
    lcd_print(buf->temperatureIndex == i ? "< " : "  ");
    // Pulse Rate
    lcd_printf("%3d", buf->pulseRates[i]);
    lcd_print(buf->pulseRateIndex == i ? "< " : "  ");
  }
}

/**
 * \brief Process user selection
 * \param pKeys user's pressed keys
 * \param pMeasureData where to store the user's selection
 */
void selectionController(KeyScanData *pKeys, MeasureData *pMeasureData) {
  if (*pKeys->keyAvailable) {
    if (*pKeys->keyPressedDown) {
      userPick = (userPick << 1);
      if (MEASURE_END == userPick) {
        userPick = MEASURE_BEGIN;
      }
    }
    if (*pKeys->keyPressedSelect) {
      *pMeasureData->measurementSelection = userPick;
    }
  }
}

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
 * \brief Main entry point of the program
 * \return not utilized
 */
int main(void) {
  // Set the clocking to run directly from the crystal.
  SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
                 SYSCTL_XTAL_8MHZ);

  // Initialize all project hardware peripherals, globals, and TCBs
  initVarsAndTasks();

  // Get pointers to the key scanner and measure data
  MeasureData
    *measureData = (MeasureData *) systemTasks[TCB_MEASURE]->pTaskData;
  KeyScanData *keys = (KeyScanData *) systemTasks[TCB_KEYSCAN]->pTaskData;

  while (true) {
    // Do not run the loop faster than a single hardware tick
    waitForNextTick();

    // Display the Measure raw data to the user
    printRawDataValues(measureData);

    // Read keys
    dispatch(TCB_KEYSCAN);

    // Determine action based on user input
    selectionController(keys, measureData);

    // Call the measure task
    dispatch(TCB_MEASURE);
  }
}

#pragma clang diagnostic pop