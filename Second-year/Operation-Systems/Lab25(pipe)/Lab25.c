#include <unistd.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#define DESCRIPTORS_NUMBER 2
#define PIPE_ERROR -1
#define FORK_ERROR -1
#define READ_ERROR -1
#define WRITE_ERROR -1
#define END_OF_FILE 0
#define ERROR 1
#define SUCCESS 0
#define FIRST 0
#define SECOND 1
#define TRUE 1
#define CHILD 0
#define MESSAGE_SIZE 256

int write_upper_message(char message[], int message_size){
    for (int i = 0; i < message_size; i++){
        message[i] = toupper(message[i]);
    }
    int write_result = write(STDOUT_FILENO, message, message_size);
    if (write_result == WRITE_ERROR){
        perror("writing to standart output");
        return ERROR;
    }
    return SUCCESS;
}

int close_file_descriptor(int file_descriptor){
    int close_result = close(file_descriptor);
    if (close_result == EOF){
        perror("closing file descriptor");
        return ERROR;
    }
    return SUCCESS;
}

int close_both_file_descriptors(int file_descriptors[]){
    int close_result = close_file_descriptor(file_descriptors[FIRST]);
    if (close_result == ERROR) return ERROR;

    close_result = close_file_descriptor(file_descriptors[SECOND]);
    if (close_result == ERROR) return ERROR;
    return SUCCESS;
}

int create_child_process(){
    int pid = fork();
    if (pid == FORK_ERROR){
        perror("fork");
        return ERROR;
    }
    return pid;
}

int writer_process(int file_descriptor[DESCRIPTORS_NUMBER]){
    int pid = create_child_process();
    if (pid == FORK_ERROR) return ERROR;
    if (pid != CHILD) return pid;

    int close_error = close_file_descriptor(file_descriptor[FIRST]);
    if (close_error == ERROR) return ERROR;

    while(TRUE){
        char message[MESSAGE_SIZE];
        int read_sesult = read(STDIN_FILENO, message, MESSAGE_SIZE);
        if (read_sesult == READ_ERROR){
            perror("read");
            return ERROR;
        }
        int write_error = write(file_descriptor[SECOND], message, read_sesult);
        if (write_error == WRITE_ERROR){
            perror("writing to pipe");
            return ERROR;
        }
        if (read_sesult == END_OF_FILE) break;
    }
    return pid;
}

int reader_process(int file_descriptor[DESCRIPTORS_NUMBER]){
    int pid = create_child_process();
    if (pid == FORK_ERROR) return ERROR;

    if (pid != CHILD){
        int error = close_both_file_descriptors(file_descriptor);
        if (error == ERROR) return ERROR;
    }
    int error = close_file_descriptor(file_descriptor[SECOND]);
    if (error == ERROR) return ERROR;
    char message[MESSAGE_SIZE];
    while (TRUE){
        int read_result = read(file_descriptor[FIRST], message, MESSAGE_SIZE);
        if (read_result == READ_ERROR){
            perror("reading from pipe");
            return ERROR;
        }
        int write_upper_result = write_upper_message(message, read_result);
        if (write_upper_result == ERROR) return ERROR; 
        if (read_result == END_OF_FILE) break; 
    }   
    
    return SUCCESS;
}

int main(){
    int file_descriptor[DESCRIPTORS_NUMBER];  
    int pipe_result = pipe(file_descriptor);
    if (pipe_result == PIPE_ERROR){
        perror("create pipe");
        return ERROR;
    }
    int pid = writer_process(file_descriptor);
    if (pid == ERROR) return ERROR;
    if (pid > CHILD){
        pid = reader_process(file_descriptor);
        if (pid == ERROR) return ERROR;
    }
    return SUCCESS;
}
