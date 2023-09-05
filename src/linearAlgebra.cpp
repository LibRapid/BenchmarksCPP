#include "include/core.hpp"

namespace bench {
    nanobench::Bench linearAlgebra(const std::vector<int64_t> &sizes) {
        nanobench::Bench benchmarker;

        int64_t minThreads = 1;

#if defined(LIBRAPID_OPTIMISE_SMALL_ARRAYS)
        // If we are optimising for small arrays, then we only want to test with 1 thread since
        // the code is not parallelised
        int64_t maxThreads = 1;
#else
        int64_t maxThreads = std::thread::hardware_concurrency();
#endif // LIBRAPID_OPTIMISE_SMALL_ARRAYS

#if defined(LIBRAPID_OPTIMISE_SMALL_ARRAYS)
        double benchTime = 0.75; // seconds
#else
        double benchTime = 0.1; // seconds
#endif // LIBRAPID_OPTIMISE_SMALL_ARRAYS

        benchmarker.minEpochTime(std::chrono::duration_cast<std::chrono::nanoseconds>(
          std::chrono::milliseconds(static_cast<int64_t>(benchTime * 1000.0))));

        for (int64_t threads = minThreads; threads <= maxThreads; ++threads) {
            for (const auto &size : sizes) {
                // Set num threads
                librapid::setNumThreads(threads);
                Eigen::setNbThreads(threads);

                {
                    // LibRapid Array Transpose
                    librapid::Array<float> a(librapid::Shape({size, size}));
                    librapid::Array<float> b(librapid::Shape({size, size}));

                    benchmarker.run(
                      fmt::format(
                        "LibRapid | Matrix Transpose | {0}x{0} | CPU | {1}_threads", size, threads),
                      [&] {
                          b = librapid::transpose(a);
                          nanobench::doNotOptimizeAway(b);
                      });
                }

                {
                    // LibRapid Matrix Transpose
                    librapid::Matrix<float> a(librapid::MatrixShape({size, size}));
                    librapid::Matrix<float> b(librapid::MatrixShape({size, size}));

                    benchmarker.run(
                      fmt::format(
                        "LibRapid (Matrix) | Matrix Transpose | {0}x{0} | CPU | {1}_threads",
                        size,
                        threads),
                      [&] {
                          b = librapid::transpose(a);
                          nanobench::doNotOptimizeAway(b);
                      });
                }

#ifdef LIBRAPID_HAS_OPENCL
                {
                    // LibRapid Array Transpose
                    librapid::Array<float, librapid::backend::OpenCL> a(
                      librapid::Shape({size, size}));
                    librapid::Array<float, librapid::backend::OpenCL> b(
                      librapid::Shape({size, size}));

                    benchmarker.run(
                      fmt::format("LibRapid | Matrix Transpose | {0}x{0} | OpenCL | {1}_threads",
                                  size,
                                  threads),
                      [&] {
                          b = librapid::transpose(a);
                          nanobench::doNotOptimizeAway(b);
                      });
                }
#endif

#ifdef LIBRAPID_HAS_CUDA
                {
                    // LibRapid Array Transpose
                    librapid::Array<float, librapid::backend::CUDA> a(
                      librapid::Shape({size, size}));
                    librapid::Array<float, librapid::backend::CUDA> b(
                      librapid::Shape({size, size}));

                    benchmarker.run(
                      fmt::format("LibRapid | Matrix Transpose | {0}x{0} | CUDA | {1}_threads",
                                  size,
                                  threads),
                      [&] {
                          b = librapid::transpose(a);
                          nanobench::doNotOptimizeAway(b);
                      });
                }
#endif

                {
                    // Eigen Array Transpose
                    Eigen::Array<float, Eigen::Dynamic, Eigen::Dynamic> a(size, size);
                    Eigen::Array<float, Eigen::Dynamic, Eigen::Dynamic> b(size, size);

                    benchmarker.run(
                      fmt::format(
                        "Eigen | Matrix Transpose | {0}x{0} | CPU | {1}_threads", size, threads),
                      [&] {
                          b = a.transpose();
                          nanobench::doNotOptimizeAway(b);
                      });
                }

                {
                    // XTensor Array Transpose
                    xt::xarray<float> a = xt::zeros<float>({size, size});
                    xt::xarray<float> b = xt::zeros<float>({size, size});

                    benchmarker.run(
                      fmt::format(
                        "XTensor | Matrix Transpose | {0}x{0} | CPU | {1}_threads", size, threads),
                      [&] {
                          b = xt::transpose(a);
                          nanobench::doNotOptimizeAway(b);
                      });
                }
            }
        }

        return benchmarker;
    }
} // namespace bench