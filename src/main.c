////////////////////////////////////////////////////////////////////////////////
// Project 5, CPSC 5530 Embedded Systems, Seattle University
// Team "ARM Brewery"
// Edward Guevara, David Pierce, and Yevgeni Kamenski
//
// main.c
// Project main entry point.
//
// This is free and unencumbered software released into the public domain.
////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <time.h>
#include <stdarg.h>
#include <stdbool.h>

#include <inc/hw_types.h>
#include <driverlib/sysctl.h>

#include <FreeRTOS.h>
#include "task.h"
#include "FreeRTOS_IP.h"
#include "FreeRTOS_Sockets.h"

#include "tasks/system.h"
#include "utils/lcd_print.h"

/**
 * \brief Initializes the board hardware peripherals
 */
void initializeHardwarePeriph();

int main(void) {
  // Initialize board hardware
  initializeHardwarePeriph();

  // Initialize system tasks
  initVarsTasksQueues();

  // Initialize network hardware and tasks
  network_init();

  // Start the RTOS scheduler.
  FreeRTOS_debug_printf(("vTaskStartScheduler\n"));
  vTaskStartScheduler();

  for (;;) {
  }
}

/**
 * \brief Initializes the board hardware peripherals
 */
void initializeHardwarePeriph() {
  static bool shouldRun = true; ///< ensures that this method runs only once
  if (shouldRun) {
    shouldRun = false;

    /* Set the clocking to run from the PLL at 50 MHz */
    SysCtlClockSet(
      SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_8MHZ);

    // Initialize the UART to display serial text to the user's PC terminal
    serial_init();

    // Initialize hardware ports needed to flash the LEDs
    led_init();

    // Initialize temperature sensor
    temperature_init();

    // Initialize the system timer
    Timer1InitDefault();

    // Initialize hardware ports needed to read key presses
    keypad_init();

    // Initialize the hardware timer to simulate the pulse transducer driver
    pulse_init();

    // Initialize the onboard OLED display
    lcd_init();

    // Initialize hardware port required for playing sound
    speaker_init();

    // Initialize the hardware required for flashing the LED and speaker
    enunciate_init();
  }
}