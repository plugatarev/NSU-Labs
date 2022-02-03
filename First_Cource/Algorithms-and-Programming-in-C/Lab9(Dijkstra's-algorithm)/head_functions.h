#ifndef HEAD
#define HEAD
#include "include.h"
void cleaning(data* input, unsigned int* graph_matrix);
unsigned int* dijkstra(data* input, unsigned int* graph, int* p, int* count);
void print_result(unsigned int* result, int size);
void print_s_to_f(data* input, unsigned int* d, int* parent, int* count);
unsigned int shortestWay(unsigned int first, unsigned int second, int third);
int min_lenght(data* input, unsigned int* d, char* used);
#endif
