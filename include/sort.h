#ifndef SORT_H
#define SORT_H

#include "stats.h"

/**
 * Sorts the subarray v[l..r] using the insertion sort algorithm.
 *
 * @param v array to sort
 * @param l left index of the subarray
 * @param r right index of the subarray
 * @param count pointer to the Stats structure to record operations
 */
void insertionSort(int* v, int l, int r, Stats* count);


/**
 * Sorts the subarray v[l..r] using quicksort with insertion sort fallback.
 * Only applies quicksort to partitions of size greater than minSize.
 *
 * @param v array to sort
 * @param l left index of the subarray
 * @param r right index of the subarray
 * @param minSize minimum partition size to apply quicksort
 * @param count pointer to the Stats structure to record operations
 */
void quickSort(int* v, int l, int r, int minSize, Stats* count);

/**
 * Performs the partitioning step for the quicksort algorithm using median-of-three.
 *
 * @param v array to partition
 * @param l left index of the subarray
 * @param r right index of the subarray
 * @param i output parameter: index marking left partition end
 * @param j output parameter: index marking right partition start
 * @param count pointer to the Stats structure to record operations
 */
void partition(int* v, int l, int r, int* i, int* j, Stats* count);


/**
 * Adaptive sorting algorithm that selects between insertion sort and quicksort
 * based on the number of breaks and partition size.
 *
 * @param v array to sort
 * @param size total size of the array
 * @param minPartitionSize minimum size for quicksort partitioning
 * @param breakThreshold threshold of disorder (number of breaks) to switch algorithms
 * @param count pointer to the Stats structure to record operations
 */
void universalSorter(int* v, int size, int minPartitionSize, int breakThreshold, Stats* count);


#endif