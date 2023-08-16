#include "include/core.hpp"

namespace bench {
    nanobench::Bench trivialOperations() {
        nanobench::Bench benchmarker;

        int64_t minThreads = 1;

#if defined(LIBRAPID_OPTIMISE_SMALL_ARRAYS)
        // If we are optimising for small arrays, then we only want to test with 1 thread since
        // the code is not parallelised
        int64_t maxThreads = 1;
#else
        int64_t maxThreads = std::thread::hardware_concurrency();
#endif // LIBRAPID_OPTIMISE_SMALL_ARRAYS

        int64_t minSize = 1ll << 3;  // 8
        int64_t maxSize = 1ll << 14; // 16384

#if defined(LIBRAPID_OPTIMISE_SMALL_ARRAYS)
        double benchTime = 0.75; // seconds
#else
        double benchTime = 0.1; // seconds
#endif // LIBRAPID_OPTIMISE_SMALL_ARRAYS

        benchmarker.minEpochTime(std::chrono::duration_cast<std::chrono::nanoseconds>(
          std::chrono::milliseconds(static_cast<int64_t>(benchTime * 1000.0))));

        for (int64_t threads = minThreads; threads <= maxThreads; ++threads) {
            for (int64_t size = minSize; size <= maxSize; size <<= 1) {
                // Set num threads
                librapid::setNumThreads(threads);
                Eigen::setNbThreads(threads);

                {
                    // LibRapid Array Addition
                    librapid::Array<float> a(librapid::Shape({size, size}));
                    librapid::Array<float> b(librapid::Shape({size, size}));
                    librapid::Array<float> c(librapid::Shape({size, size}));

                    benchmarker.run(fmt::format("LibRapid | Array Addition | {0}x{0} | {1}_threads",
                                                size,
                                                threads),
                                    [&] {
                                        c = a + b;
                                        nanobench::doNotOptimizeAway(c);
                                    });
                }

                {
                    // Eigen Array Addition
                    Eigen::Array<float, Eigen::Dynamic, Eigen::Dynamic> a(size, size);
                    Eigen::Array<float, Eigen::Dynamic, Eigen::Dynamic> b(size, size);
                    Eigen::Array<float, Eigen::Dynamic, Eigen::Dynamic> c(size, size);

                    benchmarker.run(
                      fmt::format("Eigen | Array Addition | {0}x{0} | {1}_threads", size, threads),
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

                    benchmarker.run(fmt::format("XTensor | Array Addition | {0}x{0} | {1}_threads",
                                                size,
                                                threads),
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