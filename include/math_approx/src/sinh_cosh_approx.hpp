#pragma once

#include "pow_approx.hpp"

namespace math_approx
{
// ref: https://en.wikipedia.org/wiki/Hyperbolic_functions#Definitions
// sinh = (e^(2x) - 1) / (2e^x), cosh = (e^(2x) + 1) / (2e^x)
// let B = e^x, then sinh = (B^2 - 1) / (2B), cosh = (B^2 + 1) / (2B)
// simplifying, we get: sinh = 0.5 (B - 1/B), cosh = 0.5 (B + 1/B)

/** Approximation of sinh(x), using exp(x) internally */
template <int order, typename T>
constexpr T sinh (T x)
{
    using S = scalar_of_t<T>;
    auto B = exp<order> (x);
    auto Br = (S) 0.5 / B;
    B *= (S) 0.5;
    return B - Br;
}

/** Approximation of cosh(x), using exp(x) internally */
template <int order, typename T>
constexpr T cosh (T x)
{
    using S = scalar_of_t<T>;
    auto B = exp<order> (x);
    auto Br = (S) 0.5 / B;
    B *= (S) 0.5;
    return B + Br;
}

/**
 * Simultaneous pproximation of sinh(x) and cosh(x),
 * using exp(x) internally.
 *
 * For more information see the comments above.
 */
template <int order, typename T>
constexpr auto sinh_cosh (T x)
{
    using S = scalar_of_t<T>;
    auto B = exp<order> (x);
    auto Br = (S) 0.5 / B;
    B *= (S) 0.5;

    auto sinh = B - Br;
    auto cosh = B + Br;

    return std::make_pair (sinh, cosh);
}
} // namespace math_approx
