#include <cmath>
#include <iostream>
#include <span>
#include <vector>

#include <plt/matplotlibcpp.h>
namespace plt = matplotlibcpp;

#include "../../test/src/test_helpers.hpp"
#include <math_approx/math_approx.hpp>

template <typename F_Approx>
void plot_error (std::span<const float> all_floats,
                 std::span<const float> y_exact,
                 F_Approx&& f_approx,
                 const std::string& name)
{
    const auto y_approx = test_helpers::compute_all (all_floats, f_approx);
    const auto error = test_helpers::compute_error (y_exact, y_approx);
    std::cout << "Max Error: " << test_helpers::abs_max (error) << std::endl;
    plt::named_plot<float, float> (name, all_floats, error);
}

template <typename F_Approx>
void plot_rel_error (std::span<const float> all_floats,
                     std::span<const float> y_exact,
                     F_Approx&& f_approx,
                     const std::string& name)
{
    const auto y_approx = test_helpers::compute_all (all_floats, f_approx);
    const auto rel_error = test_helpers::compute_rel_error (y_exact, y_approx);
    std::cout << "Max Relative Error: " << test_helpers::abs_max (rel_error) << std::endl;
    plt::named_plot<float, float> (name, all_floats, rel_error);
}

template <typename F_Approx>
void plot_ulp_error (std::span<const float> all_floats,
                     std::span<const float> y_exact,
                     F_Approx&& f_approx,
                     const std::string& name)
{
    const auto y_approx = test_helpers::compute_all (all_floats, f_approx);
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

int main()
{
    plt::figure();
    const auto range = std::make_pair (-3.141f, 3.141f);
    static constexpr auto tol = 1.0e-2f;

    const auto all_floats = test_helpers::all_32_bit_floats (range.first, range.second, tol);
    const auto y_exact = test_helpers::compute_all (all_floats, [] (float x)
                                                    { return std::cos (x); });

    // // plot_error (all_floats, y_exact, [] (float x) { return math_approx::sin<5> (x); }, "Sin-5");
    // // plot_error (all_floats, y_exact, [] (float x) { return math_approx::sin<7> (x); }, "Sin-7");
    // plot_ulp_error (all_floats, y_exact, [] (float x) { return math_approx::cos_mpi_pi<9> (x); }, "Cos-9");
    plot_function (all_floats, [] (float x) { return math_approx::cos_mpi_pi<9> (x); }, "Cos-9");

    plt::legend ({ { "loc", "upper right" } });
    plt::xlim (range.first, range.second);
    plt::grid (true);
    plt::show();

    return 0;
}
