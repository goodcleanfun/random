/*  Written in 2019 by David Blackman and Sebastiano Vigna (vigna@acm.org)

To the extent possible under law, the author has dedicated all copyright
and related and neighboring rights to this software to the public domain
worldwide.

Permission to use, copy, modify, and/or distribute this software for any
purpose with or without fee is hereby granted.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR
IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE. */

#ifndef RAND_U32_H
#define RAND_U32_H

#include <stdint.h>

/* This is xoshiro128++ 1.0, one of our 32-bit all-purpose, rock-solid
   generators. It has excellent speed, a state size (128 bits) that is
   large enough for mild parallelism, and it passes all tests we are aware
   of.

   For generating just single-precision (i.e., 32-bit) floating-point
   numbers, xoshiro128+ is even faster.

   The state must be seeded so that it is not everywhere zero. */

#include "rand_os.h"
#include "rand_seed.h"
#include "rand_u64.h"
#include "rotl.h"

#define RAND_U32_STATE_SIZE 4

typedef rand_u64_t rand_u32_t;

static inline void rand_u32_init_seed(rand_u32_t *rng, uint64_t seed) {
    rand_u64_init_seed(rng, seed);
}

static inline void rand_u32_init(rand_u32_t *rng) {
    rand_u32_init_seed(rng, os_random_seed());
}

static inline uint32_t rand_u32(rand_u32_t *rng) {
	uint64_t value = rand_u64(rng);
	// Use higher bits since lower bits have some corre
	uint32_t result = value >> 32;
	return result;
}

static inline uint32_t rand_u32_bounded(rand_u32_t *rng, uint32_t bound) {
    if (bound == 0) {
        return 0;
    }
    uint32_t threshold = -bound % bound;
    for (;;) {
        uint32_t r = rand_u32(rng);
        if (r >= threshold) {
            return r % bound;
        }
    }
}

static inline void rand_u32_jump(rand_u32_t *rng) {
	rand_u64_jump(rng);
}


static inline void rand_u32_long_jump(rand_u32_t *rng) {
	rand_u64_long_jump(rng);
}

#endif