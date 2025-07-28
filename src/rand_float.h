/*  Written in 2016-2018 by David Blackman and Sebastiano Vigna (vigna@acm.org)

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

#include <stdint.h>

/* This is xoroshiro128+ 1.0, our best and fastest small-state generator
   for floating-point numbers, but its state space is large enough only
   for mild parallelism. We suggest to use its upper bits for
   floating-point generation, as it is slightly faster than
   xoroshiro128++/xoroshiro128**. It passes all tests we are aware of
   except for the four lower bits, which might fail linearity tests (and
   just those), so if low linear complexity is not considered an issue (as
   it is usually the case) it can be used to generate 64-bit outputs, too;
   moreover, this generator has a very mild Hamming-weight dependency
   making our test (http://prng.di.unimi.it/hwd.php) fail after 5 TB of
   output; we believe this slight bias cannot affect any application. If
   you are concerned, use xoroshiro128++, xoroshiro128** or xoshiro256+.

   We suggest to use a sign test to extract a random Boolean value, and
   right shifts to extract subsets of bits.

   The state must be seeded so that it is not everywhere zero. If you have
   a 64-bit seed, we suggest to seed a splitmix64 generator and use its
   output to fill s. 

   NOTE: the parameters (a=24, b=16, b=37) of this version give slightly
   better results in our test than the 2016 version (a=55, b=14, c=36).
*/

#include "rand_os.h"
#include "rand_seed.h"
#include "rand_double.h"

typedef rand_double_t rand_float_t;

static inline void rand_float_init_seed(rand_float_t *rng, uint64_t seed) {   
    rand_double_init_seed(rng, seed);
}

static inline void rand_float_init(rand_float_t *rng) {
    rand_float_init_seed(rng, os_random_seed());
}

static inline uint32_t rand_float_raw(rand_float_t *rng) {
    uint64_t value = rand_double_raw(rng);
    uint32_t result = value >> 32;
    return result;
}

static inline float bits_to_float(uint32_t bits) {
    return (bits >> 8) * 0x1.0p-24f;
}

static inline float rand_float(rand_float_t *rng) {
    return bits_to_float(rand_float_raw(rng));
}

static inline float rand_float_uniform(rand_float_t *rng) {
    uint32_t bits = rand_float_raw(rng);
    return (bits >> 8) * (1.0f / (1U << 24));
}

static inline float rand_float_bounded(rand_float_t *rng, float low, float high) {
    return low + (high - low) * rand_float_uniform(rng);
}

/* This is the jump function for the generator. It is equivalent
   to 2^64 calls to next(); it can be used to generate 2^64
   non-overlapping subsequences for parallel computations. */

static inline void rand_float_jump(rand_float_t *rng) {
    rand_double_jump(rng);
}


/* This is the long-jump function for the generator. It is equivalent to
   2^96 calls to next(); it can be used to generate 2^32 starting points,
   from each of which jump() will generate 2^32 non-overlapping
   subsequences for parallel distributed computations. */

static inline void rand_float_long_jump(rand_float_t *rng) {
    rand_double_long_jump(rng);
}