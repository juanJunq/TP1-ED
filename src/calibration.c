#include "calibration.h"
#include "stats.h"
#include "utils.h"
#include "sort.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

int calculateMinPartition(int* v, int size, int minCost, double a, double b, double c, int seed)
{
    // Initial search range for Minimum Partition Size (MPS)
    // MPS cannot be less than 2 (a partition of 1 is already sorted)
    int minMPS = 2, maxMPS = size;
    int stepMPS = (maxMPS - minMPS) / 5;
    // Array to store every (MPS, cost) pair tested across all iterations
    mpsCost costMemory[100];
    int memoryCount = 0; // Counter for elements in costMemory
    // Difference in cost between extremes of the current search range, initialized to a large value
    float diffCost = DBL_MAX;

    // Allocate memory for a master copy of the input array and a working copy for sorting tests
    int* main_arr = malloc(size * sizeof(int));
    int* copy_main_arr = malloc(size * sizeof(int));

    if(main_arr == NULL || copy_main_arr == NULL) {
        printf("Erro alocacao de memoria\n");
        free(main_arr);
        free(copy_main_arr);
        return -1;
    }

    // Master copy of the initial array
    memcpy(main_arr, v, size * sizeof(int));

    int iter = 0;
    while((diffCost > minCost) && (maxMPS - minMPS >= 5)) 
    {
        // Arrays to store MPS values and their corresponding costs for the current iteration
        int currentMPS[6];
        double currentCost[6];
        int numMPS = 0; // Number of MPS values tested per iteration

        printf("iter %d\n", iter);

        for(int t = minMPS; t <= maxMPS && numMPS < 10; t += stepMPS)
        {
            currentMPS[numMPS] = t;
            // Reset working copy from the master array for each test
            memcpy(copy_main_arr, main_arr, size * sizeof(int));

            Stats s;
            initStats(&s);
            // When calibrating MPS, breakThreshold is 0
            // This makes universalSorter use quickSort with 't' as its MPS
            universalSorter(copy_main_arr, size, t, 0, &s);
            s.cost = algCost(a, b, c, &s); // Calculate the cost of this run 
            currentCost[numMPS] = s.cost; // Store the calculated cost

            // Store the (MPS, cost) pair in the overall memory if space is available
            costMemory[memoryCount++] = (mpsCost){t, s.cost};

            printStats(s, t, "mps");
            numMPS++;
        }
        // Find the index of the MPS that yielded the minimum cost in THIS iteration
        int minPartitionSize = findPositionForMinCost(currentCost, numMPS);
        // Get the actual MPS value that had the minimum cost in this iteration
        int bestMPS = currentMPS[minPartitionSize];

        // Determine the new search interval based on the position of 'bestMPS'
        // 'newMinMPS' and 'newMaxMPS' here are initially indices
        int newMinMPS = (minPartitionSize == 0) ? 0 : (minPartitionSize == numMPS - 1) ? numMPS - 3 : minPartitionSize - 1;
        int newMaxMPS = (minPartitionSize == 0) ? 2 : (minPartitionSize == numMPS - 1) ? numMPS - 1 : minPartitionSize + 1;
        newMinMPS = currentMPS[newMinMPS];
        newMaxMPS = currentMPS[newMaxMPS];

        // Recalculate 'diffCost' using costs at the new boundaries
        // These costs should be found from 'costMemory'
        double findMinCost = DBL_MAX, findMaxCost = -DBL_MAX;
        for(int i = 0; i < memoryCount; i++)
        {
            if(costMemory[i].mps == newMinMPS || costMemory[i].mps == newMaxMPS)
            {
                if(costMemory[i].cost < findMinCost) findMinCost = costMemory[i].cost;
                if(costMemory[i].cost > findMaxCost) findMaxCost = costMemory[i].cost;
            }
        }
        diffCost = fabs(findMaxCost - findMinCost);

        printf("nummps %d limParticao %d mpsdiff %.6f\n\n", numMPS, bestMPS, diffCost);
        // Update search range boundaries and step for the next iteration
        minMPS = newMinMPS;
        maxMPS = newMaxMPS;
        stepMPS = (maxMPS - minMPS) / 5;
        if(stepMPS == 0) stepMPS++; // Ensure step is at least 1

        iter++;
    }

    // Find the MPS with the lowest cost recorded overall
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
    // Initial range for the number of breaks to test
    int minBreaks = 1;
    int maxBreaks = size / 2;
    int stepsBreaks = (maxBreaks - minBreaks) / 5;
    int numBreaks; // Counter for breaks tested in an iteration
    double diffCost = DBL_MAX;

    // Allocate memory for several array copies needed during testing
    int* main_arr = (int*)malloc(size * sizeof(int)); // To hold the initially sorted array
    int* copy_main_arr = (int*)malloc(size * sizeof(int)); // Copy for shuffling 
    int* qs_array = (int*)malloc(size * sizeof(int)); // Copy for quicksort
    int* in_array = (int*)malloc(size * sizeof(int)); // Copy for insertion sort

    if(main_arr == NULL || copy_main_arr == NULL || qs_array == NULL || in_array == NULL)
    {
        printf("Error: Memory allocation failed\n");
        free(main_arr);
        free(copy_main_arr);
        free(qs_array);
        free(in_array);
        return;
    }

    sortArray(v, size);
    memcpy(main_arr, v, size * sizeof(int)); // 'main_arr' now holds the sorted array

    int iter = 0;
    // previousMin and previousMax are used to detect if the search range has stopped changing
    int previousMin = -1, previousMax = -1;
    // Main loop: refines the search range for the optimal number of breaks
    // Continues as long as the search range [minBreaks, maxBreaks] is still changing
    // An exit condition for convergence (diffCost <= minCost) is inside the loop
    while(maxBreaks != previousMax || minBreaks != previousMin)
    {
        previousMin = minBreaks; 
        previousMax = maxBreaks;
        printf("iter %d\n", iter);
        
        int currentBreaks[10];
        double currentQSCost[10], currentInCost[10];
        numBreaks = 0;
        
        // Test different numbers of breaks ('t') within the current range [minBreaks, maxBreaks]
        // The condition 'numBreaks < 10' uses the fixed array size
        for(int t = minBreaks; t <= maxBreaks && numBreaks < 10; t += stepsBreaks)
        {
            currentBreaks[numBreaks] = t; // Store the number of breaks being tested
            // Reset working copy from the sorted master array for each test
            memcpy(copy_main_arr, main_arr, size * sizeof(int));
            
            // Re-seed the random number generator and shuffle to introduce 't' breaks
            // This ensures reproducible shuffles for the same 'seed' and 't'
            srand48(seed);
            shuffleVector(copy_main_arr, size, t);
            
            // Create copies for quicksort and insertion sort tests
            memcpy(qs_array, copy_main_arr, size * sizeof(int));
            memcpy(in_array, copy_main_arr, size * sizeof(int));

            Stats qsStats, inStats;
            initStats(&qsStats);
            initStats(&inStats);
            
            // Evaluate quicksort on the shuffled array
            // 'minPartitionSize' is the pre-calibrated value from calculateMinPartition
            quickSort(qs_array, 0, size - 1, minPartitionSize, &qsStats);
            qsStats.cost = algCost(a, b, c, &qsStats);
            currentQSCost[numBreaks] = qsStats.cost; // Store quicksort cost
            
            // Evaluate quicksort on the shuffled array
            insertionSort(in_array, 0, size - 1, &inStats);
            inStats.cost = algCost(a, b, c, &inStats);
            currentInCost[numBreaks] = inStats.cost; // Store insertion sort cost
            
            printStats(qsStats, t, "qs lq");
            printStats(inStats, t, "in lq");

            numBreaks++;
        }

        // Find the number of breaks where the cost difference between insertion sort
        // and quicksort is minimal in THIS iteration
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

        // Refine the search interval [minBreaks, maxBreaks] for the next iteration
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
        // Calculate the cost difference for the new range boundaries 
        // 'costMin' and 'costMax' will be the insertion sort costs at the new 'minBreaks' and 'maxBreaks'
        double costMin = -1, costMax = -1;
        for (int i = 0; i < numBreaks; i++) {
            if (currentBreaks[i] == minBreaks) 
                costMin = currentInCost[i];
            if (currentBreaks[i] == maxBreaks) 
                costMax = currentInCost[i];
        }
        
        float lqdiff = fabs(costMax - costMin);
        diffCost = lqdiff;
        
        printf("numlq %d limQuebras %d lqdiff %.6f\n\n", numBreaks, bestBreaks, lqdiff);

        // Recalculate step for the new break range
        stepsBreaks = (maxBreaks - minBreaks) / 5;
        // Ensure step is at least 1
        if(stepsBreaks == 0)
            stepsBreaks++;

        iter++;

        // Check for convergence based on the cost difference threshold 
        if(diffCost <= minCost)
            return; // Exit if converged
   }

    free(in_array);
    free(qs_array);
    free(main_arr);
    free(copy_main_arr);
}