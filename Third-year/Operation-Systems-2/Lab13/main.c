#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

#define SUCCESS 0
#define ERROR 1
#define ERROR_CODE (void*)3
#define TRUE 1
#define FALSE 0
#define LINES_COUNT 5
#define CHILD_MESSAGE "child\n"
#define PARENT_MESSAGE "parent\n"

typedef struct Context {
    pthread_mutex_t* mutex;
    pthread_cond_t* conditional_variable;
    int* is_parent;
} Context;

void* child_print(void* param) {
    if (param == NULL) {
        printf("Unexpected argument value: NULL");
        return NULL;
    }
    Context* context = (Context*) param;
    for (int i = 0; i < LINES_COUNT; i++){
        int error_code = pthread_mutex_lock(context->mutex);
        if (error_code != SUCCESS) {
            perror("pthread_mutex_lock");
            return ERROR_CODE;
        }
        while (*(context->is_parent)){
            error_code = pthread_cond_wait(context->conditional_variable, context->mutex);
            if (error_code != SUCCESS) {
                perror("pthread_cond_wait");
                return ERROR_CODE;
            }
        }
        *(context->is_parent) = TRUE;
        printf(CHILD_MESSAGE);
        error_code = pthread_cond_signal(context->conditional_variable);
        if (error_code != SUCCESS) {
            perror("pthread_cond_signal");
            return ERROR_CODE;
        }
        error_code = pthread_mutex_unlock(context->mutex);
        if (error_code != SUCCESS) {
            perror("pthread_mutex_unlock");
            return ERROR_CODE;
        }
    }
    pthread_exit(NULL);
}

int parent_print(Context* context) {
    for (int i = 0; i < LINES_COUNT; i++) {
        int error_code = pthread_mutex_lock(context->mutex);
        if (error_code != SUCCESS) {
            perror("pthread_mutex_lock");
            return error_code;
        }
        while (!(*context->is_parent)) {
            error_code = pthread_cond_wait(context->conditional_variable, context->mutex);
            if (error_code != SUCCESS) {
                perror("pthread_cond_wait");
                return error_code;
            }
        }
        *(context->is_parent) = FALSE;
        printf(PARENT_MESSAGE);
        error_code = pthread_cond_signal(context->conditional_variable);
        if (error_code != SUCCESS) {
            perror("pthread_cond_signal");
            return error_code;
        }
        error_code = pthread_mutex_unlock(context->mutex);
        if (error_code != SUCCESS) {
            perror("pthread_mutex_unlock");
            return error_code;
        }
    }
    return SUCCESS;
}

int main() {
    pthread_t thread;
    pthread_mutex_t mutex;
    pthread_cond_t conditional_variable;
    int is_parent = TRUE;
    int error_code = pthread_mutex_init(&mutex, NULL);
    if (error_code != SUCCESS) {
        perror("pthread_mutex_init");
        return error_code;
    }
    error_code = pthread_cond_init(&conditional_variable, NULL);
    if (error_code != SUCCESS) {
        perror("pthread_cond_init");
        return error_code;
    }
    Context context = {&mutex, &conditional_variable, &is_parent};

    error_code = pthread_create(&thread, NULL, child_print, (void*)(&context));
    if (error_code != SUCCESS) {
        perror("pthread_create");
        return error_code;
    }

    error_code = parent_print(&context);
    if (error_code != SUCCESS) {
        return error_code;
    }
    pthread_exit(NULL);
}