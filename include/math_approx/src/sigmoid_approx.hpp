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
    constexpr auto sig_poly_7 (T x)
    {
        using S = scalar_of_t<T>;

        const auto x_sq = x * x;
        const auto x_qr = x_sq * x_sq;
        const auto x_4_6 = (S) 2.39795819238f + (S) 0.413088705007f * x_sq;
        const auto x_0_2 = (S) 1.44269561352f + (S) 1.48937630203f * x_sq;
        const auto x_0_2_4_6 = x_0_2 + x_qr * x_4_6;
        const auto x_5_7 = (S) 1.52750589429f + (S) 0.0461492521138f * x_sq;
        const auto x_1_3 = (S) 1 + (S) 0.87221658964f * x_sq;
        const auto x_1_3_5_7 = x_1_3 + x_qr * x_5_7;
        return std::make_pair (x_0_2_4_6, x * x_1_3_5_7);
    }

    template <typename T>
    constexpr auto sig_poly_5 (T x)
    {
        using S = scalar_of_t<T>;

        const auto x_sq = x * x;
        const auto x_2_4 = (S) 2.69262851766f + (S) 0.461242914271f * x_sq;
        const auto x_0_2_4 = (S) 1.44269222803f + x_sq * x_2_4;
        const auto x_3_5 = (S) 1.70620311079f + (S) 0.051736417778f * x_sq;
        const auto x_1_3_5 = (S) 1 + x_sq * x_3_5;
        return std::make_pair (x_0_2_4, x * x_1_3_5);
    }

    template <typename T>
    constexpr auto sig_poly_3 (T x)
    {
        using S = scalar_of_t<T>;

        const auto x_sq = x * x;
        const auto x_0_2 = (S) 1.44270650969f + (S) 0.276063954394f * x_sq;
        const auto x_1_3 = (S) 1 + (S) 0.0312633523774f * x_sq;
        return std::make_pair (x_0_2, x * x_1_3);
    }

    template <typename T>
    constexpr auto sig_poly_2 (T x)
    {
        using S = scalar_of_t<T>;

        const auto x_sq = x * x;
        const auto x_0_2 = (S) 1.44444545577f + (S) 0.222901477860f * x_sq;
        return std::make_pair (x_0_2, x);
    }

    template <int order, typename T>
    constexpr auto sig_poly (T x)
    {
        static_assert ((order == 2 || order % 2 == 1) && order <= 7 && order >= 2,
                       "Order must e an odd number within [3, 7]");

        T v1, v2 {};
        if constexpr (order == 7)
            std::tie (v1, v2) = sigmoid_detail::sig_poly_7 (x);
        else if constexpr (order == 5)
            std::tie (v1, v2) = sigmoid_detail::sig_poly_5 (x);
        else if constexpr (order == 3)
            std::tie (v1, v2) = sigmoid_detail::sig_poly_3 (x);
        else if constexpr (order == 2)
            std::tie (v1, v2) = sigmoid_detail::sig_poly_2 (x);

        return std::make_pair (v1, v2);
    }

    template <int order>
    constexpr auto sigmoid_kernel (float x)
    {
        const auto x0 = x - static_cast<int32_t> (x);

        const auto [v1, v2] = sigmoid_detail::sig_poly<order> (x0);

        auto v3 = (v2 + v1);
        *((int32_t*) &v3) += (static_cast<int32_t> (x) << 24);
        auto v4 = v2 - v1;

        return std::make_pair (v3, v4);
    }

    template <int order>
    constexpr auto sigmoid_kernel (double x)
    {
        const auto x0 = x - static_cast<int64_t> (x);

        const auto [v1, v2] = sigmoid_detail::sig_poly<order> (x0);

        auto v3 = (v2 + v1);
        *((int64_t*) &v3) += (static_cast<int64_t> (x) << 53);
        auto v4 = v2 - v1;

        return std::make_pair (v3, v4);
    }
} // namespace sigmoid_detail

template <int order, typename T>
constexpr T sigmoid (T x)
{
    using S = scalar_of_t<T>;
    x *= (sigmoid_detail::log2e<T> / (S) 2);
    const auto [v3, v4] = sigmoid_detail::sigmoid_kernel<order> (x);
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
