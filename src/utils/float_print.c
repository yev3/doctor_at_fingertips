////////////////////////////////////////////////////////////////////////////////
// Project 5, CPSC 5530 Embedded Systems, Seattle University
// Team "ARM Brewery": Edward Guevara, David Pierce, and Yevgeni Kamenski
//
// print.c - 09/26/2017 9:00 AM
// Helper function declarations to aid in printing to the LCD.
//
// This is free and unencumbered software released into the public domain.
////////////////////////////////////////////////////////////////////////////////

#include "utils/float_print.h"

/**
 * \brief Prints a formatted floating point value to the buffer, to overcome
 * the lack of the %f flag in the libraries.
 * \param buf the buffer to print to
 * \param n the size of the buffer, including zero char
 * \param val the floating point value to display
 * \param padding total padding of the value
 * \param precision number of precision digits after the decimal point
 */
int sprint_float(char *buf, unsigned long n, 
                 double val, int padding, int precision) {
  // determine number of chars not including the nullterm char
  if (n == 0) return 0;
  int bufRemain = (int) (n - 1);
  int numWritten = 0;
  char *writeBuf = buf;

  // Do not print anything if cursor is beyond the LCD edge
  if (bufRemain <= 0) return 0;

  padding = (int) (bufRemain > padding ? padding : bufRemain);

  // when value is nan, print "nan" and exit
  if (isnan(val)) {
    return usnprintf(buf, n, "nan");
  }

  // when value is inf, print "inf" and exit
  if (isinf(val)) {
    return usnprintf(buf, n, "inf");
  }

  // when value is negative, print '-' and treat remaining as positive
  if (val < 0.0) {
    padding--;
    bufRemain--;
    numWritten++;
    *writeBuf++ = '-';
    val = -val;
  }

  // Round correctly so that print(1.999, 2) prints as "2.00"
  double rounding = 0.5;
  for (int i = 0; i < precision; ++i) rounding /= 10.0;
  val += rounding;

  // Extract the integer part of the value
  unsigned long intPart = (unsigned long) val;
  double remainder = val - (double) intPart;

  // determine value of digits required
  int nIdx;
  int nDigits = 1;
  for (nIdx = 1;
       (((nIdx * 10u) <= intPart) && (((nIdx * 10u) / 10u) == nIdx));
       nIdx *= 10u) {
    nDigits++;
  }

  // adjust the count of the remaining number of buffer and padding chars
  if (nDigits > bufRemain) {
    *writeBuf++ = '\0';
    return numWritten;
  }
  bufRemain -= nDigits;
  padding -= nDigits;
  numWritten += nDigits;

  // nbuf will be used for storing the whole number part
  char *nbuf = writeBuf + nDigits;
  writeBuf = writeBuf + nDigits;

  // output the integer part of the value
  do {
    char c = (char) (intPart % 10u);
    intPart /= 10u;
    *--nbuf = (char) (c + '0');
  } while (intPart);


  // Print the decimal point, but only if there is space
  if (precision > 0) {
    if (bufRemain > 0) {
      padding--;
      bufRemain--;
      numWritten++;
      *writeBuf++ = '.';
    }
  }

  // Extract digits from the remaing fraction part and output it
  while (precision-- > 0 && bufRemain-- > 0) {
    remainder *= 10.0;
    unsigned int toPrint = (unsigned int) (remainder) % 10;
    *writeBuf++ = (char) (toPrint + '0');
    remainder -= toPrint;
    padding--;
    numWritten++;
  }

  // if the output is less than the user's padding, output padding chars
  if (padding > 0) {
    memmove(buf + padding, buf, writeBuf - buf);
    memset(buf, ' ', padding);
    numWritten += padding;
  }

  buf[numWritten] = '\0';
  return numWritten;
}

