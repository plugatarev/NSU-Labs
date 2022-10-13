#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <semaphore.h>

#define SUCCESS 0
#define ERROR 1
#define ERROR_CODE (void*)3
#define LINES_COUNT 5
#define CHILD_MESSAGE "child\n"
#define PARENT_MESSAGE "parent\n"
#define THREADS_ONLY 0
#define VALUE_ONE 1
#define VALUE_ZERO 0

typedef struct Semaphores {
    sem_t* semaphore_1;
    sem_t* semaphore_2;
} Semaphores;

void* child_print(void* param) {
    if (param == NULL) {
        printf("Unexpected argument value: NULL");
        return NULL;
    }
    Semaphores* sems = (Semaphores*)param;

    for (int i = 0; i < LINES_COUNT; i++){
        int error_code = sem_wait(sems->semaphore_1);
        if (error_code != SUCCESS) {
            perror("sem_wait");
            return ERROR_CODE;
        }
        printf(CHILD_MESSAGE);
        error_code = sem_post(sems->semaphore_2);
        if (error_code != SUCCESS) {
            perror("sem_post");
            return ERROR_CODE;
        }
    }
    pthread_exit(NULL);
}

int parent_print(Semaphores* sems) {
    for (int i = 0; i < LINES_COUNT; i++){
        int error_code = sem_wait(sems->semaphore_2);
        if (error_code != SUCCESS) {
            perror("sem_wait");
            return error_code;
        }
        printf(PARENT_MESSAGE);
        error_code = sem_post(sems->semaphore_1);
        if (error_code != SUCCESS) {
            perror("sem_post");
            return error_code;
        }
    }
    return SUCCESS;
}

int main() {
    pthread_t thread;
    sem_t semaphore_1;
    sem_t semaphore_2;
    int error_code = sem_init(&semaphore_1, THREADS_ONLY, VALUE_ZERO);
    if (error_code != SUCCESS) {
        perror("sem_init 1");
        return error_code;
    }
    error_code = sem_init(&semaphore_2, THREADS_ONLY, VALUE_ONE);
    if (error_code != SUCCESS) {
        perror("sem_init 2");
        return error_code;
    }
    Semaphores sems = {&semaphore_1, &semaphore_2};
    error_code = pthread_create(&thread, NULL, child_print, (void*)(&sems));
    if (error_code != SUCCESS){
        perror("pthread_create");
        return error_code;
    }

    error_code = parent_print(&sems);
    if (error_code != SUCCESS) {
        return error_code;
    }
    pthread_exit(NULL);
}