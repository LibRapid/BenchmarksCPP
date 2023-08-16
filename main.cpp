#include "include/core.hpp"

auto main() -> int {
    std::cout << "Hello, World!" << std::endl;

    // Warm up CPU
    // librapid::Array<float> warmup(librapid::Shape({2048, 2048}), 0.01);
    // for (int i = 0; i < 1000; ++i) { warmup += warmup; }

    // auto trivialOperations = bench::trivialOperations();
    // std::ofstream trivialOperationsFile("trivialOperations.csv");
    // trivialOperations.render(nanobench::templates::csv(), trivialOperationsFile);

    // auto combinedTrivialOperations = bench::combinedTrivialOperations();
    // std::ofstream combinedTrivialOperationsFile("combinedTrivialOperations.csv");
    // combinedTrivialOperations.render(nanobench::templates::csv(), combinedTrivialOperationsFile);

    auto sanityCheckA = librapid::Array<float>::fromData({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}});
    auto sanityCheckB = librapid::Array<float>::fromData({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}});
    auto sanityCheckC = librapid::zeros<float>({3, 3});
    auto tmp = sanityCheckA + sanityCheckB;
    sanityCheckC = tmp;
    fmt::print("Sanity Check: {}\n", sanityCheckC);

    double librapidTime, eigenTime, fastestTime;

    {
        fmt::print("LibRapid Addition\n");
        librapid::Array<float> a(librapid::Shape({64, 64}));
        librapid::Array<float> b(librapid::Shape({64, 64}));
        librapid::Array<float> c(librapid::Shape({64, 64}));

        double start = librapid::now();
        for (uint64_t i = 0; i < 5'000'000; ++i) { c = a + b; }
        double end = librapid::now();
        fmt::print("Elapsed: {}\n", librapid::formatTime(end - start));
        librapidTime = end - start;
    }

    {
        fmt::print("Eigen Addition\n");
        Eigen::MatrixXf a(64, 64);
        Eigen::MatrixXf b(64, 64);
        Eigen::MatrixXf c(64, 64);

        double start = librapid::now();
        for (uint64_t i = 0; i < 5'000'000; ++i) { c = a + b; }
        double end = librapid::now();
        fmt::print("Elapsed: {}\n", librapid::formatTime(end - start));
        eigenTime = end - start;
    }

    {
        fmt::print("Hard-Coded Addition\n");
        auto *testMemoryA = librapid::detail::safeAllocate<float>(64 * 64);
        auto *testMemoryB = librapid::detail::safeAllocate<float>(64 * 64);
        auto *testMemoryC = librapid::detail::safeAllocate<float>(64 * 64);

        double start = librapid::now();
        for (uint64_t i = 0; i < 5'000'000; ++i) {
            // Perform addition
            using Packet                   = librapid::typetraits::TypeInfo<float>::Packet;
            constexpr uint64_t packetWidth = librapid::typetraits::TypeInfo<float>::packetWidth;
            const uint64_t packetCount     = 64 * 64 / packetWidth;

            for (uint64_t j = 0; j < 64 * 64; j += packetWidth) {
                Packet packetA = Packet::load_aligned(testMemoryA + j);
                Packet packetB = Packet::load_aligned(testMemoryB + j);
                Packet packetC = packetA + packetB;
                packetC.store_aligned(testMemoryC + j);
            }
        }
        double end = librapid::now();
        fmt::print("Elapsed: {}\n", librapid::formatTime(end - start));

        librapid::detail::safeDeallocate(testMemoryA, 64 * 64);
        librapid::detail::safeDeallocate(testMemoryB, 64 * 64);
        librapid::detail::safeDeallocate(testMemoryC, 64 * 64);
        fastestTime = end - start;
    }

    fmt::print("\n");

    fmt::print("{} is {} than the fastest implementation\n",
               fmt::format(fmt::fg(fmt::color::green), "LibRapid"),
               fmt::format(fmt::fg(fmt::color::orange) | fmt::emphasis::bold,
                           "{:.3g}% slower",
                           (librapidTime / fastestTime - 1) * 100));
    fmt::print("{} is {} than the fastest implementation\n",
                fmt::format(fmt::fg(fmt::color::green), "Eigen"),
                fmt::format(fmt::fg(fmt::color::orange) | fmt::emphasis::bold,
                            "{:.3g}% slower",
                            (eigenTime / fastestTime - 1) * 100));

    return 0;
}
