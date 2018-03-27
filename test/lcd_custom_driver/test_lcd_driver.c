/*****************************************************************************
 *                                                                           *
 * Project 2, CPSC 5530 Embedded Systems, Seattle University, 2017           *
 * Team "ARM Brewery": Edward Guevara, David Pierce, and Yevgeni Kamenski    *
 *                                                                           *
 * test_lcd_driver.cpp - 9/30/2017                                           *
 *                                                                           *
 * "THE BEER-WARE LICENSE" (Revision 42):                                    *
 * As long as you retain this notice, you can do whatever you want with this *
 * software. If we meet some day, and you think this software is worth it,   *
 * you can buy us a beer in return. - Team "ARM Brewery"                     *
 *                                                                           *
 *****************************************************************************/

#include <inc/hw_types.h>
#include <inc/hw_nvic.h>
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "utils/lcd_print.h"
#include "types.h"

/**
 * \brief software delay of 100ms
 */
void delay100ms(){
  for (volatile unsigned int i = 80000; i > 0; --i);
}

/**
 * Randomly display a character on the screen.
 */
void print_random(){
  // buffer for displaying a char
  static int rowPosn[LCD_N_COLS] = { 0 };
  static char buf[2] = { 0 };

  // determine a random char and a random row and column to display it
  int col = rand() % LCD_N_COLS;
  int row = rowPosn[col];
  buf[0] = (char)(33 + (rand() % (126 - 33)));
  
  //lcd_set_cursor(row, col);
  lcd_printf_at(row, col, buf);

  // save the printed row so that we can cycle through all rows
  rowPosn[col] = (row + 1) % LCD_N_ROWS;
}

int main(void) {
  // Set the clocking to run directly from the crystal.
  SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
      SYSCTL_XTAL_8MHZ);

  // initialize the LCD display
  lcd_init();
  
  // forever print random chars in random lcd positions
  while(true) {
    print_random();
    delay100ms();
  }
}





