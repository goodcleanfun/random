#include <stdio.h>
#include <stdlib.h>

#include "greatest/greatest.h"

#include "rand32.h"
#include "rand64.h"

TEST rand32_test(void) {
    uint32_t random_value = rand32();
    PASS();
}

TEST rand32_seed_test(void) {
    rand32_seed(123456, 987654);
    PASS();
}

TEST rand32_bounded_test(void) {
    uint32_t random_value = rand32_bounded(100);
    ASSERT_LT(random_value, 100);
    PASS();
}

TEST rand32_gen_test(void) {
    rand32_gen_t rng = rand32_gen_init();
    rand32_gen_seed_os(&rng);
    uint32_t random_value = rand32_gen_random(&rng);
    PASS();
}

TEST rand32_gen_bounded_test(void) {
    rand32_gen_t rng = rand32_gen_init();
    rand32_gen_seed(&rng, 123456789, 987654321);
    uint32_t random_value = rand32_gen_bounded(&rng, 100);
    ASSERT_LT(random_value, 100);
    PASS();
}

TEST rand32_gen_advance_test(void) {
    rand32_gen_t rng = rand32_gen_init();
    rand32_gen_seed(&rng, 123456789, 987654321);
    rand32_gen_advance(&rng, 100);
    uint32_t random_value = rand32_gen_random(&rng);
    PASS();
}

TEST rand64_test(void) {
    uint64_t random_value = rand64();
    PASS();
}

TEST rand64_seed_test(void) {
    rand64_seed(1234567890123456789ULL, 9876543210987654321ULL);
    PASS();
}

TEST rand64_bounded_test(void) {
    uint64_t random_value = rand64_bounded(1234567890123456789ULL);
    ASSERT_LT(random_value, 1234567890123456789ULL);
    PASS();
}

TEST rand64_gen_test(void) {
    rand64_gen_t rng = rand64_gen_init();
    rand64_gen_seed_os(&rng);
    uint64_t random_value = rand64_gen_random(&rng);
    PASS();
}

TEST rand64_gen_bounded_test(void) {
    rand64_gen_t rng = rand64_gen_init();
    rand64_gen_seed(&rng, 123456789, 987654321);
    uint64_t random_value = rand64_gen_bounded(&rng, 100);
    ASSERT_LT(random_value, 100);
    PASS();
}

TEST rand64_gen_advance_test(void) {
    rand64_gen_t rng = rand64_gen_init();
    rand64_gen_seed(&rng, 123456789, 987654321);
    rand64_gen_advance(&rng, 100);
    uint64_t random_value = rand64_gen_random(&rng);
    PASS();
}

// Main test suite
SUITE(random_tests) {
    RUN_TEST(rand32_test);
    RUN_TEST(rand32_bounded_test);
    RUN_TEST(rand32_seed_test);
    RUN_TEST(rand32_gen_test);
    RUN_TEST(rand32_gen_bounded_test);
    RUN_TEST(rand32_gen_advance_test);
    RUN_TEST(rand64_test);
    RUN_TEST(rand64_seed_test);
    RUN_TEST(rand64_bounded_test);
    RUN_TEST(rand64_gen_test);
    RUN_TEST(rand64_gen_bounded_test);
    RUN_TEST(rand64_gen_advance_test);
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
    GREATEST_MAIN_BEGIN();

    RUN_SUITE(random_tests);

    GREATEST_MAIN_END();
}