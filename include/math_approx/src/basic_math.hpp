#pragma once

// If MATH_APPROX_XSIMD_TARGET is not defined
// the user can still use XSIMD by manually including
// it before including the math_approx header.
#if MATH_APPROX_XSIMD_TARGET
#include <xsimd/xsimd.hpp>
#endif

#if ! defined(XSIMD_HPP)
#include <cmath>
#endif

namespace math_approx
{
template <typename T>
T rsqrt (T x)
{
    // sqrtss followed by divss... this seems to measure a bit faster than the rsqrtss plus NR iteration below
    return (T) 1 / std::sqrt (x);

    // fast inverse square root (using rsqrtss hardware instruction), plus one Newton-Raphson iteration
    //    auto r = xsimd::rsqrt (xsimd::broadcast (x)).get (0);
    //    x *= r;
    //    x *= r;
    //    x += -3.0f;
    //    r *= -0.5f;
    //    return x * r;
}

#if defined(XSIMD_HPP)
template <typename T>
xsimd::batch<T> rsqrt (xsimd::batch<T> x)
{
    auto r = xsimd::rsqrt (x);
    x *= r;
    x *= r;
    x += -3.0f;
    r *= -0.5f;
    return x * r;
}
#endif
} // namespace math_approx
