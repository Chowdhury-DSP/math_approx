if($ENV{CI})
    message(STATUS "Configuring in CI, skipping benchmarks...")
    return()
endif()

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

add_executable(trig_approx_bench trig_bench.cpp)
target_link_libraries(trig_approx_bench PRIVATE benchmark::benchmark math_approx)

add_executable(inverse_trig_approx_bench inverse_trig_bench.cpp)
target_link_libraries(inverse_trig_approx_bench PRIVATE benchmark::benchmark math_approx)

add_executable(pow_approx_bench pow_bench.cpp)
target_link_libraries(pow_approx_bench PRIVATE benchmark::benchmark math_approx)

add_executable(log_approx_bench log_bench.cpp)
target_link_libraries(log_approx_bench PRIVATE benchmark::benchmark math_approx)

add_executable(wright_omega_approx_bench wright_omega_bench.cpp)
target_link_libraries(wright_omega_approx_bench PRIVATE benchmark::benchmark math_approx)

add_executable(polylog_approx_bench polylog_bench.cpp)
target_link_libraries(polylog_approx_bench PRIVATE benchmark::benchmark math_approx)

add_executable(sinh_cosh_approx_bench sinh_cosh_bench.cpp)
target_link_libraries(sinh_cosh_approx_bench PRIVATE benchmark::benchmark math_approx)

add_executable(asinh_approx_bench asinh_bench.cpp)
target_link_libraries(asinh_approx_bench PRIVATE benchmark::benchmark math_approx)
