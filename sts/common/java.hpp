//
// Created by keega on 2/20/2022.
//

#ifndef STS_LIGHTSPEED_JAVA_HPP
#define STS_LIGHTSPEED_JAVA_HPP

#include <cstdint>

namespace sts::java {

    class Random {
    private:
        uint64_t seed;
        static constexpr uint64_t multiplier = 0x5DEECE66DULL;
        static constexpr uint64_t addend = 0xBULL;
        static constexpr uint64_t mask = (1ULL << 48) - 1;
        static constexpr double DOUBLE_UNIT = 0x1.0p-53; // 1.0 / (1L << 53)

        static uint64_t initialScramble(uint64_t seed) {
            return (seed ^ multiplier) & mask;
        }

    public:
        Random(uint64_t seed) : seed(initialScramble(seed)) {}

        int32_t next(int32_t bits) {
            seed = (seed * multiplier + addend) & mask;
            return static_cast<int32_t>(seed >> (48 - bits));
        }

        int32_t nextInt() {
            return next(32);
        }

        int32_t nextInt(int32_t bound) {
            int r = next(31);
            int m = bound - 1;
            if ((bound & m) == 0)  // i.e., bound is a power of 2
                r = static_cast<int32_t>( ((bound * static_cast<uint64_t>(r)) >> 31) );
            else {
                for (int32_t u = r;
                     u - (r = u % bound) + m < 0;
                     u = next(31))
                    ;
            }
            return r;
        }
    };

    namespace Collections {

        template <typename ForwardIterator>
        void shuffle(ForwardIterator begin, ForwardIterator end, java::Random rnd) {
            auto size = static_cast<int32_t>(end-begin);

            for (int i=size; i>1; i--) {
                std::swap(*(begin + i - 1), *(begin + rnd.nextInt(i)));
            }
        }

    }
}


#endif //STS_LIGHTSPEED_JAVA_HPP
