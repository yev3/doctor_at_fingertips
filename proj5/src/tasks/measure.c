////////////////////////////////////////////////////////////////////////////////
// Project 3, CPSC 5530 Embedded Systems, Seattle University
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
#include "driverlib/adc.h"
#include "system_tasks.h"

/*******************************************************************************
 * Initialization of Temperature Sensor
 ******************************************************************************/
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
/**
 * \brief Initializes the temperature sensor on the Stellaris board.
 * Taken from the StellarisWare\examples\peripherals\adc temperature_sensor.c
 * example.
 */
void temperature_init() {
  
    //
    // The ADC0 peripheral must be enabled for use.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);

    //
    // Enable sample sequence 3 with a processor signal trigger.  Sequence 3
    // will do a single sample when the processor sends a singal to start the
    // conversion.  Each ADC module has 4 programmable sequences, sequence 0
    // to sequence 3.  This example is arbitrarily using sequence 3.
    //
    ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);

    //
    // Configure step 0 on sequence 3.  Sample the temperature sensor
    // (ADC_CTL_TS) and configure the interrupt flag (ADC_CTL_IE) to be set
    // when the sample is done.  Tell the ADC logic that this is the last
    // conversion on sequence 3 (ADC_CTL_END).  Sequence 3 has only one
    // programmable step.  Sequence 1 and 2 have 4 steps, and sequence 0 has
    // 8 programmable steps.  Since we are only doing a single conversion using
    // sequence 3 we will only configure step 0.  For more information on the
    // ADC sequences and steps, reference the datasheet.
    //
    ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_TS | ADC_CTL_IE |
                             ADC_CTL_END);

    //
    // Since sample sequence 3 is now configured, it must be enabled.
    //
    ADCSequenceEnable(ADC0_BASE, 3);

    //
    // Clear the interrupt status flag.  This is done to make sure the
    // interrupt flag is cleared before we sample.
    //
    ADCIntClear(ADC0_BASE, 3);
}

/*******************************************************************************
 * Temperature measurement section
 ******************************************************************************/

/**
 * \brief Increments the variable by 2 every even numbered time the function
 * is called and decrements by 1 every odd numbered time the function is
 * called until the value of the variable exceeds 50.  The number 0 is
 * considered to be even.  Thereafter, reverses the process until the value 
 * of the variable falls below 15.  Then, once again reverses the process.
 * \param buf pointer to the raw buffers of the measure task
 * \return true when measurement is complete, false otherwise
 */
bool processTemp(RawBuffers *buf) {

  uchar oldIdx = buf->temperatureIndex;
  uchar newIdx = (uchar) ((oldIdx + 1) % BUF_SIZE);
  uint *oldT = buf->temperatures + oldIdx;
  uint *newT = buf->temperatures + newIdx;
  buf->temperatureIndex = newIdx;

  // Trigger the ADC conversion.
  ADCProcessorTrigger(ADC0_BASE, 3);

  // Wait for conversion to be completed.
  while(!ADCIntStatus(ADC0_BASE, 3, false))
  {
  }

  // Read ADC Value.
  ADCSequenceDataGet(ADC0_BASE, 3, newT);
  
  // Always finishes measurement in one call
  return true;
}

/**
 * \brief Increments the variable by 3 every even numbered time the function
 * is called and decrements by 1 every odd numbered time the function is 
 * called until the value of the variable exceeds 100.  At such time, 
 * sets a variable indicating that the systolic pressure measurement is 
 * complete. The number 0 is considered to be even. When the diastolic 
 * measurement is complete, repeat the process. 
 * \param p current raw pressure value 
 * \return next generated pressure value
 */
uint genSystP(uint p) {
  static bool isEvenCall = false;    ///< true every other call, first call even

  // set this call to be the opposite of the previous even/odd call state
  isEvenCall = !isEvenCall;

  // Even calls increment the pressure by 3
  // Odd calls decrement the pressure by 1
  return isEvenCall ? (p + 3) : (p - 1);
}

/**
 * \brief Decrements the variable by 2 every even numbered time the function 
 * is called and increments by 1 every odd numbered time the function is 
 * called until the value of the variable drops below 40.  At such time, 
 * sets a variable indicating that the diastolic pressure measurement 
 * is complete. The number 0 is considered to be even. When the 
 * systolic measurement is complete, repeats the process. 
 * \param p current raw diastolic pressuve value
 * \return next generated diastolic pressure value
 */
uint genDiastP(uint p) {
  static bool isEvenCall = false; ///< true every other call, first call is even

  // set this call to be the opposite of the previous even/odd call state
  isEvenCall = !isEvenCall;

  // Even calls decrement the pressure by 2
  // Odd calls increment the pressure by 1
  return isEvenCall ? (p - 2) : (p + 1);
}

/**
 * \brief Processes the pressure measurements into the raw buffers
 * \param measureData Used for flags to see when sys and dias are complete
 * \param buf raw curcular buffer to store the collected measurements
 * \return True when completed
 */
bool processPressureMeasure(MeasureData *measureData, RawBuffers *buf) {
  static const uint RESET_VAL = 80;             ///< reset point of sensor

  // determine the old and new array indices in the buffer
  uint oldSysIdx = buf->pressureIndex;
  uint newSysIdx = (oldSysIdx + 1) % BUF_SIZE;
  uint oldDiaIdx = oldSysIdx + BUF_SIZE;
  uint newDiaIdx = newSysIdx + BUF_SIZE;

  // update the last written circ buffer index
  buf->pressureIndex = (uchar) newSysIdx;

  // Check the case when last measurement marked completion of both types of pr
  if (*measureData->completedSystolic && *measureData->completedDiastolic) {
    // starting new cycle of measure, so both completion flags are unmarked
    *measureData->completedSystolic = false;
    *measureData->completedDiastolic = false;
    // finish the method by setting the new sys and dias buffers to reset values
    buf->pressures[newSysIdx] = RESET_VAL;
    buf->pressures[newDiaIdx] = RESET_VAL;
  } else {
    // generate the next pressure value from old pressure value
    if (!*measureData->completedSystolic) {
      buf->pressures[newSysIdx] = genSystP(buf->pressures[oldSysIdx]);
      buf->pressures[newDiaIdx] = buf->pressures[oldDiaIdx];

      // When the pressure sensor > 100 units, mark completed systolic flag
      if (buf->pressures[newSysIdx] > 100) {
        *measureData->completedSystolic = true;
      }
    } else {
      buf->pressures[newSysIdx] = buf->pressures[oldSysIdx];
      buf->pressures[newDiaIdx] = genDiastP(buf->pressures[oldDiaIdx]);

      // When the pressure sensor falls below 40 units, set the completed flag
      if (buf->pressures[newDiaIdx] < 40) {
        *measureData->completedDiastolic = true;
      }
    }
  }

  return *measureData->completedSystolic && *measureData->completedDiastolic;
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
  for(;;) {
    // Get the flags and raw buffer data
    MeasureData *data = (MeasureData *) rawData;
    RawBuffers *buf = data->rawBuffers;

    bool measurementComplete = false;  ///< Flag to determine when completed

    // Process the measurement based on the user's selection
    switch (*data->measurementSelection) {
      case MEASURE_PRESSURE:
        // Process raw pressure data and store in the next circ buf location
        measurementComplete = processPressureMeasure(data, buf);
        break;
      case MEASURE_TEMPERATURE:
        // Process raw temperature data and store in the next circ buf location
        measurementComplete = processTemp(buf);
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
      taskScheduleForExec(TCB_COMPUTE);
      taskSuspend(TCB_MEASURE);
    }

//    vTaskDelay(pdMS_TO_TICKS(1));
  }
}
#pragma clang diagnostic pop