#ifndef CALIBRATION_H
#define CALIBRATION_H

/**
 * Associates a partition size (mps) with its corresponding sorting cost.
 */
typedef struct 
{
    int mps;
    double cost;
} mpsCost;

/**
 * Associates a number of breaks with the sorting costs of quicksort and insertion sort.
 */
typedef struct
{
    int breaks;
    double qsCost;
    double inCost;
} breakCost;

/**
 * Finds the minimum partition size (mps) that minimizes the sorting cost.
 *
 * @param v array to be sorted
 * @param size size of the array
 * @param minCost cost convergence threshold
 * @param a coefficient for comparisons
 * @param b coefficient for movements
 * @param c coefficient for calls
 * @param seed random seed for shuffling
 * @return optimal partition size
 */
int calculateMinPartition(int* v, int size, int minCost, double a, double b, double c, int seed);

/**
 * Refines the search range for minimum partition size in the calibration process.
 *
 * @param minPartitionSize current best partition size
 * @param minMPS pointer to lower bound of range
 * @param maxMPS pointer to upper bound of range
 * @param steps pointer to step size between partition sizes
 * @param mps array of tested partition sizes
 * @param numMPS number of tested partition sizes
 */
void adjustSearchRange(int minPartitionSize, int* minMPS, int* maxMPS, int* steps, int* mps, int numMPS);

/**
 * Calibrates the maximum number of breaks for which insertion sort is still more efficient.
 *
 * @param v array to be tested
 * @param size size of the array
 * @param minCost cost convergence threshold
 * @param minPartitionSize minimum partition size (for hybrid sorting)
 * @param a coefficient for comparisons
 * @param b coefficient for movements
 * @param c coefficient for calls
 * @param seed random seed for reproducible shuffling
 */
void calculateMinBreaks(int* v, int size, double minCost, int minPartitionSize, double a, double b, double c, int seed);

#endif