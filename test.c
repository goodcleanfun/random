#include <stdio.h>
#include <stdlib.h>

#include "greatest/greatest.h"

#include "rand32.h"
#include "rand64.h"

TEST rand32_test(void) {
    pcg32_random_t rng = rand32_gen_init();
    rand32_gen_seed(&rng, 123456789, 987654321);
    uint32_t random_value = rand32_gen_random(&rng);
    PASS();
}

TEST rand32_bounded_test(void) {
    pcg32_random_t rng = rand32_gen_init();
    rand32_gen_seed(&rng, 123456789, 987654321);
    uint32_t random_value = rand32_gen_bounded(&rng, 100);
    ASSERT_LT(random_value, 100);
    PASS();
}

TEST rand32_advance_test(void) {
    pcg32_random_t rng = rand32_gen_init();
    rand32_gen_seed(&rng, 123456789, 987654321);
    rand32_gen_advance(&rng, 100);
    uint32_t random_value = rand32_gen_random(&rng);
    PASS();
}

TEST rand64_test(void) {
    pcg64_random_t rng = rand64_gen_init();
    rand64_gen_seed(&rng, 123456789, 987654321);
    uint64_t random_value = rand64_gen_random(&rng);
    PASS();
}

TEST rand64_bounded_test(void) {
    pcg64_random_t rng = rand64_gen_init();
    rand64_gen_seed(&rng, 123456789, 987654321);
    uint64_t random_value = rand64_gen_bounded(&rng, 100);
    ASSERT_LT(random_value, 100);
    PASS();
}

TEST rand64_advance_test(void) {
    pcg64_random_t rng = rand64_gen_init();
    rand64_gen_seed(&rng, 123456789, 987654321);
    rand64_gen_advance(&rng, 100);
    uint64_t random_value = rand64_gen_random(&rng);
    PASS();
}

// Main test suite
SUITE(random_tests) {
    RUN_TEST(rand32_test);
    RUN_TEST(rand32_bounded_test);
    RUN_TEST(rand32_advance_test);
    RUN_TEST(rand64_test);
    RUN_TEST(rand64_bounded_test);
    RUN_TEST(rand64_advance_test);
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
    GREATEST_MAIN_BEGIN();

    RUN_SUITE(random_tests);

    GREATEST_MAIN_END();
}