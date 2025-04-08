#ifndef PCG32_H
#define PCG32_H

#include <stdint.h>

#define PCG_DEFAULT_MULTIPLIER_64  6364136223846793005ULL

struct pcg_state_setseq_64 {
    uint64_t state;
    uint64_t inc;
};

inline uint32_t pcg_rotr_32(uint32_t value, unsigned int rot)
{
    return (value >> rot) | (value << ((- rot) & 31));
}

inline void pcg_setseq_64_step_r(struct pcg_state_setseq_64* rng)
{
    rng->state = rng->state * PCG_DEFAULT_MULTIPLIER_64 + rng->inc;
}

inline uint32_t pcg_output_xsh_rr_64_32(uint64_t state)
{
    return pcg_rotr_32(((state >> 18u) ^ state) >> 27u, state >> 59u);
}

inline uint32_t
pcg_setseq_64_xsh_rr_32_random_r(struct pcg_state_setseq_64* rng)
{
    uint64_t oldstate = rng->state;
    pcg_setseq_64_step_r(rng);
    return pcg_output_xsh_rr_64_32(oldstate);
}

inline uint32_t
pcg_setseq_64_xsh_rr_32_boundedrand_r(struct pcg_state_setseq_64* rng,
                                      uint32_t bound)
{
    uint32_t threshold = -bound % bound;
    for (;;) {
        uint32_t r = pcg_setseq_64_xsh_rr_32_random_r(rng);
        if (r >= threshold)
            return r % bound;
    }
}


inline void pcg_setseq_64_srandom_r(struct pcg_state_setseq_64* rng,
                                    uint64_t initstate, uint64_t initseq)
{
    rng->state = 0U;
    rng->inc = (initseq << 1u) | 1u;
    pcg_setseq_64_step_r(rng);
    rng->state += initstate;
    pcg_setseq_64_step_r(rng);
}


extern uint64_t pcg_advance_lcg_64(uint64_t state, uint64_t delta,
                                   uint64_t cur_mult, uint64_t cur_plus);


inline void pcg_setseq_64_advance_r(struct pcg_state_setseq_64* rng,
                                    uint64_t delta)
{
    rng->state = pcg_advance_lcg_64(rng->state, delta,
                                    PCG_DEFAULT_MULTIPLIER_64, rng->inc);
}

#define pcg32_random_r                  pcg_setseq_64_xsh_rr_32_random_r
#define pcg32_boundedrand_r             pcg_setseq_64_xsh_rr_32_boundedrand_r
#define pcg32_srandom_r                 pcg_setseq_64_srandom_r
#define pcg32_advance_r                 pcg_setseq_64_advance_r
#define PCG32_INITIALIZER               PCG_STATE_SETSEQ_64_INITIALIZER

#endif