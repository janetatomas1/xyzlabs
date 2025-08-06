
#ifndef RANDOMGENERATOR_HPP
#define RANDOMGENERATOR_HPP

#include <cinttypes>
#include <random>

using uint64_t = uint64_t;

class RandomGenerator {
    std::mt19937_64 gen;
    std::uniform_int_distribution<uint64_t> dist;

public:
    RandomGenerator() {
        gen = std::mt19937_64(std::random_device{}());
    };
    ~RandomGenerator() = default;
    inline uint64_t get_id() {
        return dist(gen);
    };
    inline uint64_t operator()() {
        return get_id();
    }
};

#endif
