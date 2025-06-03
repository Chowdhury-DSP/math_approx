#include "test_helpers.hpp"
#include <catch2/catch_test_macros.hpp>
#include <iostream>

#include <math_approx/math_approx.hpp>
#include "reference/sincospi.hpp"

TEST_CASE ("Sine Approx Test")
{
#if ! defined(WIN32)
    const auto all_floats = test_helpers::all_32_bit_floats (-10.0f, 10.0f, 1.0e-3f);
#else
    const auto all_floats = test_helpers::all_32_bit_floats (-10.0f, 10.0f, 1.0e-1f);
#endif
    const auto y_exact = test_helpers::compute_all<float> (all_floats, [] (auto x)
                                                    { return sincospi::sin2pi (x); });

    const auto test_approx = [&all_floats, &y_exact] (auto&& f_approx, float rel_err_bound, uint32_t ulp_err_bound)
    {
        const auto y_approx = test_helpers::compute_all<float> (all_floats, f_approx);

        const auto rel_error = test_helpers::compute_rel_error<float> (y_exact, y_approx);
        const auto ulp_error = test_helpers::compute_ulp_error (y_exact, y_approx);

        const auto max_rel_error = test_helpers::abs_max<float> (rel_error);
        const auto max_ulp_error = *std::max_element (ulp_error.begin(), ulp_error.end());

        std::cout << max_rel_error << ", " << max_ulp_error << std::endl;
        REQUIRE (std::abs (max_rel_error) < rel_err_bound);
        REQUIRE (max_ulp_error < ulp_err_bound);
    };

    SECTION ("11th-Order")
    {
        test_approx ([] (auto x)
                     { return math_approx::sin_turns<11> (x); },
                     5.0e-7f,
                     6);
    }
    SECTION ("9th-Order")
    {
        test_approx ([] (auto x)
                     { return math_approx::sin_turns<9> (x); },
                     2.0e-6f,
                     14);
    }
    SECTION ("7th-Order")
    {
        test_approx ([] (auto x)
                     { return math_approx::sin_turns<7> (x); },
                     9.0e-5f,
                     490);
    }
    SECTION ("5th-Order")
    {
        test_approx ([] (auto x)
                     { return math_approx::sin_turns<5> (x); },
                     5.0e-3f,
                     22'000);
    }
}

TEST_CASE ("Cosine Approx Test")
{
#if ! defined(WIN32)
    const auto all_floats = test_helpers::all_32_bit_floats (-10.0f, 10.0f, 1.0e-3f);
#else
    const auto all_floats = test_helpers::all_32_bit_floats (-10.0f, 10.0f, 1.0e-1f);
#endif
    const auto y_exact = test_helpers::compute_all<float> (all_floats, [] (auto x)
                                                    { return sincospi::cos2pi (x); });

    const auto test_approx = [&all_floats, &y_exact] (auto&& f_approx, float rel_err_bound, uint32_t ulp_err_bound)
    {
        const auto y_approx = test_helpers::compute_all<float> (all_floats, f_approx);

        const auto rel_error = test_helpers::compute_rel_error<float> (y_exact, y_approx);
        const auto ulp_error = test_helpers::compute_ulp_error (y_exact, y_approx);

        const auto max_rel_error = test_helpers::abs_max<float> (rel_error);
        const auto max_ulp_error = *std::max_element (ulp_error.begin(), ulp_error.end());

        std::cout << max_rel_error << ", " << max_ulp_error << std::endl;
        REQUIRE (std::abs (max_rel_error) < rel_err_bound);
        REQUIRE (max_ulp_error < ulp_err_bound);
    };

    SECTION ("11th-Order")
    {
        test_approx ([] (auto x)
                     { return math_approx::cos_turns<11> (x); },
                     5.0e-7f,
                     6);
    }
    SECTION ("9th-Order")
    {
        test_approx ([] (auto x)
                     { return math_approx::cos_turns<9> (x); },
                     2.0e-6f,
                     10);
    }
    SECTION ("7th-Order")
    {
        test_approx ([] (auto x)
                     { return math_approx::cos_turns<7> (x); },
                     6.0e-5f,
                     270);
    }
    SECTION ("5th-Order")
    {
        test_approx ([] (auto x)
                     { return math_approx::cos_turns<5> (x); },
                     3.0e-3f,
                     14'000);
    }
}
