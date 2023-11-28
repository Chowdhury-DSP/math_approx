#include <cmath>
#include <iostream>
#include <span>
#include <vector>

#include <plt/matplotlibcpp.h>
namespace plt = matplotlibcpp;

#include "../../test/src/test_helpers.hpp"
#include "../../test/src/reference/toms917.hpp"
#include "../../test/src/reference/dangelo_omega.hpp"
#include <math_approx/math_approx.hpp>

template <typename F_Approx>
void plot_error (std::span<const float> all_floats,
                 std::span<const float> y_exact,
                 F_Approx&& f_approx,
                 const std::string& name)
{
    const auto y_approx = test_helpers::compute_all<float> (all_floats, f_approx);
    const auto error = test_helpers::compute_error<float> (y_exact, y_approx);
    std::cout << "Max Error: " << test_helpers::abs_max<float> (error) << std::endl;
    plt::named_plot<float, float> (name, all_floats, error);
}

template <typename F_Approx>
void plot_rel_error (std::span<const float> all_floats,
                     std::span<const float> y_exact,
                     F_Approx&& f_approx,
                     const std::string& name)
{
    const auto y_approx = test_helpers::compute_all<float> (all_floats, f_approx);
    const auto rel_error = test_helpers::compute_rel_error<float> (y_exact, y_approx);
    std::cout << "Max Relative Error: " << test_helpers::abs_max<float> (rel_error) << std::endl;
    plt::named_plot<float, float> (name, all_floats, rel_error);
}

template <typename F_Approx>
void plot_ulp_error (std::span<const float> all_floats,
                     std::span<const float> y_exact,
                     F_Approx&& f_approx,
                     const std::string& name)
{
    const auto y_approx = test_helpers::compute_all<float> (all_floats, f_approx);
    const auto ulp_error = test_helpers::compute_ulp_error (y_exact, y_approx);
    std::cout << "Max Relative Error: " << *std::max_element (ulp_error.begin(), ulp_error.end()) << std::endl;
    plt::named_plot<float, float> (name, all_floats, std::vector<float> { ulp_error.begin(), ulp_error.end() });
}

template <typename F>
void plot_function (std::span<const float> all_floats,
                    F&& f,
                    const std::string& name)
{
    const auto y_approx = test_helpers::compute_all (all_floats, f);
    plt::named_plot<float, float> (name, all_floats, y_approx);
}

#define FLOAT_FUNC(func) [] (float x) { return func (x); }

int main()
{
    plt::figure();
    const auto range = std::make_pair (-10.0f, 30.0f);
    static constexpr auto tol = 1.0e-1f;

    const auto all_floats = test_helpers::all_32_bit_floats (range.first, range.second, tol);
    const auto y_exact = test_helpers::compute_all<float> (all_floats, FLOAT_FUNC(toms917::wrightomega));

    // plot_error (all_floats, y_exact, FLOAT_FUNC((math_approx::wright_omega<0>)), "W-O 0-3");
    // plot_error (all_floats, y_exact, FLOAT_FUNC((math_approx::wright_omega<0, 5>)), "W-O 0-5");
    plot_error (all_floats, y_exact, FLOAT_FUNC((math_approx::wright_omega<1>)), "W-O 1-3");
    // plot_error (all_floats, y_exact, FLOAT_FUNC((math_approx::wright_omega_dangelo<0>)), "W-O D'Angelo 0");
    plot_error (all_floats, y_exact, FLOAT_FUNC((math_approx::wright_omega_dangelo<1>)), "W-O D'Angelo 1");
    plot_error (all_floats, y_exact, FLOAT_FUNC((math_approx::wright_omega_dangelo<2>)), "W-O D'Angelo 2");

    plt::legend ({ { "loc", "upper right" } });
    plt::xlim (range.first, range.second);
    plt::grid (true);
    plt::show();

    return 0;
}
