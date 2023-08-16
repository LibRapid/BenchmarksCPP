#include "include/core.hpp"

auto main() -> int {
    std::cout << "Hello, World!" << std::endl;

    // Warm up CPU
    librapid::Array<float> warmup(librapid::Shape({2048, 2048}), 0.01);
    for (int i = 0; i < 1000; ++i) { warmup += warmup; }

    auto trivialOperations = bench::trivialOperations();
    std::ofstream trivialOperationsFile("trivialOperations.csv");
    trivialOperations.render(nanobench::templates::csv(), trivialOperationsFile);

    auto combinedTrivialOperations = bench::combinedTrivialOperations();
    std::ofstream combinedTrivialOperationsFile("combinedTrivialOperations.csv");
    combinedTrivialOperations.render(nanobench::templates::csv(), combinedTrivialOperationsFile);

    return 0;
}
