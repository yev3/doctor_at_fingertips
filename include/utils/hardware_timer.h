////////////////////////////////////////////////////////////////////////////////
// Project 5, CPSC 5530 Embedded Systems, Seattle University
// Team "ARM Brewery"
// Edward Guevara, David Pierce, and Yevgeni Kamenski
//
// hardware_timer.h - 09/25/2017 5:17 PM
// Implementations of the functions needed for the hardware timer keeping
// track of the number of elapsed system ticks
//
// This is free and unencumbered software released into the public domain.
////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <stdint.h>

/******************************************************************************
 * Public interface functions and constants related to system timing
 ******************************************************************************/

/**
 * \brief Global tick counter type
 */
typedef unsigned long long system_tick_t;

/**
 * \brief Hook for any additional actions each hardware tick
 */
typedef void (*system_tick_hook_t)(void);

/**
 * \brief Attaches a user hook to be called every hardware tick.
 * \param hook function to call every hardware tick.
 */
void SysTickHookSet(system_tick_hook_t hook);

/**
 * \brief Clears the user hook from being called.
 */
void SysTickHookClear();

/**
 * \brief Default duration of one system tick in milliseconds
 */
#define SYSTEM_TICK_DEFAULT_MS 5

/**
 * \brief Macro to convert milliseconds to system ticks
 */
#define MILLISECS_TO_TICKS(X) ((uint)((uint)(X) / SYSTEM_TICK_DEFAULT_MS))

/**
 * \brief Macro to convert system ticks to millisecs
 */
#define TICKS_TO_MILLISECS(X) ((X) * SYSTEM_TICK_DEFAULT_MS)

/**
 * \brief Certain tasks will be delayed by this amount of hardware ticks.
 */
#define INACTIVE_TASK_TICKS MILLISECS_TO_TICKS(5000)

/**
 * \brief Initialize the hardware timer
 * \param tickDurationMs desired hardware tick duration in milliseconds
 */
void Timer1Init(unsigned long tickDurationMs);

/**
 * \brief Initializes the hardware tick timer using default tick duration
 */
void Timer1InitDefault();

/**
 * Tasks call this method to get the global tick counter to base their timing.
 * @return current elapsed system ticks
 */
system_tick_t GetElapsedTicks();