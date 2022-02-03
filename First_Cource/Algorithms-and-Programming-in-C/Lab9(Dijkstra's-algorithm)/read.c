#include "include.h"
#include "read.h"
#include "check.h"
#include "head_functions.h"
unsigned int* read_data(data* input) {
    FILE* fin = fopen("in.txt", "rt");
    if (fin == NULL) {
        printf("Error opening file!");
        return NULL;
    }
    if (!fscanf(fin, "%d", &input->N)) {
        printf("bad number of lines");
        fclose(fin);
        return NULL;
    }
    if (!fscanf(fin, "%d %d", &input->S, &input->F)) {
        printf("bad number of lines");
        fclose(fin);
        return NULL;
    }
    if (!fscanf(fin, "%d", &input->M)) {
        printf("bad number of lines");
        fclose(fin);
        return NULL;
    }
    if (check_data(input)) {
        fclose(fin);
        return NULL;
    }
    unsigned int* graph_matrix = (unsigned int*)malloc(sizeof(unsigned int) * input->N * input->N);
    if (graph_matrix == NULL) {
        printf("Error allocation memory!");
        fclose(fin);
        return NULL;
    }
    int from = 0, to = 0, weight = 0;
    for (int i = 0; i < input->N; i++)
        for (int j = 0; j < input->N; j++)
            graph_matrix[i * input->N + j] = INF;

    for (int i = 0; i < input->M; i++) {
        if (!fscanf(fin, "%d %d %d", &from, &to, &weight)) {
            printf("bad number of lines");
            fclose(fin);
            free(graph_matrix);
            return NULL;
        }
        from--;
        to--;
        if (check_graph(from, to, weight)) {
            fclose(fin);
            free(graph_matrix);
            return NULL;
        }
        graph_matrix[from * input->N + to] = weight;
        graph_matrix[to * input->N + from] = weight;
    }
    fclose(fin);
    return graph_matrix;
}
