#ifndef RANDOM_H
#define RANDOM_H

#include <time.h>
#include <stdlib.h>

int rand_int(int min, int max) {
    return rand() % (max - min) + min;
}

#endif