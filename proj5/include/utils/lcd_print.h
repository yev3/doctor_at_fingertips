////////////////////////////////////////////////////////////////////////////////
// Project 3, CPSC 5530 Embedded Systems, Seattle University
// Team "ARM Brewery" 
// Edward Guevara, David Pierce, and Yevgeni Kamenski
// 
// lcd_print.h - 09/26/2017 8:53 AM
// Helper function definitions to aid in printing to the LCD.
//
// This is free and unencumbered software released into the public domain.
////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "types.h"

#define LCD_CHAR_WIDTH 6U       /* LCD char width in px */
#define LCD_CHAR_HEIGHT 8U      /* LCD char height in px */
#define LCD_N_ROWS 12           /* Number of rows that can fit on the LCD */
#define LCD_N_COLS 21           /* Number of columns that can fit on the LCD */

/**
 * Sets the cursor position in the window so that the next print start at the
 * given character in  grid
 * @param row 0-indexed row number
 * @param col 0-indexed col number
 */
void lcd_set_cursor(int row, int col);

/**
 * Prints the buffer at the current cursor location.
 */
void lcd_print(const char *str);

/**
 * Display the printf-formatted string on the LCD
 * @param format the format of the string
 * @param ... varargs of the printf
 */
void lcd_printf(const char *format, ...);

/**
 * Display the printf-formatted string on the LCD at the indicated location
 * @param row 0-indexed row number
 * @param col 0-indexed col number
 * @param format the format of the string
 * @param ... varargs of the printf
 */
void lcd_printf_at(int row, int col, const char *format, ...);

/**
 * Prints a formatted floating point number, since lcd_printf does not
 * support %f flag.
 * @param value the floating point number to display
 * @param padding total padding of the number
 * @param precision number of precision digits after the decimal point
 */
void lcd_print_float(double value, int padding, int precision);

/**
 * Initializes the LCD.
 */
void lcd_init();


/**
 * \brief Clears the LCD.
 */
void lcd_clear();
