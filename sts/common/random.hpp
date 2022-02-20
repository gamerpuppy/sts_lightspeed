//
// Created by keega on 2/20/2022.
//

#ifndef STS_LIGHTSPEED_RANDOM_H
#define STS_LIGHTSPEED_RANDOM_H

#include <cstdint>

namespace sts {

    class Random {
    public:
        static constexpr double NORM_DOUBLE = 1.1102230246251565E-16;
        static constexpr double NORM_FLOAT = 5.9604644775390625E-8;
        static constexpr uint64_t ONE_IN_MOST_SIGNIFICANT = static_cast<uint64_t>(1) << 63;

        int32_t counter;
        uint64_t seed0;
        uint64_t seed1;

        static constexpr uint64_t murmurHash3(uint64_t x) {
            x ^= x >> 33;
            x *= static_cast<uint64_t>(-49064778989728563LL);
            x ^= x >> 33;
            x *= static_cast<uint64_t>(-4265267296055464877LL);
            x ^= x >> 33;
            return x;
        }

        uint64_t nextLong() {
            uint64_t s1 = seed0;
            uint64_t s0 = seed1;
            seed0 = s0;
            s1 ^= s1 << 23;

            seed1 = s1 ^ s0 ^ s1 >> 17 ^ s0 >> 26;
            return seed1 + s0;
        }


        // n must be greater than 0
        uint64_t nextLong(uint64_t n) {
            uint64_t bits;
            uint64_t value;
            do {
                bits = static_cast<uint64_t>(nextLong()) >> 1;
                value = bits % n;
            } while (static_cast<int64_t>(bits - value + n - 1) < 0LL);
            return static_cast<uint64_t>(value);
        }

        int32_t nextInt() {
            return static_cast<int32_t>(nextLong());
        }

        double nextDouble() {
            auto x = nextLong() >> 11;
            return static_cast<double>(x) * NORM_DOUBLE;
        }

        float nextFloat() {
            auto x = nextLong() >> 40;
            double d = static_cast<double>(x) * NORM_FLOAT;
            return static_cast<float>(d);
        }

        bool nextBoolean() {
            return nextLong() & 1;
        }

        bool randomBoolean(float chance) {
            ++counter;
            return nextFloat() < chance;
        }


    public:

        Random() : Random(0) {}

        Random(uint64_t seed) {
            counter = 0;
            seed0 = murmurHash3(seed == 0 ? ONE_IN_MOST_SIGNIFICANT : seed);
            seed1 = murmurHash3(seed0);
        }

        Random(uint64_t seed, int32_t targetCounter) : Random(seed) {
            for (int i = 0; i < targetCounter; i++) {
                random(999);
            }
        }

        void setCounter(int targetCounter) {
            while (counter < targetCounter) {
                randomBoolean();
            }
        }

        int32_t random(int32_t range) {
            ++counter;
            return nextInt(range + 1);
        }

        int32_t random(int32_t start, int32_t end) {
            ++counter;
            return start + nextInt(end - start + 1);
        }

        int64_t random(int64_t range) {
            ++counter;
            double d = nextDouble() * static_cast<double>(range);
            return static_cast<int64_t>(d);
        }

        int64_t random(int64_t start, int64_t end) {
            ++counter;
            double d = nextDouble() * static_cast<double>(end - start);
            return start + static_cast<int64_t>(d);
        }

        float random() {
            ++counter;
            return nextFloat();
        }

        float random(float range) {
            ++counter;
            return nextFloat() * range;
        }

        float random(float start, float end) {
            ++counter;
            return start + nextFloat() * (end - start);
        }

        int64_t randomLong() {
            ++counter;
            return nextLong();
        }

        bool randomBoolean() {
            ++counter;
            return nextBoolean();
        }

        int32_t nextInt(int32_t n) {
            return static_cast<int32_t>(nextLong(n));
        }
    };

}


#endif //STS_LIGHTSPEED_RANDOM_H
