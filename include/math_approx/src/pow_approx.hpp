#pragma once

#include "basic_math.hpp"

namespace math_approx
{
namespace pow_detail
{
    /** approximation for 2^x, optimized on the range [0, 1] */
    template <typename T, int order>
    constexpr T pow2_approx (T x)
    {
        static_assert (order >= 3 && order <= 6);
        using S = scalar_of_t<T>;

        const auto x_sq = x * x;
        if constexpr (order == 3)
        {
            const auto x_2_3 = (S) 0.226307586882 + (S) 0.0782680256330 * x;
            const auto x_0_1 = (S) 1 + (S) 0.695424387485 * x;
            return x_0_1 + x_2_3 * x_sq;
        }
        else if constexpr (order == 4)
        {
            const auto x_3_4 = (S) 0.0520324008177 + (S) 0.0135557244044 * x;
            const auto x_1_2 = (S) 0.693032120001 + (S) 0.241379754777 * x;
            const auto x_1_2_3_4 = x_1_2 + x_3_4 * x_sq;
            return (S) 1 + x_1_2_3_4 * x;
        }
        else if constexpr (order == 5)
        {
            const auto x_4_5 = (S) 0.00899009909264 + (S) 0.00187839071291 * x;
            const auto x_2_3 = (S) 0.240156326598 + (S) 0.0558229130202 * x;
            const auto x_2_3_4_5 = x_2_3 + x_4_5 * x_sq;
            const auto x_0_1 = (S) 1 + (S) 0.693152270576 * x;
            return x_0_1 + x_2_3_4_5 * x_sq;
        }
        else if constexpr (order == 6)
        {
            const auto x_5_6 = (S) 0.00124359387839 + (S) 0.000217187820427 * x;
            const auto x_3_4 = (S) 0.0554833098983 + (S) 0.00967911763840 * x;
            const auto x_1_2 = (S) 0.693147003658 + (S) 0.240229787107 * x;
            const auto x_3_4_5_6 = x_3_4 + x_5_6 * x_sq;
            const auto x_1_2_3_4_5_6 = x_1_2 + x_3_4_5_6 * x_sq;
            return (S) 1 + x_1_2_3_4_5_6 * x;
        }
        else
        {
            return {};
        }
    }

    template <typename T>
    struct BaseE
    {
        static constexpr auto log2_base = (T) 1.4426950408889634074;
    };

    template <typename T>
    struct Base2
    {
        static constexpr auto log2_base = (T) 1;
    };

    template <typename T>
    struct Base10
    {
        static constexpr auto log2_base = (T) 3.3219280948873623479;
    };
}

#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstrict-aliasing" // these methods require some type-punning
#pragma GCC diagnostic ignored "-Wuninitialized"
#endif

/** approximation for pow(Base, x) (32-bit) */
template <typename Base, int order>
float pow (float x)
{
    x = std::max (-126.0f, Base::log2_base * x);

    const auto xi = (int32_t) x;
    const auto l = x < 0.0f ? xi - 1 : xi;
    const auto f = x - (float) l;
    const auto vi = (l + 127) << 23;

    return reinterpret_cast<const float&> (vi) * pow_detail::pow2_approx<float, order> (f);
}

/** approximation for pow(Base, x) (64-bit) */
template <typename Base, int order>
double pow (double x)
{
    x = std::max (-1022.0, Base::log2_base * x);

    const auto xi = (int64_t) x;
    const auto l = x < 0.0 ? xi - 1 : xi;
    const auto d = x - (double) l;
    const auto vi = (l + 1023) << 52;

    return reinterpret_cast<const double&> (vi) * pow_detail::pow2_approx<double, order> (d);
}

#if defined(XSIMD_HPP)
/** approximation for pow(Base, x) (32-bit SIMD) */
template <typename Base, int order>
xsimd::batch<float> pow (xsimd::batch<float> x)
{
    x = xsimd::max (xsimd::broadcast (-126.0f), Base::log2_base * x);

    const auto xi = xsimd::to_int (x);
    const auto l = xsimd::select (xsimd::batch_bool_cast<int32_t> (x < 0.0f), xi - 1, xi);
    const auto f = x - xsimd::to_float (l);
    const auto vi = (l + 127) << 23;

    return reinterpret_cast<const xsimd::batch<float>&> (vi) * pow_detail::pow2_approx<xsimd::batch<float>, order> (f);
}

/** approximation for pow(Base, x) (64-bit SIMD) */
template <typename Base, int order>
xsimd::batch<double> pow (xsimd::batch<double> x)
{
    x = xsimd::max (-1022.0, Base::log2_base * x);

    const auto xi = xsimd::to_int (x);
    const auto l = xsimd::select (xsimd::batch_bool_cast<int64_t> (x < 0.0), xi - 1, xi);
    const auto d = x - xsimd::to_float (l);
    const auto vi = (l + 1023) << 52;

    return reinterpret_cast<const xsimd::batch<double>&> (vi) * pow_detail::pow2_approx<xsimd::batch<double>, order> (d);
}
#endif

#if defined(__GNUC__)
#pragma GCC diagnostic pop // end ignore strict-aliasing warnings
#endif

template <int order, typename T>
T exp (T x)
{
    return pow<pow_detail::BaseE<scalar_of_t<T>>, order> (x);
}

template <int order, typename T>
T exp2 (T x)
{
    return pow<pow_detail::Base2<scalar_of_t<T>>, order> (x);
}

template <int order, typename T>
T exp10 (T x)
{
    return pow<pow_detail::Base10<scalar_of_t<T>>, order> (x);
}
}
