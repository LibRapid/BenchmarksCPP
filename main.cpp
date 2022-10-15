#include <benchmarks/core.hpp>
#include <benchmarks/arithmeticPreallocate.hpp>
#include <benchmarks/transpose.hpp>

static const json::json allSizes = {
  {10, 10},
  {25, 25},
  {50, 50},
  {75, 75},
  {100, 100},
  {250, 250},
  {500, 500},
  {750, 750},
  {1000, 1000},
  // {1250, 1250},
  // {2500, 2500},
  // {5000, 5000},
  // {7500, 7500},
  // {10000, 10000},
  // {12500, 12500},
  // {15000, 15000},
  // {17500, 17500},
  // {20000, 20000},
};

// clang-format off
static const json::json defaultConfig = {
	{"librapid", {
			{"iters", -1},
			{"samples", -1},
			{"time", 1},
			{"dtype", "f32"},
			{"threads", 2},
			{"sizes", allSizes}
		}
	},
	{"eigen", {
			{"iters", -1},
			{"samples", -1},
			{"time", 1},
			{"dtype", "f32"},
			{"threads", 2},
			{"sizes", allSizes}
		}
	},
	{"xtensor", {
			{"iters", -1},
			{"samples", -1},
			{"time", 1},
			{"dtype", "f32"},
			{"threads", 2},
			{"sizes", allSizes}
		}
	}
};
// clang-format on

void arithmetic(const json::json &config) {
	fmt::print("================== MATRIX ARITHMETIC ==================\n");
	auto arithmeticResults = benchmarkArithmeticPreallocate(config);
	fmt::print("{}\n", tableToString(arithmeticResults));
}

void transpose(const json::json &config) {
	auto transposeResults = benchmarkTranspose(config);
	fmt::print("================== MATRIX TRANSPOSE ==================\n");
	fmt::print("{}\n", tableToString(transposeResults));
}

int main() {
	arithmetic(defaultConfig);
	transpose(defaultConfig);
	return 0;
}
