////////////////////////////////////////////////////////////////////////////////
// Project 5, CPSC 5530 Embedded Systems, Seattle University
// Team "ARM Brewery" 
// Edward Guevara, David Pierce, and Yevgeni Kamenski
// 
// cmd_parser.c - 10/21/2017 4:06 PM
// Responsible for parsing the user commands and controlling the board.
// 
// This is free and unencumbered software released into the public domain.
////////////////////////////////////////////////////////////////////////////////

#include "FreeRTOS.h"
#include "task.h"
#include "FreeRTOS_IP.h"
#include "utils/ustdlib.h"
#include "tasks/commands.h"


/**
 * \brief Output the formatted string to the command out buffer
 * \param format the format of the string
 * \param ... varargs of the printf
 */
void bufferPrintf(CmdLineOutBuf_t *outBuf, const char *fmt, ...);

// Helper for above
void bufferPrintfHelper(CmdLineOutBuf_t *outBuf, const char *format, va_list ap);

// Helper to trace the parsed user command
void traceParsedCommand(int argc, char *argv[]);

static int execCmdReset(int argc, char *argv[], CmdLineOutBuf_t *const outBuf);
static int execCmdStart(int argc, char *argv[], CmdLineOutBuf_t *const outBuf);
static int execCmdStop(int argc, char *argv[], CmdLineOutBuf_t *const outBuf);
static int execCmdDisp(int argc, char *argv[], CmdLineOutBuf_t *const outBuf);
void showHelp(CmdLineOutBuf_t *const outBuf);
int Cmd_help(int argc, char *argv[], CmdLineOutBuf_t *const outBuf);

// Table of supported commands
const tCmdLineEntry g_sCmdTable[] =
{
  {"?", Cmd_help, " : Display list of commands"},
  {"i", execCmdReset, " : Reset network connections"},
  {"s", execCmdStart, " : Init measuring tasks"},
  {"p", execCmdStop, " : Terminate measuring tasks"},
  {"d", execCmdDisp, " [on/off] : Turn display on or off"},
  {0, 0, 0}
};

// Reset command
static int execCmdReset(int argc, char *argv[], CmdLineOutBuf_t *const outBuf) {
  traceParsedCommand(argc, argv);
  cmdEnqueue(SCMD_RESET_NETWORK, NULL);
  return (0);
}


static int execCmdStart(int argc, char *argv[], CmdLineOutBuf_t *const outBuf) {
  traceParsedCommand(argc, argv);
  cmdEnqueue(SCMD_START, NULL);
  return (0);
}

static int execCmdStop(int argc, char *argv[], CmdLineOutBuf_t *const outBuf) {
  traceParsedCommand(argc, argv);
  cmdEnqueue(SCMD_STOP, NULL);
  return (0);
}

static int execCmdDisp(int argc, char *argv[], CmdLineOutBuf_t *const outBuf) {
  traceParsedCommand(argc, argv);
  int onOffVal = -1;
  if (argc == 2) {
    if (0 == ustrcmp(argv[1], "on")) {
      onOffVal = 1;
    }
    else if (0 == ustrcmp(argv[1], "off")) {
      onOffVal = 0;
    }
  }
  if (onOffVal != -1) {
    cmdEnqueue(SCMD_DISPLAY_EN, (void *)onOffVal);
  }
  else {
    bufferPrintf(outBuf,
      "Error: d command accepts a single argument 'on' or 'off'\n");
  }
  return onOffVal == -1;
}

// Help command output
void showHelp(CmdLineOutBuf_t *const outBuf) {
  bufferPrintf(outBuf, "\nAvailable commands\n");
  bufferPrintf(outBuf, "------------------\n");

  for (const tCmdLineEntry *pEntry = &g_sCmdTable[0]; pEntry->pcCmd; ++pEntry) {
    bufferPrintf(outBuf, "%s%s\n", pEntry->pcCmd, pEntry->pcHelp);
  }
}

int Cmd_help(int argc, char *argv[], CmdLineOutBuf_t *const outBuf) {
  showHelp(outBuf);
  return 1;
}

void consumeCommandBuffer(char *buf, const int bufSize,
                          CmdLineOutBuf_t *outBuf) {

  // Clear the output buffer
  outBuf->len = 0;

  int cmdStatus;                  // Status of the command processing
  char *pC;                       // ptr to the current char
  char *pCmdEnd = buf + bufSize;  // end of the received data

  // Determine if the received buffer contained a newline
  for (pC = buf; pC < pCmdEnd; ++pC) {
    if ('\n' == *pC || '\r' == *pC) {
      *pC = '\0';
      pCmdEnd = pC + 1;
      break;
    }
  }

  if (pC < buf + bufSize) {
    // Pass the line from the user to the command processor.
    // It will be parsed and valid commands executed.
    cmdStatus = CmdLineProcess(buf, outBuf);
  } else {
    // Received command did not contain a newline
    cmdStatus = CMDLINE_BAD_CMD;
  }

  if (cmdStatus == CMDLINE_BAD_CMD) {
    bufferPrintf(outBuf, "Bad command!\n");
  } else if (cmdStatus == CMDLINE_TOO_MANY_ARGS) {
    bufferPrintf(outBuf, "Too many arguments for command processor!\n");
  } else if (cmdStatus == 0) {
    bufferPrintf(outBuf, "OK\n");
  }
}

void traceParsedCommand(int argc, char *argv[]) {
  FreeRTOS_debug_printf(("Command trace:\n"));
  for (int i = 0; i < argc; ++i) {
    FreeRTOS_debug_printf(("  argv[%d]: %s\n", i, argv[i]));
  }
}


void bufferPrintfHelper(CmdLineOutBuf_t *outBuf, const char *format, va_list ap) {
  // Determine how many characters are left in the buffer
  const int nLeft = outBuf->capacity - outBuf->len;

  // Do not print anything if nothing will fit
  if (nLeft <= 0) return;

  // Generate characters for printing based on the format
  int numActual = uvsnprintf(outBuf->buf + outBuf->len,
                             (unsigned long) nLeft, format, ap);

  // When truncated, just mark the buffer as completely full
  if (numActual < 0 || numActual >= nLeft) {
    numActual = nLeft;
  }

  outBuf->len += numActual;
}

void bufferPrintf(CmdLineOutBuf_t *outBuf, const char *fmt, ...) {
  // construct var arg pointer
  va_list ap;
  va_start(ap, fmt);

  // printf the formatted arguments at the current cursor position
  bufferPrintfHelper(outBuf, fmt, ap);

  // construct var arg pointer
  va_end(ap);
}