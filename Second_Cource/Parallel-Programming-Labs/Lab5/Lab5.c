#include <pthread.h>
#include <stdio.h>
#include <math.h>
#include <mpi.h>
#include <time.h>
#include <stdlib.h>

#define ERROR 1
#define SUCCESS 0
#define NUMBER_OF_TRHEAD 2
#define NUMBER_OF_TASKS 40
#define TRUE 1
#define SENDER 0
#define WORKER 1
#define FINISH_COMMUNICATION 0 
#define END_OF_TASKS -1
#define CONNECT 1

typedef struct utils{
    int size;
    int rank;
    int numberOfTaskOnprocess;
    int* tasks;
}utils;

pthread_t threads[NUMBER_OF_TRHEAD];
pthread_mutex_t mutex;
int currentTask = 0;

void* sendTasks(void* params) {
    utils args = *((utils*) params);
    MPI_Status status;
    int request;
    int answer;

    while(TRUE){
        //Ждем информации от других процессов о том, что они готовы принимать задачи на исполнение 
        MPI_Recv(&request, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
        if (request == END_OF_TASKS) break;
        pthread_mutex_lock(&mutex);
        if (currentTask == args.numberOfTaskOnprocess){
            answer = END_OF_TASKS;
        }
        else{
            answer = args.tasks[currentTask];
            currentTask++;  
        }
        pthread_mutex_unlock(&mutex);
        MPI_Send(&answer, 1, MPI_INT, status.MPI_SOURCE, 1, MPI_COMM_WORLD);
    }
    printf("Process %d finished sending tasks.\n", args.rank);
}

void* doTasks(void* params) {
    utils args = *((utils*) params);
    while(currentTask < args.numberOfTaskOnprocess) {
        //Получаем задачу
        pthread_mutex_lock(&mutex);
        int time = args.tasks[currentTask];
        currentTask++; 
        pthread_mutex_unlock(&mutex);
        //Выполняем задачу
        sleep(time);
        printf("Process %d did task. Time = %d\n", args.rank, time);
    }
    //Принимаем задачи от всех процессов, если текущий процесс уже выполнил свои задачи
    for (int i = 0; i < args.size; i++) {
        if (args.rank == (i + args.rank) % args.size) continue;
        int request = CONNECT;
        while (TRUE) {
            MPI_Send(&request, 1, MPI_INT, (i + args.rank) % args.size, 0, MPI_COMM_WORLD);
            int answer;
            //Принимаем задачу
            MPI_Recv(&answer, 1, MPI_INT, (i + args.rank) % args.size, 1, MPI_COMM_WORLD, 0);
            //Если задач на исполнение нет, то завершаем работу цикла
            if (answer == END_OF_TASKS) break;
            //Выполняем задачу
            sleep(answer);
            printf("Process %d did task of process %d. Time = %d\n", args.rank, (i + args.rank) % args.size, answer);
        }
    }
    MPI_Barrier(MPI_COMM_WORLD);
    int request = END_OF_TASKS;
    MPI_Send(&request, 1, MPI_INT, args.rank, 0, MPI_COMM_WORLD);
    printf("Process %d finished tasks.\n", args.rank);
}

void createThreads(int size, int rank, int numberOfTaskOnprocess, int* tasks){
    pthread_attr_t attrs;
    pthread_attr_init(&attrs);
    //Устанавливаем в аттрибуте свойство "присоединяемости"(joinable) для потоков
    pthread_attr_setdetachstate(&attrs, PTHREAD_CREATE_JOINABLE);
    utils args = {size, rank, numberOfTaskOnprocess, tasks};
    pthread_create(&threads[WORKER], &attrs, &doTasks, &args);
    pthread_create(&threads[SENDER], &attrs, &sendTasks, &args);
    //Освобождаем ресурсы атрибута
    pthread_attr_destroy(&attrs);
    
    //Поток, вызвавший эту функцию, останавливается, пока не окончится выполнение потока thread. 
    pthread_join(threads[WORKER], NULL);
    pthread_join(threads[SENDER], NULL);
}

int getNumberOfTaskOnprocess(int size, int rank){
    int tmp = NUMBER_OF_TASKS % size;
    return NUMBER_OF_TASKS / size + (rank < tmp);
}

int* getTask(int numberOfTaskOnprocess, int rank){
    int* tasks = (int*)malloc(sizeof(int) * numberOfTaskOnprocess); 
    for(int i = 0; i < numberOfTaskOnprocess; i++){
        // tasks[i] = 1 + rand() % 3;
        tasks[i] = rank + 1;
    }
    return tasks;
}

int main(int argc, char** argv){
    srand(time(NULL));
    int provided = 0;
    int size, rank;
    MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &provided);
    if (provided != MPI_THREAD_MULTIPLE){
        perror("level supported don't match");
        return ERROR;
    }
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    pthread_mutex_init(&mutex, NULL);
    
    int numberOfTaskOnprocess = getNumberOfTaskOnprocess(size, rank);
    int* tasks = getTask(numberOfTaskOnprocess, rank);
    double time = MPI_Wtime();
    createThreads(size, rank, numberOfTaskOnprocess, tasks);
    time = MPI_Wtime() - time;
    double min, max;
    MPI_Reduce(&time, &max, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    MPI_Reduce(&time, &min, 1, MPI_DOUBLE, MPI_MIN, 0, MPI_COMM_WORLD);
    if(rank == 0){
        printf("Tasks = %d\nMin Time = %lf\tMax Time = %lf\n", NUMBER_OF_TASKS, min, max);
    }
    free(tasks);
    MPI_Finalize();
    return SUCCESS;
}