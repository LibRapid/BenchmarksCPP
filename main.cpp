#include "include/core.hpp"

auto main() -> int {
    std::cout << "Hello, World!" << std::endl;

    auto tmpSizes = librapid::logspace(4, 5590.2, 10);
    std::vector<int64_t> sizes(tmpSizes.storage().begin(), tmpSizes.storage().end());

    auto trivialOperations = bench::trivialOperations(sizes);
    std::ofstream trivialOperationsFile("trivialOperations.csv");
    trivialOperations.render(nanobench::templates::csv(), trivialOperationsFile);

    auto combinedTrivialOperations = bench::combinedTrivialOperations(sizes);
    std::ofstream combinedTrivialOperationsFile("combinedTrivialOperations.csv");
    combinedTrivialOperations.render(nanobench::templates::csv(), combinedTrivialOperationsFile);

    auto linearAlgebra = bench::linearAlgebra(sizes);
    std::ofstream linearAlgebraFile("linearAlgebra.csv");
    linearAlgebra.render(nanobench::templates::csv(), linearAlgebraFile);

    return 0;
}
