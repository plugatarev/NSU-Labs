#include <stdio.h>
#include "mpi.h"
#include <math.h>
#include <stdlib.h>
#define DIMENSION 3
#define Z 0
#define X 1
#define Y 2
#define A 1e5
#define E 1e-8
#define MAX_ITERATIONS 1000
#define FALSE 0
#define TRUE 1
#define UPPER 0
#define LOWER 1

int function_calculated;
int BEFORE = 0;
int AFTER = 1;
double h[DIMENSION];
double h_pow[DIMENSION];
const int N[DIMENSION] = {259, 37, 100}; // Oz, Ox, Oy
const int D[DIMENSION] = {2, 2, 2};
double f_i, f_j, f_k;

double phi(double x, double y, double z){
    return x * x + y * y + z * z;
}

double ro(double x, double y, double z){
    return (6 - A * phi(x, y, z));
}

int condition(double** F, int i, int j, int k){
    if (fabs(F[AFTER][i * N[X] * N[Y] + j * N[X] + k] - F[BEFORE][i * N[X] * N[Y] + j * N[X] + k]) > E){
        return FALSE;
    }
    return TRUE;
}

double getDifferentFunction(double** F, const int* shifts, int rank, const int* numberOfLayer){
    double max = 0;
    double tmp = 0;
    for (int i = 1; i < numberOfLayer[rank] - 1; i++){
        for (int j = 1; j < N[Y]; j++){
            for (int k = 1; k < N[X]; k++){
                tmp = fabs(F[AFTER][i * N[X] * N[Y] + j * N[X] + k] - phi(k * h[X], j * h[Y], (i + shifts[rank]) * h[Z]));
                if (tmp > max){
                    max = tmp;
                    printf("%lf %lf\n", F[AFTER][i * N[X] * N[Y] + j * N[X] + k], phi(k * h[X], j * h[Y], (i + shifts[rank]) * h[Z]));
                }
                
            
            }
        }
    }
    double resMax = 0;
    MPI_Allreduce(&max, &resMax, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);
    return resMax;
}

double getPhi(double** F, int z, int y, int x){
    return F[BEFORE][z * N[X] * N[Y] + y * N[X] + x];
}

int calculateInsideArea(double** F, const int* shifts, int rank, double coefficient, const int* numberOfLayer){
    for (int i = 1; i < numberOfLayer[rank] - 1; i++){
        for (int j = 1; j < N[Y] - 1; j++){
            for (int k = 1; k < N[X] - 1; k++){
                f_i = (getPhi(F, i, j, k + 1) + getPhi(F, i, j, k - 1)) / h_pow[X];
                f_j = (getPhi(F, i, j + 1, k) + getPhi(F, i, j - 1, k)) / h_pow[Y];
                f_k = (getPhi(F, i + 1, j, k) + getPhi(F, i - 1, j, k)) / h_pow[Z];
                F[AFTER][i * N[X] * N[Y] + j * N[X] + k] = coefficient * (f_i + f_j + f_k - ro(k * h[X], j * h[Y], (i + shifts[rank]) * h[Z]));
                if (function_calculated == TRUE && condition(F, i, j, k) == FALSE) function_calculated = FALSE;
            }
        }
    }
}

void initFunctionValuesBounds(double** F, const int* shift, const int rank, const int* numberOfLayer){
    int layer_number = shift[rank];
    for (int i = 0; i < numberOfLayer[rank]; i++, layer_number++){
        for (int j = 0; j < N[Y]; j++){
            for (int k = 0; k < N[X]; k++){
                if (layer_number != 0 && j != 0 && k != 0 && layer_number != (N[Z] - 1) && j != (N[Y] - 1) && k != (N[X] - 1)){
                    F[BEFORE][i * N[X] * N[Y] + j * N[X] + k] = 0;
                    F[AFTER][i * N[X] * N[Y] + j * N[X] + k] = 0;
                }
                else{
                    double value = phi(k * h[X], j * h[Y], layer_number * h[Z]);
                    F[BEFORE][i * N[X] * N[Y] + j * N[X] + k] = value;
                    F[AFTER][i * N[X] * N[Y] + j * N[X] + k] = value;
                }
            }
        }
    }
}

void printMatrix(const double* m, const int x, const int y, const int z){
    for (int i = 0; i < z; i++){
        for (int j = 0; j < y; j++){
            for (int k = 0; k < x; k++){
                printf("%lf ", m[i * x * y + j * x + k]);
            }
            printf("\n");
        }
        printf("\n\n");
    }
}

void borderCalculate(double** F, double** buffer, double coefficient, int rank, int numOfProc, const int* numberOfLayer, const int* shifts){
    for (int j = 1; j < N[Y] - 1; j++){
        for (int k = 1; k < N[X] - 1; k++){
            if (rank > 0){
                int i = 0;
                f_i = (getPhi(F, i, j, k + 1) + getPhi(F, i, j, k - 1)) / h_pow[X];
                f_j = (getPhi(F, i, j + 1, k) + getPhi(F, i, j - 1, k)) / h_pow[Y];
                if (numberOfLayer[rank] > 1) f_k = (buffer[0][j * N[X] + k] + getPhi(F, i + 1, j, k)) / h_pow[Z];
                else f_k = (buffer[0][j * N[X] + k] + buffer[1][j * N[X] + k]) / h_pow[Z];
                F[AFTER][i * N[X] * N[Y] + j * N[X] + k] = coefficient * (f_i + f_j + f_k - ro(j * h[X], k * h[Y], (i + shifts[rank]) * h[Z]));
                if (function_calculated == TRUE && condition(F, i, j, k) == FALSE) function_calculated = FALSE;
            }
            if (rank < numOfProc - 1){
                int i = numberOfLayer[rank] - 1;
                f_i = (getPhi(F, i, j, k + 1) + getPhi(F, i, j, k - 1)) / h_pow[X];
                f_j = (getPhi(F, i, j + 1, k) + getPhi(F, i, j - 1, k)) / h_pow[Y];
                if (numberOfLayer[rank] > 1) f_k = (getPhi(F, i - 1, j, k) + buffer[1][j * N[X] + k]) / h_pow[Z];
                else f_k = (buffer[0][j * N[X] + k] + buffer[1][j * N[X] + k]) / h_pow[Z];
                F[AFTER][i * N[X] * N[Y] + j * N[X] + k] = coefficient * (f_i + f_j + f_k - ro(j * h[X], k * h[Y], (i + shifts[rank]) * h[Z]));
                if (function_calculated == TRUE && condition(F, i, j, k) == FALSE) function_calculated = FALSE;
            }
        }
    }
}

void sendBoundlyLayer(double** F, double** buffer, int rank, const int* numberOfLayer, int numOfProc, MPI_Request* send, MPI_Request *receive){
    if (rank > 0){
        MPI_Isend(&(F[BEFORE][0]), N[X] * N[Y], MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD, &send[0]);
        MPI_Irecv(buffer[UPPER], N[X] * N[Y], MPI_DOUBLE, rank - 1, 1, MPI_COMM_WORLD, &receive[1]);
    }
    if (rank < numOfProc - 1){
        MPI_Isend(&(F[BEFORE][(numberOfLayer[rank] - 1) * N[X] * N[Y]]), N[X] * N[Y], MPI_DOUBLE, rank + 1, 1, MPI_COMM_WORLD, &send[1]);
        MPI_Irecv(buffer[LOWER], N[X] * N[Y], MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD, &receive[0]);
    }
}

void wait(int rank, int numOfProc, MPI_Request *send, MPI_Request* receive){
    if (rank > 0){
        MPI_Wait(&receive[1], MPI_STATUS_IGNORE);
        MPI_Wait(&send[0], MPI_STATUS_IGNORE);
    }
    if (rank < numOfProc - 1){ 
        MPI_Wait(&receive[0], MPI_STATUS_IGNORE);
        MPI_Wait(&send[1], MPI_STATUS_IGNORE);
    }
}

int* getNumberOfLayer(int size){
    int* numberOfLayer = (int*)calloc(size, sizeof(int));
    int count1 = N[Z] / size;
    int count2 = N[Z] % size;
    for (int i = 0; i < size; i++){
        numberOfLayer[i] = count1 + (i < count2);
    }
    return numberOfLayer;
}

int* getShift(int size, int* numberOfRows){
    int* shift = (int*)calloc(size, sizeof(int));
    for (int i = 1; i < size; i++){
        shift[i] = shift[i - 1] + numberOfRows[i - 1];
    }
    return shift;
}

int main(int argc, char* argv[]){
    int size, rank;
    double phi_start = 0;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
    h[DIMENSION], h_pow[DIMENSION];
    for (int i = 0; i < DIMENSION; i++){
        h[i] = (double)D[i] / (N[i] - 1);
        h_pow[i] = h[i] * h[i];
    }
    int* numberOfLayer = getNumberOfLayer(size);
    int* shift = getShift(size, numberOfLayer);
    double coefficient = 1 / (2 / h_pow[X] + 2 / h_pow[Y] + 2 / h_pow[Z] + A);
    double* F[2];
    F[BEFORE] = (double*)malloc(N[X] * N[Y] * numberOfLayer[rank] * sizeof(double));
    F[AFTER] = (double*)malloc(N[X] * N[Y] * numberOfLayer[rank] * sizeof(double));
    double* buffer[2];
    buffer[UPPER] = (double*)malloc(N[X] * N[Y] * sizeof(double));
    buffer[LOWER] = (double*)malloc(N[X] * N[Y] * sizeof(double));
    initFunctionValuesBounds(F, shift, rank, numberOfLayer);
    MPI_Request send[2] = {};
    MPI_Request receive[2] = {};
    double startTime = MPI_Wtime();
    int i = 0;
    while (!function_calculated && i++ < MAX_ITERATIONS){
        function_calculated = TRUE;
        sendBoundlyLayer(F, buffer, rank, numberOfLayer, size, send, receive);
        calculateInsideArea(F, shift, rank, coefficient, numberOfLayer);
        wait(rank, size, send, receive);
        borderCalculate(F, buffer, coefficient, rank, size, numberOfLayer, shift);
        int minF;
        MPI_Allreduce(&function_calculated, &minF, 1, MPI_INT, MPI_MIN, MPI_COMM_WORLD);
        function_calculated = minF;
        BEFORE = !BEFORE;
        AFTER = !AFTER;
    }
    for (int i = 0; i < size; i++){
        if (rank == i) printMatrix(F[AFTER], N[X], N[Y], numberOfLayer[rank]);
        MPI_Barrier(MPI_COMM_WORLD);
    }
    double different = getDifferentFunction(F, shift, rank, numberOfLayer);
    if (rank == 0){
        printf("Execution time: %lf\nIterations = %d\n", MPI_Wtime() - startTime, i);
        printf("Different = %lf\n", different);
    }
    free(numberOfLayer);
    free(shift);
    free(F[AFTER]);
    free(F[BEFORE]);
    free(buffer[UPPER]);
    free(buffer[LOWER]);
    MPI_Finalize();
    return 0;
}