#include "test_helpers.hpp"
#include <catch2/catch_test_macros.hpp>
#include <iostream>

#include <math_approx/math_approx.hpp>

TEST_CASE ("Tanh Approx Test")
{
    const auto all_floats = test_helpers::all_32_bit_floats (-10.0f, 10.0f, 1.0e-5f);
    const auto y_exact = test_helpers::compute_all (all_floats, [] (auto x)
                                                    { return std::tanh (x); });

    const auto test_approx = [&all_floats, &y_exact] (auto&& f_approx, float err_bound, float rel_err_bound, uint32_t ulp_bound)
    {
        const auto y_approx = test_helpers::compute_all (all_floats, f_approx);

        const auto error = test_helpers::compute_error (y_exact, y_approx);
        const auto rel_error = test_helpers::compute_rel_error (y_exact, y_approx);
        const auto ulp_error = test_helpers::compute_ulp_error (y_exact, y_approx);

        const auto max_error = test_helpers::abs_max (error);
        const auto max_rel_error = test_helpers::abs_max (rel_error);
        const auto max_ulp_error = *std::max_element (ulp_error.begin(), ulp_error.end());

        std::cout << max_error << ", " << max_rel_error << ", " << max_ulp_error << std::endl;
        REQUIRE (std::abs (max_error) < err_bound);
        REQUIRE (std::abs (max_rel_error) < rel_err_bound);
        if (ulp_bound > 0)
            REQUIRE (max_ulp_error < ulp_bound);
    };

    SECTION ("11th-Order")
    {
        test_approx ([] (auto x)
                     { return math_approx::tanh<11> (x); },
                     1.9e-7f,
                     3.4e-7f,
                     5);
    }
    SECTION ("9th-Order")
    {
        test_approx ([] (auto x)
                     { return math_approx::tanh<9> (x); },
                     1.2e-6f,
                     1.3e-6f,
                     18);
    }
    SECTION ("7th-Order")
    {
        test_approx ([] (auto x)
                     { return math_approx::tanh<7> (x); },
                     1.4e-5f,
                     1.5e-5f,
                     226);
    }
    SECTION ("5th-Order")
    {
        test_approx ([] (auto x)
                     { return math_approx::tanh<5> (x); },
                     2.2e-4f,
                     2.2e-4f,
                     0);
    }
    SECTION ("3th-Order")
    {
        test_approx ([] (auto x)
                     { return math_approx::tanh<3> (x); },
                     3.7e-3f,
                     3.8e-3f,
                     0);
    }
}
