#include <stdio.h>
#include <pthread.h>
#include <string.h>

#define SUCCESS 0
#define THREADS_NUMBER 4
#define STRINGS_NUMBER 3

void* printLines(void* param){
    if (param == NULL) {
        printf("Unexpected argument value: NULL");
        return NULL;
    }

    char** stringArray = (char**) param;
    for (int i = 0; i < STRINGS_NUMBER; i++) {
        printf("%s", stringArray[i]);
    }
    
    return NULL;
}

int main() {
    pthread_t threads[THREADS_NUMBER];
    char* stringArrays[THREADS_NUMBER][STRINGS_NUMBER] = { {"1 ", "2 ", "3\n"}, 
                                                           {"4 ", "5 ", "6\n"}, 
                                                           {"9 ", "10 ", "11\n"}, 
                                                           {"12 ", "13 ", "14\n"} 
                                                          };
    
    int errorCode;
    for (int i = 0; i < THREADS_NUMBER; i++) {
        errorCode = pthread_create(&threads[i], NULL, printLines, &stringArrays[i]);
        if (errorCode != SUCCESS) {
            perror("pthread_create");
            return errorCode;
        }
    }

    for (int i = 0; i < THREADS_NUMBER; i++) {
        errorCode = pthread_join(threads[i], NULL);
        if (errorCode != SUCCESS) {
            perror("pthread_join");
            return errorCode;
        }
    }

    return SUCCESS;
}