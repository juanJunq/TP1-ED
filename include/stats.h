#ifndef STATS_H
#define STATS_H

typedef struct
{
    int calls;
    int moves;
    int cmp;
    double cost;  
} Stats;

void initStats(Stats* count);

double algCost(double a, double b, double c, Stats* count);

int determineBreaks(int* v, int size);

Stats saveStats(Stats s, double a, double b, double c);
void printStats(Stats s, int mps, char* sort_alg);

int findPositionForMinCost(double* v_cost, int numMPS);


#endif