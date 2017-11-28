/*****************************************************************************
 * Project 2, CPSC 5530 Embedded Systems, Seattle University, 2017           *
 * Team "ARM Brewery": Edward Guevara, David Pierce, and Yevgeni Kamenski    *
 *                                                                           *
 * test_task_timings.c - 9/30/2017                                           *
 * Timing of the measure, compute, enunciate, and status TCBs                *
 *                                                                           *
 * "THE BEER-WARE LICENSE" (Revision 42):                                    *
 * As long as you retain this notice, you can do whatever you want with this *
 * software. If we meet some day, and you think this software is worth it,   *
 * you can buy us a beer in return. - Team "ARM Brewery"                     *
 *                                                                           *
 *****************************************************************************/

#include <inc/hw_nvic.h>
#include <utils/uartstdio.h>
#include <utils/led_control.h>
#include <driverlib/systick.h>
#include <driverlib/interrupt.h>
#include <utils/hardware_timer.h>
#include <drivers/rit128x96x4.h>
#include "utils/lcd_print.h"
#include "tasks/tasks.h"
#include "utils/float_print.h"

// 24 bit according to the datasheet on page 98: 3.1.1 System Timer
#define SYS_TICK_MAX ((1<<24) - 1)

/**
 * Resets the timer.
 */
void timerReset() {
  SysTickPeriodSet(SYS_TICK_MAX);
  HWREG(NVIC_ST_CURRENT) = 0;
}

/**
 * \brief Dispatches a given TCB 10 times, returns the elapsed time
 * \param tcb TCB to dispatch
 * \return elapsed time in system ticks
 */
ulong timedDispatch10(TaskName id) {
  TaskControlBlock *const tcb = systemTasks[id];
  timerReset();
  for (int i = 0; i < 10; ++i) {
    tcb->pTask(tcb->pTaskData);
  }
  return SYS_TICK_MAX - SysTickValueGet();
}

const char *taskName(TaskName name) {
  switch (name) {
    case TCB_MEASURE: return "Measure";
    case TCB_COMPUTE: return "Compute";
    case TCB_DISPLAY: return "Display";
    case TCB_ENUNCIATE: return "Enunciate";
    case TCB_STATUS: return "Battery Status";
    case TCB_KEYSCAN: return "Key Scanner";
    case TCB_CONTROLLER: return "UI Controller";
    case TCB_SERIAL: return "Serial Comms";
    default: return "ERROR";
  }
}

/**
 * \brief Main entry point of the program
 * \return not utilized
 */
int main(void) {
  // Set the clocking to run directly from the crystal.
  SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
                 SYSCTL_XTAL_8MHZ);

  // Initialize all project hardware peripherals, globals, and TCBs
  initVarsAndTasks();

  // Disable all peripheral interrupts for timing
  IntMasterDisable();

  // Initialize the system hardware tick timer
  SysTickEnable();

  UARTprintf("Calling each of the following functions 10 times \n");

  for (TaskName name = TCB_NAME_BEGIN; name < TCB_NAME_END; ++name) {
    if (TCB_SERIAL == name) continue;
    UARTprintf("%15s : ", taskName(name));
    ulong elapsedTicks = timedDispatch10(name);
    ullong elapsedMicro = (ullong)elapsedTicks * 1000000 / SysCtlClockGet();
    UARTprintf("%8d",(int)elapsedMicro);
    UARTprintf(" microsecs\n");
  }

  UARTprintf("# ticks in 1 second: %d\n", SysCtlClockGet());
  UARTprintf("Task timing program is finished\n");
}

void TimerInitDefault() {}
system_tick_t GetElapsedTicks() { return 0; }
void SysTickHookSet(system_tick_hook_t hook) {}
void SysTickHookClear() {}
void pulse_init() {}
void pulseHookSet() {}
void pulseHookClear() {}