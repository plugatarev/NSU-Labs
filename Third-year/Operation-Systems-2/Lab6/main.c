#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#define SUCCESS 0
#define COEFFICIENT 100000
#define MAX_NUM_LINES 100
#define TRUE 1
#define ERROR (void*)1
#define BUFFER_SIZE 256
#define END_OF_FILE (void*)2
#define EXIT_ERROR 1

void* sort(void* param) {
    if (param == NULL) {
        printf("Unexpected argument value: NULL");
        return NULL;
    }

    char* line = (char*) param;
    size_t length = strlen(line);
    int errorCode = usleep(length * COEFFICIENT);
    if (errorCode != SUCCESS) {
        perror("usleep");
        return ERROR;
    }

    printf("%s", line);
    return NULL;
}

char* readLine() {
    char* line = NULL;
    unsigned int length = 0;

    while (TRUE) {
        char* ptr = (char*)realloc(line, (length + BUFFER_SIZE) * sizeof(char));
        if (ptr == NULL) {
            perror("realloc");
            return ERROR;
        }
        line = ptr;

        char* result = fgets(line + length, BUFFER_SIZE, stdin);
        if (result == NULL) {
            if (feof(stdin)) return END_OF_FILE;
            perror("fgets");
            return ERROR;
        }

        length += strlen(result);
        if (line[length - 1] == '\n') {
            break;
        }
    }

    return line;
}

void freeLines(char** lines, int numberLines) {
    for (int i = 0; i < numberLines; i++) {
        if (lines[i] != NULL) free(lines[i]);
    }
}

int main() {
    int numberLines = 0;
    char* lines[MAX_NUM_LINES];
    do {
        lines[numberLines] = readLine();
        if (lines[numberLines] == ERROR) {
            freeLines(lines, numberLines);
            return EXIT_ERROR;
        }
        if (lines[numberLines] == END_OF_FILE) {
            break;
        }
        numberLines++;
    }while (numberLines < MAX_NUM_LINES);

    printf("----- SORTING -----\n");
    pthread_t threads[numberLines];
    int errorCode;
    for (int i = 0; i < numberLines; i++) {
        errorCode = pthread_create(&threads[i], NULL, sort, lines[i]);
        if (errorCode != SUCCESS) {
            perror("pthread_create");
            freeLines(lines, numberLines);
        }
    }

    for (int i = 0; i < numberLines; i++) {
        errorCode = pthread_join(threads[i], NULL);
        if (errorCode != SUCCESS) {
            perror("pthread_join");
            freeLines(lines, numberLines);
            return errorCode;
        }
    }
    freeLines(lines, numberLines);
    return SUCCESS;
}
