/*****************************************************************************
 * Project 3, CPSC 5530 Embedded Systems, Seattle University, 2017           *
 * Team "ARM Brewery": Edward Guevara, David Pierce, and Yevgeni Kamenski    *
 *                                                                           *
 * key_scan.c - 10/14/2017                                                   *
 *                                                                           *
 * "THE BEER-WARE LICENSE" (Revision 42):                                    *
 * As long as you retain this notice, you can do whatever you want with this *
 * software. If we meet some day, and you think this software is worth it,   *
 * you can buy us a beer in return. - Team "ARM Brewery"                     *
 *****************************************************************************/

#include <inc/hw_memmap.h>
#include <driverlib/sysctl.h>
#include <driverlib/gpio.h>
#include <stdint.h>
#include "tasks/system.h"
#include "tasks/commands.h"
/******************************************************************************
 * Button state    0 - Up
 *                 1 - Down
 *                 2 - Left
 *                 3 - Right
 *                 4 - Select
 *****************************************************************************/

static uchar buttonsChangedDown = 0x00; ///< Bitpacked btn changed down state
static uint32_t keyState = 0;           ///< Bit-shifted state used to debounce


/*
 * Queue to send the key strokes to, initialized on task start
 */
static QueueHandle_t keyQueue;

/**
 * \brief Helper to send a key stroke
 * \param key Key press to enqueue
 */
void keyPress(KeyPress_t key) {
  // Does not wait when the queue is full, just disregard the key
  xQueueSend(keyQueue, &key, 0);  
}

/**
 * \brief Scans and debounces key presses and updates new key press flags
 * \param rawData raw data passed by the task scheduler
 */
void key_scan(void *taskArg) {
  keyQueue = *(QueueHandle_t*) taskArg;

  for(;;) {
    // raw key states read from ports
    uchar keyRead = (uchar)(
            (GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_1) << 3) |
            GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_0 | GPIO_PIN_1 |
                                         GPIO_PIN_2 | GPIO_PIN_3));

    // shift the old states to the left, update the current state in the LSBs
    keyState = (keyState << 8) | (unsigned long) keyRead;

    // Note: when the key goes down, the bits 16 - 23 will be in up state,
    // and the bits 0-15 will be in the down state, with bounces settled
    // the result will be stored in the lower 8 bits, masked, and returned
    buttonsChangedDown = (uchar)((keyState >> 16) & ~(keyState >> 8) &
                                 ~keyState & ((1 << 5) - 1));

    // Update the key state variables
    if (buttonsChangedDown > 0) {
      if(HWREGBITB(&buttonsChangedDown, 0)) keyPress(cmdKEY_UP);
      if(HWREGBITB(&buttonsChangedDown, 1)) keyPress(cmdKEY_DOWN);
      if(HWREGBITB(&buttonsChangedDown, 2)) keyPress(cmdKEY_LEFT);
      if(HWREGBITB(&buttonsChangedDown, 3)) keyPress(cmdKEY_RIGHT);
      if(HWREGBITB(&buttonsChangedDown, 4)) keyPress(cmdKEY_SELECT);
    }

    vTaskDelay(pdMS_TO_TICKS(5));
  }
}