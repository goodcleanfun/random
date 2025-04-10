#ifndef RAND64_H
#define RAND64_H

#include "pcg64.h"
#include <stdbool.h>

static pcg64_random_t pcg64_global;
static bool pcg64_initialized = false;

static inline void rand64_init() {
    pcg64_global = (pcg64_random_t) PCG64_INITIALIZER;
    pcg64_initialized = true;
}

static inline void rand64_seed(pcg64_random_t *rng, uint64_t seed1, uint64_t seed2) {
    if (!pcg64_initialized) rand64_init();
    pcg64_srandom_r(rng, PCG_128BIT_CONSTANT(0, seed1), PCG_128BIT_CONSTANT(0, seed2));
}

static inline uint64_t rand64() {
    if (!pcg64_initialized) rand64_init();
    return pcg64_random_r(&pcg64_global);
}

static inline uint64_t rand64_bounded(uint64_t bound) {
    if (!pcg64_initialized) rand64_init();
    return pcg64_boundedrand_r(&pcg64_global, bound);
}

static inline pcg64_random_t rand64_gen_init() {
    return (pcg64_random_t) PCG64_INITIALIZER;
}

static inline void rand64_gen_seed(pcg64_random_t *rng, uint64_t seed1, uint64_t seed2) {
    pcg64_srandom_r(rng, PCG_128BIT_CONSTANT(0, seed1), PCG_128BIT_CONSTANT(0, seed2));
}

static inline uint64_t rand64_gen_random(pcg64_random_t *rng) {
    return pcg64_random_r(rng);
}

static inline uint64_t rand64_gen_bounded(pcg64_random_t *rng, uint64_t bound) {
    return pcg64_boundedrand_r(rng, bound);
}

static inline void rand64_gen_advance(pcg64_random_t *rng, uint64_t delta) {
    pcg64_advance_r(rng, PCG_128BIT_CONSTANT(0, delta));
}

#endif // RAND64_H
