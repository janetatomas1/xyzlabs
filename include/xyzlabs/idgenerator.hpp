
#ifndef IDGENERATOR_HPP
#define IDGENERATOR_HPP

#include <cinttypes>
#include <random>

using IDType = uint64_t;

class IDGenerator {
    std::mt19937_64 gen;
    std::uniform_int_distribution<IDType> dist;

public:
    IDGenerator() {
        gen = std::mt19937_64(std::random_device{}());
    };
    ~IDGenerator() = default;
    inline IDType get_id() {
        return dist(gen);
    };
    inline IDType operator()() {
        return get_id();
    }
};

#endif
