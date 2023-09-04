# LibRapid Benchmarks

## Run the Benchmarks Yourself

You can run the benchmarks yourself by cloning the
[Benchmark repository](https://github.com/LibRapid/BenchmarksCPP) and running the necessary
CMake commands.

```
git clone --recursive https://github.com/LibRapid/BenchmarksCPP.git

cd BenchmarksCPP
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release
```

:::{warning}
Make sure to run the benchmarks in `Release` mode. Otherwise, the compiler will not produce
optimized code and the results will not be accurate.
:::

## Warning

The benchmarks included in the documentation were run on free GitHub Actions runners. These
machines have a limited number of virtual CPU cores, a small amount of RAM and are not designed
for intensive workloads.

Running the benchmarks on my personal machine produces fairly different results from those shown in
documentation.

As a result, ***the benchmarks do not necessarily represent the true performance characteristics of each library***.

It is ***highly recommended that you run the benchmarks yourself*** for the best results.

:::{note}
LibRapid's developers are looking into getting more powerful servers to run the benchmarks on, but
we do not currently have the funding or resources to do so.
:::

