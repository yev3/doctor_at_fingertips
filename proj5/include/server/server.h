////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <FreeRTOS.h>
#include "task.h"

/* FreeRTOS+TCP includes. */
#include "FreeRTOS_IP.h"
#include "FreeRTOS_Sockets.h"

#include "server/FreeRTOS_TCP_server.h"
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
 * \brief Read the file into the buffer.
 * \param outBuf Destination of file read data
 * \param itemSize Size of a single item
 * \param itemCount Count of items to read
 * \param file File handle to use
 * \return Count of bytes read
 */
size_t ff_fread(void *outBuf, const size_t itemSize, 
                const size_t itemCount, FF_FILE *file);

/**
 * \brief Opens the main page file
 * \return Pointer to a file
 */
FF_FILE *ff_fopen_main();

/**
 * \brief Opens the dynamic board status file.
 * \return Pointer to a file
 */
FF_FILE *ff_fopen_json();
