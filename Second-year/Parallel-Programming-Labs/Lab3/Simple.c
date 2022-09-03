#include <stdio.h>
#include <stdlib.h>

int N = 3;
int K = 3;
int M = 3;

void printMatrix(double* matrix, int n, int m){
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            printf("%lf ", matrix[i * n + j]);
        }
        printf("\n");
    }
}

void matrixMultiply(const double* a, const double* b, double* result) {
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < K; k++) {
                result[i * K + k] += a[i * N + j] * b[j * K + k];
            }
        }
    }
}

void fillMatrix(double* matrix, int n, int m) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            matrix[i * m + j] = i * m + j;
        } 
    }
}

int isDigit(char* s){
    for (int i = 0; s[i] != '\0'; i++){
	    if (!(s[i] >= '0' && s[i] <= '9')) return 0;
    }
    return 1;
}

int main(int argc, char *argv[]) {
    if (argc == 4 && isDigit(argv[1]) && isDigit(argv[2]) && isDigit(argv[3])){
        N = atoi(argv[1]);
        K = atoi(argv[2]);
        M = atoi(argv[3]);
    }
    double* A = (double*)malloc(M * N * sizeof(double));
    double* B = (double*)malloc(K * N * sizeof(double));
    double* C = (double*)malloc(K * M * sizeof(double));
    fillMatrix(A, M, N);
    fillMatrix(B, N, K);
    matrixMultiply(A, B, C);
    printMatrix(C, M, K);
    free(A);
    free(B);
    free(C);
    return 0;
}