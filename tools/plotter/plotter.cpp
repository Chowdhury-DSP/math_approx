#include <cmath>
#include <iostream>
#include <span>
#include <vector>

#include <plt/matplotlibcpp.h>
namespace plt = matplotlibcpp;

#include "../../test/src/reference/polylogarithm.hpp"
#include "../../test/src/reference/toms917.hpp"
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

// template <typename T, int order>
// T sin_turns (T x)
// {
//     return math_approx::sin<order> ((T) 2 * (T) M_PI * x);
// }

/* Writes result sine result sin(πa) to the location pointed to by sp
   Writes result cosine result cos(πa) to the location pointed to by cp

   In exhaustive testing, the maximum error in sine results was 0.96677 ulp,
   the maximum error in cosine results was 0.96563 ulp, meaning results are
   faithfully rounded.

   Copied from: https://stackoverflow.com/questions/42792939/implementation-of-sinpi-and-cospi-using-standard-c-math-library
*/
void sincospif (float a, float *sp, float *cp)
{
    float az, t, c, r, s;
    int32_t i;

    az = a * 0.0f; // must be evaluated with IEEE-754 semantics
    /* for |a| > 2**24, cospi(a) = 1.0f, but cospi(Inf) = NaN */
    a = (fabsf (a) < 0x1.0p24f) ? a : az;
    r = nearbyintf (a + a); // must use IEEE-754 "to nearest" rounding
    i = (int32_t)r;
    t = fmaf (-0.5f, r, a);
    /* compute core approximations */
    s = t * t;
    /* Approximate cos(pi*x) for x in [-0.25,0.25] */
    r =              0x1.d9e000p-3f;
    r = fmaf (r, s, -0x1.55c400p+0f);
    r = fmaf (r, s,  0x1.03c1cep+2f);
    r = fmaf (r, s, -0x1.3bd3ccp+2f);
    c = fmaf (r, s,  0x1.000000p+0f);
    /* Approximate sin(pi*x) for x in [-0.25,0.25] */
    r =             -0x1.310000p-1f;
    r = fmaf (r, s,  0x1.46737ep+1f);
    r = fmaf (r, s, -0x1.4abbfep+2f);
    r = (t * s) * r;
    s = fmaf (t, 0x1.921fb6p+1f, r);
    if (i & 2) {
        s = 0.0f - s; // must be evaluated with IEEE-754 semantics
        c = 0.0f - c; // must be evaluated with IEEE-754 semantics
    }
    if (i & 1) {
        t = 0.0f - s; // must be evaluated with IEEE-754 semantics
        s = c;
        c = t;
    }
    /* IEEE-754: sinPi(+n) is +0 and sinPi(-n) is -0 for positive integers n */
    if (a == floorf (a)) s = az;
    *sp = s;
    *cp = c;
}

float sinpi (float x)
{
    float s, c;
    sincospif (2.0f * x, &s, &c);
    return s;
}

#define FLOAT_FUNC(func) [] (float x) { return func (x); }

int main()
{
    // std::cout << math_approx::sin_turns_mhalfpi_halfpi<9> (0.0f) << '\n';
    // std::cout << math_approx::sin_turns_mhalfpi_halfpi<9> (0.5f) << '\n';
    // std::cout << math_approx::sin_turns_mhalfpi_halfpi<9> (-0.5f) << '\n';

    const auto tt = sinpi (0.5f);
    const auto dd = math_approx::sin_turns_mhalfpi_halfpi<9> (0.5f);
    std::cout << tt << ' ' << dd << '\n';
    const auto err = test_helpers::compute_ulp_error ({ &tt, 1 }, { &dd, 1 });
    std::cout << err.front()  << '\n';

    plt::figure();
    const auto range = std::make_pair (-0.5f, 0.5f);
    static constexpr auto tol = 4.0e-3f;

    const auto all_floats = test_helpers::all_32_bit_floats (range.first, range.second, tol);
    const auto y_exact = test_helpers::compute_all<float> (all_floats, FLOAT_FUNC (sinpi));
    plot_ulp_error (all_floats, y_exact, FLOAT_FUNC ((math_approx::sin_turns_mhalfpi_halfpi<9>) ), "sint-9");

    plt::legend ({ { "loc", "upper right" } });
    plt::xlim (range.first, range.second);
    plt::grid (true);
    plt::show();

    return 0;
}
