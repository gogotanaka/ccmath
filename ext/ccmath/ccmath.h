#ifndef CCMATH_H
#define CCMATH_H 1

#include "ruby.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifndef DBL_MANT_DIG
#define DBL_MAX_EXP 1024
#endif

#ifndef DBL_MANT_DIG
#define DBL_MANT_DIG 53
#endif

#ifndef DBL_EPSILON
#define DBL_EPSILON 2.2204460492503131e-16
#endif

#define LARGE_DBL (DBL_MAX/4.)

struct RComplex {
    struct RBasic basic;
    const VALUE real;
    const VALUE imag;
};

#define RCOMPLEX(obj) (R_CAST(RComplex)(obj))
#define RCOMPLEX_SET_REAL(cmp, r) RB_OBJ_WRITE((cmp), &((struct RComplex *)(cmp))->real,(r))
#define RCOMPLEX_SET_IMAG(cmp, i) RB_OBJ_WRITE((cmp), &((struct RComplex *)(cmp))->imag,(i))

static inline double
m_atan2(double y, double x)
{
    if (x == 0.0 && y == 0.0) {
        if (!signbit(x))
            return y;
        if (!signbit(y))
            return M_PI;
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

static inline double
m_log1p(double x)
{
#ifdef HAVE_LOG1P
    if (x == 0.0)
        return x;

    return log1p(x);
#else
    if (fabs(x) < DBL_EPSILON/2.0)
        return x;

    return log(1.0+x);
#endif /* ifdef HAVE_LOG1P */
}

#endif /* CCMATH_H */
