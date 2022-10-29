#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define SUCCESS 0
#define ERROR (void*)1

#define VALUE_ZERO 0
#define MAX_COUNT_WIDGET 15
#define TRUE 1
#define SLEEP_A 1
#define SLEEP_B 2
#define SLEEP_C 3

typedef struct Semaphores {
    sem_t* sem_a;
    sem_t* sem_b;
    sem_t* sem_c;
    sem_t* sem_widget;
    sem_t* sem_module;
} semaphores_t;

typedef struct Threads {
    pthread_t* producer_a;
    pthread_t* producer_b;
    pthread_t* producer_c;
    pthread_t* producer_module;
    pthread_t* producer_widget;
} threads_t;

int semaphore_wait(sem_t* sem) {
    int error_code = sem_wait(sem);
    if (error_code != SUCCESS) {
        perror("sem_wait");
        return error_code;
    }
    return SUCCESS;
}

int semaphore_post(sem_t* sem) {
    int error_code = sem_post(sem);
    if (error_code != SUCCESS) {
        perror("sem_post");
        return error_code;
    }
    return SUCCESS;
}

void* produce_a(void* param) {
    semaphores_t* sems = (semaphores_t*) param;
    sem_t* sem = sems->sem_a;
    int error_code;
	while (TRUE) {
		sleep(SLEEP_A);
		error_code = semaphore_post(sem);
        if (error_code != SUCCESS) return ERROR;
	}
}

void* produce_b(void* param) {
    semaphores_t* sems = (semaphores_t*) param;
    sem_t* sem = sems->sem_b;
    int error_code;
	while (TRUE) {
		sleep(SLEEP_B);
		error_code = semaphore_post(sem);
        if (error_code != SUCCESS) return ERROR;
	}
}

void* producer_c(void* param) {
    semaphores_t* sems = (semaphores_t*) param;
    sem_t* sem = sems->sem_c;
    int error_code;
	while (TRUE) {
		sleep(SLEEP_C);
		error_code = semaphore_post(sem);
        if (error_code != SUCCESS) return ERROR;
	}
}

void* produce_module(void* param) {
    semaphores_t* sems = (semaphores_t*) param;
    int error_code;
	while (TRUE) {
	 	error_code = semaphore_wait(sems->sem_a);
        if (error_code != SUCCESS) return ERROR;

		error_code = semaphore_wait(sems->sem_b);
        if (error_code != SUCCESS) return ERROR;

		error_code = semaphore_post(sems->sem_module);
        if (error_code != SUCCESS) return ERROR;
	}
}

void* produce_widget(void* param) {
    semaphores_t* sems = (semaphores_t*) param;
    int error_code;
    int i = 1;
	while (TRUE) {
        error_code = semaphore_wait(sems->sem_c);
        if (error_code != SUCCESS) return ERROR;

		error_code = semaphore_wait(sems->sem_module);
        if (error_code != SUCCESS) return ERROR;

		error_code = semaphore_post(sems->sem_widget);
        if (error_code != SUCCESS) return ERROR;
        printf("New widget [%d] produced\n", i++);
        fflush(stdout);
	}
}

int sem_zero_init(void* param) {
    sem_t* sem = (sem_t*) param;
    int error_code = sem_init(sem, VALUE_ZERO, VALUE_ZERO);
    if (error_code != SUCCESS) {
        perror("sem_init");
        return error_code;
    }
    return SUCCESS;
}

int init_sem(semaphores_t* sems) {
    int error_code = sem_zero_init(sems->sem_a);
    if (error_code != SUCCESS) return error_code;

	error_code = sem_zero_init(sems->sem_b);
    if (error_code != SUCCESS) return error_code;

	error_code = sem_zero_init(sems->sem_c);
    if (error_code != SUCCESS) return error_code;

	error_code = sem_zero_init(sems->sem_widget);
    if (error_code != SUCCESS) return error_code;

	error_code = sem_zero_init(sems->sem_module);
    if (error_code != SUCCESS) return error_code;
    return SUCCESS;
}

int pthread_default_create(pthread_t* thread, void *(*fun) (void *), semaphores_t* params) {
    int error_code = pthread_create(thread, NULL, fun, params);
    if (error_code != SUCCESS) {
        perror("pthread_create");
        return error_code;
    }
    return SUCCESS;
}

int create_pthreads(threads_t* threads, semaphores_t* params) {
    int error_code = pthread_default_create(threads->producer_a, produce_a, params);
    if (error_code != SUCCESS) return error_code;

	error_code = pthread_default_create(threads->producer_b, produce_b, params);
    if (error_code != SUCCESS) return error_code;

	error_code = pthread_default_create(threads->producer_c, producer_c, params);
    if (error_code != SUCCESS) return error_code;

	error_code = pthread_default_create(threads->producer_widget, produce_widget, params);
    if (error_code != SUCCESS) return error_code;

	error_code = pthread_default_create(threads->producer_module, produce_module, params);
    if (error_code != SUCCESS) return error_code;

    return SUCCESS;
}

int main () {
    sem_t sem_a;
    sem_t sem_b;
    sem_t sem_c;
    sem_t sem_widget;
    sem_t sem_module;
    semaphores_t semaphores = {&sem_a, &sem_b, &sem_c, &sem_widget, &sem_module};
	int error_code = init_sem(&semaphores);
    if (error_code != SUCCESS) return error_code;

    pthread_t producer_a;
    pthread_t producer_b;
    pthread_t producer_c;
    pthread_t producer_module;
    pthread_t producer_widget;
    threads_t threads = {&producer_a, &producer_b, &producer_c, &producer_module, &producer_widget};
	error_code = create_pthreads(&threads, &semaphores);
    if (error_code != SUCCESS) return error_code;
    sleep(31);
	return SUCCESS;
}