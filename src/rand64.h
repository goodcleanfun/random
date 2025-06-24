#ifndef RAND64_H
#define RAND64_H

#include <stdatomic.h>
#include <stdbool.h>

#include "os.h"
#include "pcg64.h"
#include "spinlock/spinlock.h"

typedef pcg64_random_t rand64_gen_t;
static rand64_gen_t pcg64_global;
// default value of static atomic variables is zero without initialization
static atomic_bool pcg64_initialized;
static spinlock_t pcg64_lock;

static inline void rand64_init(bool random_seed) {
    spinlock_lock(&pcg64_lock);
    if (atomic_load(&pcg64_initialized)) {
        spinlock_unlock(&pcg64_lock);
        return;
    }
    pcg64_global = (rand64_gen_t) PCG64_INITIALIZER;
    if (random_seed) {
        pcg128_t seeds[2];
        os_random_bytes(seeds, sizeof(seeds));
        pcg64_srandom_r(&pcg64_global, seeds[0], seeds[1]);
    }
    atomic_store(&pcg64_initialized, true);
    spinlock_unlock(&pcg64_lock);
}

static inline void rand64_seed(uint64_t seed1, uint64_t seed2) {
    if (!atomic_load(&pcg64_initialized)) rand64_init(false);
    pcg64_srandom_r(&pcg64_global, (pcg128_t) PCG_128BIT_CONSTANT(0, seed1), (pcg128_t) PCG_128BIT_CONSTANT(0, seed2));
}

static inline uint64_t rand64() {
    if (!atomic_load(&pcg64_initialized)) rand64_init(true);
    return pcg64_random_r(&pcg64_global);
}

static inline uint64_t rand64_bounded(uint64_t bound) {
    if (!atomic_load(&pcg64_initialized)) rand64_init(true);
    return pcg64_boundedrand_r(&pcg64_global, bound);
}

static inline rand64_gen_t rand64_gen_init() {
    return (rand64_gen_t) PCG64_INITIALIZER;
}

static inline void rand64_gen_seed(rand64_gen_t *rng, uint64_t seed1, uint64_t seed2) {
    pcg64_srandom_r(rng, (pcg128_t) PCG_128BIT_CONSTANT(0, seed1), (pcg128_t) PCG_128BIT_CONSTANT(0, seed2));
}

static inline void rand64_gen_seed_os(rand64_gen_t *rng) {
    pcg128_t seeds[2];
    if (!os_random_bytes(seeds, sizeof(seeds))) {
        fallback_os_random_bytes(seeds, sizeof(seeds));
    }
    pcg64_srandom_r(rng, seeds[0], seeds[1]);
}

static inline uint64_t rand64_gen_random(rand64_gen_t *rng) {
    return pcg64_random_r(rng);
}

static inline uint64_t rand64_gen_bounded(rand64_gen_t *rng, uint64_t bound) {
    return pcg64_boundedrand_r(rng, bound);
}

static inline void rand64_gen_advance(rand64_gen_t *rng, uint64_t delta) {
    pcg64_advance_r(rng, (pcg128_t) PCG_128BIT_CONSTANT(0, delta));
}

#endif // RAND64_H
