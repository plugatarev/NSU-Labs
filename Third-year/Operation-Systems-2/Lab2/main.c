#include <stdio.h>
#include <pthread.h>
#include <string.h>

#define SUCCESS 0
#define LINES_COUNT 10
#define CHILD_MESSAGE "child"
#define PARENT_MESSAGE "parent"

void* printLines(void* param){
    if (param == NULL) {
        printf("Unexpected argument value: NULL");
        return NULL;
    }

    char* line = (char*) param;

    for (int i = 0; i < LINES_COUNT; i++) {
        printf("%s\n", line);
    }

    return NULL;
}

int main(int argc, char** argv) {
    pthread_t thread;
    int error = pthread_create(&thread, NULL, printLines, CHILD_MESSAGE);
    if (error != SUCCESS){
        perror("pthread_create");
    }

    pthread_join(thread, NULL);

    printLines(PARENT_MESSAGE);

    pthread_exit(NULL);
}