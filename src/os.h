#ifndef OS_RANDOM_H
#define OS_RANDOM_H

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>

#include "pcg32.h"
#include "spinlock/spinlock.h"

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
#define HAVE_DEV_RANDOM         IS_UNIX
#endif


#if HAVE_DEV_RANDOM
    #include <fcntl.h>
    #include <unistd.h>
#endif

/* fallback_os_random_bytes(dest, size):
 *     Works like the /dev/random version above, but avoids using /dev/random.
 *     Instead, it uses a private RNG (so that repeated calls will return
 *     different seeds).  Makes no attempt at cryptographic security.
 */

static atomic_bool fallback_os_random_initialized;
static spinlock_t fallback_os_random_lock;
static pcg32_random_t fallback_os_random_entropy_rng;

static void fallback_os_random_bytes(void *dest, size_t size)
{
    if (!atomic_load(&fallback_os_random_initialized)) {
        spinlock_lock(&fallback_os_random_lock);
        if (!atomic_load(&fallback_os_random_initialized)) {
            /* Most modern OSs use address-space randomization, meaning that we can
            use the address of stack variables and system library code as
            initializers.  It's not as good as using /dev/random, but probably
            better than using the current time alone. */

            int local;  // Used as a local variable for address-space randomization    
            pcg32_srandom_r(&fallback_os_random_entropy_rng,
                            time(NULL) ^ (intptr_t)&fallback_os_random_bytes,
                            (intptr_t)&local);

            atomic_store(&fallback_os_random_initialized, true);
        }
        spinlock_unlock(&fallback_os_random_lock);
    }
    
    char *dest_cp = (char *) dest;
    for (size_t i = 0; i < size; ++i) {
        dest_cp[i] = (char) pcg32_random_r(&fallback_os_random_entropy_rng);
    }
}

#if HAVE_DEV_RANDOM 
/* os_random_bytes(dest, size):
 *     Use /dev/urandom to get some external entropy for seeding purposes.
 *
 * Note:
 *     If reading /dev/urandom fails (which ought to never happen), it returns
 *     false, otherwise it returns true.  If it fails, you could instead call
 *     fallback_os_random_bytes which always succeeds.
 */

static bool os_random_bytes(void *dest, size_t size)
{
    int fd = open("/dev/urandom", O_RDONLY);
    if (fd < 0)
        return false;
    ssize_t sz = read(fd, dest, size);
    return (close(fd) == 0) && (sz == (ssize_t) size);
}
#else
static bool os_random_bytes(void *dest, size_t size)
{
    fallback_os_random_bytes(dest, size);
    return true;
}
#endif


#endif // OS_RANDOM_H
