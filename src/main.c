#include <stdio.h>
#include <stdlib.h>
#include "../include/sort.h"
#include "../include/calibration.h"
#include "../include/stats.h"

static int loadInputData(const char* file, int* seed, double* minCost, double* a, double* b, double* c, int* size, int** v)
{
    FILE* inputFile = NULL;
    // Open input file passed as command line argument
    inputFile = fopen(file, "r");
    // Ensure the input file was opened successfully
    if(inputFile == NULL)
    {
        perror("Error opening input file");
        return 1;
    }
    // Read seed, cost threshold and weights from input
    if (fscanf(inputFile, "%d %lf %lf %lf %lf %d", seed, minCost, a, b, c, size) != 6) 
    {
        fprintf(stderr, "Error: Input file has incorrect format for configuration parameters.\n");
        fclose(inputFile); 
        return 1;
    }

    *v = (int*)malloc(*size * sizeof(int));
    if(*v == NULL)
    {
        fprintf(stderr, "Error: Memory allocation failed\n");
        fclose(inputFile);
        return 1;
    }

    for (int i = 0; i < *size; i++) 
    {
        // Validate that all configuration parameters were read
        if (fscanf(inputFile, "%d", &((*v)[i])) != 1) 
        {
            fprintf(stderr, "Error: Failed to read element from input file\n");
            free(*v); 
            *v = NULL; // Prevents possible double free
            fclose(inputFile);
            return 1; 
        }
    }
    fclose(inputFile);
    return 0;
}

int main(int argc, char** argv)
{
    // Check if the input file was provided as an argument
    if(argc < 2)
    {
        fprintf(stderr, "Usage: %s input.txt\n", argv[0]);
        return 1;
    }
    
    int seed, size;
    double minCost, a, b, c;
    int* v = NULL;

    // Check if data was successfully loaded
    if(loadInputData(argv[1], &seed, &minCost, &a, &b, &c, &size, &v) != 0)
    {
        free(v);
        return 1;
    }

    int breaks = determineBreaks(v, size);
    printf("size %d seed %d breaks %d\n\n", size, seed, breaks);

    // Phase 1: Find best partition size based on cost convergence    
    int minPartitionSize = calculateMinPartition(v, size, minCost, a, b, c, seed);
    // Phase 2: Find break threshold where insertion sort is better than quicksort
    calculateMinBreaks(v, size, minCost, minPartitionSize, a, b, c, seed);

    free(v);
    return 0;
}
