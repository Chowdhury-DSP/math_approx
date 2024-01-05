#include <math_approx/math_approx.hpp>
#include <benchmark/benchmark.h>

static constexpr size_t N = 1000;
const auto data = []
{
    std::vector<float> x;
    x.resize (N, 0.0f);
    for (size_t i = 0; i < N; ++i)
        x[i] = -1.0f + 2.0f * (float) i / (float) N;
    return x;
}();

#define TRIG_BENCH(name, func) \
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

// TRIG_BENCH (asin_std, std::asin)
// TRIG_BENCH (asin_approx11, math_approx::asin<11>)
// TRIG_BENCH (asin_approx10, math_approx::asin<10>)
// TRIG_BENCH (asin_approx9, math_approx::asin<9>)
// TRIG_BENCH (asin_approx8, math_approx::asin<8>)
// TRIG_BENCH (asin_approx7, math_approx::asin<7>)
// TRIG_BENCH (asin_approx6, math_approx::asin<6>)
// TRIG_BENCH (asin_approx5, math_approx::asin<5>)
// TRIG_BENCH (asin_approx4, math_approx::asin<4>)
// TRIG_BENCH (asin_approx3, math_approx::asin<3>)

#define TRIG_SIMD_BENCH(name, func) \
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

TRIG_SIMD_BENCH (asin_xsimd, xsimd::asin)
TRIG_SIMD_BENCH (asin_simd_approx11, math_approx::asin<11>)
TRIG_SIMD_BENCH (asin_simd_approx10, math_approx::asin<10>)
TRIG_SIMD_BENCH (asin_simd_approx9, math_approx::asin<9>)
TRIG_SIMD_BENCH (asin_simd_approx8, math_approx::asin<8>)
TRIG_SIMD_BENCH (asin_simd_approx7, math_approx::asin<7>)
TRIG_SIMD_BENCH (asin_simd_approx6, math_approx::asin<6>)
TRIG_SIMD_BENCH (asin_simd_approx5, math_approx::asin<5>)
TRIG_SIMD_BENCH (asin_simd_approx4, math_approx::asin<4>)
TRIG_SIMD_BENCH (asin_simd_approx3, math_approx::asin<3>)

BENCHMARK_MAIN();
