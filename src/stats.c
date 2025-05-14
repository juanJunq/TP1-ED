#include "stats.h"
#include <stdio.h>

void initStats(Stats* s)
{
    s->calls = 0;
    s->cmp = 0;
    s->moves = 0;
}

float algCost(int a, int b, int c, Stats* count)
{
    return ((a * count->cmp) + (b * count->moves) + (c * count->calls)); 
}

int determineBreaks(int* v, int size)
{
    int breaks = 0;
    for(int i = 1; i < size; i++)
    {
        if(v[i] < v[i - 1])
            breaks++;
    }
    return breaks;
}

int findPositionForMinCost(int* v_cost, int numMPS) 
{
    int i = 0;
    float minCost = v_cost[0];

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

//Stats saveStats(Stats s, float a, float b, float c)
// {
//     FILE *file = fopen("stats.txt", "a");
    
    
//     //fprintf(file, "iter %d\n", iter);
//     s.cost = algCost(a, b, c, &s);
//     for(int i = 0; i < 6; i++)
//     {
//         fprintf(file, "cost %.9f cmp %d move %d calls %d\n",
//             s.cost,
//             s.cmp,
//             s.moves,
//             s.calls
//         );
//     }
//     // fprintf(file, "num%s 6 lim%d %d %sdiff %.6f\n\n",
//     //     param, param, mps, param, costDiff
//     // );

//     fclose(file);
// }

void printStats(Stats s, int mps, char* sort_alg)
{
    printf("%s mps %d cost %.9f cmp %d move %d calls %d\n", 
        sort_alg, mps, s.cost, s.cmp, s.moves, s.calls);   
}