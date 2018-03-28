/* xtgmath.h internal header */
/* Copyright 2009-2017 IAR Systems AB. */
#ifndef _XTGMATH
#define _XTGMATH

#ifndef _SYSTEM_BUILD
  #pragma system_include
#endif

#ifndef _YVALS
  #include <yvals.h>
#endif /* _YVALS */

#if !_DLIB_ONLY_C89

#ifdef __cplusplus
  extern "C++" {
    #include <xtr1common>
  }

  namespace std {
    extern "C++" {
      template<class _Ty>
      struct _Promote_to_float
      { // promote integral to double
        typedef typename conditional<is_integral<_Ty>::value, 
                                     double, 
                                     _Ty>::type type;
      };

      template<class _Ty1, class _Ty2>
        struct _Common_float_type
        { // find type for two-argument math function
          typedef typename _Promote_to_float<_Ty1>::type _Ty1f;
          typedef typename _Promote_to_float<_Ty2>::type _Ty2f;
          typedef typename conditional<is_same<
               _Ty1f, long double>::value 
            || is_same<_Ty2f, long double>::value,
            long double,
            typename conditional<   is_same<_Ty1f, double>::value
                                 || is_same<_Ty2f, double>::value, 
                                 double,
                                 float>::type>::type type;
        };
    } /* extern "C++" */
  } /* namespace "std" */
#endif /* __cplusplus */

#ifdef __cplusplus
  extern "C++" {
    // MACROS (ONE ARGUMENT)
    #define _TGEN_COMTYP(Carg)     typename _Combined_type<Carg,            \
      typename _Real_type<_T1>::_Type>::_Type

    #define _TGEN1(fn, Carg, Ret) template<class _T1> inline                \
      Ret fn(_T1 _Left)                                                     \
      { /* overload fn(x) returning Ret, real/complex is Carg */            \
        typedef typename _Combined_type<Carg,                               \
                typename _Real_type<_T1>::_Type>::_Type _Tw;                \
        return fn(_Rc_convert(_Left,                                        \
                              (_Tw *)0,                                     \
                              (typename _Rc_type<_T1>::_Type *)0,           \
                              (Carg *)0));                                  \
      }

    #define _TGEN_C(fn)    _TGEN1(fn, char, _TGEN_COMTYP(char))

    #define _TGEN_C0(fn)   _TGEN1(fn, char, typename _Real_type<_T1>::_Type)

    #define _TGEN_R(fn)    _TGEN1(fn, float, typename _Real_type<_T1>::_Type)

    #define _TGEN_RI(fn, Ret) _TGEN1(fn, float, Ret)

    #define _TGEN_RC(fn)   _TGEN1(fn, typename _Rc_type<_T1>::_Type,        \
                                  _TGEN_COMTYP(typename _Rc_type<_T1>::_Type))

    #define _TGEN_RC0(fn)  _TGEN1(fn, typename _Rc_type<_T1>::_Type,        \
                                  typename _Real_type<_T1>::_Type)

    #define _TGEN_RX(fn, Xarg) template<class _T1> inline                   \
      typename _Real_type<_T1>::_Type                                       \
      fn(_T1 _Left, Xarg _Xtra)                                             \
      { /* overload fn(x, z) returning real, with Xarg z */                 \
        typedef typename _Real_type<_T1>::_Type _Tw;                        \
        return fn((_Tw)_Left, _Xtra);                                       \
      }

    // MACROS (TWO ARGUMENTS)
    #define _TGEN2(fn, Carg1, Carg2) template<class _T1, class _T2> inline  \
        typename _Combined_type<                                            \
        typename _Rc_widened<Carg1, Carg2>::_Type,                          \
        typename _Real_widened<typename _Real_type<_T1>::_Type,             \
        typename _Real_type<_T2>::_Type>::_Type>::_Type                     \
        fn(_T1 _Left, _T2 _Right)                                           \
      { /* overload fn(x, y) returning Ret, real/complex is Carg1/Carg2 */  \
        typedef typename _Rc_widened<Carg1, Carg2>::_Type Carg;             \
        typedef typename _Combined_type<Carg,                               \
          typename _Real_widened<typename _Real_type<_T1>::_Type,           \
          typename _Real_type<_T2>::_Type>::_Type>::_Type _Tw;              \
        return fn(_Rc_convert(_Left, (_Tw *)0, (Carg1 *)0, (Carg *)0),      \
                  _Rc_convert(_Right, (_Tw *)0, (Carg2 *)0, (Carg *)0));    \
      }

    #define _TGEN_RC2(fn)  _TGEN2(fn, typename _Rc_type<_T1>::_Type,        \
                                  typename _Rc_type<_T2>::_Type)

    #define _TGEN_R2(fn)   _TGEN2(fn, float, float)

    #define _TGEN_R2X(fn, Xarg) template<class _T1, class _T2> inline       \
        typename _Real_widened<typename _Real_type<_T1>::_Type,             \
        typename _Real_type<_T2>::_Type>::_Type                             \
        fn(_T1 _Left, _T2 _Right, Xarg _Xtra)                               \
      { /* overload fn(x, y, z) returning real, with Xarg z */              \
        typedef typename _Real_widened<typename _Real_type<_T1>::_Type,     \
          typename _Real_type<_T2>::_Type>::_Type _Tw;                      \
        return fn((_Tw)_Left, (_Tw)_Right, _Xtra);                          \
      }

    // MACROS (THREE ARGUMENTS)
    #define _TGEN_R3(fn) template<class _T1, class _T2, class _T3> inline   \
        typename _Real_widened<                                             \
        typename _Real_widened<                                             \
        typename _Real_type<_T1>::_Type,                                    \
        typename _Real_type<_T2>::_Type>::_Type,                            \
        typename _Real_type<_T3>::_Type>::_Type                             \
        fn(_T1 _Left, _T2 _Right, _T3 _Xtra)                                \
      { /* overload fn(x, y, z) returning real */                           \
        typedef typename _Real_widened<                                     \
          typename _Real_widened<                                           \
          typename _Real_type<_T1>::_Type,                                  \
          typename _Real_type<_T2>::_Type>::_Type,                          \
          typename _Real_type<_T3>::_Type>::_Type _Tw;                      \
        return fn((_Tw)_Left, (_Tw)_Right, (_Tw)_Xtra);                     \
      }

    // TEMPLATE FUNCTION _Rc_convert
    template<class _From, class _To,
             class _From_rc, class _To_rc>
      _To _Rc_convert(const _From& _Val, _To *, _From_rc *, _To_rc *)
    { // convert _Val to desired type
      return (_To)_Val;
    }

    template<class _From, class _To>
      _To _Rc_convert(const _From& _Val, _To *, char *, char *)
    { // convert _Val to desired type, complex = complex
      _To _Ans;

      _Ans._Val[0] = _Val._Val[0];
      _Ans._Val[1] = _Val._Val[1];
      return _Ans;
    }

    template<class _From, class _To>
      _To _Rc_convert(_From _Val, _To *, float *, char *)
    { // convert _Val to desired type, complex = real
      _To _Ans;

      _Ans._Val[0] = _Val;
      _Ans._Val[1] = 0;
      return _Ans;
    }

    // TEMPLATE CLASS _Rc_type
    template<class _Ty>
    struct _Rc_type
    { // determine if type is real or complex
      typedef float _Type;    // default is real
    };

    // TEMPLATE CLASS _Rc_widened
    template<class _Ty, class _T2>
      struct _Rc_widened
      { // determine real/complex type
        typedef char _Type;     // non (real, real) is char (complex)
      };

    template<> struct _Rc_widened<float, float>
    { // determine real/complex
      typedef float _Type;    // (real, real) is real
    };

    // TEMPLATE CLASS _Real_type
    template<class _Ty>
    struct _Real_type
    { // determine equivalent real type
      typedef double _Type;   // default is double
    };

    template<> struct _Real_type<float>
    { // determine equivalent real type
      typedef float _Type;
    };

    template<> struct _Real_type<long double>
    { // determine equivalent real type
      typedef long double _Type;
    };

    // TEMPLATE CLASS _Real_widened
    template<class _Ty, class _T2>
      struct _Real_widened
      { // determine widened real type
        typedef long double _Type;      // default is long double
      };

    template<> struct _Real_widened<float, float>
    { // determine widened real type
      typedef float _Type;
    };

    template<> struct _Real_widened<float, double>
    { // determine widened real type
      typedef double _Type;
    };

    template<> struct _Real_widened<double, float>
    { // determine widened real type
      typedef double _Type;
    };

    template<> struct _Real_widened<double, double>
    { // determine widened real type
      typedef double _Type;
    };

    // TEMPLATE CLASS _Combined_type
    template<class _Trc, class _Tre>
    struct _Combined_type
    { // determine combined type
      typedef float _Type;    // (real, float) is float
    };

    template<> struct _Combined_type<float, double>
    { // determine combined type
      typedef double _Type;
    };

    template<> struct _Combined_type<float, long double>
    { // determine combined type
      typedef long double _Type;
    };
  } /* extern "C++" */
#endif /* __cplusplus */

#endif /* !_DLIB_ONLY_C89 */
#endif /* _XTGMATH */

/*
 * Copyright (c) by P.J. Plauger. All rights reserved.
 * Consult your license regarding permissions and restrictions.
V6.50:0576 */
