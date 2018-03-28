/***************************************************
 *
 * DLib_Threads.h is the library threads manager.
 *
 * Copyright 2004-2017 IAR Systems AB.  
 *
 * This configuration header file sets up the thread support in the library.
 *
 ***************************************************/

#ifndef _DLIB_THREADS_H
#define _DLIB_THREADS_H

#ifndef _SYSTEM_BUILD
  #pragma system_include
#endif

#include <yvals.h>

_EXTERN_C

/*
 *
 ****************************************************************************
 * Thread support
 * --------------
 *
 * The IAR thread support contains support for variables placed in thread-local
 * storage, TLS (used by the compiler and in the library) and basic locks
 * (used in the library). To get proper thread support the user either has to
 * find a third-party RTOS that is compatible with the IAR library or he has to
 * make one himself.
 *
 * If the user wants to use the IAR thread support, the TLS access function
 * and the lock functions with the names __iar_system_Mtx*() and
 * __iar_file_Mtx*() must be implemented, either with a proper implementation
 * or a dummy one. See below for more info.
 */



/*
 *
 ****************************************************************************
 * TLS
 * -----------
 *
 * To create a TLS variable in the user code, use the _Thread_local (C) or
 * thread_local (C++) type specifiers.
 *
 * Each creation of a thread except the main one (automatically created and
 * initialized at startup) needs to create a TLS area as well. That includes:
 * - Allocating space for the TLS. The size is __iar_tls_size().
 * - Initializing it by calling __iar_tls_init().
 * 
 * At destruction of a thread, if using C++, the function __call_thread_dtors()
 * must be called to destruct any C++ object in the TLS prior to deallocating
 * the TLS space.
 *
 * The access to a TLS variable will call a function that should return the
 * start of the TLS area for the thread. The name of the actual function is
 * dependent on the IAR compiler used. An example of a name of the function
 * is __aeabi_read_tp for ARM, which could look like:
 *
 *   #pragma section="__iar_tls$$DATA"
 *
 *   void *__aeabi_read_tp(void)
 *   {
 *     if (iSMainThread())
 *       return __section_begin("__iar_tls$$DATA");
 *     return threadPtr->TLS_Area;
 *   }
 *
 * The create and destruct functions could look like (we assume that tp is
 * a pointer to the thread control structure):
 *
 *   void create(void)
 *   {
 *     void *p = malloc(__iar_tls_size());
 *     __iar_tls_init(p);
 *     tp->tls = p;
 *   }
 *
 *   void destruct(void)
 *   {
 *     // If using C++.
 *     __call_thread_dtors();
 *   }
 */

/* Function that returns the size needed for the TLS memory area. */
__ATTRIBUTES int __iar_tls_size(void);

/* Function that initializes an allocated memory area in the TLS segment
 * __iar_tls$$DATA. The remaining memory is zeroed based on __iar_tls_size(). */
__ATTRIBUTES void __iar_tls_init(void *);

/* Function that calls the destructor for each dynamically initialized object
 * of a thread (C++). */
__WEAK __ATTRIBUTES void __call_thread_dtors(void);

/* Initializes the main thread, called automatically. */
__ATTRIBUTES void const * __iar_cstart_tls_init(void const * p);

/* Access function */
//  #ifdef __ICCARM__
//    #pragma build_attribute vfpcc_compatible
//    #pragma build_attribute arm_thumb_compatible
//  #endif
//__ATTRIBUTES void *__aeabi_read_tp(void);



/*
 *
 ****************************************************************************
 * LOCKS
 * -----------
 *
 * The following lock interface must be implemented (either with a proper
 * implementation or a dummy one).
 *
 * The interface uses the type __iar_Rmtx * as a pointer to a lock. The
 * __iar_Rmtx type is a pointer to default memory. The interface has three
 * parts:
 * - system locks.      These locks are used for the heap, the file system
 *                      structure, the initialization of statics in C++
 *                      functions, etc. The max number of system locks used in
 *                      C is _MAX_LOCK. C++ also uses some of these locks. 
 * - file locks.        These locks are used for the file streams. The max
 *                      number is FOPEN_MAX. 
 * - C++ dynamic locks. These locks are used for certain C++ objects.
 *
 * The lock and unlock implementation must survive nested calls.
 *
 * Prior to the usage of any locks the function __iar_Initlocks() must be
 * called. It will initialize all of the system and file locks, even if they
 * aren't used by the application.
 *
 * If the main thread uses C++ dynamically initialized objects that use locks,
 * then the locks need to be initialized prior to the C++ dynamic
 * initialization phase in cstartup. The linker option
 * --manual_dynamic_initialization can be used provided that you initialize the
 * main thread's C++ dynamically initialized objects manually by calling the
 * function __iar_dynamic_initialization() (declared in iar_dynamic_init.h)
 * after __iar_Initlocks() has been called.
 *
 * After the last usage of any locks the function __iar_clearlocks() must be
 * called. The functionality that can use locks last are: the calls to the
 * atexit functions and, if files have been used, the call to _Close_all in
 * _exit().
 */

#define _MAX_LOCK          4

__ATTRIBUTES void __iar_system_Mtxinit(__iar_Rmtx *);   // Initialize a system
                                                        // lock
__ATTRIBUTES void __iar_system_Mtxdst(__iar_Rmtx *);    // Destroy a system
                                                        // lock 
__ATTRIBUTES void __iar_system_Mtxlock(__iar_Rmtx *);   // Lock a system lock 
__ATTRIBUTES void __iar_system_Mtxunlock(__iar_Rmtx *); // Unlock a system lock 

__ATTRIBUTES void __iar_file_Mtxinit(__iar_Rmtx *);     // Initialize a file
                                                        // lock
__ATTRIBUTES void __iar_file_Mtxdst(__iar_Rmtx *);      // Destroy a file lock
__ATTRIBUTES void __iar_file_Mtxlock(__iar_Rmtx *);     // Lock a file lock
__ATTRIBUTES void __iar_file_Mtxunlock(__iar_Rmtx *);   // Unlock a file lock

__ATTRIBUTES void __iar_Initdynamiclock(__iar_Rmtx *);  // Initialize a C++
                                                        // dynamic lock
__ATTRIBUTES void __iar_Dstdynamiclock(__iar_Rmtx *);   // Destroy a C++
                                                        // dynamic lock
__ATTRIBUTES void __iar_Lockdynamiclock(__iar_Rmtx *);  // Lock a C++ dynamic
                                                        // lock
__ATTRIBUTES void __iar_Unlockdynamiclock(__iar_Rmtx *);// Unlock a C++
                                                        // dynamic lock




__ATTRIBUTES void __iar_Initlocks(void);

__ATTRIBUTES void __iar_clearlocks(void);

_EXTERN_C_END

#endif /* _DLIB_THREADS_H */

