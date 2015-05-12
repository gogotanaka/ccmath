#include "ccmath.h"

VALUE rb_mCcmath;

static ID id_abs, id_arg, id_convert,
    id_denominator, id_eqeq_p, id_expt, id_fdiv,
    id_negate, id_numerator, id_quo,
    id_real_p, id_to_f, id_to_i, id_to_r,
    id_i_real, id_i_imag;

#define fun1(n) \
inline static VALUE \
f_##n(VALUE x)\
{\
    return rb_funcall(x, id_##n, 0);\
}


fun1(real_p)

static VALUE
ccmath_cos(VALUE obj, VALUE x)
{
    if (f_real_p(x))
        return x;
    {
        return x;
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

void
Init_ccmath(void)
{
  id_real_p = rb_intern("real?");
  rb_mCcmath = rb_define_module("CCMath");
  rb_define_module_function(rb_mCcmath, "cos", ccmath_cos, 1);
  // rb_define_module_function(rb_mCcmath, "sin", ccmath_sin, 1);
}
