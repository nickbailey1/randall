#include <immintrin.h>
#include <cpuid.h>
#include <time.h>
#include <stdio.h>
#include <errno.h>

#include "rand64-hw.h"

/* Hardware implementation.  */

/* Description of the current CPU.  */
struct cpuid { unsigned eax, ebx, ecx, edx; };

/* Return information about the CPU.  See <http://wiki.osdev.org/CPUID>.  */
struct cpuid cpuid (unsigned int leaf, unsigned int subleaf)
{
    struct cpuid result;
    asm ("cpuid"
       : "=a" (result.eax), "=b" (result.ebx),
     "=c" (result.ecx), "=d" (result.edx)
       : "a" (leaf), "c" (subleaf));
    return result;
}

/* Return true if the CPU supports the RDRAND instruction.  */
_Bool rdrand_supported (void)
{
    struct cpuid extended = cpuid (1, 0);
    return (extended.ecx & bit_RDRND) != 0;
}

struct drand48_data drand_buf;

/* Initialize the hardware rand64 implementation.  */
void hardware_rand64_init (void)
{
    // seed the buffer (https://refspecs.linuxbase.org/LSB_5.0.0/LSB-Core-generic/LSB-Core-generic/baselib-mrand48-r-1.html)
    srand48_r(time(NULL), &drand_buf);
}

/* Return a random value, using hardware operations.  */
unsigned long long hardware_rand64 ()
{
    unsigned long long int x;
    while (! _rdrand64_step (&x))
        continue;
    return x;
}

/* uses mrand48_r as okayed by TAs on Piazza. Achieves 64 bits by creating 2 32-bits and shifting */
unsigned long long hardware_lrand48 ()
{
    long int upper32;
    long int lower32;
    mrand48_r(&drand_buf, &upper32);
    mrand48_r(&drand_buf, &lower32);
    unsigned long long upper_cast64 = (unsigned long long) upper32;
    upper_cast64 = upper_cast64 << 32;
    unsigned long long lower_cast64 = (unsigned long long) lower32;
    unsigned long long result = upper_cast64 | lower_cast64;
    return result;
}

/* Finalize the hardware rand64 implementation.  */
void hardware_rand64_fini (void)
{
}