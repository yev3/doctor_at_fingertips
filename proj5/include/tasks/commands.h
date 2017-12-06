/*****************************************************************************
 * Project 5, CPSC 5530 Embedded Systems, Seattle University, 2017           *
 * Team "ARM Brewery": Edward Guevara, David Pierce, and Yevgeni Kamenski    *
 *                                                                           *
 * task_commands.h - 11/26/2017                                              *
 *                                                                           *
 * "THE BEER-WARE LICENSE" (Revision 42):                                    *
 * As long as you retain this notice, you can do whatever you want with this *
 * software. If we meet some day, and you think this software is worth it,   *
 * you can buy us a beer in return. - Team "ARM Brewery"                     *
 * 
 * Based on:
 * cmdline.h - Prototypes for command line processing functions.
 *
 * Copyright (c) 2007-2013 Texas Instruments Incorporated.  
 * All rights reserved.  
 *                                                                           *
 *****************************************************************************/

#pragma once

#include <portmacro.h>
#include <stdbool.h>

typedef enum {
  cmdKEY_UP,
  cmdKEY_DOWN,
  cmdKEY_LEFT,
  cmdKEY_RIGHT,
  cmdKEY_SELECT,
} KeyPress_t;

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


typedef enum SCDisplayArgsEnum_t{
  SC_ARG_DISP_ON = 1,
  SC_ARG_DISP_OFF = 0,
} SCDisplayArgsEnum_t;

/**
 * \brief Queue a system command to execute
 * \param cmd_ Command to enqueue for processing
 * \param arg_ Optional argument for the command
 * \return True if successfull
 */
bool cmdEnqueue(SysCommandsEnum_t cmd_, void *arg_);

//*****************************************************************************
// Define some status codes to return when parsing commands. 
//*****************************************************************************
#define CMDLINE_BAD_CMD         (-1)
#define CMDLINE_TOO_MANY_ARGS   (-2)
#define CMDLINE_TOO_FEW_ARGS   (-3)
#define CMDLINE_INVALID_ARG   (-4)

typedef struct {
  char *const buf;    // Buffer
  const int capacity; // Max buffer size
  int len;            // Current stored size
} CmdLineOutBuf_t;

//*****************************************************************************
// Command line function callback type.
//*****************************************************************************
typedef int (*pfnCmdLine)(int argc, char *argv[], CmdLineOutBuf_t *const outBuf);

//*****************************************************************************
//! Structure for an entry in the command list table.
//*****************************************************************************
typedef struct
{
    //! A pointer to a string containing the name of the command.
    const char *pcCmd;
    //! A function pointer to the implementation of the command.
    pfnCmdLine pfnCmd;
    //! A pointer to a string of brief help text for the command.
    const char *pcHelp;
}
tCmdLineEntry;

//*****************************************************************************
//! This is the command table that must be provided by the application.
//*****************************************************************************
extern const tCmdLineEntry g_sCmdTable[];

//*****************************************************************************
// Prototypes for the APIs.
//*****************************************************************************
extern int CmdLineProcess(char *pcCmdLine, CmdLineOutBuf_t *outBuf);

// Processes the received command string
void consumeCommandBuffer(char *buf, int32_t lRecvSize,
                          CmdLineOutBuf_t *outBuf);


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