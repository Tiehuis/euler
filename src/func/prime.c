#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include "prime.h"

int eratos_check(si_t *sieve, const ull_t n)
{
    if ((n != 2 && ~n & 1) || (n < 2)) 
        return 0;
    else
        return !(sieve[n >> 6] & (1 << (n >> 1 & 31)));
}

si_t *eratos_sieve(const ull_t ulim)
{
    ull_t i, j;
    si_t *sieve = calloc((ulim >> 6) + 1, sizeof(si_t));

    for (i = 3; i < sqrt(ulim) + 1; i += 2)
        if (!(sieve[i >> 6] & (1 << (i >> 1 & 31))))
            for (j = i*i; j < ulim; j += (i << 1))
                sieve[j >> 6] |= (1 << (j >> 1 & 31));

    return sieve;
}

ull_t modular_exp(ul_t base, ul_t exp, ul_t mod)
{
    ull_t result = 1;
    while (exp) {
        if (exp & 1)
            result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return result;
}

static int mr_witness(ul_t rand, ul_t val, ul_t exp, ul_t rem)
{
    ull_t x = modular_exp(rand, rem, val);
    ull_t y = 0;

    while (exp) {
        y = (x * x) % val;
        if (y == 1 && x != 1 && x != val - 1)
            return 0;
        x = y;
        --exp;
    }

    return y != 1 ? 0 : 1;
}

int mr_prime_test(const ul_t val)
{
    if (((~val & 1) && val != 2) || (val < 2) || (!(val % 3) && val != 3))
        return 0;
    if (val <= 3)
        return 1;

    int rem = val >> 1;
    int exp = 1;
    while (~rem & 1) {
        rem >>= 1;
        exp++;
    }

    if (val < 1373653)
        return mr_witness(2, val, exp, rem) && mr_witness(3, val, exp, rem);
    if (val < 9080191)
        return mr_witness(31, val, exp, rem) && mr_witness(73, val, exp, rem);
    
    return mr_witness(2, val, exp, rem) && mr_witness(7, val, exp, rem) &&
           mr_witness(61, val, exp, rem);
}
