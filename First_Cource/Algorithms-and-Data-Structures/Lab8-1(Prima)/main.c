#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
//#pragma warning(disable : 4996)
#define INF 2147483649

#if 0 // Enable memory allocation/free balance check
//#define _ALLOCATION_LIMIT 20
int __allocs = 0;
static void* my_malloc(size_t size) {
#ifdef _ALLOCATION_LIMIT 
    if (__allocs > _ALLOCATION_LIMIT) return NULL;
#endif
    __allocs++;
    return malloc(size);
}
static void my_free(void* ptr) {
    __allocs--;
    free(ptr);
}
int n_main();
int main() {
    int result = n_main();
    fprintf(stderr, "Allocation balance %d %s\n", __allocs, __allocs ? "- Memory leaks occur!!!" : "is ok");
    return result;
}
#define malloc my_malloc
#define free my_free
#define main n_main
#endif

struct data {
    int N;
    int M;
};
typedef struct data data;

int check_data(data* input) {
    if (!(input->N >= 0 && input->N <= 5000)) {
        printf("bad number of vertices");
        return 1;
    }

    if (!(input->M >= 0 && input->M <= (input->N * (input->N + 1) / 2))) {
        printf("bad number of edges");
        return 1;
    }
    return 0;
}

int chech_edge(int from, int to, int len_edge, data* input) {
    if (from < 0 || from > input->N || to < 0 || to > input->N) {
        printf("bad vertex");
        return 1;
    }
    if (len_edge < 0 || len_edge > INT_MAX) {
        printf("bad length");
        return 1;
    }
    return 0;
}

unsigned int* read_data(FILE* f, data* input) {
    if (!fscanf(f, "%d", &input->N)) return NULL;
    if (!fscanf(f, "%d", &input->M)) return NULL;
    if (check_data(input)) return NULL;
    if (input->N == 1) return NULL;

    if (input->N == 0 || (input->M == 0 && input->N != 1) || input->M < input->N - 1) {
        printf("no spanning tree");
        return NULL;
    }
    unsigned int* graph = (unsigned int*)malloc(sizeof(unsigned int) * input->N * input->N);
    if (graph == NULL) {
        printf("Error allocatian memory!");
        return NULL;
    }
    for (int i = 0; i < input->N; i++)
        for (int j = 0; j < input->N; j++)
            graph[i * input->N + j] = INF;
    int from = 0;
    int to = 0;
    int weight = 0;
    for (int i = 0; i < input->M; i++) {
        if (fscanf(f, "%d %d %d", &from, &to, &weight) == EOF) {
            free(graph);
            printf("bad number of lines");
            return NULL;
        }
        if (chech_edge(from, to, weight, input)) {
            free(graph);
            return NULL;
        }
        from -= 1;
        to -= 1;
        graph[from * input->N + to] = weight;
        graph[to * input->N + from] = weight;


    }
    return graph;
}
void update_length(unsigned int* graph, unsigned int* pathLen, int* parents, int n, int from) {

    for (int to = 0; to < n; ++to)
        if (graph[from * n + to] < pathLen[to] && parents[from] != to) {
            pathLen[to] = graph[from * n + to];
            parents[to] = from;
        }
}

void initialization(char* used, unsigned int* pathLen, int* parents, int n) {
    for (int i = 0; i < n; ++i) {
        used[i] = 0;
        pathLen[i] = INF;
        parents[i] = -1;
    }
    pathLen[1] = 0;
}

void free_prim(char* used, unsigned int* pathLen) {
    free(used);
    free(pathLen);
}
int Prim(unsigned int* graph, int n, int* parents) {
    //parent - хранит один из концов
    int countVertex = 0;
    char* used = (char*)malloc(sizeof(char) * n);
    if (used == NULL) {
        printf("Error memory allocation!");
        return 1;
    }
    unsigned int* pathLen = (unsigned int*)malloc(sizeof(unsigned int) * n); //хранит минимальную длину для parent
    if (pathLen == NULL) {
        printf("Error memory allocation!");
        free(used);
        return 1;
    }
    initialization(used, pathLen, parents, n);

    for (int i = 0; i < n; ++i) {
        int v = -1;
        for (int j = 0; j < n; ++j)
            if (!used[j] && (v == -1 || pathLen[j] < pathLen[v]))
                v = j;

        if (pathLen[v] == INF) {
            printf("no spanning tree");
            free_prim(used, pathLen);
            return 1;
        }

        used[v] = 1;
        countVertex++;
        update_length(graph, pathLen, parents, n, v);
    }
    if (countVertex != n || countVertex == 0) { //не остовное дерево
        printf("no spanning tree");
        free_prim(used, pathLen);
        return 1;
    }
    free_prim(used, pathLen);
    return 0;
}
void cleaning(unsigned int* graph, data* input, FILE* f, int* parents) {
    free(graph);
    free(input);
    free(parents);
    fclose(f);
}

void print_result(int n, int* parents) {
    for (int i = 0; i < n; ++i) {
        if (parents[i] != -1) {
            printf("%d %d\n", parents[i] + 1, i + 1);

        }
    }
}
int main(void) {
    FILE* f = fopen("in.txt", "rt");
    if (f == NULL) {
        printf("Error opening file!");
        return 0;
    }

    data* input = (data*)malloc(sizeof(data));
    if (input == NULL) {
        printf("Error memory allocation!");
        fclose(f);
        return 0;
    }
    unsigned int* graph = NULL;
    if (!(graph = read_data(f, input))) {
        fclose(f);
        free(input);
        return 0;
    }

    int* parents = (int*)malloc(sizeof(int) * input->N);
    if (parents == NULL) {
        printf("Error memory allocation!");
        cleaning(graph, input, f, parents);
        return 0;
    }

    if (!Prim(graph, input->N, parents)) print_result(input->N, parents);

    cleaning(graph, input, f, parents);
    return 0;
}
