#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

#define MAX_STRING_SIZE 80
#define LINE_SEPARATOR "---------------------------------------\n"
#define TRUE 1
#define FALSE 0
#define SLEEP_TIME 5

#define SUCCESS 0
#define SUCCESS_PTR (void*)0
#define ERROR 1
#define ERROR_PTR (void*)1

typedef struct Node {
    char* str;
    struct Node* next;
} node_t;

typedef struct Context {
    pthread_mutex_t* mutex;
    node_t* head_list;
} context_t;

int len(node_t* head) {
    node_t* tmp = head ;
    int i = 0 ;
    while(tmp != NULL) {
        i++;
        tmp = tmp->next;
    }
    return i ;
}

node_t* add_node(node_t* head, char* string) {
    node_t* new_node = (node_t*)malloc(sizeof(node_t));
    if (new_node == NULL) {
        perror("malloc");
        return ERROR_PTR;
    }
    new_node->str = (char*)calloc(MAX_STRING_SIZE + 1, sizeof(char));
    if (new_node == NULL) {
        perror("calloc");
        return ERROR_PTR;
    }
    strcpy(new_node->str, string);
    new_node->next = head;
    return new_node;
}

void print_list(node_t* head) {
    node_t* cur = head;
    while (cur != NULL){
        printf("%s\n", cur->str);
        cur = cur->next;
    }
}

node_t* swap(node_t* ptr1, node_t* ptr2) {
    node_t* tmp = ptr2->next;
    ptr2->next = ptr1;
    ptr1->next = tmp;
    return ptr2;
}

void sort(node_t** head) {
    if (*head == NULL) return;
    node_t* left;
    node_t* prev = NULL;
    node_t* tmp;
    int is_sort;
    do {
        left = *head;
        is_sort = FALSE;
        prev = NULL;
        while (left->next != NULL) {
            int is_swap = FALSE;
            if (strncmp(left->str, left->next->str, MAX_STRING_SIZE) > 0) {
                is_swap = TRUE;
                is_sort = TRUE;
                if (prev == NULL) {
                    *head = swap(left, left->next);
                    tmp = *head;
                }
                else {
                    prev->next = swap(left, left->next);
                    tmp = prev->next;
                }
            }
            prev = is_swap ? tmp : left;
            left = prev->next;
        }
    } while (is_sort);
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

void* sort_list(void* param) {
    if (param == NULL) {
        printf("Unexpected argument value: NULL");
        return NULL;
    }
    context_t* context = (context_t*) param;

    while (TRUE) {
        sleep(SLEEP_TIME);
        int error_code = lock_mutex(context->mutex);
        if (error_code == ERROR) return ERROR_PTR;

        sort(&context->head_list);

        error_code = unlock_mutex(context->mutex);
        if (error_code == ERROR) return ERROR_PTR;
    }
}

int equal(char* s1, char* s2) {
    if (strcmp(s1, s2) == 0) return TRUE;
    return FALSE;
}

void free_list(node_t* head_list) {
    node_t* tmp;
    while (head_list != NULL) {
        tmp = head_list;
        head_list = head_list->next;
        free(tmp->str);
        free(tmp);
    }
}

int main() {
    pthread_mutex_t mutex;
    int error_code = pthread_mutex_init(&mutex, NULL);
    if (error_code != SUCCESS) {
        perror("pthread_mutex_init");
        return error_code;
    }
    context_t context = {&mutex, NULL};
    pthread_t thread;
    error_code = pthread_create(&thread, NULL, sort_list, (void*)&context);
    if (error_code != SUCCESS) {
        perror("pthread_create");
        return SUCCESS;
    }
    char string[MAX_STRING_SIZE + 1] = {0};
    while (TRUE) {
        char* result = fgets(string, MAX_STRING_SIZE + 1, stdin);
        if (result == NULL) {
            if (feof(stdin)) break;
            perror("fgets");
            return ERROR;
        }
        int len = strlen(string);
        if (len != 1 && len != MAX_STRING_SIZE) string[len - 1] = '\0';
        
        error_code = lock_mutex(&mutex);
        if (error_code == ERROR) return ERROR;
        if (equal("\n", string)) {
            printf(LINE_SEPARATOR);
            print_list(context.head_list);
            printf(LINE_SEPARATOR);
        }
        else {
            context.head_list = add_node(context.head_list, string);
        }
        error_code = unlock_mutex(&mutex);
        if (error_code == ERROR) return ERROR;
    }
    free_list(context.head_list);
    return SUCCESS;
}