/* Copyright 2017 IAR Systems AB. */

#ifndef _XATOMIC_H_
#define _XATOMIC_H_

#ifndef _SYSTEM_BUILD
  #pragma system_include
#endif

#ifdef __cplusplus
namespace std {
#endif

// lock-free property
#define ATOMIC_BOOL_LOCK_FREE     __ATOMIC_BOOL_LOCK_FREE
#define ATOMIC_CHAR_LOCK_FREE     __ATOMIC_CHAR_LOCK_FREE
#define ATOMIC_CHAR16_T_LOCK_FREE __ATOMIC_CHAR16_T_LOCK_FREE
#define ATOMIC_CHAR32_T_LOCK_FREE __ATOMIC_CHAR32_T_LOCK_FREE
#define ATOMIC_WCHAR_T_LOCK_FREE  __ATOMIC_WCHAR_T_LOCK_FREE
#define ATOMIC_SHORT_LOCK_FREE    __ATOMIC_SHORT_LOCK_FREE
#define ATOMIC_INT_LOCK_FREE      __ATOMIC_INT_LOCK_FREE
#define ATOMIC_LONG_LOCK_FREE     __ATOMIC_LONG_LOCK_FREE
#define ATOMIC_LLONG_LOCK_FREE    __ATOMIC_LLONG_LOCK_FREE
#define ATOMIC_POINTER_LOCK_FREE  __ATOMIC_POINTER_LOCK_FREE

typedef enum memory_order {
  memory_order_relaxed = __MEMORY_ORDER_RELAXED__, 
  memory_order_consume = __MEMORY_ORDER_CONSUME__, 
  memory_order_acquire = __MEMORY_ORDER_ACQUIRE__,
  memory_order_release = __MEMORY_ORDER_RELEASE__, 
  memory_order_acq_rel = __MEMORY_ORDER_ACQ_REL__, 
  memory_order_seq_cst = __MEMORY_ORDER_SEQ_CST__
} memory_order;

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
  #define _ATOMIC_BOOL bool
#else
  #define _ATOMIC_BOOL _Bool
#endif

typedef _Atomic _ATOMIC_BOOL __iar_atomic_flag;

__intrinsic _ATOMIC_BOOL __iar_atomic_flag_test_and_set(volatile __iar_atomic_flag *object, int memory_order);

__intrinsic void __iar_atomic_flag_clear(volatile __iar_atomic_flag *object, int memory_order);

// Support for reference counting in DLib
typedef long _counter_t;
typedef _Atomic _counter_t _Atomic_counter_t;

#define _Get_atomic_count(_Counter)	(_Counter)

#define _Init_atomic_counter(_Counter, _Value)	\
	(_Counter = _Value)

#define _Inc_atomic_counter_explicit(_Counter, _Order)	\
	(__iar_atomic_add_fetch(&_Counter, 1, _Order))

#define _Inc_atomic_counter(_Counter)	\
	(_Inc_atomic_counter_explicit(_Counter, memory_order_seq_cst))

#define _Dec_atomic_counter_explicit(_Counter, _Order)	\
	(__iar_atomic_sub_fetch(&_Counter, 1, _Order))

#define _Dec_atomic_counter(_Counter)	\
	(_Dec_atomic_counter_explicit(_Counter, memory_order_seq_cst))

#define _Load_atomic_counter_explicit(_Counter, _Order)	\
	__iar_atomic_load(&_Counter, _Order)

#define _Load_atomic_counter(_Counter)	\
	_Load_atomic_counter_explicit(_Counter, memory_order_seq_cst)

#define _Compare_increment_atomic_counter_explicit(_Counter, _Expected, _Order)	\
	__iar_atomic_compare_exchange_weak(&_Counter, &_Expected, _Expected + 1, \
	_Order, _Order)

#define _Compare_increment_atomic_counter(_Counter, _Expected)	\
	_Compare_increment_atomic_counter_explicit( \
		_Counter, _Expected, memory_order_seq_cst)

_EXTERN_C

	void _Lock_shared_ptr_spin_lock(void);
	void _Unlock_shared_ptr_spin_lock(void);

_EXTERN_C_END


#endif /* _XATOMIC_H */
