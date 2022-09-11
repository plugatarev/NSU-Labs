#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

#define SUCCESS 0
#define SLEEP_TIME_SEC 2
#define TRUE 1

void* printLine(void* param){
    while (TRUE) {
        printf("Hello\n");
        pthread_testcancel();
    }
    return NULL;
}

int main() {
    pthread_t thread;
    int errorCode = pthread_create(&thread, NULL, printLine, NULL);
    if (errorCode != SUCCESS) {
        perror("pthread_create");
        return errorCode;
    }

    int timeLeft = SLEEP_TIME_SEC;
    do {
        timeLeft = sleep(timeLeft);
    } while (timeLeft > 0);

    errorCode = pthread_cancel(thread);
    if (errorCode != SUCCESS) {
        perror("pthread_cancel");
        return errorCode;
    }

    errorCode = pthread_join(thread, NULL);
    if (errorCode != SUCCESS) {
        perror("pthread_join");
        return errorCode;
    }

    return SUCCESS;
}