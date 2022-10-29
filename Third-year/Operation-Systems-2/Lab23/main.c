#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SUCCESS 0
#define ERROR 1
#define ERROR_PTR (void*)1

#define LINE_SEPARATOR "---------------------------------------\n"
#define COEFFICIENT 100000
#define MAX_NUM_LINES 100
#define TRUE 1
#define BUFFER_SIZE 256
#define END_OF_FILE (void*)2

typedef struct Node {
    char* str;
    struct Node* next;
} node_t;

typedef struct Context {
    char* string;
    pthread_mutex_t* mutex;
} context_t;

node_t* head_list;

node_t* add_node(char* string) {
    node_t* new_node = (node_t*)malloc(sizeof(node_t));
    if (new_node == NULL) {
        perror("malloc");
        return ERROR_PTR;
    }
    new_node->str = string;
    new_node->next = head_list;
    return new_node;
}

node_t* reverse_list() {
    node_t* result = NULL;
    while (head_list) {
        node_t* next = head_list->next;
        head_list->next = result;
        result = head_list;
        head_list = next;
    }
    return result;
}

void print_list() {
    node_t* cur = head_list;
    while (cur != NULL){
        printf("%s", cur->str);
        cur = cur->next;
    }
}

void list_free() {
    node_t* tmp;
    while (head_list != NULL) {
        tmp = head_list;
        head_list = head_list->next;
        free(tmp->str);
        free(tmp);
    }
}

int lock_mutex(pthread_mutex_t* mutex) {
    if (mutex == NULL) {
        printf("lock_mutex: mutex was NULL\n");
        return ERROR;
    }
    int error_code = pthread_mutex_lock(mutex);
    if (error_code != SUCCESS) {
        perror("lock_mutex");
        return ERROR;
    }
    return SUCCESS;
}

int unlock_mutex(pthread_mutex_t* mutex) {
    if (mutex == NULL) {
        printf("unlock_mutex: mutex was NULL\n");
        return ERROR;
    }
    int error_code = pthread_mutex_unlock(mutex);
    if (error_code != SUCCESS) {
        perror("unlock_mutex");
        return ERROR;
    }
    return SUCCESS;
}

void* sort(void* param) {
    if (param == NULL) {
        printf("Unexpected argument value: NULL");
        return ERROR_PTR;
    }

    context_t* context = (context_t*) param;
    usleep(strlen(context->string) * COEFFICIENT);

    int error_code = lock_mutex(context->mutex);
    if (error_code == ERROR) return ERROR_PTR;
    head_list = add_node(context->string);

    error_code = unlock_mutex(context->mutex);
    if (error_code == ERROR) return ERROR_PTR;

    return head_list;
}

void free_lines(char** lines, int numberLines) {
    for (int i = 0; i < numberLines; i++) {
        if (lines[i] != NULL) free(lines[i]);
    }
}

char* read_line() {
    char* line = NULL;
    unsigned int length = 0;

    while (TRUE) {
        char* pointer = (char*)realloc(line, (length + BUFFER_SIZE) * sizeof(char));
        if (pointer == NULL) {
            perror("realloc");
            return ERROR_PTR;
        }
        line = pointer;

        char* result = fgets(line + length, BUFFER_SIZE, stdin);
        if (result == NULL) {
            if (feof(stdin)) return END_OF_FILE;
            perror("fgets");
            return ERROR_PTR;
        }

        length += strlen(result);
        if (line[length - 1] == '\n') {
            break;
        }
    }
    return line;
}

int read_lines(char** lines, int* numberLines) {
    do {
        lines[*numberLines] = read_line();
        if (lines[*numberLines] == ERROR_PTR) {
            free_lines(lines, *numberLines);
            return ERROR;
        }
        if (lines[*numberLines] == END_OF_FILE) {
            break;
        }
        (*numberLines)++;
    } while (*numberLines < MAX_NUM_LINES);
    return SUCCESS;
}

int main() {
    int number_lines = 0;
    char* lines[MAX_NUM_LINES];
    int error_code = read_lines(lines, &number_lines);
    if (error_code == ERROR) return error_code;
    pthread_mutex_t mutex;
    error_code = pthread_mutex_init(&mutex, NULL);
    if (error_code != SUCCESS) {
        perror("pthread_mutex_init");
        return error_code;
    }
    pthread_t threads[number_lines];
    context_t contexts[number_lines];
    for (int i = 0; i < number_lines; i++) {
        contexts[i].string = lines[i];
        contexts[i].mutex = &mutex;
        error_code = pthread_create(&threads[i], NULL, sort, (void*)&contexts[i]);
        if (error_code != SUCCESS) {
            printf("pthread_create");
            free_lines(lines, number_lines);
        }
    }
    for (int i = 0; i < number_lines; i++) {
        error_code = pthread_join(threads[i], NULL);
        if (error_code != SUCCESS) {
            printf("pthread_join");
            list_free();
            return error_code;
        }
    }
    error_code = pthread_mutex_destroy(&mutex);
    if (error_code != SUCCESS) {
        perror("pthread_mutex_destroy");
        list_free();
        return ERROR;
    }

    printf(LINE_SEPARATOR);
    head_list = reverse_list();
    print_list();
    list_free();
    return SUCCESS;
}