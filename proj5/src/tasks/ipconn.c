////////////////////////////////////////////////////////////////////////////////
// Project 5, CPSC 5530 Embedded Systems, Seattle University
// Team "ARM Brewery" 
// Edward Guevara, David Pierce, and Yevgeni Kamenski
// 
// ipconn.c
// IP connectivity task. Defines functions for processing web page requests,
// and handles telnet command line interface to control the board.
// 
// This is free and unencumbered software released into the public domain.
////////////////////////////////////////////////////////////////////////////////

/* Standard includes. */
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <inc/hw_types.h>
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

/**
 * \brief Main server task. Blocks on IP events, waiting for connections.
 * When connection is established, uses a socket set to monitor traffic
 * events and dispatches the attached workers. Both webserver and telnet
 * connections are thus handles by this single task
 * \param pvParameters Passed parameters by the init task.
 */
void ipServerTask(void *pvParameters);

/**
 * \brief Worker for the telnet connection. Receives commands, parses
 * them, and dispatches controls to the rest of the board. 
 * \param pxTCPClient Connected client
 * \return Socket read/write result
 */
static BaseType_t telnetWork(TCPClient_t *pxTCPClient);

/**
 * \brief Server configuration. Web interface is on port 80, telnet
 * command line interface is on port 23.
 */
static ServerConfig_t serverConfig[] = {
  {serverTYPE_HTTP, 80, 3, xHTTPClientWork, vHTTPClientDelete},
  {serverTYPE_TELNET, 23, 2, telnetWork, vHTTPClientDelete},
};

/*
 * Network Configuration Parameters.  These are used should DHCP fail.
 */
static const uint8_t ucIPAddress[4] = { 169, 254, 163, 6 };
static const uint8_t ucNetMask[4] = { 255, 255, 255, 0 };
static const uint8_t ucGatewayAddress[4] = { 192, 168, 1, 254 };
static const uint8_t ucDNSServerAddress[4] = { 208, 67, 222, 222 };
uint8_t ucMACAddress[8];

/**
 * \brief Server work task handle. Passed by the initialization task.
 * Used to send the network up event, so that can create listening
 * sockets.
 */
static TaskHandle_t serverWorkTaskHandle = NULL;

/**
 * \brief Initializes the network stack
 */
void network_init() {
  // Seed random number
  usrand(37);

  // Initialize the mac address
  NetworkGetMAC(ucMACAddress);

  FreeRTOS_IPInit(ucIPAddress, ucNetMask, ucGatewayAddress,
    ucDNSServerAddress, ucMACAddress);
}

/**
 * \brief Used to help trace network connection problems
 */
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

/**
 * \brief Called by FreeRTOS+TCP when the network connects or disconnects.
 * Disconnect events are only received if implemented in the MAC driver. 
 * \param eNetworkEvent Type of event that occurred
 */
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

/**
 * \brief Worker for the telnet connection. Receives commands, parses
 * them, and dispatches controls to the rest of the board. 
 * \param pxTCPClient Connected client
 * \return Socket read/write result
 */
static BaseType_t telnetWork( TCPClient_t *pxTCPClient ) 
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

/**
 * \brief Main server task. Blocks on IP events, waiting for connections.
 * When connection is established, uses a socket set to monitor traffic
 * events and dispatches the attached workers. Both webserver and telnet
 * connections are thus handles by this single task
 * \param pvParameters Passed parameters by the init task.
 */
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