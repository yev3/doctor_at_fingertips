////////////////////////////////////////////////////////////////////////////////
// Project 3, CPSC 5530 Embedded Systems, Seattle University
// Team "ARM Brewery"
// Edward Guevara, David Pierce, and Yevgeni Kamenski
//
// warning_alarm.c - 10/21/2017 4:00 PM
// Responsible for managing the alarm and warning subsystem
// and flashing the indicator LED.
//
// This is free and unencumbered software released into the public domain.
////////////////////////////////////////////////////////////////////////////////

#include <assert.h>
#include <inc/hw_ints.h>
#include <inc/hw_memmap.h>
#include <inc/hw_types.h>
#include <driverlib/interrupt.h>
#include <driverlib/sysctl.h>
#include <driverlib/timer.h>
#include "tasks/system.h"
#include "ranges.h"
#include "inc/lm3s8962.h"
#include "types.h"
#include "utils/led_control.h"
#include "utils/speaker_control.h"
#include "utils/hardware_timer.h"
#include "drivers/pressure_cuff.h"

/******************************************************************************
 *
 * FUNCTIONS DEFINING STATES OF THE ALARM FLAGS
 *
 *****************************************************************************/

/**
 * Determines if the systolic pressure is within an acceptable range
 * @param pSys corrected pressure in mmHg
 * @return true if the pressure is within the normal range
 */
bool pSysNormal(const float *pSys) {
  return (SYS_PRESS_NORMAL_MIN <= *pSys) && (*pSys <= SYS_PRESS_NORMAL_MAX);
}

/**
 * Determines if the systolic pressure is >20% above accepted limit
 * @param pSys corrected pressure in mmHg
 * @return true if the pressure is >20% above accepted limit
 */
bool pSysHighAlarm(const float *pSys) {
  float pressure = 9 + 2.0f * (*pSys);
  return ( pressure >= SYS_PRESS_NORMAL_MAX * 1.2f);
}

/**
 * Determines if the diastolic pressure is within an acceptable range
 * @param pDia corrected pressure in mmHg
 * @return true if the pressure is within the normal range
 */
bool pDiaNormal(const float *pDia) {
  return (DIA_PRESS_NORMAL_MIN <= *pDia) && (*pDia <= DIA_PRESS_NORMAL_MAX);
}

/**
 * Determines if the temperature is within an acceptable range
 * @param temp corrected temperature in degrees celsius
 * @return true if the temperature is within the normal range
 */
bool tempNormal(const float *temp) {
  return (TEMP_NORM_MIN <= *temp) && (*temp <= TEMP_NORM_MAX);
}

/**
 * Determines if the pulse rate is within an acceptable range
 * @param pulse corrected pulse rate in beats per minute
 * @return true if the pulse is within the normal range
 */
bool pulseNormal(const float *pulse) {
  return (PULSE_NORM_MIN <= *pulse) && (*pulse <= PULSE_NORM_MAX);
}

/**
 * Determines if the battery holds enough charge
 * @param battPct battery percentage
 * @return true if the battery percentage is acceptable
 */
bool batteryIsNormal(const float *battPct) {
  return *battPct >= BATT_WARN_LOW_PCT;
}

/******************************************************************************
 *
 * FUNCTIONS DEFINING STATES OF THE ALARM FLAGS
 *
 *****************************************************************************/

/**
 * State of the LED0
 */
typedef enum Led0State {
  LED0_OFF,       ///< Turned off
  LED0_SOLID,     ///< Solid green
  LED0_1SEC,      ///< 1 second flash
  LED0_2SEC       ///< 2 second flash
} Led0State;

/**
 * State of the LED1
 */
typedef enum Led1State {
  LED1_OFF,       ///< Turned off
  LED1_HALF_SEC   ///< 1/2 second flash
} Led1State;

/**
 * State of the LED2
 */
typedef enum Led2State {
  LED2_OFF,       ///< Turned off
  LED2_1SEC       ///< 1 second flash
} Led2State;

/**
 * Represents the state of the system speaker
 */
typedef enum SpkrState {
  SPKR_OFF,       ///< Turned off
  SPKR_ON,        ///< Sounds an alarm
  SPKR_SILENCED   ///< Is in the temporarily silenced state
} SpkrState;

/**
 * Groups definitions for system timing duration constants
 */
static const struct {
  const ullong LED_ON;        ///< Led flash duration
  const ullong SPKR_ON;       ///< Speaker active duration
  const ullong SPKR_CHIRP;    ///< Speaker chirp total period
  const ullong SPKR_CHIRP_ON; ///< Speaker chirp active period
  const ullong SEC_HALF;      ///< 1/2 second duration
  const ullong SEC_1;         ///< 1 second duration
  const ullong SEC_2;         ///< 2 second duration
} nTicksFor = {
  MILLISECS_TO_TICKS(50),
  MILLISECS_TO_TICKS(50),
  MILLISECS_TO_TICKS(50),
  MILLISECS_TO_TICKS(40),
  MILLISECS_TO_TICKS(500),
  MILLISECS_TO_TICKS(1000),
  MILLISECS_TO_TICKS(2000),
};

/**
 * Groups and holds the values representing the state of the system alarms
 */
typedef struct AlarmState {
  bool alarmIsActive;       ///< Indicates that one of the alarms is triggered
  Led0State led0;           ///< State of the system's LED 0
  Led1State led1;           ///< State of the system's LED 1
  Led2State led2;           ///< State of the system's LED 2
  SpkrState spkr;           ///< State of the system's speaker
  uint numSilencedBpAlarms; ///< How many of next BP high ararms are silenced
} AlarmState;

/**
 * Defines and initializes the state of the system alarms when turned on
 */
static volatile AlarmState aState = {
  false,        ///< No alarm when device is powered
  LED0_SOLID,   ///< LED 0 is solid green
  LED1_OFF,     ///< LED 1 is turned off
  LED2_OFF,     ///< LED 1 is turned off
  SPKR_OFF,     ///< Speaker is off
  0u            ///< None of the blood pressure alarms are silenced
};

/**
 * \brief This hook runs on every hardware tick and simulates high priority
 */
void UpdateHardwarePortsTask() {
  ///! Number of hardware ticks elapsed
  system_tick_t ticks = GetElapsedTicks();

  // Set system state when alarm is inactive
  // Suspend this task from running one every system tick
  if (!aState.alarmIsActive) {
    led0_on();
    led1_off();
    led2_off();
    sound_off();
    // Do not run next time
    SysTickHookClear();
  }
    // When the alarm is active, update the board ports
  else {
    // Update the state of the LED 0
    switch (aState.led0) {
      case LED0_OFF: led0_off();
        break;
      case LED0_SOLID: led0_on();
        break;
      case LED0_1SEC:
        if (ticks % nTicksFor.SEC_1 < nTicksFor.LED_ON) {
          led0_on();
        } else {
          led0_off();
        }
        break;
      case LED0_2SEC:
        if (ticks % nTicksFor.SEC_2 < nTicksFor.LED_ON) {
          led0_on();
        } else {
          led0_off();
        }
        break;
      default:assert(0);
        break;
    }

    // Update the state of the LED 1
    switch (aState.led1) {
      case LED1_OFF: led1_off();
        break;
      case LED1_HALF_SEC:
        if (ticks % nTicksFor.SEC_HALF < nTicksFor.LED_ON) {
          led1_on();
        } else {
          led1_off();
        }
        break;
      default: assert(0);
        break;
    }

    // Update the state of the LED 2
    switch (aState.led2) {
      case LED2_OFF: led2_off();
        break;
      case LED2_1SEC:
        if (ticks % nTicksFor.SEC_1 < nTicksFor.LED_ON) {
          led2_on();
        } else {
          led2_off();
        }
        break;
      default: assert(0);
        break;
    }

    // Update the state of the board speaker
    switch (aState.spkr) {
      case SPKR_OFF:
      case SPKR_SILENCED:sound_off();
        break;
      case SPKR_ON:
        if (ticks % nTicksFor.SEC_1 < nTicksFor.SPKR_ON) {
          if (ticks % nTicksFor.SPKR_CHIRP < nTicksFor.SPKR_CHIRP_ON) {
            sound_on();
          } else {
            sound_off();
          }
        } else {
          sound_off();
        }
        break;
      default: assert(0);
        break;
    }
  }
}

/**
 * \brief Responsible for processing the triggered alarms and updating the
 * system state.
 * \param alarms pointer to the EnunciateData alarm flags
 */
void updateCurrentAlarmState(EnunciateData *enunciateData) {

  WarningAlarmStates *alarms = enunciateData->warningAlarmStates;

  // When no alarm is triggered, go to ALARM_TRIGGERED state
  // Also schedule the UpdateHardwarePortsTask to be run on every sys tick
  if (!aState.alarmIsActive) {
    aState.alarmIsActive = true;
    aState.led0 = LED0_OFF;
    aState.led1 = LED1_OFF;
    aState.led2 = LED2_OFF;
    aState.spkr = SPKR_OFF;
    // Enable the peripherals to be updated every hardware tick
    SysTickHookSet(UpdateHardwarePortsTask);
  }

  // Determine the substate of the HAS_ALARM state
  if (alarms->tempOutOfRange) {
    aState.led0 = LED0_1SEC;
  } else if (alarms->pulseOutOfRange) {
    aState.led0 = LED0_2SEC;
  } else {
    aState.led0 = LED0_OFF;
  }

  // Determine the state of the LED1 from the blood press out of range alarm
  if (alarms->bpOutOfRange) {
    aState.led1 = LED1_HALF_SEC;
  } else {
    aState.led1 = LED1_OFF;
  }

  // Determine the state of the LED2 from the battery low alarm
  if (alarms->battLowAlarm) {
    aState.led2 = LED2_1SEC;
  } else {
    aState.led2 = LED2_OFF;
  }

  // Determine the state of the system speaker from the last high BP reading
  // Only do that if the last measurement was pressure
  /*
   * TODO
   */
  if (alarms->bpHighAlarm &&
      (*enunciateData->measurementSelection)) {

    *enunciateData->measurementSelection = MEASURE_NONE;

    if (aState.spkr == SPKR_OFF) {
      aState.numSilencedBpAlarms = 0;
      aState.spkr = SPKR_ON;
    } else if (aState.spkr == SPKR_SILENCED) {
      // When the alarm is silenced by user, ignore this high reading
      if (aState.numSilencedBpAlarms > 0) {
        --aState.numSilencedBpAlarms;
      }
        // Otherwise when out of ignores, resume sounding
      else {
        aState.spkr = SPKR_ON;
      }
    }
  } else {
    aState.spkr = SPKR_OFF;
  }

  // When the user silences the alarm when it's already on or silenced
  // places the speaker into silenced state , sets the num of reads to skip
  if (aState.spkr == SPKR_ON || aState.spkr == SPKR_SILENCED) {
    if (*enunciateData->soundAlarmSilenced) {
      *enunciateData->soundAlarmSilenced = false;
      aState.spkr = SPKR_SILENCED;
      aState.numSilencedBpAlarms = 4;
    }
  }
}

static void clearAlarms() {
  // When no alarm is present, unset the active flag and reset state
  aState.alarmIsActive = false;
  aState.led0 = LED0_SOLID;
  aState.led1 = LED1_OFF;
  aState.led2 = LED2_OFF;
  aState.spkr = SPKR_OFF;
}



/**
 * \brief Initialize the system alarms to the default state
 */
void enunciate_init() {
  led0_on();
  led1_off();
  led2_off();
  sound_off();
}

/**
 * \brief Updates the alarm value and returns if it's a new alarm
 * \param curAlarmState Pointer to the current alarm value
 * \param newAlarmState New alarm value
 * \return If the new alarm was triggered
 */
bool updateAlarmState(bool *curAlarmState, const bool newAlarmState) {
  const bool isNew = (!*curAlarmState && newAlarmState);
  *curAlarmState = newAlarmState;
  return isNew;
}

/**
 * \brief Sets the warning and alarm flags based on the corrected sensor
 * values.
 * \param rawData raw data passed by the task scheduler
 */
void alarmWarn(void *rawData) {
  // TCB data and buffer
  EnunciateData *data = (EnunciateData *)rawData;
  WarningAlarmStates *alarms = data->warningAlarmStates;
  CorrectedBuffers *buf = (CorrectedBuffers *)data->correctedBuffers;

  for(;;) {
    bool newAlarmTriggered = false; // When set, a new alarm is triggered
    bool alrm = false;              // Current alarm considered

    // Check if blood pressure is within acceptable range
    alrm = !(pSysNormal(buf->pressures + buf->pressureIndex) &&
             pDiaNormal(buf->pressures + buf->pressureIndex + BUF_SIZE));
    newAlarmTriggered |= updateAlarmState(&alarms->bpOutOfRange, alrm);

    // Check if systolic pressure is >20% above acceptable range
    alrm = pSysHighAlarm(buf->pressures + buf->pressureIndex);
    newAlarmTriggered |= updateAlarmState(&alarms->bpHighAlarm, alrm);

    // Check if temperature is within acceptable range
    alrm = !tempNormal(buf->temperatures + buf->temperatureIndex);
    newAlarmTriggered |= updateAlarmState(&alarms->tempOutOfRange, alrm);

    // Check if pulse rate is within acceptable range
    alrm = !pulseNormal(buf->pulseRates + buf->pulseRateIndex);
    newAlarmTriggered |= updateAlarmState(&alarms->pulseOutOfRange, alrm);

    // Check if battery level is acceptable
    alrm = !batteryIsNormal(data->batteryPercentage);
    newAlarmTriggered |= updateAlarmState(&alarms->battLowAlarm, alrm);

    // Update the hardware state from the triggered alarms

    // Determine if any of the alarms were triggered
    if (newAlarmTriggered) {
      // Update the current system alarm state
      updateCurrentAlarmState(data);
      // Information is output to the serial communications port when 
      // a new alarm becomes acive
      taskScheduleForExec(sysTCB_SERIAL);
    } else {
      static void clearAlarms();
    }

    // Prevent this task from running unless scheduled by other tasks
    vTaskSuspend(NULL);
  }
}