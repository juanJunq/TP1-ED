#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include "../include/sort.h"
#include "../include/calibration.h"
#include "../include/stats.h"

int main(int argc, char** argv)
{
    FILE* inputFile = fopen(argv[1], "r");

    int seed, size;
    double minCost, a, b, c;

    fscanf(inputFile, "%d", &seed);
    fscanf(inputFile, "%lf", &minCost);
    fscanf(inputFile, "%lf", &a);
    fscanf(inputFile, "%lf", &b);
    fscanf(inputFile, "%lf", &c);
    fscanf(inputFile, "%d", &size);

    int* v = (int*)malloc(size * sizeof(int));

    for (int i = 0; i < size; i++) {
        fscanf(inputFile, "%d", &v[i]);
    }

    fclose(inputFile);

    int breaks = determineBreaks(v, size);

    int minPartitionSize = calculateMinPartition(v, size, minCost, a, b, c, seed);
    int minBreaks = calculateMinBreaks(v, size, minCost, minPartitionSize, a, b, c, seed);

    free(v);
    return 0;
}
