////////////////////////////////////////////////////////////////////////////////
// Project 5, CPSC 5530 Embedded Systems, Seattle University
// Team "ARM Brewery"
// Edward Guevara, David Pierce, and Yevgeni Kamenski
//
// computeEKG.c - 10/21/2017 4:00 PM
// Responsible for converting the raw EKG measurements into frequency reading
// using Fourier Transformation
//
// This is free and unencumbered software released into the public domain.
////////////////////////////////////////////////////////////////////////////////

#include <math.h>
#include "utils/optfft.h"
#include "tasks/system.h"
#include "RTOSDebugTrace.h"

/**
 * \brief When available, the ComputeEKG task will take the data acquired by
 * the MeasureEKG task and perform Fourier Transformation to get the frequency of
 * the EKG measurement.
 * \param rawData raw data passed by the task scheduler
 */

void computeEKG(void* rawData) {
  ComputeEKGData *data = (ComputeEKGData *) rawData;

  // EKG real and imag buffers
  int *realBuffer = data->ekgBuffer->ekgMeasures;
  static int imagBuffer[BUF_SIZE_EKG] = {0};

  // storage loc
  CorrectedBuffers *correctedBuffers = data->correctedBuffers;

  for(;;) {
    FreeRTOS_debug_printf(("Starting EKG Compute\n"));
    // Clear the imaginary buffer
    memset(imagBuffer, 0, BUF_SIZE_EKG * sizeof(imagBuffer[0]));

    //evaluated using optfft function in the optfft.c file
    const int ekgFreq = optfft(realBuffer, imagBuffer); 

    // Index of the next storage loc
    const int i = ((correctedBuffers->ekgIndex + 1) % BUF_SIZE);

    // Store the result in the cir
    correctedBuffers->ekgFrequency[i] = ekgFreq;
    correctedBuffers->ekgIndex = i;

    FreeRTOS_debug_printf(("EKG Compute done, scheduling display\n"));

    // when done computing frequency of EKG, suspend ComputeEKG Task
    taskScheduleForExec(sysTCB_DISPLAY);
    vTaskSuspend(NULL);
  }
}