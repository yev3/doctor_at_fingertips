////////////////////////////////////////////////////////////////////////////////
// Project 5, CPSC 5530 Embedded Systems, Seattle University
// Team "ARM Brewery"
// Edward Guevara, David Pierce, and Yevgeni Kamenski
//
// tasks.c - 10/21/2017 4:06 PM
// Responsible for setting up the global variable sharing between tasks.
// Declares initialization functions for Task Control Blocks used by the scheduler.
//
// This is free and unencumbered software released into the public domain.
////////////////////////////////////////////////////////////////////////////////

#include "drivers/pressure_cuff.h"
#include "utils/ustdlib.h"
#include <stdint.h>
#include "FreeRTOS_IP.h"
#include "tasks/system.h"

PressureCuffState_t pressureCuffState = {
  false, false, false, PRESSURE_MIN
};

// For readability
#define cuff (pressureCuffState)

bool measureSystolic = false;                 ///< Flag to measure systolic
float triggerPressure;
//bool measureDiastolic = false;                ///< Flag to measure diastolic
//bool beginPressureMeasurement = false;        ///< Flag to begin measuring pressure
//static float currentPressure = PRESSURE_MIN;  ///< initial pressure


// Sends off a signal to measurement task for processing
void TriggerMeasureInterrupt() {
  MeasureSelection selection = 
    measureSystolic ? MEASURE_PRESSURE_SYST : MEASURE_PRESSURE_DIAS;
  xQueueSend(measurementCommands, &selection, portMAX_DELAY);
}

// Interrupt will randomly trigger between 75-90 raw
void randSyst() {
  triggerPressure = 75.0 + ((float)(uint32_t)urand() * 15.0 / (float)UINT32_MAX);
}

// Interrupt will randomly trigger between 33-53 raw
void randDias() {
  triggerPressure = 33.0 + ((float)(uint32_t)urand() * 20.0 / (float)UINT32_MAX);
}

/**
 * \brief Increases the pressure of the cuff by default percent step
 */
void IncreasePressure(){
  // increase pressure by percent step
  cuff.current *= 1.1;

  if (measureSystolic && cuff.current > triggerPressure) {
    cuff.current = triggerPressure;
    TriggerMeasureInterrupt();
    measureSystolic = false;
    randDias();
  }

  if(cuff.current > PRESSURE_MAX) 
    cuff.current = PRESSURE_MAX;
}

/**
 * \brief Decreases the pressure of the cuff by default percent step
 */
void DecreasePressure(){
  cuff.current *= 0.9;

  if (!measureSystolic && cuff.current < triggerPressure) {
    cuff.current = triggerPressure;
    TriggerMeasureInterrupt();
    measureSystolic = true;
    randSyst();
  }

  if(cuff.current < PRESSURE_MIN)
    cuff.current = PRESSURE_MIN;
}


/**
 * \brief Resets pressure to minimum pressure
 */
void ResetPressure() {
  cuff.current = PRESSURE_MIN;
  measureSystolic = true;
  randSyst();
  FreeRTOS_debug_printf(("Resetting pressure, will randomly trigger at %.2f\n",
    triggerPressure));
}

/**
 * \brief Get current pressure of the cuff
 */
float GetPressure() { return cuff.current; }