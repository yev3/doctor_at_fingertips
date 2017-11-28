cmake_minimum_required(VERSION 3.8)
project(ex-comms C)

#set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD 99 )

#ifdef __INT8_T_TYPE__
#ifdef __INT16_T_TYPE__
#ifdef __INT32_T_TYPE__
#ifdef __INT64_T_TYPE__

add_definitions(-D__INT8_T_TYPE__=1)
add_definitions(-D__INT16_T_TYPE__=1)
add_definitions(-D__INT32_T_TYPE__=1)
add_definitions(-D__INT64_T_TYPE__=1)

add_definitions(-D_CLION_)
add_definitions(-DRUN_LOCALLY)
add_definitions(-D__IAR_SYSTEMS_ICC__=9)

add_definitions(-D_DLIB_ONLY_C89=0)
add_definitions(-DIAR_ARMCM3_LM)
add_definitions(-DPACK_STRUCT_END=1)
add_definitions(-Dewarm)
add_definitions(-DPART_LM3S8962)

set(COMMON_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../common/)

set(MINGW_DIR ${COMMON_DIR}FreeRTOS/Source/portable/MSVC-MingW/)

include_directories(
#        ${COMMON_DIR}include/inc/c
        ${MINGW_DIR}

        ${CMAKE_CURRENT_SOURCE_DIR}/include

        ${COMMON_DIR}FreeRTOS/Source/include
#        ${COMMON_DIR}FreeRTOS/Source/portable/IAR/ARM_CM3
        ${COMMON_DIR}FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/include
        ${COMMON_DIR}FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/portable/Compiler/IAR

        ${COMMON_DIR}include




)



set(SOURCE_FILES
        ${MINGW_DIR}port.c
        ${MINGW_DIR}portmacro.h

        src/main.c
        src/tasks/system_tasks.c
        src/tasks/compute.c
        src/tasks/computeEKG.c
        src/tasks/controller.c
        src/tasks/display.c
        src/tasks/enunciate.c
        src/tasks/ipconn.c
        src/tasks/key_scan.c
        src/tasks/measure.c
        src/tasks/measureEKG.c
        src/tasks/serial_comms.c
        src/tasks/status.c

        src/startup_ewarm.c

        include/system_tasks.h
        include/FreeRTOSIPConfig.h
        include/FreeRTOSConfig.h

        src/utils/float_print.c
        src/utils/hardware_timer.c
        src/utils/lcd_print.c
        src/utils/led_control.c
        src/utils/optfft.c
        src/utils/RTOSIPTrace.c
        src/utils/RTOSDebugTrace.c
        src/utils/sockcmdline.c
        src/utils/speaker_control.c
        src/utils/tables.c

        include/task_commands.h src/tasks/cmd_dispatch.c)

set(SOURCE_LIB_UTILS
        ${COMMON_DIR}include/utils/cmdline.c
#        ${COMMON_DIR}include/utils/cpu_usage.c
#        ${COMMON_DIR}include/utils/crc.c
#        ${COMMON_DIR}include/utils/flash_pb.c
#        ${COMMON_DIR}include/utils/fswrapper.c
#        ${COMMON_DIR}include/utils/isqrt.c
#        ${COMMON_DIR}include/utils/locator.c
#        ${COMMON_DIR}include/utils/lwiplib.c
#        ${COMMON_DIR}include/utils/ptpdlib.c
#        ${COMMON_DIR}include/utils/ringbuf.c
#        ${COMMON_DIR}include/utils/scheduler.c
#        ${COMMON_DIR}include/utils/sine.c
#        ${COMMON_DIR}include/utils/softi2c.c
#        ${COMMON_DIR}include/utils/softssi.c
#        ${COMMON_DIR}include/utils/softuart.c
#        ${COMMON_DIR}include/utils/swupdate.c
#        ${COMMON_DIR}include/utils/tftp.c
        ${COMMON_DIR}include/utils/uartstdio.c
        ${COMMON_DIR}include/utils/ustdlib.c
        )

set(RTOS_DIR ${COMMON_DIR}FreeRTOS/Source/)

set(RTOS_SOURCE_FILES
        ${RTOS_DIR}croutine.c
        ${RTOS_DIR}event_groups.c
        ${RTOS_DIR}list.c
        ${RTOS_DIR}queue.c
        ${RTOS_DIR}tasks.c
        ${RTOS_DIR}timers.c

        ${RTOS_DIR}portable/Common/mpu_wrappers.c
#        ${RTOS_DIR}portable/IAR/ARM_CM3/port.c
#        ${RTOS_DIR}portable/IAR/ARM_CM3/portasm.s
#        ${RTOS_DIR}portable/IAR/ARM_CM3/portmacro.h
        ${RTOS_DIR}portable/MemMang/heap_4.c
        )

set(RTOS_IP_DIR ${COMMON_DIR}FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/)

set(RTOS_IP_SOURCE_FILES
        ${RTOS_IP_DIR}FreeRTOS_ARP.c
        ${RTOS_IP_DIR}FreeRTOS_DHCP.c
        ${RTOS_IP_DIR}FreeRTOS_DNS.c
        ${RTOS_IP_DIR}FreeRTOS_IP.c
        ${RTOS_IP_DIR}FreeRTOS_Sockets.c
        ${RTOS_IP_DIR}FreeRTOS_Stream_Buffer.c
        ${RTOS_IP_DIR}FreeRTOS_TCP_IP.c
        ${RTOS_IP_DIR}FreeRTOS_TCP_WIN.c
        ${RTOS_IP_DIR}FreeRTOS_UDP_IP.c

        ${RTOS_IP_DIR}portable/BufferManagement/BufferAllocation_2.c
        ${RTOS_IP_DIR}portable/NetworkInterface/LM3S-8962/NetworkInterface.c

        ${RTOS_IP_DIR}protocols/Common/FreeRTOS_TCP_server.c
        )

add_executable(ex-comms
        ${SOURCE_FILES}
        ${RTOS_SOURCE_FILES}
        ${RTOS_IP_SOURCE_FILES}
        ${SOURCE_LIB_UTILS}
        )
