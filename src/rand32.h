#ifndef RAND32_H
#define RAND32_H

#include <stdbool.h>

#include "os.h"
#include "pcg32.h"
#include "spinlock/spinlock.h"

typedef pcg32_random_t rand32_gen_t;
static rand32_gen_t pcg32_global;
static bool pcg32_initialized = false;
static spinlock_t pcg32_lock = SPINLOCK_INIT;

static inline void rand32_init(bool random_seed) {
    spinlock_lock(&pcg32_lock);
    if (pcg32_initialized) {
        spinlock_unlock(&pcg32_lock);
        return;
    }
    pcg32_global = (rand32_gen_t) PCG32_INITIALIZER;
    if (random_seed) {
        uint32_t seeds[2];
        if (!os_random_bytes(seeds, sizeof(seeds))) {
            fallback_os_random_bytes(seeds, sizeof(seeds));
        }
        pcg32_srandom_r(&pcg32_global, seeds[0], seeds[1]);
    }
    pcg32_initialized = true;
    spinlock_unlock(&pcg32_lock);
}

static inline void rand32_seed(uint32_t seed1, uint32_t seed2) {
    if (!pcg32_initialized) rand32_init(false);
    pcg32_srandom_r(&pcg32_global, seed1, seed2);
}

static inline uint32_t rand32() {
    if (!pcg32_initialized) rand32_init(true);
    return pcg32_random_r(&pcg32_global);
}

static inline uint32_t rand32_bounded(uint32_t bound) {
    if (!pcg32_initialized) rand32_init(true);
    return pcg32_boundedrand_r(&pcg32_global, bound);
}

static inline rand32_gen_t rand32_gen_init() {
    return (rand32_gen_t) PCG32_INITIALIZER;
}

static inline void rand32_gen_seed_os(rand32_gen_t *rng) {
    uint32_t seeds[2];
    if (!os_random_bytes(seeds, sizeof(seeds))) {
        fallback_os_random_bytes(seeds, sizeof(seeds));
    }
    pcg32_srandom_r(rng, seeds[0], seeds[1]);
}

static inline void rand32_gen_seed(rand32_gen_t *rng, uint32_t seed1, uint32_t seed2) {
    pcg32_srandom_r(rng, seed1, seed2);
}

static inline uint32_t rand32_gen_random(rand32_gen_t *rng) {
    return pcg32_random_r(rng);
}

static inline uint32_t rand32_gen_bounded(rand32_gen_t *rng, uint32_t bound) {
    return pcg32_boundedrand_r(rng, bound);
}

static inline void rand32_gen_advance(rand32_gen_t *rng, uint64_t delta) {
    pcg32_advance_r(rng, delta);
}

#endif // RAND32_H