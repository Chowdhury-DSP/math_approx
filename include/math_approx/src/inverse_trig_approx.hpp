#pragma once

#include "basic_math.hpp"

namespace math_approx
{
namespace asin_detail
{
    template <int order, typename T>
    constexpr T asin_kernel (T x)
    {
        using S = scalar_of_t<T>;
        static_assert (order >= 1 && order <= 4);

        if constexpr (order == 1)
        {
            return (S) 0.16443531037029196495 + x * (S) 0.097419577664394046979;
        }
        if constexpr (order == 2)
        {
            return (S) 0.16687742065041710759 + x * ((S) 0.070980446338571381859 + x * (S) 0.066682760821292624831);
        }
        if constexpr (order == 3)
        {
            return (S) 0.16665080061757006624 + x * ((S) 0.075508850204912977833 + x * ((S) 0.039376231206556484843 + x * (S) 0.051275338699694958389));
        }
        if constexpr (order == 4)
        {
            return (S) 0.16666803275183153521 + x * ((S) 0.074936964020844071266 + x * ((S) 0.045640288439217274741 + x * ((S) 0.023435504410713306478 + x * (S) 0.043323710842752508055)));
        }

        return {};
    }
} // namespace asin_detail

template <int order, typename T>
T asin (T x)
{
    using S = scalar_of_t<T>;

    using std::abs, std::sqrt;
#if defined(XSIMD_HPP)
    using xsimd::abs, xsimd::sqrt;
#endif

    const auto abs_x = abs (x);

    const auto reflect = abs_x > (S) 0.5;
    auto z0 = select (reflect, (S) 0.5 * ((S) 1 - abs_x), abs_x * abs_x);

    auto x2 = select (reflect, sqrt (z0), abs_x);
    auto z1 = asin_detail::asin_kernel<order> (z0);

    auto z2 = z1 = z1 * (z0 * x2) + x2;
    auto res = select (reflect, (S) M_PI_2 - (z2 + z2), z2);
    return select (x > (S) 0, res, -res);
}
} // namespace math_approx
