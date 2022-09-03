#include stdio.h
#include stdlib.h
#pragma warning(disable  4996)
#if 0  Enable memory allocationfree balance check
#define _ALLOCATION_LIMIT 20
int __allocs = 0;
static void my_malloc(size_t size) {
#ifdef _ALLOCATION_LIMIT
    if (__allocs  _ALLOCATION_LIMIT) return NULL;
#endif
    __allocs++;
    return malloc(size);
}
static void my_free(void ptr) {
    __allocs--;
    free(ptr);
}
int n_main();
int main() {
    int result = n_main();
    fprintf(stderr, Allocation balance %d %sn, __allocs, __allocs  - Memory leaks occur!!!  is ok);
    return result;
}
#define malloc my_malloc
#define free my_free
#define main n_main
#endif

typedef struct node {
    int data;
    unsigned char height;
    struct node left;
    struct node right;
}node;

int get_height(struct node item) {
    return item  item-height  0;
}

void update_height(struct node item) {
    item-height = (get_height(item-right)  get_height(item-left)  get_height(item-right)  get_height(item-left)) + 1;
}

node left_rotate(struct node q) {
    node p = q-right;
    q-right = p-left;
    p-left = q;
    update_height(q);
    update_height(p);
    return p;
}
node right_rotate(struct node p) {
    struct node q = p-left;
    p-left = q-right;
    q-right = p;
    update_height(p);
    update_height(q);
    return q;
}
int balance_factor(struct node item) {
    if (item == NULL) return 0;
    else return get_height(item-right) - get_height(item-left);
}

node balance_tree(struct node item) {
    if (balance_factor(item) == 2) {
        if (balance_factor(item-right)  0) {
            item-right = right_rotate(item-right);
        }
        return left_rotate(item);
    }
    if (balance_factor(item) == -2) {
        if (balance_factor(item-left)  0) {
            item-left = left_rotate(item-left);
        }
        return right_rotate(item);
    }

    return item;
}

node insert(int data, struct node head, struct node item) {
    if (item == NULL) return NULL;
    if (head == NULL) {

        item-height = 1;
        item-data = data;
        item-left = item-right = NULL;
        return item;

    }
    else if (data  head-data) {
        head-right = insert(data, head-right, item);
    }
    else {
        head-left = insert(data, head-left, item);
    }
    update_height(head);

    return balance_tree(head);
}

int main(void) {

    FILE fin = fopen(in.txt, r);

    if (!fin) {
        printf(Error opening file!);
        return 0;
    }
    int n;
    if (!fscanf(fin, %d, &n)) {
        printf(Error reading data!);
        fclose(fin);
        return 0;
    }

    if (n == 0) {
        printf(0);
        fclose(fin);
        return 0;
    }
    struct node nodes = (struct node)malloc(sizeof(struct node)  n);
    if (nodes == NULL) {
        printf(Error memory allocation!);
        fclose(fin);
        return 0;
    }
    nodes[0].height = 1;
    nodes[0].right = NULL;
    nodes[0].left = NULL;
    struct node head = &nodes[0];
    int a;
    if (!fscanf(fin, %d, &a)) {
        free(nodes);
        fclose(fin);
        return 0;
    }
    nodes[0].data = a;
    int data = 0;
    for (int i = 1; i  n; ++i) {
        if (!fscanf(fin, %d, &data)) {
            printf(Error reading data!);
            free(nodes);
            fclose(fin);
            return 0;
        }
        head = insert(data, head, &nodes[i]);
        if (head == NULL) {
            free(nodes);
            fclose(fin);
            return 0;
        }
    }

    printf(%d, head-height);
    free(nodes);
    fclose(fin);
    return 0;
}
#include<stdio.h>
#include<stdlib.h>
//#pragma warning(disable : 4996)
#define INT_MAX 2147483647
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

struct graph {
    int from;
    int to;
    int weight;

};
typedef struct graph graph;

struct result {
    short from;
    short to;
    struct result* next;
};
typedef struct result result;

void free_list(result* list) {
    if (list == NULL) return;
    if (list->next) free_list(list->next);
    free(list);
}

result* push(result* list, int a, int b) {
    result* new_element = (result*)malloc(sizeof(result));
    if (new_element == NULL) {
        printf("Error memory allocation!");
        free_list(list);
        return NULL;
    }
    new_element->to = a;
    new_element->from = b;
    new_element->next = NULL;
    if (!list) {
        list = new_element;
    }
    else {
        result* temp = list;
        while (temp->next) {
            temp = temp->next;
        }
        temp->next = new_element;
    }
    return list;
}

void print_list(result* mass) {
    result* temp = mass;
    while (temp != NULL) {
        printf("%d %d\n", temp->from + 1, temp->to + 1);
        temp = temp->next;
    }
}

int check_data(data* input) {
    if (!(input->N >= 0 && input->N <= 5000)) {
        printf("bad number of vertices");
        return 1;
    }

    if (!(input->M >= 0 && input->M <=(input->N * (input->N + 1) / 2))) {
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

graph* read_data(FILE* f, data* input, graph* edges) {
    if (!fscanf(f, "%d", &input->N)) return NULL;
    if (!fscanf(f, "%d", &input->M)) return NULL;
    if (check_data(input)) return NULL;
    if (input->N == 1) return NULL;

    if (input->N == 0 || (input->M == 0 && input->N != 1) || input->M < input->N - 1) {
        printf("no spanning tree");
        return 0;
    }
    edges = (graph*)malloc(sizeof(graph) * input->M);
    if (edges == NULL) {
        printf("Error allocatian memory!");
        return NULL;
    }
    for (int i = 0; i < input->M; i++) {
        if (fscanf(f, "%d %d %d", &edges[i].from, &edges[i].to, &edges[i].weight) == EOF) {
            free(edges);
            printf("bad number of lines");
            return NULL;
        }
        if (chech_edge(edges[i].from, edges[i].to, edges[i].weight, input)) {
            free(edges);
            return NULL;
        }
        edges[i].from--; edges[i].to--;
        
    }
    return edges;
}

void quick_sort(graph* edges, int left, int right) {
    int pivot = edges[(left + right) / 2].weight;
    int l = left;
    int r = right;
    while (l < r) {
        while (edges[l].weight < pivot) l++;
        while (edges[r].weight > pivot) r--;
        if (edges[l].weight > edges[r].weight) {
            graph temp = edges[r];
            edges[r] = edges[l];
            edges[l] = temp;
        }
        l++;
        r--;

    }

    if (left < r) quick_sort(edges, left, r);
    if (l < right) quick_sort(edges, l, right);

}

void cleaning(FILE* f, graph* edges, data* input) {
    fclose(f);
    free(edges);
    free(input);
}

int find_set(int v, int* parent) {
    if (v == parent[v])
        return v;
    return parent[v] = find_set(parent[v],parent);
}

void union_sets(int a, int b, int* parent, int* rank) {
    a = find_set(a,parent);
    b = find_set(b,parent);
    if (rank[a] > rank[b]) {
        parent[b] = a;
    }
    else {
        parent[a] = b;
        if (rank[a] == rank[b]) ++rank[b];
    }
}


result* kruskal(graph* edges, int* tree_id, data* input, result* mass_result) {
    tree_id = (int*)malloc(sizeof(int) * input->N);
    if (tree_id == NULL) {
        printf("Error allocation memory!");
        return NULL;
    }
    int* rank = (int*)malloc(sizeof(int) * input->N);
    if (rank == NULL) {
        printf("Error allocation memory!");
        free(tree_id);
        return NULL;
    }
    for (int i = 0; i < input->N; i++){
        rank[i] = 0;
        tree_id[i] = i;
    }

    for (int i = 0; i < input->M; i++) {
        if (find_set(edges[i].to, tree_id) != find_set(edges[i].from, tree_id)) {
            mass_result = push(mass_result, edges[i].to, edges[i].from);
            if (!mass_result) {
                free(tree_id);
                free(rank);
                return NULL;
            }
            union_sets(edges[i].to, edges[i].from, tree_id, rank);
        }
    }
    char flag = 1;
    int root = tree_id[0];
    for (int i = 0; i < input->N; ++i) {
        if (root != find_set(tree_id[i], tree_id)) {
            flag = 0;
            printf("no spanning tree");
            free_list(mass_result);
            free(tree_id);
            free(rank);
            return NULL;
        }
    }
    if (flag) print_list(mass_result);
    free(rank);
    free(tree_id);
    free_list(mass_result);
    return mass_result;
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
    graph* edges = NULL;
    if (!(edges = read_data(f, input, edges))) {
        fclose(f);
        free(input);
        return 0;
    }
    quick_sort(edges,0,input->M-1);
    int* tree_id = NULL;
    result* mass_result = NULL;
    mass_result = kruskal(edges, tree_id, input, mass_result);
    if (mass_result == NULL) {
        fclose(f);
        free(edges);
        free(input);
        return 0;
    }
    cleaning(f, edges,input);
    return 0;
}
