////////////////////////////////////////////////////////////////////////////////
// Project 5, CPSC 5530 Embedded Systems, Seattle University
// Team "ARM Brewery" 
// Edward Guevara, David Pierce, and Yevgeni Kamenski
// 
// measure.c - 09/25/2017 5:17 PM
// Responsible for collecting raw measurement values from sensors ready for
// use by the compute task.
//
// This is free and unencumbered software released into the public domain.
////////////////////////////////////////////////////////////////////////////////

#include <assert.h>
#include "drivers/pulse_transducer.h"
#include "drivers/pressure_cuff.h"
#include "driverlib/adc.h"
#include "tasks/system.h"


/*******************************************************************************
 * Temperature measurement section
 ******************************************************************************/

extern uint getRawTempVal();

/**
 * \brief Measure temperature from chip sensor.
 * \param buf pointer to the raw buffers of the measure task
 * \return true when measurement is complete, false otherwise
 */
bool processTempMeasure(RawBuffers *buf) {

  uchar newIdx = (uchar)((buf->temperatureIndex + 1) % BUF_SIZE);
  uint *newT = buf->temperatures + newIdx;
  buf->temperatureIndex = newIdx;

  *newT = getRawTempVal();
  
  // Always finishes measurement in one call
  return true;
}

static bool systolicMeasured = false;
static bool diastolicMeasured = false;
static uint systolicMeasurement;
static uint diastolicMeasurement;

static void measureSystolic() {
  systolicMeasured = true;
  diastolicMeasured = false;
  systolicMeasurement = (uint)GetPressure();
}

static void measureDiastolic() {
  if (systolicMeasured) {
    diastolicMeasured = true;
    diastolicMeasurement = (uint)GetPressure();
  }
}

static bool recordRawPressureMeasurements(RawBuffers *buf) {
  if (systolicMeasured && diastolicMeasured) {
    systolicMeasured = false;
    diastolicMeasured = false;
    // determine the old and new array indices in the buffer
    uint newSysIdx = (buf->pressureIndex + 1) % BUF_SIZE;
    uint newDiaIdx = newSysIdx + BUF_SIZE;

    // update the last written circ buffer index
    buf->pressures[newSysIdx] = systolicMeasurement;
    buf->pressures[newDiaIdx] = diastolicMeasurement;
    buf->pressureIndex = (uchar)newSysIdx;
    return true;
  }
  return false;
}

/******************************************************************************
 * Pulse measurement section
 ******************************************************************************/

static uint rawPulseMeasurement = 0;  ///< Raw pulse aggregate of samples
static uint rawSamplesMeasured = 0;   ///< # of samples included in the aggrega
static bool isPulseMeasuring = false; ///< set to T when pulse is currently meas
static bool isPulseFinished = false;  ///< set to T when done sampling

/**
 * \brief Simulated pulse interrupt routine. Gets called by the hw timer
 * \param ms number of milliseconds since last pulse spike
 */
static void pulseMeasureHook(int ms) {
  rawPulseMeasurement += ms;
  if (++rawSamplesMeasured == PULSE_MEASURE_SAMPLES) {
    isPulseFinished = true;
    pulseHookClear();
  }
}

/**
 * \brief Marks the start of the pulse measurement.
 */
static void startPulseMeasurement() {
  rawPulseMeasurement = 0;
  rawSamplesMeasured = 0;
  isPulseMeasuring = true;
  isPulseFinished = false;
  pulseHookSet(pulseMeasureHook);
}

/**
 * \brief Starts or continues pulse measurement.
 * \param buf pointer to the task raw buffers
 * \return True if the measurement is complete, false otherwise
 */
static bool processPulseRate(RawBuffers *buf) {
  if (!isPulseMeasuring) {
    startPulseMeasurement();
  } else if (isPulseFinished) {
    // process it
    buf->pulseRateIndex = (uchar) ((buf->pulseRateIndex + 1) % BUF_SIZE);
    buf->pulseRates[buf->pulseRateIndex] = rawPulseMeasurement;
    startPulseMeasurement();
  }
  return isPulseFinished;
}

/**
 * \brief Process raw measurement data.
 * \param rawData raw data passed by the task scheduler
 */
void measure(void *rawData) {
  MeasureData *data = (MeasureData *) rawData;
  RawBuffers *buf = data->rawBuffers;
  QueueHandle_t measureCmds = *data->measurementCommands;
  MeasureSelection requestedMeasurement;

  for(;;) {
    if (xQueueReceive(measureCmds, &requestedMeasurement,
      portMAX_DELAY) == pdPASS) {

      bool measurementComplete = false;  ///< Flag to determine when completed

      // Process the measurement based on the user's selection
      switch (requestedMeasurement) {
      case MEASURE_PRESSURE_SYST: {
        // Process raw pressure data and store in the next circ buf location
        measureSystolic();
        break;
      }
      case MEASURE_PRESSURE_DIAS: {
        // Process raw pressure data and store in the next circ buf location
        measureDiastolic();
        measurementComplete = recordRawPressureMeasurements(buf);
        break;
      }
      case MEASURE_TEMPERATURE:
        // Process raw temperature data and store in the next circ buf location
        measurementComplete = processTempMeasure(buf);
        break;
      case MEASURE_PULSE:
        // Process raw pulse rate data and store in the next circ buf location
        measurementComplete = processPulseRate(buf);
        break;
      default:
        configASSERT(0);
        break;
      }

      // When measurement is completed, suspend measure task in schedule
      // and schedule the compute task. Otherwise, this method will keep being
      // scheduled for execution
      if (measurementComplete) {
        taskScheduleForExec(sysTCB_COMPUTE);
      }
    }
  }
}