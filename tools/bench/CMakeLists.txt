CPMAddPackage(
    NAME benchmark
    GITHUB_REPOSITORY google/benchmark
    VERSION 1.5.2
    OPTIONS "BENCHMARK_ENABLE_TESTING Off"
)

add_executable(tanh_approx_bench tanh_bench.cpp)
target_link_libraries(tanh_approx_bench PRIVATE benchmark::benchmark math_approx)

add_executable(sigmoid_approx_bench sigmoid_bench.cpp)
target_link_libraries(sigmoid_approx_bench PRIVATE benchmark::benchmark math_approx)
