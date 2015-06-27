#include "ccmath.h"

VALUE rb_mCcmath;

static ID id_real_p;

#define RB_BIGNUM_TYPE_P(x) RB_TYPE_P((x), T_BIGNUM)
#define BIGNUM_POSITIVE_P(b) (FIX2LONG(rb_big_cmp((b), INT2FIX(0))) >= 0)

#define fix2dbl_without_to_f(x) (double) FIX2LONG(x)
#define big2dbl_without_to_f(x) rb_big2dbl(x)
#define int2dbl_without_to_f(x) \
    (FIXNUM_P(x) ? fix2dbl_without_to_f(x) : big2dbl_without_to_f(x))
#define rat2dbl_without_to_f(x) \
    (int2dbl_without_to_f(rb_rational_num(x)) / int2dbl_without_to_f(rb_rational_den(x)))

static inline double num2dbl_without_to_f(VALUE num)
{
    if (SPECIAL_CONST_P(num)) {
        if (FIXNUM_P(num)) {
            return fix2dbl_without_to_f(num);
        }
        else if (FLONUM_P(num)) {
            return RFLOAT_VALUE(num);
        }
    }
    else {
        switch (BUILTIN_TYPE(num)) {
        case T_FLOAT:
            return RFLOAT_VALUE(num);
        case T_BIGNUM:
            return big2dbl_without_to_f(num);
            break;
        case T_RATIONAL:
            return rat2dbl_without_to_f(num);
            break;
        }
    }
    return RFLOAT_VALUE(rb_to_float(num));
}

#define NUM2DBL_F(x) num2dbl_without_to_f(x)

inline static VALUE f_real_p(VALUE x)
{
    return rb_funcall(x, id_real_p, 0);
}

inline static VALUE DBL2COMP(double real, double imag)
{
    NEWOBJ_OF(obj, struct RComplex, rb_cComplex,
        T_COMPLEX | (RGENGC_WB_PROTECTED_COMPLEX ? FL_WB_PROTECTED : 0));

    RCOMPLEX_SET_REAL(obj, DBL2NUM(real));
    RCOMPLEX_SET_IMAG(obj, DBL2NUM(imag));

    return (VALUE)obj;
}

#define EXTRACT_DBL(z)                          \
    struct RComplex* dat_##z;                   \
    dat_##z = ((struct RComplex*)(z));          \
    double z##_real = NUM2DBL_F(dat_##z->real); \
    double z##_imag = NUM2DBL_F(dat_##z->imag);

#define domain_error(msg) \
    rb_raise(rb_eMathDomainError, "Numerical argument is out of domain - " #msg)

static VALUE
ccmath_sqrt(VALUE obj, VALUE z)
{
    if (f_real_p(z)) {
        double dz = NUM2DBL_F(z);
        if (dz < 0.0) {
            return DBL2COMP(0.0, sqrt(fabs(dz)));
        }
        else {
            return DBL2NUM(sqrt(NUM2DBL_F(dz)));
        }
    }
    else {
        EXTRACT_DBL(z)
        double s = sqrt((hypot(z_real, z_imag) + z_real) / 2.0);
        return DBL2COMP(s, z_imag / (2 * s));
    }
}

static VALUE
ccmath_cos(VALUE obj, VALUE z)
{
    if (f_real_p(z))
        return DBL2NUM(cos(NUM2DBL_F(z)));

    EXTRACT_DBL(z);
    return DBL2COMP(cos(z_real) * cosh(z_imag), -sin(z_real) * sinh(z_imag));
}

static VALUE
ccmath_sin(VALUE obj, VALUE z)
{
    if (f_real_p(z))
        return DBL2NUM(sin(NUM2DBL_F(z)));

    EXTRACT_DBL(z);
    return DBL2COMP(sin(z_real) * cosh(z_imag), cos(z_real) * sinh(z_imag));
}

static VALUE
ccmath_tan(VALUE obj, VALUE z)
{
    if (f_real_p(z))
        return DBL2NUM(tan(NUM2DBL_F(z)));

    return rb_funcall(ccmath_sin(obj, z), '/', 1, ccmath_cos(obj, z));
}

static VALUE
ccmath_cosh(VALUE obj, VALUE z)
{
    if (f_real_p(z))
        return DBL2NUM(cosh(NUM2DBL_F(z)));

    EXTRACT_DBL(z);
    return DBL2COMP(cosh(z_real) * cos(z_imag), sinh(z_real) * sin(z_imag));
}

static VALUE
ccmath_sinh(VALUE obj, VALUE z)
{
    if (f_real_p(z))
        return DBL2NUM(sinh(NUM2DBL_F(z)));

    EXTRACT_DBL(z);
    return DBL2COMP(sinh(z_real) * cos(z_imag), cosh(z_real) * sin(z_imag));
}

static VALUE
ccmath_tanh(VALUE obj, VALUE z)
{
    if (f_real_p(z))
        return DBL2NUM(tan(NUM2DBL_F(z)));

    return rb_funcall(ccmath_sinh(obj, z), '/', 1, ccmath_cosh(obj, z));
}

static VALUE
ccmath_exp(VALUE obj, VALUE z)
{
    if (f_real_p(z))
        return DBL2NUM(exp(NUM2DBL_F(z)));

    EXTRACT_DBL(z);
    double ere = exp(z_real);
    return DBL2COMP(ere * cos(z_imag), ere * sin(z_imag));
}

static double
internal_log(VALUE x)
{
    double d;
    size_t numbits;

    if (RB_BIGNUM_TYPE_P(x) && BIGNUM_POSITIVE_P(x) && DBL_MAX_EXP <= (numbits = rb_absint_numwords(x, 1, NULL))) {
        numbits -= DBL_MANT_DIG;
        x = rb_big_rshift(x, SIZET2NUM(numbits));
    }
    else {
        numbits = 0;
    }

    d = NUM2DBL_F(x);
    /* check for domain error */
    if (d < 0.0)
        domain_error("log");
    /* check for pole error */
    if (d == 0.0)
        return -INFINITY;

    return log(d) + numbits * log(2); /* log(d * 2 ** numbits) */
}

static VALUE
ccmath_log(int argc, const VALUE* argv, VALUE obj)
{
    VALUE z, base;
    double d;

    rb_scan_args(argc, argv, "11", &z, &base);

    if (f_real_p(z)) {
        d = internal_log(z);
        if (argc == 2) {
            d /= internal_log(base);
        }
        return DBL2NUM(d);
    }
    else {
        EXTRACT_DBL(z);
        float r = hypot(z_real, z_imag);
        return DBL2COMP(log(r), m_atan2(z_imag, z_real));
    }
}

static VALUE
ccmath_asinh(VALUE obj, VALUE z)
{
    if (f_real_p(z))
        return DBL2NUM(asinh(NUM2DBL_F(z)));

    EXTRACT_DBL(z);
    VALUE s1 = DBL2COMP(1.0 + z_imag, -z_real);
    VALUE s2 = DBL2COMP(1.0 - z_imag, z_real);

    s1 = ccmath_sqrt(obj, s1);
    s2 = ccmath_sqrt(obj, s2);
    EXTRACT_DBL(s1);
    EXTRACT_DBL(s2);

    return DBL2COMP(asinh(s1_real * s2_imag - s2_real * s1_imag), atan2(z_imag, s1_real * s2_real - s1_imag * s2_imag));
}

static VALUE
ccmath_asin(VALUE obj, VALUE z)
{
    if (f_real_p(z))
        return DBL2NUM(asin(NUM2DBL_F(z)));

    EXTRACT_DBL(z);
    VALUE s = ccmath_asinh(obj, DBL2COMP(-z_imag, z_real));
    EXTRACT_DBL(s);
    return DBL2COMP(s_imag, -s_real);
}

static VALUE
ccmath_acosh(VALUE obj, VALUE z)
{
    if (f_real_p(z))
        return DBL2NUM(acosh(NUM2DBL_F(z)));

    EXTRACT_DBL(z);
    VALUE s1 = DBL2COMP(z_real - 1.0, z_imag);
    VALUE s2 = DBL2COMP(z_real + 1.0, z_imag);

    s1 = ccmath_sqrt(obj, s1);
    s2 = ccmath_sqrt(obj, s2);
    EXTRACT_DBL(s1);
    EXTRACT_DBL(s2);

    return DBL2COMP(asinh(s1_real * s2_real + s1_imag * s2_imag), 2.0 * atan2(s1_imag, s2_real));
}

static VALUE
ccmath_acos(VALUE obj, VALUE z)
{
    if (f_real_p(z))
        return DBL2NUM(acos(NUM2DBL_F(z)));

    return rb_funcall(DBL2NUM(M_PI / 2.0), '-', 1, ccmath_asin(obj, z));
}

static VALUE
ccmath_atanh(VALUE obj, VALUE z)
{
    if (f_real_p(z))
        return DBL2NUM(atanh(NUM2DBL_F(z)));

    EXTRACT_DBL(z);
    double sq_imag = z_imag * z_imag;

    return DBL2COMP(m_log1p(4.0 * z_real / ((1.0 - z_real) * (1.0 - z_real) + sq_imag)) / 4.0,
                    -m_atan2(-2.0 * z_imag, (1.0 - z_real) * (1.0 + z_real) - sq_imag) / 2.0);
}

static VALUE
ccmath_atan(VALUE obj, VALUE z)
{
    if (f_real_p(z))
        return DBL2NUM(atan(NUM2DBL_F(z)));

    EXTRACT_DBL(z);
    VALUE s = ccmath_atanh(obj, DBL2COMP(-z_imag, z_real));
    EXTRACT_DBL(s);
    return DBL2COMP(s_imag, -s_real);
}

void Init_ccmath(void)
{
    id_real_p = rb_intern("real?");
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
    rb_define_module_function(rb_mCcmath, "asinh", ccmath_asinh, 1);
    rb_define_module_function(rb_mCcmath, "asin", ccmath_asin, 1);
    rb_define_module_function(rb_mCcmath, "acosh", ccmath_acosh, 1);
    rb_define_module_function(rb_mCcmath, "acos", ccmath_acos, 1);
    rb_define_module_function(rb_mCcmath, "atanh", ccmath_atanh, 1);
    rb_define_module_function(rb_mCcmath, "atan", ccmath_atan, 1);
}
