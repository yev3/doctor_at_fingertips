/* Copyright 2017 IAR Systems AB. */

#ifndef _STDATOMIC_H
#define _STDATOMIC_H

#ifdef __cplusplus
#error "Header stdatomic.h is not supported in C++"
#endif

#if defined(__STDC_NO_ATOMICS__)
#error "Atomic not supported in current code model"
#endif

#include <stddef.h>
#include <stdint.h>

#include <xatomic.h>

/* 7.17.1 Introduction */

    // atomic lock-free macros are defined in xatomic.h 

/* 7.17.2 Initialization */
  
#define ATOMIC_VAR_INIT(value) (value)
#define atomic_init __iar_atomic_init

/* 7.17.3 Order and consistency */

    // memory_order enum is defined in xatomic.h
    
#define kill_dependency(y) (y)

/* 7.17.4 Fences */

void atomic_thread_fence(memory_order order);
void atomic_signal_fence(memory_order order);

/* 7.17.5 Lock-free property */

#define atomic_is_lock_free(X) \
                          __iar_atomic_is_lock_free(sizeof(*(X)), __alignof__(*(X))) 
  
/* 7.17.6 Atomic integer types */

#ifdef __cplusplus
typedef _Atomic(bool)               atomic_bool;
#else
typedef _Atomic(_Bool)              atomic_bool;
#endif
typedef _Atomic(char)               atomic_char;
typedef _Atomic(signed char)        atomic_schar;
typedef _Atomic(unsigned char)      atomic_uchar;
typedef _Atomic(short)              atomic_short;
typedef _Atomic(unsigned short)     atomic_ushort;
typedef _Atomic(int)                atomic_int;
typedef _Atomic(unsigned int)       atomic_uint;
typedef _Atomic(long)               atomic_long;
typedef _Atomic(unsigned long)      atomic_ulong;
typedef _Atomic(long long)          atomic_llong;
typedef _Atomic(unsigned long long) atomic_ullong;
typedef _Atomic(uint_least16_t)     atomic_char16_t;
typedef _Atomic(uint_least32_t)     atomic_char32_t;
typedef _Atomic(wchar_t)            atomic_wchar_t;
typedef _Atomic(int_least8_t)       atomic_int_least8_t;
typedef _Atomic(uint_least8_t)      atomic_uint_least8_t;
typedef _Atomic(int_least16_t)      atomic_int_least16_t;
typedef _Atomic(uint_least16_t)     atomic_uint_least16_t;
typedef _Atomic(int_least32_t)      atomic_int_least32_t;
typedef _Atomic(uint_least32_t)     atomic_uint_least32_t;
typedef _Atomic(int_least64_t)      atomic_int_least64_t;
typedef _Atomic(uint_least64_t)     atomic_uint_least64_t;
typedef _Atomic(int_fast8_t)        atomic_int_fast8_t;
typedef _Atomic(uint_fast8_t)       atomic_uint_fast8_t;
typedef _Atomic(int_fast16_t)       atomic_int_fast16_t;
typedef _Atomic(uint_fast16_t)      atomic_uint_fast16_t;
typedef _Atomic(int_fast32_t)       atomic_int_fast32_t;
typedef _Atomic(uint_fast32_t)      atomic_uint_fast32_t;
typedef _Atomic(int_fast64_t)       atomic_int_fast64_t;
typedef _Atomic(uint_fast64_t)      atomic_uint_fast64_t;
typedef _Atomic(intptr_t)           atomic_intptr_t;
typedef _Atomic(uintptr_t)          atomic_uintptr_t;
typedef _Atomic(size_t)             atomic_size_t;
typedef _Atomic(ptrdiff_t)          atomic_ptrdiff_t;
typedef _Atomic(intmax_t)           atomic_intmax_t;
typedef _Atomic(uintmax_t)          atomic_uintmax_t;

/* 7.17.7 Operations on atomic types */

#define atomic_store(object, desired)      __iar_atomic_store(object, desired, memory_order_seq_cst)
#define atomic_store_explicit              __iar_atomic_store

#define atomic_load(object)                __iar_atomic_load(object, memory_order_seq_cst)
#define atomic_load_explicit               __iar_atomic_load

#define atomic_exchange(object, desired)   __iar_atomic_exchange(object, desired, memory_order_seq_cst)
#define atomic_exchange_explicit           __iar_atomic_exchange

#define atomic_compare_exchange_strong(object, expected, desired) \
        __iar_atomic_compare_exchange_strong(object, expected, desired, memory_order_seq_cst, memory_order_seq_cst)
#define atomic_compare_exchange_strong_explicit __iar_atomic_compare_exchange_strong

#define atomic_compare_exchange_weak(object, expected, desired) \
       __iar_atomic_compare_exchange_weak(object, expected, desired, memory_order_seq_cst, memory_order_seq_cst)
#define atomic_compare_exchange_weak_explicit __iar_atomic_compare_exchange_weak

#define atomic_fetch_add(object, operand)  __iar_atomic_fetch_add(object, operand, memory_order_seq_cst)
#define atomic_fetch_add_explicit          __iar_atomic_fetch_add

#define atomic_fetch_sub(object, operand)  __iar_atomic_fetch_sub(object, operand, memory_order_seq_cst)
#define atomic_fetch_sub_explicit          __iar_atomic_fetch_sub

#define atomic_fetch_or(object, operand)   __iar_atomic_fetch_or(object, operand, memory_order_seq_cst)
#define atomic_fetch_or_explicit           __iar_atomic_fetch_or

#define atomic_fetch_xor(object, operand)  __iar_atomic_fetch_xor(object, operand, memory_order_seq_cst)
#define atomic_fetch_xor_explicit          __iar_atomic_fetch_xor

#define atomic_fetch_and(object, operand)  __iar_atomic_fetch_and(object, operand, memory_order_seq_cst)
#define atomic_fetch_and_explicit          __iar_atomic_fetch_and

/* 7.17.8 Atomic flag type and operations */

typedef struct atomic_flag { __iar_atomic_flag _flag; } atomic_flag;

#define ATOMIC_FLAG_INIT { 0 }

#define atomic_flag_test_and_set(object)              __iar_atomic_flag_test_and_set(&((object)->_flag), memory_order_seq_cst)
#define atomic_flag_test_and_set_explicit(object, mo) __iar_atomic_flag_test_and_set(&((object)->_flag), mo)

#define atomic_flag_clear(object)                     __iar_atomic_flag_clear(&((object)->_flag), memory_order_seq_cst)
#define atomic_flag_clear_explicit(object, mo)        __iar_atomic_flag_clear(&((object)->_flag), mo)


#pragma inline=forced
__intrinsic _Bool (atomic_flag_test_and_set)(volatile atomic_flag *object) 
  { return atomic_flag_test_and_set(object); }

#pragma inline=forced
__intrinsic _Bool (atomic_flag_test_and_set_explicit)(volatile atomic_flag *object, memory_order order) 
  {  return atomic_flag_test_and_set_explicit(object, order); }

#pragma inline=forced
__intrinsic void (atomic_flag_clear)(volatile atomic_flag *object) 
  { atomic_flag_clear(object); }

#pragma inline=forced
__intrinsic void (atomic_flag_clear_explicit)(volatile atomic_flag *object, memory_order order) 
  { atomic_flag_clear_explicit(object, order); }


#endif /* _STDATOMIC_H */


