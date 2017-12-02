//
// Created by YK on 11/26/2017.
//

#ifndef EX_COMMS_RTOSDEBUGTRACE_H
#define EX_COMMS_RTOSDEBUGTRACE_H

int lTraceLoggingPrintf( const char *pcFormatString, ... );

#if( ipconfigHAS_DEBUG_PRINTF == 1 )
#define FreeRTOS_debug_printf(X)	lTraceLoggingPrintf X
#endif

#if( ipconfigHAS_PRINTF == 1 )
#define FreeRTOS_printf(X)			lTraceLoggingPrintf X
#endif


#endif //EX_COMMS_RTOSDEBUGTRACE_H
