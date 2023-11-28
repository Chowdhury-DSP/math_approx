#pragma once

#include "basic_math.hpp"

namespace math_approx
{
template <int num_nr_iters, int poly_order = 3, int log_order = (num_nr_iters <= 1 ? 3 : 4), int exp_order = log_order, typename T>
T wright_omega (T x)
{
    static_assert (poly_order == 3 || poly_order == 5);

    using S = scalar_of_t<T>;
    static constexpr auto E = (S) 2.7182818284590452354;

    const auto x1 = [](T _x)
    {
        const auto x_sq = _x * _x;
        if constexpr (poly_order == 3)
        {
            const auto y_2_3 = (S) 0.0534379648805832 + (S) -0.00251076420630778 * _x;
            const auto y_0_1 = (S) 0.616522951065868 + (S) 0.388418422853809 * _x;
            return y_0_1 + y_2_3 * x_sq;
        }
        else if constexpr (poly_order == 5)
        {
            const auto y_4_5 = (S) -0.00156418794118294 + (S) -0.00151562297325209 * _x;
            const auto y_2_3 = (S) 0.0719291313363515 + (S) 0.0216881206167543 * _x;
            const auto y_0_1 = (S) 0.569291529016010 + (S) 0.290890537885083 * _x;
            const auto y_2_3_4_5 = y_2_3 + y_4_5 * x_sq;
            return y_0_1 + y_2_3_4_5 * x_sq;
        }
        else
        {
            return T{};
        }
    } (x);
    const auto x2 = x - log<log_order> (x) + (S) 0.32352057096397160124 * exp<exp_order> ((S) -0.029614177658043381316 * x);

    auto y = select (x < (S) -3, T{}, select (x < (S) E, x1, x2));

    const auto nr_update = [](T _x, T _y)
    {
        return _y - (_y - exp<exp_order> (_x - _y)) / (_y + (S) 1);
    };

    for (int i = 0; i < num_nr_iters; ++i)
        y = nr_update (x, y);

    return y;
}
}
