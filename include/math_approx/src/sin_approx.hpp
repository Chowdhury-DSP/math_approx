#pragma once

#include "basic_math.hpp"

namespace math_approx
{
namespace sin_detail
{
    template <typename T>
    T truncate (T x)
    {
        return static_cast<T> (static_cast<int> (x));
    }

#if defined(XSIMD_HPP)
    template <typename T>
    xsimd::batch<T> truncate (xsimd::batch<T> x)
    {
        return xsimd::to_float (xsimd::to_int (x));
    }
#endif

    /** Fast method to wrap a value into the range [-pi, pi] */
    template <typename T>
    T fast_mod_mpi_pi (T x)
    {
        using S = scalar_of_t<T>;
        static constexpr auto pi = static_cast<S> (M_PI);
        static constexpr auto two_pi = static_cast<S> (2.0 * M_PI);
        static constexpr auto recip_two_pi = static_cast<S> (1) / two_pi;

        x += pi;
        const auto mod = x - two_pi * truncate (x * recip_two_pi);
        return select (x >= (T) 0, mod, mod + two_pi) - pi;
    }

    template <typename T>
    T sin_poly_9 (T x, T x_sq)
    {
        using S = scalar_of_t<T>;
        const auto x_7_9 = (S) -2.49397084313e-6 + (S) 2.00382818811e-8 * x_sq;
        const auto x_5_7_9 = (S) 0.000173405228576 + x_7_9 * x_sq;
        const auto x_3_5_7_9 = (S) -0.00662075636230 + x_5_7_9 * x_sq;
        const auto x_1_3_5_7_9 = (S) 0.101321159036 + x_3_5_7_9 * x_sq;
        return x * x_1_3_5_7_9;
    }

    template <typename T>
    T sin_poly_7 (T x, T x_sq)
    {
        using S = scalar_of_t<T>;
        const auto x_5_7 = (S) 0.000170965340046 + (S) -2.09843101304e-6 * x_sq;
        const auto x_3_5_7 = (S) -0.00661594021539 + x_5_7 * x_sq;
        const auto x_1_3_5_7 = (S) 0.101319673615 + x_3_5_7 * x_sq;
        return x * x_1_3_5_7;
    }

    template <typename T>
    T sin_poly_5 (T x, T x_sq)
    {
        using S = scalar_of_t<T>;
        const auto x_3_5 = (S) -0.00650096169550 + (S) 0.000139899314103 * x_sq;
        const auto x_1_3_5 = (S) 0.101256629587 + x_3_5 * x_sq;
        return x * x_1_3_5;
    }
} // namespace sin_detail

template <int order, typename T>
T sin_mpi_pi (T x)
{
    static_assert (order % 2 == 1 && order <= 9 && order >= 5, "Order must e an odd number within [5, 9]");

    using S = scalar_of_t<T>;
    static constexpr auto pi = static_cast<S> (M_PI);
    static constexpr auto pi_sq = pi * pi;
    const auto x_sq = x * x;

    T x_poly {};
    if constexpr (order == 9)
        x_poly = sin_detail::sin_poly_9 (x, x_sq);
    else if constexpr (order == 7)
        x_poly = sin_detail::sin_poly_7 (x, x_sq);
    else if constexpr (order == 5)
        x_poly = sin_detail::sin_poly_5 (x, x_sq);

    return (pi_sq - x_sq) * x_poly;
}

template <int order, typename T>
T sin (T x)
{
    return sin_mpi_pi<order, T> (sin_detail::fast_mod_mpi_pi (x));
}

template <int order, typename T>
T cos_mpi_pi (T x)
{
    static_assert (order % 2 == 1 && order <= 9 && order >= 5, "Order must e an odd number within [5, 9]");

    using S = scalar_of_t<T>;
    static constexpr auto pi = static_cast<S> (M_PI);
    static constexpr auto pi_o_2 = pi * (S) 0.5;;

    const auto hpmx = (x > (S) 0 ? (S) 1 : (S) -1) * pi_o_2 - x;
    const auto thpmx = (x > (S) 0 ? (S) 3 : (S) -3) * pi_o_2 - x;
    const auto nhpmx = (x > (S) 0 ? (S) -1 : (S) 1) * pi_o_2 - x;
    const auto hpmx_sq = hpmx * hpmx;

    T x_poly {};
    if constexpr (order == 9)
        x_poly = sin_detail::sin_poly_9 (hpmx, hpmx_sq);
    else if constexpr (order == 7)
        x_poly = sin_detail::sin_poly_7 (hpmx, hpmx_sq);
    else if constexpr (order == 5)
        x_poly = sin_detail::sin_poly_5 (hpmx, hpmx_sq);

    return thpmx * nhpmx * (x > (S) 0 ? (S) -1 : (S) 1) * x_poly;
}

template <int order, typename T>
T cos (T x)
{
    return cos_mpi_pi<order, T> (sin_detail::fast_mod_mpi_pi (x));
}
} // namespace math_approx
