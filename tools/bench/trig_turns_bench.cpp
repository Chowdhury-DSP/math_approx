#include <math_approx/math_approx.hpp>
#include <benchmark/benchmark.h>
#include "../test/src/reference/sincospi.hpp"

static constexpr size_t N = 2000;
const auto data = []
{
    std::vector<float> x;
    x.resize (N, 0.0f);
    for (size_t i = 0; i < N; ++i)
        x[i] = -10.0f + 20.0f * (float) i / (float) N;
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

TRIG_BENCH (cos_std, std::cos)
TRIG_BENCH (cos_ref, sincospi::cos2pi)
TRIG_BENCH (cos_turns_approx11, math_approx::cos_turns<11>)
TRIG_BENCH (cos_turns_approx9, math_approx::cos_turns<9>)
TRIG_BENCH (cos_turns_approx7, math_approx::cos_turns<7>)
TRIG_BENCH (cos_turns_approx5, math_approx::cos_turns<5>)

TRIG_BENCH (sin_std, std::sin)
TRIG_BENCH (sin_turns_ref, sincospi::sin2pi)
TRIG_BENCH (sin_turns_approx11, math_approx::sin_turns<11>)
TRIG_BENCH (sin_turns_approx9, math_approx::sin_turns<9>)
TRIG_BENCH (sin_turns_approx7, math_approx::sin_turns<7>)
TRIG_BENCH (sin_turns_approx5, math_approx::sin_turns<5>)

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

TRIG_SIMD_BENCH (sin_xsimd, xsimd::sin)
TRIG_SIMD_BENCH (sin_turns_simd_approx11, math_approx::sin_turns<11>)
TRIG_SIMD_BENCH (sin_turns_simd_approx9, math_approx::sin_turns<9>)
TRIG_SIMD_BENCH (sin_turns_simd_approx7, math_approx::sin_turns<7>)
TRIG_SIMD_BENCH (sin_turns_simd_approx5, math_approx::sin_turns<5>)

TRIG_SIMD_BENCH (cos_xsimd, xsimd::cos)
TRIG_SIMD_BENCH (cos_turns_simd_approx11, math_approx::cos_turns<11>)
TRIG_SIMD_BENCH (cos_turns_simd_approx9, math_approx::cos_turns<9>)
TRIG_SIMD_BENCH (cos_turns_simd_approx7, math_approx::cos_turns<7>)
TRIG_SIMD_BENCH (cos_turns_simd_approx5, math_approx::cos_turns<5>)

BENCHMARK_MAIN();
