#pragma once

#include "basic_math.hpp"
#include "pow_approx.hpp"

namespace math_approx
{
namespace sigmoid_detail
{
    template <typename T>
    static constexpr auto log2e = pow_detail::BaseE<scalar_of_t<T>>::log2_base;

    template <typename T>
    constexpr auto sig_poly_9 (T x)
    {
        using S = scalar_of_t<T>;

        const auto x_sq = x * x;
        const auto x_qr = x_sq * x_sq;
        const auto x_6_8 = 223345.949650f + 38992.2399197f * x_sq;
        const auto x_2_4 = 110467.772178f + 119991.618389f * x_sq;
        const auto x_2_4_6_8 = x_2_4 + x_6_8 * x_qr;
        const auto x_0_2_4_6_8 = log2e<T> + x_2_4_6_8 * x_sq;
        const auto x_7_9 = 143840.354230f + 4357.68062098f * x_sq;
        const auto x_3_5 = 76570.2195229f + 70910.1991445f * x_sq;
        const auto x_1_3_5_7 = x_3_5 + x_7_9 * x_qr;
        const auto x_1_3_5_7_9 = 1.0f + x_1_3_5_7 * x_sq;
        return std::make_pair (x_0_2_4_6_8, x * x_1_3_5_7_9);
    }

    template <typename T>
    constexpr auto sig_poly_7 (T x)
    {
        using S = scalar_of_t<T>;

        const auto x_sq = x * x;
        const auto x_qr = x_sq * x_sq;
        const auto x_4_6 = 211549.762093f + 36963.3549143f * x_sq;
        const auto x_0_2 = log2e<T> + 93728.4745225f * x_sq;
        const auto x_0_2_4_6 = x_0_2 + x_qr * x_4_6;
        const auto x_5_7 = 136227.523294f + 4149.74187650f * x_sq;
        const auto x_1_3 = 1.0f + 64967.6539703f * x_sq;
        const auto x_1_3_5_7 = x_1_3 + x_qr * x_5_7;
        return std::make_pair (x_0_2_4_6, x * x_1_3_5_7);
    }

    template <typename T>
    constexpr auto sig_poly_5 (T x)
    {
        using S = scalar_of_t<T>;

        const auto x_sq = x * x;
        const auto x_2_4 = 2225.38914940f + 425.929092006f * x_sq;
        const auto x_0_2_4 = log2e<T> + x_sq * x_2_4;
        const auto x_3_5 = 1542.35319922f + 48.3063674592f * x_sq;
        const auto x_1_3_5 = 1.0f + x_sq * x_3_5;
        return std::make_pair (x_0_2_4, x * x_1_3_5);
    }

    template <typename T>
    constexpr auto sig_poly_3 (T x)
    {
        using S = scalar_of_t<T>;

        const auto x_sq = x * x;
        const auto x_0_2 = log2e<T> + 0.276281267f * x_sq;
        const auto x_1_3 = 1.0f + 0.03138777f * x_sq;
        return std::make_pair (x_0_2, x * x_1_3);
    }
} // namespace sigmoid_detail

template <int order, typename T>
constexpr T sigmoid (T x)
{
    static_assert (order % 2 == 1 && order <= 9 && order >= 3, "Order must e an odd number within [3, 9]");

    x *= (sigmoid_detail::log2e<T> / 2.0f);

    const auto x0 = x - static_cast<int32_t> (x);

    T v1, v2 {};
    if constexpr (order == 9)
        std::tie (v1, v2) = sigmoid_detail::sig_poly_9 (x0);
    else if constexpr (order == 7)
        std::tie (v1, v2) = sigmoid_detail::sig_poly_7 (x0);
    else if constexpr (order == 5)
        std::tie (v1, v2) = sigmoid_detail::sig_poly_5 (x0); // don't need?
    else if constexpr (order == 3)
        std::tie (v1, v2) = sigmoid_detail::sig_poly_3 (x0);

    auto v3 = (v2 + v1);
    *((int32_t*) &v3) += (static_cast<int32_t> (x) << 24);

    auto v4 = v2 - v1;
    return v3 / (v3 - v4);
}

namespace sigmoid_rsqrt_detail
{
    // for polynomial derivations, see notebooks/sigmoid_approx.nb

    template <typename T>
    constexpr T sig_poly_9 (T x)
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
    constexpr T sig_poly_7 (T x)
    {
        using S = scalar_of_t<T>;
        const auto x_sq = x * x;
        const auto y_5_7 = (S) 0.000255174491559 + (S) 1.90805380557e-6 * x_sq;
        const auto y_3_5_7 = (S) 0.0208503675870 + y_5_7 * x_sq;
        const auto y_1_3_5_7 = (S) 0.5 + y_3_5_7 * x_sq;
        return x * y_1_3_5_7;
    }

    template <typename T>
    constexpr T sig_poly_5 (T x)
    {
        using S = scalar_of_t<T>;
        const auto x_sq = x * x;
        const auto y_3_5 = (S) 0.0206108521251 + (S) 0.000307906311109 * x_sq;
        const auto y_1_3_5 = (S) 0.5 + y_3_5 * x_sq;
        return x * y_1_3_5;
    }

    template <typename T>
    constexpr T sig_poly_3 (T x)
    {
        using S = scalar_of_t<T>;
        const auto x_sq = x * x;
        const auto y_1_3 = (S) 0.5 + (S) 0.0233402955195 * x_sq;
        return x * y_1_3;
    }
} // namespace sigmoid_rsqrt_detail

/**
 * Approximation of sigmoid(x) := 1 / (1 + e^-x),
 * using sigmoid(x) ≈ (1/2) p(x) / (p(x)^2 + 1) + (1/2),
 * where p(x) is an odd polynomial fit to minimize the maxinimum relative error.
 */
template <int order, typename T>
T sigmoid_rsqrt (T x)
{
    static_assert (order % 2 == 1 && order <= 9 && order >= 3, "Order must e an odd number within [3, 9]");

    T x_poly {};
    if constexpr (order == 9)
        x_poly = sigmoid_rsqrt_detail::sig_poly_9 (x);
    else if constexpr (order == 7)
        x_poly = sigmoid_rsqrt_detail::sig_poly_7 (x);
    else if constexpr (order == 5)
        x_poly = sigmoid_rsqrt_detail::sig_poly_5 (x);
    else if constexpr (order == 3)
        x_poly = sigmoid_rsqrt_detail::sig_poly_3 (x);

    using S = scalar_of_t<T>;
    return (S) 0.5 * x_poly * rsqrt (x_poly * x_poly + (S) 1) + (S) 0.5;
}

/**
 * Approximation of sigmoid(x) := 1 / (1 + e^-x),
 * using math_approx::exp (x).
 *
 * So far this has tested slower than the above approximation
 * for similar absolute error, but has better relative error
 * characteristics.
 */
template <int order, bool C1_continuous = false, typename T>
T sigmoid_exp (T x)
{
    return (T) 1 / ((T) 1 + math_approx::exp<order, C1_continuous> (-x));
}
} // namespace math_approx
