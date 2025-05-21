#ifndef UTILS_H
#define UTILS_H

#include "stats.h"

/**
 * Swaps two integer values and updates the movement counter.
 *
 * @param a pointer to the first value
 * @param b pointer to the second value
 * @param count pointer to the Stats structure to record movements
 */
void swap(int* a, int* b, Stats* count);

/**
 * Returns the median value among three integers.
 *
 * @param a first integer
 * @param b second integer
 * @param c third integer
 * @return the median of the three values
 */
int medianOf3(int a, int b, int c);

/**
 * Shuffles the array by performing a fixed number of swaps.
 *
 * @param v pointer to the array to be shuffled
 * @param size size of the array
 * @param numShuffle number of swaps to perform
 * @return 0 on success
 */
int shuffleVector(int* v, int size, int numShuffle);

/**
 * Sorts an array of integers using an auxiliary sorting algorithm (insertion sort).
 *
 * @param v pointer to the array
 * @param size size of the array
 */
void sortArray(int* v, int size);

#endif