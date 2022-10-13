#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>
#include <fcntl.h>

#define LINES_COUNT 5
#define ERROR -1
#define CHILD 0
#define PARENT 1
#define SUCCESS 0
#define VALUE_ONE 1
#define VALUE_ZERO 0
#define FILE_PERMISSIONS 0600
#define CHILD_MESSAGE "child\n"
#define PARENT_MESSAGE "parent\n"

int child_print(sem_t* semaphore_1, sem_t* semaphore_2) {
    for (int i = 0; i < LINES_COUNT; i++){
        int error_code = sem_wait(semaphore_1);
        if (error_code != SUCCESS) {
            perror("sem_wait");
            return ERROR;
        }
        printf(CHILD_MESSAGE);
        error_code = sem_post(semaphore_2);
        if (error_code != SUCCESS) {
            perror("sem_post");
            return ERROR;
        }
    }
    return SUCCESS;
}

int parent_print(sem_t* semaphore_1, sem_t* semaphore_2) {
    for (int i = 0; i < LINES_COUNT; i++){
        int error_code = sem_wait(semaphore_2);
        if (error_code == ERROR) {
            perror("sem_wait");
            return ERROR;
        }
        printf(PARENT_MESSAGE);
        error_code = sem_post(semaphore_1);
        if (error_code == ERROR) {
            perror("sem_post");
            return ERROR;
        }
    }
    return SUCCESS;
}

int semathores_close(sem_t* semaphore_1, sem_t* semaphore_2) {
    int error_code = sem_close(semaphore_1);
    if (error_code == ERROR) {
        perror("sem_close 1");
        return ERROR;
    }
    error_code = sem_close(semaphore_2);
    if (error_code == ERROR) {
        perror("sem_close 2");
        return ERROR;
    }
    return SUCCESS;
}

int main() {
    sem_t* semaphore_1 = sem_open("sem1", O_CREAT, FILE_PERMISSIONS, VALUE_ZERO);
    if (semaphore_1 == SEM_FAILED) {
        perror("sem_open 1");
        return ERROR;
    }
    sem_t* semaphore_2 = sem_open("sem2", O_CREAT, FILE_PERMISSIONS, VALUE_ONE);
    if (semaphore_2 == SEM_FAILED) {
        perror("sem_open 2");
        return ERROR;
    }
    pid_t pid = fork();
    if (pid == ERROR) {
        perror("fork");
        return ERROR;
    }
    if (pid == CHILD) {
        int error_code = child_print(semaphore_1, semaphore_2);
        if (error_code != SUCCESS) return error_code;
    }
    else {
        int error_code = parent_print(semaphore_1, semaphore_2);
        if (error_code != SUCCESS) return error_code;
        error_code = semathores_close(semaphore_1, semaphore_2);
        if (error_code == ERROR) return error_code;
    }
    return SUCCESS;
}