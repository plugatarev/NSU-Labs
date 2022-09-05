#include <stdio.h>
#include <pthread.h>
#include <string.h>

#define SUCCESS 0
#define LINES_COUNT 10

void* printLines(void* param){
    if (param == NULL) {
        printf("Unexpected argument value: NULL");
        return NULL;
    }
    for (int i = 0; i < LINES_COUNT; i++) {
        printf("hello child %d\n", i);
    }
    return NULL;
}

int main(int argc, char** argv) {
    pthread_t thread;
    int error = pthread_create(&thread, NULL, threadFunction, NULL);
    if (error != SUCCESS){
        perror("pthread_create");
    }

    for (int i = 0; i < 10; i++) {
        printf("hello parent %d\n", i);
    }

    pthread_exit(NULL);
    return SUCCESS;
}