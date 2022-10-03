#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#define STEPS_NUMBER 2000000000
#define SUCCESS 0
#define ERROR -1
#define COMMAND_ARGUMENT_COUNT 2
#define THREADS_MIN 1

typedef struct Limit {
    long long int start;
    long long int end;
    double partPI;
} Limit;

void* calculatePartPI(void* args){
    if (args == NULL){
        printf("Unexpected argument value: NULL");
        return NULL;
    }

    Limit* limitsSerial = (Limit*) args;
    long long int start = limitsSerial->start;
    long long int end = limitsSerial->end;
    
    double localPI = 0;
    for (int i = start; i < end ; i++) {
        localPI += 1.0 / (i*4.0 + 1.0);
        localPI -= 1.0 / (i*4.0 + 3.0);
    }

    limitsSerial->partPI = localPI;
    return (void*)(&limitsSerial->partPI);
}

int createThreadsCalculatingPI(int threadsNumber, Limit* limitsSerial, pthread_t* threads) {
    for (int i = 0; i < threadsNumber; i++) {
        int errorCode = pthread_create(&threads[i], NULL, calculatePartPI, &(limitsSerial[i]));
        if (errorCode != SUCCESS) {
            perror("pthread_create");
            return errorCode;
        }
    }
    return SUCCESS;
}

long readThreadsNumber(int argc, char** argv) {
    if (argc != COMMAND_ARGUMENT_COUNT){
        printf("Usage: <program> <threads_number>\n");
        return ERROR;
    }

    long threadsNumber = atol(argv[1]);
    if (threadsNumber < THREADS_MIN) {
        printf("Invalid threads number. Minimal number: %d", THREADS_MIN);
        return ERROR;
    }

    return threadsNumber;
}

void fillLimits(Limit* limits, int threadsNumber) {
    int mainTasks = STEPS_NUMBER / threadsNumber;
    int ostTasks = STEPS_NUMBER % threadsNumber;
    int current = 0;
    for (int i = 0; i < threadsNumber; i++) {
        int count = mainTasks + (i < ostTasks);
        limits[i].start = current;
        limits[i].end = current + count - 1;
        current+=count;
    }
}

int main(int argc, char** argv) {
    long threadsNumber = readThreadsNumber(argc, argv);
    if (threadsNumber == ERROR) return ERROR;
    pthread_t threads[threadsNumber];

    Limit limits[threadsNumber];
    fillLimits(limits, threadsNumber);

    int errorCode = createThreadsCalculatingPI(threadsNumber, limits, threads);
    if (errorCode != SUCCESS) return errorCode;
    double pi = 0;
    
    for (int i = 0; i < threadsNumber; i++) {
        void* partPI;
        errorCode = pthread_join(threads[i], &partPI);
        if (errorCode != SUCCESS) {
            printf("pthread_join");
            return errorCode;
        }
        pi = pi + *((double*) partPI);
    }

    pi = pi * 4.0;
    printf("pi done - %.15g \n", pi);
    return SUCCESS;
}
