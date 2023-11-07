#include <omp.h>

// Include the libraries

// ----- LibRapid ----------------
#include <librapid>

// ----- eigen -------------------
#include <Eigen>

// ----- xtensor -----------------
#include <xtensor/xarray.hpp>
#include <xtensor/xio.hpp>
#include <xtensor/xview.hpp>

#include <nanobench.h>

#include <fstream>

using namespace ankerl;

#include "config.hpp"
#include "trivialOperations.hpp"
#include "combinedTrivialOperations.hpp"
#include "linearAlgebra.hpp"
