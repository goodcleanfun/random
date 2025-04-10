#ifndef RAND32_H
#define RAND32_H

#include <stdbool.h>

#include "pcg32.h"
#include "spinlock/spinlock.h"

static pcg32_random_t pcg32_global;
static bool pcg32_initialized = false;
static spinlock_t pcg32_lock = SPINLOCK_INIT;

static inline void rand32_init() {
    spinlock_lock(&pcg32_lock);
    pcg32_global = (pcg32_random_t) PCG32_INITIALIZER;
    pcg32_initialized = true;
    spinlock_unlock(&pcg32_lock);
}


static inline void rand32_seed(pcg32_random_t *rng, uint32_t seed1, uint32_t seed2) {
    if (!pcg32_initialized) rand32_init();
    pcg32_srandom_r(rng, seed1, seed2);
}

static inline uint32_t rand32() {
    if (!pcg32_initialized) rand32_init();
    return pcg32_random_r(&pcg32_global);
}

static inline uint32_t rand32_bounded(uint32_t bound) {
    if (!pcg32_initialized) rand32_init();
    return pcg32_boundedrand_r(&pcg32_global, bound);
}

static inline pcg32_random_t rand32_gen_init() {
    return (pcg32_random_t) PCG32_INITIALIZER;
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