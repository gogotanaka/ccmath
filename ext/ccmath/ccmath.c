#include "ccmath.h"

VALUE rb_mCcmath;

static ID id_abs, id_arg, id_convert, id_denominator, id_eqeq_p, id_expt,
    id_fdiv, id_negate, id_numerator, id_quo, id_real_p, id_to_f, id_to_i,
    id_to_r, id_i_real, id_i_imag;

static inline double num2dbl_with_to_f(VALUE num) {
  if (RB_TYPE_P(num, T_FLOAT)) {
    return RFLOAT_VALUE(num);
  } else {
    return RFLOAT_VALUE(rb_to_float(num));
  }
}

#define Get_Double(x) num2dbl_with_to_f(x)

#define fun1(n) \
  inline static VALUE f_##n(VALUE x) { return rb_funcall(x, id_##n, 0); }

fun1(real_p)
fun1(negate)

    inline static VALUE f_mul(VALUE x, VALUE y) {
  return rb_funcall(x, '*', 1, y);
}

struct RComplex {
  struct RBasic basic;
  const VALUE real;
  const VALUE imag;
};

#define RCOMPLEX(obj) (R_CAST(RComplex)(obj))
#define RCOMPLEX_SET_REAL(cmp, r) RB_OBJ_WRITE((cmp), &((struct RComplex *)(cmp))->real,(r))
#define RCOMPLEX_SET_IMAG(cmp, i) RB_OBJ_WRITE((cmp), &((struct RComplex *)(cmp))->imag,(i))

inline static VALUE DBL2COMP(double real, double imag) {
  NEWOBJ_OF(obj, struct RComplex, rb_cComplex,
            T_COMPLEX | (RGENGC_WB_PROTECTED_COMPLEX ? FL_WB_PROTECTED : 0));

  RCOMPLEX_SET_REAL(obj, DBL2NUM(real));
  RCOMPLEX_SET_IMAG(obj, DBL2NUM(imag));

  return (VALUE)obj;
}

#define get_dat1(x)     \
  struct RComplex *dat; \
  dat = ((struct RComplex *)(x))

static inline double c_cos(VALUE x) { return cos(Get_Double(x)); }

static inline double c_sin(VALUE x) { return sin(Get_Double(x)); }

static inline double c_cosh(VALUE x) { return cosh(Get_Double(x)); }

static inline double c_sinh(VALUE x) { return sinh(Get_Double(x)); }

static VALUE ccmath_cos(VALUE obj, VALUE x) {
  if (f_real_p(x)) return DBL2NUM(c_cos(x));
  {
    get_dat1(x);
    return DBL2COMP(c_cos(dat->real) * c_cosh(dat->imag),
                    -c_sin(dat->real) * c_sinh(dat->imag));
  }
}

// static VALUE
// ccmath_sin(VALUE x)
// {
//     if (f_real_p(x))
//         return m_sin_bang(x);
//     {
//         get_dat1(x);
//         return f_complex_new2(rb_cComplex,
//                               f_mul(m_sin_bang(dat->real),
//                                     m_cosh_bang(dat->imag)),
//                               f_mul(m_cos_bang(dat->real),
//                                     m_sinh_bang(dat->imag)));
//     }
// }

void Init_ccmath(void) {
  id_real_p = rb_intern("real?");
  id_negate = rb_intern("-@");
  rb_mCcmath = rb_define_module("CCMath");
  rb_cComplex = rb_define_class("Complex", rb_cNumeric);
  rb_define_module_function(rb_mCcmath, "cos", ccmath_cos, 1);
  // rb_define_module_function(rb_mCcmath, "sin", ccmath_sin, 1);
}
