#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int N = 4;
int K = 4;
int M = 4;

void printMatrix(double* matrix, int n, int m){
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            printf("%lf ", matrix[i * m + j]);
        }
        printf("\n");
    }
}

void fillMatrixA(double* matrix, int n, int m) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            matrix[i * m + j] = i * m + j;
        } 
    }
}

void fillMatrixB(double* matrix, int n, int m){
    for (int i = 0; i < n; i++){
        for (int j = 0; j < m; j++){
            if (i == j) matrix[i * m + j] = 2;
        }
    }
}

void fillBArrays(int *elemsNumB, int *shiftsB, const int *dims) {
    for (int i = 0; i < dims[1]; i++) {
        shiftsB[i] = i;
        elemsNumB[i] = 1;
    }
}

void fillCArrays(int *elemsNumC, int *shiftsC, int blockHeight, const int *dims, int grid2DSize) {
    for (int i = 0; i < grid2DSize; i++) {
        elemsNumC[i] = 1;
    }
    for (int i = 0; i < dims[0]; i++) {
        for (int j = 0; j < dims[1]; j++) {
            shiftsC[i * dims[1] + j] = i * dims[1] * blockHeight + j;
        }
    }
}

int isDigit(char* s){
    for (int i = 0; s[i] != '\0'; i++){
	    if (!(s[i] >= '0' && s[i] <= '9')) return 0;
    }
    return 1;
}

void create1DComms(MPI_Comm grid2D, MPI_Comm* columns, MPI_Comm* rows) {
    int rowsDims[2] = {0, 1};
    int columnsDims[2] = {1, 0};
    MPI_Cart_sub(grid2D, columnsDims, columns);
    MPI_Cart_sub(grid2D, rowsDims, rows);
}

void createTypeB(MPI_Datatype* BType, int blockWidth){
    MPI_Type_vector(N, blockWidth, K, MPI_DOUBLE, BType); //N - число блоков(высота), blockWidth - число э-ов в каждом блоке
                                                          //K - расстояние между блоками
    MPI_Type_create_resized(*BType, 0, blockWidth * sizeof(double), BType);
    MPI_Type_commit(BType);
}

void createTypeC(MPI_Datatype* CType, int blockHeight, int blockWidth){
    MPI_Type_vector(blockHeight, blockWidth, K, MPI_DOUBLE, CType);
    MPI_Type_create_resized(*CType, 0, blockWidth * sizeof(double), CType);
    MPI_Type_commit(CType);
}

void multPartsMatrix(const double* a, const double* b, double* result, int n1, int n2){
    for (int i = 0; i < n1; i++) {
        for (int j = 0; j < n2; j++) {
            for (int k = 0; k < N; k++) {
                result[i * n2 + j] += a[i * N + k] * b[k * n2 + j];
            }
        }
    }
}

void matrixMultiply(double* A, double* B, double* C, int* dims, int size, int rank, MPI_Comm comm2d){
    int blockHeight = M / dims[0];
    int blockWidth = K / dims[1];
    int coordinatesOfProc[2];
    MPI_Cart_coords(comm2d, rank, 2, coordinatesOfProc); //Декартовы координаты соответствующего процесса
    int* elemsNumB = NULL;
    int* shiftsB = NULL;
    int* elemsNumC = NULL;
    int* shiftsC = NULL;
    MPI_Datatype BType, CType;
    if (rank == 0) {
        elemsNumB = (int*)malloc(dims[1] * sizeof(int)); //Число элементов в куске
        shiftsB = (int*)malloc(dims[1] * sizeof(int));   //Отступ
        elemsNumC = (int*)malloc(size * sizeof(int));
        shiftsC = (int*)malloc(size * sizeof(int));
        createTypeB(&BType, blockWidth);
        createTypeC(&CType, blockHeight, blockWidth);
        fillBArrays(elemsNumB, shiftsB, dims);
        fillCArrays(elemsNumC, shiftsC, blockHeight, dims, size);
    }
    MPI_Comm columns1D;
    MPI_Comm rows1D;
    create1DComms(comm2d, &columns1D, &rows1D);
    double* partA = (double*)malloc(blockHeight * N * sizeof(double));
    double* partB = (double*)malloc(blockWidth * N * sizeof(double));
    double* partC = (double*)malloc(blockWidth * blockHeight * sizeof(double));
    if (coordinatesOfProc[1] == 0) {
        MPI_Scatter(A, blockHeight * N, MPI_DOUBLE, partA, blockHeight * N, MPI_DOUBLE, 0, columns1D);
    }
    if (coordinatesOfProc[0] == 0) {
        MPI_Scatterv(B, elemsNumB, shiftsB, BType, partB, blockWidth * N, MPI_DOUBLE, 0, rows1D);
    }
    MPI_Bcast(partA, blockHeight * N, MPI_DOUBLE, 0, rows1D);
    MPI_Bcast(partB, blockWidth * N, MPI_DOUBLE, 0, columns1D);
    multPartsMatrix(partA, partB, partC, blockHeight, blockWidth);
    MPI_Gatherv(partC, blockWidth * blockHeight, MPI_DOUBLE, C, elemsNumC, shiftsC, CType, 0, comm2d);
    if (rank == 0) {
        free(elemsNumB);
        free(shiftsB);
        free(elemsNumC);
        free(shiftsC);
        MPI_Type_free(&BType);
        MPI_Type_free(&CType);
    }
    free(partA);
    free(partB);
    free(partC);
}

int main(int argc, char *argv[]) {
    int size, rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size); //получение числа процессов
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); //получение номера процесса
    int dims[2] = {0}; 
    int periods[2] = {0};
    int reorder = 0;
    MPI_Comm comm2d;
    MPI_Dims_create(size, 2, dims); //Определили размер решетки

    MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, reorder, &comm2d); //Создали решетку(сздает двумерную сетку процессов)

    double* A = NULL;
    double* B = NULL;
    double* C = NULL;
    if (rank == 0){
        if (argc == 4 && isDigit(argv[1]) && isDigit(argv[2]) && isDigit(argv[3])){
            N = atoi(argv[1]);
            K = atoi(argv[2]);
            M = atoi(argv[3]);
        }
        A = (double*)malloc(M * N * sizeof(double));
        B = (double*)calloc(K * N, sizeof(double));
        C = (double*)malloc(K * M * sizeof(double));
        fillMatrixA(A, M, N);
        fillMatrixB(B, N, K);
    }
    MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&M, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&K, 1, MPI_INT, 0, MPI_COMM_WORLD);
    double startTime = MPI_Wtime();
    matrixMultiply(A, B, C, dims, size, rank, comm2d);
    if (rank == 0){
        double time = MPI_Wtime() - startTime;
        printMatrix(C, M, N);
        printf("Execution time %lf \n", time);
        free(A);
        free(B);
        free(C);
    }
    MPI_Finalize();
    return 0;
}