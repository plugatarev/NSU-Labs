#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <mpi.h>

#define E 0.00001
#define MAX_ITERATIONS 10000
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

void mulMatrixOnVector(double* A_MPI, double* v, double* res, int numberOfElem) {
    for (int i = 0; i < numberOfElem; i++){
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
    int* shift = getShift(size, numberOfRows); //сдвиг диагонали

    double* b = getVectorB();
    double* x = getVectorX();
    double* A_MPI = getArray(SIZE * numberOfRows[rank]); //кусок матрицы процесса
    double* y_MPI = getArray(numberOfRows[rank]);        //Ax-b  для каждого процесса
    double* Ax = getArray(numberOfRows[rank]);           //произведение Ax
    double* y = getArray(SIZE);                          //Ax-b  собранный
    fill_matrix(A_MPI, shift[rank], numberOfRows[rank]);
    double normB = norm(b);
    double tetta = 0.0001;
    int iterations = 0;
    double condition = 1;
    
    double startTime = MPI_Wtime();
    while (condition > E && iterations++ < MAX_ITERATIONS) {
        mulMatrixOnVector(A_MPI, x, Ax, numberOfRows[rank]); //A*x
        for (int i = 0; i < numberOfRows[rank]; i++){
            y_MPI[i] = Ax[i] - b[i]; // y=Ax-b
        }
        MPI_Allgatherv(y_MPI, numberOfRows[rank], MPI_DOUBLE, y, numberOfRows, shift, MPI_DOUBLE, MPI_COMM_WORLD);
        for (int i = 0; i < numberOfRows[rank]; i++) {
            x[i] -= tetta * y_MPI[i]; // x(n+1) = x(n)-tetta(Ax(n)-b)
        }
        if (rank == 0){
            printf("x");
            for (int j = 0; j < SIZE; j++) printf("%lf ", x[j]);
        }
        if (rank == 0) printf("\n");
        MPI_Allgatherv(x, numberOfRows[rank], MPI_DOUBLE, x, numberOfRows, shift, MPI_DOUBLE, MPI_COMM_WORLD);
        condition = norm(y) / normB;
    }
    if (rank == 0) {
        printf("Execution time %lf \n", MPI_Wtime() - startTime);
        printf("X = ");
        printVector(x, SIZE);
    }
    
    free(x);
    free(b);
    free(A_MPI);
    free(Ax);
    free(y);
    free(y_MPI);
    free(numberOfRows);
    free(shift);

    MPI_Finalize();
    return 0;
}