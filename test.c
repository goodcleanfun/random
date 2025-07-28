#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <float.h>

#include "greatest/greatest.h"

#include "rand_u32.h"
#include "rand_u64.h"
#include "rand_float.h"
#include "rand_double.h"

TEST rand32_test(void) {
    rand_u32_gen_t rng;
    rand_u32_init(&rng);
    uint32_t random_value = rand_u32(&rng);
    PASS();
}

TEST rand32_seed_test(void) {
    rand_u32_gen_t rng;
    rand_u32_init_seed(&rng, 123456);
    PASS();
}

TEST rand32_bounded_test(void) {
    rand_u32_gen_t rng;
    rand_u32_init(&rng);
    uint32_t random_value = rand_u32_bounded(&rng, 100);
    ASSERT_LT(random_value, 100);
    PASS();
}

TEST rand64_test(void) {
    rand_u64_gen_t rng;
    rand_u64_init(&rng);
    uint64_t random_value = rand_u64(&rng);
    PASS();
}

TEST rand64_seed_test(void) {
    rand_u64_gen_t rng;
    rand_u64_init_seed(&rng, 1234567890123456789ULL);
    PASS();
}

TEST rand64_bounded_test(void) {
    rand_u64_gen_t rng;
    rand_u64_init(&rng);
    uint64_t random_value = rand_u64_bounded(&rng, 1234567890123456789ULL);
    ASSERT_LT(random_value, 1234567890123456789ULL);
    PASS();
}

TEST rand_float_uniform_test(void) {
    rand_float_gen_t rng;
    rand_float_init(&rng);
    float random_value = rand_float_uniform(&rng);
    ASSERT_LT(random_value, 1.0f + FLT_EPSILON);
    ASSERT_GT(random_value, 0.0f - FLT_EPSILON);
    PASS();
}

TEST rand_float_bounded_test(void) {
    rand_float_gen_t rng;
    rand_float_init(&rng);
    float random_value = rand_float_bounded(&rng, 0.0f, 100.0f);
    ASSERT_LT(random_value, 100.0f + FLT_EPSILON);
    ASSERT_GT(random_value, 0.0f - FLT_EPSILON);
    PASS();
}

TEST rand_float_test(void) {
    rand_float_gen_t rng;
    rand_float_init(&rng);
    float random_value = rand_float(&rng);
    PASS();
}

TEST rand_double_uniform_test(void) {
    rand_double_gen_t rng;
    rand_double_init(&rng);
    double random_value = rand_double_uniform(&rng);
    ASSERT_LT(random_value, 1.0 + DBL_EPSILON);
    ASSERT_GT(random_value, 0.0 - DBL_EPSILON);
    PASS();
}

TEST rand_double_bounded_test(void) {
    rand_double_gen_t rng;
    rand_double_init(&rng);
    double random_value = rand_double_bounded(&rng, 0.0, 100.0);
    ASSERT_LT(random_value, 100.0 + DBL_EPSILON);
    ASSERT_GT(random_value, 0.0 - DBL_EPSILON);
    PASS();
}

TEST rand_double_test(void) {
    rand_double_gen_t rng;
    rand_double_init(&rng);
    double random_value = rand_double(&rng);
    PASS();
}

// Main test suite
SUITE(random_tests) {
    RUN_TEST(rand32_test);
    RUN_TEST(rand32_bounded_test);
    RUN_TEST(rand32_seed_test);
    RUN_TEST(rand64_test);
    RUN_TEST(rand64_seed_test);
    RUN_TEST(rand64_bounded_test);
    RUN_TEST(rand_float_test);
    RUN_TEST(rand_float_uniform_test);
    RUN_TEST(rand_float_bounded_test);
    RUN_TEST(rand_double_test);
    RUN_TEST(rand_double_uniform_test);
    RUN_TEST(rand_double_bounded_test);
}

GREATEST_MAIN_DEFS();

int main(int argc, char **argv) {
    GREATEST_MAIN_BEGIN();

    RUN_SUITE(random_tests);

    GREATEST_MAIN_END();
}