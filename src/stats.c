#include "stats.h"
#include <stdio.h>


// Initializes the fields of a Stats structure to zero.
void initStats(Stats* s)
{
    s->calls = 0;
    s->cmp = 0;
    s->moves = 0;
    s->cost = 0.0;
}

double algCost(double a, double b, double c, Stats* count)
{
    // Calculates cost based on the weighted sum of operations 
    return ((a * count->cmp) + (b * count->moves) + (c * count->calls)); 
}

int determineBreaks(int* v, int size)
{
    int breaks = 0;
    // Count how many times an element is smaller than its predecessor
    // A "break" occurs when v[i] < v[i - 1]
    for(int i = 1; i < size; i++)
    {
        if(v[i] < v[i - 1])
            breaks++;
    }
    return breaks;
}

int findPositionForMinCost(double* v_cost, int numMPS) 
{
    int i = 0;
    double minCost = v_cost[0];
    // Find index of the smallest cost value in the array
    for(int j = 1; j < numMPS; j++)
    {
        if(v_cost[j] < minCost)
        {
            minCost = v_cost[j];
            i = j;
        }
    }

    return i;
}

void printStats(Stats s, int mps, char* sort_alg)
{
    printf("%s %d cost %.9f cmp %d move %d calls %d\n", 
        sort_alg, mps, s.cost, s.cmp, s.moves, s.calls);   
}