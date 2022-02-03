#include "include.h"
#include "read.h"
#include "check.h"
#include "head_functions.h"

int main(void) {

    data* input = (data*)malloc(sizeof(data));
    if (input == NULL) {
        printf("Error memory allocation!");
        return 0;
    }
    unsigned int* graph_matrix = read_data(input);
    if (graph_matrix == NULL) {
        free(input);
        return 0;
    }
    int* p = (int*)malloc(input->N * sizeof(int));
    if (p == NULL) {
        printf("Error memory allocation!");
        cleaning(input, graph_matrix);
        return 0;
    }
    for (int i = 0; i < input->N; i++) p[i] = -1;
    int count = 0;
    unsigned int* head = dijkstra(input, graph_matrix, p, &count);
    if (head) {
        print_result(head, input->N);
        print_s_to_f(input, head, p, &count);
        free(head);
    }
    free(p);
    cleaning(input, graph_matrix);
    return 0;
}
