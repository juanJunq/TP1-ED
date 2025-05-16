#include "calibration.h"
#include "stats.h"
#include "utils.h"
#include "sort.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

void adjustSearchRange(int minPartitionSize, int* minMPS, int* maxMPS, int* steps, int* mps, int numMPS)
{
    int newMin, newMax;

    if(minPartitionSize == 0)
    {
        newMin = 0;
        newMax = 2;
    } else if(minPartitionSize == numMPS - 1) 
    {
        newMin = numMPS - 3;
        newMax = numMPS - 1;
    } else
    {
        newMin = minPartitionSize - 1;
        newMax = minPartitionSize + 1;
    }

    *minMPS = mps[newMin];
    *maxMPS = mps[newMax];
    *steps = (*maxMPS - *minMPS) / 5;
    if(*steps == 0)
        (*steps)++;
}

int calculateMinPartition(int* v, int size, int minCost, double a, double b, double c, int seed)
{
    int minMPS = 2, maxMPS = size;
    int stepMPS = (maxMPS - minMPS) / 5;
    int numMPS;
    float diffCost = DBL_MAX;
    int mps[6]; 
    double cost[10];
    int* main_arr = (int*)malloc(size * sizeof(int));
    int* copy_main_arr = (int*)malloc(size * sizeof(int));

    if(main_arr == NULL || copy_main_arr == NULL)
    {
        printf("Erro alocacao de memoria\n");
        return -1;
    }

    memcpy(main_arr, v, size * sizeof(int));

    int iter = 0;
    while((diffCost > minCost) && (maxMPS - minMPS >= 5)) 
    {
        printf("iter %d\n\n", iter);

        numMPS = 0;
        for(int t = minMPS; t <= maxMPS && numMPS < 10; t += stepMPS)
        {
            mps[numMPS] = t;

            memcpy(copy_main_arr, main_arr, size * sizeof(int));

            Stats s;
            initStats(&s);

            ordenadorUniversal(copy_main_arr, size, t, 0, &s);
            s.cost = algCost(a, b, c, &s);
            cost[numMPS] = s.cost;
            printStats(s, t, "mps");

            numMPS++;
        }
        int minPartitionSize = findPositionForMinCost(cost, numMPS);

        adjustSearchRange(minPartitionSize, &minMPS, &maxMPS, &stepMPS, mps, numMPS);
        
        // printf("\n\nCOST: ");
        // for(int i = 0; i < 6; i++) {
        //     printf("%f ", cost[i]);
        // }
        // printf("\n\n");
        diffCost = fabs(cost[0] - cost[numMPS - 1]);
        printf("nummps %d limParticao %d mpsdiff %.6f\n\n", numMPS, mps[minPartitionSize], diffCost);

        iter++;
    }
    int foundMinPartition = mps[findPositionForMinCost(cost, numMPS)];

    free(main_arr);
    free(copy_main_arr);

    return foundMinPartition;
}

int calculateMinBreaks(int* v, int size, double minCost, int minPartitionSize, double a, double b, double c, int seed)
{
    int minBreaks = 1;
    int maxBreaks = size / 2;
    int stepsBreaks = (maxBreaks - minBreaks) / 5;
    int numBreaks;
    float diffCost = DBL_MAX;
    int breaks[7];
    double qsCost[10], inCost[10];
    int* main_arr = (int*)malloc(size * sizeof(int));
    int* copy_main_arr = (int*)malloc(size * sizeof(int));

    if(main_arr == NULL || copy_main_arr == NULL)
    {
        printf("Erro alocacao de memoria\n");
        return -1;
    }

    sortArray(v, size);

    memcpy(main_arr, v, size * sizeof(int));

    int iter = 0;
    while((diffCost > minCost) && (maxBreaks - minBreaks >= 5))
    {
        printf("iter %d\n\n", iter);

        numBreaks = 0;
        for(int t = minBreaks; t <= maxBreaks && numBreaks < 10; t += stepsBreaks)
        {
            breaks[numBreaks] = t;


            memcpy(copy_main_arr, main_arr, size * sizeof(int));

            srand48(seed);
            shuffleVector(copy_main_arr, size, t);
            
            Stats qsStats, inStats;
            initStats(&qsStats);
            initStats(&inStats);
            
            quickSort(copy_main_arr, 0, size - 1, minPartitionSize, &qsStats);
            qsStats.cost = algCost(a, b, c, &qsStats);
            qsCost[numBreaks] = qsStats.cost;
            printStats(qsStats, t, "qs lq");
            
            insertionSort(copy_main_arr, 0, size - 1, &inStats);
            inStats.cost = algCost(a, b, c, &inStats);
            inCost[numBreaks] = inStats.cost;
            printStats(inStats, t, "in lq");

            numBreaks++;
        }
        int bestBreakPos = findPositionForMinCost(inCost, numBreaks);

        adjustSearchRange(bestBreakPos, &minBreaks, &maxBreaks, &stepsBreaks, breaks, numBreaks);

        diffCost = fabs(inCost[0] - inCost[numBreaks - 1]);

        printf("numlq %d limQuebras %d lqdiff %.6f\n\n", numBreaks, breaks[bestBreakPos], diffCost);

        iter++;
    }
    int foundPositionForMinCost = findPositionForMinCost(inCost, numBreaks);
    int limBreaks = breaks[foundPositionForMinCost];

    free(main_arr);
    free(copy_main_arr);

    return limBreaks;
}
