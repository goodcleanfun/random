# random
Fast, non-cryptographic random number generation using the [xoshiro256++ for ints and xoshirto256+ for floats](https://prng.di.unimi.it/) family. Includes the SplitMix64 seed generator and a cross-platform function get OS randomness from either /dev/urandom if available or address-space randomization if not.

