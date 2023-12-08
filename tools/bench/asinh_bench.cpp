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

#define ASINH_BENCH(name, func) \
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
ASINH_BENCH (asinh_std, std::asinh)
ASINH_BENCH (asinh_approx7, math_approx::asinh<7>)
ASINH_BENCH (asinh_approx6, math_approx::asinh<6>)
ASINH_BENCH (asinh_approx5, math_approx::asinh<5>)
ASINH_BENCH (asinh_approx4, math_approx::asinh<4>)
ASINH_BENCH (asinh_approx3, math_approx::asinh<3>)

#define ASINH_SIMD_BENCH(name, func) \
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
ASINH_SIMD_BENCH (asinh_xsimd, xsimd::asinh)
ASINH_SIMD_BENCH (asinh_simd_approx7, math_approx::asinh<7>)
ASINH_SIMD_BENCH (asinh_simd_approx6, math_approx::asinh<6>)
ASINH_SIMD_BENCH (asinh_simd_approx5, math_approx::asinh<5>)
ASINH_SIMD_BENCH (asinh_simd_approx4, math_approx::asinh<4>)
ASINH_SIMD_BENCH (asinh_simd_approx3, math_approx::asinh<3>)

BENCHMARK_MAIN();
