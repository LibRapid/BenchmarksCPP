#include "include/core.hpp"

namespace bench {
    nanobench::Bench trivialOperations(const std::vector<int64_t> &sizes) {
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
        double benchTime = 0.05; // 0.75; // seconds
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
                    // LibRapid Array Addition
                    librapid::Array<float> a(librapid::Shape({size, size}));
                    librapid::Array<float> b(librapid::Shape({size, size}));
                    librapid::Array<float> c(librapid::Shape({size, size}));

                    benchmarker.run(
                      fmt::format(
                        "LibRapid | Array Addition | {0}x{0} | CPU | {1}_threads", size, threads),
                      [&] {
                          c = a + b;
                          nanobench::doNotOptimizeAway(c);
                      });
                }

                {
                    // LibRapid Array Addition
                    librapid::Matrix<float> a(librapid::MatrixShape({size, size}));
                    librapid::Matrix<float> b(librapid::MatrixShape({size, size}));
                    librapid::Matrix<float> c(librapid::MatrixShape({size, size}));

                    benchmarker.run(
                      fmt::format(
                        "LibRapid (Matrix) | Array Addition | {0}x{0} | CPU | {1}_threads",
                        size,
                        threads),
                      [&] {
                          c = a + b;
                          nanobench::doNotOptimizeAway(c);
                      });
                }

#ifdef LIBRAPID_HAS_OPENCL
                {
                    // LibRapid Array Addition
                    librapid::Array<float, librapid::backend::OpenCL> a(
                      librapid::Shape({size, size}));
                    librapid::Array<float, librapid::backend::OpenCL> b(
                      librapid::Shape({size, size}));
                    librapid::Array<float, librapid::backend::OpenCL> c(
                      librapid::Shape({size, size}));

                    benchmarker.run(
                      fmt::format("LibRapid | Array Addition | {0}x{0} | OpenCL | {1}_threads",
                                  size,
                                  threads),
                      [&] {
                          c = a + b;
                          nanobench::doNotOptimizeAway(c);
                      });
                }
#endif

#ifdef LIBRAPID_HAS_CUDA
                {
                    // LibRapid Array Addition
                    librapid::Array<float, librapid::backend::CUDA> a(
                      librapid::Shape({size, size}));
                    librapid::Array<float, librapid::backend::CUDA> b(
                      librapid::Shape({size, size}));
                    librapid::Array<float, librapid::backend::CUDA> c(
                      librapid::Shape({size, size}));

                    benchmarker.run(
                      fmt::format(
                        "LibRapid | Array Addition | {0}x{0} | CUDA | {1}_threads", size, threads),
                      [&] {
                          c = a + b;
                          nanobench::doNotOptimizeAway(c);
                      });
                }
#endif

                {
                    // Eigen Array Addition
                    Eigen::Array<float, Eigen::Dynamic, Eigen::Dynamic> a(size, size);
                    Eigen::Array<float, Eigen::Dynamic, Eigen::Dynamic> b(size, size);
                    Eigen::Array<float, Eigen::Dynamic, Eigen::Dynamic> c(size, size);

                    benchmarker.run(
                      fmt::format(
                        "Eigen | Array Addition | {0}x{0} | CPU | {1}_threads", size, threads),
                      [&] {
                          c = a + b;
                          nanobench::doNotOptimizeAway(c);
                      });
                }

                {
                    // XTensor Array Addition
                    xt::xarray<float> a = xt::zeros<float>({size, size});
                    xt::xarray<float> b = xt::zeros<float>({size, size});
                    xt::xarray<float> c = xt::zeros<float>({size, size});

                    benchmarker.run(
                      fmt::format(
                        "XTensor | Array Addition | {0}x{0} | CPU | {1}_threads", size, threads),
                      [&] {
                          c = a + b;
                          nanobench::doNotOptimizeAway(c);
                      });
                }
            }
        }

        return benchmarker;
    }
} // namespace bench