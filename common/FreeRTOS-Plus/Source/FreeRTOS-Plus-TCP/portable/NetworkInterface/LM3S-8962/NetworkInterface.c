/*
 * Some constants, hardware definitions and comments taken from ST's HAL driver
 * library, COPYRIGHT(c) 2015 STMicroelectronics.
 */

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



#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "inc/hw_ethernet.h"

#include "driverlib/ethernet.h"
#include "driverlib/flash.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "utils/ustdlib.h"

#include <stdint.h>

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

/* FreeRTOS+TCP includes. */
#include "FreeRTOS_IP.h"
#include "FreeRTOS_Sockets.h"
#include "FreeRTOS_IP_Private.h"
#include "NetworkBufferManagement.h"
#include "NetworkInterface.h"


/* Default the size of the stack used by the EMAC deferred handler task to 4x
the size of the stack used by the idle task - but allow this to be overridden in
FreeRTOSConfig.h as configMINIMAL_STACK_SIZE is a user definable constant. */
#ifndef configEMAC_TASK_STACK_SIZE
    #define configEMAC_TASK_STACK_SIZE ( 4 * configMINIMAL_STACK_SIZE )
#endif

/*
 * A deferred interrupt handler task that processes
 */
static void prvEMACDeferredInterruptHandlerTask( void *pvParameters );
/*-----------------------------------------------------------*/


/* Holds the handle of the task used as a deferred interrupt processor.  The
handle is used so direct notifications can be sent to the task for all EMAC/DMA
related interrupts. */
static TaskHandle_t xEMACTaskHandle = NULL;

extern unsigned char ucMACAddress[8];

BaseType_t xNetworkInterfaceInitialise( void )
{

    if( xEMACTaskHandle == NULL )
    {
        //
        // Enable and Reset the Ethernet Controller.
        // Also, disable RX and TX while modifying the parameters.
        //
        SysCtlPeripheralEnable(SYSCTL_PERIPH_ETH);
        SysCtlPeripheralReset(SYSCTL_PERIPH_ETH);

        /* Enable the Ethernet Controller transmitter and receiver. */
        EthernetEnable(ETH_BASE);

        /* Do whatever else is needed to initialize interface. */
        /* Disable all Ethernet Interrupts. */
        EthernetIntDisable(ETH_BASE, (ETH_INT_PHY | ETH_INT_MDIO | ETH_INT_RXER |
                           ETH_INT_RXOF | ETH_INT_TX | ETH_INT_TXER | ETH_INT_RX));
        BaseType_t temp = EthernetIntStatus(ETH_BASE, false);
        EthernetIntClear(ETH_BASE, temp);

        //
        // Initialize the Ethernet controller for operation
        //
        EthernetInitExpClk(ETH_BASE, SysCtlClockGet());

        //
        // Configure the Ethernet controller for normal operation
        // Enable TX Duplex Mode
        // Enable TX Padding
        //
        EthernetConfigSet(ETH_BASE, (ETH_CFG_TX_DPLXEN | ETH_CFG_TX_CRCEN | ETH_CFG_TX_PADEN | ETH_CFG_RX_BADCRCDIS));
        HWREGBITW(ETH_BASE + MAC_O_TCTL, MAC_TCTL_CRC);


        EthernetMACAddrSet(ETH_BASE, ucMACAddress);

        //
        // Enable Port F for Ethernet LEDs.
        //  LED0        Bit 3   Output
        //  LED1        Bit 2   Output
        //
        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
        GPIOPinTypeEthernetLED(GPIO_PORTF_BASE, GPIO_PIN_2 | GPIO_PIN_3);

        /* Clear the receive buffer */
        HWREGBITW(ETH_BASE + MAC_O_RCTL, MAC_RCTL_RSTFIFO);

        /* The handler task is created at the highest possible priority to
        ensure the interrupt handler can return directly to it. */
        xTaskCreate( prvEMACDeferredInterruptHandlerTask, "EMAC", configEMAC_TASK_STACK_SIZE, NULL, configMAX_PRIORITIES - 1, &xEMACTaskHandle );

        configASSERT( xEMACTaskHandle );

        // To let the link negotiate
        vTaskDelay( 2000 );



        IntPrioritySet( INT_ETH, configMAX_SYSCALL_INTERRUPT_PRIORITY + ( 1 << 5 ) );

        /* Enable the Ethernet Interrupt handler. */
        IntEnable(INT_ETH);

        /* Enable Ethernet TX and RX Packet Interrupts. */
        EthernetIntEnable(ETH_BASE, ETH_INT_RX);

    }

    /* When returning non-zero, the stack will become active and
    start DHCP (in configured) */
    return pdTRUE;
}
/*-----------------------------------------------------------*/

BaseType_t xNetworkInterfaceOutput( NetworkBufferDescriptor_t * const pxDescriptor, BaseType_t bReleaseAfterSend )
{
    /* Simple network interfaces (as opposed to more efficient zero copy network
    interfaces) just use Ethernet peripheral driver library functions to copy
    data from the FreeRTOS+TCP buffer into the peripheral driver's own buffer.
    This example assumes SendData() is a peripheral driver library function that
    takes a pointer to the start of the data to be sent and the length of the
    data to be sent as two separate parameters.  The start of the data is located
    by pxDescriptor->pucEthernetBuffer.  The length of the data is located
    by pxDescriptor->xDataLength. */


    // Wait until there is space
    while(!EthernetSpaceAvail(ETH_BASE)) {
        vTaskDelay(pdMS_TO_TICKS(10));
    }

    BaseType_t xReturn = EthernetPacketPutNonBlocking(ETH_BASE, pxDescriptor->pucEthernetBuffer, pxDescriptor->xDataLength);

    configASSERT(xReturn > 0);

    /* Call the standard trace macro to log the send event. */
    iptraceNETWORK_INTERFACE_TRANSMIT();

    if( bReleaseAfterSend != pdFALSE )
    {
        /* It is assumed SendData() copies the data out of the FreeRTOS+TCP Ethernet
        buffer.  The Ethernet buffer is therefore no longer needed, and must be
        freed for re-use. */
        vReleaseNetworkBufferAndDescriptor( pxDescriptor );
    }

    return xReturn;
}
/*-----------------------------------------------------------*/


/* The deferred interrupt handler is a standard RTOS task.  FreeRTOS's centralised
deferred interrupt handling capabilities can also be used. */
static void prvEMACDeferredInterruptHandlerTask( void *pvParameters )
{
    NetworkBufferDescriptor_t *pxBufferDescriptor;
    size_t xBytesReceived;
    BaseType_t temp;             // Used to hold size and first 2 bytes when reading first byte from FIFO
    unsigned long *buf;     // Used to help read from ETH FIFO

    /* Used to indicate that xSendEventStructToIPTask() is being called because
    of an Ethernet receive event. */
    IPStackEvent_t xRxEvent;

    for( ;; )
    {
        /* Wait for the Ethernet MAC interrupt to indicate that another packet
        has been received.  The task notification is used in a similar way to a
        counting semaphore to count Rx events, but is a lot more efficient than
        a semaphore. */
        ulTaskNotifyTake( pdFALSE, portMAX_DELAY );

        while(EthernetPacketAvail(ETH_BASE)) {

            /* See how much data was received.  Here it is assumed ReceiveSize() is
            a peripheral driver function that returns the number of bytes in the
            received Ethernet frame. */
            temp = HWREG(ETH_BASE + MAC_O_DATA);
            xBytesReceived = temp & 0xFFFF;

            if( xBytesReceived > 0 )
            {
                // Ignore the size read
                xBytesReceived -= 4; 
                
                /* Allocate a network buffer descriptor that points to a buffer
                large enough to hold the received frame.  As this is the simple
                rather than efficient example the received data will just be copied
                into this buffer. */
                pxBufferDescriptor = pxGetNetworkBufferWithDescriptor( xBytesReceived, 0 );

                if( pxBufferDescriptor != NULL )
                {
                    /* pxBufferDescriptor->pucEthernetBuffer now points to an Ethernet
                    buffer large enough to hold the received data.  Copy the
                    received data into pcNetworkBuffer->pucEthernetBuffer.  Here it
                    is assumed ReceiveData() is a peripheral driver function that
                    copies the received data into a buffer passed in as the function's
                    parameter.  Remember! While is is a simple robust technique -
                    it is not efficient.  An example that uses a zero copy technique
                    is provided further down this page. */
                  
                    
                    // Temp to traverse in 4-byte chunks
                    buf = (unsigned long*)pxBufferDescriptor->pucEthernetBuffer;
                    
                    // Already read 2 bytes when read size
                    *buf = (temp>>16);
                    buf = (unsigned long*)(pxBufferDescriptor->pucEthernetBuffer + 2);
                    
                    for (size_t i = 0; i < xBytesReceived; i += 4) {
                        *buf++ = HWREG(ETH_BASE + MAC_O_DATA);
                    }

                    pxBufferDescriptor->xDataLength = xBytesReceived;

                    /* See if the data contained in the received Ethernet frame needs
                    to be processed.  NOTE! It is preferable to do this in
                    the interrupt service routine itself, which would remove the need
                    to unblock this task for packets that don't need processing. */
                    if( eConsiderFrameForProcessing( pxBufferDescriptor->pucEthernetBuffer )
                                                                          == eProcessBuffer )
                    {
                        /* The event about to be sent to the TCP/IP is an Rx event. */
                        xRxEvent.eEventType = eNetworkRxEvent;

                        /* pvData is used to point to the network buffer descriptor that
                        now references the received data. */
                        xRxEvent.pvData = ( void * ) pxBufferDescriptor;

                        /* Send the data to the TCP/IP stack. */
                        if( xSendEventStructToIPTask( &xRxEvent, 0 ) == pdFALSE )
                        {
                            /* The buffer could not be sent to the IP task so the buffer
                            must be released. */
                            vReleaseNetworkBufferAndDescriptor( pxBufferDescriptor );

                            /* Make a call to the standard trace macro to log the
                            occurrence. */
                            iptraceETHERNET_RX_EVENT_LOST();
                        }
                        else
                        {
                            /* The message was successfully sent to the TCP/IP stack.
                            Call the standard trace macro to log the occurrence. */
                            iptraceNETWORK_INTERFACE_RECEIVE();
                        }
                    }
                    else
                    {
                        /* The Ethernet frame can be dropped, but the Ethernet buffer
                        must be released. */
                        vReleaseNetworkBufferAndDescriptor( pxBufferDescriptor );
                    }
                }
                else
                {
                    /* The event was lost because a network buffer was not available.
                    Call the standard trace macro to log the occurrence. */
                    iptraceETHERNET_RX_EVENT_LOST();

                    // Still need to drain the buffer
                    for (int i = 0; i < xBytesReceived; i += 4) {
                        temp = HWREG(ETH_BASE + MAC_O_DATA);
                    }
                }

            }
        }

        EthernetIntEnable(ETH_BASE, ETH_INT_RX);
    }
}

/*-----------------------------------------------------------*/


void ETHInterruptHandler(void)
{
BaseType_t xHigherPriTaskWoken = pdFALSE;

    /* Clear the interrupt. */
    unsigned long ulTemp = EthernetIntStatus( ETH_BASE, pdFALSE );
    EthernetIntClear( ETH_BASE, ulTemp );

    /* Was it an Rx interrupt? */
    if( ulTemp & ETH_INT_RX )
    {
        vTaskNotifyGiveFromISR( xEMACTaskHandle, &( xHigherPriTaskWoken ) );
        // The receiving task will re-enable the interrupts
        EthernetIntDisable( ETH_BASE, ETH_INT_RX );
    }

    /* Switch to the uIP task. */
    portYIELD_FROM_ISR( xHigherPriTaskWoken );
}

