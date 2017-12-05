/*****************************************************************************
 * Project 3 CPSC 5530 Embedded Systems, Seattle University, 2017            *
 * Team "ARM Brewery": Edward Guevara, David Pierce, and Yevgeni Kamenski    *
 *                                                                           *
 * controller.c - 10/18/2017                                                 *
 * Responsible for the state of the user interface, dispatching tasks as
 * needed, and controlling the display characteristics
 *                                                                           *
 * "THE BEER-WARE LICENSE" (Revision 42):                                    *
 * As long as you retain this notice, you can do whatever you want with this *
 * software. If we meet some day, and you think this software is worth it,   *
 * you can buy us a beer in return. - Team "ARM Brewery"                     *
 *****************************************************************************/

#include <utils/lcd_print.h>
#include "tasks/system.h"
#include "tasks/commands.h"
#include "FreeRTOS_IP.h"

#define NSCROLL 3   /* Number of items available in the menu */

/**
 * \brief Processes key presses or other user input and takes appropriate
 * actions according to the current system state.
 * \param rawData raw data passed by the task scheduler
 */
void uiControl(void *rawData) {
  ControllerData *data = (ControllerData *) rawData;
  QueueHandle_t keyPresses = data->keyPressQueueHandle;
  DispViewModel_t *ui = data->viewModel;
  KeyPress_t key;

  for(;;) {
    if (xQueueReceive(keyPresses, &key, portMAX_DELAY) == pdPASS) {
      // Silence the alarm when the user presses the silence key
      if (cmdKEY_LEFT == key) {
        *data->auralAlarmSilenced = true;
        taskScheduleForExec(sysTCB_ENUNCIATE);
      }

      // Mode selection is toggled by the right key
      if (cmdKEY_RIGHT == key) {
        ui->mode = (MENU_DISP_MODE == ui->mode)
                          ? ENUNCIATE_DISP_MODE
                          : MENU_DISP_MODE;
      }

      if (MENU_DISP_MODE == ui->mode) {
        // Menu mode

        // Scroll up when the up key is pressed
        if (cmdKEY_UP == key && (ui->scrollPosn > 0)) {
          --(ui->scrollPosn);
        }

        // Scroll down when the down key is pressed
        if (cmdKEY_DOWN == key && (ui->scrollPosn < (NSCROLL - 1))) {
          ++(ui->scrollPosn);
        }

        if (cmdKEY_SELECT == key) {
          switch (ui->scrollPosn) {
          case 0:
            *data->measureSelect = MEASURE_PRESSURE;
            break;
          case 1:
            *data->measureSelect = MEASURE_TEMPERATURE;
            break;
          case 2:
            *data->measureSelect = MEASURE_PULSE;
            break;
          default:
            break;
          }

          // place into enunciate mode after measurement was selected
          ui->mode = ENUNCIATE_DISP_MODE;

          //schedule measure task when a measurement is chosen
          taskScheduleForExec(sysTCB_MEASURE);
        }

      } else {
        // Enunciate mode
      }

      // User pressed a key, so update the display
      taskScheduleForExec(sysTCB_DISPLAY);

    } else {
      FreeRTOS_debug_printf(("Unable to read from the key queue, even after blocking"));
    }
  }
}
