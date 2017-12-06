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

#include "server/server.h"
#include "tasks/system.h"
#include "tasks/commands.h"
#include "hardware_port.h"

// Compressed webpage data
#include "server/webpage_data.c"
#include "utils/float_print.h"

#define SERVER_COUNT 2

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
 * \brief Reference to corrected buffers when outputting device data.
 * Initialized when task starts up.
 */
static CorrectedBuffers * correctedBuffers = NULL;

/**
 * \brief Reference to alarm state when outputting device data.
 * Initialized when task starts up.
 */
static WarningAlarmStates * warnAlarms = NULL;

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
  serverWorkTaskHandle = xTaskGetCurrentTaskHandle();
  IPTaskData_t * data = (IPTaskData_t*)pvParameters;
  correctedBuffers = data->correctedBuffers;
  warnAlarms = data->warnAlarms;

  TCPServer_t *pxTCPServer = NULL;
  const TickType_t xInitialBlockTime = portMAX_DELAY;
  //const TickType_t xInitialBlockTime = pdMS_TO_TICKS(200UL);

  /* Wait until the network is up before creating the servers.  The
  notification is given from the network event hook. */
  ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

  // Initialize the server
  pxTCPServer = getStaticTCPServer(serverConfig, SERVER_COUNT);

  for (;;) {
    FreeRTOS_TCPServerWork(pxTCPServer, xInitialBlockTime);
  }
}



////////////////////////////////////////////////////////////////////////////////
// File manipulation decls
////////////////////////////////////////////////////////////////////////////////
size_t ff_fread(void *outBuf, const size_t itemSize, 
                const size_t itemCount, FF_FILE *file) {
  // Determine how much to read
  const BaseType_t avail = file->ulFileSize - file->curReadPosn;
  if (avail <= 0) {
    return 0;
  }

  // Read up to the smaller of (buffer or available)
  const BaseType_t reqReadSize = itemSize * itemCount;
  const BaseType_t readSize = reqReadSize < avail ? reqReadSize : avail;

  memcpy(outBuf, file->fileBuf + file->curReadPosn, readSize);
  file->curReadPosn += readSize;

  return readSize;
}

/**
 * \brief Opens the main page file
 * \return Pointer to a file
 */
FF_FILE *ff_fopen_main() {
  static FF_FILE file;
  file = (FF_FILE) {
    .fileBuf = index_min_html_gz_dat,
    .curReadPosn = 0,
    .ulFileSize = index_min_html_gz_dat_len
  };
  return &file;
}

static char jsonBuf[750] = {0};
static FF_FILE jsonFile;
#define NUM_BUF_SIZE 7
static char numBuf[NUM_BUF_SIZE];

// Helper to put the string to the buffer
void putStr(const char* src, int n) {
  if (n > jsonFile.ulFileSize)
    n = jsonFile.ulFileSize;
  memcpy(jsonFile.fileBuf, src, n);
  jsonFile.fileBuf += n;
  jsonFile.curReadPosn += n;
  jsonFile.ulFileSize -= n;
}

  //sprint_float(num, (sizeof(num)), 12.0, 0, 0);

void putPressEntry(int idx) {
  static const char syst[] = "{\"syst\":";
  static const char dias[] = ",\"dias\":";

  putStr(syst, (sizeof(syst) - 1));
  int n = usnprintf(numBuf, NUM_BUF_SIZE, "%d", 
                    (int)correctedBuffers->pressures[idx]);
  putStr(numBuf, n);

  putStr(dias, (sizeof(syst) - 1));
  n = usnprintf(numBuf, NUM_BUF_SIZE, "%d", 
                (int)correctedBuffers->pressures[idx + BUF_SIZE]);
  putStr(numBuf, n);
  putStr("}", 1);
}

void putPressures() {
  static const char head[] = "\"bloodPressure\":[";
  putStr(head, (sizeof(head) - 1));

  for (int i = BUF_SIZE; i > 0; --i) {
    const int idx = (correctedBuffers->pressureIndex + i) % BUF_SIZE;
    putPressEntry(idx);
    if (i != 1) {
      putStr(",", 1);
    }
  }
  putStr("]", 1);
}

void putTemp() {
  static const char head[] = "\"temperature\":[";
  putStr(head, (sizeof(head) - 1));

  for (int i = BUF_SIZE; i > 0; --i) {
    const int idx = (correctedBuffers->temperatureIndex + i) % BUF_SIZE;
    const int n = sprint_float(numBuf, NUM_BUF_SIZE, 
                               correctedBuffers->temperatures[idx], 0, 1);
    putStr(numBuf, n);
    if (i != 1) {
      putStr(",", 1);
    }
  }

  putStr("]", 1);
}

void putPulse() {
  static const char head[] = "\"pulseRate\":[";
  putStr(head, (sizeof(head) - 1));

  for (int i = BUF_SIZE; i > 0; --i) {
    const int idx = (correctedBuffers->pulseRateIndex + i) % BUF_SIZE;
    const int n = usnprintf(numBuf, NUM_BUF_SIZE, "%d",
                            (int)correctedBuffers->pulseRates[idx]);
    putStr(numBuf, n);
    if (i != 1) {
      putStr(",", 1);
    }
  }

  putStr("]", 1);
}

void putEKG() {
  static const char head[] = "\"ECG\":[";
  putStr(head, (sizeof(head) - 1));

  for (int i = BUF_SIZE; i > 0; --i) {
    const int idx = (correctedBuffers->ekgIndex + i) % BUF_SIZE;
    const int n = usnprintf(numBuf, NUM_BUF_SIZE, "%d",
                            (int)correctedBuffers->ekgFrequency[idx]);
    putStr(numBuf, n);
    if (i != 1) {
      putStr(",", 1);
    }
  }

  putStr("]", 1);
}

void putTF(bool val) {
  if (val) {
    putStr("true", 4);
  } else {
    putStr("false", 5);
  }
}

void putWarnAlarm() {
  static const char head[] = "\"warningAlarm\":{";
  putStr(head, (sizeof(head) - 1));

  putStr("\"bpOutOfRange\":", 15);
  putTF(warnAlarms->bpOutOfRange);

  putStr(",\"tempOutOfRange\":", 18);
  putTF(warnAlarms->tempOutOfRange);

  putStr(",\"pulseOutOfRange\":", 19);
  putTF(warnAlarms->pulseOutOfRange);

  putStr(",\"bpHighAlarm\":", 15);
  putTF(warnAlarms->bpHighAlarm);

  putStr(",\"tempHighAlarm\":", 17);
  putTF(warnAlarms->tempHighAlarm);

  putStr(",\"pulseLowAlarm\":", 17);
  putTF(warnAlarms->pulseLowAlarm);

  putStr(",\"battLowAlarm\":", 16);
  putTF(warnAlarms->battLowAlarm);

  putStr("}", 1);
}


void putBoardState() {
  static const char head[] = "\"boardState\":{";
  putStr(head, (sizeof(head) - 1));

  putStr("}", 1);
}

void putMeasurements() {
  static const char head[] = "\"measurements\":{";
  putStr(head, (sizeof(head) - 1));

  putPressures();
  putStr(",", 1);
  putTemp();
  putStr(",", 1);
  putPulse();
  putStr(",", 1);
  putEKG();

  putStr("}", 1);
}

void putJson() {
  putStr("{", 1);
  putMeasurements();
  putStr(",", 1);
  putWarnAlarm();
  putStr(",", 1);
  putBoardState();
  putStr("}", 1);
}

/**
 * \brief Opens the board status file. It is generated from the buffers and alarm status
 * \return Pointer to a file
 */
FF_FILE *ff_fopen_json() {
  jsonFile = (FF_FILE) {
    .curReadPosn = 0,
    .fileBuf = jsonBuf,
    .ulFileSize = (sizeof(jsonBuf))
  };

  static const char head[] =
    "HTTP/1.0 200 OK\r\n"
    "Content-Type: application/json; charset=utf-8\r\n"
    "Connection: close\r\n"
    "\r\n";

  putStr(head, (sizeof(head)) - 1);
  putJson();

  jsonFile.ulFileSize = jsonFile.curReadPosn;
  jsonFile.curReadPosn = 0;
  jsonFile.fileBuf = jsonBuf;

  return &jsonFile;
}
