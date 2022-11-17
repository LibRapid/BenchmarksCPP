#pragma once

#include <omp.h>
#include <json.hpp> // JSON

// Include the libraries

// ----- LibRapid ----------------
#include <librapid>

// ----- eigen -------------------
#include <Eigen>

// ----- xtensor -----------------
#include <xtensor/xarray.hpp>
#include <xtensor/xio.hpp>
#include <xtensor/xview.hpp>

// Namespace aliases
namespace lrc  = librapid;
namespace json = nlohmann;
using namespace nlohmann::literals;

// Utility functions
bool jsonContains(const json::json &j, const std::string &key);
std::string tableToString(const json::json &table);

std::string formatTime(double time);

struct Bench {
	int64_t samples = 0;
	int64_t iters	= 0;
	double avg		= 0;
	double stddev	= 0;
	double elapsed	= 0;
};

template<typename LAMBDA, typename... Args>
inline Bench timeFunction(const LAMBDA &op, int64_t samples = -1, int64_t iters = -1,
						  double time = -1, Args... vals) {
	if (samples < 1) samples = 10;

	// Call the function to compile any kernels
	op(vals...);

	double loopTime		   = 1e300;
	int64_t itersCompleted = 0;
	if (iters < 1) {
		loopTime	   = 5e9 / (double)samples;
		iters		   = 1e10;
		itersCompleted = 0;
	}

	if (time > 0) {
		loopTime = time * lrc::time::second;
		loopTime /= (double)samples;
	}

	std::vector<double> times;

	double mainStart = lrc::now<lrc::time::nanosecond>();
	for (int64_t sample = 0; sample < samples; ++sample) {
		itersCompleted = 0;
		double start   = lrc::now<lrc::time::nanosecond>();
		while (itersCompleted++ < iters && lrc::now<lrc::time::nanosecond>() - start < loopTime) {
			op(vals...);
		}
		double end = lrc::now<lrc::time::nanosecond>();
		times.emplace_back((end - start) / (double)itersCompleted);
	}
	double mainEnd = lrc::now<lrc::time::nanosecond>();

	// Calculate average (mean) time and standard deviation
	double avg = std::accumulate(times.begin(), times.end(), 0.0) / (double)times.size();
	double stddev =
	  std::sqrt(std::accumulate(times.begin(),
								times.end(),
								0.0,
								[avg](double a, double b) { return a + (b - avg) * (b - avg); }) /
				(double)times.size());
	return {samples, itersCompleted - 1, avg, stddev, mainEnd - mainStart};
}

inline std::string formatBench(const Bench &bench, bool includeIters = true) {
	std::string res = fmt::format("Mean {:>9} ± {:>9}",
								  lrc::formatTime<lrc::time::nanosecond>(bench.avg, "{:.3f}"),
								  lrc::formatTime<lrc::time::nanosecond>(bench.stddev, "{:.3f}"));

	if (includeIters) {
		res +=
		  fmt::format(" after {} samples, each with {} iterations", bench.samples, bench.iters);
	}

	return res;
}

void saveTableToFile(const std::string &filename, const json::json &data);
