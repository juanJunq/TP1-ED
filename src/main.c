#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include "../include/sort.h"
#include "../include/calibration.h"
#include "../include/stats.h"

int main()
{
    int seed, size;
    double minCost, a, b, c;

    scanf("%d", &seed);
    scanf("%lf", &minCost);
    scanf("%lf", &a);
    scanf("%lf", &b);
    scanf("%lf", &c); 
    scanf("%d", &size);

    int* v = (int*)malloc(size * sizeof(int));
    
    for(int i = 0; i < size; i++)
    {
        scanf("%d", &v[i]);
    }

    int breaks = determineBreaks(v, size);
    printf("size %d seed %d breaks %d\n", size, seed, breaks);

    int minPartitionSize = calculateMinPartition(v, size, minCost, a, b, c, seed);

    int minBreaks = calculateMinBreaks(v, size, minCost, minPartitionSize, a, b, c, seed);

    free(v);
    int i;
    scanf("%d", &i);
    return 0;
}