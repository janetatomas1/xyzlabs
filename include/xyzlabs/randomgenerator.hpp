
#pragma once

#include <cinttypes>
#include <random>

namespace xyzlabs {

class RandomGenerator {
    std::mt19937_64 gen;
    std::uniform_int_distribution<uint64_t> dist;

public:
    RandomGenerator() {
        gen = std::mt19937_64(std::random_device{}());
    };
    ~RandomGenerator() = default;
    inline uint64_t random() {
        return dist(gen);
    };
    inline uint64_t operator()() {
        return random();
    }
};

}