//
// Created by gamerpuppy on 6/24/2021.
//

#ifndef STS_LIGHTSPEED_RANDOM_H
#define STS_LIGHTSPEED_RANDOM_H

namespace sts {

    class Random {
    public:
        static constexpr double NORM_DOUBLE = 1.1102230246251565E-16;
        static constexpr double NORM_FLOAT = 5.9604644775390625E-8;
        static constexpr std::uint64_t ONE_IN_MOST_SIGNIFICANT = static_cast<std::uint64_t>(1) << 63;

        std::int32_t counter;
        std::uint64_t seed0;
        std::uint64_t seed1;

        static constexpr std::uint64_t murmurHash3(std::uint64_t x) {
            x ^= x >> 33;
            x *= static_cast<std::uint64_t>(-49064778989728563LL);
            x ^= x >> 33;
            x *= static_cast<std::uint64_t>(-4265267296055464877LL);
            x ^= x >> 33;
            return x;
        }

        std::uint64_t nextLong() {
            std::uint64_t s1 = seed0;
            std::uint64_t s0 = seed1;
            seed0 = s0;
            s1 ^= s1 << 23;

            seed1 = s1 ^ s0 ^ s1 >> 17 ^ s0 >> 26;
            return seed1 + s0;
        }


        // n must be greater than 0
        std::uint64_t nextLong(std::uint64_t n) {
            std::uint64_t bits;
            std::uint64_t value;
            do {
                bits = static_cast<std::uint64_t>(nextLong()) >> 1;
                value = bits % n;
            } while (static_cast<int64_t>(bits - value + n - 1) < 0LL);
            return static_cast<std::uint64_t>(value);
        }

        std::int32_t nextInt() {
            return static_cast<std::int32_t>(nextLong());
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

        Random(std::uint64_t seed) {
            counter = 0;
            seed0 = murmurHash3(seed == 0 ? ONE_IN_MOST_SIGNIFICANT : seed);
            seed1 = murmurHash3(seed0);
        }

        Random(std::uint64_t seed, std::int32_t targetCounter) : Random(seed) {
            for (int i = 0; i < targetCounter; i++) {
                random(999);
            }
        }

        void setCounter(int targetCounter) {
            while (counter < targetCounter) {
                randomBoolean();
            }
        }

        std::int32_t random(std::int32_t range) {
            ++counter;
            return nextInt(range + 1);
        }

        std::int32_t random(std::int32_t start, std::int32_t end) {
            ++counter;
            return start + nextInt(end - start + 1);
        }

        std::int64_t random(std::int64_t range) {
            ++counter;
            double d = nextDouble() * static_cast<double>(range);
            return static_cast<std::int64_t>(d);
        }

        std::int64_t random(std::int64_t start, std::int64_t end) {
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

        std::int64_t randomLong() {
            ++counter;
            return nextLong();
        }

        bool randomBoolean() {
            ++counter;
            return nextBoolean();
        }

        std::int32_t nextInt(std::int32_t n) {
            return static_cast<std::int32_t>(nextLong(n));
        }
    };

}




#endif //STS_LIGHTSPEED_RANDOM_H
