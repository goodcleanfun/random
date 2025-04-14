#ifndef PCG32_H
#define PCG32_H

#include <stdint.h>

#define PCG_DEFAULT_MULTIPLIER_64  6364136223846793005ULL
#define PCG_STATE_SETSEQ_64_INITIALIZER                                        \
    { 0x853c49e6748fea9bULL, 0xda3e39cb94b95bdbULL }

typedef struct {
    uint64_t state;
    uint64_t inc;
} pcg_state_setseq_64;

inline uint32_t pcg_rotr_32(uint32_t value, unsigned int rot)
{
    return (value >> rot) | (value << ((- rot) & 31));
}

inline void pcg_setseq_64_step_r(pcg_state_setseq_64* rng)
{
    rng->state = rng->state * PCG_DEFAULT_MULTIPLIER_64 + rng->inc;
}

inline uint32_t pcg_output_xsh_rr_64_32(uint64_t state)
{
    return pcg_rotr_32(((state >> 18u) ^ state) >> 27u, state >> 59u);
}

inline uint32_t
pcg_setseq_64_xsh_rr_32_random_r(pcg_state_setseq_64* rng)
{
    uint64_t oldstate = rng->state;
    pcg_setseq_64_step_r(rng);
    return pcg_output_xsh_rr_64_32(oldstate);
}

inline uint32_t
pcg_setseq_64_xsh_rr_32_boundedrand_r(pcg_state_setseq_64* rng,
                                      uint32_t bound)
{
    uint32_t threshold = -bound % bound;
    for (;;) {
        uint32_t r = pcg_setseq_64_xsh_rr_32_random_r(rng);
        if (r >= threshold)
            return r % bound;
    }
}


inline void pcg_setseq_64_srandom_r(pcg_state_setseq_64* rng,
                                    uint64_t initstate, uint64_t initseq)
{
    rng->state = 0U;
    rng->inc = (initseq << 1u) | 1u;
    pcg_setseq_64_step_r(rng);
    rng->state += initstate;
    pcg_setseq_64_step_r(rng);
}

inline uint64_t pcg_advance_lcg_64(uint64_t state, uint64_t delta, uint64_t cur_mult,
                            uint64_t cur_plus)
{
    uint64_t acc_mult = 1u;
    uint64_t acc_plus = 0u;
    while (delta > 0) {
        if (delta & 1) {
            acc_mult *= cur_mult;
            acc_plus = acc_plus * cur_mult + cur_plus;
        }
        cur_plus = (cur_mult + 1) * cur_plus;
        cur_mult *= cur_mult;
        delta /= 2;
    }
    return acc_mult * state + acc_plus;
}

inline void pcg_setseq_64_advance_r(pcg_state_setseq_64* rng,
                                    uint64_t delta)
{
    rng->state = pcg_advance_lcg_64(rng->state, delta,
                                    PCG_DEFAULT_MULTIPLIER_64, rng->inc);
}

typedef pcg_state_setseq_64             pcg32_random_t;
#define pcg32_random_r                  pcg_setseq_64_xsh_rr_32_random_r
#define pcg32_boundedrand_r             pcg_setseq_64_xsh_rr_32_boundedrand_r
#define pcg32_srandom_r                 pcg_setseq_64_srandom_r
#define pcg32_advance_r                 pcg_setseq_64_advance_r
#define PCG32_INITIALIZER               PCG_STATE_SETSEQ_64_INITIALIZER

extern inline uint32_t pcg_rotr_32(uint32_t value, unsigned int rot);
extern inline void pcg_setseq_64_step_r(pcg_state_setseq_64* rng);
extern inline uint32_t pcg_output_xsh_rr_64_32(uint64_t state);
extern inline void pcg_setseq_64_srandom_r(pcg_state_setseq_64* rng,
				     uint64_t initstate, uint64_t initseq);
extern inline uint32_t
pcg_setseq_64_xsh_rr_32_random_r(pcg_state_setseq_64* rng);
extern inline uint32_t
pcg_setseq_64_xsh_rr_32_boundedrand_r(pcg_state_setseq_64* rng,
				       uint32_t bound);
extern inline uint64_t pcg_advance_lcg_64(uint64_t state, uint64_t delta, uint64_t cur_mult,
                            uint64_t cur_plus);
extern inline void pcg_setseq_64_advance_r(pcg_state_setseq_64* rng, uint64_t delta);

#endif