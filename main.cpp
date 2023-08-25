#include "include/core.hpp"

auto main() -> int {
    std::cout << "Hello, World!" << std::endl;

    // Warm up CPU
    // librapid::Array<float> warmup(librapid::Shape({2048, 2048}), 0.01);
    // for (int i = 0; i < 10000; ++i) { warmup += warmup; }

    // auto trivialOperations = bench::trivialOperations();
    // std::ofstream trivialOperationsFile("trivialOperations.csv");
    // trivialOperations.render(nanobench::templates::csv(), trivialOperationsFile);

    // auto combinedTrivialOperations = bench::combinedTrivialOperations();
    // std::ofstream combinedTrivialOperationsFile("combinedTrivialOperations.csv");
    // combinedTrivialOperations.render(nanobench::templates::csv(), combinedTrivialOperationsFile);

    auto linearAlgebra = bench::linearAlgebra();
    std::ofstream linearAlgebraFile("linearAlgebra.csv");
    linearAlgebra.render(nanobench::templates::csv(), linearAlgebraFile);

    // auto sanityCheckA = librapid::Array<float>::fromData({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}});
    // auto sanityCheckB = librapid::Array<float>::fromData({{1, 2, 3}, {4, 5, 6}, {7, 8, 9}});
    // auto sanityCheckC = librapid::zeros<float>({3, 3});
    // auto tmp          = sanityCheckA + sanityCheckB;
    // sanityCheckC      = tmp;
    // fmt::print("Sanity Check: {}\n", sanityCheckC);

    double librapidTime, librapidMatrixTime, eigenTime, fastestTime;
    size_t iterations = 200'000'000;
    librapid::Shape shape({8, 8});

    {
        fmt::print("LibRapid Addition\n");
        librapid::Array<float> a(shape);
        librapid::Array<float> b(shape);
        librapid::Array<float> c(shape);
        librapid::Array<float> d(shape);
        librapid::Array<float> e(shape);
        librapid::Array<float> f(shape);
        librapid::Array<float> g(shape);
        librapid::Array<float> h(shape);
        librapid::Array<float> i(shape);

        double start = librapid::now();
        for (uint64_t iter = 0; iter < iterations; ++iter) {
            i = (a + b) * (c + d) + (e + f) * (g + h);
        }
        double end = librapid::now();
        fmt::print("Elapsed: {}\n", librapid::formatTime(end - start));
        librapidTime = end - start;
    }

    {
        fmt::print("LibRapid Matrix Addition\n");
        librapid::Matrix<float> a(librapid::MatrixShape({shape[0], shape[1]}));
        librapid::Matrix<float> b(librapid::MatrixShape({shape[0], shape[1]}));
        librapid::Matrix<float> c(librapid::MatrixShape({shape[0], shape[1]}));
        librapid::Matrix<float> d(librapid::MatrixShape({shape[0], shape[1]}));
        librapid::Matrix<float> e(librapid::MatrixShape({shape[0], shape[1]}));
        librapid::Matrix<float> f(librapid::MatrixShape({shape[0], shape[1]}));
        librapid::Matrix<float> g(librapid::MatrixShape({shape[0], shape[1]}));
        librapid::Matrix<float> h(librapid::MatrixShape({shape[0], shape[1]}));
        librapid::Matrix<float> i(librapid::MatrixShape({shape[0], shape[1]}));

        double start = librapid::now();
        for (uint64_t iter = 0; iter < iterations; ++iter) {
            i = (a + b) * (c + d) + (e + f) * (g + h);
        }
        double end = librapid::now();
        fmt::print("Elapsed: {}\n", librapid::formatTime(end - start));
        librapidMatrixTime = end - start;
    }

    // {
    //     fmt::print("Eigen Addition\n");
    //     Eigen::MatrixXf a(shape[0], shape[1]);
    //     Eigen::MatrixXf b(shape[0], shape[1]);
    //     Eigen::MatrixXf c(shape[0], shape[1]);
    //     Eigen::MatrixXf d(shape[0], shape[1]);
    //     Eigen::MatrixXf e(shape[0], shape[1]);
    //     Eigen::MatrixXf f(shape[0], shape[1]);
    //     Eigen::MatrixXf g(shape[0], shape[1]);
    //     Eigen::MatrixXf h(shape[0], shape[1]);
    //     Eigen::MatrixXf i(shape[0], shape[1]);

    //     double start = librapid::now();
    //     for (uint64_t iter = 0; iter < iterations; ++iter) {
    //         i = (a + b) * (c + d) + (e + f) * (g + h);
    //     }
    //     double end = librapid::now();
    //     fmt::print("Elapsed: {}\n", librapid::formatTime(end - start));
    //     eigenTime = end - start;
    // }

    {
        fmt::print("Hard-Coded Addition\n");
        auto testMemoryA =
          LIBRAPID_ASSUME_ALIGNED(librapid::detail::safeAllocate<float>(shape.size()));
        auto testMemoryB =
          LIBRAPID_ASSUME_ALIGNED(librapid::detail::safeAllocate<float>(shape.size()));
        auto testMemoryC =
          LIBRAPID_ASSUME_ALIGNED(librapid::detail::safeAllocate<float>(shape.size()));
        auto testMemoryD =
          LIBRAPID_ASSUME_ALIGNED(librapid::detail::safeAllocate<float>(shape.size()));
        auto testMemoryE =
          LIBRAPID_ASSUME_ALIGNED(librapid::detail::safeAllocate<float>(shape.size()));
        auto testMemoryF =
          LIBRAPID_ASSUME_ALIGNED(librapid::detail::safeAllocate<float>(shape.size()));
        auto testMemoryG =
          LIBRAPID_ASSUME_ALIGNED(librapid::detail::safeAllocate<float>(shape.size()));
        auto testMemoryH =
          LIBRAPID_ASSUME_ALIGNED(librapid::detail::safeAllocate<float>(shape.size()));
        auto testMemoryI =
          LIBRAPID_ASSUME_ALIGNED(librapid::detail::safeAllocate<float>(shape.size()));

        double start = librapid::now();
        for (uint64_t i = 0; i < iterations; ++i) {
            // Perform addition
            using Packet                   = librapid::typetraits::TypeInfo<float>::Packet;
            constexpr uint64_t packetWidth = librapid::typetraits::TypeInfo<float>::packetWidth;
            uint64_t size                  = shape.size();

            for (uint64_t j = 0; j < size; j += packetWidth) {
                Packet packetA = Packet::load_aligned(testMemoryA + j);
                Packet packetB = Packet::load_aligned(testMemoryB + j);
                Packet packetC = Packet::load_aligned(testMemoryC + j);
                Packet packetD = Packet::load_aligned(testMemoryD + j);
                Packet packetE = Packet::load_aligned(testMemoryE + j);
                Packet packetF = Packet::load_aligned(testMemoryF + j);
                Packet packetG = Packet::load_aligned(testMemoryG + j);
                Packet packetH = Packet::load_aligned(testMemoryH + j);

                Packet packetI = (packetA + packetB) * (packetC + packetD) +
                                 (packetE + packetF) * (packetG + packetH);
                packetI.store_aligned(testMemoryI + j);
            }
        }
        double end = librapid::now();
        fmt::print("Elapsed: {}\n", librapid::formatTime(end - start));
        fastestTime = end - start;

        librapid::detail::safeDeallocate(testMemoryA, shape.size());
        librapid::detail::safeDeallocate(testMemoryB, shape.size());
        librapid::detail::safeDeallocate(testMemoryC, shape.size());
        librapid::detail::safeDeallocate(testMemoryD, shape.size());
        librapid::detail::safeDeallocate(testMemoryE, shape.size());
        librapid::detail::safeDeallocate(testMemoryF, shape.size());
        librapid::detail::safeDeallocate(testMemoryG, shape.size());
        librapid::detail::safeDeallocate(testMemoryH, shape.size());
        librapid::detail::safeDeallocate(testMemoryI, shape.size());
    }

    fmt::print("\n");

    fmt::print("{:<20} is {} than the fastest implementation\n",
               fmt::format(fmt::fg(fmt::color::green), "LibRapid"),
               fmt::format(fmt::fg(fmt::color::orange) | fmt::emphasis::bold,
                           "{:.3g}% slower",
                           (librapidTime / fastestTime - 1) * 100));
    fmt::print("{:<20} is {} than the fastest implementation\n",
               fmt::format(fmt::fg(fmt::color::green), "LibRapid (Matrix)"),
               fmt::format(fmt::fg(fmt::color::orange) | fmt::emphasis::bold,
                           "{:.3g}% slower",
                           (librapidMatrixTime / fastestTime - 1) * 100));
    fmt::print("{:<20} is {} than the fastest implementation\n",
               fmt::format(fmt::fg(fmt::color::green), "Eigen"),
               fmt::format(fmt::fg(fmt::color::orange) | fmt::emphasis::bold,
                           "{:.3g}% slower",
                           (eigenTime / fastestTime - 1) * 100));

    return 0;
}
