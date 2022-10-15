#include <benchmarks/arithmeticPreallocate.hpp>

int main() {
	json::json testTable {
	  {"Column 1", {"A", "B", "C", "E", "F"}},
	  {"Column 2", {1, 2, "3", 123, 456}},
	  {"Column 3", {9, 8, 7, 2, 789}},
	  {"Column 4", {"X", "Y", "Z", "ABC", 120}},
	};

	fmt::print("{}\n", tableToString(testTable));

	json::json arithmeticSizesAll = {
	  {10, 10},
	  {25, 25},
	  {50, 50},
	  {75, 75},
	  {100, 100},
	  {250, 250},
	  {500, 500},
	  {750, 750},
	  {1000, 1000},
	  {1250, 1250},
	  {2500, 2500},
	  {5000, 5000},
	  {7500, 7500},
	  {10000, 10000},
	  {12500, 12500},
	  {15000, 15000},
	  {17500, 17500},
	  {20000, 20000},
	};

	json::json arithmeticSizesLowHalf = {
	  {10, 10},
	  {25, 25},
	  {50, 50},
	  {75, 75},
	  {100, 100},
	  {250, 250},
	  {500, 500},
	  {750, 750},
	  {1000, 1000},
	  {1250, 1250},
	  {2500, 2500},
	  {5000, 5000},
	  {10000, 10000},
	  {12500, 12500},
	};

	json::json arithmeticSizesHighHalf = {
	  {500, 500},
	  {750, 750},
	  {1000, 1000},
	  {1250, 1250},
	  {2500, 2500},
	  {5000, 5000},
	  {7500, 7500},
	  {10000, 10000},
	  {12500, 12500},
	  {15000, 15000},
	  {17500, 17500},
	  {20000, 20000},
	};

	// clang-format off

	json::json config {
		{"librapid", {
				{"iters", -1},
				{"samples", -1},
				{"time", 1},
				{"dtype", "f32"},
				{"threads", 8},
				{"sizes", arithmeticSizesAll}
			}
		},
		{"eigen", {
				{"iters", -1},
				{"samples", -1},
				{"time", 1},
				{"dtype", "f32"},
				{"threads", 8},
				{"sizes", arithmeticSizesLowHalf}
			}
		},
		{"xtensor", {
				{"iters", -1},
				{"samples", -1},
				{"time", 1},
				{"dtype", "f32"},
				{"threads", 8},
				{"sizes", arithmeticSizesLowHalf}
			}
		}
	};

	// clang-format on

	auto results = benchmarkArithmeticPreallocate(config);

	fmt::print("{}\n", tableToString(results));

	return 0;
}
