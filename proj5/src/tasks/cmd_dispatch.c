/*****************************************************************************
 *                                                                           *
 * Project ?, CPSC 5530 Embedded Systems, Seattle University, 2017           *
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



#if _CLION_ == 1
//typedef unsigned char uint8_t;
//typedef long int32_t;
//#include <stdint.h>
//#include <unitypes.h>
//#include <stdbool.h>
#endif

#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "FreeRTOS_IP.h"
#include "task_commands.h"

static TaskHandle_t dispatchTaskHandle = NULL;
static QueueHandle_t cmdQueue = NULL;

static void cmdDispatchTask(void *pvParameters);

void cmdStartDispatchTask(uint16_t usStackSize, UBaseType_t uxPriority) {
  const BaseType_t uxQueueSize = 10;

  /* Create the queue on which errors will be reported. */
  cmdQueue = xQueueCreate(uxQueueSize, sizeof(SysCommand_t));
  configASSERT(cmdQueue);

  BaseType_t createResult =
      xTaskCreate(cmdDispatchTask, "CmdDispatch", usStackSize, NULL,
                  uxPriority + 1, &dispatchTaskHandle);
  configASSERT(createResult);

}

bool cmdEnqueue(SysCommandsEnum_t cmd_, void *arg_) {
  SysCommand_t sysCmd = (SysCommand_t) {
    .cmd = cmd_,
    .arg = arg_
  };

  return (pdPASS == xQueueSend(cmdQueue, &sysCmd, (TickType_t) 0));
}

static void cmdDispatchTask(void *pvParameters) {
  SysCommand_t receivedCmd;
  BaseType_t xStatus;

  FreeRTOS_debug_printf(("Dispatch task running..\n"));

  for (;;) {

    xStatus = xQueueReceive(cmdQueue, &receivedCmd, portMAX_DELAY);
    if (xStatus == pdPASS) {
      switch (receivedCmd.cmd) {

        case SCMD_RESET_NETWORK:FreeRTOS_debug_printf(("Reset\n"));
          break;
        case SCMD_START:FreeRTOS_debug_printf(("Start\n"));
          break;
        case SCMD_STOP:FreeRTOS_debug_printf(("Stop\n"));
          break;
        case SCMD_DISPLAY_EN: {
          SCDisplayArgsEnum_t arg = (SCDisplayArgsEnum_t) receivedCmd.arg;
          FreeRTOS_debug_printf(("Display=%d\n", arg));
        }
          break;
      }

    } else {
      FreeRTOS_debug_printf(("Error receiving the command to dispatch\n"));
    }
  }
}