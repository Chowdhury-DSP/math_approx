#include "test_helpers.hpp"
#include <catch2/catch_test_macros.hpp>
#include <iostream>

#include <math_approx/math_approx.hpp>

TEST_CASE ("Sine Approx Test")
{
#if ! defined(WIN32)
    const auto all_floats = test_helpers::all_32_bit_floats (-10.0f, 10.0f, 1.0e-3f);
#else
    const auto all_floats = test_helpers::all_32_bit_floats (-10.0f, 10.0f, 1.0e-1f);
#endif
    const auto y_exact = test_helpers::compute_all (all_floats, [] (auto x)
                                                    { return std::sin (x); });

    const auto test_approx = [&all_floats, &y_exact] (auto&& f_approx, float err_bound)
    {
        const auto y_approx = test_helpers::compute_all (all_floats, f_approx);

        const auto error = test_helpers::compute_error (y_exact, y_approx);
        const auto max_error = test_helpers::abs_max (error);

        // std::cout << max_error << std::endl;
        REQUIRE (std::abs (max_error) < err_bound);
    };

    SECTION ("9th-Order")
    {
        test_approx ([] (auto x)
                     { return math_approx::sin<9> (x); },
                     8.5e-7f);
    }
    SECTION ("7th-Order")
    {
        test_approx ([] (auto x)
                     { return math_approx::sin<7> (x); },
                     1.8e-5f);
    }
    SECTION ("5th-Order")
    {
        test_approx ([] (auto x)
                     { return math_approx::sin<5> (x); },
                     7.5e-4f);
    }
}
