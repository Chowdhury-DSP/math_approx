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

#define POW_BENCH(name, func) \
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
POW_BENCH (exp_std, std::exp)
POW_BENCH (exp_approx6, math_approx::exp<6>)
POW_BENCH (exp_approx5, math_approx::exp<5>)
POW_BENCH (exp_approx4, math_approx::exp<4>)
POW_BENCH (exp_approx3, math_approx::exp<3>)

// #define POW_SIMD_BENCH(name, func) \
// void name (benchmark::State& state) \
// { \
// for (auto _ : state) \
// { \
// for (auto& x : data) \
// { \
// auto y = func (xsimd::broadcast (x)); \
// static_assert (std::is_same_v<xsimd::batch<float>, decltype(y)>); \
// benchmark::DoNotOptimize (y); \
// } \
// } \
// } \
// BENCHMARK (name);
// POW_SIMD_BENCH (exp_xsimd, xsimd::exp)
// POW_SIMD_BENCH (exp_simd_approx6, math_approx::exp<6>)
// POW_SIMD_BENCH (exp_simd_approx5, math_approx::exp<5>)
// POW_SIMD_BENCH (exp_simd_approx4, math_approx::exp<4>)
// POW_SIMD_BENCH (exp_simd_approx3, math_approx::exp<3>)

BENCHMARK_MAIN();
