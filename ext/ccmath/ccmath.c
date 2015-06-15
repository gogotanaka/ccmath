#include "ccmath.h"

VALUE rb_mCcmath;

static ID id_real_p, id_to_f;

static inline int basic_to_f_p(VALUE klass) {
  return rb_method_basic_definition_p(klass, id_to_f);
}

#define RB_BIGNUM_TYPE_P(x) RB_TYPE_P((x), T_BIGNUM)
#define BIGNUM_POSITIVE_P(b) (FIX2LONG(rb_big_cmp((b), INT2FIX(0))) >= 0)

#ifndef DBL_MANT_DIG
#define DBL_MAX_EXP 1024
#endif

#ifndef DBL_MANT_DIG
#define DBL_MANT_DIG 53
#endif

#define fix2dbl_without_to_f(x) (double) FIX2LONG(x)
#define big2dbl_without_to_f(x) rb_big2dbl(x)
#define int2dbl_without_to_f(x) (FIXNUM_P(x) ? fix2dbl_without_to_f(x) : big2dbl_without_to_f(x))
#define rat2dbl_without_to_f(x)               \
  (int2dbl_without_to_f(rb_rational_num(x)) / \
   int2dbl_without_to_f(rb_rational_den(x)))

static inline double num2dbl_with_to_f(VALUE num) {
  if (SPECIAL_CONST_P(num)) {
    if (FIXNUM_P(num)) {
      if (basic_to_f_p(rb_cFixnum)) return fix2dbl_without_to_f(num);
    } else if (FLONUM_P(num)) {
      return RFLOAT_VALUE(num);
    }
  } else {
    switch (BUILTIN_TYPE(num)) {
      case T_FLOAT:
        return RFLOAT_VALUE(num);
      case T_BIGNUM:
        if (basic_to_f_p(rb_cBignum)) return big2dbl_without_to_f(num);
        break;
      case T_RATIONAL:
        if (basic_to_f_p(rb_cRational)) return rat2dbl_without_to_f(num);
        break;
    }
  }
  return RFLOAT_VALUE(rb_to_float(num));
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

#define EXTRACT_DBL(z)                   \
  struct RComplex *dat;                  \
  dat = ((struct RComplex *)(z));        \
  double z_real = Get_Double(dat->real); \
  double z_imag = Get_Double(dat->imag);

#define domain_error(msg) \
    rb_raise(rb_eMathDomainError, "Numerical argument is out of domain - " #msg)

static VALUE ccmath_sqrt(VALUE obj, VALUE z) {
  if (f_real_p(z)) {
    double dz = Get_Double(z);
    if (dz < 0.0) {
      return DBL2COMP(0.0, sqrt(fabs(dz)));
    } else {
      return DBL2NUM(sqrt(Get_Double(dz)));
    }
  } else {
    EXTRACT_DBL(z)
    double r = hypot(z_real, z_imag);
    return DBL2COMP(sqrt((r+z_real)/2.0), sqrt((r-z_real)/2.0));
  }
}

static VALUE ccmath_cos(VALUE obj, VALUE z) {
  if (f_real_p(z)) return DBL2NUM(cos(Get_Double(z)));

  EXTRACT_DBL(z);
  return DBL2COMP(cos(z_real) * cosh(z_imag), -sin(z_real) * sinh(z_imag));
}

static VALUE ccmath_sin(VALUE obj, VALUE z) {
  if (f_real_p(z)) return DBL2NUM(sin(Get_Double(z)));

  EXTRACT_DBL(z);
  return DBL2COMP(sin(z_real) * cosh(z_imag), cos(z_real) * sinh(z_imag));
}

static VALUE ccmath_tan(VALUE obj, VALUE z) {
  if (f_real_p(z)) return DBL2NUM(tan(Get_Double(z)));

  return rb_funcall(ccmath_sin(obj, z), '/', 1, ccmath_cos(obj, z));
}

static VALUE ccmath_cosh(VALUE obj, VALUE z) {
  if (f_real_p(z)) return DBL2NUM(cosh(Get_Double(z)));

  EXTRACT_DBL(z);
  return DBL2COMP(cosh(z_real) * cos(z_imag), sinh(z_real) * sin(z_imag));
}

static VALUE ccmath_sinh(VALUE obj, VALUE z) {
  if (f_real_p(z)) return DBL2NUM(sinh(Get_Double(z)));

  EXTRACT_DBL(z);
  return DBL2COMP(sinh(z_real) * cos(z_imag), cosh(z_real) * sin(z_imag));
}

static VALUE ccmath_tanh(VALUE obj, VALUE z) {
  if (f_real_p(z)) return DBL2NUM(tan(Get_Double(z)));

  return rb_funcall(ccmath_sinh(obj, z), '/', 1, ccmath_cosh(obj, z));
}

static VALUE ccmath_exp(VALUE obj, VALUE z) {
  if (f_real_p(z)) return DBL2NUM(exp(Get_Double(z)));

  EXTRACT_DBL(z);
  double ere = exp(z_real);
  return DBL2COMP(ere * cos(z_imag), ere * sin(z_imag));
}

static double
math_log1(VALUE x)
{
    double d;
    size_t numbits;

    if (RB_BIGNUM_TYPE_P(x) && BIGNUM_POSITIVE_P(x) &&
            DBL_MAX_EXP <= (numbits = rb_absint_numwords(x, 1, NULL))) {
        numbits -= DBL_MANT_DIG;
        x = rb_big_rshift(x, SIZET2NUM(numbits));
    }
    else {
        numbits = 0;
    }

    d = num2dbl_with_to_f(x);
    /* check for domain error */
    if (d < 0.0) domain_error("log");
    /* check for pole error */
    if (d == 0.0) return -INFINITY;

    return log(d) + numbits * log(2); /* log(d * 2 ** numbits) */
}

static double math_log(int argc, const VALUE *argv)
{
    VALUE x, base;
    double d;

    rb_scan_args(argc, argv, "11", &x, &base);
    d = math_log1(x);
    if (argc == 2) {
        d /= math_log1(base);
    }
    return d;
}

static double internal_atan2(double y, double x)
{
#ifndef M_PI
# define M_PI 3.14159265358979323846
#endif

    if (x == 0.0 && y == 0.0) {
        if (!signbit(x)) return y;
        if (!signbit(y)) return M_PI;
        return -M_PI;
    }
#ifndef ATAN2_INF_C99
    if (isinf(x) && isinf(y)) {
        /* optimization for FLONUM */
        double z = (x < 0.0) ? (3.0 * M_PI / 4.0) : (M_PI / 4.0)
        return (y < 0.0) ? -z : z;
    }
#endif
    return atan2(y, x);
}


static VALUE
ccmath_log(int argc, const VALUE *argv, VALUE obj)
{
    VALUE z, base;
    double d;

    rb_scan_args(argc, argv, "11", &z, &base);

    if (f_real_p(z)) return DBL2NUM(math_log(argc, argv));

    // d = math_log1(x);
    // if (argc == 2) {
    //     d /= math_log1(base);
    // }
    EXTRACT_DBL(z);
    float r = hypot(z_real, z_imag);
    return DBL2COMP(log(r), internal_atan2(z_imag, z_real));
}
      // if z.real? && z >= 0 && b >= 0
      //   log!(z, b)
      // else
      //   Complex(log!(z.abs), z.arg) / log(b)
      // end



void Init_ccmath(void) {
  id_real_p = rb_intern("real?");
  id_to_f   = rb_intern("to_f");
  rb_mCcmath = rb_define_module("CCMath");
  rb_cComplex = rb_define_class("Complex", rb_cNumeric);
  rb_define_module_function(rb_mCcmath, "cos", ccmath_cos, 1);
  rb_define_module_function(rb_mCcmath, "sin", ccmath_sin, 1);
  rb_define_module_function(rb_mCcmath, "tan", ccmath_tan, 1);
  rb_define_module_function(rb_mCcmath, "cosh", ccmath_cosh, 1);
  rb_define_module_function(rb_mCcmath, "sinh", ccmath_sinh, 1);
  rb_define_module_function(rb_mCcmath, "tanh", ccmath_tanh, 1);
  rb_define_module_function(rb_mCcmath, "exp", ccmath_exp, 1);
  rb_define_module_function(rb_mCcmath, "log", ccmath_log, -1);
  rb_define_module_function(rb_mCcmath, "sqrt", ccmath_sqrt, 1);

  // rb_define_module_function(rb_mCcmath, "sin", ccmath_sin, 1);
}
