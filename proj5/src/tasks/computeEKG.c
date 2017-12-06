////////////////////////////////////////////////////////////////////////////////
// Project 3, CPSC 5530 Embedded Systems, Seattle University
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

/**
 * \brief When available, the ComputeEKG task will take the data acquired by
 * the MeasureEKG task and perform Fourier Transformation to get the frequency of
 * the EKG measurement.
 * \param rawData raw data passed by the task scheduler
 */

void computeEKG(void* rawData) {
  static int imagBuffer[BUF_SIZE_EKG] = {0};
  while (1) {
    ComputeEKGData *data = (ComputeEKGData *) rawData;
    EKGBuffer *ekgBuffer = data->ekgBuffer;
    CorrectedBuffers *correctedBuffers = data->correctedBuffers;
    for(uint i = 0; i < BUF_SIZE_EKG; i++) imagBuffer[i] = 0;
    //evaluated using optfft function in the optfft.c file
    correctedBuffers->ekgFrequency[++correctedBuffers->ekgIndex] =
            optfft(ekgBuffer->ekgMeasures, imagBuffer);

    // when done computing frequency of EKG, suspend ComputeEKG Task
    taskScheduleForExec(sysTCB_DISPLAY);
    vTaskSuspend(NULL);
  }
}