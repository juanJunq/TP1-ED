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
    mpsCost costMemory[100];
    int memoryCount = 0;
    float diffCost = DBL_MAX;
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
        int currentMPS[6];
        double currentCost[6];
        int numMPS = 0;

        printf("iter %d\n", iter);
        for(int t = minMPS; t <= maxMPS && numMPS < 10; t += stepMPS)
        {
            currentMPS[numMPS] = t;

            memcpy(copy_main_arr, main_arr, size * sizeof(int));

            Stats s;
            initStats(&s);
            ordenadorUniversal(copy_main_arr, size, t, 0, &s);
            s.cost = algCost(a, b, c, &s);
            currentCost[numMPS] = s.cost;

            costMemory[memoryCount].mps = t;
            costMemory[memoryCount].cost = s.cost;
            memoryCount++;
            
            printStats(s, t, "mps");

            numMPS++;
        }
        int minPartitionSize = findPositionForMinCost(currentCost, numMPS);
        int bestMPS = currentMPS[minPartitionSize];        
        
        int newMinMPS, newMaxMPS;
        if(minPartitionSize == 0)
        {
            newMinMPS = 0;
            newMaxMPS = 2;
        } else if(minPartitionSize == numMPS - 1)
        {
            newMinMPS = numMPS - 3;
            newMaxMPS = numMPS - 1;
        } else 
        {
            newMinMPS = minPartitionSize - 1;
            newMaxMPS = minPartitionSize + 1;
        }

        newMinMPS = currentMPS[newMinMPS];
        newMaxMPS = currentMPS[newMaxMPS];

        double findMinCost = DBL_MAX, findMaxCost = -DBL_MAX;
        for(int i = 0; i < memoryCount; i++)
        {
            if(costMemory[i].mps == newMinMPS || costMemory[i].mps == newMaxMPS)
            {
                if(costMemory[i].cost < findMinCost)
                    findMinCost = costMemory[i].cost;
                if(costMemory[i].cost > findMaxCost)
                    findMaxCost = costMemory[i].cost;
            }
        }

        diffCost = fabs(findMaxCost - findMinCost);

        printf("nummps %d limParticao %d mpsdiff %.6f\n\n", numMPS, bestMPS, diffCost);

        minMPS = newMinMPS;
        maxMPS = newMaxMPS;
        stepMPS = (maxMPS - minMPS) / 5;
        if(stepMPS == 0)
            stepMPS++;

        iter++;
    }
    
    int iForBestMPS = 0;
    double minFinalCost = DBL_MAX;
    for(int i = 0; i < memoryCount; i++)
    {
        if(costMemory[i].cost < minFinalCost)
        {
            minFinalCost = costMemory[i].cost;
            iForBestMPS = i;
        }
    }

    int foundMinPartition = costMemory[iForBestMPS].mps;
    free(main_arr);
    free(copy_main_arr);
    return foundMinPartition;
}

void calculateMinBreaks(int* v, int size, double minCost, int minPartitionSize, double a, double b, double c, int seed)
{
    int minBreaks = 1;
    int maxBreaks = size / 2;
    int stepsBreaks = (maxBreaks - minBreaks) / 5;
    int numBreaks;
    double diffCost = DBL_MAX;

    int* main_arr = (int*)malloc(size * sizeof(int));
    int* copy_main_arr = (int*)malloc(size * sizeof(int));
    int* qs_array = (int*)malloc(size * sizeof(int));
    int* in_array = (int*)malloc(size * sizeof(int));

    if(main_arr == NULL || copy_main_arr == NULL || qs_array == NULL || in_array == NULL)
    {
        printf("Erro alocacao de memoria\n");
        return;
    }

    sortArray(v, size);
    memcpy(main_arr, v, size * sizeof(int));

    int iter = 0;
    int previousMin = -1, previousMax = -1;
    while((diffCost > minCost) && (maxBreaks != previousMax || minBreaks != previousMin))
    {
        previousMin = minBreaks;
        previousMax = maxBreaks;
        printf("iter %d\n", iter);
        
        int currentBreaks[7];
        double currentQSCost[7], currentInCost[7];
        numBreaks = 0;
        
        for(int t = minBreaks; t <= maxBreaks && numBreaks < 7; t += stepsBreaks)
        {
            currentBreaks[numBreaks] = t;
            memcpy(copy_main_arr, main_arr, size * sizeof(int));

            srand48(seed);
            shuffleVector(copy_main_arr, size, t);
            
            memcpy(qs_array, copy_main_arr, size * sizeof(int));
            memcpy(in_array, copy_main_arr, size * sizeof(int));

            Stats qsStats, inStats;
            initStats(&qsStats);
            initStats(&inStats);
            
            quickSort(qs_array, 0, size - 1, minPartitionSize, &qsStats);
            qsStats.cost = algCost(a, b, c, &qsStats);
            currentQSCost[numBreaks] = qsStats.cost;
            
            insertionSort(in_array, 0, size - 1, &inStats);
            inStats.cost = algCost(a, b, c, &inStats);
            currentInCost[numBreaks] = inStats.cost;
            
            printStats(qsStats, t, "qs lq");
            printStats(inStats, t, "in lq");

            numBreaks++;
        }

        double minDiff = DBL_MAX;
        int bestBreakPos = 0;
        for (int i = 0; i < numBreaks; i++) {
            double diff = fabs(currentInCost[i] - currentQSCost[i]);
            if (diff < minDiff) {
                minDiff = diff;
                bestBreakPos = i;
            }
        }

        int bestBreaks = currentBreaks[bestBreakPos];

        if (bestBreakPos <= 1) {
            minBreaks = currentBreaks[0];
            maxBreaks = currentBreaks[2];
        } else if (bestBreakPos >= numBreaks - 2) {
            minBreaks = currentBreaks[numBreaks - 3];
            maxBreaks = currentBreaks[numBreaks - 1];
        } else {
            minBreaks = currentBreaks[bestBreakPos - 1];
            maxBreaks = currentBreaks[bestBreakPos + 1];
        }
        double costMin = -1, costMax = -1;
        for (int i = 0; i < numBreaks; i++) {
            if (currentBreaks[i] == minBreaks) costMin = currentInCost[i];
            if (currentBreaks[i] == maxBreaks) costMax = currentInCost[i];
        }
        
        float lqdiff = fabs(costMax - costMin);
        diffCost = lqdiff;
        
        printf("numlq %d limQuebras %d lqdiff %.6f\n\n", numBreaks, bestBreaks, lqdiff);
        
        
        // diffCost = fabs(maxInCost - minInCost);
        // printf("numlq %d limQuebras %d lqdiff %.6f\n\n", numBreaks, bestBreaks, diffCost);

        stepsBreaks = (maxBreaks - minBreaks) / 5;
        if(stepsBreaks == 0)
            stepsBreaks++;

        iter++;
        //printf("diff %.6f > min %.6f\nmaxBreaks %d - minBreaks %d >= 5\n", diffCost, minCost, maxBreaks, minBreaks);        
    }

    free(in_array);
    free(qs_array);
    free(main_arr);
    free(copy_main_arr);
}