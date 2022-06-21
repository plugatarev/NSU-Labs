#include "include.h"
#include "check.h"
int check_data(data* input) {
    if (!(input->N >= 0 && input->N <= 5000)) {
        printf("bad number of vertices");
        return 1;
    }
    if (!(input->M >= 0 && input->M <= input->N * (input->N + 1) / 2)) {
        printf("bad number of edges");
        return 1;
    }
    if (input->S < 1 || input->S > 5000 || input->F < 1 || input->F > 5000) {
        printf("bad vertex");
        return 1;
    }
    input->F--; input->S--;
    return 0;
}

int check_graph(int from, int to, int weight) {
    if (from < 0 || from > 5000 || to < 0 || to > 5000) {
        printf("bad vertex");
        return 1;
    }
    if (weight < 0 || weight > INT_MAX) {
        printf("bad length");
        return 1;
    }
    return 0;
}
