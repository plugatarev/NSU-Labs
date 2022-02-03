#include "include.h"
#include "head_functions.h"

void cleaning(data* input, unsigned int* graph_matrix) {
    free(graph_matrix);
    free(input);
}

void print_result(unsigned int* result, int size) {
    for (int i = 0; i < size; i++) {
        if (result[i] == INF_2) printf("oo ");
        else if (result[i] <= INT_MAX) printf("%u ", result[i]);
        else printf("INT_MAX+ ");
    }
    printf("\n");
}

void print_s_to_f(data* input, unsigned int* d, int* parent, int* count) {
    if (d[input->F] == INF_2) printf("no path");
    else if (d[input->F] >= INT_MAX && (*count) >= 2) printf("overflow");
    else {
        if (input->S == input->F) printf("%d ", input->S + 1);
        while (input->F != input->S) {
            printf("%d ", input->F + 1);
            input->F = parent[input->F];
        }
        printf("%d\n", input->S + 1);
    }
}


int min_lenght(data* input, unsigned int* d, char* used) {
    unsigned int min = INF;
    int minIndex = 0;
    for (int i = 0; i < input->N; ++i)
        if (d[i] <= min && !used[i]) {
            min = d[i];
            minIndex = i;
        }
    return minIndex;

}
unsigned int shortest_way(unsigned int first, unsigned int second, unsigned int third) {
    unsigned int b;
    if ((second + third) > INT_MAX) b = INF;
    else b = second + third;
    if (first < b) return first;
    else return b;
}


unsigned int* dijkstra(data* input, unsigned int* graph, int* p, int* count) {
    unsigned int* d = (unsigned int*)malloc(sizeof(unsigned int) * input->N);
    if (d == NULL) {
        printf("Error memory allocation!");
        return NULL;
    }
    for (int i = 0; i < input->N; i++) d[i] = INF_2;

    char* used = (char*)calloc(input->N, sizeof(char));
    if (used == NULL) {
        printf("Error memory allocation!");
        free(d);
        return NULL;
    }

    
    int cur_vertex = input->S;
    d[cur_vertex] = 0;

    for (int i = 0; i < input->N; i++) {
        
        cur_vertex = min_lenght(input, d, used);
        used[cur_vertex] = 1;

        for (int u = 0; u < input->N; ++u) {
            if (graph[cur_vertex * input->N + u] != INF && !used[u]) {
                unsigned int oldLength = d[u];
                d[u] = shortest_way(d[u], d[cur_vertex], graph[cur_vertex * input->N + u]);
                if (d[u] > INT_MAX)
                    if (u == input->F) (*count)++;
                if (d[u] < oldLength) {
                    p[u] = cur_vertex;
                }
            }
        }

    }
    free(used);
    return d;
}
