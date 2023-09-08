# BenchmarksCPP

A Suite of C++ Benchmarks for LibRapid, comparing it against [Eigen](https://gitlab.com/libeigen/eigen.git)
and [XTensor](https://github.com/xtensor-stack/xtensor.git).

## Contributing to Benchmarks

If you know how to increase the performance of ***any*** benchmarks here, please feel free to create a pull request with
some changes. Ideally, we'll be benchmarking the optimal performance of each of these libraries.

Additionally, I only have access to a single machine, so if you want to run the benchmark on your own machine and have
the results included, please create a pull request specifying the specs of your machine and the result of a given
benchmark!

## The Benchmarks

Each benchmark is contained in a different file, and can be found in `benchmarks/src/*.cpp`. If you want to change the
configuration for a given benchmark, edit the JSON object for it in `main.cpp` and recompile the program.

Current benchmarks include:

- Arithmetic operations (currently just array-array addition)
- Matrix transposition

## Running the Benchmarks

To run the benchmarks, you can follow these steps:

#### 1. Clone the repository

```bash
git clone --recursive https://github.com/LibRapid/BenchmarksCPP.git
```

#### 2. Build the benchmarks

```bash
cd BenchmarksCPP
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release
```

#### 3. Run the benchmarks

The exact location of the executable will depend on your operating system, but it will be in the `build` directory.

```bash
# Linux
./BenchmarkCPP

# Windows
./release/BenchmarkCPP.exe
```

#### 4. View the results

This repository contains some scripts to help interpret the results of the benchmarks. The executable writes the results
to CSV files in the same directory as the executable. To plot the results, you can use the `dataProcessor.py` script in
`./scripts/`.

You can use `--help` to see the available options.

Simply specify an input file/directory and an output location, and the script will generate a plot for each benchmark.
You can specify whether to plot the graphs relative to a given library, or to plot the absolute performance.

## Results

The benchmark results are stored as artefacts on LibRapid's CI pipeline, and can also be found on
the [LibRapid website](https://librapid.readthedocs.io/en/test/BenchmarkResults/BenchmarkResults.html)

## Contributing to LibRapid

LibRapid's goal is to be as fast as physically possible, while still presenting a simple-to-use interface.

If you want to improve LibRapid's performance, find a bug, or believe something is not fast enough, please feel free to
create a pull request or file an issue. Additionally, if you need to talk with me or anyone else supporting the library,
our [Discord](https://discord.gg/cGxTFTgCAC) server is a great place to ask questions and get help.
