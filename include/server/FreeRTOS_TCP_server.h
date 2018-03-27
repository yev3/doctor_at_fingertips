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

/*
	Some code which is common to TCP servers like HTTP en FTP
*/

#ifndef FREERTOS_TCP_SERVER_H
#define	FREERTOS_TCP_SERVER_H

#ifdef __cplusplus
extern "C" {
#endif

typedef BaseType_t ( * FTCPWorkFunction ) ( struct xTCP_CLIENT * /* pxClient */ );
typedef void ( * FTCPDeleteFunction ) ( struct xTCP_CLIENT * /* pxClient */ );

#if( ipconfigFTP_HAS_RECEIVED_HOOK != 0 )
	extern void vApplicationFTPReceivedHook( const char *pcFileName, uint32_t ulSize, struct xFTP_CLIENT *pxFTPClient );
	extern void vFTPReplyMessage( struct xFTP_CLIENT *pxFTPClient, const char *pcMessage );
#endif /* ipconfigFTP_HAS_RECEIVED_HOOK != 0 */

#if( ipconfigFTP_HAS_USER_PASSWORD_HOOK != 0 )
	/*
	 * Function is called when a user name has been submitted.
	 * The function may return a string such as: "331 Please enter your password"
	 * or return NULL to use the default reply.
	 */
	extern const char *pcApplicationFTPUserHook( const char *pcUserName );
#endif /* ipconfigFTP_HAS_USER_PASSWORD_HOOK */

#if( ipconfigFTP_HAS_USER_PASSWORD_HOOK != 0 )
	/*
	 * Function is called when a password was received.
	 * Return positive value to allow the user
	 */
	extern BaseType_t xApplicationFTPPasswordHook( const char *pcUserName, const char *pcPassword );
#endif /* ipconfigFTP_HAS_USER_PASSWORD_HOOK */

typedef enum xSERVER_CONFIG_TYPE
{
  serverTYPE_HTTP = 1,
  serverTYPE_TELNET = 2
} ServerConfigType_t;

typedef struct xSERVER_CONF
{
  ServerConfigType_t serverType;
  BaseType_t xPortNumber;		/* e.g. 80, 8080, 21 */
  BaseType_t xBackLog;			/* e.g. 10, maximum number of connected TCP clients */
  FTCPWorkFunction workFn;
  FTCPDeleteFunction deleteFn;
  Socket_t xSocket;
} ServerConfig_t;


struct xTCP_SERVER;
typedef struct xTCP_SERVER TCPServer_t;

TCPServer_t *getStaticTCPServer(ServerConfig_t config[], const BaseType_t numServers);
void FreeRTOS_TCPServerWork( TCPServer_t *pxServer, TickType_t xBlockingTime );

#if( ipconfigSUPPORT_SIGNALS != 0 )
	/* FreeRTOS_TCPServerWork() calls select().
	The two functions below provide a possibility to interrupt
	the call to select(). After the interruption, resume
	by calling FreeRTOS_TCPServerWork() again. */
	BaseType_t FreeRTOS_TCPServerSignal( TCPServer_t *pxServer );
	BaseType_t FreeRTOS_TCPServerSignalFromISR( TCPServer_t *pxServer, BaseType_t *pxHigherPriorityTaskWoken );
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* FREERTOS_TCP_SERVER_H */
