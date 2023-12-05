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
TANH_BENCH (sinh_std, std::sinh)
TANH_BENCH (sinh_approx6, math_approx::sinh<6>)
TANH_BENCH (sinh_approx5, math_approx::sinh<5>)
TANH_BENCH (sinh_approx4, math_approx::sinh<4>)
TANH_BENCH (sinh_approx3, math_approx::sinh<3>)

TANH_BENCH (cosh_std, std::sinh)
TANH_BENCH (cosh_approx6, math_approx::cosh<6>)
TANH_BENCH (cosh_approx5, math_approx::cosh<5>)
TANH_BENCH (cosh_approx4, math_approx::cosh<4>)
TANH_BENCH (cosh_approx3, math_approx::cosh<3>)

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
TANH_SIMD_BENCH (sinh_xsimd, xsimd::tanh)
TANH_SIMD_BENCH (sinh_simd_approx6, math_approx::sinh<6>)
TANH_SIMD_BENCH (sinh_simd_approx5, math_approx::sinh<5>)
TANH_SIMD_BENCH (sinh_simd_approx4, math_approx::sinh<4>)
TANH_SIMD_BENCH (sinh_simd_approx3, math_approx::sinh<3>)

TANH_SIMD_BENCH (cosh_xsimd, xsimd::tanh)
TANH_SIMD_BENCH (cosh_simd_approx6, math_approx::cosh<6>)
TANH_SIMD_BENCH (cosh_simd_approx5, math_approx::cosh<5>)
TANH_SIMD_BENCH (cosh_simd_approx4, math_approx::cosh<4>)
TANH_SIMD_BENCH (cosh_simd_approx3, math_approx::cosh<3>)

BENCHMARK_MAIN();
