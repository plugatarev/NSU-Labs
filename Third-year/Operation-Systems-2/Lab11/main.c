#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#define SUCCESS 0
#define ERROR 1
#define LINES_COUNT 10
#define CHILD_MESSAGE "child\n"
#define PARENT_MESSAGE "parent\n"
#define MUTEX_COUNT 3
#define FIRST_MUTEX 0
#define SECOND_MUTEX 1
#define THIRD_MUTEX 2
#define SLEEP_TIME 2

int lock_mutex(pthread_mutex_t* mutex) {
    if (mutex == NULL) {
        printf("lock_mutex: mutex was NULL\n");
        return ERROR;
    }
    int error_code = pthread_mutex_lock(mutex);
    if (error_code != SUCCESS) {
        perror("lock_mutex");
        return ERROR;
    }
    return SUCCESS;
}

int unlock_mutex(pthread_mutex_t* mutex) {
    if (mutex == NULL) {
        printf("unlock_mutex: mutex was NULL\n");
        return ERROR;
    }
    int error_code = pthread_mutex_unlock(mutex);
    if (error_code != SUCCESS) {
        perror("unlock_mutex");
        return ERROR;
    }
    return SUCCESS;
}

//2 1 -> 3 -> 2 -> 1
void* child_print(void* arg) {
    pthread_mutex_t* mutexes = (pthread_mutex_t*) arg;
    lock_mutex(&mutexes[SECOND_MUTEX]);
    for (int i = 0; i < LINES_COUNT; i++) {
        lock_mutex(&mutexes[FIRST_MUTEX]);
        printf(CHILD_MESSAGE);
        unlock_mutex(&mutexes[SECOND_MUTEX]);
        lock_mutex(&mutexes[THIRD_MUTEX]);
        unlock_mutex(&mutexes[FIRST_MUTEX]);
        lock_mutex(&mutexes[SECOND_MUTEX]);
        unlock_mutex(&mutexes[THIRD_MUTEX]);
    }
    unlock_mutex(&mutexes[SECOND_MUTEX]);
    pthread_exit(arg);
}

//1 -> 3 -> 2 -> 1
void parent_print(pthread_mutex_t* mutexes) {
    for (int i = 0; i < LINES_COUNT; i++) {
        printf(PARENT_MESSAGE);
        lock_mutex(&mutexes[THIRD_MUTEX]);
        unlock_mutex(&mutexes[FIRST_MUTEX]);
        lock_mutex(&mutexes[SECOND_MUTEX]);
        unlock_mutex(&mutexes[THIRD_MUTEX]);
        lock_mutex(&mutexes[FIRST_MUTEX]);
        unlock_mutex(&mutexes[SECOND_MUTEX]);
    }
    unlock_mutex(&mutexes[FIRST_MUTEX]);
}

int init_mutexes(pthread_mutex_t* mutexes) {
    pthread_mutexattr_t mutex_attr;
    int error_code = pthread_mutexattr_init(&mutex_attr);
    if (error_code != SUCCESS) {
        perror("pthread_mutexattr_init");
        return ERROR;
    }
    error_code = pthread_mutexattr_settype(&mutex_attr, PTHREAD_MUTEX_ERRORCHECK);
    if (error_code != SUCCESS) {
        perror("pthread_mutexattr_settype");
        error_code = pthread_mutexattr_destroy(&mutex_attr);
        if (error_code != SUCCESS) {
            perror("pthread_mutexattr_destroy");
            return error_code;
        }
        return error_code;
    }
    for (int i = 0; i < MUTEX_COUNT; i++){
        error_code = pthread_mutex_init(&mutexes[i], &mutex_attr);
        if (error_code != SUCCESS) {
            perror("pthread_mutex_init");
            return error_code;
        }
    }
    return SUCCESS;
}

int destroy_mutexes(pthread_mutex_t* mutexes) {
    for (int i = 0; i < MUTEX_COUNT; i++) {
        int error_code = pthread_mutex_destroy(&mutexes[i]);
        if (error_code != SUCCESS) {
            perror("pthread_mutex_destroy");
            return error_code;
        }
    }
    return SUCCESS;
}

int main() {
    pthread_mutex_t mutexes[MUTEX_COUNT];
    int error_code = init_mutexes(mutexes);
    if (error_code != SUCCESS) return error_code;

    lock_mutex(&mutexes[FIRST_MUTEX]);

    pthread_t thread;
    error_code = pthread_create(&thread, NULL, child_print, (void*) mutexes);
    if (error_code != SUCCESS) {
        perror("pthread_create");
        return error_code;
    }
    sleep(SLEEP_TIME);
    parent_print(mutexes);

    error_code = pthread_join(thread, NULL);
    if (error_code != SUCCESS) {
        printf("pthread_join");
        return error_code;
    }

    error_code = destroy_mutexes(mutexes);
    if (error_code != SUCCESS) return error_code;

    return SUCCESS;
}