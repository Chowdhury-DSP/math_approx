#pragma once

#include "basic_math.hpp"
#include "pow_approx.hpp"

namespace math_approx
{
namespace log_detail
{
    /** approximation for log2(x), optimized on the range [1, 2] */
    template <typename T, int order>
    constexpr T log2_approx (T x)
    {
        static_assert (order >= 3 && order <= 6);
        using S = scalar_of_t<T>;

        const auto x_sq = x * x;
        if constexpr (order == 3)
        {
            const auto x_2_3 = (S) -1.05974531422 + (S) 0.159220010975 * x;
            const auto x_0_1 = (S) -2.16417056258 + (S) 3.06469586582 * x;
            return x_0_1 + x_2_3 * x_sq;
        }
        else if constexpr (order == 4)
        {
            const auto x_3_4 = (S) 0.649709537672 + (S) -0.0821303550902 * x;
            const auto x_1_2 = (S) 4.08637809379 + (S) -2.13412984371 * x;
            const auto x_1_2_3_4 = x_1_2 + x_3_4 * x_sq;
            return (S) -2.51982743265 + x_1_2_3_4 * x;
        }
        else if constexpr (order == 5)
        {
            const auto x_4_5 = (S) -0.419319345483 + (S) 0.0451488402558 * x;
            const auto x_2_3 = (S) -3.56885211615 + (S) 1.64139451414 * x;
            const auto x_0_1 = (S) -2.80534277658 + (S) 5.10697088382 * x;
            const auto x_2_3_4_5 = x_2_3 + x_4_5 * x_sq;
            return x_0_1 + x_2_3_4_5 * x_sq;
        }
        else if constexpr (order == 6)
        {
            const auto x_5_6 = (S) 0.276834061071 + (S) -0.0258400886535 * x;
            const auto x_3_4 = (S) 3.30388341157 + (S) -1.27446900713 * x;
            const auto x_1_2 = (S) 6.12708086513 + (S) -5.36371998242 * x;
            const auto x_3_4_5_6 = x_3_4 + x_5_6 * x_sq;
            const auto x_1_2_3_4_5_6 = x_1_2 + x_3_4_5_6 * x_sq;
            return (S) -3.04376925958 + x_1_2_3_4_5_6 * x;
        }
        else
        {
            return {};
        }
    }
}

/** approximation for log(Base, x) (32-bit) */
template <typename Base, int order>
float log (float x)
{
    const auto vi = reinterpret_cast<int32_t&> (x); // NOSONAR
    const auto ex = vi & 0x7f800000;
    const auto e = (ex >> 23) - 127;
    const auto vfi = (vi - ex) | 0x3f800000;
    const auto vf = reinterpret_cast<const float&> (vfi); // NOSONAR

    static constexpr auto log2_base_r = 1.0f / Base::log2_base;
    return log2_base_r * ((float) e + log_detail::log2_approx<float, order> (vf));
}

/** approximation for log(x) (64-bit) */
template <typename Base, int order>
double log (double x)
{
    const auto vi = reinterpret_cast<int64_t&> (x); // NOSONAR
    const auto ex = vi & 0x7ff0000000000000;
    const auto e = (ex >> 52) - 1023;
    const auto vfi = (vi - ex) | 0x3ff0000000000000;
    const auto vf = reinterpret_cast<const double&> (vfi); // NOSONAR

    static constexpr auto log2_base_r = 1.0 / Base::log2_base;
    return log2_base_r * ((double) e + log_detail::log2_approx<double, order> (vf));
}

template <int order, typename T>
T log (T x)
{
    return log<pow_detail::BaseE<scalar_of_t<T>>, order> (x);
}

template <int order, typename T>
T log2 (T x)
{
    return log<pow_detail::Base2<scalar_of_t<T>>, order> (x);
}

template <int order, typename T>
T log10 (T x)
{
    return log<pow_detail::Base10<scalar_of_t<T>>, order> (x);
}
}
