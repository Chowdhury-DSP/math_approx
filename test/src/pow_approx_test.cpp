#include "test_helpers.hpp"
#include <catch2/catch_test_macros.hpp>
#include <iostream>

#include <math_approx/math_approx.hpp>

void test_approx (const auto& all_floats, const auto& y_exact, auto&& f_approx, float rel_err_bound, uint32_t ulp_bound)
{
    const auto y_approx = test_helpers::compute_all (all_floats, f_approx);

    const auto error = test_helpers::compute_error (y_exact, y_approx);
    const auto rel_error = test_helpers::compute_rel_error (y_exact, y_approx);
    const auto ulp_error = test_helpers::compute_ulp_error (y_exact, y_approx);

    const auto max_rel_error = test_helpers::abs_max (rel_error);
    const auto max_ulp_error = *std::max_element (ulp_error.begin(), ulp_error.end());

    std::cout << max_rel_error << ", " << max_ulp_error << std::endl;
    REQUIRE (std::abs (max_rel_error) < rel_err_bound);
    if (ulp_bound > 0)
        REQUIRE (max_ulp_error < ulp_bound);
};

TEST_CASE ("Exp Approx Test")
{
#if ! defined(WIN32)
    static const auto all_floats = test_helpers::all_32_bit_floats (-10.0f, 10.0f, 1.0e-3f);
#else
    static const auto all_floats = test_helpers::all_32_bit_floats (-10.0f, 10.0f, 1.0e-1f);
#endif
    const auto y_exact = test_helpers::compute_all (all_floats, [] (auto x)
                                                    { return std::exp (x); });

    SECTION ("6th-Order")
    {
        test_approx (all_floats, y_exact, [] (auto x)
                     { return math_approx::exp<6> (x); },
                     6.0e-7f,
                     10);
    }
    SECTION ("5th-Order")
    {
        test_approx (all_floats, y_exact, [] (auto x)
                     { return math_approx::exp<5> (x); },
                     7.5e-7f,
                     15);
    }
    SECTION ("4th-Order")
    {
        test_approx (all_floats, y_exact, [] (auto x)
                     { return math_approx::exp<4> (x); },
                     4.0e-6f,
                     80);
    }
    SECTION ("3th-Order")
    {
        test_approx (all_floats, y_exact, [] (auto x)
                     { return math_approx::exp<3> (x); },
                     1.5e-4f,
                     0);
    }
}

TEST_CASE ("Exp2 Approx Test")
{
#if ! defined(WIN32)
    static const auto all_floats = test_helpers::all_32_bit_floats (-10.0f, 10.0f, 1.0e-3f);
#else
    static const auto all_floats = test_helpers::all_32_bit_floats (-10.0f, 10.0f, 1.0e-1f);
#endif
    const auto y_exact = test_helpers::compute_all (all_floats, [] (auto x)
                                                    { return std::exp2 (x); });

    SECTION ("6th-Order")
    {
        test_approx (all_floats, y_exact, [] (auto x)
                     { return math_approx::exp2<6> (x); },
                     3.0e-7f,
                     4);
    }
    SECTION ("5th-Order")
    {
        test_approx (all_floats, y_exact, [] (auto x)
                     { return math_approx::exp2<5> (x); },
                     4.0e-7f,
                     5);
    }
    SECTION ("4th-Order")
    {
        test_approx (all_floats, y_exact, [] (auto x)
                     { return math_approx::exp2<4> (x); },
                     4.0e-6f,
                     70);
    }
    SECTION ("3th-Order")
    {
        test_approx (all_floats, y_exact, [] (auto x)
                     { return math_approx::exp2<3> (x); },
                     1.5e-4f,
                     0);
    }
}

TEST_CASE ("Exp10 Approx Test")
{
#if ! defined(WIN32)
    static const auto all_floats = test_helpers::all_32_bit_floats (-10.0f, 10.0f, 1.0e-3f);
#else
    static const auto all_floats = test_helpers::all_32_bit_floats (-10.0f, 10.0f, 1.0e-1f);
#endif
    const auto y_exact = test_helpers::compute_all (all_floats, [] (auto x)
                                                    { return std::pow (10.0f, x); });

    SECTION ("6th-Order")
    {
        test_approx (all_floats, y_exact, [] (auto x)
                     { return math_approx::exp10<6> (x); },
                     2.0e-6f,
                     32);
    }
    SECTION ("5th-Order")
    {
        test_approx (all_floats, y_exact, [] (auto x)
                     { return math_approx::exp10<5> (x); },
                     2.5e-6f,
                     35);
    }
    SECTION ("4th-Order")
    {
        test_approx (all_floats, y_exact, [] (auto x)
                     { return math_approx::exp10<4> (x); },
                     5.5e-6f,
                     90);
    }
    SECTION ("3th-Order")
    {
        test_approx (all_floats, y_exact, [] (auto x)
                     { return math_approx::exp10<3> (x); },
                     1.5e-4f,
                     0);
    }
}
