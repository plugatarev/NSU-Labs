#include<stdio.h>
#include<stdlib.h>
//#pragma warning(disable : 4996)

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

void add_edge(int start, int end, char* graph, int n) {
	graph[start * n + end] = 1;
}
 //1 - связь есть || 2 - обработали вершину || 3 - добавили в out
int dfs (int n, char* graph, unsigned short* out, int start,int* pos, char* used){
	if (used[start] == 2) return 0;
	if (used[start] == 1) {
		printf("impossible to sort");
		return 1;
	}

	used[start] = 1;
	for (int i = n-1; i >= 0; i--) {
		if (graph[start * n + i] == 1) {
			if (dfs(n, graph, out, i, pos,used)) return 1;
		}
	}
	out[*pos] = start;
	used[start] = 2;
	(*pos)++;
	return 0;
}

/*void print_graph(char* graph, int n) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			printf("%d    ",graph[i * n + j]);
		}
		printf("\n");
	}
}*/

int main() {

	FILE* fin = fopen("in.txt", "r");
	if (!fin) {
		printf("Error opening file!");
		return 0;
	}
	int n;
	if (fscanf(fin, "%d", &n) == EOF) {
		printf("bad number of lines");
		fclose(fin);
		return 0;
	}
	int m;
	if (fscanf(fin, "%d", &m) == EOF) {
		printf("bad number of lines");
		fclose(fin);
		return 0;
	}

	if (n < 0 || n > 2000) {
		printf("bad number of vertices");
		fclose(fin);
		return 0;
	}
	if (m < 0 || m >(n * (n + 1) / 2)) {
		printf("bad number of edges");
		fclose(fin);
		return 0;
	}
	char* graph = (char*)(malloc(sizeof(char) * n * n));
	if (graph == NULL) {
		printf("Error memory allocation!");
		fclose(fin);
		return 0;
	}

	int start, end;
	for (int i = 0; i < m; i++) {
		if (fscanf(fin, "%d %d", &start, &end) == EOF) {
			printf("bad number of lines");
			free(graph);
			fclose(fin);
			return 0;
		}
		if (start < 0 || start > n || end < 0 || end > n) {
			printf("bad vertex");
			free(graph);
			fclose(fin);
			return 0;
		}
		add_edge(start - 1, end - 1, graph, n);
	}
	//print_graph(graph, n);

	unsigned short* out = (unsigned short*)malloc(sizeof(unsigned short) * n);
	if (out == NULL) {
		printf("Error memory allocation!");
		fclose(fin);
		free(graph);
		return 0;
	}
	char* used = (char*)malloc(sizeof(char) * n);
	if (used == NULL) {
		printf("Error memory allocation!");
		fclose(fin);
		free(graph);
		free(out);
		return 0;
	}
	int pos = 0;
	for (int i = 0; i < n; i++) {
		if (dfs(n, graph, out, i, &pos, used)) {
			fclose(fin);
			free(graph);
			free(used);
			free(out);
			return 0;
		}
	}
	for (int i = n - 1; i >= 0; i--) printf("%d ", out[i] + 1);

	fclose(fin);
	free(used);
	free(graph);
	free(out);
	return 0;
}
