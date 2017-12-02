/*****************************************************************************
 * Project 3, CPSC 5530 Embedded Systems, Seattle University, 2017           *
 * Team "ARM Brewery": Edward Guevara, David Pierce, and Yevgeni Kamenski    *
 *                                                                           *
 * test_hardware_announce.c - 9/30/2017                                              *
 * Verify that all
 * Verify that the LEDs are all working properly                             *
 *                                                                           *
 * "THE BEER-WARE LICENSE" (Revision 42):                                    *
 * As long as you retain this notice, you can do whatever you want with this *
 * software. If we meet some day, and you think this software is worth it,   *
 * you can buy us a beer in return. - Team "ARM Brewery"                     *
 *****************************************************************************/

#include <types.h>
#include <drivers/pulse_transducer.h>
#include "utils/led_control.h"
#include <utils/speaker_control.h>
#include <tasks/tasks.h>
#include <utils/lcd_print.h>
#include <utils/hardware_timer.h>
#include <assert.h>

/* For convenience: number of elems in statically allocated array */
#define GET_ARR_SIZE(A) ((sizeof((A))) / (sizeof((A)[0])))

/**
 * \brief Menu options for the test
 */
static const char *menu[] = {
  "  Pulse           ",
  "  Temperature     ",
  "  Pressure OOR    ",
  "  Pressure High   ",
  "  Battery Low     ",
  "  Sound Silenced  ",
};

/**
 * User's current selection
 */
static int curSelection = 0;

/**
 * Number of items in the menu
 */
static uint numSelections = GET_ARR_SIZE(menu);

/**
 * \brief Prints the menu to the user
 * \param opts User's currently selected options
 */
void printMenu(bool *opts[]) {
  for (uint i = 0; i < numSelections; ++i) {
    lcd_printf_at(i, 0, menu[i]);
    lcd_printf("%3s", *opts[i] ? "ON" : "OFF");
  }
  lcd_printf_at(curSelection, 0, ">>");
}

/**
 * \brief Scans the keyboard waiting for a key press. Updates the menu scroll.
 * Selects the user's choice. Calls the display function. Returns to caller.
 * \param tcb Key scanner TCB
 * \param opts Array of user options representing each line in the menu
 */
void uiController(TaskControlBlock *const tcb, bool *opts[]) {
  static system_tick_t prevTicks = 0; ///< Previous number of ticks
  static KeyScanData *keys;           ///< Key scanner data
  keys = tcb->pTaskData;

  // Loop until a key press is detected
  while (1) {
    while (GetElapsedTicks() == prevTicks);
    prevTicks = GetElapsedTicks();
    tcb->pTask(keys);
    if (*keys->keyAvailable) break;
  }

  // Update the scroll position
  if (*keys->keyPressedDown)
    curSelection = (curSelection + 1) % numSelections;

  // Update the scroll position
  if (*keys->keyPressedUp)
    curSelection = (curSelection + numSelections - 1) % numSelections;

  // Update the boolean selection flag
  if (*keys->keyPressedSelect)
    *opts[curSelection] = !*opts[curSelection];
}

/**
 * \brief Responsible for processing the triggered alarms and updating the
 * system state.
 * \param alarms pointer to the EnunciateData alarm flags
 */
extern void updateHardwareStateFrom(EnunciateData *alarms);

/**
 * \brief Main entry point of the program
 * \return not utilized
 */
int main(void) {
  // Set the clocking to run directly from the crystal.
  SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
                 SYSCTL_XTAL_8MHZ);

  // Initialize all TCB structures and global variables
  initVarsAndTasks();

  // Get the Key scanner TCB
  TaskControlBlock *const keyScanTCB = systemTasks[TCB_KEYSCAN];

  // Get the structure with alarm flags
  TaskControlBlock *enunciateTCB = systemTasks[TCB_ENUNCIATE];
  EnunciateData *alarms = enunciateTCB->pTaskData;

  // Set the options to point to the alarm flags to be triggered
  bool *opts[] = {
    alarms->pulseOutOfRange,
    alarms->tempOutOfRange,
    alarms->bpOutOfRange,
    alarms->bpHighAlarm,
    alarms->battLowAlarm,
    alarms->soundAlarmSilenced,
  };

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
  while (1) {
    // Print the menu to the user
    printMenu(opts);

    // Wait for a key press and update to the system alarm states
    uiController(keyScanTCB, opts);

    // Run the enunciate task to update the state of the system
    updateHardwareStateFrom(alarms);
  }
#pragma clang diagnostic pop
}
