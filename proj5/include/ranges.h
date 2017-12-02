////////////////////////////////////////////////////////////////////////////////
// Project 3, CPSC 5530 Embedded Systems, Seattle University
// Team "ARM Brewery" 
// Edward Guevara, David Pierce, and Yevgeni Kamenski
// 
// ranges.h - 09/26/2017 6:41 PM
// Defines acceptable ranges for the warning_alarm TCB module
// 
// This is free and unencumbered software released into the public domain.
////////////////////////////////////////////////////////////////////////////////
#pragma once

/**
 * Battery fully-charged raw value
 */
#define BATT_FULL_CHARGE 200

/**
 * Battery low warning raw value
 */
#define BATT_WARN_LOW_PCT 20

/**
 * Minimum of the normal temperature range in degrees Celsius.
 */
#define TEMP_NORM_MIN 36.1f

/**
 * Maximum of the normal temperature range in degrees Celsius.
 */
#define TEMP_NORM_MAX 37.8f

/**
 * Minimum of the normal systolic pressure range in mmHg
 */
#define SYS_PRESS_NORMAL_MIN 120

/**
 * Maximum of the normal systolic pressure range in mmHg
 */
#define SYS_PRESS_NORMAL_MAX 130

/**
 * Minimum of the normal diastolic pressure range in mmHg
 */
#define DIA_PRESS_NORMAL_MIN 70

/**
 * Maximum of the normal diastolic pressure range in mmHg
 */
#define DIA_PRESS_NORMAL_MAX 80

/**
 * Minimum of the normal pulse rate in beats per minute
 */
#define PULSE_NORM_MIN 60

/**
 * Maximum of the normal pulse rate in beats per minute
 */
#define PULSE_NORM_MAX 100