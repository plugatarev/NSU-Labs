#include<stdio.h>
#include<stdlib.h>
//#pragma warning(disable : 4996)

void swap(int* a, int* b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void heapify(int size, int i,int* num) {
    while (2 * i + 1 < size) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        //Находим максимум из потомков
        int point = left;
        if (right < size && num[right] > num[left]) point = right;

        if (num[i] >= num[point]) break;
        swap(&num[i], &num[point]);
        
        //Строим кучу поддерева(Если поменяли)
        i = point;
    }
}

void heap_sort(int* num, int size) {
    int i;

    //Построение двоичной кучи(i - родитель)
    for (i = size / 2 - 1; i >= 0; --i) {
        heapify(size, i, num);
    }

    for (i = size - 1; i > 0; --i) {
        swap(&num[0], &num[i]);
        heapify(i, 0, num);
    }
}

int main(void) {
	FILE* f = fopen("in.txt", "rt");
	if (f == NULL) return 0;

    int size = 0;
    if (!fscanf(f, "%d", &size)) {
        fclose(f);
        return 0;
    }

    int* num = (int*)malloc(size * sizeof(int));
    if (num == NULL) {
        fclose(f);
        return 0;

    }
    for (int i = 0; i < size; i++) {
        if (!fscanf(f, "%d", &num[i])) {
            free(num);
            fclose(f);
            return 0;
        }
    }
    heap_sort(num, size);

    for (int i = 0; i < size; i++) {
        printf("%d ",num[i]);
    }
    free(num);
	fclose(f);
	return  EXIT_SUCCESS;
}
