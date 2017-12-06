//
// Main entry Point
//

/* Standard includes. */
#include <stdio.h>
#include <time.h>
#include <stdarg.h>
#include <stdbool.h>

#include <inc/hw_types.h>
#include <driverlib/sysctl.h>

/* FreeRTOS includes. */
#include <FreeRTOS.h>
#include "task.h"

/* Demo application includes. */
#include "FreeRTOS_IP.h"
#include "FreeRTOS_Sockets.h"

#include "tasks/commands.h"
#include "tasks/system.h"
#include "server/FreeRTOS_TCP_server.h"

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

  //// TODO: remove when changes merged
  taskSuspend(sysTCB_MEAS_EKG);
  taskSuspend(sysTCB_COMP_EKG);
  
  // Start the RTOS scheduler.
  FreeRTOS_debug_printf(("vTaskStartScheduler\n"));
  vTaskStartScheduler();

  for (;;) {
    // Should never reach
#ifdef WIN32
    Sleep(pdMS_TO_TICKS(1000UL));
#endif
  }
}
/*-----------------------------------------------------------*/


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

    // Initialize hardware timer to simulate pressure cuff
    pressure_cuff_init();

    // Initialize the onboard OLED display
    lcd_init();

    // Initialize hardware port required for playing sound
    speaker_init();

    // Initialize the hardware required for flashing the LED and speaker
    enunciate_init();
  }
}