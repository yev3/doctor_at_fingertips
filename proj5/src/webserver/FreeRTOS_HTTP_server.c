/*
 * Modified from:
 * FreeRTOS+TCP Labs Build 160919 (C) 2016 Real Time Engineers ltd.
 * Authors include Hein Tibosch and Richard Barry
 */

#include <stdio.h>
#include <stdlib.h>

#include "FreeRTOS.h"
#include "task.h"

#include "FreeRTOS_IP.h"
#include "FreeRTOS_Sockets.h"

#include "server/FreeRTOS_server_private.h"
#include "server/server.h"

// To make the code more readable
#define pcCOMMAND_BUFFER	pxClient->pxParent->pcCommandBuffer

/**
 * \brief Handles an HTTP GET command from the client
 * \param pxClient HTTP Client
 * \return Return -1 in the event of an error
 */
static BaseType_t handleGETCommand( HTTPClient_t *pxClient );
static BaseType_t prvSendFile( HTTPClient_t *pxClient );

static const char pcEmptyString[1] = { '\0' };

void vHTTPClientDelete( TCPClient_t *pxTCPClient )
{
HTTPClient_t *pxClient = ( HTTPClient_t * ) pxTCPClient;

	/* This HTTP client stops, close / release all resources. */
	if( pxClient->xSocket != FREERTOS_NO_SOCKET )
	{
		FreeRTOS_FD_CLR( pxClient->xSocket, pxClient->pxParent->xSocketSet, eSELECT_ALL );
		FreeRTOS_closesocket( pxClient->xSocket );
		pxClient->xSocket = FREERTOS_NO_SOCKET;
	}
	//prvFileClose( pxClient );
}
/*-----------------------------------------------------------*/

/**
 * \brief 
 * \param pxClient 
 * \return Returns 0 when finished sending file
 */
static BaseType_t prvSendFile( HTTPClient_t *pxClient )
{
size_t uxSpace;
size_t uxCount;
BaseType_t xRc = 0;

  
	if( pxClient->bits.bReplySent == pdFALSE_UNSIGNED )
	{
		pxClient->bits.bReplySent = pdTRUE_UNSIGNED;
	}

	if( xRc >= 0 ) do
	{
		uxSpace = FreeRTOS_tx_space( pxClient->xSocket );

		if( pxClient->uxBytesLeft < uxSpace )
		{
			uxCount = pxClient->uxBytesLeft;
		}
		else
		{
			uxCount = uxSpace;
		}

		if( uxCount > 0u )
		{
			if( uxCount > sizeof( pxClient->pxParent->pcFileBuffer ) )
			{
				uxCount = sizeof( pxClient->pxParent->pcFileBuffer );
			}
			ff_fread( pxClient->pxParent->pcFileBuffer, 1, uxCount, pxClient->pxFileHandle );
			pxClient->uxBytesLeft -= uxCount;

			xRc = FreeRTOS_send( pxClient->xSocket, pxClient->pxParent->pcFileBuffer, uxCount, 0 );
			if( xRc < 0 )
			{
				break;
			}
		}
	} while( uxCount > 0u );

	if( pxClient->uxBytesLeft == 0u )
	{
		/* Writing is ready, no need for further 'eSELECT_WRITE' events. */
		FreeRTOS_FD_CLR( pxClient->xSocket, pxClient->pxParent->xSocketSet, eSELECT_WRITE );

    // Close the connection after file finished
    FreeRTOS_shutdown(pxClient->xSocket, FREERTOS_SHUT_RDWR);

    // Send back 0 to indicate finished reading file 
    xRc = 0;
	}
	else
	{
		/* Wake up the TCP task as soon as this socket may be written to. */
		FreeRTOS_FD_SET( pxClient->xSocket, pxClient->pxParent->xSocketSet, eSELECT_WRITE );
	}

  if (0 <= xRc) {
    FreeRTOS_debug_printf(("File successfully sent: %s\n", xRc == 0 ? "True" : "False"));
  }

	return xRc;
}
/*-----------------------------------------------------------*/

static FF_FILE *openFile(const char *fname) {
  static const char dataURL[] = "/data.json";
  FF_FILE *file = NULL;

  if (strncmp(fname, dataURL, (sizeof(dataURL))) == 0) {
    file = ff_fopen_json();
  } else {
    file = ff_fopen_main();
  }

	FreeRTOS_printf( ( "Open file '%s': %s\n", fname, file != NULL ? "Ok" : "Error" ) );
  return file;
}

static BaseType_t handleGETCommand( HTTPClient_t *pxClient )
{
  pxClient->bits.ulFlags = 0;

  pxClient->pxFileHandle = openFile(pxClient->pcUrlData);
  pxClient->uxBytesLeft = ( size_t ) pxClient->pxFileHandle->ulFileSize;

  return prvSendFile( pxClient );
}
/*-----------------------------------------------------------*/

BaseType_t xHTTPClientWork( TCPClient_t *pxTCPClient )
{
BaseType_t xRc;
HTTPClient_t *pxClient = ( HTTPClient_t * ) pxTCPClient;

  // Already processed the request, so continue sending the file
	if( pxClient->pxFileHandle != NULL )
	{
		xRc = prvSendFile( pxClient );
	}

  // Determine if client sent anything
	xRc = FreeRTOS_recv( pxClient->xSocket, ( void * )pcCOMMAND_BUFFER, sizeof( pcCOMMAND_BUFFER ), 0 );

	if( xRc > 0 )
	{
	const char *pcEndOfCmd;
	char *pcBuffer = pcCOMMAND_BUFFER;

		if( xRc < ( BaseType_t ) sizeof( pcCOMMAND_BUFFER ) )
		{
			pcBuffer[ xRc ] = '\0';
		}

    // Remove any \r or \n from the end of the received buffer
		while( xRc && ( pcBuffer[ xRc - 1 ] == 13 || pcBuffer[ xRc - 1 ] == 10 ) )
		{
			pcBuffer[ --xRc ] = '\0';
		}

    // Mark the end of the command to be where the very last \r or \n was found
		pcEndOfCmd = pcBuffer + xRc;

		/* Pointing to "/index.html HTTP/1.1". */
		pxClient->pcUrlData = pcBuffer;

		/* Pointing to "HTTP/1.1". */
		pxClient->pcRestData = pcEmptyString;

    static const char getCommand[] = "GET";
    if(strncmp(getCommand, pcBuffer, ((sizeof(getCommand)) - 1)) == 0)
    {
      pxClient->pcUrlData += (sizeof(getCommand));
      for(char *pLast = (char*)pxClient->pcUrlData; pLast < pcEndOfCmd; pLast++ )
      {
        char ch = *pLast;
        if( ( ch == '\0' ) || ( strchr( "\n\r \t", ch ) != NULL ) )
        {
          *pLast = '\0';
          pxClient->pcRestData = pLast + 1;
          break;
        }
      }
      xRc = handleGETCommand( pxClient );
    } else {
      // Some other command!
      FreeRTOS_printf(("Unknown HTTP command: %s\n", pcBuffer));

      xRc = -1; // return error
    }

	}
	else if( xRc < 0 )
	{
		/* The connection will be closed and the client will be deleted. */
		FreeRTOS_printf( ( "xHTTPClientWork: rc = %ld\n", xRc ) );
	}
	return xRc;
}
/*-----------------------------------------------------------*/


