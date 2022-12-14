#include <benchmarks/core.hpp>
#include <benchmarks/arithmeticPreallocate.hpp>

static const json::json allSizes = {
  // {10, 10},
  // {25, 25},
  // {50, 50},
  // {75, 75},
  // {100, 100},
  // {250, 250},
  // {500, 500},
  // {750, 750},
  // {1000, 1000},
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

static const json::json smallSizes = {
  {10, 10},	  {20, 20},	  {30, 30},	  {40, 40},	  {50, 50},	  {60, 60},	  {70, 70},
  {80, 80},	  {90, 90},	  {100, 100}, {110, 110}, {120, 120}, {130, 130}, {140, 140},
  {150, 150}, {160, 160}, {170, 170}, {180, 180}, {190, 190}, {200, 200}, {210, 210},
  {220, 220}, {230, 230}, {240, 240}, {250, 250},
};

// clang-format off
static const json::json defaultConfig = {
	{"librapid", {
			{"iters", -1},
			{"samples", -1},
			{"time", 0.001},
			{"dtype", "f32"},
			{"threads", 1},
			{"sizes", allSizes}
		}
	},
	{"eigen", {
			{"iters", -1},
			{"samples", -1},
			{"time", 3},
			{"dtype", "f32"},
			{"threads", 1},
			{"sizes", allSizes}
		}
	},
	{"xtensor", {
			{"iters", -1},
			{"samples", -1},
			{"time", 0.001},
			{"dtype", "f32"},
			{"threads", 1},
			{"sizes", allSizes}
		}
	}
};
// clang-format on

json::json arithmetic(const json::json &config) {
	fmt::print("================== MATRIX ARITHMETIC ==================\n");
	auto arithmeticResults = benchmarkArithmeticPreallocate(config);
	return arithmeticResults;
}

template<typename T>
void printArrayThing(const xt::xarray<T> &arr) {
	std::cout << arr << "\n";
}

int main() {
#if defined(LIBRAPID_OPTIMISE_SMALL_ARRAYS)
	fmt::print("Small arrays optimised\n");
#else
	fmt::print("Small arrays not optimised\n");
#endif

#if defined(USING_GITHUB_ACTIONS)
	json::json sizes;
	for (size_t i = 10; i <= 5000; i += 10) { sizes.push_back({i, i}); }
	double time = 2.5;
#else
	json::json sizes;
	for (size_t i = 50; i <= 1000; i += 50) { sizes.push_back({i, i}); }
	double time = 1;
#endif // USING_GITHUB_ACTIONS

	json::json config {{"librapid",
						{{"iters", -1},
						 {"samples", -1},
						 {"time", time},
						 {"dtype", "f32"},
						 {"threads", 1},
						 {"sizes", sizes}}},
					   {"eigen",
						{{"iters", -1},
						 {"samples", -1},
						 {"time", time},
						 {"dtype", "f32"},
						 {"threads", 1},
						 {"sizes", sizes}}},
					   {"xtensor",
						{{"iters", -1},
						 {"samples", -1},
						 {"time", time},
						 {"dtype", "f32"},
						 {"threads", 1},
						 {"sizes", sizes}}}};

	json::json arithmeticResults = arithmetic(config);
	fmt::print("{}\n", tableToString(arithmeticResults));

	saveTableToFile("arithmeticResults.txt", arithmeticResults);

#if defined(LIBRAPID_OPTIMISE_SMALL_ARRAYS)
	fmt::print("Small array optimisation enabled\n");
#endif

	xt::xarray<float> a({{1, 2, 3}, {4, 5, 6}});
	xt::xarray<float> b({{1, 2, 3}, {4, 5, 6}});
	xt::xarray<float> c = a + b;
	c = a + b;
	auto func = a + b;

	return 0;
}
