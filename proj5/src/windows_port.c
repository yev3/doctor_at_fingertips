#include "ConsoleLogger.h"
#include "curses.h"
#include "portmacro.h"
#include "utils/hardware_timer.h"
#include <drivers/pulse_transducer.h>
#include <FreeRTOS.h>
#include "task.h"
#include "queue.h"
#include "tasks/commands.h"

/*
 * HARDWARE TIMER
 */

/**
 * \brief Attaches a user hook to be called every hardware tick.
 * \param hook function to call every hardware tick.
 */
void SysTickHookSet(system_tick_hook_t hook) {}

/**
 * \brief Clears the user hook from being called.
 */
void SysTickHookClear() {}

/**
 * \brief Initialize the hardware timer
 * \param tickDurationMs desired hardware tick duration in milliseconds
 */
void Timer1Init(ulong tickDurationMs){}

/**
 * \brief Initializes the hardware tick timer using default tick duration
 */
void Timer1InitDefault() {}

/**
 * Tasks call this method to get the global tick counter to base their timing.
 * @return current elapsed system ticks
 */
system_tick_t GetElapsedTicks() { return 0; }







void SysCtlClockSet(unsigned long ulConfig) {}

// Initialize the UART to display serial text to the user's PC terminal
void serial_init() {
  LoggerInitWindow("serial_port", "ConsoleLoggerHelper.exe");
}

// Initialize hardware ports needed to flash the LEDs
void led_init(){}

// Initialize temperature sensor
void temperature_init(){}

//    // Initialize simulated EKG measurement interrupt
//    measure_ekg_init();

// Initialize hardware ports needed to read key presses
void keypad_init(){}

// Initialize the hardware timer to simulate the pulse transducer driver
void pulse_init(){}

// Initialize hardware port required for playing sound
void speaker_init(){}

// Initialize the hardware required for flashing the LED and speaker
void enunciate_init(){}


////////////////////////////////////////////////////////////////////////////////
// Output functions
////////////////////////////////////////////////////////////////////////////////

int UARTwrite(const char *pcBuf, unsigned long ulLen) {
  return LoggerWrite(pcBuf, ulLen);
}

void lcd_init() {
  initscr();
}
void lcd_clear() {
  clear();
  refresh();
}

void lcd_set_cursor(int row, int col) {
  move(row, col);
}

void lcd_print(const char *str) {
  addstr(str);
  refresh();
}

void lcd_printf(const char *format, ...) {
  va_list args;
  va_start(args, format);
  vwprintw(stdscr, format, args);
  va_end(args);
  refresh();
}

void lcd_printf_at(int row, int col, const char *format, ...) {
  va_list args;
  if (move(row, col) == ERR) {
    addstr("ERR");
    return;
  }

  va_start(args, format);
  vwprintw(stdscr, format, args);
  va_end(args);
  refresh();
}

void lcd_print_float(double value, int padding, int precision) {
  printw("%*.*f", padding, precision, value);
  refresh();
}

////////////////////////////////////////////////////////////////////////////////
// TASKS
////////////////////////////////////////////////////////////////////////////////

void enunciate(void* arg) {
  for(;;) {
    portYIELD();
  }
}

void serial_comms(void* arg) {
  for(;;) {
    portYIELD();
  }
}

void sendKeyPress(QueueHandle_t keyQueue, KeyPress_t key) {
  xQueueSend(keyQueue, &key, 0);  
}

void key_scan(void *arg) {

  nodelay(stdscr,TRUE); // turn off getch() wait
  curs_set(0);          // turn off the cursor
  keypad(stdscr, TRUE); // use keypad feature of ncurses
  noecho();

  QueueHandle_t keyQueue = *(QueueHandle_t*) arg;
  for (;;) {

    int key = getch();
    if (ERR != key) {
      if (KEY_LEFT == key) {
        sendKeyPress(keyQueue, cmdKEY_LEFT);
      } else if (KEY_RIGHT == key) {
        sendKeyPress(keyQueue, cmdKEY_RIGHT);
      } else if (KEY_UP == key) {
        sendKeyPress(keyQueue, cmdKEY_UP);
      } else if (KEY_DOWN == key) {
        sendKeyPress(keyQueue, cmdKEY_DOWN);
      } else if ('\n' == key) {
        sendKeyPress(keyQueue, cmdKEY_SELECT);
      } 
    }
    vTaskDelay(pdMS_TO_TICKS(5));
  }

  
}

////////////////////////////////////////////////////////////////////////////////
// NETWORK
////////////////////////////////////////////////////////////////////////////////

void NetworkGetMAC(uint8_t *ucMACAddress) {
  ucMACAddress[0] = 0x11;
  ucMACAddress[1] = 0x22;
  ucMACAddress[2] = 0x33;
  ucMACAddress[3] = 0x44;
  ucMACAddress[4] = 0x55;
  ucMACAddress[5] = 0x66;
}

////////////////////////////////////////////////////////////////////////////////
// ADC
////////////////////////////////////////////////////////////////////////////////
uint getRawTempVal() {
  return 100;
}

////////////////////////////////////////////////////////////////////////////////
// Pulse Hooks
////////////////////////////////////////////////////////////////////////////////

/**
 * \brief Sets the function to be called on every timer interrupt
 * \param hook the function that is called when a pulse occurs
 */
void pulseHookSet(pulseIntHook_t hook) {
  // TODO
}

/**
 * \brief Clears the interrupt function. Simulates disabling the pulse
 * measurement interrupt.
 */
void pulseHookClear() {
  // TODO
}
