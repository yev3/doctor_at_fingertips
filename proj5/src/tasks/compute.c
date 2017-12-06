////////////////////////////////////////////////////////////////////////////////
// Project 5, CPSC 5530 Embedded Systems, Seattle University
// Team "ARM Brewery" 
// Edward Guevara, David Pierce, and Yevgeni Kamenski
// 
// compute.c - 09/25/2017 5:17 PM
// Responsible for converting raw sensor measurements into corrected
// measurements expressed in standard units.
//
// This is free and unencumbered software released into the public domain.
////////////////////////////////////////////////////////////////////////////////

#include "tasks/system.h"
#include "ranges.h"
#include "drivers/pulse_transducer.h"

/**
 * \brief When available, the Compute task will take the data acquired by 
 * the Measure task and perform any necessary transformations or corrections.  
 * \param rawData raw data passed by the task scheduler
 */
void compute(void *rawData) {
  ComputeData *data = (ComputeData *) rawData;
  RawBuffers *raw = data->rawBuffers;                // for readability
  CorrectedBuffers *corr = data->correctedBuffers;   // for readability

  for(;;) {

    while (corr->temperatureIndex != raw->temperatureIndex) {
      uint i = ((corr->temperatureIndex + 1u) % BUF_SIZE);
      corr->temperatures[i] = ((1475*1023)-(2250*raw->temperatures[i]))/10230;
      corr->temperatureIndex = (uchar) i;
    }

    while (corr->pressureIndex != raw->pressureIndex) {
      uint i = ((corr->pressureIndex + 1u) % BUF_SIZE);
      corr->pressures[i] = 9.0f + 2.0f * raw->pressures[i];
      uint j = (uint)(i + BUF_SIZE);
      corr->pressures[j] = 6.0f + 1.5f * raw->pressures[j];
      corr->pressureIndex = (uchar) i;
    }

    while (corr->pulseRateIndex != raw->pulseRateIndex) {
      uint i = ((corr->pulseRateIndex + 1u) % BUF_SIZE);
      corr->pulseRates[i] =
              (float) (60000 * PULSE_MEASURE_SAMPLES) / raw->pulseRates[i];
      corr->pulseRateIndex = (uchar) i;
    }

    // Calculate the battery percentage from raw battery measurement
    *data->batteryPercentage =
            ((float) *data->batteryState) * 100.0f / (float) BATT_FULL_CHARGE;

    // suspend compute task in schedule and schedule display task
    taskScheduleForExec(sysTCB_ENUNCIATE);
    taskScheduleForExec(sysTCB_DISPLAY);
    vTaskSuspend(NULL);
  }
}