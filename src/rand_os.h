#ifndef OS_RANDOM_H
#define OS_RANDOM_H

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>

#ifndef IS_UNIX
#if !defined(_WIN32) && (defined(__unix__)  || defined(__unix) \
                         || (defined(__APPLE__) && defined(__MACH__)))
    #define IS_UNIX 1
#else
    #define IS_UNIX 0
#endif
#endif

/* If HAVE_DEV_RANDOM is set, we use that value, otherwise we guess */
#ifndef HAVE_DEV_RANDOM
#define HAVE_DEV_RANDOM IS_UNIX
#endif


#if HAVE_DEV_RANDOM
    #include <fcntl.h>
    #include <unistd.h>
#endif

static uint64_t fallback_random_seed(void) {
    /* Most modern OSs use address-space randomization, meaning that we can
    use the address of stack variables and system library code as
    initializers.  It's not as good as using /dev/random, but probably
    better than using the current time alone. */

    return (uint64_t)(time(NULL) ^ (intptr_t)&fallback_random_seed);

}

#if HAVE_DEV_RANDOM 
/* os_random_bytes(dest, size):
 *     Use /dev/urandom to get some external entropy for seeding purposes.
 *
 * Note:
 *     If reading /dev/urandom fails (which ought to never happen), it returns
 *     false, otherwise it returns true.  If it fails, you could instead call
 *     fallback_random_seed which always succeeds.
 */

static inline uint64_t os_random_seed(void) {
    int fd = open("/dev/urandom", O_RDONLY);
    if (fd < 0)
        return fallback_random_seed();
    uint64_t result;
    ssize_t sz = read(fd, &result, sizeof(uint64_t));
    if ((close(fd) == 0) && (sz == (ssize_t) sizeof(uint64_t))) {
        return result;
    }
    return fallback_random_seed();
}
#else
static inline bool os_random_seed(void) {
    return fallback_random_seed();
}
#endif


#endif // OS_RANDOM_H
