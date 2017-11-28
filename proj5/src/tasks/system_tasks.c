////////////////////////////////////////////////////////////////////////////////
// Project 3, CPSC 5530 Embedded Systems, Seattle University
// Team "ARM Brewery" 
// Edward Guevara, David Pierce, and Yevgeni Kamenski
// 
// tasks.c - 10/21/2017 4:06 PM
// Responsible for setting up the global variable sharing between tasks.
// Declares initialization functions for Task Control Blocks used by the scheduler.
// 
// This is free and unencumbered software released into the public domain.
////////////////////////////////////////////////////////////////////////////////
// @formatter:off

#include <utils/hardware_timer.h>
#include "system_tasks.h"
#include "utils/led_control.h"
#include "utils/speaker_control.h"
#include "drivers/pulse_transducer.h"

/*****************************************************************************
 * Global variable declaration section. Declared static on purpose to allow
 * variable access only through getters.
 *****************************************************************************/

/*
 * Raw temperature readings from the sensors, initialized in the startup task
 */
static RawBuffers rawBuffers;           ///< Raw sensor buffers and indices
static ushort rawBatteryState = 200;    ///< Raw battery state reading

/*
 * Flags indicating the state of the blood pressure reading
 */
static bool completedSystolicMeasure  = false;  ///< T when systolic read done
static bool completedDiastolicMeasure = false;  ///< T when diastolic read done

/*
 * Corrected sensor readings
 */
static CorrectedBuffers correctedBuffers;  ///< Corrected sensor buffers
static float batteryPercentage       = 0;  ///< Current battery in %

/**
 * Warning and alarm states of the system.
 */
static WarningAlarmStates warningAlarmStates = { 0 };

/*
 * User interface controller state
 */
static int scrollPosn = 0;                    ///< current scroll position
static MeasureSelection measureSelection = MEASURE_PRESSURE; ///< cur measure
static bool auralAlarmSilenced = false;       ///< when true, alarm silenced
static DisplayMode displayMode = MENU_DISP_MODE;     ///< Current display mode

/*
 * EKG measurements and flag when measurement completes
 */
static EKGBuffer ekgBuffer;               ///< buffer holding EKG measurements
static bool completedEKGMeasure = false;  ///< flag when measurement is done

/*****************************************************************************
 * Task shared data declarations section.
 *****************************************************************************/

static MeasureData measureData        = {0};  ///< measure task data
static ComputeData computeData        = {0};  ///< compute task data
static DisplayViewModel displayData   = {0};  ///< display task data
static EnunciateData enunciateData    = {0};  ///< compute task data
static StatusData statusData          = {0};  ///< status task data
static KeyScanData keyScanData        = {0};  ///< key scan task data
static ControllerData controllerData  = {0};  ///< ui controller task data
static SerialCommData serialData      = {0};  ///< ui controller task data
static MeasureEKGData measureEKGData  = {0};  ///< EKG measure task data
static ComputeEKGData computeEKGData  = {0};  ///< EKG compute task data

#define STATIC_TCB_COUNT (TCB_NAME_END - TCB_NAME_BEGIN)
#define STATIC_TCB_STACK_SIZE 200
#define STATIC_TCB_PRIORITY (tskIDLE_PRIORITY + 3)

TaskHandle_t taskHandles[STATIC_TCB_COUNT];
static StaticTask_t taskControlBuffers[STATIC_TCB_COUNT];
static StackType_t taskControlStacks[STATIC_TCB_COUNT][STATIC_TCB_STACK_SIZE];

extern void measure(void *rawData);
extern void compute(void *rawData);
extern void display(void *rawData);
extern void enunciate(void *rawData);
extern void key_scan(void *rawData);
extern void ui_controller(void *rawData);
extern void status(void *rawData);
extern void serial_comms(void *rawData);
extern void measureEKG(void *rawData);
extern void computeEKG(void *rawData);

typedef struct {
  TaskFunction_t pxTaskCode;
  const char *const pcName;
  void * const pvParameters;
} TCBStaticEntry_t;

#define TCB_ENTRY(FN,DATA) {(FN), (#FN), &(DATA)}

void dummyTask(void *data) {
  for(;;) {
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

static const TCBStaticEntry_t taskEntries[] = {
  TCB_ENTRY(measure,       measureData),    /* TCB_MEASURE     */
  TCB_ENTRY(compute,       computeData),    /* TCB_COMPUTE     */
  TCB_ENTRY(display,       displayData),    /* TCB_DISPLAY     */
  TCB_ENTRY(dummyTask,     enunciateData),  /* TCB_ENUNCIATE   */
//  TCB_ENTRY(enunciate,     enunciateData),  /* TCB_ENUNCIATE   */
  TCB_ENTRY(status,        statusData),     /* TCB_STATUS      */
  TCB_ENTRY(key_scan,      keyScanData),    /* TCB_KEYSCAN     */
  TCB_ENTRY(ui_controller, controllerData), /* TCB_CONTROLLER  */
  TCB_ENTRY(serial_comms,  serialData),     /* TCB_SERIAL      */
  TCB_ENTRY(dummyTask/*measureEKG*/,    measureData),    /* TCB_MEASURE_EKG */
//  TCB_ENTRY(measureEKG,    measureData),    /* TCB_MEASURE_EKG */
  TCB_ENTRY(dummyTask/*computeEKG*/,    measureData),    /* TCB_COMPUTE_EKG */
//  TCB_ENTRY(computeEKG,    measureData),    /* TCB_COMPUTE_EKG */
};

void initStaticTCBs() {
  for (int i = TCB_NAME_BEGIN; i < TCB_NAME_END; ++i) {
    /* Create the task without using any dynamic memory allocation. */
    taskHandles[i] = xTaskCreateStatic(
      taskEntries[i].pxTaskCode,   // Function that implements the task.
      taskEntries[i].pcName,       // Text name for the task.
      STATIC_TCB_STACK_SIZE,       // Number of indexes in the xStack array.
      taskEntries[i].pvParameters, // Parameter passed into the task.
      STATIC_TCB_PRIORITY,         // Priority at which the task is created.
      taskControlStacks[i],        // Array to use as the task's stack.
      &taskControlBuffers[i] );    // Var to hold the task's data structure.
  }
}

#undef TCB_ENTRY

// @formatter:on

/*****************************************************************************
 * Forward declarations of the external routines and initialization functions
 * referenced in the initializations. Note that these are no longer included
 * in the tasks.h to encapsulate the visibility only to this module.
 *****************************************************************************/




/*****************************************************************************
 * Global variable initialization
 *****************************************************************************/

/**
 * \brief Initializes buffers
 */
void buffersInit() {
  // Set the raw buffer indices to first element
  rawBuffers = (RawBuffers) {
    .pressureIndex    = 0,
    .pulseRateIndex   = 0,
    .temperatureIndex = 0,
  };

  // raw buffers are initialized to the predetermined values
  for (int i = 0; i < BUF_SIZE; ++i) {
    rawBuffers.temperatures[i] = BUF_INIT_TEMP;
    rawBuffers.pressures[i] = BUF_INIT_PRESS_SYST;
    rawBuffers.pressures[i + BUF_SIZE] = BUF_INIT_PRESS_DIAS;
    rawBuffers.pulseRates[i] = BUF_INIT_PULSE;
  }

  // Set the corrected buffer indices to first element
  correctedBuffers = (CorrectedBuffers) {
    .pressureIndex    = 0,
    .pulseRateIndex   = 0,
    .temperatureIndex = 0,
    .ekgIndex = 0
  };

  // Note: corrected buffers are uninitialized
}

/*****************************************************************************
 * Initialization functions to ensure all of the data structures above are
 * in the proper state upon system start-up
 *****************************************************************************/

/**
 * \brief Initializes and returns a pointer to an initialized MEASURE TCB
 * \return pointer to an initialized TCB
 */
void globalVariablesInit() {
  // Initialize the Measure data pointers
  measureData = (MeasureData) {
    .rawBuffers = &rawBuffers,
    .completedSystolic = &completedSystolicMeasure,
    .completedDiastolic = &completedDiastolicMeasure,
    .measurementSelection = &measureSelection,
  };

  // Initialize the Compute data pointers
  computeData = (ComputeData) {
    .rawBuffers = &rawBuffers,
    .batteryState = &rawBatteryState,
    .correctedBuffers = &correctedBuffers,
    .batteryPercentage = &batteryPercentage,
    .measurementSelection = &measureSelection,
  };

  // Initialize the display data pointers
  displayData = (DisplayViewModel) {
    .correctedBuffers = &correctedBuffers,
    .batteryPercentage =&batteryPercentage,
    .mode = &displayMode,
    .scrollPosn = &scrollPosn,
  };

  // Initialize the warning data pointers
  enunciateData = (EnunciateData) {
    .correctedBuffers = &correctedBuffers,
    .measurementSelection = &measureSelection,
    .batteryPercentage = &batteryPercentage,
    .warningAlarmStates = &warningAlarmStates,
    .soundAlarmSilenced = &auralAlarmSilenced,
  };

  // Initialize the status data pointers
  statusData.batteryState = &rawBatteryState;

  // Initialize the ui controller data pointers
  controllerData = (ControllerData) {
    .keyScanData = &keyScanData,
    .mode = &displayMode,
    .scrollPosn = &scrollPosn,
    .auralAlarmSilenced = &auralAlarmSilenced,
    .measurementSelection = &measureSelection,
  };

  // Initialize the warning data pointers
  serialData = (SerialCommData) {
    .correctedBuffers = &correctedBuffers,
    .batteryPercentage = &batteryPercentage,
    .warningAlarmStates = &warningAlarmStates
  };

  // Initialize the EKG measurements pointers
  measureEKGData = (MeasureEKGData) {
    .ekgBuffer = &ekgBuffer,
    .completedEKGMeasure = &completedEKGMeasure
  };

  // Initialize the EKG measurements pointers
  computeEKGData = (ComputeEKGData) {
    .ekgBuffer = &ekgBuffer,
    .correctedBuffers = &correctedBuffers,
    .completedEKGMeasure = &completedEKGMeasure
  };

}

/**
 * \brief Suspends the tasks by removing it from the execution queue. Takes no
 * action if the task is already in the suspended state
 * \param id Task to suspend
 */
void taskSuspend(TaskNameEnum_t id) {
  vTaskSuspend(taskHandles[id]);
}

/**
 * \brief Unsuspends the tasks by adding it to the execution queue. Takes no
 * action if the task is already in the in the execution queue.
 * \param id Task to unsuspend
 */
void taskScheduleForExec(TaskNameEnum_t id){
  vTaskResume(taskHandles[id]);
}

/**
 * \brief Initializes the task control blocks and global variables
 */
void initVarsAndTasks() {
  static bool runOnce = true; ///< ensures that this method runs only once
  if (runOnce) {
    runOnce = false;

    // Initialize the shared global vars to default values
    buffersInit();

    // Initialize the global variable links among tasks
    globalVariablesInit();

    // Initialize statically-allocated TCBs
    initStaticTCBs();
  }
}

/*
 * Static memory for the system tasks
 */
StaticTask_t sysTaskBufs[2];
StackType_t sysStackBufs[2][configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
  StackType_t **ppxIdleTaskStackBuffer,
  uint32_t *pulIdleTaskStackSize) {
  *ppxIdleTaskTCBBuffer = sysTaskBufs;
  *ppxIdleTaskStackBuffer = sysStackBufs[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}

void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
  StackType_t **ppxTimerTaskStackBuffer,
  uint32_t *pulTimerTaskStackSize) {
  *ppxTimerTaskTCBBuffer = sysTaskBufs + 1;
  *ppxTimerTaskStackBuffer = sysStackBufs[1];
  *pulTimerTaskStackSize = configMINIMAL_STACK_SIZE;
}
