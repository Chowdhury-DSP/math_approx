#include <math_approx/math_approx.hpp>
#include <benchmark/benchmark.h>

static constexpr size_t N = 2000;
const auto data = []
{
    std::vector<float> x;
    x.resize (N, 0.0f);
    for (size_t i = 0; i < N; ++i)
        x[i] = -10.0f + 20.0f * (float) i / (float) N;
    return x;
}();

#define TANH_BENCH(name, func) \
void name (benchmark::State& state) \
{ \
    for (auto _ : state) \
    { \
        for (auto& x : data) \
        { \
            auto y = func (x); \
            benchmark::DoNotOptimize (y); \
        } \
    } \
} \
BENCHMARK (name);
TANH_BENCH (tanh_std, std::tanh)
TANH_BENCH (tanh_approx11, math_approx::tanh<11>)
TANH_BENCH (tanh_approx9, math_approx::tanh<9>)
TANH_BENCH (tanh_approx7, math_approx::tanh<7>)
TANH_BENCH (tanh_approx5, math_approx::tanh<5>)

#define TANH_SIMD_BENCH(name, func) \
void name (benchmark::State& state) \
{ \
    for (auto _ : state) \
    { \
        for (auto& x : data) \
        { \
            auto y = func (xsimd::broadcast (x)); \
            static_assert (std::is_same_v<xsimd::batch<float>, decltype(y)>); \
            benchmark::DoNotOptimize (y); \
        } \
    } \
} \
BENCHMARK (name);
TANH_SIMD_BENCH (tanh_xsimd, xsimd::tanh)
TANH_SIMD_BENCH (tanh_simd_approx11, math_approx::tanh<11>)
TANH_SIMD_BENCH (tanh_simd_approx9, math_approx::tanh<9>)
TANH_SIMD_BENCH (tanh_simd_approx7, math_approx::tanh<7>)
TANH_SIMD_BENCH (tanh_simd_approx5, math_approx::tanh<5>)

BENCHMARK_MAIN();
