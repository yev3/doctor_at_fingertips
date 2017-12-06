/*****************************************************************************
 * Project 5, CPSC 5530 Embedded Systems, Seattle University, 2017           *
 * Team "ARM Brewery": Edward Guevara, David Pierce, and Yevgeni Kamenski    *
 *                                                                           *
 * pressure_cuff.c - 10/21/2017                                                *
 * PWM speaker init and beep on/off functions                                *
 *                                                                           *
 * "THE BEER-WARE LICENSE" (Revision 42):                                    *
 * As long as you retain this notice, you can do whatever you want with this *
 * software. If we meet some day, and you think this software is worth it,   *
 * you can buy us a beer in return. - Team "ARM Brewery"                     *
 *****************************************************************************/

#pragma once

#include <inc/hw_ints.h>
#include <inc/hw_memmap.h>
#include <inc/hw_types.h>
#include <driverlib/interrupt.h>
#include <driverlib/sysctl.h>
#include <driverlib/timer.h>
#include "utils/hardware_timer.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

typedef struct PressureCuffState{
  bool measureSystolic;   ///< Flag to measure systolic
  bool measureDiastolic;  ///< Flag to measure diastolic
  bool begin;             ///< Flag to begin measuring pressure
  float current;          ///< initial pressure
}PressureCuffState_t;


extern PressureCuffState_t pressureCuffState;
extern QueueHandle_t measurementCommands;

#define PRESSURE_MIN 20     /* Raw sensor units */
#define PRESSURE_MAX 100    /* Raw sensor units */

/**
 * \brief Initializes the pulse measurement driver
 */
//void pressure_cuff_init();

/**
 * \brief Increases the pressure of the cuff by default percent step
 */
void IncreasePressure();

/**
 * \brief Decreases the pressure of the cuff by default percent step
 */
void DecreasePressure();

/**
 * \brief Resets pressure to minimum pressure
 */
void ResetPressure();

/**
 * \brief Resets pressure to minimum pressure
 */
float GetPressure();