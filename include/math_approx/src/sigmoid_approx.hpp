#pragma once

#include "basic_math.hpp"

namespace math_approx
{
namespace sigmoid_detail
{
    // These polynomial fits were generated from: https://www.wolframcloud.com/obj/chowdsp/Published/sigmoid_approx.nb

    template <typename T>
    T sig_poly_9 (T x)
    {
        using S = scalar_of_t<T>;
        const auto x_sq = x * x;
        const auto y_7_9 = (S) 1.50024356624e-6 + (S) 6.92468584642e-9 * x_sq;
        const auto y_5_7_9 = (S) 0.000260923534301 + y_7_9 * x_sq;
        const auto y_3_5_7_9 = (S) 0.0208320229264 + y_5_7_9 * x_sq;
        const auto y_1_3_5_7_9 = (S) 0.5 + y_3_5_7_9 * x_sq;
        return x * y_1_3_5_7_9;
    }

    template <typename T>
    T sig_poly_7 (T x)
    {
        using S = scalar_of_t<T>;
        const auto x_sq = x * x;
        const auto y_5_7 = (S) 0.000255174491559 + (S) 1.90805380557e-6 * x_sq;
        const auto y_3_5_7 = (S) 0.0208503675870 + y_5_7 * x_sq;
        const auto y_1_3_5_7 = (S) 0.5 + y_3_5_7 * x_sq;
        return x * y_1_3_5_7;
    }

    template <typename T>
    T sig_poly_5 (T x)
    {
        using S = scalar_of_t<T>;
        const auto x_sq = x * x;
        const auto y_3_5 = (S) 0.0206108521251 + (S) 0.000307906311109 * x_sq;
        const auto y_1_3_5 = (S) 0.5 + y_3_5 * x_sq;
        return x * y_1_3_5;
    }

    template <typename T>
    T sig_poly_3 (T x)
    {
        using S = scalar_of_t<T>;
        const auto x_sq = x * x;
        const auto y_1_3 = (S) 0.5 + (S) 0.0233402955195 * x_sq;
        return x * y_1_3;
    }
} // namespace sigmoid_detail

template <int order, typename T>
T sigmoid (T x)
{
    static_assert (order % 2 == 1 && order <= 9 && order >= 3, "Order must e an odd number within [3, 9]");

    T x_poly {};
    if constexpr (order == 9)
        x_poly = sigmoid_detail::sig_poly_9 (x);
    else if constexpr (order == 7)
        x_poly = sigmoid_detail::sig_poly_7 (x);
    else if constexpr (order == 5)
        x_poly = sigmoid_detail::sig_poly_5 (x);
    else if constexpr (order == 3)
        x_poly = sigmoid_detail::sig_poly_3 (x);

    using S = scalar_of_t<T>;
    return (S) 0.5 * x_poly * rsqrt (x_poly * x_poly + (S) 1) + (S) 0.5;
}

// So far this has tested slower than the above approx (for equivalent error),
// but maybe it will be useful for someone!
// template <int order, typename T>
// T sigmoid_exp (T x)
// {
//     return (T) 1 / ((T) 1 + math_approx::exp<order> (-x));
// }
} // namespace math_approx
