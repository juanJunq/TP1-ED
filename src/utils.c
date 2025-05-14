#include "utils.h"
#include <stdlib.h>

int medianOf3(int a, int b, int c)
{
    if ((a <= b) && (b <= c)) return b; 
    if ((a <= c) && (c <= b)) return c; 
    if ((b <= a) && (a <= c)) return a; 
    if ((b <= c) && (c <= a)) return c; 
    if ((c <= a) && (a <= b)) return a; 
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
        while(p1 == p2)
        {
            p1 = (int)(drand48() * size);
            p2 = (int)(drand48() * size);
        }
        temp = v[p1];
        v[p1] = v[p2];
        v[p2] = temp;
        p1 = p2 = 0;
    }

    return 0;
}

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
    return;
}