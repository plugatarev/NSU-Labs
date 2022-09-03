#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#define E 0.00001
#define MAX_ITERATIONS 10000
int SIZE = 17000;

int is_digit(char* s){
    for (int i = 0; s[i] != '\0'; i++){
	    if (!(s[i] >= '0' && s[i] <= '9')) return 0;
    }
    return 1;
}

double* getVectorB(){
    double* b = (double*)malloc(SIZE * sizeof(double));
    #pragma omp parallel for
    for (int i = 0; i < SIZE; i++){
        b[i] = SIZE + 1;
    }
    return b;
}

double* getVectorX(){
    return (double*)calloc(SIZE, sizeof(double));
}

double* getArray(int size){
    double* arr = (double*)calloc(size, sizeof(double));
    return arr;
}

int* getNumberOfRows(int size){
    int* numberOfRows = (int*)calloc(size, sizeof(int));
    int count1 = SIZE / size;
    int count2 = SIZE % size;
    #pragma omp parallel for
    for (int i = 0; i < size; ++i) {
        numberOfRows[i] = count1 + (i < count2);
    }
    return numberOfRows;
}

int* getShift(int size, int* numberOfRows){
    int* shift = (int*)calloc(size, sizeof(int));
    #pragma omp parallel for
    for (int i = 1; i < size; ++i) {
        shift[i] = shift[i - 1] + numberOfRows[i - 1];
    }
    return shift;
}

double scalar(double* v1, double* v2, int size) {
    double result = 0;
    //#pragma omp parallel for reduction(+:result)
    for (int i = 0; i < size; i++) {
        result += v1[i] * v2[i];
    }
    return result;
}

double norm(double* vector) {
    double result = 0;
    #pragma omp parallel for reduction(+:result)
    for (int i = 0; i < SIZE; i++) {
        result += vector[i] * vector[i];
    }
    return sqrt(result);
}

void fill_matrix(double* matrix) {
    #pragma omp parallel for
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (i == j) matrix[i * SIZE + j] = 2.0;
            else matrix[i * SIZE + j] = 1.0;
        }
    }
}

void mulMatrixOnVector(double* A_MPI, double* v, double* res) {
    #pragma omp parallel for
    for (int i = 0; i < SIZE; i++){
        res[i] = scalar(&A_MPI[i * SIZE], v, SIZE);
    }
}

void printVector(double* v, int size){
    for (int i = 0; i < size; ++i){
        printf("%2f ", v[i]);
    }
    printf("\n");
}

void printMatrix(double* matrix, int a, int b){
    for (int i = 0; i < a; i++){
        for (int j = 0; j < b; j++){
            printf("%lf ",matrix[i * a + j]);
        }
        printf("\n");
    }
}
int main(int argc, char *argv[]) {
    if (argc > 1 && is_digit(argv[1]))
    SIZE = atoi(argv[1]);
    double* b = getVectorB();
    double* x = getVectorX();
    double* A = getArray(SIZE * SIZE);
    double* Ax = getArray(SIZE);       //произведение Ax
    double* y = getArray(SIZE);        //Ax-b  собранный
    fill_matrix(A);
    double normB = norm(b);
    double tetta = 0.0001;
    int iterations = 0;
    double condition = 1;
    time_t start = time(NULL);
    while (condition > E && iterations++ < MAX_ITERATIONS) {
        mulMatrixOnVector(A, x, Ax); //A*x
        #pragma omp parallel for
        for (int i = 0; i < SIZE; i++){
            y[i] = Ax[i] - b[i]; // y=Ax-b
        }

        #pragma omp parallel for 
        for (int i = 0; i < SIZE; i++) {
            x[i] -= tetta * y[i]; // x(n+1) = x(n)-tetta(Ax(n)-b)
        }
        condition = norm(y) / normB;
    }
    time_t finish = time(NULL);
    printf("X = ");
    printVector(x, SIZE);
    printf("Execution time %ld \n", (finish - start)); 
    free(x);
    free(A);
    free(b);
    free(Ax);
    free(y);
    return 0;
}