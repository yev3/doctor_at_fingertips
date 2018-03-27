/*
 * FreeRTOS+TCP Labs Build 160919 (C) 2016 Real Time Engineers ltd.
 * Authors include Hein Tibosch and Richard Barry
 *
 *******************************************************************************
 ***** NOTE ******* NOTE ******* NOTE ******* NOTE ******* NOTE ******* NOTE ***
 ***                                                                         ***
 ***                                                                         ***
 ***   FREERTOS+TCP IS STILL IN THE LAB (mainly because the FTP and HTTP     ***
 ***   demos have a dependency on FreeRTOS+FAT, which is only in the Labs    ***
 ***   download):                                                            ***
 ***                                                                         ***
 ***   FreeRTOS+TCP is functional and has been used in commercial products   ***
 ***   for some time.  Be aware however that we are still refining its       ***
 ***   design, the source code does not yet quite conform to the strict      ***
 ***   coding and style standards mandated by Real Time Engineers ltd., and  ***
 ***   the documentation and testing is not necessarily complete.            ***
 ***                                                                         ***
 ***   PLEASE REPORT EXPERIENCES USING THE SUPPORT RESOURCES FOUND ON THE    ***
 ***   URL: http://www.FreeRTOS.org/contact  Active early adopters may, at   ***
 ***   the sole discretion of Real Time Engineers Ltd., be offered versions  ***
 ***   under a license other than that described below.                      ***
 ***                                                                         ***
 ***                                                                         ***
 ***** NOTE ******* NOTE ******* NOTE ******* NOTE ******* NOTE ******* NOTE ***
 *******************************************************************************
 *
 * FreeRTOS+TCP can be used under two different free open source licenses.  The
 * license that applies is dependent on the processor on which FreeRTOS+TCP is
 * executed, as follows:
 *
 * If FreeRTOS+TCP is executed on one of the processors listed under the Special
 * License Arrangements heading of the FreeRTOS+TCP license information web
 * page, then it can be used under the terms of the FreeRTOS Open Source
 * License.  If FreeRTOS+TCP is used on any other processor, then it can be used
 * under the terms of the GNU General Public License V2.  Links to the relevant
 * licenses follow:
 *
 * The FreeRTOS+TCP License Information Page: http://www.FreeRTOS.org/tcp_license
 * The FreeRTOS Open Source License: http://www.FreeRTOS.org/license
 * The GNU General Public License Version 2: http://www.FreeRTOS.org/gpl-2.0.txt
 *
 * FreeRTOS+TCP is distributed in the hope that it will be useful.  You cannot
 * use FreeRTOS+TCP unless you agree that you use the software 'as is'.
 * FreeRTOS+TCP is provided WITHOUT ANY WARRANTY; without even the implied
 * warranties of NON-INFRINGEMENT, MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE. Real Time Engineers Ltd. disclaims all conditions and terms, be they
 * implied, expressed, or statutory.
 *
 * 1 tab == 4 spaces!
 *
 * http://www.FreeRTOS.org
 * http://www.FreeRTOS.org/plus
 * http://www.FreeRTOS.org/labs
 *
 */


/* Standard includes. */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"

/* FreeRTOS+TCP includes. */
#include "FreeRTOS_IP.h"
#include "FreeRTOS_Sockets.h"
#include "server/FreeRTOS_TCP_server.h"
#include "server/FreeRTOS_server_private.h"
#include "FreeRTOS_IP_Private.h"

/* Remove the entire file if TCP is not being used. */
#if( ipconfigUSE_TCP == 1 )

#if !defined( ARRAY_SIZE )
	#define ARRAY_SIZE(x) ( BaseType_t ) (sizeof( x ) / sizeof( x )[ 0 ] )
#endif


static void prvReceiveNewClient(TCPServer_t *pxServer, ServerConfig_t *config, Socket_t xNexSocket);

SocketSet_t getStaticSocketSet( void )
{
  static SocketSelect_t pxSocketSet = { 0 };
  static StaticEventGroup_t eventGroup = { 0 };
  pxSocketSet.xSelectGroup = xEventGroupCreateStatic(&eventGroup);
  return (SocketSet_t *) &pxSocketSet;
}

TCPServer_t *getStaticTCPServer(ServerConfig_t config[], const BaseType_t numServers) {
  static TCPServer_t server = {0};
  struct freertos_sockaddr xAddress;
  BaseType_t xNoTimeout = 0;

  server.xServerCount = numServers;
  server.xSocketSet = getStaticSocketSet();

  for (BaseType_t i = 0; i < numServers; ++i) {
    BaseType_t port = config[i].xPortNumber;
    if (port > 0) {

      Socket_t xSocket = FreeRTOS_socket(FREERTOS_AF_INET, FREERTOS_SOCK_STREAM,
                                         FREERTOS_IPPROTO_TCP);
      FreeRTOS_printf( ( "TCP socket on port %d\n", ( int )port ) );

      if (xSocket != FREERTOS_NO_SOCKET) {
        xAddress.sin_addr = FreeRTOS_GetIPAddress();
        // Single NIC, currently not used
        xAddress.sin_port = FreeRTOS_htons( port );

        FreeRTOS_bind(xSocket, &xAddress, sizeof(xAddress));
        FreeRTOS_listen(xSocket, config[i].xBackLog);

        FreeRTOS_setsockopt(xSocket, 0, FREERTOS_SO_RCVTIMEO,
                            (void *)&xNoTimeout, sizeof( BaseType_t));
        FreeRTOS_setsockopt(xSocket, 0, FREERTOS_SO_SNDTIMEO,
                            (void *)&xNoTimeout, sizeof( BaseType_t));

        if (serverTYPE_HTTP == config[i].serverType) {
          WinProperties_t xWinProps;

          memset(&xWinProps, '\0', sizeof(xWinProps));
          /* The parent socket itself won't get connected.  The properties below
          will be inherited by each new child socket. */
          xWinProps.lTxBufSize = ipconfigHTTP_TX_BUFSIZE;
          xWinProps.lTxWinSize = ipconfigHTTP_TX_WINSIZE;
          xWinProps.lRxBufSize = ipconfigHTTP_RX_BUFSIZE;
          xWinProps.lRxWinSize = ipconfigHTTP_RX_WINSIZE;

          /* Set the window and buffer sizes. */
          FreeRTOS_setsockopt(xSocket, 0, FREERTOS_SO_WIN_PROPERTIES,
                              (void *)&xWinProps, sizeof(xWinProps));
        }

        config[i].xSocket = xSocket;
        FreeRTOS_FD_SET(xSocket, server.xSocketSet, eSELECT_READ | eSELECT_EXCEPT);
      }
    }
  }
  server.config = config;
  return &server;
}

/*-----------------------------------------------------------*/

static void prvReceiveNewClient( TCPServer_t *pxServer, ServerConfig_t *config, Socket_t xNexSocket )
{
TCPClient_t *pxClient = NULL;
BaseType_t xSize = 0;
const char *pcType = "Unknown";

	/*_RB_ Can the work and delete functions be part of the xSERVER_CONFIG structure
	becomes generic, with no pre-processing required? */
  xSize = sizeof( HTTPClient_t );
  pcType = "Active";

	/* Malloc enough space for a new HTTP-client */
	if( xSize )
	{
		pxClient = ( TCPClient_t* ) pvPortMallocLarge( xSize );
	}

	if( pxClient != NULL )
	{
		memset( pxClient, '\0', xSize );

		/* Put the new client in front of the list. */
		pxClient->pxParent = pxServer;
		pxClient->xSocket = xNexSocket;
		pxClient->pxNextClient = pxServer->pxClients;
		pxClient->fWorkFunction = config->workFn;
		pxClient->fDeleteFunction = config->deleteFn;
		pxServer->pxClients = pxClient;

		FreeRTOS_FD_SET( xNexSocket, pxServer->xSocketSet, eSELECT_READ|eSELECT_EXCEPT );
	}
	else
	{
		pcType = "closed";
		FreeRTOS_closesocket( xNexSocket );
	}

	FreeRTOS_printf( ( "TPC-server: new %s client\n", pcType ) );

	/* Remove compiler warnings in case FreeRTOS_printf() is not used. */
	( void ) pcType;
}
/*-----------------------------------------------------------*/

void FreeRTOS_TCPServerWork( TCPServer_t *pxServer, TickType_t xBlockingTime )
{
TCPClient_t **ppxClient;
  BaseType_t xRc;

	/* Let the server do one working cycle */
	xRc = FreeRTOS_select( pxServer->xSocketSet, xBlockingTime );

	if( xRc != 0 )
	{
		for( BaseType_t i = 0; i < pxServer->xServerCount; i++ )
		{
      struct freertos_sockaddr xAddress;
      Socket_t sock = pxServer->config[i].xSocket;    // Current socket
      Socket_t xNexSocket;
      socklen_t xSocketLength;

			if( sock == FREERTOS_NO_SOCKET ) {
				continue;
			}

			xSocketLength = sizeof( xAddress );
			xNexSocket = FreeRTOS_accept( sock, &xAddress, &xSocketLength);

			if( ( xNexSocket != FREERTOS_NO_SOCKET ) && ( xNexSocket != FREERTOS_INVALID_SOCKET ) )
			{
				prvReceiveNewClient( pxServer, &pxServer->config[i], xNexSocket );
			}
		}
	}

	ppxClient = &pxServer->pxClients;

	while( ( * ppxClient ) != NULL )
	{
	TCPClient_t *pxThis = *ppxClient;

		/* Almost C++ */
		xRc = pxThis->fWorkFunction( pxThis );

		if (xRc < 0 )
		{
      FreeRTOS_debug_printf(("Closing connection: %d\n", pxThis->xSocket));
			*ppxClient = pxThis->pxNextClient;
			/* Close handles, resources */
			pxThis->fDeleteFunction( pxThis );
			/* Free the space */
			vPortFreeLarge( pxThis );
		}
		else
		{
			ppxClient = &( pxThis->pxNextClient );
		}
	}
}
/*-----------------------------------------------------------*/


#endif /* ipconfigUSE_TCP != 1 */
