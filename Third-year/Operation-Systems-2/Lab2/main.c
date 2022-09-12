#include <stdio.h>
#include <pthread.h>
#include <string.h>

#define SUCCESS 0
#define LINES_COUNT 10
#define CHILD_MESSAGE "child"
#define PARENT_MESSAGE "parent"
#define EXIT_ERROR 1
#define ERROR (void*)1

void* printLines(void* param){
    if (param == NULL) {
        printf("Unexpected argument value: NULL");
        return ERROR;
    }

    char* line = (char*) param;

    for (int i = 0; i < LINES_COUNT; i++) {
        printf("%s\n", line);
    }

    return NULL;
}

int main() {
    pthread_t thread;
    int errorCode = pthread_create(&thread, NULL, printLines, CHILD_MESSAGE);
    if (errorCode != SUCCESS){
        perror("pthread_create");
    }

    errorCode = pthread_join(thread, NULL);
    if (errorCode != SUCCESS) {
        perror("pthread_join");
        return errorCode;
    }

    void* error = printLines(PARENT_MESSAGE);
    if (error == ERROR) return EXIT_ERROR;

    return SUCCESS;
}