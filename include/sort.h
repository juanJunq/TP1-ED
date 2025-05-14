#ifndef SORT_H
#define SORT_H

#include "stats.h"

void insertionSort(int* v, int l, int r, Stats* count);

void quickSort(int* v, int l, int r, int minSize, Stats* count);
void partition(int* v, int l, int r, int* i, int* j, Stats* count);

void ordenadorUniversal(int* v, int size, int minPartitionSize, int breakThreshold, Stats* count);


#endif