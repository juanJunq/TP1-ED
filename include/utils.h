#ifndef UTILS_H
#define UTILS_H

#include "stats.h"

void swap(int* fromX, int* toY, Stats* count);

int medianOf3(int a, int b, int c);

int shuffleVector(int* v, int size, int numShuffle);
void sortArray(int* v, int size);

#endif