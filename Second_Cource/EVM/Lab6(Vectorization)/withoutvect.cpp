#include <iostream>
#include <ctime>
#include <cstdlib>
#include <float.h>
#include<immintrin.h>
const int M = 1000000;
const int N = 4;
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

void SubtractMatrix(float* M1, float* M2, float* M3, int N) {
    for (size_t i = 0; i < N; i++)
        for (size_t j = 0; j < N; j++)
            M3[i * N + j] = M1[i * N + j] - M2[i * N + j];
}

float* MultiplicateMatrix(float* M1, float* M2, int N) {
    float* M3 = new float[N * N];
    for (size_t i = 0; i < N; ++i) {
        float* c = M3 + i * N;
        for (size_t j = 0; j < N; ++j) c[j] = 0;
        for (size_t k = 0; k < N; ++k) {
            const float* b = M2 + k * N;
            float a = M1[i * N + k];
            for (size_t j = 0; j < N; ++j) c[j] += a * b[j];
        }
    }
    return M3;
}

void SumMatrix(float* M1, float* M2, float* result, int N) {
    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < N; j++) {
            result[i * N + j] = M1[i * N + j] + M2[i * N + j];
        }
    }
}

float* GenerateMatrix(int N) {
    float* A = new float[N * N];
    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < N; j++) {
            A[i * N + j] = rand() % 4;
        }
    }
    return A;
}

float MaxLineCount(float* matrix, int N) {
    float maximum = FLT_MIN;
    for (size_t i = 0; i < N; i++) {
        float temp = 0;

        for (size_t j = 0; j < N; j++) {
            temp += matrix[i * N + j];
        }

        if (temp > maximum) {
            maximum = temp;
        }
    }
    return maximum;
}

void TransposeMatrix(float* transpMatrix, float* matrix, int N) {
    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < N; j++) {
            transpMatrix[i * N + j] = matrix[j * N + i];
        }
    }
}

float* GenerateMatrixB(float* A, int N) {
    float* B = new float[N * N];
    float* transposedA = new float[N * N];
    TransposeMatrix(transposedA, A, N);
    float maxRowCount = MaxLineCount(A, N);
    float maxColumnCount = MaxLineCount(transposedA, N);
    float divider = 1 / (maxRowCount * maxColumnCount);
    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < N; j++) {
            B[i * N + j] = transposedA[i * N + j] * divider;
        }
    }
    delete[] transposedA;
    return B;
}

float* GenerateMatrixI(int N) {
    float* I = new float[N * N];
    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < N; j++) {
            if (i == j) I[i * N + j] = 1.0;
            else I[i * N + j] = 0.0;
        }
    }
    return I;
}

float* GenerateMatrixR(float* A, float* I, float* B, int N) {
    float* R = new float[N * N];
    float* multed = new float[N * N];
    multed = MultiplicateMatrix(B, A, N);
    SubtractMatrix(I, multed, R, N);
    delete[] multed;
    return R;
}

void CopyMatrix(float* IN, float* OUT, int N) {
    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < N; j++) {
            IN[i * N + j] = OUT[i * N + j];
        }
    }

}

float* GetInversedMatrix(float* A, int N, int M) {
    float* I = GenerateMatrixI(N);
    float* B = GenerateMatrixB(A, N);
    float* R = GenerateMatrixR(A, I, B, N);
    float* Rn = R;
    float* t;
    SumMatrix(I, Rn, I, N);
    for (size_t i = 0; i < M; i++) {
        t = MultiplicateMatrix(Rn, R, N);
        if (i != 0) delete Rn;
        Rn = t;
        SumMatrix(I, Rn, I, N);
    }
    I = MultiplicateMatrix(I, B, N);
    delete B;
    delete R;
    return I;
}


int main() {
    srand(time(0));
    clock_t a = clock();
    float* A = GenerateMatrix(N);
    //showMatrix(A, N);
    float* InversedA = GetInversedMatrix(A, N, M);

    //showMatrix(InversedA, N);
    delete[] A;
    delete[] InversedA;
    cout << clock() - a << endl;
    return 0;
}