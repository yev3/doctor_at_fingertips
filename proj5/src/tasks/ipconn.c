// TODO Header

/* Standard includes. */
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <inc/hw_types.h>
#include <utils/sockcmdline.h>
#include "utils/ustdlib.h"
#include "utils/uartstdio.h"
#include "tasks/system.h"
#include "hardware_port.h"

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/* FreeRTOS+TCP includes. */
#include "FreeRTOS_IP.h"
#include "FreeRTOS_Sockets.h"
#include "tasks/commands.h"

/* The maximum time to wait for a closing socket to close. */
#define ipconnSHUTDOWN_DELAY  ( pdMS_TO_TICKS( 5000 ) )

/* The standard echo port number. */
#define ipconnLISTEN_PORT    10000

/* If ipconfigUSE_TCP_WIN is 1 then the Tx sockets will use a buffer size set by
ipconfigTCP_TX_BUFFER_LENGTH, and the Tx window size will be
configECHO_SERVER_TX_WINDOW_SIZE times the buffer size.  Note
ipconfigTCP_TX_BUFFER_LENGTH is set in FreeRTOSIPConfig.h as it is a standard TCP/IP
stack constant, whereas configECHO_SERVER_TX_WINDOW_SIZE is set in
FreeRTOSConfig.h as it is a demo application constant. */
#ifndef configECHO_SERVER_TX_WINDOW_SIZE
#define configECHO_SERVER_TX_WINDOW_SIZE  1
#endif

/* If ipconfigUSE_TCP_WIN is 1 then the Rx sockets will use a buffer size set by
ipconfigTCP_RX_BUFFER_LENGTH, and the Rx window size will be
configECHO_SERVER_RX_WINDOW_SIZE times the buffer size.  Note
ipconfigTCP_RX_BUFFER_LENGTH is set in FreeRTOSIPConfig.h as it is a standard TCP/IP
stack constant, whereas configECHO_SERVER_RX_WINDOW_SIZE is set in
FreeRTOSConfig.h as it is a demo application constant. */
#ifndef configECHO_SERVER_RX_WINDOW_SIZE
#define configECHO_SERVER_RX_WINDOW_SIZE  1
#endif

/* Echo client task parameters - used for both TCP and UDP echo clients. */
#define ipconnCLIENT_STACK_SIZE        ( configMINIMAL_STACK_SIZE * 2 )  /* Not used in the Windows port. */
#define ipconnCLIENT_TASK_PRIORITY          ( tskIDLE_PRIORITY + 2 )

/* Define a name that will be used for LLMNR and NBNS searches. */
#define mainHOST_NAME        "doc"
#define mainDEVICE_NICK_NAME "doc"

/*
 * Network Configuration Parameters
 */
static const uint8_t ucIPAddress[4] = { 192, 168, 1, 10 };
static const uint8_t ucNetMask[4] = { 255, 255, 255, 0 };
static const uint8_t ucGatewayAddress[4] = { 192, 168, 1, 254 };
static const uint8_t ucDNSServerAddress[4] = { 208, 67, 222, 222 };
uint8_t ucMACAddress[8];

/* Use by the pseudo random number generator. */
static UBaseType_t ulNextRand;

/*
 * Just seeds the simple pseudo random number generator.
 */
static void prvSRand(UBaseType_t ulSeed);

/*
 * Start listening
 */
void StartTCPListeningTask(uint16_t usStackSize, UBaseType_t uxPriority);

extern void NetworkGetMAC(uint8_t *ucMACAddress);

// Initializes the network stack
void network_init() {
  // Seed random number
  prvSRand((uint32_t)37);

  // Initialize the mac address
  NetworkGetMAC(ucMACAddress);

  FreeRTOS_IPInit(ucIPAddress, ucNetMask, ucGatewayAddress,
    ucDNSServerAddress, ucMACAddress);
}

void logNetworkConfig() {
  uint32_t ulIPAddress, ulNetMask, ulGatewayAddress, ulDNSServerAddress;
  char addrStrBuf[16];

  /* Print out the network configuration, which may have come from a DHCP
  server. */
  FreeRTOS_GetAddressConfiguration(&ulIPAddress,
    &ulNetMask,
    &ulGatewayAddress,
    &ulDNSServerAddress);
  FreeRTOS_inet_ntoa(ulIPAddress, addrStrBuf);
  FreeRTOS_printf(("\r\n\r\nIP Address: %s\r\n", addrStrBuf));

  FreeRTOS_inet_ntoa(ulNetMask, addrStrBuf);
  FreeRTOS_printf(("Subnet Mask: %s\r\n", addrStrBuf));

  FreeRTOS_inet_ntoa(ulGatewayAddress, addrStrBuf);
  FreeRTOS_printf(("Gateway Address: %s\r\n", addrStrBuf));

  FreeRTOS_inet_ntoa(ulDNSServerAddress, addrStrBuf);
  FreeRTOS_printf(("DNS Server Address: %s\r\n\r\n\r\n", addrStrBuf));

}

/*-----------------------------------------------------------*/

/* Called by FreeRTOS+TCP when the network connects or disconnects.  Disconnect
events are only received if implemented in the MAC driver. */
void vApplicationIPNetworkEventHook(eIPCallbackEvent_t eNetworkEvent) {
  static bool tasksCreated = false;

  if (eNetworkEvent == eNetworkUp) {
    /* Create the tasks that use the IP stack if they have not already been
    created. */
    if (!tasksCreated) {
      StartTCPListeningTask(ipconnCLIENT_STACK_SIZE,
        ipconnCLIENT_TASK_PRIORITY);
    }
    tasksCreated = pdTRUE;
    logNetworkConfig();
  }
}

/*-----------------------------------------------------------*/

UBaseType_t uxRand(void) {
  const uint32_t ulMultiplier = 0x015a4e35UL, ulIncrement = 1UL;

  /* Utility function to generate a pseudo random number. */

  ulNextRand = (ulMultiplier * ulNextRand) + ulIncrement;
  return ((int)(ulNextRand >> 16UL) & 0x7fffUL);
}
/*-----------------------------------------------------------*/

static void prvSRand(UBaseType_t ulSeed) {
  /* Utility function to seed the pseudo random number generator. */
  ulNextRand = ulSeed;
}
/*-----------------------------------------------------------*/


#if(ipconfigUSE_LLMNR != 0) || (ipconfigUSE_NBNS != 0) || (ipconfigDHCP_REGISTER_HOSTNAME == 1)

const char *pcApplicationHostnameHook(void) {
  /* Assign the name "FreeRTOS" to this network node.  This function will
  be called during the DHCP: the machine will be registered with an IP
  address plus this name. */
  return mainHOST_NAME;
}

#endif
/*-----------------------------------------------------------*/

#if(ipconfigUSE_LLMNR != 0) || (ipconfigUSE_NBNS != 0)

BaseType_t xApplicationDNSQueryHook(const char *pcName)
{
  BaseType_t xReturn;

  /* Determine if a name lookup is for this node.  Two names are given
  to this node: that returned by pcApplicationHostnameHook() and that set
  by mainDEVICE_NICK_NAME. */
  if (_stricmp(pcName, pcApplicationHostnameHook()) == 0)
  {
    xReturn = pdPASS;
  }
  else if (_stricmp(pcName, mainDEVICE_NICK_NAME) == 0)
  {
    xReturn = pdPASS;
  }
  else
  {
    xReturn = pdFAIL;
  }

  return xReturn;
}

#endif

/*-----------------------------------------------------------*/


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
    } else if (0 == ustrcmp(argv[1], "off")) {
      onOffVal = 0;
    }
  }
  if (onOffVal != -1) {
    cmdEnqueue(SCMD_DISPLAY_EN, (void *) onOffVal);
  } else {
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


/*
 * Uses FreeRTOS+TCP to listen for incoming echo connections, creating a task
 * to handle each connection.
 */
static void prvConnectionListeningTask(void *pvParameters);

/*
 * Created by the connection listening task to handle a single connection.
 */
static void prvServerConnectionInstance(void *pvParameters);

/*-----------------------------------------------------------*/

/* Stores the stack size passed into vStartSimpleTCPServerTasks() so it can be
reused when the server listening task creates tasks to handle connections. */
static uint16_t usUsedStackSize = 0;

/*-----------------------------------------------------------*/

void StartTCPListeningTask(uint16_t usStackSize, UBaseType_t uxPriority) {

  FreeRTOS_debug_printf(("Start ServerListener\n"));
  /* Create the TCP echo server. */
  xTaskCreate(prvConnectionListeningTask,
              "ServerListener",
              usStackSize,
              NULL,
              uxPriority + 1,
              NULL);

  /* Remember the requested stack size so it can be re-used by the server
  listening task when it creates tasks to handle connections. */
  usUsedStackSize = usStackSize;
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
/*-----------------------------------------------------------*/

static void prvConnectionListeningTask(void *pvParameters) {
  struct freertos_sockaddr xClient, xBindAddress;
  Socket_t xListeningSocket, xConnectedSocket;
  socklen_t xSize = sizeof(xClient);
  static const TickType_t xReceiveTimeOut = portMAX_DELAY;
  const BaseType_t xBacklog = 20;

#if(ipconfigUSE_TCP_WIN == 1)
  WinProperties_t xWinProps;

  /* Fill in the buffer and window sizes that will be used by the socket. */
  xWinProps.lTxBufSize = ipconfigTCP_TX_BUFFER_LENGTH;
  xWinProps.lTxWinSize = configECHO_SERVER_TX_WINDOW_SIZE;
  xWinProps.lRxBufSize = ipconfigTCP_RX_BUFFER_LENGTH;
  xWinProps.lRxWinSize = configECHO_SERVER_RX_WINDOW_SIZE;
#endif /* ipconfigUSE_TCP_WIN */

  /* Just to prevent compiler warnings. */
  (void) pvParameters;

  /* Attempt to open the socket. */
  xListeningSocket = FreeRTOS_socket(FREERTOS_AF_INET,
                                     FREERTOS_SOCK_STREAM,
                                     FREERTOS_IPPROTO_TCP);
  configASSERT(xListeningSocket != FREERTOS_INVALID_SOCKET);

  /* Set a time out so accept() will just wait for a connection. */
  FreeRTOS_setsockopt(xListeningSocket,
                      0,
                      FREERTOS_SO_RCVTIMEO,
                      &xReceiveTimeOut,
                      sizeof(xReceiveTimeOut));

  /* Set the window and buffer sizes. */
#if(ipconfigUSE_TCP_WIN == 1)
  {
    FreeRTOS_setsockopt(xListeningSocket,
                        0,
                        FREERTOS_SO_WIN_PROPERTIES,
                        (void *) &xWinProps,
                        sizeof(xWinProps));
  }
#endif /* ipconfigUSE_TCP_WIN */

  /* Bind the socket to the port that the client task will send to, then
  listen for incoming connections. */
  xBindAddress.sin_port = ipconnLISTEN_PORT;
  xBindAddress.sin_port = FreeRTOS_htons(xBindAddress.sin_port);
  FreeRTOS_bind(xListeningSocket, &xBindAddress, sizeof(xBindAddress));
  FreeRTOS_listen(xListeningSocket, xBacklog);

  for (;;) {
    /* Wait for a client to connect. */
    xConnectedSocket = FreeRTOS_accept(xListeningSocket, &xClient, &xSize);
    configASSERT(xConnectedSocket != FREERTOS_INVALID_SOCKET);

    FreeRTOS_debug_printf(("Accepted connection, creating task..\n"));

    /* Spawn a task to handle the connection. */
    xTaskCreate(prvServerConnectionInstance,
                "CommandInterpreterServer",
                usUsedStackSize,
                (void *) xConnectedSocket,
                tskIDLE_PRIORITY+3,
                NULL);
  }
}
#pragma clang diagnostic pop
/*-----------------------------------------------------------*/

static int connInstanceCount = 0;

static void prvServerConnectionInstance(void *pvParameters) {
  static const TickType_t xReceiveTimeOut = pdMS_TO_TICKS(5000);
  static const TickType_t xSendTimeOut = pdMS_TO_TICKS(5000);
  TickType_t xTimeOnShutdown;

  int32_t lRecvSize;    // Num bytes received from socket
  int32_t lSent;
  int32_t lTotalSent;

  int cmdStatus;        // Status of the command processing

  // Received command buffer and size
  char recvBuf[100];
  const BaseType_t recvBufSize = sizeof(recvBuf);

  // Command response buffer
  char respBuf[512];
  CmdLineOutBuf_t outBuf = (CmdLineOutBuf_t) {
    .buf = respBuf,
    .capacity = sizeof(respBuf),
    .len = 0
  };

  Socket_t xConnectedSocket = (Socket_t) pvParameters;

  int myTaskInstNum = ++connInstanceCount;

  FreeRTOS_debug_printf(("#%d: Running listening task ..\n", myTaskInstNum));

  FreeRTOS_setsockopt(xConnectedSocket, 0, FREERTOS_SO_RCVTIMEO,
                      &xReceiveTimeOut, sizeof(xReceiveTimeOut));

  FreeRTOS_setsockopt(xConnectedSocket, 0, FREERTOS_SO_SNDTIMEO,
                      &xSendTimeOut, sizeof(xReceiveTimeOut));

  for (;;) {

    // Clear the output buffer
    outBuf.len = 0;

    // Receive command line
    while (!(lRecvSize =
               FreeRTOS_recv(xConnectedSocket, recvBuf, recvBufSize, 0))) {}

    if (lRecvSize < 0) {
      FreeRTOS_debug_printf(("#%d: Receive size invalid, timeout?\n", myTaskInstNum));
      /* Socket closed? */
      break;
    }

    char *pC;                             // ptr to the current char
    char *pCmdEnd = recvBuf + lRecvSize;  // end of the received data

    for (pC = recvBuf; pC < pCmdEnd; ++pC) {
      if ('\n' == *pC || '\r' == *pC) {
        *pC = '\0';
        pCmdEnd = pC + 1;
        break;
      }
    }

    if (pC < recvBuf + lRecvSize) {
      // Pass the line from the user to the command processor.
      // It will be parsed and valid commands executed.
      cmdStatus = CmdLineProcess(recvBuf, &outBuf);
    } else {
      // Received command did not contain a newline
      cmdStatus = CMDLINE_BAD_CMD;
    }

    if (cmdStatus == CMDLINE_BAD_CMD) {
      bufferPrintf(&outBuf, "Bad command!\n");
    } else if (cmdStatus == CMDLINE_TOO_MANY_ARGS) {
      bufferPrintf(&outBuf, "Too many arguments for command processor!\n");
    } else if (cmdStatus == 0) {
      bufferPrintf(&outBuf, "OK\n");
    }

    /* If there is data to send back */
    if (outBuf.len > 0) {
      lSent = 0;
      lTotalSent = 0;

      outBuf.buf[outBuf.capacity - 1] = '\0';
      FreeRTOS_debug_printf(("Sending back:\n%s\n", outBuf.buf));

      /* Call send() until all the data has been sent. */
      while ((lSent >= 0) && (lTotalSent < outBuf.len)) {
        lSent =
          FreeRTOS_send(xConnectedSocket,
                        outBuf.buf,
                        outBuf.len - lTotalSent,
                        0);
        lTotalSent += lSent;
      }

      if (lSent < 0) {
        /* Socket closed? */
        break;
      }
    }
  }

  FreeRTOS_debug_printf(("#%d: Shutting down listening task ..\n", myTaskInstNum));

  /* Initiate a shutdown in case it has not already been initiated. */
  FreeRTOS_shutdown(xConnectedSocket, FREERTOS_SHUT_RDWR);

  /* Wait for the shutdown to take effect, indicated by FreeRTOS_recv()
  returning an error. */
  xTimeOnShutdown = xTaskGetTickCount();
  do {
    if (FreeRTOS_recv(xConnectedSocket, recvBuf, recvBufSize, 0) < 0) {
      break;
    }
  } while ((xTaskGetTickCount() - xTimeOnShutdown) < ipconnSHUTDOWN_DELAY);

  /* Finished with the socket and the task. */
  FreeRTOS_closesocket(xConnectedSocket);

  vTaskDelete(NULL);
}

/*-----------------------------------------------------------*/

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

void traceParsedCommand(int argc, char *argv[]) {
  FreeRTOS_debug_printf(("Command trace:\n"));
  for (int i = 0; i < argc; ++i) {
    FreeRTOS_debug_printf(("  argv[%d]: %s\n", i, argv[i]));
  }
}

