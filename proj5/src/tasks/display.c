////////////////////////////////////////////////////////////////////////////////
// Project 3, CPSC 5530 Embedded Systems, Seattle University
// Team "ARM Brewery" 
// Edward Guevara, David Pierce, and Yevgeni Kamenski
// 
// display.c - 09/26/2017 5:32 PM
// The purpose of this file is to display the measurements to the user's LCD
// in the format described below:
// ╔═════════════════════╗
// ║ Syst / Diast        ║ 0
// ║ ####   #####   mmHg ║ 1
// ║                     ║ 2
// ║ Temperature ###.# *C║ 3
// ║ Pulse rate  ###.#   ║ 4
// ║ Battery     ###   % ║ 5
// ║                     ║ 6
// ║                     ║ 7
// ║                     ║ 8
// ║                     ║ 9
// ║                     ║ 10
// ║                     ║ 11
// ╚═════════════════════╝
//
// This is free and unencumbered software released into the public domain.
////////////////////////////////////////////////////////////////////////////////

#include "tasks/system.h"
#include "utils/lcd_print.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
#define MENU_LINE1 " MEASUREMENT SELECT  "
#define DISP_LINE2 "---------------------"
#define MENU_LEADR ">>"
#define MENU_LINE3 "   Blood Pressure    "
#define MENU_LINE4 "   Temperature       "
#define MENU_LINE5 "   Heart Rate        "

#define ENUN_LINE1 "    DEVICE STATUS    "

/**
 * \brief Display the measurement values to the user on the LCD
 * \param rawData raw data ptr passed by the task dispatcher
 */
void display(void *rawData) {
  static DisplayMode lastMode = (DisplayMode) -1;   ///< Last display mode
  static int lastScroll = -1;                       ///< Last scroll posn

  // TCB data and buffer
  DisplayViewModel *data = (DisplayViewModel *) rawData;
  CorrectedBuffers *buf = (CorrectedBuffers *) data->correctedBuffers;

  for(;;) {
    // clear the screen if the mode has changed
    if (lastMode != *data->mode) {
      lcd_clear();
      lastScroll = -1;
      lastMode = *data->mode;
    }

    int lineno = 0; ///! Current line number being displayed

    if (MENU_DISP_MODE == *data->mode) {
      if (lastScroll != *data->scrollPosn) {
        lastScroll = *data->scrollPosn;
        lcd_printf_at(lineno++, 0, MENU_LINE1);
        lcd_printf_at(lineno++, 0, DISP_LINE2);
        lcd_printf_at(lineno++, 0, MENU_LINE3);
        lcd_printf_at(lineno++, 0, MENU_LINE4);
        lcd_printf_at(lineno++, 0, MENU_LINE5);
        lcd_printf_at(2 + *data->scrollPosn, 0, MENU_LEADR);
      }
    } else {
      // Enunciate mode
      lcd_printf_at(lineno++, 0, ENUN_LINE1);
      lcd_printf_at(lineno++, 0, DISP_LINE2);

      // Display blood presure information on the LCD
      lcd_set_cursor(lineno++, 1);
      lcd_printf("Syst / Diast");
      lcd_set_cursor(lineno++, 1);
      lcd_print_float(buf->pressures[buf->pressureIndex], 4, 0);
      lcd_printf("   ");
      lcd_print_float(buf->pressures[buf->pressureIndex + BUF_SIZE], 5, 0);
      lcd_printf("   mmHg");

      // Display temperature information on the LCD
      lcd_set_cursor(lineno++, 1);
      lcd_printf("Temperature ");
      lcd_print_float(buf->temperatures[buf->temperatureIndex], 5, 1);
      lcd_printf(" *C");

      // Display pulse rate information on the LCD
      lcd_set_cursor(lineno++, 1);
      lcd_printf("Pulse rate  ");
      lcd_print_float(buf->pulseRates[buf->pulseRateIndex], 5, 1);

      // Display battery state information on the LCD
      lcd_set_cursor(lineno++, 1);
      lcd_printf("Battery       %3d ", (int) *data->batteryPercentage);
      lcd_print("%");
    }

    vTaskSuspend(NULL);
  }
}
#pragma clang diagnostic pop