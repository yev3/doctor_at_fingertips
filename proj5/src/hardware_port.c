#include <stdint.h>
#include <assert.h>

#include <inc/hw_memmap.h>
#include <driverlib/sysctl.h>
#include <driverlib/gpio.h>
#include <driverlib/adc.h>
#include <driverlib/flash.h>

#include "drivers/pulse_transducer.h"
#include "tasks/commands.h"
#include "tasks/system.h"

#include <FreeRTOS_IP.h>

/**
 * \brief Initializes the hardware peripherals needed to read the keypad
 */
#include <drivers/pulse_transducer.h>

void keypad_init() {
  // Enable the peripherals used by the application.
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

  // Configure the GPIOs used to read the state of the on-board push buttons.
  GPIOPinTypeGPIOInput(GPIO_PORTE_BASE,
                       GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
  GPIOPadConfigSet(GPIO_PORTE_BASE,
                   GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3,
                   GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
  GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_1);
  GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_STRENGTH_2MA,
                   GPIO_PIN_TYPE_STD_WPU);
}

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

uint getRawTempVal() {
  // Trigger the ADC conversion.
  ADCProcessorTrigger(ADC0_BASE, 3);

  // Wait for conversion to be completed.
  while(!ADCIntStatus(ADC0_BASE, 3, false)) {
    portYIELD();
  }

  // Read ADC Value.
  unsigned long val;
  ADCSequenceDataGet(ADC0_BASE, 3, &val);
  return (uint)val;
}

void NetworkGetMAC(uint8_t *ucMACAddress) {
  // Configure the hardware MAC address for Ethernet Controller
  // filtering of incoming packets.
  //
  // For the LM3S6965 Evaluation Kit, the MAC address will be stored in the
  // non-volatile USER0 and USER1 registers.  These registers can be read
  // using the FlashUserGet function, as illustrated below.
  unsigned long ulUser0, ulUser1;
  FlashUserGet(&ulUser0, &ulUser1);

  //
  // Convert the 24/24 split MAC address from NV ram into a 32/16 split
  // MAC address needed to program the hardware registers, then program
  // the MAC address into the Ethernet Controller registers.
  //
  ucMACAddress[0] = ((ulUser0 >> 0) & 0xff);
  ucMACAddress[1] = ((ulUser0 >> 8) & 0xff);
  ucMACAddress[2] = ((ulUser0 >> 16) & 0xff);
  ucMACAddress[3] = ((ulUser1 >> 0) & 0xff);
  ucMACAddress[4] = ((ulUser1 >> 8) & 0xff);
  ucMACAddress[5] = ((ulUser1 >> 16) & 0xff);

  char *a = (char *)ucMACAddress;
  FreeRTOS_debug_printf(("Using MAC %02X:%02X:%02X:%02X:%02X:%02X\n",
    a[0], a[1], a[2], a[3], a[4], a[5]));
}

/**
 * \brief Handler for simulated EKG measurements. It uses a sinusoidal sensor output.
 */
void Timer2IntHandler() {
  static ulong ticks = 0;
  TimerIntClear(TIMER2_BASE, TIMER_TIMA_TIMEOUT);
  ticks ++;
}

/**
 * \brief Initialization of EKG measurement timer interrupt.
 */
void measure_ekg_init() {
  // Get the number of system time cycles (not ticks) for one sys tick
  ulong sysTickPeriod = SysCtlClockGet();

  // Enable the timer peripheral
  SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2);

  // Enable processor interrupts.
  IntMasterEnable();

  // Configure 32-bit periodic timer.
  TimerConfigure(TIMER2_BASE,  TIMER_CFG_32_BIT_PER);
  TimerLoadSet(TIMER2_BASE, TIMER_A, sysTickPeriod);

  // Load the next timer period
  Timer2IntHandler();

  // Setup the interrupt for the timer timeout.
  IntEnable(INT_TIMER2A);
  TimerIntEnable(TIMER2_BASE, TIMER_TIMA_TIMEOUT);

  // Enable the timer.
  TimerEnable(TIMER2_BASE, TIMER_A);
}

