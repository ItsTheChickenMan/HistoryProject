/* date = January 25th 2021 4:44 pm */

#ifndef MATH_H
#define MATH_H

#include <stdlib.h>
#include <time.h>

// Randomness

void setup_rng()
{
    srand(time(0));
}

double random_range(double min, double max) 
{
    double range = (max - min); 
    double div = RAND_MAX / range;
    return min + (rand() / div);
}

#endif //MATH_H
