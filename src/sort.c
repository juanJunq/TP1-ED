#include "sort.h"
#include "utils.h"

void insertionSort(int* v, int l, int r, Stats* count)
{
    count->calls++;
    int aux, j;
    for(int i = l + 1; i <= r; i++)
    {
        count->moves++;
        aux = v[i];
        j = i - 1;
        count->cmp++;
        while((j >= 0) && (v[j] > aux))
        {
            count->cmp++;
            count->moves++;
            v[j + 1] = v[j];
            j--;
        }
        count->moves++;
        v[j + 1] = aux;
    }
    return;
}

void partition(int* v, int l, int r, int* i, int* j, Stats* count)
{
    count->calls++;
    *i = l, *j = r;
    int middle = ((*i + *j) / 2);
    int pivot = medianOf3(v[*i], v[*j], v[middle]);
    do
    {
        while(pivot > v[*i])
        {
            count->cmp++;
            (*i)++;
        }
        count->cmp++;

        while(pivot < v[*j])
        {
            count->cmp++;
            (*j)--;
        }
        count->cmp++;

        if(*i <= *j)
        {
            swap(&v[*i], &v[*j], count);
            (*i)++;
            (*j)--;
        }
    } while (*i <= *j);
    
}

void quickSort(int* v, int l, int r, int minSize, Stats* count)
{
    count->calls++;
    int i, j;
    if(l < r) {
        partition(v, l, r, &i, &j, count);
        if(j - l > 0) {
            if((j - l + 1) <= minSize)
            {
                insertionSort(v, l, j, count);
                return;
            } else
                quickSort(v, l, j, minSize, count);
        }
        if(r - i > 0) {
            if((r - i + 1) <= minSize)
            {    
                insertionSort(v, i, r, count);
                return;
            } else
                quickSort(v, i, r, minSize, count);
        }
    }    
}

void ordenadorUniversal(int* v, int size, int minPartitionSize, int breakThreshold, Stats* count)
{
    int numBreaks = determineBreaks(v, size);
    if(numBreaks < breakThreshold)
        insertionSort(v, 0, size - 1, count);
    else
        quickSort(v, 0, size - 1, minPartitionSize, count);
}
