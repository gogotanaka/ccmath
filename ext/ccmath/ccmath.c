#include "ccmath.h"

VALUE rb_mCcmath;

static ID id_real_p;

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

#define EXTRACT_DBL(z)     \
  struct RComplex *dat; \
  dat = ((struct RComplex *)(z)); \
  double z_real = Get_Double(dat->real); \
  double z_imag = Get_Double(dat->imag);


static VALUE ccmath_cos(VALUE obj, VALUE z) {
  if (f_real_p(z)) return DBL2NUM(cos(Get_Double(z)));
  {
    EXTRACT_DBL(z);
    return DBL2COMP(cos(z_real) * cosh(z_imag),
                    - sin(z_real) * sinh(z_imag));
  }
}

static VALUE ccmath_sin(VALUE obj, VALUE z) {
  if (f_real_p(z)) return DBL2NUM(sin(Get_Double(z)));
  {
    EXTRACT_DBL(z);
    return DBL2COMP(sin(z_real) * cosh(z_imag),
                    + cos(z_real) * sinh(z_imag));
  }
}

void Init_ccmath(void) {
  id_real_p = rb_intern("real?");
  rb_mCcmath = rb_define_module("CCMath");
  rb_cComplex = rb_define_class("Complex", rb_cNumeric);
  rb_define_module_function(rb_mCcmath, "cos", ccmath_cos, 1);
  rb_define_module_function(rb_mCcmath, "sin", ccmath_sin, 1);
  // rb_define_module_function(rb_mCcmath, "sin", ccmath_sin, 1);
}
