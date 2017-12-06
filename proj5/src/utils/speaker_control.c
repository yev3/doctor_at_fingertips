/*****************************************************************************
 * Project 5, CPSC 5530 Embedded Systems, Seattle University, 2017           *
 * Team "ARM Brewery": Edward Guevara, David Pierce, and Yevgeni Kamenski    *
 *                                                                           *
 * pwm_control.c - 10/21/2017                                                *
 * PWM speaker init and beep on/off functions                                *
 *                                                                           *
 * "THE BEER-WARE LICENSE" (Revision 42):                                    *
 * As long as you retain this notice, you can do whatever you want with this *
 * software. If we meet some day, and you think this software is worth it,   *
 * you can buy us a beer in return. - Team "ARM Brewery"                     *
 *****************************************************************************/
#include "utils/speaker_control.h"

/**
 * Initializes the PWM port (G)
 */
void speaker_init() {
  ulong ulPeriod = SysCtlClockGet() / 2048;    ///< alarm Hz period
  SysCtlPWMClockSet(SYSCTL_PWMDIV_1);

  // Enable sound peripherals used by alarm
  SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);
  GPIOPinTypePWM(GPIO_PORTG_BASE, GPIO_PIN_1);

  // Set PWM Generation frequency
  PWMGenConfigure(PWM0_BASE, PWM_GEN_0,
                  PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC);
  PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, ulPeriod);

  // Lower the volume a bit
  PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1, ulPeriod / 512);

  // Enable the PWM1 output signals.
  PWMOutputState(PWM0_BASE, PWM_OUT_1_BIT, true);

  // Ensure that the sound is initially off
  sound_off();
}

/**
 * \brief Turns on the board speaker
 */
void sound_on() {
  PWMGenEnable(PWM0_BASE, PWM_GEN_0);
}

/**
 * \brief Turns off the board speaker
 */
void sound_off() {
  PWMGenDisable(PWM0_BASE, PWM_GEN_0);
}