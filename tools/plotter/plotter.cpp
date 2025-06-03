#include <cmath>
#include <iostream>
#include <span>
#include <vector>

#include <plt/matplotlibcpp.h>
namespace plt = matplotlibcpp;

#include "../../test/src/reference/polylogarithm.hpp"
#include "../../test/src/reference/toms917.hpp"
#include "../../test/src/reference/sincospi.hpp"
#include "../../test/src/test_helpers.hpp"
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

template <typename T>
T sigmoid_ref (T x)
{
    return (T) 1 / ((T) 1 + std::exp (-x));
}

template <typename T>
T std_sin_turns (T x)
{
    return std::sin ((T) 2 * (T) M_PI * x);
}

#define FLOAT_FUNC(func) [] (float x) { return func (x); }

int main()
{
    std::cout << sincospi::sin2pi (0.0f) << '\n';
    std::cout << sincospi::sin2pi (0.5f) << '\n';
    std::cout << sincospi::sin2pi (-0.5f) << '\n';
    std::cout << math_approx::sin_turns<9> (0.0f) << '\n';
    std::cout << math_approx::sin_turns<9> (0.5f) << '\n';
    std::cout << math_approx::sin_turns<9> (-0.5f) << '\n';

    // const auto tt = sincospi::sin2pi (0.5f);
    // const auto dd = math_approx::sin_turns_mhalfpi_halfpi<9> (0.5f);
    // std::cout << tt << ' ' << dd << '\n';
    // const auto err = test_helpers::compute_ulp_error ({ &tt, 1 }, { &dd, 1 });
    // std::cout << err.front()  << '\n';

    plt::figure();
    const auto range = std::make_pair (-0.5f, 0.5f);
    static constexpr auto tol = 1.0e-3f;

    const auto all_floats = test_helpers::all_32_bit_floats (range.first, range.second, tol);
    const auto y_exact = test_helpers::compute_all<float> (all_floats, FLOAT_FUNC (sincospi::sin2pi));
    // plot_ulp_error (all_floats, y_exact, FLOAT_FUNC ((math_approx::sin_turns<5>) ), "sint-5");
    plot_ulp_error (all_floats, y_exact, FLOAT_FUNC ((math_approx::sin_turns<7>) ), "sint-7");
    plot_ulp_error (all_floats, y_exact, FLOAT_FUNC ((math_approx::sin_turns<9>) ), "sint-9");
    plot_ulp_error (all_floats, y_exact, FLOAT_FUNC ((math_approx::sin_turns<11>) ), "sint-11");

    plt::legend ({ { "loc", "upper right" } });
    plt::xlim (range.first, range.second);
    plt::grid (true);
    plt::show();

    return 0;
}
