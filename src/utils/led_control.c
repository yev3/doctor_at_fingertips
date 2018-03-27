/*****************************************************************************
 * Project 5, CPSC 5530 Embedded Systems, Seattle University, 2017           *
 * Team "ARM Brewery": Edward Guevara, David Pierce, and Yevgeni Kamenski    *
 *                                                                           *
 * led.c - 10/20/2017                                                        *
 * LED flashing init and on/off functions                                    *
 *                                                                           *
 * "THE BEER-WARE LICENSE" (Revision 42):                                    *
 * As long as you retain this notice, you can do whatever you want with this *
 * software. If we meet some day, and you think this software is worth it,   *
 * you can buy us a beer in return. - Team "ARM Brewery"                     *
 *****************************************************************************/
#include "utils/led_control.h"

/**
 * Initializes the LED ports
 */
void led_init() {
  // Enable the peripheral port that has the LEDs
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

  // Set the LED pins for output. The Ethernet LEDs are configured for open drn
  GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, BOARD_LED0_PORT);
  GPIOPinTypeGPIOOutputOD(GPIO_PORTF_BASE, BOARD_LED1_PORT);
  GPIOPinTypeGPIOOutputOD(GPIO_PORTF_BASE, BOARD_LED2_PORT);

  // set all LEDs initially to off
   GPIO_PORTF_DATA_R = (unsigned long) ~BOARD_LED0_PORT;
}

/**
 * \brief Turns on the Led 0
 */
void led0_on() {
  GPIOPinWrite(GPIO_PORTF_BASE, BOARD_LED0_PORT, BOARD_LED0_PORT);
}

/**
 * \brief Turns on the Led 1
 */
void led1_on() {
  GPIOPinWrite(GPIO_PORTF_BASE, BOARD_LED1_PORT, 0);
}

/**
 * \brief Turns on the Led 2
 */
void led2_on() {
  GPIOPinWrite(GPIO_PORTF_BASE, BOARD_LED2_PORT, 0);
}

/**
 * \brief Turns off the Led 0
 */
void led0_off() {
  GPIOPinWrite(GPIO_PORTF_BASE, BOARD_LED0_PORT, 0);
}

/**
 * \brief Turns off the Led 1
 */
void led1_off() {
  GPIOPinWrite(GPIO_PORTF_BASE, BOARD_LED1_PORT, BOARD_LED1_PORT);
}

/**
 * \brief Turns off the Led 2
 */
void led2_off() {
  GPIOPinWrite(GPIO_PORTF_BASE, BOARD_LED2_PORT, BOARD_LED2_PORT);
}