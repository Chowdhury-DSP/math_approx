#pragma once

#include "basic_math.hpp"

namespace math_approx
{
namespace asin_detail
{
    template <int order, typename T>
    constexpr T asin_0_rsqrt2 (T x)
    {
        using S = scalar_of_t<T>;
        static_assert (order >= 3 && order <= 11);

        const auto x_sq = x * x;
        if constexpr (order == 3)
        {
            const auto y_2_3 = (S) -0.0536922932754174 + (S) 0.297373838424192 * x;
            return x + x_sq * y_2_3;
        }
        if constexpr (order == 4)
        {
            const auto y_3_4 = (S) -0.00535062837316264 + (S) 0.257252341545375 * x;
            const auto y_2_3_4 = (S) 0.0317400592553864 + x * y_3_4;
            return x + x_sq * y_2_3_4;
        }
        if constexpr (order == 5)
        {
            const auto y_4_5 = (S) -0.304640601352515 + (S) 0.353208342056560 * x;
            const auto y_2_3 = (S) -0.0132122795426018 + (S) 0.278935718011026 * x;

            const auto y_2_3_4_5 = y_2_3 + x_sq * y_4_5;
            return x + x_sq * y_2_3_4_5;
        }
        if constexpr (order == 6)
        {
            const auto y_5_6 = (S) -0.516068582317285 + (S) 0.437544978265334 * x;
            const auto y_3_4 = (S) 0.0946375652126262 + (S) 0.313911974469437 * x;

            const auto y_3_4_5_6 = y_3_4 + x_sq * y_5_6;

            const auto y_2_3_4_5_6 = (S) 0.00577946556085762 + x * y_3_4_5_6;
            return x + x_sq * y_2_3_4_5_6;
        }
        if constexpr (order == 7)
        {
            const auto y_6_7 = (S) -0.993023225129115 + (S) 0.604213345541030 * x;
            const auto y_4_5 = (S) -0.242568404368623 + (S) 0.780715776826480 * x;
            const auto y_2_3 = (S) -0.00231743294930714 + (S) 0.205916081200406 * x;

            const auto y_4_5_6_7 = y_4_5 + x_sq * y_6_7;

            const auto y_2_3_4_5_6_7 = y_2_3 + x_sq * y_4_5_6_7;
            return x + x_sq * y_2_3_4_5_6_7;
        }
        if constexpr (order == 8)
        {
            const auto y_7_8 = (S) -1.68181413527251 + (S) 0.833569228384441 * x;
            const auto y_5_6 = (S) -0.614138628435564 + (S) 1.51390471735914 * x;
            const auto y_3_4 = (S) 0.146440161696543 + (S) 0.167766328527588 * x;

            const auto y_5_6_7_8 = y_5_6 + x_sq * y_7_8;
            const auto y_3_4_5_6_7_8 = y_3_4 + x_sq * y_5_6_7_8;

            const auto y_2_3_4_5_6_7_8 = (S) 0.000914775210828589 + x * y_3_4_5_6_7_8;
            return x + x_sq * y_2_3_4_5_6_7_8;
        }
        if constexpr (order == 9)
        {
            const auto y_8_9 = (S) -2.8729113246543627191 + (S) 1.1910880616677141930 * x;
            const auto y_6_7 = (S) -1.7250043993765906691 + (S) 3.0742940024198017746 * x;
            const auto y_4_5 = (S) -0.10358468520191396745 + (S) 0.63814601829123507315 * x;
            const auto y_2_3 = (S) -0.00034869418257434217938 + (S) 0.17639243703620430259 * x;

            const auto y_6_7_8_9 = y_6_7 + x_sq * y_8_9;
            const auto y_2_3_4_5 = y_2_3 + x_sq * y_4_5;

            const auto y_2_3_4_5_6_7_8_9 = y_2_3_4_5 + (x_sq * x_sq) * y_6_7_8_9;
            return x + x_sq * y_2_3_4_5_6_7_8_9;
        }
        if constexpr (order == 10)
        {
            const auto y_9_10 = (S) -4.7928604989214971255 + (S) 1.7203396621648587850 * x;
            const auto y_7_8 = (S) -3.9882416242478972990 + (S) 5.9100541437570059955 * x;
            const auto y_5_6 = (S) -0.33522250091818628359 + (S) 1.6520149306717599735 * x;
            const auto y_3_4 = (S) 0.16219681678629885302 + (S) 0.059321118077451009953 * x;

            const auto y_7_8_9_10 = y_7_8 + x_sq * y_9_10;
            const auto y_3_4_5_6 = y_3_4 + x_sq * y_5_6;

            const auto y_3_4_5_6_7_8_9_10 = y_3_4_5_6 + (x_sq * x_sq) * y_7_8_9_10;
            const auto y_2_3_4_5_6_7_8_9_10 = (S) 0.00013025301412532343010 + x * y_3_4_5_6_7_8_9_10;
            return x + x_sq * y_2_3_4_5_6_7_8_9_10;
        }
        if constexpr (order == 11)
        {
            const auto y_10_11 = (S) -7.8570177355488999282 + (S) 2.4911046380177723769 * x;
            const auto y_8_9 = (S) -8.7527722722991097015 + (S) 11.027490029915364644 * x;
            const auto y_6_7 = (S) -1.3541397019129590706 + (S) 4.3623048430828985644 * x;
            const auto y_4_5 = (S) -0.031316768977302081312 + (S) 0.34169372825309551889 * x;
            const auto y_2_3 = (S) -0.000047491279899706856284 + (S) 0.16861521810527382859 * x;

            const auto y_8_9_10_11 = y_8_9 + x_sq * y_10_11;
            const auto y_4_5_6_7 = y_4_5 + x_sq * y_6_7;

            const auto y_4_5_6_7_8_9_10_11 = y_4_5_6_7 + (x_sq * x_sq) * y_8_9_10_11;
            const auto y_2_3_4_5_6_7_8_9_10_11 = y_2_3 + x_sq * y_4_5_6_7_8_9_10_11;

            return x + x_sq * y_2_3_4_5_6_7_8_9_10_11;
        }
        return {};
    }
} // namespace asin_detail

/** Asin(x) approximation, valid on the range [-1, 1] */
template <int order, typename T>
T asin (T x)
{
    using S = scalar_of_t<T>;
    static constexpr auto rsqrt2 = (S) 0.707106781186547524400844362105;

    using std::abs, std::sqrt;
#if defined(XSIMD_HPP)
    using xsimd::abs, xsimd::sqrt;
#endif
    const auto abs_x = abs (x);

    const auto reflect = abs_x > rsqrt2;
    const auto poly_arg = select (reflect, sqrt ((S) 1 - abs_x * abs_x), abs_x);
    const auto poly_res = asin_detail::asin_0_rsqrt2<order> (poly_arg);
    const auto res = select (reflect, (S) M_PI_2 - poly_res, poly_res);

    return select (x > (S) 0, res, -res);
}
} // namespace math_approx
