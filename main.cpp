#include "include/core.hpp"

auto main() -> int {
    std::cout << "Hello, World!" << std::endl;

    auto trivialOperations = bench::trivialOperations();
    std::ofstream trivialOperationsFile("trivialOperations.csv");
    trivialOperations.render(nanobench::templates::csv(), trivialOperationsFile);

    auto combinedTrivialOperations = bench::combinedTrivialOperations();
    std::ofstream combinedTrivialOperationsFile("combinedTrivialOperations.csv");
    combinedTrivialOperations.render(nanobench::templates::csv(), combinedTrivialOperationsFile);

    auto linearAlgebra = bench::linearAlgebra();
    std::ofstream linearAlgebraFile("linearAlgebra.csv");
    linearAlgebra.render(nanobench::templates::csv(), linearAlgebraFile);

    return 0;
}
