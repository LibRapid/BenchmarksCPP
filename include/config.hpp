#pragma once

namespace bench {
    struct BenchConfig {
        std::string name;
        int64_t numThreads;
        std::chrono::nanoseconds minEpochTime;
    };
} // namespace bench