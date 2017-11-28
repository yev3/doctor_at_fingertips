/*****************************************************************************
 *                                                                           *
 * Project ?, CPSC 5530 Embedded Systems, Seattle University, 2017           *
 * Team "ARM Brewery": Edward Guevara, David Pierce, and Yevgeni Kamenski    *
 *                                                                           *
 * task_commands.h - 11/26/2017                                              *
 *                                                                           *
 * "THE BEER-WARE LICENSE" (Revision 42):                                    *
 * As long as you retain this notice, you can do whatever you want with this *
 * software. If we meet some day, and you think this software is worth it,   *
 * you can buy us a beer in return. - Team "ARM Brewery"                     *
 *                                                                           *
 *****************************************************************************/

#pragma once

#include <portmacro.h>
#include <stdbool.h>

typedef enum {
  SCMD_RESET_NETWORK,
  SCMD_START,
  SCMD_STOP,
  SCMD_DISPLAY_EN,
} SysCommandsEnum_t;

typedef struct {
  SysCommandsEnum_t cmd;
  void * arg;
}SysCommand_t;


typedef enum {
  SC_ARG_DISP_ON = 1,
  SC_ARG_DISP_OFF = 0,
} SCDisplayArgsEnum_t;

void cmdStartDispatchTask(uint16_t usStackSize, UBaseType_t uxPriority);
bool cmdEnqueue(SysCommandsEnum_t cmd_, void *arg_);


//I The I command initializes the network communications between your system and the
//browser page.

//E The E error response is given for incorrect commands or non-existent commands.

//S The S command indicates START mode. The command shall start the embedded
//tasks by directing the hardware to initiate the measurement tasks. In doing so, the
//command shall also enable all the interrupts.

//P The P command indicates STOP mode. This command shall stop the embedded tasks
//by terminating any running measurement tasks. Such an action shall disable any data
//collecting interrupts.

//D The D command enables or disables the local display.

//M The M command. The M command requests the return of the most recent value(s) of
//all measurement data.

//W The W command. The W command requests the return of the most recent value(s) of
//all warning and alarm data.