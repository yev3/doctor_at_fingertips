//
// Created by YK on 11/26/2017.
//

#include <stdarg.h>
#include <FreeRTOS.h>
#include <task.h>
#include <FreeRTOS_IP.h>

#include "RTOSDebugTrace.h"
#include "utils/ustdlib.h"
#include "utils/uartstdio.h"


int lTraceLoggingPrintf( const char *pcFormatString, ... )
{
#define LOG_BUF_LEN 256
  static char buf[LOG_BUF_LEN+1];

  static int currentNum = 0;

  va_list args;
  va_start (args, pcFormatString);
  //   xLength = prvBufferFormattedString (pcFormatString, args);

  char * taskName = (xTaskGetSchedulerState() == taskSCHEDULER_RUNNING) ?
                    pcTaskGetName(NULL) :
                    "-";

  BaseType_t xLength = ( BaseType_t ) snprintf( buf, LOG_BUF_LEN, "%3d [%-10s] ",
                                                currentNum, taskName );
  xLength += ( size_t ) vsnprintf( buf + xLength, LOG_BUF_LEN - xLength, pcFormatString, args );
  buf[LOG_BUF_LEN] = '\0';

  va_end (args);

  UARTwrite(buf, xLength);

  return ( int ) xLength;
}

void vAssertCalled(const char *pcFile, unsigned long ulLine) {
  const uint32_t ulLongSleep = 1000UL;
  volatile uint32_t ulBlockVariable = 0UL;
  volatile char *pcFileName = (volatile char *)pcFile;
  volatile uint32_t ulLineNumber = ulLine;

  (void)pcFileName;
  (void)ulLineNumber;

  FreeRTOS_debug_printf(("vAssertCalled( %s, %ld\n", pcFile, ulLine));

  /* Setting ulBlockVariable to a non-zero value in the debugger will allow
  this function to be exited. */
  taskDISABLE_INTERRUPTS();
  {
    while (ulBlockVariable == 0UL) {
      // Enter loop for debugging
    }
  }
  taskENABLE_INTERRUPTS();
}
/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName) {
  (void)pcTaskName;
  (void)pxTask;

  /* Run time stack overflow checking is performed if
  configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
  function is called if a stack overflow is detected. */
  taskDISABLE_INTERRUPTS();
  for (;;);
}

/*-----------------------------------------------------------*/

