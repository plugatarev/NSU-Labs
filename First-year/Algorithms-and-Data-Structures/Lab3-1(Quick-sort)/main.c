#include<stdio.h>
#include <malloc.h>
#include <stdlib.h>
//#pragma warning(disable : 4996)

void swap(int* a, int* b) {
    int tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}

void quicksort(int* num, int left, int right) {
    int pivot = num[(left + right) / 2];
    int i = left, j = right;
     while (i < j) {
        while (num[i] < pivot) i++;
        while (num[j] > pivot) j--;
        
            if (num[i] > num[j]) {
                swap(&num[i], &num[j]);
            }
            i++;
            j--;
    }
    if (left < j) quicksort(num, left, j);
    if (i < right) quicksort(num, i, right);
}

int main(void) {
    FILE* f = fopen("in.txt", "rt");

    if (f == NULL) {
        return 0;
    }

    int size = 0;
    if (!fscanf(f, "%d", &size)) {
        fclose(f);
        return 0;
    }
    int* num = (int*)malloc(size * sizeof(int));

    for(int i = 0; i < size; i++) {
        if (!fscanf(f, "%d", &num[i])) {
            free(num);
            fclose(f);
            return 0;
        }
    }
    
    int left = 0; int right = size - 1;

    quicksort(num, left, right);

    for (int i = 0; i < size; i++) {
        printf("%d ", num[i]);
    }

    free(num);
    fclose(f);

    return EXIT_SUCCESS;
}
