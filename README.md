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

## Results

The results of the benchmarks can be found in `results/*.md`.

## Learnings

Based on these preliminary benchmarks, it's clear that LibRapid makes very effective use of multithreading, as, when
more threads are used, it is often able to out-perform Eigen and XTensor. However, when using a single thread, LibRapid
is almost always slower than its competitors.

Additionally, some algorithms, such as matrix transposition, are unacceptably slow. These will be the primary focus of
future optimisations for LibRapid until they reach or exceed the performance of Eigen and XTensor. Luckily (?), in many
cases, the algorithms which perform poorly are already known to be inefficient, and have issues open requiring algorithm
redesigns and code rewrites to accelerate them.

Overall, considering LibRapid's current state and the fact that it has been developed solely
by [me](https://github.com/Pencilcaseman), I am quite pleased that I'm even in a position where I can compare its
performance against well-established libraries like Eigen and XTensor.

## Contributing to LibRapid

LibRapid's goal is to be as fast as physically possible, while still presenting a simple-to-use interface.

If you want to improve LibRapid's performance, find a bug, or believe something is not fast enough, please feel free to
create a pull request or file an issue. Additionally, if you need to talk with me or anyone else supporting the library,
our [Discord](https://discord.gg/cGxTFTgCAC) server is a great place to ask questions and get help.
