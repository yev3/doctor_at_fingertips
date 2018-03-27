/*****************************************************************************
 * Project 3, CPSC 5530 Embedded Systems, Seattle University, 2017           *
 * Team "ARM Brewery": Edward Guevara, David Pierce, and Yevgeni Kamenski    *
 *                                                                           *
 * test_compute_task.c - 9/30/2017                                           *
 * Verifies the functionality of the compute module                          *
 *                                                                           *
 * "THE BEER-WARE LICENSE" (Revision 42):                                    *
 * As long as you retain this notice, you can do whatever you want with this *
 * software. If we meet some day, and you think this software is worth it,   *
 * you can buy us a beer in return. - Team "ARM Brewery"                     *
 *****************************************************************************/
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

#include <utils/lcd_print.h>
#include "driverlib/sysctl.h"
#include "tasks/tasks.h"

#include <inc/lm3s8962.h>
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "tasks/tasks.h"
#include "utils/lcd_print.h"

// =====================
// Selection: Pressure
// Selection: Temprature
// Selection: Heart Rate
// Syst Dias  Temp  Plse
// ###< ###< ###.#< ###<
// ###< ###< ###.#< ###<
// ###< ###< ###.#< ###<
// ###< ###< ###.#< ###<
// ###< ###< ###.#< ###<
// ###< ###< ###.#< ###<
// ###< ###< ###.#< ###<
// ###< ###< ###.#< ###<

const char *getSelectionStr(MeasureSelection selection) {
  switch (selection) {
    case MEASURE_PRESSURE :return "Pressure";
    case MEASURE_TEMPERATURE : return "Temprature";
    case MEASURE_PULSE: return "Heart Rate";
    default: return "ERROR";
  }
}

/**
 * \brief Prints the raw data values of the measure task
 */
void printCorrectedVals(ComputeData *data) {
  const char *selectionStr = getSelectionStr(*data->measurementSelection);
  lcd_printf_at(0, 0, "Selection: %10s", selectionStr);
  lcd_printf_at(2, 0, "Syst Dias  Temp  Plse");

  const CorrectedBuffers *const buf = data->correctedBuffers;
  for (int i = 0; i < BUF_SIZE; ++i) {
    lcd_set_cursor(3 + i, 0);
    // Systolic
    lcd_printf("%3d", (int) buf->pressures[i]);
    lcd_print(buf->pressureIndex == i ? "< " : "  ");
    // Diastolic
    lcd_printf("%3d", (int) buf->pressures[i + BUF_SIZE]);
    lcd_print(buf->pressureIndex == i ? "< " : "  ");
    // Temperature
    lcd_print_float(buf->temperatures[i], 5, 1);
    lcd_print(buf->temperatureIndex == i ? "< " : "  ");
    // Pulse Rate
    lcd_printf("%3d", (int) buf->pulseRates[i]);
    lcd_print(buf->pulseRateIndex == i ? "< " : "  ");
  }
}

/**
 * \brief Dispatches a single TCB
 * \param tcb TCB to dispatch
 */
void dispatch(TaskName id) {
  TaskControlBlock *const tcb = systemTasks[id];
  tcb->pTask(tcb->pTaskData);
}


void selectionController(KeyScanData *pKeys, ComputeData *pCompData) {
  if (*pKeys->keyAvailable) {
    if (*pKeys->keyPressedDown) {
      MeasureSelection *sel = pCompData->measurementSelection;
      *sel = (*sel << 1);
      if(MEASURE_END == *sel) *sel = MEASURE_BEGIN;
    }
  }

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
  ComputeData *compData = (ComputeData *) systemTasks[TCB_COMPUTE]->pTaskData;
  KeyScanData *keys = (KeyScanData *) systemTasks[TCB_KEYSCAN]->pTaskData;

  while (true) {
    // Display the Measure raw data to the user
    printCorrectedVals(compData);

//    waitTicks(1);

    dispatch(TCB_KEYSCAN);
    selectionController(keys, compData);
    dispatch(TCB_MEASURE);
    dispatch(TCB_COMPUTE);

  }
}

#pragma clang diagnostic pop