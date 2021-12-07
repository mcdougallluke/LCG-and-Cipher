/*************************************************/
/* Luke McDougall                                */
/* 11/03/2021                                    */
/* CS-241 Section 1                              */
/*************************************************/

#include "lcg.h"

/***************************************************************
 * Takes in an LCG pointer and updates the LCG to the next 
 * value in its sequence
 **************************************************************/
unsigned long getNextRandomValue(struct LinearCongruentialGenerator* lcg)
{
    unsigned long nextRandVal = 1;

    nextRandVal = lcg -> x;
    lcg -> x = (lcg->a * lcg->x + lcg->c) % lcg->m;

    return nextRandVal;
}

/***************************************************************
 * Returns the product of all of the unique prime factors of
 * the parameter n. Follows the steps of find unique prime
 * factors from the lab PDF.
 **************************************************************/
unsigned long findProductUniquePrimeFactors(unsigned long n)
{
    unsigned long product = 1;
    unsigned long i = 2;

    if (n < 2)
    {
        return n;
    }
    while (n > 1)
    {
        if ((i * i) > n)
        {
            product = product * n;
            return product;
        }
        else if ((i * i) == n)
        {
            product = product * i;
            return product;
        }
        else if (n % i == 0)
        {
            product = product * i;
            while(n % i == 0)
            {
                n = n / i;
            }
        }
        else
        {
            i++;
        }
    }
    return product;
}

/***************************************************************
 * Initializes an LCG with the required steps described in the
 * lab PDF. a = 1+2p, if 4 is a factor of m, otherwise, a = 1+p
 * p = (product of m's unique prime factors).
 * a < m. If values are invalid for LCG, set all fields to zero.
 **************************************************************/
struct LinearCongruentialGenerator makeLCG(unsigned long m, unsigned long c)
{
    struct LinearCongruentialGenerator lcg;
    unsigned long product = findProductUniquePrimeFactors(m);

    if ((m > 0) && (c < m) && (c > 0))
    {
        lcg.m = m;
        lcg.c = c;
        lcg.x = c;

        if (m % 4 == 0)
        {
            lcg.a = 1 + (2 * product);
        }
        else
        {
            lcg.a = 1 + product;
        }

        if (lcg.a > m || lcg.a < 0)
        {
            lcg.m = lcg.c  = lcg.x = lcg.a = 0;
            return lcg;
        }
    }
    else
    {
        lcg.m = lcg.c  = lcg.x = lcg.a = 0;
    }
    return lcg;
}