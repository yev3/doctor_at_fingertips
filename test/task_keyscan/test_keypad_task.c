////////////////////////////////////////////////////////////////////////////////
// Project 3, CPSC 5530 Embedded Systems, Seattle University
// Team "ARM Brewery"
// Edward Guevara, David Pierce, and Yevgeni Kamenski
//
// test_keypad_task.c - 10/12/2017
// Establish software delay to flash exactly one second. The LED flash rate
// is then used to verify the accuracy of the software delay.
//
// This is free and unencumbered software released into the public domain.
////////////////////////////////////////////////////////////////////////////////

#include <driverlib/sysctl.h>
#include <utils/hardware_timer.h>
#include "../../utils/lcd_print.h"
#include "../../tasks/tasks.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

/**
 * \brief Dispatches a single TCB
 * \param tcb TCB to dispatch
 */
void dispatch(TaskName id) {
  TaskControlBlock *const tcb = systemTasks[id];
  tcb->pTask(tcb->pTaskData);
}

/**
 * \brief Does not run if at least one hardware tick elapsed since last call.
 * Spins the cpu otherwise, waiting for a tick to occur.
 */
void waitForNextTick() {
  static system_tick_t prevTicksElapsed = 0;
  while (GetElapsedTicks() == prevTicksElapsed);
  prevTicksElapsed = GetElapsedTicks();
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

  // just once so that corrected vals are populated
  dispatch(TCB_COMPUTE);

  // Display a timer to the user forever
  while (1) {
    // Do not run the loop faster than a single hardware tick
    waitForNextTick();

    // first scan the keypad, then update the state, then display the state
    dispatch(TCB_KEYSCAN);
    dispatch(TCB_CONTROLLER);
    dispatch(TCB_DISPLAY);
  }
}


#pragma clang diagnostic pop