#include <iostream>
#include <ctime>
#include <cstdlib>
#include <float.h>
#include<immintrin.h>
#include <cblas.h>

using namespace std;

void showMatrix(float* matrix, size_t size) {
    size_t i, j;
    for (i = 0; i < size; ++i) {
        for (j = 0; j < size; ++j) {
            cout << matrix[i * size + j] << " ";
        }
        cout << endl;
    }
    cout << "\n\n\n";
}

float* GenerateMatrix(int N) {
    float* A = new float[N * N];
    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < N; j++) {
            A[i * N + j] = (float)(rand() % 5);
        }
    }
    return A;
}

float MaxColumnCount(float* A, const int N) {
    float* res = new float[N];
    for (int i = 0; i < N; i++) {
        res[i] = cblas_sasum(N, &A[i], N);
    }
    return res[cblas_isamax(N, res, 1)];
}

float MaxRowCount(float* A, const int N) {
    float* res = new float[N];
    for (int i = 0; i < N; i++) {
        res[i] = cblas_sasum(N, &A[i * N], 1);
    }
    return res[cblas_isamax(N, res, 1)];
}

float* GenerateMatrixB(float* A, int N, float* I) {
    float* B = new float[N * N];
    float maxRowCount = MaxRowCount(A, N);
    float maxColumnCount = MaxColumnCount(A, N);
    cout << maxRowCount << " " << maxColumnCount << "\n" << endl;
    float divider = 1 / (maxRowCount * maxColumnCount);
    cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasTrans, N, N, N, divider, I, N, A, N, 0.0, B, N);
    return B;
}

float* GenerateMatrixI(int N) {
    float* I = new float[N * N];
    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < N; j++) {
            if (i == j)
                I[i * N + j] = 1.0;
            else 
                I[i * N + j] = 0.0;
        }
    }
    return I;
}

float* GenerateMatrixR(float* A, float* I, float* B, int N) {
    float* multed = new float[N * N];
    for (size_t i = 0; i < N * N; i++) multed[i] = 0;
    cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, N, N, N, 1.0, B, N, A, N, 0.0, multed, N);
    cblas_saxpy(N * N, -1.0, multed, 1, I, 1);
    delete[] multed;
    return I;
}

void CopyMatrix(float* dest, float* src, int N) {
    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < N; j++) {
            dest[i * N + j] = src[i * N + j];
        }
    }
}


float* GetInversedMatrix(float* A, int N, int M){
    float* I = GenerateMatrixI(N);
    float* B = GenerateMatrixB(A, N, I);
    float* R = GenerateMatrixR(A, I, B, N);
    float* tmp = new float[N * N];
    float* res = new float[N * N];
    for (size_t i = 0; i < N * N; i++) res[i] = 0;

    float* result = new float[N * N];
    for (size_t i = 0; i < N * N; i++) result[i] = 0;
    CopyMatrix(tmp, R, N);
    cblas_saxpy(N * N, 1.0, I, 1, result, 1);
    for (size_t i = 0; i < M; i++) {
        cblas_saxpy(N * N, 1.0, tmp, 1, result, 1);
        cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, N, N, N, 1.0, R, N, tmp, N, 0.0, res, N);
        CopyMatrix(tmp, res, N);
    }
    cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, N, N, N, 1.0, result, N, B, N, 0.0, res, N);
    delete[] B;
    delete[] R;
    return res;
}

int main() {
    const int M = 10;
    const int N = 2048;
    srand(time(0));
    float* A = GenerateMatrix(N);
    showMatrix(A, N);
    float* InversedA = GetInversedMatrix(A, N, M);

    showMatrix(InversedA, N);
    double total_time = (clock() / CLOCKS_PER_SEC);
    cout  << "Total time: " << total_time << endl;
    delete[] A;
    delete[] InversedA;
    return 0;
}