#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <mpi.h>

#define E 0.00001
#define MAX_ITERATIONS 100000
int SIZE = 1000;

int is_digit(char* s){
    for (int i = 0; s[i] != '\0'; i++){
	    if (!(s[i] >= '0' && s[i] <= '9')) return 0;
    }
    return 1;
}

double* getVectorB(){
    double* b = (double*)malloc(SIZE * sizeof(double));
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
    for (int i = 0; i < size; ++i) {
        numberOfRows[i] = count1 + (i < count2);
    }
    return numberOfRows;
}

int* getShift(int size, int* numberOfRows){
    int* shift = (int*)calloc(size, sizeof(int));
    for (int i = 1; i < size; ++i) {
        shift[i] = shift[i - 1] + numberOfRows[i - 1];
    }
    return shift;
}

double scalar(double* v1, double* v2, int size) {
    double result = 0;
    for (int i = 0; i < size; i++) {
        result += v1[i] * v2[i];
    }
    return result;
}

double norm(double* vector) {
    double result = 0;
    for (int i = 0; i < SIZE; i++) {
        result += vector[i] * vector[i];
    }
    return sqrt(result);
}

void fill_matrix(double* matrix, int shift, int numberOfElem) {
    for (int i = 0; i < numberOfElem; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (shift + i == j) matrix[i * SIZE + j] = 2.0;
            else matrix[i * SIZE + j] = 1.0;
        }
    }
}

void mulMatrixOnVector(double* A_MPI, double* v, double* res, int numberOfElem, int length, int shift) {
    for (int i = 0; i < numberOfElem; i++){
        res[i] += scalar(&A_MPI[i * SIZE + shift], v, length);
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

void cyclicShift(int* length, int* shift, int size, int rank){
    if (rank == 0){
        int tmp1 = length[0];
        int tmp2 = shift[0];
        for (int j = 0; j < size - 1; j++){
            length[j] = length[j + 1];
            shift[j] = shift[j + 1];
        }
        length[size - 1] = tmp1;
        shift[size - 1] = tmp2;
    }
    MPI_Bcast(length, size, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(shift, size, MPI_INT, 0, MPI_COMM_WORLD);
}

int main(int argc, char *argv[]) {
    int size, rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size); //получение числа процессов
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); //получение номера процесса
    if (rank == 0){
        if (argc > 1 && is_digit(argv[1]))
        SIZE = atoi(argv[1]);
    }
    MPI_Bcast(&SIZE, 1, MPI_INT, 0, MPI_COMM_WORLD);
    int* numberOfRows = getNumberOfRows(size); //количество строк в процессе
    int* length = getNumberOfRows(size);
    for (int i = 0; i < size; i++) length[i] = numberOfRows[i];
    int* shift = getShift(size, numberOfRows); //сдвиг диагонали
    double* b = getVectorB(numberOfRows[rank]);
    int max_rows = numberOfRows[0];
    double* x = getVectorX(max_rows);
    double* A_MPI = getArray(SIZE * numberOfRows[rank]);
    double* y_MPI = getArray(numberOfRows[rank]);
    double* Ax_MPI = getArray(numberOfRows[rank]);
    fill_matrix(A_MPI, shift[rank], numberOfRows[rank]);
    double b_MPI_norm = scalar(b, b, numberOfRows[rank]);
    double normB = 0;
    MPI_Allreduce(&b_MPI_norm, &normB, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
    normB = sqrt(normB);
    double tetta = 0.00001;
    double condition = 1;
    int iterations = 0;    

    double startTime = MPI_Wtime();
    while (condition > E && iterations++ < MAX_ITERATIONS) { 
        for (int i = 0; i < numberOfRows[rank]; i++) Ax_MPI[i] = 0;
        for (int i = 0; i < size; i++){
            mulMatrixOnVector(A_MPI, x, Ax_MPI, numberOfRows[rank], length[rank], shift[rank]);
            MPI_Sendrecv_replace(x, max_rows, MPI_DOUBLE, (rank + size - 1) % size, 0, (rank + 1) % size, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            cyclicShift(length, shift, size, rank);
        }
        for (int i = 0; i < numberOfRows[rank]; i++){
            y_MPI[i] = Ax_MPI[i] - b[i]; // y=Ax-b
        }
        for (int i = 0; i < numberOfRows[rank]; i++) {
            x[i] -= tetta * y_MPI[i]; // x(n+1) = x(n)-tetta(Ax(n)-b)
        }
        double y_MPI_norm = scalar(y_MPI, y_MPI, numberOfRows[rank]);
        double y_norm = 0;
        MPI_Allreduce(&y_MPI_norm, &y_norm, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
        condition = sqrt(y_norm) / normB;
    }
    double* xf = getArray(SIZE);
    MPI_Allgatherv(x, numberOfRows[rank], MPI_DOUBLE, xf, numberOfRows, shift, MPI_DOUBLE, MPI_COMM_WORLD);
    if (rank == 0) {
        printf("Execution time %lf \n", MPI_Wtime() - startTime);
        printf("X = ");
        printVector(xf, SIZE);
    }
    free(shift);
    free(numberOfRows);
    free(Ax_MPI);
    free(A_MPI);
    free(y_MPI);
    free(b);       
    free(x);
    free(xf);
    free(length);
    MPI_Finalize();
    return 0;
}