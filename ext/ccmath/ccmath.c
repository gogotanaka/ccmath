#include "ccmath.h"

VALUE rb_mCCMath;
VALUE rb_eMathDomainError;

static ID id_real_p, id_set, id_power;

#define RB_BIGNUM_TYPE_P(x) RB_TYPE_P((x), T_BIGNUM)
#define BIGNUM_POSITIVE_P(b) (FIX2LONG(rb_big_cmp((b), INT2FIX(0))) >= 0)

#define fix2dbl_without_to_f(x) (double) FIX2LONG(x)
#define big2dbl_without_to_f(x) rb_big2dbl(x)
#define int2dbl_without_to_f(x) \
    (FIXNUM_P(x) ? fix2dbl_without_to_f(x) : big2dbl_without_to_f(x))
#define rat2dbl_without_to_f(x) \
    (int2dbl_without_to_f(rb_rational_num(x)) / int2dbl_without_to_f(rb_rational_den(x)))

static inline double
num2dbl_without_to_f(VALUE num)
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
          case T_FLOAT:    return RFLOAT_VALUE(num);
          case T_BIGNUM:   return big2dbl_without_to_f(num);
          case T_RATIONAL: return rat2dbl_without_to_f(num);
        }
    }
    return RFLOAT_VALUE(rb_to_float(num));
}

#define NUM2DBL_F(x) num2dbl_without_to_f(x)

inline static int
real_p(VALUE x)
{
    if (SPECIAL_CONST_P(x)){
        if (FIXNUM_P(x)) {
            return 1;
        }
        else if (FLONUM_P(x)) {
            return 1;
        }
    }
    else {
        switch (BUILTIN_TYPE(x)) {
          case T_FLOAT:    return 1;
          case T_BIGNUM:   return 1;
          case T_RATIONAL: return 1;
          case T_COMPLEX:  return 0;
        }
    }
    return 0;
}

#define binop(n,op) \
inline static VALUE \
f_##n(VALUE x, VALUE y)\
{\
    return rb_funcall(x, (op), 1, y);\
}

binop(add, '+')
binop(div, '/')
binop(sub, '-')
binop(mul, '*')
binop(pow, id_power)

inline static VALUE
DBLS2COMP(double real, double imag)
{
    NEWOBJ_OF(obj, struct RComplex, rb_cComplex, T_COMPLEX | FL_WB_PROTECTED);

    RCOMPLEX_SET_REAL(obj, DBL2NUM(real));
    RCOMPLEX_SET_IMAG(obj, DBL2NUM(imag));

    return (VALUE)obj;
}

#define EXTRACT_DBLS_FROM_REAL(x) \
    x##_real = NUM2DBL_F(x);      \
    x##_imag = 0.0;

#define EXTRACT_DBLS_FROM_COMP(z)        \
    struct RComplex* dat_##z;            \
    dat_##z = ((struct RComplex*)(z));   \
    z##_real = NUM2DBL_F(dat_##z->real); \
    z##_imag = NUM2DBL_F(dat_##z->imag);

#define EXTRACT_DBLS(z);              \
    double z##_real, z##_imag;        \
    do {                              \
        if (real_p(z)) {              \
            EXTRACT_DBLS_FROM_REAL(z) \
        } else {                      \
            EXTRACT_DBLS_FROM_COMP(z) \
        }                             \
    } while(0)

#define domain_error(msg) \
    rb_raise(rb_eMathDomainError, "Numerical argument is out of domain - " #msg)

static VALUE
ccmath_sqrt(VALUE obj, VALUE z)
{
    EXTRACT_DBLS(z);
    if (z_imag == 0.0) {
        if (z_real < 0.0) {
            return DBLS2COMP(0.0, sqrt(fabs(z_real)));
        }
        else {
            return DBLS2COMP(sqrt(z_real), 0.0);
        }
    }
    else {
        double s = sqrt((hypot(z_real, z_imag) + z_real) / 2.0);
        return DBLS2COMP(s, z_imag / (2 * s));
    }
}

static VALUE
ccmath_exp(VALUE obj, VALUE z)
{
    EXTRACT_DBLS(z);
    if (z_imag == 0.0) return DBLS2COMP(exp(z_real), 0.0);

    double ere = exp(z_real);
    return DBLS2COMP(ere * cos(z_imag), ere * sin(z_imag));
}

void
assing_numbits(VALUE x, double *p_d, size_t *p_numbits) {
    if (RB_BIGNUM_TYPE_P(x) && BIGNUM_POSITIVE_P(x) && DBL_MAX_EXP <= (*p_numbits = rb_absint_numwords(x, 1, NULL))) {
        *p_numbits -= DBL_MANT_DIG;
        x = rb_big_rshift(x, SIZET2NUM(*p_numbits));
    }
    else {
        *p_numbits = 0;
    }

    *p_d = NUM2DBL_F(x);
}

static double
internal_log(VALUE x)
{
    double d;
    size_t numbits;

    assing_numbits(x, &d, &numbits);

    if (d < 0.0) domain_error("log");
    if (d == 0.0) return -INFINITY;

    return log(d) + numbits * log(2); /* log(d * 2 ** numbits) */
}

static VALUE
ccmath_log(int argc, const VALUE* argv, VALUE obj)
{
    VALUE z, base;
    rb_scan_args(argc, argv, "11", &z, &base);

    EXTRACT_DBLS(z);
    float r = hypot(z_real, z_imag);
    if (argc == 2) {
        if (!rb_respond_to(base, id_real_p)) rb_raise(rb_eTypeError, "Numeric Number required");
        EXTRACT_DBLS(base);
        if (base_imag != 0.0) domain_error("log");
        if (base_real > 0.0) {
            double ln_base = log(base_real);
            return DBLS2COMP(log(r) / ln_base, m_atan2(z_imag, z_real) / ln_base);
        }
        else {
            VALUE ln_base = DBLS2COMP(log(fabs(base_real)), M_PI);
            return f_div(DBLS2COMP(log(r), m_atan2(z_imag, z_real)), ln_base);
        }
    }
    else {
        return DBLS2COMP(log(r), m_atan2(z_imag, z_real));
    }
}

static VALUE
ccmath_log2(VALUE obj, VALUE z)
{
    EXTRACT_DBLS(z);
    float r = hypot(z_real, z_imag);
    return DBLS2COMP(log(r) / M_LN2, m_atan2(z_imag, z_real) / M_LN2);
}

static VALUE
ccmath_log10(VALUE obj, VALUE z)
{
    EXTRACT_DBLS(z);
    float r = hypot(z_real, z_imag);
    return DBLS2COMP(log(r) / M_LN10, m_atan2(z_imag, z_real) / M_LN10);
}

static VALUE
ccmath_cos(VALUE obj, VALUE z)
{
    EXTRACT_DBLS(z);
    if (z_real == 0.0) return DBLS2COMP(cosh(z_imag), 0.0);
    if (z_imag == 0.0) return DBLS2COMP(cos(z_real), 0.0);
    return DBLS2COMP(cos(z_real) * cosh(z_imag), -sin(z_real) * sinh(z_imag));
}

static VALUE
ccmath_sin(VALUE obj, VALUE z)
{
    EXTRACT_DBLS(z);
    if (z_real == 0.0) return DBLS2COMP(0.0, sinh(z_imag));
    if (z_imag == 0.0) return DBLS2COMP(sin(z_real), 0.0);
    return DBLS2COMP(sin(z_real) * cosh(z_imag), cos(z_real) * sinh(z_imag));
}

static VALUE
ccmath_tan(VALUE obj, VALUE z)
{
    return f_div(ccmath_sin(obj, z), ccmath_cos(obj, z));
}

static VALUE
ccmath_cosh(VALUE obj, VALUE z)
{
    EXTRACT_DBLS(z);
    if (z_real == 0.0) return DBLS2COMP(cos(z_imag), 0.0);
    if (z_imag == 0.0) return DBLS2COMP(cosh(z_real), 0.0);
    return DBLS2COMP(cosh(z_real) * cos(z_imag), sinh(z_real) * sin(z_imag));
}

static VALUE
ccmath_sinh(VALUE obj, VALUE z)
{
    EXTRACT_DBLS(z);
    if (z_real == 0.0) return DBLS2COMP(0.0, sin(z_imag));
    if (z_imag == 0.0) return DBLS2COMP(sinh(z_real), 0.0);
    return DBLS2COMP(sinh(z_real) * cos(z_imag), cosh(z_real) * sin(z_imag));
}

static VALUE
ccmath_tanh(VALUE obj, VALUE z)
{
    return f_div(ccmath_sinh(obj, z), ccmath_cosh(obj, z));
}

static VALUE
ccmath_asinh(VALUE obj, VALUE z)
{
    EXTRACT_DBLS(z);

    if (z_imag == 0.0) return DBLS2COMP(asinh(z_real), 0.0);

    VALUE s1 = DBLS2COMP(1.0 + z_imag, -z_real);
    VALUE s2 = DBLS2COMP(1.0 - z_imag, z_real);

    s1 = ccmath_sqrt(obj, s1);
    s2 = ccmath_sqrt(obj, s2);
    EXTRACT_DBLS(s1);
    EXTRACT_DBLS(s2);

    return DBLS2COMP(asinh(s1_real * s2_imag - s2_real * s1_imag), atan2(z_imag, s1_real * s2_real - s1_imag * s2_imag));
}

static VALUE
ccmath_asin(VALUE obj, VALUE z)
{
    EXTRACT_DBLS(z);

    if (z_imag == 0.0) return DBLS2COMP(asin(z_real), 0.0);

    VALUE s = ccmath_asinh(obj, DBLS2COMP(-z_imag, z_real));
    EXTRACT_DBLS(s);
    return DBLS2COMP(s_imag, -s_real);
}

static VALUE
ccmath_acosh(VALUE obj, VALUE z)
{
    EXTRACT_DBLS(z);

    if (z_imag == 0.0) return DBLS2COMP(acosh(z_real), 0.0);

    VALUE s1 = DBLS2COMP(z_real - 1.0, z_imag);
    VALUE s2 = DBLS2COMP(z_real + 1.0, z_imag);

    s1 = ccmath_sqrt(obj, s1);
    s2 = ccmath_sqrt(obj, s2);
    EXTRACT_DBLS(s1);
    EXTRACT_DBLS(s2);

    return DBLS2COMP(asinh(s1_real * s2_real + s1_imag * s2_imag), 2.0 * atan2(s1_imag, s2_real));
}

static VALUE
ccmath_acos(VALUE obj, VALUE z)
{
    EXTRACT_DBLS(z);

    if (z_imag == 0.0) return DBLS2COMP(acos(z_real), 0.0);

    return f_sub(DBL2NUM(M_PI / 2.0), ccmath_asin(obj, z));
}

static VALUE
ccmath_atanh(VALUE obj, VALUE z)
{
    EXTRACT_DBLS(z);

    if (z_imag == 0.0) return DBLS2COMP(atanh(z_real), 0.0);

    double sq_imag = z_imag * z_imag;

    return DBLS2COMP(m_log1p(4.0 * z_real / ((1.0 - z_real) * (1.0 - z_real) + sq_imag)) / 4.0,
                    -m_atan2(-2.0 * z_imag, (1.0 - z_real) * (1.0 + z_real) - sq_imag) / 2.0);
}

static VALUE
ccmath_atan(VALUE obj, VALUE z)
{
    EXTRACT_DBLS(z);

    if (z_imag == 0.0) return DBLS2COMP(atan(z_real), 0.0);

    VALUE s = ccmath_atanh(obj, DBLS2COMP(-z_imag, z_real));
    EXTRACT_DBLS(s);
    return DBLS2COMP(s_imag, -s_real);
}

static VALUE
ccmath_gamma(VALUE obj, VALUE z)
{
    EXTRACT_DBLS(z);
    if (z_real < 0.5) {
        VALUE s1, s2;
        s1 = ccmath_gamma(obj, f_sub(DBL2NUM(1), z));
        s2 = ccmath_sin(obj, f_mul(DBL2NUM(M_PI), z));
        return f_div(DBL2NUM(M_PI), f_mul(s1, s2));
    }
    else {
        static const double lanczos_coef[] = {
            0.99999999999980993,
            676.5203681218851,
            -1259.1392167224028,
            771.32342877765313,
            -176.61502916214059,
            12.507343278686905,
            -0.13857109526572012,
            9.9843695780195716e-6,
            1.5056327351493116e-7
        };
        int i;
        double g, s, x_real, x_imag;

        g = 7.0;
        z_real -= 1;
        x_real = lanczos_coef[0];
        x_imag = 0.0;
        for(i=1; i<g+2; i++) {
            s = lanczos_coef[i] / ((z_real+i) * (z_real+i) + (z_imag * z_imag));
            x_real += s * (z_real+i);
            x_imag -= s * z_imag;
        }
        VALUE x = DBLS2COMP(x_real, x_imag);
        VALUE sqrt_2_pi = DBL2NUM(sqrt(2 * M_PI));
        VALUE t = DBLS2COMP(z_real + g + 0.5, z_imag);
        return f_mul(sqrt_2_pi,
               f_mul(f_pow(t, DBLS2COMP(z_real+0.5, z_imag)),
               f_mul(f_div(DBL2NUM(1), ccmath_exp(obj,t)), x)));

    }
}

// static VALUE
// ccmath_define_set(VALUE obj, VALUE str)
// {
//     rb_ivar_set(obj, id_set, str);
//     return obj;
// }

void Init_ccmath(void)
{
    id_real_p = rb_intern("real?");
    id_power = rb_intern("**");
    id_set = rb_intern("set");
    rb_mCCMath = rb_define_module("CCMath");

    rb_eMathDomainError = rb_define_class_under(rb_mCCMath, "DomainError", rb_eStandardError);
    #ifdef M_PI
        rb_define_const(rb_mCCMath, "PI", DBL2NUM(M_PI));
    #else
        rb_define_const(rb_mCCMath, "PI", DBL2NUM(atan(1.0)*4.0));
    #endif

    #ifdef M_E
        rb_define_const(rb_mCCMath, "E", DBL2NUM(M_E));
    #else
        rb_define_const(rb_mCCMath, "E", DBL2NUM(exp(1.0)));
    #endif

    rb_define_module_function(rb_mCCMath, "sqrt", ccmath_sqrt, 1);
    rb_define_module_function(rb_mCCMath, "exp", ccmath_exp, 1);
    rb_define_module_function(rb_mCCMath, "log", ccmath_log, -1);
    rb_define_module_function(rb_mCCMath, "log2", ccmath_log2, 1);
    rb_define_module_function(rb_mCCMath, "log10", ccmath_log10, 1);
    rb_define_module_function(rb_mCCMath, "cos", ccmath_cos, 1);
    rb_define_module_function(rb_mCCMath, "sin", ccmath_sin, 1);
    rb_define_module_function(rb_mCCMath, "tan", ccmath_tan, 1);
    rb_define_module_function(rb_mCCMath, "cosh", ccmath_cosh, 1);
    rb_define_module_function(rb_mCCMath, "sinh", ccmath_sinh, 1);
    rb_define_module_function(rb_mCCMath, "tanh", ccmath_tanh, 1);
    rb_define_module_function(rb_mCCMath, "asinh", ccmath_asinh, 1);
    rb_define_module_function(rb_mCCMath, "asin", ccmath_asin, 1);
    rb_define_module_function(rb_mCCMath, "acosh", ccmath_acosh, 1);
    rb_define_module_function(rb_mCCMath, "acos", ccmath_acos, 1);
    rb_define_module_function(rb_mCCMath, "atanh", ccmath_atanh, 1);
    rb_define_module_function(rb_mCCMath, "atan", ccmath_atan, 1);
    rb_define_module_function(rb_mCCMath, "gamma", ccmath_gamma, 1);
}
