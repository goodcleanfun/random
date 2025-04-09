#ifndef RAND32_H
#define RAND32_H

#include "pcg32.h"

static pcg32_random_t pcg32_global = PCG32_INITIALIZER;

static inline pcg32_random_t rand32_gen_init() {
    return (pcg32_random_t) PCG32_INITIALIZER;
}

static inline void rand32_seed(pcg32_random_t *rng, uint32_t seed1, uint32_t seed2) {
    pcg32_srandom_r(rng, seed1, seed2);
}

static inline uint32_t rand32() {
    return pcg32_random_r(&pcg32_global);
}

static inline uint32_t rand32_bounded(uint32_t bound) {
    return pcg32_boundedrand_r(&pcg32_global, bound);
}

static inline void rand32_gen_seed(pcg32_random_t *rng, uint32_t seed1, uint32_t seed2) {
    pcg32_srandom_r(rng, seed1, seed2);
}

static inline uint32_t rand32_gen_random(pcg32_random_t *rng) {
    return pcg32_random_r(rng);
}

static inline uint32_t rand32_gen_bounded(pcg32_random_t *rng, uint32_t bound) {
    return pcg32_boundedrand_r(rng, bound);
}

static inline void rand32_gen_advance(pcg32_random_t *rng, uint64_t delta) {
    pcg32_advance_r(rng, delta);
}

#endif // RAND32_H