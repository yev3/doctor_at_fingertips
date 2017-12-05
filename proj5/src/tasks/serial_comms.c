/*****************************************************************************
 * Project 3, CPSC 5530 Embedded Systems, Seattle University, 2017           *
 * Team "ARM Brewery": Edward Guevara, David Pierce, and Yevgeni Kamenski    *
 *                                                                           *
 * serial_comms.c - 10/14/2017                                               *
 * Prints out the alarm information to the user's PC terminal
 * using a serial connection
 *                                                                           *
 * "THE BEER-WARE LICENSE" (Revision 42):                                    *
 * As long as you retain this notice, you can do whatever you want with this *
 * software. If we meet some day, and you think this software is worth it,   *
 * you can buy us a beer in return. - Team "ARM Brewery"                     *
 *****************************************************************************/

#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include <driverlib/gpio.h>
#include <driverlib/sysctl.h>
#include "utils/uartstdio.h"
#include "utils/float_print.h"
#include "tasks/system.h"


/**
 * \brief Outputs the state of the warning/alarm to the serial
 * \param rawData raw data passed by the task scheduler
 */
void serial_comms(void *rawData) {
  // TCB data and buffer
  SerialCommData *data = (SerialCommData *) rawData;
  CorrectedBuffers *buf = (CorrectedBuffers *) data->correctedBuffers;

#define OUT_BUF_N 21
  static char outBuf[OUT_BUF_N];

  for (;;) {
    UARTprintf("\nA NEW WARNING HAS OCCURRED IN THE SYSTEM\n");
    UARTwrite("+---------------------+\n", 24);

    UARTprintf("|  Syst / Diast       |\n|");
    int n = sprint_float(outBuf, OUT_BUF_N,
                         buf->pressures[buf->pressureIndex], 4, 0);
    UARTwrite(outBuf, (ulong) n);
    UARTprintf("   ");
    n = sprint_float(outBuf, OUT_BUF_N,
                     buf->pressures[buf->pressureIndex + BUF_SIZE], 5, 0);
    UARTwrite(outBuf, (ulong) n);
    UARTprintf("   mmHg  |\n|");

    // Display temperature information on the LCD
    UARTprintf("Temperature ");
    n = sprint_float(outBuf, OUT_BUF_N,
                     buf->temperatures[buf->temperatureIndex], 5, 1);
    UARTwrite(outBuf, (ulong) n);
    UARTprintf(" *C |\n|");

    // Display pulse rate information on the LCD
    UARTprintf("Pulse rate  ");
    n = sprint_float(outBuf, OUT_BUF_N,
                     buf->pulseRates[buf->pulseRateIndex], 5, 1);
    UARTwrite(outBuf, (ulong) n);
    UARTprintf("    |\n|");

    // Display battery state information on the LCD
    UARTprintf("Battery       %3d ", (int) *data->batteryPercentage);
    UARTwrite("%  |\n", 6);
    UARTwrite("+---------------------+\n", 24);

    WarningAlarmStates *alarms = data->warningAlarmStates;
#define D(X) (alarms->##X)
#define B(X) ((D(X)) ? "True" : "False")  /* FOR READABILITY */

    UARTprintf("-- OUT OF RANGE FLAGS --\n");
    UARTprintf("Blood pressure: %5s\n", B(bpOutOfRange));
    UARTprintf("Temperature:    %5s\n", B(tempOutOfRange));
    UARTprintf("Pulse rate:     %5s\n", B(pulseOutOfRange));

    UARTprintf("-- ALARM FLAGS --\n");
    UARTprintf("Blood pressure high: %5s\n", B(bpHighAlarm));
    UARTprintf("Temperature high:    %5s\n", B(tempHighAlarm));
    UARTprintf("Pulse rate low:      %5s\n", B(pulseLowAlarm));
    UARTprintf("Battery low:         %5s\n", B(battLowAlarm));
    UARTprintf("\n");

#undef B
#undef D

    // Prevent this task from running again. It is scheduled as needed.
    taskSuspend(sysTCB_SERIAL);
//    vTaskDelay(pdMS_TO_TICKS(1));
  }
}