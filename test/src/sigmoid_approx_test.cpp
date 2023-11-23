#include "test_helpers.hpp"
#include <catch2/catch_test_macros.hpp>
#include <iostream>

#include <math_approx/math_approx.hpp>

TEST_CASE ("Sigmoid Approx Test")
{
#if ! defined(WIN32)
    const auto all_floats = test_helpers::all_32_bit_floats (-10.0f, 10.0f, 1.0e-3f);
#else
    const auto all_floats = test_helpers::all_32_bit_floats (-10.0f, 10.0f, 1.0e-1f);
#endif
    const auto y_exact = test_helpers::compute_all<float> (all_floats, [] (auto x)
                                                    { return 1.0f / (1.0f + std::exp (-x)); });

    const auto test_approx = [&all_floats, &y_exact] (auto&& f_approx, float err_bound)
    {
        const auto y_approx = test_helpers::compute_all<float> (all_floats, f_approx);

        const auto error = test_helpers::compute_error<float> (y_exact, y_approx);
        const auto max_error = test_helpers::abs_max<float> (error);

        std::cout << max_error << std::endl;
        REQUIRE (std::abs (max_error) < err_bound);
    };

    SECTION ("9th-Order")
    {
        test_approx ([] (auto x)
                     { return math_approx::sigmoid<9> (x); },
                     6.5e-7f);
    }
    SECTION ("7th-Order")
    {
        test_approx ([] (auto x)
                     { return math_approx::sigmoid<7> (x); },
                     7.0e-6f);
    }
    SECTION ("5th-Order")
    {
        test_approx ([] (auto x)
                     { return math_approx::sigmoid<5> (x); },
                     1.0e-4f);
    }
    SECTION ("3th-Order")
    {
        test_approx ([] (auto x)
                     { return math_approx::sigmoid<3> (x); },
                     2.0e-3f);
    }
}
