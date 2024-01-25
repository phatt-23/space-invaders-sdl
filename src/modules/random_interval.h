#ifndef RANDOM_INTERVAL_H
#define RANDOM_INTERVAL_H

#include <time.h>
#include <stdlib.h>
#include <stdio.h>

/*
 * Initialises the random seed.
 * calls srand(time(NULL))
 */
extern void rand_init(void);

/*
 * Generate a random integer
 * inside a closed interval
 * from number 'a' to 'b'
 *
 *      function call rand_interval(10, 20)
 *      will return an int from 10 to 20 (including)
 *      - interval <10, 20>
 */
extern int rand_interval(int a, int b);

extern double drand_interval(double a, double b);

/*
 * Randomly decides the sign of the floating number (double)
 */
extern double db_sign(double number);

/*
 * Randomly decides the sign of the integer number (int)
 */
extern int int_sign(int number);

#endif
