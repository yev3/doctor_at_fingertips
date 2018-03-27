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

#include <stdbool.h>
#include <stdint.h>

// Types of key presses
typedef enum {
  cmdKEY_UP,
  cmdKEY_DOWN,
  cmdKEY_LEFT,
  cmdKEY_RIGHT,
  cmdKEY_SELECT,
} KeyPress_t;

// Commands that are dispatched from the network task
typedef enum {
  SCMD_RESET_NETWORK, // i command
  SCMD_START,         // s command
  SCMD_STOP,          // p command
  SCMD_DISPLAY_EN,    // d command
} SysCommandsEnum_t;

// Command to enqueue to the dispatcher
typedef struct {
  SysCommandsEnum_t cmd;  // Command
  void * arg;             // Command arguments
}SysCommand_t;


// Arguments for the display command to turn on or off
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
#define CMDLINE_BAD_CMD       (-1)
#define CMDLINE_TOO_MANY_ARGS (-2)
#define CMDLINE_TOO_FEW_ARGS  (-3)
#define CMDLINE_INVALID_ARG   (-4)

// Buffer to place the output of the command
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

/**
 * \brief Parses the command and dispatches an appropriate handler
 * \param pcCmdLine command line to process
 * \param outBuf where to put the result of the command
 * \return result code
 */
extern int CmdLineProcess(char *pcCmdLine, CmdLineOutBuf_t *outBuf);

// Processes the received command string
void consumeCommandBuffer(char *buf, int32_t lRecvSize,
                          CmdLineOutBuf_t *outBuf);


