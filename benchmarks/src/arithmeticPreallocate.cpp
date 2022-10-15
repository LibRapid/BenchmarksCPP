#include <benchmarks/arithmeticPreallocate.hpp>

// Benchmark arithmetic operations
json::json benchmarkArithmeticPreallocate(const json::json &options) {
	json::json results {
	  // {"#0 Dimensions", {}},
	  // {"#1 LibRapid (ms)", {}},
	  // {"#2 Eigen (ms)", {}},
	  // {"#3 XTensor (ms)", {}},
	};

	if (jsonContains(options, "librapid")) {
		fmt::print("============= Testing LibRapid =============\n");

		const json::json &config = options["librapid"];

		LR_ASSERT(jsonContains(config, "sizes"), "Benchmark sizes not specified");

		json::json sizes  = config["sizes"];
		std::string dtype = jsonContains(config, "dtype") ? config["dtype"] : "f32";
		lrc::i64 iters	  = jsonContains(config, "iters") ? (lrc::i64)config["iters"] : -1;
		lrc::i64 samples  = jsonContains(config, "samples") ? (lrc::i64)config["samples"] : -1;
		double time		  = jsonContains(config, "time") ? (double)config["time"] : -1;
		lrc::i64 threads  = jsonContains(config, "threads") ? (lrc::i64)config["threads"] : 1;

		lrc::setNumThreads(threads);

		for (const auto &size : sizes) {
			lrc::Extent extent(size.get<std::vector<lrc::i64>>());
			// Add the size to the results
			std::stringstream stream;
			stream << size;
			results["#0 Dimensions"] += stream.str();

			lrc::Bench bench {};

			// Time the addition
			if (dtype == "f32") {
				lrc::Array<float> lhs(extent);
				lrc::Array<float> rhs(extent);
				lrc::Array<float> res(extent);
				bench = lrc::timeFunction([&]() { res = lhs + rhs; }, samples, iters, time);
			} else if (dtype == "f64") {
				lrc::Array<double> lhs(extent);
				lrc::Array<double> rhs(extent);
				lrc::Array<double> res(extent);
				bench = lrc::timeFunction([&]() { res = lhs + rhs; }, samples, iters, time);
			} else if (dtype == "i32") {
				lrc::Array<lrc::i32> lhs(extent);
				lrc::Array<lrc::i32> rhs(extent);
				lrc::Array<lrc::i32> res(extent);
				bench = lrc::timeFunction([&]() { res = lhs + rhs; }, samples, iters, time);
			} else if (dtype == "i64") {
				lrc::Array<lrc::i64> lhs(extent);
				lrc::Array<lrc::i64> rhs(extent);
				lrc::Array<lrc::i64> res(extent);
				bench = lrc::timeFunction([&]() { res = lhs + rhs; }, samples, iters, time);
			} else {
				LR_ASSERT(false, "Invalid dtype");
			}

			results["#1 LibRapid (ms)"] += formatTime(bench.avg / 1000000);
			std::cout << "Benchmarked " << size << "\n";
		}
	}

	if (jsonContains(options, "eigen")) {
		fmt::print("============= Testing Eigen =============\n");

		const json::json &config = options["eigen"];

		LR_ASSERT(jsonContains(config, "sizes"), "Benchmark sizes not specified");

		json::json sizes  = config["sizes"];
		std::string dtype = jsonContains(config, "dtype") ? config["dtype"] : "f32";
		lrc::i64 iters	  = jsonContains(config, "iters") ? (lrc::i64)config["iters"] : -1;
		lrc::i64 samples  = jsonContains(config, "samples") ? (lrc::i64)config["samples"] : -1;
		double time		  = jsonContains(config, "time") ? (double)config["time"] : -1;
		lrc::i64 threads  = jsonContains(config, "threads") ? (lrc::i64)config["threads"] : 1;

		omp_set_num_threads(threads);
		Eigen::setNbThreads(threads);

		for (const auto &size : sizes) {
			auto extent = size.get<std::vector<lrc::i64>>();
			// Add the size to the results
			std::stringstream stream;
			stream << size;

			lrc::Bench bench {};

			// Time the addition
			if (dtype == "f32") {
				Eigen::MatrixXf lhs(extent[0], extent.size() > 1 ? extent[1] : 1);
				Eigen::MatrixXf rhs(extent[0], extent.size() > 1 ? extent[1] : 1);
				Eigen::MatrixXf res(extent[0], extent.size() > 1 ? extent[1] : 1);
				bench = lrc::timeFunction([&]() { res = lhs + rhs; }, samples, iters, time);
			} else if (dtype == "f64") {
				Eigen::MatrixXd lhs(extent[0], extent.size() > 1 ? extent[1] : 1);
				Eigen::MatrixXd rhs(extent[0], extent.size() > 1 ? extent[1] : 1);
				Eigen::MatrixXd res(extent[0], extent.size() > 1 ? extent[1] : 1);
				bench = lrc::timeFunction([&]() { res = lhs + rhs; }, samples, iters, time);
			} else if (dtype == "i32") {
				Eigen::MatrixXi lhs(extent[0], extent.size() > 1 ? extent[1] : 1);
				Eigen::MatrixXi rhs(extent[0], extent.size() > 1 ? extent[1] : 1);
				Eigen::MatrixXi res(extent[0], extent.size() > 1 ? extent[1] : 1);
				bench = lrc::timeFunction([&]() { res = lhs + rhs; }, samples, iters, time);
			} else if (dtype == "i64") {
				LR_ASSERT(false, "Eigen does not support i64");
			} else {
				LR_ASSERT(false, "Invalid dtype");
			}

			results["#2 Eigen (ms)"] += formatTime(bench.avg / 1000000);
			std::cout << "Benchmarked " << size << "\n";
		}
	}

	if (jsonContains(options, "xtensor")) {
		fmt::print("============= Testing XTensor =============\n");

		const json::json &config = options["xtensor"];

		LR_ASSERT(jsonContains(config, "sizes"), "Benchmark sizes not specified");

		json::json sizes  = config["sizes"];
		std::string dtype = jsonContains(config, "dtype") ? config["dtype"] : "f32";
		lrc::i64 iters	  = jsonContains(config, "iters") ? (lrc::i64)config["iters"] : -1;
		lrc::i64 samples  = jsonContains(config, "samples") ? (lrc::i64)config["samples"] : -1;
		double time		  = jsonContains(config, "time") ? (double)config["time"] : -1;
		lrc::i64 threads  = jsonContains(config, "threads") ? (lrc::i64)config["threads"] : 1;

		omp_set_num_threads(threads);

		for (const auto &size : sizes) {
			auto extent = size.get<std::vector<lrc::i64>>();
			// Add the size to the results
			std::stringstream stream;
			stream << size;

			lrc::Bench bench {};

			// Time the addition
			if (dtype == "f32") {
				xt::xarray<float>::shape_type shape(extent.begin(), extent.end());
				xt::xarray<float> lhs(shape);
				xt::xarray<float> rhs(shape);
				xt::xarray<float> res(shape);
				bench = lrc::timeFunction([&]() { res = lhs + rhs; }, samples, iters, time);
			} else if (dtype == "f64") {
				xt::xarray<double>::shape_type shape(extent.begin(), extent.end());
				xt::xarray<double> lhs(shape);
				xt::xarray<double> rhs(shape);
				xt::xarray<double> res(shape);
				bench = lrc::timeFunction([&]() { res = lhs + rhs; }, samples, iters, time);
			} else if (dtype == "i32") {
				xt::xarray<int>::shape_type shape(extent.begin(), extent.end());
				xt::xarray<int> lhs(shape);
				xt::xarray<int> rhs(shape);
				xt::xarray<int> res(shape);
				bench = lrc::timeFunction([&]() { res = lhs + rhs; }, samples, iters, time);
			} else if (dtype == "i64") {
				xt::xarray<long>::shape_type shape(extent.begin(), extent.end());
				xt::xarray<long> lhs(shape);
				xt::xarray<long> rhs(shape);
				xt::xarray<long> res(shape);
				bench = lrc::timeFunction([&]() { res = lhs + rhs; }, samples, iters, time);
			} else {
				LR_ASSERT(false, "Invalid dtype");
			}

			results["#3 XTensor (ms)"] += formatTime(bench.avg / 1000000);
			std::cout << "Benchmarked " << size << "\n";
		}
	}

	lrc::ui64 maxSize = lrc::max(results["#1 LibRapid (ms)"].size(),
								 results["#2 Eigen (ms)"].size(),
								 results["#3 XTensor (ms)"].size());

	std::vector<double> librapidTimes, eigenTimes, xtensorTimes;
	for (const auto &time : results["#1 LibRapid (ms)"]) {
		librapidTimes.push_back(std::stod(time.get<std::string>()));
	}
	for (const auto &time : results["#2 Eigen (ms)"]) {
		eigenTimes.push_back(std::stod(time.get<std::string>()));
	}
	for (const auto &time : results["#3 XTensor (ms)"]) {
		xtensorTimes.push_back(std::stod(time.get<std::string>()));
	}

	// Pad the results to fill in empty values
	for (lrc::ui64 i = librapidTimes.size(); i < maxSize; ++i) { librapidTimes.push_back(1e100); }
	for (lrc::ui64 i = eigenTimes.size(); i < maxSize; ++i) { eigenTimes.push_back(1e100); }
	for (lrc::ui64 i = xtensorTimes.size(); i < maxSize; ++i) { xtensorTimes.push_back(1e100); }

	results["#4 Fastest"] = {};
	for (lrc::i64 i = 0; i < librapidTimes.size(); ++i) {
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