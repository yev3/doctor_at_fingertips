/*****************************************************************************
 *                                                                           *
 * Project 5, CPSC 5530 Embedded Systems, Seattle University, 2017           *
 * Team "ARM Brewery": Edward Guevara, David Pierce, and Yevgeni Kamenski    *
 *                                                                           *
 * cmd_dispatch.c - 11/26/2017                                               *
 *                                                                           *
 * "THE BEER-WARE LICENSE" (Revision 42):                                    *
 * As long as you retain this notice, you can do whatever you want with this *
 * software. If we meet some day, and you think this software is worth it,   *
 * you can buy us a beer in return. - Team "ARM Brewery"                     *
 *                                                                           *
 *****************************************************************************/


#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "FreeRTOS_IP.h"
#include "tasks/system.h"
#include "tasks/commands.h"
#include "utils/lcd_print.h"

static QueueHandle_t cmdQueue = NULL;

bool cmdEnqueue(SysCommandsEnum_t cmd_, void *arg_) {
  SysCommand_t sysCmd = (SysCommand_t) {
    .cmd = cmd_,
    .arg = arg_
  };

  return (pdPASS == xQueueSend(cmdQueue, &sysCmd, (TickType_t) 0));
}

void cmdDispatch(void *pvParameters) {
  cmdQueue = *(QueueHandle_t*)pvParameters;
  SysCommand_t receivedCmd;
  BaseType_t xStatus;

  FreeRTOS_debug_printf(("Dispatch task running..\n"));

  for (;;) {

    xStatus = xQueueReceive(cmdQueue, &receivedCmd, portMAX_DELAY);
    if (xStatus == pdPASS) {
      switch (receivedCmd.cmd) {

        // Resets the network connection by re-initializing the IP stack
        case SCMD_RESET_NETWORK: {
          FreeRTOS_debug_printf(("Network Reset\n"));
          network_reset();
          break;
        }

        // Start the measuring tasks. 
        // They listen for measuring commands in the queue
        case SCMD_START:FreeRTOS_debug_printf(("Start\n"));
        {
          taskScheduleForExec(sysTCB_MEASURE);
          taskScheduleForExec(sysTCB_MEAS_EKG);
          break;
          
        }

        // Suspend the measuring tasks. they are not able to listen 
        // for measuring commands from the user's UI
        case SCMD_STOP: {
          FreeRTOS_debug_printf(("Stop\n"));
          taskSuspend(sysTCB_MEASURE);
          taskSuspend(sysTCB_MEAS_EKG);
          break;
        }
        
        // Turns the display on or off
        case SCMD_DISPLAY_EN: {
          SCDisplayArgsEnum_t arg = (SCDisplayArgsEnum_t)(BaseType_t) receivedCmd.arg;
          FreeRTOS_debug_printf(("Display=%d\n", arg));
          switch (arg) {
            case SC_ARG_DISP_ON: {
              taskScheduleForExec(sysTCB_DISPLAY);
              break;
            }

            case SC_ARG_DISP_OFF: {
              taskSuspend(sysTCB_DISPLAY);
              lcd_clear();
              break;
            }

            default: break;
          }
          break;
        }

        default: {
          FreeRTOS_debug_printf(("Unknown command received, can't dispatch"));
          break;
        }
      }

    } else {
      FreeRTOS_debug_printf(("Error receiving the command to dispatch\n"));
    }
  }
}