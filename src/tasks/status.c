////////////////////////////////////////////////////////////////////////////////
// Project 5, CPSC 5530 Embedded Systems, Seattle University
// Team "ARM Brewery" 
// Edward Guevara, David Pierce, and Yevgeni Kamenski
// 
// status.c - 09/27/2017 11:47 AM
// Responsible for managing the battery subsystem.
// 
// This is free and unencumbered software released into the public domain.
////////////////////////////////////////////////////////////////////////////////

#include "tasks/system.h"

/**
 * \brief Adjusts the status of the raw battery values.
 * \param rawData raw sensor data of the task
 */
void status(void *rawData) {
  // This task is always scheduled to run, but is active only once every 5 secs
  StatusData *data = (StatusData *) rawData;

  while (1) {

    // Decrement the battery state raw value when greater than 0
    if (*data->batteryState > 0) --*data->batteryState;

    //schedule the compute task to update the battery
    taskScheduleForExec(sysTCB_COMPUTE);
    vTaskDelay(pdMS_TO_TICKS(5000));
  }
}