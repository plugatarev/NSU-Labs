#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

#define SUCCESS 0
#define ERROR 1
#define ERROR_PTR (void*)1

#define MAX_STRING_SIZE 80
#define TRUE 1
#define FALSE 0
#define SLEEP_TIME 5
#define LINE_SEPARATOR "---------------------------------------\n"

typedef struct Node {
    char* str;
    struct Node* next;
    pthread_mutex_t* mutex;
} node_t;

node_t *head;
pthread_mutex_t head_m;

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

int equal(char* s1, char* s2) {
    if (strcmp(s1, s2) == 0) return TRUE;
    return FALSE;
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

node_t* add_node(char* string) {
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
    pthread_mutex_t* mutex = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
    int error_code = pthread_mutex_init(mutex, NULL);
    if (error_code != SUCCESS) {
        perror("pthread_mutex_init");
        return ERROR_PTR;
    }
    new_node->mutex = mutex;
    strcpy(new_node->str, string);
    if (head != NULL) lock_mutex(head->mutex);
    new_node->next = head;
    if (head != NULL) unlock_mutex(head->mutex);
    return new_node;
}

void print_list() {
    node_t* cur = head;
    lock_mutex(head->mutex);
    while (cur != NULL){
        printf("%s\n", cur->str);
        unlock_mutex(cur->mutex);
        if (cur->next != NULL) lock_mutex(cur->next->mutex);
        cur = cur->next;
    }
}

void free_list() {
    node_t* tmp;
    while (head != NULL) {
        tmp = head;
        head = head->next;
        free(tmp->str);
        free(tmp);
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

    int is_sort = FALSE;
    node_t* tmp;
    do {
        node_t* left = *head;
        is_sort = FALSE;
        node_t* prev = NULL;
        while (left->next != NULL) {
            int is_swap = FALSE;

            //lock_free (CAS) livelock не все задачи решаются
            //mutex плохо потому что надо продумывать работу с мьютексами для потоков (нет блокировки - нет дедлоков)
            //переключения контекста
            pthread_mutex_t pm;
            if (prev != NULL) pm = *(prev->mutex);
            pthread_mutex_t lm = *(left->mutex);
            pthread_mutex_t lnm = *(left->next->mutex);
            if (prev != NULL) lock_mutex(&pm);
            lock_mutex(&lm);
            lock_mutex(&lnm);

            if (strncmp(left->str, left->next->str, MAX_STRING_SIZE) > 0) {
                is_swap = TRUE;
                is_sort = TRUE;
                if (prev == NULL) {
                    lock_mutex(&head_m);
                    *head = swap(left, left->next);
                    unlock_mutex(&head_m);
                    tmp = *head;
                }
                else {
                    prev->next = swap(left, left->next);
                    tmp = prev->next;
                }
            }

            unlock_mutex(&lnm);
            unlock_mutex(&lm);
            if (prev != NULL) unlock_mutex(&pm);

            prev = is_swap ? tmp : left;
            left = prev->next;

        }
    } while (is_sort);
}

void* sort_list(void* param) {
    while (TRUE) {
        sleep(SLEEP_TIME);
        sort(&head);
    }
}

int create_sorting_thread() {
    pthread_t thread;
    int error_code = pthread_create(&thread, NULL, sort_list, NULL);
    if (error_code != SUCCESS) {
        perror("pthread_create");
        return error_code;
    }
    return SUCCESS;
}

int main() {
    pthread_mutex_t mutex;
    int error_code = pthread_mutex_init(&mutex, NULL);
    if (error_code != SUCCESS) {
        perror("pthread_mutex_init");
        return error_code;
    }
    error_code = pthread_mutex_init(&head_m, NULL);
    if (error_code != SUCCESS) {
        perror("pthread_mutex_init");
        return error_code;
    }
    head = NULL;
    create_sorting_thread();

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

        if (equal("\n", string)) {
            printf(LINE_SEPARATOR);
            print_list();
            printf(LINE_SEPARATOR);
        }
        else {
            lock_mutex(&head_m);
            head = add_node(string);
            unlock_mutex(&head_m);
        }
    }
    free_list();
    return SUCCESS;
}