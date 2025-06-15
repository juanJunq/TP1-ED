#include "utils.h"
#include <stdlib.h>

int medianOf3(int a, int b, int c)
{
    // Checks all permutations to find the median
    if ((a <= b) && (b <= c)) return b; 
    if ((a <= c) && (c <= b)) return c; 
    if ((b <= a) && (a <= c)) return a; 
    if ((b <= c) && (c <= a)) return c; 
    if ((c <= a) && (a <= b)) return a; 
    // 'return b' acts as a fallback
    return b;                           
}

void swap(int* a, int* b, Stats* count)
{
    int temp = *a;
    *a = *b; 
    *b = temp;
    count->moves += 3;
}

int shuffleVector(int* v, int size, int numShuffle)
{   
    int p1 = 0, p2 = 0, temp;
    
    for(int t = 0; t < numShuffle; t++)
    {
        // Ensure two distinct random positions are selected
        while(p1 == p2)
        {
            // Generate random index p1 and p2
            p1 = (int)(drand48() * size);
            p2 = (int)(drand48() * size);
        }
        // Swap elements to create one local break        
        temp = v[p1];
        v[p1] = v[p2];
        v[p2] = temp;
        // p1 = p2 = 0 reset at the end of the previous iteration ensures the while loop is entered
        p1 = p2 = 0;
    }

    return 0;
}

// Used to create a fully sorted array (0 breaks) before applying a controlled number of shuffles
// Operations in this sort are NOT counted towards the main algorithm statistics
void sortArray(int* v, int size)
{
    int aux, j;
    for(int i = 1; i < size; i++)
    {
        aux = v[i];
        j = i - 1;
        while((j >= 0) && (v[j] > aux))
        {
            v[j + 1] = v[j];
            j--;
        }
        v[j + 1] = aux;
    }
}