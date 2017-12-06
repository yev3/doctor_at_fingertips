////////////////////////////////////////////////////////////////////////////////
// Project 5, CPSC 5530 Embedded Systems, Seattle University
// Team "ARM Brewery"
// Edward Guevara, David Pierce, and Yevgeni Kamenski
//
// float_print.h - 09/26/2017 8:53 AM
// Routine to print floating point numbers to the buffer
//
// This is free and unencumbered software released into the public domain.
////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <string.h>
#include <math.h>
#include "types.h"
#include "utils/ustdlib.h"
#include "hardware_port.h"

/**
 * \brief Prints a formatted floating point number to the buffer, to overcome
 * the lack of the %f flag in the libraries.
 * \param buf the buffer to print to
 * \param n the size of the buffer, including zero char
 * \param value the floating point number to display
 * \param padding total padding of the number
 * \param precision number of precision digits after the decimal point
 */
int sprint_float(char* buf, ulong n, double val, int padding, int precision);