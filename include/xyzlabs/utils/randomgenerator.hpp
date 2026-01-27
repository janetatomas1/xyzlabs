#pragma once

#include <cinttypes>
#include <random>

namespace xyzlabs {

/**
 * @brief Simple wrapper around a 64-bit Mersenne Twister random number generator.
 *
 * RandomGenerator provides uniformly distributed 64-bit random integers.
 * It encapsulates a std::mt19937_64 generator with std::uniform_int_distribution
 * and offers convenient methods for generating random numbers.
 */
class RandomGenerator {
    /// Internal 64-bit Mersenne Twister generator
    std::mt19937_64 gen;

    /// Uniform distribution for uint64_t
    std::uniform_int_distribution<uint64_t> dist;

public:
    /**
     * @brief Constructs a RandomGenerator seeded with a random_device.
     */
    RandomGenerator() {
        gen = std::mt19937_64(std::random_device{}());
    }

    /**
     * @brief Default destructor.
     */
    ~RandomGenerator() = default;

    /**
     * @brief Generates a uniformly distributed random 64-bit integer.
     *
     * @return uint64_t A random number.
     */
    inline uint64_t random() {
        return dist(gen);
    }

    /**
     * @brief Callable operator equivalent to random().
     *
     * Allows using the generator like a function:
     * @code
     * RandomGenerator rng;
     * uint64_t value = rng(); // same as rng.random()
     * @endcode
     *
     * @return uint64_t A random number.
     */
    inline uint64_t operator()() {
        return random();
    }
};

} // namespace xyzlabs
