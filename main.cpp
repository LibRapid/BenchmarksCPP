#include "include/core.hpp"

auto main() -> int {
    std::cout << "Hello, World!" << std::endl;

    auto trivialOperations = bench::trivialOperations();
    std::ofstream trivialOperationsFile("trivialOperations.csv");
    trivialOperations.render(nanobench::templates::csv(), trivialOperationsFile);

    auto combinedTrivialOperations = bench::combinedTrivialOperations();
    std::ofstream combinedTrivialOperationsFile("combinedTrivialOperations.csv");
    combinedTrivialOperations.render(nanobench::templates::csv(), combinedTrivialOperationsFile);

    return 0;
}
