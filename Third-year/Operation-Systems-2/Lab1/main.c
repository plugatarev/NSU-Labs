#include <stdio.h>
#include <pthread.h>
#include <string.h>

#define SUCCESS 0

void* threadFunction(void* param){
    for (int i = 0; i < 10; i++) {
        printf("hello child %d\n", i);
    }
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
}