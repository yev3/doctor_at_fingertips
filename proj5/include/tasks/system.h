////////////////////////////////////////////////////////////////////////////////
// Project 3, CPSC 5530 Embedded Systems, Seattle University
// Team "ARM Brewery" 
// Edward Guevara, David Pierce, and Yevgeni Kamenski
// 
// tasks.h - 09/25/2017 5:17 PM
// Defines Task Control Data structures used by the task scheduler
// 
// This is free and unencumbered software released into the public domain.
////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "types.h"
#include "FreeRTOS.h"
#include "task.h"
#include "utils/lcd_print.h"

// @formatter:off
/******************************************************************************
 * Declarations of TCB structures, allocations, and initialization routines
 ******************************************************************************/

/**
 * \brief Task names that are statically allocated in the system.
 */
typedef enum {
  sysTCB_MEASURE     = 0,   ///< MEASURE 
  sysTCB_COMPUTE     = 1,   ///< COMPUTE 
  sysTCB_DISPLAY     = 2,   ///< DISPLAY 
  sysTCB_ENUNCIATE   = 3,   ///< ENUNCIATE 
  sysTCB_STATUS      = 4,   ///< STATUS 
  sysTCB_KEYSCAN     = 5,   ///< KEY SCAN 
  sysTCB_CONTROLLER  = 6,   ///< CONTROLLER 
  sysTCB_SERIAL      = 7,   ///< SERIAL COMMS 
  sysTCB_MEAS_EKG    = 8,  ///< EKG MEASURE 
  sysTCB_COMP_EKG    = 9,  ///< EKG COMPUTE 
} TaskNameEnum_t;

#define sysTASK_COUNT (sysTCB_COMP_EKG + 1)

/*
 * Priorities above idle for tasks
 */
#define sysPRI_MEAS  3
#define sysPRI_COMP  3
#define sysPRI_DISP  3
#define sysPRI_ENUN  3
#define sysPRI_STAT  3
#define sysPRI_KEYS  3
#define sysPRI_CONT  3
#define sysPRI_SERL  3
#define sysPRI_MEKG  3
#define sysPRI_CEKG  3

/*
 * Stack sizes
 */
#define sysSTK_MEAS configMINIMAL_STACK_SIZE
#define sysSTK_COMP configMINIMAL_STACK_SIZE
#define sysSTK_DISP configMINIMAL_STACK_SIZE
#define sysSTK_ENUN configMINIMAL_STACK_SIZE
#define sysSTK_STAT configMINIMAL_STACK_SIZE
#define sysSTK_KEYS configMINIMAL_STACK_SIZE
#define sysSTK_CONT configMINIMAL_STACK_SIZE
#define sysSTK_SERL configMINIMAL_STACK_SIZE 
#define sysSTK_MEKG configMINIMAL_STACK_SIZE
#define sysSTK_CEKG configMINIMAL_STACK_SIZE + 500

#define sysSTK_TOTAL (sysSTK_MEAS + sysSTK_COMP + sysSTK_DISP + \
                      sysSTK_ENUN + sysSTK_STAT + sysSTK_KEYS + \
                      sysSTK_CONT + sysSTK_SERL + sysSTK_MEKG + \
                      sysSTK_CEKG)


/**
 * \brief User's measurement selection values
 */
typedef enum MeasureSelection_t {
  MEASURE_NONE        = 0,
  MEASURE_BEGIN       = (1<<0),
  MEASURE_PRESSURE    = (1<<0),  ///< Blood pressure measurement selection
  MEASURE_TEMPERATURE = (1<<1),  ///< Temperature measurement selection
  MEASURE_PULSE       = (1<<2),  ///< Pulse rate measurement selection
  MEASURE_EKG         = (1<<3),  ///< EKG measurement selection
  MEASURE_END         = (1<<4),
} MeasureSelection;

// @formatter:on

/**
 * \brief Iliffe vector of system TCBs, fully initialized by initTCB() and
 * accessed by the indices defined by TaskName
 */
extern TaskHandle_t taskHandles[];

/**
 * \brief Initialize hardware required by the keypad
 */
extern void keypad_init();

/**
 * \brief Initializes the board UART hardware
 */
extern void serial_init();

/**
 * \brief Initializes the alarm module
 */
extern void enunciate_init();

/**
 * \brief Initializes the temperature sensor
 */
extern void temperature_init();

/**
 * \brief Initializes the EKG measurement simulation
 */
extern void measure_ekg_init();

/**
 * \brief Initializes the pulse measurement driver
 */
extern void pulse_init();

/**
 * Initializes the PWM port (G)
 */
extern void speaker_init();

/**
 * \brief Initializes the hardware tick timer using default tick duration
 */
extern void Timer1InitDefault();

/**
 * Initializes the LED ports
 */
extern void led_init();

// Initializes the network stack
extern void network_init();

/**
 * \brief Init all global variables, TCB structures & underlying data pointers.
 */
void initVarsAndTasks();

/******************************************************************************
 * Declarations of the scheduling routines
 ******************************************************************************/

/**
 * \brief Suspends the tasks by removing it from the execution queue. Takes no
 * action if the task is already in the suspended state
 * \param id Task to suspend
 */
void taskSuspend(TaskNameEnum_t id);

/**
 * \brief Unsuspends the tasks by adding it to the execution queue. Takes no
 * action if the task is already in the in the execution queue.
 * \param id Task to unsuspend
 */
void taskScheduleForExec(TaskNameEnum_t id);

/******************************************************************************
 * Declarations of the data structures used in the tasks
 ******************************************************************************/

#define BUF_SIZE            ((uchar)8)  /* buffer size in number of readings */
#define BUF_SIZE_EKG        ((uint)256) /* buffer size of EKG raw readings */
#define BUF_INIT_TEMP       ((uint)75)  /* init raw temperature buffer value */
#define BUF_INIT_PRESS_SYST ((uint)80)  /* init raw systolic pressure buffer */
#define BUF_INIT_PRESS_DIAS ((uint)80)  /* init raw diastolic pressure buffer */
#define BUF_INIT_PULSE      ((uint)0)   /* init raw pulse rate buffer value */

/*
 * Circular buffers and their current indices storing raw readings from the
 * sensors, defined in the global var sect and initialized in the startup task.
 */
typedef struct RawBuffers {
  uint temperatures[BUF_SIZE];        ///< Raw temperature circular buffer
  uint pressures[2 * BUF_SIZE];       ///< Raw pressures circular buffer
  uint pulseRates[BUF_SIZE];          ///< Raw pulse rate circular buffer
  uchar temperatureIndex;             ///< Current raw temperature buffer index
  uchar pressureIndex;                ///< Current raw pressure buffer index
  uchar pulseRateIndex;               ///< Current raw pulse rate buffer index
} RawBuffers;

/*
 * Circular buffers and their current indices storing corrected readings from
 * the sensors, defined in the global var sect and init in the startup task.
 */
typedef struct CorrectedBuffers {
  float temperatures[BUF_SIZE];       ///< Corrected Temperature in deg C
  float pressures[2 * BUF_SIZE];      ///< Corrected pressures in mmHg
  float pulseRates[BUF_SIZE];         ///< Corrected Pulse Rate in BPM
  float ekgFrequency[2 * BUF_SIZE];   ///< Corrected EKG measurements
  uchar temperatureIndex;             ///< Current corrected temp buffer index
  uchar pressureIndex;                ///< Current corrected pres buffer index
  uchar pulseRateIndex;               ///< Current corrected PR buffer index
  uchar ekgIndex;                     ///< Current corrected EKG buffer index
} CorrectedBuffers;

typedef struct EKGBuffer {
    int ekgMeasures[BUF_SIZE_EKG];      ///< Raw EKG reading buffer
    uchar ekgIndex;                     ///< Current raw EKG buffer index
} EKGBuffer;

/**
 * \brief MEASUREMENTS data references
 */
typedef struct MeasureData {
  RawBuffers *rawBuffers;                 ///< Raw circular buffers and indices
  bool *completedSystolic;                ///< T when systolic read done
  bool *completedDiastolic;               ///< T when diastolic read done
  MeasureSelection *measurementSelection; ///< User's selected measurement
  uint *currentPressure;
} MeasureData;

/**
 * \brief COMPUTE data references
 */
typedef struct ComputeData {
  RawBuffers *rawBuffers;                 ///< Raw circular buffers and indices
  CorrectedBuffers *correctedBuffers;     ///< Corrected buffers and indices
  ushort *batteryState;                   ///< Raw battery state reading
  float *batteryPercentage;               ///< Current battery percentage
  MeasureSelection *measurementSelection; ///< User's selected measurement
} ComputeData;


/**
 * Warning and alarm states of the system.
 */
typedef struct {
  /* ALARMS */
  bool bpOutOfRange;                ///< Blood pressure out of range warn
  bool tempOutOfRange;              ///< Temperature out of range warning
  bool pulseOutOfRange;             ///< Pulse rate out of range warning

  /* Warnings */
  bool bpHighAlarm;                 ///< Blood pressure high alarm
  bool tempHighAlarm;               ///< Temperature high alarm
  bool pulseLowAlarm;               ///< Pulse rate low alarm
  bool battLowAlarm;                ///< Battery low alarm

} WarningAlarmStates;

/**
 * \brief ALARMS data references
 */
typedef struct EnunciateData {
  CorrectedBuffers *correctedBuffers;     ///< Corrected buffers and indices
  MeasureSelection *measurementSelection; ///< User's selected measurement
  float *batteryPercentage;               ///< Current battery percentage
  
  WarningAlarmStates *warningAlarmStates; ///< Ptr to warnings and alarms
  
  bool *soundAlarmSilenced;       ///< True when user acknowledges the alarm
} EnunciateData;


/**
 * \brief Used by the serial communications module
 */
typedef struct SerialCommData {
  CorrectedBuffers *correctedBuffers;     ///< Corrected buffers and indices
  float *batteryPercentage;               ///< Current battery percentage
  WarningAlarmStates *warningAlarmStates; ///< Ptr to warnings and alarms
} SerialCommData;

/**
 * \brief Key scanner data references
 */
typedef struct KeyScanData {
  bool keyAvailable;              ///< Indicates new key press occurred
  bool keyPressedUp;              ///< Indicates a new UP press
  bool keyPressedDown;            ///< Indicates a new DOWN press
  bool keyPressedLeft;            ///< Indicates a new LEFT press
  bool keyPressedRight;           ///< Indicates a new RIGHT press
  bool keyPressedSelect;          ///< Indicates a new SELECT press
} KeyScanData;

typedef enum DisplayMode {
  MENU_DISP_MODE = 0,
  ENUNCIATE_DISP_MODE = 1
} DisplayMode;

/**
 * \brief DISPLAY data references
 */
typedef struct DisplayViewModel {
  CorrectedBuffers *correctedBuffers; ///< Corrected buffers and indices
  float *batteryPercentage;           ///< Current battery percentage
  DisplayMode *mode;                  ///< Current display mode
  int *scrollPosn;                    ///< User's current scroll posn
  bool *cuffControl;                  ///< toggle pressure control of cuff
  uint *currentPressure;              ///< percent applied pressure
} DisplayViewModel;

/**
 * \brief Holds references to data needed for the controller to operate
 */
typedef struct ControllerData {
  KeyScanData *keyScanData;       ///< Key scanner data references
  DisplayMode *mode;              ///< Current display mode
  int *scrollPosn;                ///< Current scroll position
  MeasureSelection *measurementSelection; ///< User's selected measurement
  bool *auralAlarmSilenced;       ///< True when user acknowledges the alarm
  bool *cuffControl;                  ///< toggle pressure control of cuff
  uint *currentPressure;              ///< percent applied pressure
} ControllerData;

/**
 * \brief STATUS data references
 */
typedef struct StatusData {
  ushort *batteryState;           ///< Raw battery state reading
} StatusData;

/**
 * \brief EKGMEASURE data references
 */
typedef struct MeasureEKGData {
    EKGBuffer *ekgBuffer;                 ///< Raw circular buffers and indices
    bool *completedEKGMeasure;            ///< T when measurement is done
} MeasureEKGData;

/**
 * \brief EKGCOMPUTE data references
 */
typedef struct ComputeEKGData {
    EKGBuffer *ekgBuffer;                 ///< Raw circular buffers and indices
    CorrectedBuffers *correctedBuffers;   ///< Raw circular buffers and indices
    bool *completedEKGMeasure;            ///< F when computation is done
} ComputeEKGData;