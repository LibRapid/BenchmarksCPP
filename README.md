# BenchmarksCPP
A Suite of C++ Benchmarks for LibRapid, comparing it against [Eigen](https://gitlab.com/libeigen/eigen.git) and [XTensor](https://github.com/xtensor-stack/xtensor.git).

## Contributing
If you know how to increase the performance of ***any*** of the libraries benchmarked here, please feel free to create a pull request with some changes. Ideally, we'll be benchmarking the optimal performance of each of these libraries.

Additionally, I only have access to a single machine, so if you want to run the benchmark on your own machine and have the results included, please create a pull request specifying the specs of your machine and the result of a given benchmark!

## The Benchmarks

Each benchmark is contained in a different file, and can be found in `benchmarks/src/*.cpp`. If you want to change the configuration for a given benchmark, edit the JSON object for it in `main.cpp` and recompile the program.

## Results

The results of the benchmarks can be found in `results/*.md`.
