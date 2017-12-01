#pragma once
#include <stdint.h>

#ifdef WIN32
#define WIN32 1
#endif

#if WIN32 == 1
#define uvsnprintf vsnprintf
#define usnprintf snprintf
#define ustrcmp strcmp
#endif


uint getRawTempVal();
void NetworkGetMAC(uint8_t *ucMACAddress);
