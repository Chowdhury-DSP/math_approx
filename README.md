# math_approx

`math_approx` is a C++ library for math approximations.

Currently supported:

- sin/cos/tan
- arcsin/arccos
- exp/exp2/exp10/expm1
- log/log2/log10/log1p
- sinh/cosh/tanh
- arcsinh
- sigmoid
- Wright-Omega function
- Dilogarithm function

Showing my work:
TODO

At the moment, most of these implementations have been "good enough"
for my own use cases (both in terms of performance and accuracy). That
said, I definitely believe that it's possible to achieve better results
for many of these functions. If you have ideas for improving these
approximations, either by:
- Modifying an approximation to achieve better accuracy with the same (or similar) performance
- Modifying an approximation to achieve better performance with the same (or similar) accuracy

then please get in touch with a GitHub issue or pull request!

## A few other thoughts

### Accuracy vs. Performance

Most of the methods in this library are provided with template
arguments which control the "order" of the approximation. The
"order" typically refers to the order of a polynomial used in
the approximation. In general, higher-order approximations will
achieve greater accuracy, while taking longer to compute.

### Measuring Accuracy

This library uses three approaches for measuring accuracy:
- Absolute error (`Error = |actual - approx|`)
- Relative error (`Error = |(actual - approx) / actual|`)
- [ULP Distance](https://en.wikipedia.org/wiki/Unit_in_the_last_place#:~:text=In%20computer%20science%20and%20numerical,of%20accuracy%20in%20numeric%20calculations.)

### Single vs. Double Precision

At the moment, the approximations in this library have been
primarily tested with single-precision floating-point numbers.
It is possible (maybe even likely) that most of the approximations
do not achieve sufficient accuracy for double-precision computations.

### SIMD

These approximations are intended to work for both scalar floating-point
data types, as well as SIMD floating-point data types. At the moment,
the library is set up to be compatible with the [XSIMD library](https://github.com/xtensor-stack/xsimd).
That said, I would like to make it as easy as possible to use this
library with other SIMD libraries (or matrix math libraries), so if
anyone has some suggestions, please let me know!
