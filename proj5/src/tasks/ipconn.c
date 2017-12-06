// TODO Header

/* Standard includes. */
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <inc/hw_types.h>
#include <utils/sockcmdline.h>
#include "utils/ustdlib.h"
#include "utils/uartstdio.h"

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/* FreeRTOS+TCP includes. */
#include "FreeRTOS_IP.h"
#include "FreeRTOS_Sockets.h"
#include "tasks/commands.h"

#include "tasks/system.h"
#include "hardware_port.h"
#include "server/FreeRTOS_TCP_server.h"
#include "FreeRTOS_IP_Private.h"
#include "server/FreeRTOS_server_private.h"

#define mainTCP_SERVER_TASK_PRIORITY (tskIDLE_PRIORITY + 2)

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

/*
 * Network Configuration Parameters
 */
static const uint8_t ucIPAddress[4] = { 169, 254, 163, 6 };
static const uint8_t ucNetMask[4] = { 255, 255, 255, 0 };
static const uint8_t ucGatewayAddress[4] = { 192, 168, 1, 254 };
static const uint8_t ucDNSServerAddress[4] = { 208, 67, 222, 222 };
uint8_t ucMACAddress[8];

// Gets the network MAC
extern void NetworkGetMAC(uint8_t *ucMACAddress);

// Processes the received command string
extern void consumeCommandBuffer(char *buf, int32_t lRecvSize,
                                 CmdLineOutBuf_t *outBuf);

////////////////////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////////////////////

// Initializes the network stack
void network_init() {
  // Seed random number
  usrand(37);

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

static TaskHandle_t serverWorkTaskHandle = NULL;

/* Called by FreeRTOS+TCP when the network connects or disconnects.  Disconnect
events are only received if implemented in the MAC driver. */
void vApplicationIPNetworkEventHook( eIPCallbackEvent_t eNetworkEvent )
{
static BaseType_t xTasksAlreadyCreated = pdFALSE;

	/* If the network has just come up...*/
	if( eNetworkEvent == eNetworkUp )
	{
		/* Create the tasks that use the IP stack if they have not already been
		created. */
		if( xTasksAlreadyCreated == pdFALSE )
		{

      /* See TBD.
      Let the server work task now it can now create the servers. */
      configASSERT(serverWorkTaskHandle);
      xTaskNotifyGive( serverWorkTaskHandle );

			xTasksAlreadyCreated = pdTRUE;
		}

    logNetworkConfig();
	}
}

BaseType_t telnetWork( TCPClient_t *pxTCPClient ) 
{
  static char recvBuf[100];     // Received command buffer and size
  static const BaseType_t recvBufSize = sizeof(recvBuf);
  static char respBuf[512];     // Command response buffer

  Socket_t sock = (Socket_t)pxTCPClient->xSocket;

  int32_t lSent;
  int32_t lTotalSent;

  BaseType_t xRc = FreeRTOS_recv(sock, recvBuf, recvBufSize, 0);

  if (xRc > 0) {

    CmdLineOutBuf_t outBuf = (CmdLineOutBuf_t) {
      .buf = respBuf,
      .capacity = sizeof(respBuf),
      .len = 0
    };

    consumeCommandBuffer(recvBuf, xRc, &outBuf);

    /* If there is data to send back */
    if (outBuf.len > 0) {
      lSent = 0;
      lTotalSent = 0;

      outBuf.buf[outBuf.capacity - 1] = '\0';
      FreeRTOS_debug_printf(("Sending back:\n%s\n", outBuf.buf));

      /* Call send() until all the data has been sent. */
      while (lTotalSent < outBuf.len) {
        lSent =
          FreeRTOS_send(sock,
                        outBuf.buf,
                        outBuf.len - lTotalSent,
                        0);
        if (lSent <= 0) {
          break;
        }
        lTotalSent += lSent;
      }

      if (lSent < 0) {
        /* Socket closed? */
        //break;
      }
    }
  }

  return xRc;
}


static ServerConfig_t serverConfig[] = {
  {serverTYPE_HTTP, 80, 3, xHTTPClientWork, vHTTPClientDelete},
  {serverTYPE_TELNET, 23, 2, telnetWork, vHTTPClientDelete},
};

void ipServerTask(void *pvParameters)
{
  serverWorkTaskHandle = *(TaskHandle_t*)pvParameters;

  TCPServer_t *pxTCPServer = NULL;
  const TickType_t xInitialBlockTime = portMAX_DELAY;
  //const TickType_t xInitialBlockTime = pdMS_TO_TICKS(200UL);

  /* Wait until the network is up before creating the servers.  The
  notification is given from the network event hook. */
  ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

  // Initialize the server
  pxTCPServer = getStaticTCPServer(serverConfig, ARRAY_SIZE(serverConfig));

  for (;;) {
    FreeRTOS_TCPServerWork(pxTCPServer, xInitialBlockTime);
  }
}


/*-----------------------------------------------------------*/


/* Stores the stack size passed into vStartSimpleTCPServerTasks() so it can be
reused when the server listening task creates tasks to handle connections. */
static uint16_t usUsedStackSize = 0;

static void _prvServerConnectionInstance(void *pvParameters) {
  static const TickType_t xReceiveTimeOut = pdMS_TO_TICKS(5000);
  static const TickType_t xSendTimeOut = pdMS_TO_TICKS(5000);
  TickType_t xTimeOnShutdown;

  int32_t lRecvSize;    // Num bytes received from socket
  int32_t lSent;
  int32_t lTotalSent;


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

  FreeRTOS_debug_printf(("Running new listening task ..\n"));

  FreeRTOS_setsockopt(xConnectedSocket, 0, FREERTOS_SO_RCVTIMEO,
                      &xReceiveTimeOut, sizeof(xReceiveTimeOut));

  FreeRTOS_setsockopt(xConnectedSocket, 0, FREERTOS_SO_SNDTIMEO,
                      &xSendTimeOut, sizeof(xReceiveTimeOut));

  for (;;) {

    // Receive command line
    while (!(lRecvSize =
               FreeRTOS_recv(xConnectedSocket, recvBuf, recvBufSize, 0))) {}

    if (lRecvSize < 0) {
      FreeRTOS_debug_printf(("Receive size invalid, timeout?\n"));
      /* Socket closed? */
      break;
    }

    consumeCommandBuffer(recvBuf, lRecvSize, &outBuf);

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

  FreeRTOS_debug_printf(("Shutting down listening task ..\n"));

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

