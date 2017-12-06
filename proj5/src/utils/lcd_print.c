////////////////////////////////////////////////////////////////////////////////
// Project 5, CPSC 5530 Embedded Systems, Seattle University
// Team "ARM Brewery" 
// Edward Guevara, David Pierce, and Yevgeni Kamenski
// 
// print.c - 09/26/2017 9:00 AM
// Helper function declarations to aid in printing to the LCD.
// 
// This is free and unencumbered software released into the public domain.
////////////////////////////////////////////////////////////////////////////////

#include <driverlib/rit128x96x4.h>
#include <math.h>
#include <string.h>
#include "utils/ustdlib.h"
#include "utils/lcd_print.h"
#include "tasks/system.h"

#define UC_LEVEL 15                     /* Contrast level of the LCD chars */
static int cursorX = 0;                 ///< Current LCD X position
static int cursorY = 0;                 ///< Current LCD Y position
static char lineBuffer[LCD_N_COLS + 1]; ///< LCD line buffer

/**
 * \brief Sets the cursor position in the window so that the next print start at the
 * given character in  grid
 * \param row 0-indexed row number
 * \param col 0-indexed col number
 */
void lcd_set_cursor(int row, int col) {
  cursorY = row;
  cursorX = col;
}

/**
 * \brief Prints the buffer at the current cursor location.
 * \param buf c-string to printout
 */
void lcd_print(const char *buf) {
  RIT128x96x4StringDraw(buf,
                        cursorX * LCD_CHAR_WIDTH,
                        cursorY * LCD_CHAR_HEIGHT,
                        UC_LEVEL);
  // set the cursor position to the location after the last output char
  cursorX += strlen(buf);
}

/**
 * \brief Helper function to print the formatted string using variable arg
 * \param format printf-formatted string
 * \param ap variable argument list
 */
void lcd_printf_args(const char *format, va_list ap) {
  // Determine how many characters are left until the edge of the LCD
  const int numToEdge = LCD_N_COLS - cursorX;

  // Do not print anything if nothing will fit
  if (numToEdge <= 0) return;

  // Generate characters for printing based on the format
  int numActual = uvsnprintf(lineBuffer, (ulong) (numToEdge) + 1ul, format, ap);

  // ensure the buffer is null-terminated no matter what
  lineBuffer[LCD_N_COLS] = 0;

  // when truncated
  if (numActual < 0 || numActual >= numToEdge) {
    numActual = numToEdge;
  }

  // copy up to the edge and print to console
  lcd_print(lineBuffer);
}

/**
 * \brief Display the printf-formatted string on the LCD
 * \param format the format of the string
 * \param ... varargs of the printf
 */
void lcd_printf(const char *format, ...) {
  // construct var arg pointer
  va_list ap;
  va_start(ap, format);

  // printf the formatted arguments at the current cursor position
  lcd_printf_args(format, ap);

  // construct var arg pointer
  va_end(ap);
}

/**
 * \brief Display the printf-formatted string on the LCD at the indicated
 * location
 * \param row 0-indexed row number
 * \param col 0-indexed col number
 * \param format the format of the string
 * \param ... varargs of the printf
 */
void lcd_printf_at(int row, int col, const char *format, ...) {
  // Set the LCD cursor at the user's desired position
  lcd_set_cursor(row, col);

  // construct var arg pointer
  va_list ap;
  va_start(ap, format);

  // printf the formatted arguments at the current cursor position
  lcd_printf_args(format, ap);

  // destroy var arg pointer
  va_end(ap);
}

/**
 * \brief Initializes the LCD.
 */
void lcd_init() {
  // Initialize the OLED display
  RIT128x96x4Init(1000000);

  // Clear the contents of the OLED display
  RIT128x96x4Clear();
}

/**
 * \brief Prints a formatted floating point number, since lcd_printf does not
 * support %f flag.
 * Note: Ideas borrowed from https://github.com/arduino/Arduino implementation
 * \param value the floating point number to display
 * \param padding total padding of the number
 * \param precision number of precision digits after the decimal point
 */
void lcd_print_float(double number, int padding, int digits) {
  // declare the current line buffer and initialize all chars to 0
  // static char lineBuffer[LCD_N_COLS + 1];
  memset(lineBuffer, 0, sizeof(lineBuffer));

  // determine number of chars to edge of LCD
  const int numToEdge = LCD_N_COLS - cursorX;

  // Do not print anything if cursor is beyond the LCD edge
  if (numToEdge <= 0) return;

  char *buf = lineBuffer;
  int remBuffer = LCD_N_COLS;
  padding = LCD_N_COLS > padding ? padding : LCD_N_COLS;

  // when number is nan, print "nan" and exit
  if (isnan(number)) {
    lcd_print("nan");
    return;
  }

  // when number is inf, print "inf" and exit
  if (isinf(number)) {
    lcd_print("inf");
    return;
  }

  // when number is too large for LCD, print "ovf" and exit
  if (number > 4294967040.0) {
    lcd_print("ovf");  // constant determined empirically
    return;
  }

  // when number is too small for LCD, print "ovf" and exit
  if (number < -4294967040.0) {
    lcd_print("ovf");  // constant determined empirically
    return;
  }

  // when number is negative, print '-' and treat remaining as positive
  if (number < 0.0) {
    padding--;
    remBuffer--;
    *buf++ = '-';
    number = -number;
  }

  // Round correctly so that print(1.999, 2) prints as "2.00"
  double rounding = 0.5;
  for (int i = 0; i < digits; ++i) rounding /= 10.0;
  number += rounding;

  // Extract the integer part of the number
  unsigned long intPart = (unsigned long) number;
  double remainder = number - (double) intPart;

  // determine number of digits required
  int nIdx;
  int nDigits = 1;
  for (nIdx = 1;
       (((nIdx * 10u) <= intPart) && (((nIdx * 10u) / 10u) == nIdx));
       nIdx *= 10u) {
    nDigits++;
  }

  // adjust the count of the remaining number of buffer and padding chars
  if (nDigits > remBuffer) return;
  remBuffer -= nDigits;
  padding -= nDigits;

  // nbuf will be used for storing the whole number part
  char *nbuf = buf + nDigits;
  buf = buf + nDigits;

  // output the integer part of the number
  do {
    char c = intPart % 10u;
    intPart /= 10u;
    *--nbuf = c + '0';
  } while (intPart);


  // Print the decimal point, but only if there is space
  if (digits > 0) {
    if (remBuffer > 0) {
      padding--;
      remBuffer--;
      *buf++ = '.';
    }
  }

  // Extract digits from the remaing fraction part and output it
  while (digits-- > 0 && remBuffer-- > 0) {
    remainder *= 10.0;
    uint toPrint = (unsigned int) (remainder) % 10;
    *buf++ = toPrint + '0';
    remainder -= toPrint;
    padding--;
  }

  // if the output is less than the user's padding, output padding chars
  if (padding > 0) {
    memmove(lineBuffer + padding, lineBuffer, buf - lineBuffer);
    memset(lineBuffer, ' ', padding);
  }

  // output the result to the LCD
  lcd_print(lineBuffer);
}

/**
 * \brief Clears the LCD.
 */
void lcd_clear() {
  RIT128x96x4Clear();
}
