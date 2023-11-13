#pragma once

#include <cmath>
#include <numeric>
#include <span>
#include <vector>

namespace test_helpers
{
inline auto all_32_bit_floats (float begin, float end, float tol = 1.0e-10f)
{
    std::vector<float> vec;
    vec.reserve (1 << 20);
    begin = vec.emplace_back (begin);
    while (begin < end)
    {
        if (std::abs (begin) < tol)
        {
            begin = vec.emplace_back (0.0f);
            begin = vec.emplace_back (tol);
        }
        begin = vec.emplace_back (std::nextafter (begin, end));
    }

    return vec;
}

template <typename F>
auto compute_all (std::span<const float> all_floats,
                  F&& f)
{
    std::vector<float> y;
    y.resize (all_floats.size());
    for (size_t i = 0; i < all_floats.size(); ++i)
        y[i] = f (all_floats[i]);

    return y;
}

inline std::vector<float> compute_error (std::span<const float> actual, std::span<const float> approx)
{
    std::vector<float> err;
    err.resize (actual.size());
    for (size_t i = 0; i < actual.size(); ++i)
        err[i] = (actual[i] - approx[i]);
    return err;
}

inline std::vector<float> compute_rel_error (std::span<const float> actual, std::span<const float> approx)
{
    std::vector<float> err;
    err.resize (actual.size());
    for (size_t i = 0; i < actual.size(); ++i)
        err[i] = (actual[i] - approx[i]) / actual[i];
    return err;
}

// mostly borrowed from Catch2
inline uint32_t f32_ulp_dist (float lhs, float rhs) // NOLINT
{
    // We want X == Y to imply 0 ULP distance even if X and Y aren't
    // bit-equal (-0 and 0), or X - Y != 0 (same sign infinities).
    if (lhs == rhs)
        return 0;

    // We need a properly typed positive zero for type inference.
    static constexpr float positive_zero {};

    // We want to ensure that +/- 0 is always represented as positive zero
    if (lhs == positive_zero)
        lhs = positive_zero;
    if (rhs == positive_zero)
        rhs = positive_zero;

    // If arguments have different signs, we can handle them by summing
    // how far are they from 0 each.
    if (std::signbit (lhs) != std::signbit (rhs))
    {
        return f32_ulp_dist (std::abs (lhs), positive_zero)
               + f32_ulp_dist (std::abs (rhs), positive_zero);
    }

    // get the bit pattern of 'x'
    const auto f32_to_bits = [] (float x) -> uint32_t
    {
        uint32_t u;
        memcpy (&u, &x, 4);
        return u;
    };

    // When both lhs and rhs are of the same sign, we can just
    // read the numbers bitwise as integers, and then subtract them
    // (assuming IEEE).
    uint32_t lc = f32_to_bits (lhs);
    uint32_t rc = f32_to_bits (rhs);

    // The ulp distance between two numbers is symmetric, so to avoid
    // dealing with overflows we want the bigger converted number on the lhs
    if (lc < rc)
        std::swap (lc, rc);

    return lc - rc;
};

inline auto compute_ulp_error (std::span<const float> actual, std::span<const float> approx)
{


    std::vector<uint32_t> err;
    err.resize (actual.size());
    for (size_t i = 0; i < actual.size(); ++i)
        err[i] = f32_ulp_dist (actual[i], approx[i]);
    return err;
}

inline float abs_max (std::span<const float> x)
{
    const auto [min, max] = std::minmax_element (x.begin(), x.end());

    if (std::abs (*min) > std::abs (*max))
        return *min;
    return *max;
}
} // namespace test_helpers
