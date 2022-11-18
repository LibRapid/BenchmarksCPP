#include <benchmarks/arithmeticPreallocate.hpp>

// Benchmark arithmetic operations
json::json benchmarkArithmeticPreallocate(const json::json &options) {
	json::json results {};

	if (jsonContains(options, "eigen")) {
		fmt::print("============= Testing Eigen =============\n");

		const json::json &config = options["eigen"];

		LIBRAPID_ASSERT(jsonContains(config, "sizes"), "Benchmark sizes not specified");

		json::json sizes  = config["sizes"];
		std::string dtype = jsonContains(config, "dtype") ? config["dtype"] : "f32";
		int64_t iters	  = jsonContains(config, "iters") ? (int64_t)config["iters"] : -1;
		int64_t samples	  = jsonContains(config, "samples") ? (int64_t)config["samples"] : -1;
		double time		  = jsonContains(config, "time") ? (double)config["time"] : -1;
		int64_t threads	  = jsonContains(config, "threads") ? (int64_t)config["threads"] : 1;

		omp_set_num_threads(threads);
		Eigen::setNbThreads(threads);

		for (const auto &size : sizes) {
			auto extent = size.get<std::vector<int64_t>>();
			// Add the size to the results
			std::stringstream stream;
			stream << size;

			Bench bench {};

			// Time the addition
			if (dtype == "f32") {
				Eigen::MatrixXf lhs(extent[0], extent.size() > 1 ? extent[1] : 1);
				Eigen::MatrixXf rhs(extent[0], extent.size() > 1 ? extent[1] : 1);
				Eigen::MatrixXf res(extent[0], extent.size() > 1 ? extent[1] : 1);
				bench = timeFunction([&]() { res = lhs + rhs; }, samples, iters, time);
				fmt::print("Preventing Optimisation: {}\n", res(0, 0));
			} else if (dtype == "f64") {
				Eigen::MatrixXd lhs(extent[0], extent.size() > 1 ? extent[1] : 1);
				Eigen::MatrixXd rhs(extent[0], extent.size() > 1 ? extent[1] : 1);
				Eigen::MatrixXd res(extent[0], extent.size() > 1 ? extent[1] : 1);
				bench = timeFunction([&]() { res = lhs + rhs; }, samples, iters, time);
				fmt::print("Preventing Optimisation: {}\n", res(0, 0));
			} else if (dtype == "int32_t") {
				Eigen::MatrixXi lhs(extent[0], extent.size() > 1 ? extent[1] : 1);
				Eigen::MatrixXi rhs(extent[0], extent.size() > 1 ? extent[1] : 1);
				Eigen::MatrixXi res(extent[0], extent.size() > 1 ? extent[1] : 1);
				bench = timeFunction([&]() { res = lhs + rhs; }, samples, iters, time);
				fmt::print("Preventing Optimisation: {}\n", res(0, 0));
			} else if (dtype == "i64") {
				LIBRAPID_ASSERT(false, "Eigen does not support i64");
			} else {
				LIBRAPID_ASSERT(false, "Invalid dtype");
			}

			results["#1 Eigen"] += formatTime(bench.avg);
			std::cout << "Benchmarked " << size << "\n";
		}
	}

	if (jsonContains(options, "xtensor")) {
		fmt::print("============= Testing XTensor =============\n");

		const json::json &config = options["xtensor"];

		LIBRAPID_ASSERT(jsonContains(config, "sizes"), "Benchmark sizes not specified");

		json::json sizes  = config["sizes"];
		std::string dtype = jsonContains(config, "dtype") ? config["dtype"] : "f32";
		int64_t iters	  = jsonContains(config, "iters") ? (int64_t)config["iters"] : -1;
		int64_t samples	  = jsonContains(config, "samples") ? (int64_t)config["samples"] : -1;
		double time		  = jsonContains(config, "time") ? (double)config["time"] : -1;
		int64_t threads	  = jsonContains(config, "threads") ? (int64_t)config["threads"] : 1;

		omp_set_num_threads(threads);

		for (const auto &size : sizes) {
			auto extent = size.get<std::vector<int64_t>>();
			// Add the size to the results
			std::stringstream stream;
			stream << size;

			Bench bench {};

			// Time the addition
			if (dtype == "f32") {
				xt::xarray<float>::shape_type shape(extent.begin(), extent.end());
				xt::xarray<float> lhs(shape);
				xt::xarray<float> rhs(shape);
				xt::xarray<float> res(shape);
				bench = timeFunction([&]() { res = lhs + rhs; }, samples, iters, time);
				fmt::print("Preventing Optimisation: {}\n", res(0, 0));
			} else if (dtype == "f64") {
				xt::xarray<double>::shape_type shape(extent.begin(), extent.end());
				xt::xarray<double> lhs(shape);
				xt::xarray<double> rhs(shape);
				xt::xarray<double> res(shape);
				bench = timeFunction([&]() { res = lhs + rhs; }, samples, iters, time);
				fmt::print("Preventing Optimisation: {}\n", res(0, 0));
			} else if (dtype == "int32_t") {
				xt::xarray<int>::shape_type shape(extent.begin(), extent.end());
				xt::xarray<int> lhs(shape);
				xt::xarray<int> rhs(shape);
				xt::xarray<int> res(shape);
				bench = timeFunction([&]() { res = lhs + rhs; }, samples, iters, time);
				fmt::print("Preventing Optimisation: {}\n", res(0, 0));
			} else if (dtype == "i64") {
				xt::xarray<long>::shape_type shape(extent.begin(), extent.end());
				xt::xarray<long> lhs(shape);
				xt::xarray<long> rhs(shape);
				xt::xarray<long> res(shape);
				bench = timeFunction([&]() { res = lhs + rhs; }, samples, iters, time);
				fmt::print("Preventing Optimisation: {}\n", res(0, 0));
			} else {
				LIBRAPID_ASSERT(false, "Invalid dtype");
			}

			results["#2 XTensor"] += formatTime(bench.avg);
			std::cout << "Benchmarked " << size << "\n";
		}
	}

	if (jsonContains(options, "librapid")) {
		fmt::print("============= Testing LibRapid =============\n");

		const json::json &config = options["librapid"];

		LIBRAPID_ASSERT(jsonContains(config, "sizes"), "Benchmark sizes not specified");

		json::json sizes  = config["sizes"];
		std::string dtype = jsonContains(config, "dtype") ? config["dtype"] : "f32";
		int64_t iters	  = jsonContains(config, "iters") ? (int64_t)config["iters"] : -1;
		int64_t samples	  = jsonContains(config, "samples") ? (int64_t)config["samples"] : -1;
		double time		  = jsonContains(config, "time") ? (double)config["time"] : -1;
		int64_t threads	  = jsonContains(config, "threads") ? (int64_t)config["threads"] : 1;

		lrc::global::numThreads = threads;

		for (const auto &size : sizes) {
			lrc::Shape shape(size.get<std::vector<int64_t>>());
			// Add the size to the results
			std::stringstream stream;
			stream << size;
			results["#0 Dimensions"] += stream.str();

			Bench bench {};

			// Time the addition
			if (dtype == "f32") {
				lrc::Array<float> lhs(shape);
				lrc::Array<float> rhs(shape);
				lrc::Array<float> res(shape);
				bench = timeFunction([&]() { res = lhs + rhs; }, samples, iters, time);
				fmt::print("Preventing Optimisation: {}\n", res.storage()[0]);
			} else if (dtype == "f64") {
				lrc::Array<double> lhs(shape);
				lrc::Array<double> rhs(shape);
				lrc::Array<double> res(shape);
				bench = timeFunction([&]() { res = lhs + rhs; }, samples, iters, time);
				fmt::print("Preventing Optimisation: {}\n", res.storage()[0]);
			} else if (dtype == "int32_t") {
				lrc::Array<int32_t> lhs(shape);
				lrc::Array<int32_t> rhs(shape);
				lrc::Array<int32_t> res(shape);
				bench = timeFunction([&]() { res = lhs + rhs; }, samples, iters, time);
				fmt::print("Preventing Optimisation: {}\n", res.storage()[0]);
			} else if (dtype == "i64") {
				lrc::Array<int64_t> lhs(shape);
				lrc::Array<int64_t> rhs(shape);
				lrc::Array<int64_t> res(shape);
				LIBRAPID_ASSERT(false, "LibRapid arrays don't currently support int64_t");
				// bench = timeFunction([&]() { res = lhs + rhs; }, samples, iters, time);
				fmt::print("Preventing Optimisation: {}\n", res.storage()[0]);
			} else {
				LIBRAPID_ASSERT(false, "Invalid dtype");
			}

			results["#3 LibRapid"] += formatTime(bench.avg);
			std::cout << "Benchmarked " << size << "\n";
		}
	}

	uint64_t maxSize = lrc::max(
	  results["#1 Eigen"].size(), results["#2 XTensor"].size(), results["#3 LibRapid"].size());

	std::vector<double> librapidTimes, eigenTimes, xtensorTimes;
	for (const auto &time : results["#1 Eigen"]) {
		eigenTimes.push_back(std::stod(time.get<std::string>()));
	}
	for (const auto &time : results["#2 XTensor"]) {
		xtensorTimes.push_back(std::stod(time.get<std::string>()));
	}
	for (const auto &time : results["#3 LibRapid"]) {
		librapidTimes.push_back(std::stod(time.get<std::string>()));
	}

	// Pad the results to fill in empty values
	for (uint64_t i = eigenTimes.size(); i < maxSize; ++i) { eigenTimes.push_back(1e100); }
	for (uint64_t i = xtensorTimes.size(); i < maxSize; ++i) { xtensorTimes.push_back(1e100); }
	for (uint64_t i = librapidTimes.size(); i < maxSize; ++i) { librapidTimes.push_back(1e100); }

	results["#4 Fastest"] = {};
	for (int64_t i = 0; i < librapidTimes.size(); ++i) {
		if (librapidTimes[i] < eigenTimes[i] && librapidTimes[i] < xtensorTimes[i]) {
			results["#4 Fastest"] += "LibRapid";
		} else if (eigenTimes[i] < librapidTimes[i] && eigenTimes[i] < xtensorTimes[i]) {
			results["#4 Fastest"] += "Eigen";
		} else if (xtensorTimes[i] < librapidTimes[i] && xtensorTimes[i] < eigenTimes[i]) {
			results["#4 Fastest"] += "XTensor";
		} else {
			results["#4 Fastest"] += "Tie";
		}
	}

	return results;
}