#ifndef STATS_H
#define STATS_H

/**
 * Structure used to store performance statistics of sorting algorithms.
 * - calls: number of recursive calls or function invocations
 * - moves: number of element moves
 * - cmp: number of element comparisons
 * - cost: total cost computed from a weighted sum of the above
 */
typedef struct
{
    int calls;
    int moves;
    int cmp;
    double cost;  
} Stats;

/**
 * Initializes the statistics structure with zeroed values.
 *
 * @param count pointer to Stats structure
 */
void initStats(Stats* count);

/**
 * Computes the cost of a sorting execution based on given weights.
 *
 * @param a weight for comparisons
 * @param b weight for moves
 * @param c weight for calls
 * @param count pointer to Stats structure with collected metrics
 * @return the computed cost
 */
double algCost(double a, double b, double c, Stats* count);

/**
 * Determines the number of "breaks" (inversions) in the array.
 * A break is a position where v[i] < v[i-1].
 *
 * @param v array to analyze
 * @param size size of the array
 * @return number of breaks found
 */
int determineBreaks(int* v, int size);

/**
 * Prints the statistics to standard output, formatted by algorithm and partition size.
 *
 * @param s Stats structure to print
 * @param mps minimum partition size used in the test
 * @param sort_alg name of the algorithm used (e.g., "qs", "in")
 */
void printStats(Stats s, int mps, char* sort_alg);

/**
 * Finds the index of the lowest cost in a vector of costs.
 *
 * @param v_cost array of cost values
 * @param numMPS number of elements in the array
 * @return position of the minimum cost
 */
int findPositionForMinCost(double* v_cost, int numMPS);


#endif