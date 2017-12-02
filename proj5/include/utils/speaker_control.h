/*****************************************************************************
 * Project 3, CPSC 5530 Embedded Systems, Seattle University, 2017           *
 * Team "ARM Brewery": Edward Guevara, David Pierce, and Yevgeni Kamenski    *
 *                                                                           *
 * speaker_control.h - 10/21/2017                                                *
 *                                                                           *
 * "THE BEER-WARE LICENSE" (Revision 42):                                    *
 * As long as you retain this notice, you can do whatever you want with this *
 * software. If we meet some day, and you think this software is worth it,   *
 * you can buy us a beer in return. - Team "ARM Brewery"                     *
 *****************************************************************************/
#pragma once

#include <inc/lm3s8962.h>
#include <inc/hw_memmap.h>
#include <inc/hw_types.h>
#include <inc/hw_pwm.h>
#include <driverlib/pwm.h>
#include <driverlib/gpio.h>
#include <driverlib/sysctl.h>
#include "types.h"

/**
 * \brief Initializes the PWM port (G)
 */
void speaker_init();

/**
 * \brief Turns on the board speaker
 */
void sound_on();

/**
 * \brief Turns off the board speaker
 */
void sound_off();
