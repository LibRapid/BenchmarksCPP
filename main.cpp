#include "include/core.hpp"

auto main() -> int {
    std::cout << "Hello, World!" << std::endl;

#ifdef LIBRAPID_HAS_OPENCL
    librapid::configureOpenCL(true);
#endif

    auto tmpSizes = librapid::logspace(4, 5592, 10); // I want to increase this, but the GitHub
                                                     // servers can't handle it

    // Convert to vector to eliminate any issues with LibRapid that may arise
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
