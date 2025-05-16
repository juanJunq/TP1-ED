#ifndef STATS_H
#define STATS_H

typedef struct
{
    int calls;
    int moves;
    int cmp;
    float cost;  
} Stats;

void initStats(Stats* count);

float algCost(float a, float b, float c, Stats* count);

int determineBreaks(int* v, int size);

Stats saveStats(Stats s, float a, float b, float c);
void printStats(Stats s, int mps, char* sort_alg);

int findPositionForMinCost(float* v_cost, int numMPS);


#endif