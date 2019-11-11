/*

   Thanks to Prof. Henrique Cota!

*/

#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <stdlib.h>


void swap(int *a, int i, int j)
{
    int t = a[i];
    a[i] = a[j];
    a[j] = t;
}

int partition(int *a, int left,int right,int pivot)
{
    int pos, i;
    swap(a, pivot, right);
    pos = left;
    for(i = left; i < right; i++)
    {
        if (a[i] < a[right])
        {
            swap(a, i, pos);
            pos++;
        }
    }
    swap(a, right, pos);
    return pos;
}

void quickParallel(int *a, int left, int right, int stop)
{
    if (left < right)
    {
        int pivot = (left + right) / 2;
        int pos = partition(a,left,right,pivot);
        if (stop > 1) {     // chamadas paralelizadas
          quickParallel(a, left, pos - 1, stop);
          quickParallel(a, pos + 1, right, stop );
        }
        else {              // chamadas sequenciais
          quickParallel(a, left, pos - 1, stop);
          quickParallel(a, pos + 1, right, stop);
        }
    }
}

void quickSequential(int *a, int left, int right)
{
    if (left < right)
    {
        int pivot = (left + right) / 2;
        int pos = partition(a,left,right,pivot);

        quickSequential(a, left, pos - 1);
        quickSequential(a, pos + 1, right);
    }
}


int IsSort(int *array, int size) {
	int i;
	for(i = 1; i < size; i++)
		if(array[i-1] > array[i])
			return 0;
	return 1;
}


int main(int argc, char** argv) {
	int size = 1000000, i, *array;

	array = malloc(size* sizeof(int));
	srand(time(NULL));
	for (i = 0; i < size; i++)
		array[i] = rand()%size;
	double start, end;
	int MAX_THREADS = 4096;     // Caution!

	for (int threads =  1; threads <= MAX_THREADS; threads *= 2) {
        omp_set_num_threads(threads);   // threads equals to 1 should be sequential...
        for (int nested = 0; nested <= 1; nested++) {  // false or true
            omp_set_nested( nested );
            for (int stop = 2; stop <= 64; stop *= 2) {
                start = omp_get_wtime();
                quickParallel(array, 0, size - 1, stop);
                end = omp_get_wtime();

                printf("Tempo: %.3f threads: %d  nested: %d stop: %d\n", end - start, threads, nested, stop);
                if(IsSort(array, size) == 1)
                    printf("Result: Sorted\n");
                else
                    printf("Result: Not Sorted\n");
            }
        }
	}
	return 0;
}





