/*****************************************************************************
 * Project 3, CPSC 5530 Embedded Systems, Seattle University, 2017           *
 * Team "ARM Brewery": Edward Guevara, David Pierce, and Yevgeni Kamenski    *
 *                                                                           *
 * led_control.h - 10/20/2017                                                        *
 * LED flashing init and on/off functions                                    *
 *                                                                           *
 * "THE BEER-WARE LICENSE" (Revision 42):                                    *
 * As long as you retain this notice, you can do whatever you want with this *
 * software. If we meet some day, and you think this software is worth it,   *
 * you can buy us a beer in return. - Team "ARM Brewery"                     *
 *****************************************************************************/
#pragma once

#include <inc/lm3s8962.h>
#include <inc/hw_types.h>
#include <driverlib/gpio.h>
#include <inc/hw_memmap.h>
#include <driverlib/sysctl.h>

#define BOARD_LED0_PORT GPIO_PIN_0  /* LED0 Physical Port */
#define BOARD_LED1_PORT GPIO_PIN_2  /* LED0 Physical Port */
#define BOARD_LED2_PORT GPIO_PIN_3  /* LED0 Physical Port */

/**
 * \brief Initializes the LED ports
 */
void led_init();

/**
 * \brief Turns on the Led 0
 */
void led0_on();

/**
 * \brief Turns on the Led 2
 */
void led1_on();

/**
 * \brief Turns on the Led 2
 */
void led2_on();

/**
 * \brief Turns off the Led 0
 */
void led0_off();

/**
 * \brief Turns off the Led 1
 */
void led1_off();

/**
 * \brief Turns off the Led 2
 */
void led2_off();
