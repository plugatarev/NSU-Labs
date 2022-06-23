#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#define WAIT_ERROR -1
#define FORK_ERROR -1
#define ERROR 1
#define SUCCESS 0
#define CHILD 0

int main() {
    int pid = fork();
    if (pid == FORK_ERROR) {
        perror("fork");
        return ERROR;
    }
    if (pid == CHILD) {
        execlp("cat", "cat", "text.txt", NULL);
        perror("execlp: cat");
        return ERROR;
    }
    int wait_error = wait(NULL);
    if (wait_error == WAIT_ERROR){
        perror("wait");
        return ERROR;
    }
    printf("Hello, World!\n");    
    return SUCCESS;
}