#pragma once

#include "basic_math.hpp"

namespace math_approx
{
namespace tanh_detail
{
    // These polynomial fits were generated from: https://www.wolframcloud.com/obj/chowdsp/Published/tanh_approx.nb

    template <typename T>
    constexpr T tanh_poly_11 (T x)
    {
        using S = scalar_of_t<T>;
        const auto x_sq = x * x;
        const auto y_9_11 = (S) 2.63661358122e-6 + (S) 3.33765558362e-8 * x_sq;
        const auto y_7_9_11 = (S) 0.000199027336899 + y_9_11 * x_sq;
        const auto y_5_7_9_11 = (S) 0.00833223857843 + y_7_9_11 * x_sq;
        const auto y_3_5_7_9_11 = (S) 0.166667159320 + y_5_7_9_11 * x_sq;
        const auto y_1_3_5_7_9_11 = (S) 1 + y_3_5_7_9_11 * x_sq;
        return x * y_1_3_5_7_9_11;
    }

    template <typename T>
    constexpr T tanh_poly_9 (T x)
    {
        using S = scalar_of_t<T>;
        const auto x_sq = x * x;
        const auto y_7_9 = (S) 0.000192218110330 + (S) 3.54808622170e-6 * x_sq;
        const auto y_5_7_9 = (S) 0.00834777254865 + y_7_9 * x_sq;
        const auto y_3_5_7_9 = (S) 0.166658873283 + y_5_7_9 * x_sq;
        const auto y_1_3_5_7_9 = (S) 1 + y_3_5_7_9 * x_sq;
        return x * y_1_3_5_7_9;
    }

    template <typename T>
    constexpr T tanh_poly_7 (T x)
    {
        using S = scalar_of_t<T>;
        const auto x_sq = x * x;
        const auto y_5_7 = (S) 0.00818199927912 + (S) 0.000243153287690 * x_sq;
        const auto y_3_5_7 = (S) 0.166769941467 + y_5_7 * x_sq;
        const auto y_1_3_5_7 = (S) 1 + y_3_5_7 * x_sq;
        return x * y_1_3_5_7;
    }

    template <typename T>
    constexpr T tanh_poly_5 (T x)
    {
        using S = scalar_of_t<T>;
        const auto x_sq = x * x;
        const auto y_3_5 = (S) 0.165326984031 + (S) 0.00970240200826 * x_sq;
        const auto y_1_3_5 = (S) 1 + y_3_5 * x_sq;
        return x * y_1_3_5;
    }

    template <typename T>
    constexpr T tanh_poly_3 (T x)
    {
        using S = scalar_of_t<T>;
        const auto x_sq = x * x;
        const auto y_1_3 = (S) 1 + (S) 0.183428244899 * x_sq;
        return x * y_1_3;
    }
} // namespace tanh_detail

template <int order, typename T>
T tanh (T x)
{
    static_assert (order % 2 == 1 && order <= 11 && order >= 3, "Order must e an odd number within [3, 11]");

    T x_poly {};
    if constexpr (order == 11)
        x_poly = tanh_detail::tanh_poly_11 (x);
    else if constexpr (order == 9)
        x_poly = tanh_detail::tanh_poly_9 (x);
    else if constexpr (order == 7)
        x_poly = tanh_detail::tanh_poly_7 (x);
    else if constexpr (order == 5)
        x_poly = tanh_detail::tanh_poly_5 (x);
    else if constexpr (order == 3)
        x_poly = tanh_detail::tanh_poly_3 (x);

    using S = scalar_of_t<T>;
    return x_poly * rsqrt (x_poly * x_poly + (S) 1);
}
} // namespace math_approx
