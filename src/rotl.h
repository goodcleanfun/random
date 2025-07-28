#ifndef ROTL_H
#define ROTL_H

#include <stdint.h>

static inline uint64_t rotl(const uint64_t x, int k) {
	return (x << k) | (x >> (64 - k));
}

#endif