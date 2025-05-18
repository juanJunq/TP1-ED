#ifndef CALIBRATION_H
#define CALIBRATION_H

typedef struct 
{
    int mps;
    double cost;
} mpsCost;

typedef struct
{
    int breaks;
    double qsCost;
    double inCost;
} breakCost;

int calculateMinPartition(int* v, int size, int minCost, double a, double b, double c, int seed);
void adjustSearchRange(int minPartitionSize, int* minMPS, int* maxMPS, int* steps, int* mps, int numMPS);

int calculateMinBreaks(int* v, int size, double minCost, int minPartitionSize, double a, double b, double c, int seed);

#endif